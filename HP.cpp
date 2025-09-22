#include "HP.h"
#include "Engine/CsvReader.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"
#include "Player.h"
#include "Goal.h"
#include "Stage.h"
#include "Timer.h"

void HP::ChangeState()
{
	if (currentPercentage_ <= redLine_)
	{
		currentState_ = RED;
	}
	else if (currentPercentage_ <= yellowLine_)
	{
		currentState_ = YELLOW;
	}
	else
	{
		currentState_ = GREEN;
	}
}

HP::HP(GameObject* parent)
	:GameObject(parent, "HP"), player_(nullptr), isDamageTime_(nullptr), hBehindHP_(NULL_NUM), hHPFrame_(NULL_NUM), hPoisonFrame_(NULL_NUM),
	maxHP_(0), yellowLine_(0), redLine_(0), hpBarImageSize_(0,0), hpBarSize_(0,0), vibDecreaseSpeed_(0.0f),isDamageInterval_(0.0f),
	hpFrameDist_(0.0f, 0.0f),poisonEfPos_(0.0f, 0.0f, 0.0f),hpBarPos_(0.0f, 0.0f, 0.0f),hpFramePos_(0.0f, 0.0f, 0.0f), hpFrameScale_(0.0f, 0.0f, 0.0f),
	currentPercentage_(0.0f), angle_(0.0f), cutNum_(0.0f),vibrationRange_(0.0f,0.0f), center_(0.0f, 0.0f), isDamage_(true), currentState_(GREEN)
{
	for (int h = 0; h < HP_MAX; h++)
	{
		hHPBars_[h] = NULL_NUM;
	}
}

void HP::Initialize()
{
	//csvからデータ読み込み
	CsvReader csv;//データを読むクラスのインスタンスを作成
	bool ret = csv.Load("CsvData\\Chara\\HPData.csv");
	assert(ret);
	int row = 1, column = 0;

	//ハンドルにデータをセット
	for (int h = 0; h < HP_MAX; h++)
	{
		hHPBars_[h] = Image::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
		assert(hHPBars_[h] > NULL_NUM);
	}
	hBehindHP_ = Image::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hBehindHP_ > NULL_NUM);
	hHPFrame_ = Image::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hHPFrame_ > NULL_NUM);
	hPoisonFrame_ = Image::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hPoisonFrame_ > NULL_NUM);

	maxHP_ = csv.extractInt(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	currentHP_ = maxHP_;
	yellowLine_ = csv.extractInt(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	redLine_ = csv.extractInt(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	hpBarImageSize_ = csv.extractInt2(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	hpBarSize_ = { hpBarImageSize_.first / 2,hpBarImageSize_.second };
	vibrationSpeed_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	vibDecreaseSpeed_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	isDamageInterval_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	poisonEfPos_ = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	hpBarPos_ = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	hpFramePos_ = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	hpFrameDist_ = { hpFramePos_.x - hpBarPos_.x ,hpFramePos_.y - hpBarPos_.y };
	hpFrameScale_ = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column)));

	player_ = GetParent()->FindGameObject<Player>();
	isDamageTime_ = Instantiate<Timer>(this);

	poisonEf_ = VFX::SetEffect("HPPoisonEffectData.csv");

	transform_.position_ = hpBarPos_;
	center_ = { hpBarPos_.x,hpBarPos_.y };
	frameTrans_.position_ = hpFramePos_;
	frameTrans_.scale_ = hpFrameScale_;
}

void HP::Update()
{
	if (isDamage_ == false)
	{
		if (isDamageTime_->TimeElapsed(isDamageInterval_))
		{
			isDamage_ = true;
		}
	}

	//currentHp_が減ってたらその分HP画像を切り抜く
	currentPercentage_ = ((currentHP_ / maxHP_) * MAX_PERCENTAGE);//現在currentHP_はMAX_HPの何パーセントか
	float DecreasePer = (MAX_PERCENTAGE - currentPercentage_) / MAX_PERCENTAGE;//残りの割合から減らすパーセントを計算
	cutNum_ = hpBarSize_.first * DecreasePer;

	if (currentState_ == HP_POISON)
	{
		if (vibrationRange_.y <= 0) 
		{
			ChangeState();
		}
	}
	else
	{
		ChangeState();
	}

	if (vibrationRange_.x > 0)
	{
		transform_.position_.x = center_.x + cos(angle_) * vibrationRange_.x;
		frameTrans_.position_.x = (center_.x + cos(angle_) * vibrationRange_.x) + hpFrameDist_.x;
		angle_ += vibrationSpeed_;
		vibrationRange_.x -= vibDecreaseSpeed_;
	}

	if (vibrationRange_.y > 0)
	{
		transform_.position_.y = center_.y + sin(angle_) * vibrationRange_.y;
		frameTrans_.position_.y = (center_.y + sin(angle_) * vibrationRange_.y) + hpFrameDist_.y;
		angle_ += vibrationSpeed_;
		vibrationRange_.y -= vibDecreaseSpeed_;
	}

	if (currentHP_ <= HP_DEAD_LINE)
	{
		Stage* stage = GetParent()->FindGameObject<Stage>();
		Goal* goal = (Goal*)(stage->FindGameObject<Goal>());
		goal->EndEf();
		player_->GameOver();
	}
}

void HP::Draw()
{
	Image::SetTransform(hBehindHP_, transform_);
	Image::Draw(hBehindHP_);

	//HPバーを描画
	Image::SetRect(hHPBars_[currentState_], cutNum_, 0, hpBarImageSize_.first - (cutNum_ + cutNum_), hpBarImageSize_.second);
	Image::SetTransform(hHPBars_[currentState_], transform_);
	Image::Draw(hHPBars_[currentState_]);

	//HPフレームを描画
	Image::SetTransform(hHPFrame_, frameTrans_);
	Image::Draw(hHPFrame_);
	if (currentState_ == HP_POISON)
	{
		Image::SetTransform(hPoisonFrame_, frameTrans_);
		Image::Draw(hPoisonFrame_);
	}
}

void HP::PlayDamageEffect(const HPSTATE& hpState)
{
	if (hpState == HP_POISON)
	{
		poisonEf_.position.x = player_->GetPosition().x + poisonEfPos_.x;
		poisonEf_.position.y = player_->GetPosition().y + poisonEfPos_.y;
		poisonEf_.position.z = player_->GetPosition().z + poisonEfPos_.z;
		VFX::Start(poisonEf_);
	}
}
