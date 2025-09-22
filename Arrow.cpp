#include "Arrow.h"
#include "Engine/BoxCollider.h"
#include "Engine/CsvReader.h"
#include "Engine/Model.h"
#include "Engine/Audio.h"
#include "Enemys.h"
#include "Player.h"
#include "PlayScene.h"
#include "Stage.h"
#include "Timer.h"

Arrow::Arrow(GameObject* parent)
	:GameObject(parent, "Arrow"), boxColl_(nullptr), playscene_(nullptr), eraseTimer_(nullptr),hArrow_(NULL_NUM), hArrowHitSound_(NULL_NUM), 
	attack_(0), moveSpeed_(0.0f), gravity_(0.0f), eraseTime_(0.0f), magniDist_(0.0f),defaultDirPos_(0.0f), maxDir_(0.0f), rotateSpeed_(0.0f), 
	dirY_(0.0f), isMove_(true), objDir_(RIGHT)
{
}

void Arrow::Initialize()
{
	//csvからデータ読み込み
	CsvReader csv;//データを読むクラスのインスタンスを作成
	bool ret = csv.Load("CsvData\\ArrowData.csv");
	assert(ret);
	int row = 1, column = 0;

	//ハンドルにデータをセット
	hArrow_ = Model::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hArrow_ > NULL_NUM);
	hArrowHitSound_ = Audio::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hArrowHitSound_ > NULL_NUM);

	attack_ = csv.extractInt(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	moveSpeed_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	gravity_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	eraseTime_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	magniDist_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	defaultDirPos_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	maxDir_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	rotateSpeed_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));

	boxColl_ = new BoxCollider({ 0,0,0 }, { 0,0,0 });
	boxColl_->Setcenter(csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	boxColl_->Setsize(csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	AddCollider(boxColl_);

	Player* player = GetParent()->FindGameObject<Player>();
	playscene_ = (PlayScene*)GetParent();
	eraseTimer_ = Instantiate<Timer>(this);

	transform_.position_ = player->GetPosition();
	arrowHitEf_ = VFX::SetEffect("ArrowHitEffectData.csv");
}

void Arrow::Update()
{
	if (isMove_)
	{
		//重力で少しずつ角度と高度が落ちる
		transform_.position_.y += dirY_;
		if (transform_.rotate_.x < maxDir_)
		{
			transform_.rotate_.x -= dirY_ * rotateSpeed_;
		}
		dirY_ -= gravity_;

		//角度（度単位）をラジアンに変換
		float angleInDegrees = transform_.rotate_.x;
		float angleInRadians = angleInDegrees * DEG_TO_RAD;

		//ベクトルの計算
		float unitVectorX = cos(angleInRadians) * magniDist_;
		float unitVectorY = sin(angleInRadians) * magniDist_;

		//プレイヤーの向きで進む方向を変える
		if (objDir_ == RIGHT)
		{
			boxColl_->Setcenter({ unitVectorX,-unitVectorY,boxColl_->Getcenter().z});
		}
		else
		{
			boxColl_->Setcenter({ -unitVectorX,-unitVectorY,boxColl_->Getcenter().z });
		}
		transform_.position_.x += moveSpeed_ * NUM_DIR[objDir_].x;
	}

	ArrowCollision();

	if (!isMove_)
	{
		if (eraseTimer_->TimeElapsed(eraseTime_))
		{
			KillMe();
		}
	}
}

void Arrow::Draw()
{
	Model::SetTransform(hArrow_, transform_);
	Model::Draw(hArrow_);
}

void Arrow::Release()
{
	Model::Release(hArrow_);
}

void Arrow::SetArrowDir(const bool& PisRight)
{
	if (PisRight)
	{
		transform_.rotate_.y = OBJ_RIGHT_ANGLE;
		objDir_ = RIGHT;
	}
	else
	{
		transform_.rotate_.y = OBJ_LEFT_ANGLE;
		objDir_ = LEFT;
	}
}

void Arrow::ArrowCollision()
{
	//ブロックとの当たり判定
	Stage* stage = GetParent()->FindGameObject<Stage>();
	for (int b = 0; b < stage->GetBlocksSize(); b++)
	{
		if (stage->GetboxColl(b) != nullptr)//ブロックの当たり判定がnullptr(空白)じゃなかったら当たり判定
		{
			if (boxColl_->IsHit(stage->GetboxColl(b))) {
				isMove_ = false;
			}
		}
	}

	//動きが止まってなければ敵との当たり判定
	if (isMove_)
	{
		Enemys* eColl = playscene_->GetPlayScene_enemys();
		for (int e = 0; e < eColl->GetEnemys().size(); e++)
		{
			if (eColl->GetEnemys()[e]->GetCurrentHP() > 0 && eColl->GetEnemys()[e]->GetAttackedColl() != nullptr)
			{
				//敵の体力が残ってるかつ既に攻撃を受けていないか
				if (boxColl_->IsHit(eColl->GetEnemys()[e]->GetAttackedColl())) 
				{
					//位置を設定してエフェクト再生
					arrowHitEf_.position = transform_.position_;
					VFX::Start(arrowHitEf_);
					Audio::Play(hArrowHitSound_);//音再生

					eColl->GetEnemys()[e]->SetisBlinking(true);
					eColl->GetEnemys()[e]->DecreaseHP(attack_);

					KillMe();
				}
			}
		}
	}
}
