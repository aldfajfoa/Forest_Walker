#include "Slash.h"
#include "Engine/Input.h"
#include "Engine/Audio.h"
#include "Engine/CsvReader.h"
#include "Enemys.h"
#include "Player.h"
#include "PlayScene.h"
#include "Timer.h"

Slash::Slash(GameObject* parent)
	:GameObject(parent, "Slash"), boxColl_(nullptr), player_(nullptr), slashIntervalTime_(nullptr), hSlashHitSound_(NULL_NUM), 
	attack_(0),rotateYRight_(0),rotateYLeft_(0),slashInterval_(0.0f),slashDist_({ 0,0,0 }),slashShineDist_({ 0,0,0 }),slashBoxSize_({ 0,0,0 }),
	isSlash_(true),isSlashCollision_(true)
{
}

void Slash::Initialize()
{
	//csvからデータ読み込み
	CsvReader csv;//データを読むクラスのインスタンスを作成
	bool ret = csv.Load("CsvData\\SlashData.csv");
	assert(ret);
	int row = 1, column = 0;

	//ハンドルにデータをセット
	hSlashHitSound_ = Audio::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hSlashHitSound_ > NULL_NUM);

	attack_ = csv.extractInt(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	rotateYRight_ = csv.extractInt(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	rotateYLeft_ = csv.extractInt(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	slashInterval_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	slashDist_ = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	slashShineDist_ = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	slashBoxSize_ = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column)));

	player_ = GetParent()->FindGameObject<Player>();
	boxColl_ = new BoxCollider(player_->GetPosition(), slashBoxSize_);
	AddCollider(boxColl_);

	slashIntervalTime_ = Instantiate<Timer>(this);

	slashEf_= VFX::SetEffect("SlashEffectData.csv");
	shineEf_= VFX::SetEffect("SlashShineEffectData.csv");
	slashHitEf_ = VFX::SetEffect("SlashHitEffectData.csv");
}

void Slash::Update()
{
	XMFLOAT3 pPos = player_->GetPosition();

	if (player_->GetisRight_())
	{
		transform_.position_ = { (float)(pPos.x + slashDist_.x),pPos.y,pPos.z };
	}
	else
	{
		transform_.position_ = { (float)(pPos.x - slashDist_.x),pPos.y,pPos.z };
	}

	if ((Input::IsKeyDown(DIK_G) || Input::IsPadButtonDown(XINPUT_GAMEPAD_Y)) && isSlash_)
	{
		isSlash_ = false;
		isSlashCollision_ = true;

		if (player_->GetisRight_())
		{
			slashEf_.position = { (float)(pPos.x + slashDist_.x),pPos.y,slashDist_.z };
			slashEf_.rotate.y = rotateYRight_;
			shineEf_.position = { (float)(pPos.x + slashShineDist_.x),pPos.y,pPos.z };
		}
		else
		{
			slashEf_.position = { (float)(pPos.x - slashDist_.x),pPos.y,slashDist_.z };
			slashEf_.rotate.y = rotateYLeft_;
			shineEf_.position = { (float)(pPos.x - slashShineDist_.x),pPos.y,pPos.z };
		}
		VFX::Start(slashEf_);
		VFX::Start(shineEf_);
	}

	if(isSlash_ == false)
	{
		if (slashIntervalTime_->TimeElapsed(slashInterval_))
		{
			isSlash_ = true;
		}
	}

	Collision();
}

void Slash::Collision()
{
	PlayScene* playscene = (PlayScene*)GetParent();
	Enemys* eColl = playscene->GetPlayScene_enemys();

	if (isSlashCollision_)
	{
		//エフェクトが出てる間だけ当たり判定
		for (int e = 0; e < eColl->GetEnemys().size(); e++)
		{
			//敵の体力が残ってるかつ既に攻撃を受けていないか
			if (eColl->GetEnemys()[e]->GetCurrentHP() > HP_DEAD_LINE && eColl->GetEnemys()[e]->GetisAffectedSlash())
			{
				if (boxColl_->IsHit(eColl->GetEnemys()[e]->GetAttackedColl())) {
					//位置を設定してエフェクト再生
					slashHitEf_.position = transform_.position_;
					VFX::Start(slashHitEf_);
					Audio::Play(hSlashHitSound_);//音再生
					eColl->GetEnemys()[e]->SetisAffectedSlash(false);
					eColl->GetEnemys()[e]->SetisBlinking(true);
					eColl->GetEnemys()[e]->DecreaseHP(attack_);
				}
			}
		}

		isSlashCollision_ = false;
	}
	else
	{
		Enemys* eColl = playscene->GetPlayScene_enemys();
		for (int e = 0; e < eColl->GetEnemys().size(); e++)
		{
			eColl->GetEnemys()[e]->SetisAffectedSlash(true);
		}
	}
}
