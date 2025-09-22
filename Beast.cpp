#include "Beast.h"
#include "Engine/BoxCollider.h"
#include "Engine/CsvReader.h"
#include "Engine/Model.h"
#include "Enemys.h"
#include "HP.h"
#include "Player.h"
#include "PlayScene.h"
#include "Stage.h"
#include "Vine.h"
#include "Timer.h"

Beast::Beast(GameObject* parent)
	:GameObject(parent, "Beast"), player_(nullptr), stage_(nullptr), hBeast_(NULL_NUM),
	maxHP_(0), attack_(0), moveSpeed_(0.0f), jumpHeight_(0.0f), gravity_(0.0f), pBlowDist_(0.0f), rotSpeed_(0.0f),
	jumpSpeed_(0.0f), ground_(0.0f), ceiling_(0.0f), isGround_(false), isLeftMove_(true), isRightMove_(true), isJump_(true)
{
	for (int i = 0; i < ENEMY_COLL_NUM; i++)
	{
		boxColl_[i] = nullptr;
	}

	for (int i = 0; i < DETECT_BOX_NUM; i++)
	{
		detectBox_[i] = nullptr;
	}

	for (int i = 0; i < DETECT_BOX_NUM; i++)
	{
		isChase_[i] = false;
	}
}

void Beast::Initialize()
{
	//csvからデータ読み込み
	CsvReader csv;//データを読むクラスのインスタンスを作成
	bool ret = csv.Load("CsvData\\Chara\\BeastData.csv");
	assert(ret);
	int row = 1, column = 0;

	//ハンドルにデータをセット
	hBeast_ = Model::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hBeast_ > NULL_NUM);
	hAttackSound_ = Audio::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hAttackSound_ > NULL_NUM);

	maxHP_ = csv.extractInt(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	currentHP_ = maxHP_;
	attack_ = csv.extractInt(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	moveSpeed_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	jumpHeight_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	gravity_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	pBlowDist_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	rotSpeed_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));

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

	//検知範囲の設定
	for (int i = 0; i < DETECT_BOX_NUM; i++)
	{
		detectBox_[i] = new BoxCollider(csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column))),
			                            csv.extractFloat3(csv.GetString(row, csv.ColumnSearch(csv,"detectBoxSize"))));
		AddCollider(detectBox_[i]);
	}

	player_ = ((Player*)GetParent()->GetParent()->FindGameObject<Player>());
	stage_ = GetParent()->GetParent()->FindGameObject<Stage>();
	blinkTime_ = Instantiate<Timer>(this);
}

void Beast::Update()
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
		MoveBeast();
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

	//Y位置が天井を超えてたら下げる
	if (transform_.position_.y >= ceiling_) {
		transform_.position_.y = ceiling_;
		jumpSpeed_ = 0.0f;
		isJump_ = false;
	}

	//Y位置が地面より下なら上げる
	if (transform_.position_.y <= ground_) {
		transform_.position_.y = ground_;
		jumpSpeed_ = 0.0f;
		isJump_ = true;
	}

	isJump_ = true;
	isLeftMove_ = true;
	isRightMove_ = true;
	ground_ = DEFAULT_GROUND;
}

void Beast::Draw()
{
	if (isDraw_)
	{
		Model::SetTransform(hBeast_, transform_);
		Model::Draw(hBeast_);
	}
}

void Beast::Release()
{
	Model::Release(hBeast_);
}

void Beast::RotBeast()
{
	if (isRight_)
	{
		if (transform_.rotate_.y != OBJ_RIGHT_ANGLE)
		{
			transform_.rotate_.y -= rotSpeed_;
		}
	}
	else
	{
		if (transform_.rotate_.y != OBJ_LEFT_ANGLE)
		{
			transform_.rotate_.y += rotSpeed_;
		}
	}
}

