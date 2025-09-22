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
	//csv����f�[�^�ǂݍ���
	CsvReader csv;//�f�[�^��ǂރN���X�̃C���X�^���X���쐬
	bool ret = csv.Load("CsvData\\Chara\\FlowerData.csv");
	assert(ret);
	int row = 1, column = 0;

	//�n���h���Ƀf�[�^���Z�b�g
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
	leftAngle_ = 360 - rightAngle_;//�������p�x*/

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

	if (currentHP_ <= HP_DEAD_LINE)//HP_DEAD_LINE�ȉ��ɂȂ�����폜
	{
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

	if (!player_->GetIsEnd())
	{
		MoveFlower();
		DetermineRot();
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
	//�u���b�N�Ƃ̓����蔻��
	for (int b = 0; b < stage_->GetBlocksSize(); b++)
	{
		if (stage_->GetboxColl(b) != nullptr)//�u���b�N�̓����蔻�肪nullptr(��)����Ȃ������瓖���蔻��
		{
			if (boxColl_[LEFT]->IsHit(stage_->GetboxColl(b))) {
				isLeftMove_ = false;//���������獶�ɐi�߂Ȃ�����
				isRight_ = true;
			}

			if (boxColl_[RIGHT]->IsHit(stage_->GetboxColl(b))) {
				isRightMove_ = false;//����������E�ɐi�߂Ȃ�����
				isRight_ = false;
			}

			if (boxColl_[DOWN]->IsHit(stage_->GetboxColl(b))) {
				//ground_ = stage_->GetboxColl(b)->Getcenter().y + DEFAULT_BLOCK_SIZE.y;//�n�ʂɏ悹��
				float dist = (abs)(boxColl_[DOWN]->Getcenter().y);//�u���b�N����ǂ̂��炢������(�v���C���[�̒��S���瓖���蔻��̒��S�̋�����)
				ground_ = (stage_->GetboxColl(b)->Getcenter().y + (stage_->GetboxColl(b)->Getsize().y * HALF)) + (dist);//�n�ʂɏ悹��
			}
		}
	}

	if (!player_->GetIsEnd())
	{
		if (boxColl_[CENTER]->IsHit(player_->GetPBoxColl(CENTER))) {
			HP* hp = ((HP*)GetParent()->GetParent()->FindGameObject<HP>());
			if (hp->GetIsDamage())
			{
				//�ʒu��ݒ肵�ăG�t�F�N�g�Đ�
				attackEf_.position = player_->GetPosition();
				VFX::Start(attackEf_);
				Audio::Play(hAttackSound_);//���Đ�
			}
			hp->DecreaseHP(attack_);//�v���C���[��HP�����炷
			hp->SetVibrationRangeX(HP_VIB_RANGE.x);//HP�o�[��U��������

			//�����Ă�����Ƀv���C���[���m�b�N�o�b�N
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
	//�i�s�����ɑ��ꂪ����������i�s�����𔽓]
	int b = 0;
	for (b = 0; b < stage_->GetBlocksSize(); b++)
	{
		if ((checkScaffoldColl_->IsHit(stage_->GetboxColl(b)))) {//�ǂ����ɓ����������_�Ń��[�v�𒆒f
			break;
		}
	}

	if (!(b < stage_->GetBlocksSize()))//���[�v�̍Ō�܂łǂ��Ƃ��������Ė��������甽�]
	{
		isRight_ = !isRight_;
	}
}

void Flower::MoveFlower()
{
	if (isLeftMove_ && isRight_ == false)//���֐i��
	{
		transform_.position_.x -= moveSpeed_;
	}
	if (isRightMove_ && isRight_)//�E�֐i��
	{
		transform_.position_.x += moveSpeed_;
	}

	transform_.position_.y -= gravity_;

	RotFlower(isRight_);
}
