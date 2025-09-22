#include "Nest.h"
#include "Engine/Model.h"
#include "Engine/CsvReader.h"
#include "Engine/BoxCollider.h"
#include "Bee.h"
#include "Enemys.h"
#include "Player.h"
#include "PlayScene.h"
#include "Stage.h"
#include "Timer.h"

Nest::Nest(GameObject* parent)
	:GameObject(parent, "Nest"), boxColl_(nullptr), playScene_(nullptr), player_(nullptr), enemys_(nullptr), birthIntervalTime_(nullptr),
	hNest_(NULL_NUM), maxHP_(0), birthInterval_(0.0f), beeDist_(0.0f), isBirthBee_(true)
{
}

void Nest::Initialize()
{
	//csvからデータ読み込み
	CsvReader csv;//データを読むクラスのインスタンスを作成
	bool ret = csv.Load("CsvData\\Chara\\BeeNestData.csv");
	assert(ret);
	int row = 1, column = 0;

	//ハンドルにデータをセット
	hNest_ = Model::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hNest_ > NULL_NUM);

	maxHP_ = csv.extractInt(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	currentHP_ = maxHP_;
	birthInterval_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	beeDist_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));

	//当たり判定の設定
	boxColl_ = new BoxCollider({ 0,0,0 }, { 0,0,0 });
	boxColl_->Setcenter(csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	boxColl_->Setsize(csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	AddCollider(boxColl_);
	attackedColl_ = boxColl_;

	playScene_ = (PlayScene*)(GetParent()->GetParent());
	enemys_ = playScene_->GetPlayScene_enemys();
	player_ = ((Player*)GetParent()->GetParent()->FindGameObject<Player>());
	blinkTime_ = Instantiate<Timer>(this);
	birthIntervalTime_ = Instantiate<Timer>(this);
}

void Nest::Update()
{
	if (isSet_)
	{
		if (isRight_) 
		{
			transform_.rotate_.y = OBJ_RIGHT_ANGLE;
		}
		else 
		{
			transform_.rotate_.y = OBJ_LEFT_ANGLE;
		}
		isSet_ = false;
	}

	if (currentHP_ <= HP_DEAD_LINE)//HP_DEAD_LINEを下回ったら削除
	{
		Audio::Play(hDieSound_);
		dieEf_.position = transform_.position_;
		VFX::Start(dieEf_);
		isAlive_ = false;
		enemys_->DeleteEnemy();//敵配列からも削除
		KillMe();
	}

	if (isBirthBee_ && !player_->GetIsEnd())
	{
		if (birthIntervalTime_->TimeElapsed(birthInterval_))
		{
			isBirthBee_ = false;
			XMFLOAT3 p = transform_.position_;
			Bee* bee = Instantiate<Bee>(FindObject("PlayScene"));
			bee->SetNestPointer(this);
			bee->SetisRight(isRight_);
			if (isRight_)
			{
				bee->SetPosition({ p.x + beeDist_,p.y,p.z });
			}
			else
			{
				bee->SetPosition({ p.x - beeDist_,p.y,p.z });
			}
			enemys_->AddEnemy(bee);
		}
	}

	if (isBlinking_)
	{
		if (!blinkTime_->TimeElapsed(BLINKING_INTERVAL))
		{
			Blinking(isDraw_);
		}
		else
		{
			isDraw_ = true;
			isBlinking_ = false;
		}
	}
}

void Nest::Draw()
{
	if (isDraw_ && currentHP_ > HP_DEAD_LINE)
	{
		Model::SetTransform(hNest_, transform_);
		Model::Draw(hNest_);
	}
}

void Nest::Release()
{
	Model::Release(hNest_);
}