void Beast::OnCollision(GameObject* pTarget)
{
	XMFLOAT3 collPos[5], collSize[5];
	//当たり判定の設定
	for (int i = 0; i < ENEMY_COLL_NUM; i++)
	{
		collPos[i] = boxColl_[i]->Getcenter();
		collSize[i] = boxColl_[i]->Getsize();
	}

	//ブロックの当たり判定
	for (int b = 0; b < stage_->GetBlocksSize(); b++)
	{
		if (stage_->GetboxColl(b) != nullptr)//ブロックの当たり判定がnullptr(空白)じゃなかったら当たり判定
		{
			if (boxColl_[LEFT]->IsHit(stage_->GetboxColl(b))) {
				isLeftMove_ = false;//当たったら左に進めなくする
				if (isGround_)
				{
					float dist = (abs)(collPos[LEFT].x) + (abs)(collPos[LEFT].x * HALF);//ブロックからどのくらい離すか(プレイヤーの中心から当たり判定の中心の距離分)
					transform_.position_.x = (stage_->GetboxColl(b)->Getcenter().x + (stage_->GetboxColl(b)->Getsize().x * HALF)) + dist;
					jumpSpeed_ = -sqrtf(gravity_ * jumpHeight_);//ジャンプさせる
					isGround_ = false;
				}
			}

			if (boxColl_[RIGHT]->IsHit(stage_->GetboxColl(b))) {
				isRightMove_ = false;//当たったら右に進めなくする
				if (isGround_)
				{
					float dist = (abs)(collPos[RIGHT].x) + (abs)(collSize[RIGHT].x * HALF);//ブロックからどのくらい離すか(プレイヤーの中心から当たり判定の中心の距離分)
					transform_.position_.x = (stage_->GetboxColl(b)->Getcenter().x - (stage_->GetboxColl(b)->Getsize().x * HALF)) - dist;
					jumpSpeed_ = -sqrtf(gravity_ * jumpHeight_);//ジャンプさせる
					isGround_ = false;
				}
			}

			if (isJump_) {
				if (boxColl_[UP]->IsHit(stage_->GetboxColl(b))) {
					jumpSpeed_ = 0;//当たったらジャンプを止める
					float dist = (abs)(collPos[UP].y);//ブロックからどのくらい離すか(プレイヤーの中心から当たり判定の中心の距離分)
					ceiling_ = (stage_->GetboxColl(b)->Getcenter().y - (stage_->GetboxColl(b)->Getsize().y * HALF)) - (dist);
					isJump_ = false;
				}
			}

			if (boxColl_[DOWN]->IsHit(stage_->GetboxColl(b))) {
				float dist = (abs)(collPos[DOWN].y);//ブロックからどのくらい離すか(プレイヤーの中心から当たり判定の中心の距離分)
				ground_ = (stage_->GetboxColl(b)->Getcenter().y + (stage_->GetboxColl(b)->Getsize().y * HALF)) + (dist);//地面に乗せる
				isGround_ = true;
			}
		}
	}

	if (!player_->GetIsEnd())
	{
		for (int i = 0; i < DETECT_BOX_NUM; i++)
		{
			//プレイヤーの追跡範囲　これに当たったらプレイヤーを追う
			if (detectBox_[i]->IsHit(player_->GetPBoxColl(CENTER)))
			{
				isChase_[i] = true;
			}
			else
			{
				isChase_[i] = false;
			}
		}

		//プレイヤーに当たったらHPを減らしてノックバック
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

void Beast::MoveBeast()
{
	for (int i = 0; i < DETECT_BOX_NUM; i++)
	{
		if (isChase_[i] == true)//isChase_のどれか一つでもtrueだったら追跡する
		{
			XMFLOAT3 pPos = player_->GetPosition();
			if (transform_.position_.x >= pPos.x)
			{
				isRight_ = false;
				if (isLeftMove_)
				{
					transform_.position_.x -= moveSpeed_;
				}
			}
			else if (transform_.position_.x <= pPos.x)
			{
				isRight_ = true;
				if (isRightMove_)
				{
					transform_.position_.x += moveSpeed_;
				}
			}

			break;
		}
	}

	jumpSpeed_ += gravity_;
	transform_.position_.y -= jumpSpeed_;

	RotBeast();
}