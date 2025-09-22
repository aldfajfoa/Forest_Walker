#include "Flower.h"
#include "Engine/BoxCollider.h"
#include "Engine/CsvReader.h"
#include "Engine/Model.h"
#include "Enemys.h"
#include "HP.h"
#include "Player.h"
#include "PlayScene.h"
#include "Stage.h"
#include "Timer.h"

Flower::Flower(GameObject* parent)
	:GameObject(parent, "Flower"), checkScaffoldColl_(nullptr), player_(nullptr), stage_(nullptr),
	hFlower_(NULL_NUM), maxHP_(0), attack_(0), moveSpeed_(0.0f), gravity_(0.0f), pBlowDist_(0.0f), rotSpeed_(0.0f), 
	rightAngle_(0.0f), leftAngle_(0.0f), checkScaffoldPosL_({ 0,0,0 }), checkScaffoldPosR_({ 0,0,0 }), checkScaffoldSize_({ 0,0,0 }), 
	ground_(0.0f), isLeftMove_(true), isRightMove_(true)
{
	for (int i = 0; i < ENEMY_COLL_NUM; i++)
	{
		boxColl_[i] = nullptr;
	}
}

void Flower::Initialize()
{
	//csvからデータ読み込み
	CsvReader csv;//データを読むクラスのインスタンスを作成
	bool ret = csv.Load("CsvData\\Chara\\FlowerData.csv");
	assert(ret);
	int row = 1, column = 0;

	//ハンドルにデータをセット
	hFlower_ = Model::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hFlower_ > NULL_NUM);
	hAttackSound_ = Audio::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hAttackSound_ > NULL_NUM);

	maxHP_ = csv.extractInt(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	currentHP_ = maxHP_;

	attack_ = csv.extractInt(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	moveSpeed_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	gravity_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	pBlowDist_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	rotSpeed_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	rightAngle_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	leftAngle_ = 360 - rightAngle_;//左向き角度*/

	//当たり判定の設定
	for (int i = 0; i < ENEMY_COLL_NUM; i++)
	{
		boxColl_[i] = new BoxCollider({ 0,0,0 }, { 0,0,0 });
		boxColl_[i]->Setcenter(csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column))));//位置設定
	}
	for (int i = 0; i < ENEMY_COLL_NUM; i++)
	{
		boxColl_[i]->Setsize(csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column))));//サイズ設定
		AddCollider(boxColl_[i]);
	}
	attackedColl_ = boxColl_[CENTER];

	checkScaffoldPosL_ = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	checkScaffoldPosR_ = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	checkScaffoldSize_ = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column)));

	checkScaffoldColl_ = new BoxCollider(checkScaffoldPosL_, checkScaffoldSize_);
	AddCollider(checkScaffoldColl_);

	player_ = ((Player*)GetParent()->GetParent()->FindGameObject<Player>());
	stage_ = (Stage*)GetParent();
	blinkTime_ = Instantiate<Timer>(this);
}

void Flower::Update()
{
	if (isSet_)
	{
		if (isRight_)
		{
			transform_.rotate_.y = rightAngle_;
		}
		else
		{
			transform_.rotate_.y = leftAngle_;
		}
		isSet_ = false;
	}

	if (currentHP_ <= HP_DEAD_LINE)//HP_DEAD_LINE以下になったら削除
	{
		//位置を設定してエフェクト再生
		dieEf_.position = transform_.position_;
		VFX::Start(dieEf_);
		Audio::Play(hDieSound_);//音再生
		isAlive_ = false;
		PlayScene* p = (PlayScene*)(stage_->GetParent());
		Enemys* enemys = p->GetPlayScene_enemys();
		enemys->DeleteEnemy();//敵配列からも削除
		KillMe();
	}

	if (!player_->GetIsEnd())
	{
		MoveFlower();
		DetermineRot();
	}

	if (isBlinking_)//点滅させる
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
	
	if (transform_.position_.y <= ground_) {
		transform_.position_.y = ground_;
	}

	isLeftMove_ = true;
	isRightMove_ = true;
	ground_ = DEFAULT_GROUND;
}

