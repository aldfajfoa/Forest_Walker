#include "Bee.h"
#include "Engine/CsvReader.h"
#include "Engine/Model.h"
#include "Enemys.h"
#include "Nest.h"
#include "HP.h"
#include "Player.h"
#include "PlayScene.h"
#include "Stage.h"
#include "Timer.h"

Bee::Bee(GameObject* parent)
	:GameObject(parent, "Bee"), detectBox_(nullptr), nest_(nullptr), player_(nullptr), stage_(nullptr), blowInterval_(nullptr),
	hBee_(NULL_NUM), maxHP_(0), attack_(0),pBlowDist_(0.0f), rotSpeed_(0.0f), acceleration_(0.0f), maxSpeed_(0.0f), addSizeAngleNum_(0.0f), 
	blowIntervalTime_(0.0f), detectBoxPosL_({0,0,0}), detectBoxPosR_({ 0,0,0 }), detectBoxSize_({ 0,0,0 }),
	sinAngle_(0.0f), baseY_(0.0f), speed_(0.0f), isHovering_(true), isUp_(false), isBlow_(false)
{
	for (int i = 0; i < ENEMY_COLL_NUM; i++)
	{
		boxColl_[i] = nullptr;
	}

	for (int i = 0; i < IS_MOVE_NUM; i++)
	{
		isMove_[i] = false;
	}
}

void Bee::Initialize()
{
	//csv����f�[�^�ǂݍ���
	CsvReader csv;//�f�[�^��ǂރN���X�̃C���X�^���X���쐬
	bool ret = csv.Load("CsvData\\Chara\\BeeData.csv");
	assert(ret);
	int row = 1, column = 0;

	//�n���h���Ƀf�[�^���Z�b�g
	hBee_ = Model::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hBee_ > NULL_NUM);
	hAttackSound_ = Audio::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hAttackSound_ > NULL_NUM);

	maxHP_ = csv.extractInt(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	currentHP_ = maxHP_;

	attack_ = csv.extractInt(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	pBlowDist_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	rotSpeed_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	acceleration_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	maxSpeed_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	addSizeAngleNum_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	blowIntervalTime_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	
	//�����蔻��̐ݒ�
	for (int i = 0; i < ENEMY_COLL_NUM; i++)
	{
		boxColl_[i] = new BoxCollider({ 0,0,0 }, { 0,0,0 });
		boxColl_[i]->Setcenter(csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column))));//�ʒu�ݒ�
	}
	for (int i = 0; i < ENEMY_COLL_NUM; i++)
	{
		boxColl_[i]->Setsize(csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column))));//�T�C�Y�ݒ�
		AddCollider(boxColl_[i]);
	}
	attackedColl_ = boxColl_[CENTER];

	detectBoxPosL_ = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	detectBoxPosR_ = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	detectBoxSize_ = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column)));

	player_ = (Player*)GetParent()->FindGameObject<Player>();
	stage_ = (Stage*)GetParent()->FindGameObject<Stage>();
	blowInterval_ = Instantiate<Timer>(this);
	blinkTime_ = Instantiate<Timer>(this);
}

void Bee::Update()
{
	if (currentHP_ <= HP_DEAD_LINE)//HP_DEAD_LINE�ȉ��ɂȂ�����폜
	{
		if (nest_ != nullptr)
		{
			nest_->SetisBirthBee(true);
		}

		//�ʒu��ݒ肵�ăG�t�F�N�g�Đ�
		dieEf_.position = transform_.position_;
		VFX::Start(dieEf_);
		Audio::Play(hDieSound_);//���Đ�
		isAlive_ = false;
		PlayScene* p = (PlayScene*)(stage_->GetParent());
		Enemys* enemys = p->GetPlayScene_enemys();
		enemys->DeleteEnemy();//�G�z�񂩂���폜
		KillMe();
	}

	if (isSet_)
	{
		baseY_ = transform_.position_.y;
		if (isRight_)
		{
			transform_.rotate_.y = OBJ_RIGHT_ANGLE;
			detectBox_ = new BoxCollider(detectBoxPosR_, detectBoxSize_);
		}
		else
		{
			transform_.rotate_.y = OBJ_LEFT_ANGLE;
			detectBox_ = new BoxCollider(detectBoxPosL_, detectBoxSize_);
		}

		AddCollider(detectBox_);
		isSet_ = false;
	}

	if (!player_->GetIsEnd())
	{
		MoveBee();
	}

	if (isBlinking_)//�_�ł�����
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

	if (isBlow_== false)
	{
		if (blowInterval_->TimeElapsed(blowIntervalTime_))
		{
			isBlow_ = true;
		}
		else
		{
			isBlow_ = false;
		}
	}

	for (int i = 0; i < IS_MOVE_NUM; i++)
	{
		isMove_[i] = true;
	}
}

