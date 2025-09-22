#include "Vine.h"
#include "Engine/BoxCollider.h"
#include "Engine/Audio.h"
#include "Engine/Model.h"
#include "HP.h"
#include "Player.h"
#include "Stage.h"
#include "Timer.h"

Vine::Vine(GameObject* parent)
	:GameObject(parent, "Vine"), vineColl_(nullptr), vTipColl_(nullptr), vBlockColl_(nullptr), player_(nullptr), stage_(nullptr), timer_(nullptr),
	hVine_(NULL_NUM), hTipVine_(NULL_NUM), hSpinyHitSound_(NULL_NUM), attack_(0), moveSpeed_(0.0f), pBlowDist_(0.0f), vineTimer_(0.0f), 
	vineSize_({ 0,0 }),vCollPos_({ 0,0,0 }), vCollSize_({ 0,0,0 }), vBlockCollPos_({ 0,0,0 }), vBlockCollSize_({ 0,0,0 }),vines_(0), addVinesLine_(0), 
	vineCollTall_(0), moveNum_(0.0f),isGrow_(true), isMove_(true), isPCollision_(true), vType_(NORMAL), dir_(UP)
{
	for (int v = 0; v < V_MAX; v++)
	{
		vTipCollPos_[v] = { 0,0,0 };
		vTipCollSize_[v] = { 0,0,0 };
	}
}

void Vine::Initialize()
{
	//csvからデータ読み込み
	bool ret = csv.Load("CsvData\\Stage\\VineData.csv");
	assert(ret);
	int row = 1, column = 0;

	//ハンドルにデータをセット
	hVine_ = Model::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hVine_ > NULL_NUM);

	attack_    = csv.extractInt(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	moveSpeed_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	pBlowDist_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	vineTimer_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	vineSize_  = csv.extractFloat2(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	vCollPos_  = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column))); 
	vCollSize_ = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	for (int v = 0; v < V_MAX; v++)
	{
		vTipCollPos_[v] = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	}
	for (int v = 0; v < V_MAX; v++)
	{
		vTipCollSize_[v] = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	}
	vBlockCollPos_ = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	vBlockCollSize_ = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column)));


	stage_ = (Stage*)GetParent();
	player_ = stage_->GetParent()->FindGameObject<Player>();
	timer_ = Instantiate<Timer>(this);

	vines_ = DEFAULT_VINE_NUM;
	spinyHitEf_ = VFX::SetEffect("SpinyHitEffectData.csv");
}

void Vine::Update()
{
	if (isMove_)
	{
		if (isGrow_)
		{
			transform_.position_.x += moveSpeed_ * NUM_DIR[dir_].x;
			transform_.position_.y += moveSpeed_ * NUM_DIR[dir_].y;
			moveNum_ += moveSpeed_;
		}
		else
		{
			transform_.position_.x -= moveSpeed_ * NUM_DIR[dir_].x;
			transform_.position_.y -= moveSpeed_ * NUM_DIR[dir_].y;
			moveNum_ -= moveSpeed_;
		}
	}

	if (isGrow_) {
		if (moveNum_ >= addVinesLine_)
		{
			GrowVine(isGrow_);
		}
	}
	else
	{
		if (moveNum_ <= addVinesLine_ - (vineColl_->Getsize().x * HALF))
		{
			if (vines_ > 0)
			{
				GrowVine(isGrow_);
			}
			else if (vines_ == 0)
			{
				if (timer_->TimeElapsed(vineTimer_))
				{
					isGrow_ = true;
				}
			}
		}
	}

	Collision();
}

void Vine::GrowVine(const bool& isGrow)
{
	XMFLOAT2 absDir = { abs(NUM_DIR[dir_].x),abs(NUM_DIR[dir_].y) };
	XMFLOAT2 vSize = { vineColl_->Getsize().x ,vineColl_->Getsize().y };

	if (isGrow_)
	{
		vines_++;
		addVinesLine_++;
	}
	else
	{
		vines_--;
		addVinesLine_--;
	}

	vineColl_->Setcenter({ ((-HALF) * (vines_ - 1)) * NUM_DIR[dir_].x,
						   ((-HALF) * (vines_ - 1)) * NUM_DIR[dir_].y, vCollPos_.z });

	if (dir_ == LEFT || dir_ == RIGHT)
	{
		vineColl_->Setsize({ (float)vines_,vSize.y, vCollSize_.z });
	}
	else
	{
		vineColl_->Setsize({ vSize.x,(float)vines_, vCollSize_.z });
	}
}