void Flower::Draw()
{
	if (isDraw_)
	{
		Model::SetTransform(hFlower_, transform_);
		Model::Draw(hFlower_);
	}
}

void Flower::Release()
{
	Model::Release(hFlower_);
}

void Flower::RotFlower(const bool& isRight)
{
	if (isRight_)
	{
		if (transform_.rotate_.y != rightAngle_)
		{
			transform_.rotate_.y -= rotSpeed_;
		}
		checkScaffoldColl_->Setcenter(checkScaffoldPosR_);
	}
	else
	{
		if (transform_.rotate_.y != leftAngle_)
		{
			transform_.rotate_.y += rotSpeed_;
		}
		checkScaffoldColl_->Setcenter(checkScaffoldPosL_);
	}
}

void Flower::OnCollision(GameObject* pTarget)
{
	//ブロックとの当たり判定
	for (int b = 0; b < stage_->GetBlocksSize(); b++)
	{
		if (stage_->GetboxColl(b) != nullptr)//ブロックの当たり判定がnullptr(空白)じゃなかったら当たり判定
		{
			if (boxColl_[LEFT]->IsHit(stage_->GetboxColl(b))) {
				isLeftMove_ = false;//当たったら左に進めなくする
				isRight_ = true;
			}

			if (boxColl_[RIGHT]->IsHit(stage_->GetboxColl(b))) {
				isRightMove_ = false;//当たったら右に進めなくする
				isRight_ = false;
			}

			if (boxColl_[DOWN]->IsHit(stage_->GetboxColl(b))) {
				//ground_ = stage_->GetboxColl(b)->Getcenter().y + DEFAULT_BLOCK_SIZE.y;//地面に乗せる
				float dist = (abs)(boxColl_[DOWN]->Getcenter().y);//ブロックからどのくらい離すか(プレイヤーの中心から当たり判定の中心の距離分)
				ground_ = (stage_->GetboxColl(b)->Getcenter().y + (stage_->GetboxColl(b)->Getsize().y * HALF)) + (dist);//地面に乗せる
			}
		}
	}

	if (!player_->GetIsEnd())
	{
		if (boxColl_[CENTER]->IsHit(player_->GetPBoxColl(CENTER))) {
			HP* hp = ((HP*)GetParent()->GetParent()->FindGameObject<HP>());
			if (hp->GetIsDamage())
			{
				//位置を設定してエフェクト再生
				attackEf_.position = player_->GetPosition();
				VFX::Start(attackEf_);
				Audio::Play(hAttackSound_);//音再生
			}
			hp->DecreaseHP(attack_);//プレイヤーのHPを減らす
			hp->SetVibrationRangeX(HP_VIB_RANGE.x);//HPバーを振動させる

			//向いてる方向にプレイヤーをノックバック
			if (player_->GetPosition().x >= transform_.position_.x)
			{
				player_->SetDistandDir(pBlowDist_, RIGHT);
			}
			else
			{
				player_->SetDistandDir(pBlowDist_, LEFT);
			}
			player_->SetisBlinking(true);
		}
	}
}

void Flower::DetermineRot()
{
	//進行方向に足場が無かったら進行方向を反転
	int b = 0;
	for (b = 0; b < stage_->GetBlocksSize(); b++)
	{
		if ((checkScaffoldColl_->IsHit(stage_->GetboxColl(b)))) {//どこかに当たった時点でループを中断
			break;
		}
	}

	if (!(b < stage_->GetBlocksSize()))//ループの最後までどことも当たって無かったら反転
	{
		isRight_ = !isRight_;
	}
}

void Flower::MoveFlower()
{
	if (isLeftMove_ && isRight_ == false)//左へ進む
	{
		transform_.position_.x -= moveSpeed_;
	}
	if (isRightMove_ && isRight_)//右へ進む
	{
		transform_.position_.x += moveSpeed_;
	}

	transform_.position_.y -= gravity_;

	RotFlower(isRight_);
}