void Bee::Draw()
{
	if (isDraw_)
	{
		Model::SetTransform(hBee_, transform_);
		Model::Draw(hBee_);
	}
}

void Bee::Release()
{
	Model::Release(hBee_);
}

void Bee::MoveBee()
{
	if (isHovering_)
	{
		float tmpY = transform_.position_.y;
		float prevY = transform_.position_.y;

		sinAngle_ += addSizeAngleNum_;//�x
		float sinValue = sinf(sinAngle_ * DEG_TO_RAD);
		tmpY = baseY_ + sinValue;

		if (prevY > tmpY)
		{
			isUp_ = false;
		}
		else
		{
			isUp_ = true;
		}

		if((isUp_ && isMove_[UP]) || (isUp_==false && isMove_[DOWN]))
		{
			transform_.position_.y = tmpY;
		}
	}
	else
	{
		if (speed_ <= maxSpeed_)
		{
			speed_ += acceleration_;
		}

		if (isRight_ && isMove_[RIGHT])
		{
			transform_.position_.x += speed_;
		}
		else if (isRight_ == false && isMove_[LEFT])
		{
			transform_.position_.x -= speed_;
		}
	}

	RotBee();
}

void Bee::OnCollision(GameObject* pTarget)
{
	//�u���b�N�̓����蔻��
	for (int b = 0; b < stage_->GetBlocksSize(); b++)
	{
		if (stage_->GetboxColl(b) != nullptr)//�u���b�N�̓����蔻�肪nullptr(��)����Ȃ������瓖���蔻��
		{
			if (boxColl_[LEFT]->IsHit(stage_->GetboxColl(b))) {
				isMove_[LEFT] = false;//���������獶�ɐi�߂Ȃ�����
				isHovering_ = true;
				speed_ = 0;
				isRight_ = true;
			}

			if (boxColl_[RIGHT]->IsHit(stage_->GetboxColl(b))) {
				isMove_[RIGHT] = false;//����������E�ɐi�߂Ȃ�����
				isHovering_ = true;
				speed_ = 0;
				isRight_ = false;
			}

			if (boxColl_[UP]->IsHit(stage_->GetboxColl(b))) {
				isMove_[UP] = false;
			}

			if (boxColl_[DOWN]->IsHit(stage_->GetboxColl(b))) {
				isMove_[DOWN] = false;
			}
		}
	}

	if (!player_->GetIsEnd())
	{
		if (isHovering_)
		{
			//�v���C���[�̒ǐՔ͈́@����ɓ���������v���C���[��ǂ�
			if (detectBox_->IsHit(player_->GetPBoxColl(CENTER)))
			{
				if (player_->GetPosition().x < transform_.position_.x)
				{
					isRight_ = false;
				}
				else
				{
					isRight_ = true;
				}

				isHovering_ = false;
			}
		}

		//�v���C���[�ɓ���������HP�����炵�ăm�b�N�o�b�N
		if (boxColl_[CENTER]->IsHit(player_->GetPBoxColl(CENTER))) {
			HP* hp = ((HP*)GetParent()->FindGameObject<HP>());
			if (hp->GetIsDamage())
			{
				//�ʒu��ݒ肵�ăG�t�F�N�g�Đ�
				attackEf_.position = player_->GetPosition();
				VFX::Start(attackEf_);
				Audio::Play(hAttackSound_);//���Đ�
			}
			hp->DecreaseHP(attack_);//�v���C���[��HP�����炷
			hp->SetVibrationRangeX(HP_VIB_RANGE.x);//HP�o�[��U��������

			if (isBlow_)//�����Ă�����Ƀv���C���[���m�b�N�o�b�N
			{
				if (player_->GetPosition().x >= transform_.position_.x)
				{
					player_->SetDistandDir(pBlowDist_, RIGHT);
				}
				else
				{
					player_->SetDistandDir(pBlowDist_, LEFT);
				}
				isBlow_ = false;
			}

			player_->SetisBlinking(true);
		}
	}
}

void Bee::RotBee()
{
	if (!isRight_)
	{
		detectBox_->Setcenter(detectBoxPosL_);
		if (transform_.rotate_.y != OBJ_LEFT_ANGLE)
		{
			transform_.rotate_.y += rotSpeed_;
		}
	}
	else
	{
		detectBox_->Setcenter(detectBoxPosR_);
		if (transform_.rotate_.y != OBJ_RIGHT_ANGLE)
		{
			transform_.rotate_.y -= rotSpeed_;
		}
	}
}