void Vine::Draw()
{
	for (int i = 0; i < vines_; i++)
	{
		Transform tmp = transform_;
		tmp.position_.x -= i*NUM_DIR[dir_].x;
		tmp.position_.y -= i*NUM_DIR[dir_].y;
		if (i == 0)
		{
			Model::SetTransform(hTipVine_, tmp);
			Model::Draw(hTipVine_);
		}
		else
		{
			Model::SetTransform(hVine_, tmp);
			Model::Draw(hVine_);
		}
	}
}

void Vine::SetvType(const VINETYPE& vType)
{
	vType_ = vType;
	int row = 1;

	switch (vType_)
	{
	case SPINY:
		hTipVine_ = Model::Load(csv.GetString(row, csv.ColumnSearch(csv, "hTipSpiny")));
		hSpinyHitSound_ = Audio::Load(csv.GetString(row, csv.ColumnSearch(csv, "hSpinyHitSound")));
		assert(hSpinyHitSound_ > NULL_NUM);
		break;
	case LEAF:
		hTipVine_ = Model::Load(csv.GetString(row, csv.ColumnSearch(csv, "hTipLeaf")));
		break;
	default:
		hTipVine_ = Model::Load(csv.GetString(row, csv.ColumnSearch(csv, "hTipNormal")));
		break;
	}
	assert(hTipVine_ > NULL_NUM);
}

void Vine::SetvDir(const DIR& dir)
{
	dir_ = dir;
	transform_.rotate_.z = -GetAngleFromVector(NUM_DIR[dir_], true);//dir_を元に向きを設定

	switch (dir_)
	{
	case LEFT:
	case RIGHT:
		vineColl_ = new BoxCollider(vCollPos_, { vCollSize_.y,vCollSize_.x,vCollSize_.z });

		vTipColl_ = new BoxCollider({ vTipCollPos_[vType_].y*NUM_DIR[dir_].x,vTipCollPos_[vType_].x,vTipCollPos_[vType_].z},
			                        { vTipCollSize_[vType_].y,vTipCollSize_[vType_].x,vTipCollSize_[vType_].z });

		vBlockColl_ = new BoxCollider({ vBlockCollPos_.y * NUM_DIR[dir_].x, vBlockCollPos_.x, vBlockCollPos_.z },
			                          { vBlockCollSize_.y, vBlockCollSize_.x, vBlockCollSize_.z });
		break;
	case UP:
	case DOWN:
		vineColl_ = new BoxCollider(vCollPos_, vCollSize_);

		vTipColl_ = new BoxCollider({ vTipCollPos_[vType_].x,vTipCollPos_[vType_].y * NUM_DIR[dir_].y,vTipCollPos_[vType_].z }, vTipCollSize_[vType_]);

		vBlockColl_ = new BoxCollider({ vBlockCollPos_.x, vBlockCollPos_.y * NUM_DIR[dir_].y, vBlockCollPos_.z }, vBlockCollSize_);
		break;
	default://デフォルトはUPなのでUPだった場合はここに含む
		vineColl_ = new BoxCollider(vCollPos_, vCollSize_);

		vTipColl_ = new BoxCollider(vTipCollPos_[vType_], vTipCollSize_[vType_]);

		vBlockColl_ = new BoxCollider(vBlockCollPos_, vBlockCollSize_);
		break;
	}

	AddCollider(vineColl_);
	AddCollider(vTipColl_);
	AddCollider(vBlockColl_);
}

void Vine::Collision()
{
	for (int b = 0; b < stage_->GetBlocksSize(); b++)
	{
		if (stage_->GetboxColl(b) != nullptr)//ブロックの当たり判定がnullptr(空白)じゃなかったら当たり判定
		{
			if (vBlockColl_->IsHit(stage_->GetboxColl(b)))
			{
				if (timer_->TimeElapsed(vineTimer_))
				{
					isMove_ = true;
					isGrow_ = !isGrow_;
					isPCollision_ = true;
				}
				else
				{
					if (isGrow_ == false)
					{
						isPCollision_ = false;
					}
					isMove_ = false;
				}
			}
		}
	}


	if (vType_ == SPINY && isPCollision_)
	{
		if (vTipColl_->IsHit(player_->GetPBoxColl(LEFT)) || vTipColl_->IsHit(player_->GetPBoxColl(RIGHT)) || vTipColl_->IsHit(player_->GetPBoxColl(CENTER))) 
		{
			HP* hp = ((HP*)GetParent()->GetParent()->FindGameObject<HP>());
			if (hp->GetIsDamage())
			{
				//位置を設定してエフェクト再生
				spinyHitEf_.position = player_->GetPosition();
				VFX::Start(spinyHitEf_);
				Audio::Play(hSpinyHitSound_);//音再生
			}
			hp->DecreaseHP(attack_);//プレイヤーのHPを減らす
			hp->SetVibrationRangeX(HP_VIB_RANGE.x);//HPバーを振動させる

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