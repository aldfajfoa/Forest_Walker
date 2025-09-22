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
	//csv����f�[�^�ǂݍ���
	CsvReader csv;//�f�[�^��ǂރN���X�̃C���X�^���X���쐬
	bool ret = csv.Load("CsvData\\Chara\\BeastData.csv");
	assert(ret);
	int row = 1, column = 0;

	//�n���h���Ƀf�[�^���Z�b�g
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

	//���m�͈͂̐ݒ�
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
		MoveBeast();
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

	//Y�ʒu���V��𒴂��Ă��牺����
	if (transform_.position_.y >= ceiling_) {
		transform_.position_.y = ceiling_;
		jumpSpeed_ = 0.0f;
		isJump_ = false;
	}

	//Y�ʒu���n�ʂ�艺�Ȃ�グ��
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
	//�����蔻��̐ݒ�
	for (int i = 0; i < ENEMY_COLL_NUM; i++)
	{
		collPos[i] = boxColl_[i]->Getcenter();
		collSize[i] = boxColl_[i]->Getsize();
	}

	//�u���b�N�̓����蔻��
	for (int b = 0; b < stage_->GetBlocksSize(); b++)
	{
		if (stage_->GetboxColl(b) != nullptr)//�u���b�N�̓����蔻�肪nullptr(��)����Ȃ������瓖���蔻��
		{
			if (boxColl_[LEFT]->IsHit(stage_->GetboxColl(b))) {
				isLeftMove_ = false;//���������獶�ɐi�߂Ȃ�����
				if (isGround_)
				{
					float dist = (abs)(collPos[LEFT].x) + (abs)(collPos[LEFT].x * HALF);//�u���b�N����ǂ̂��炢������(�v���C���[�̒��S���瓖���蔻��̒��S�̋�����)
					transform_.position_.x = (stage_->GetboxColl(b)->Getcenter().x + (stage_->GetboxColl(b)->Getsize().x * HALF)) + dist;
					jumpSpeed_ = -sqrtf(gravity_ * jumpHeight_);//�W�����v������
					isGround_ = false;
				}
			}

			if (boxColl_[RIGHT]->IsHit(stage_->GetboxColl(b))) {
				isRightMove_ = false;//����������E�ɐi�߂Ȃ�����
				if (isGround_)
				{
					float dist = (abs)(collPos[RIGHT].x) + (abs)(collSize[RIGHT].x * HALF);//�u���b�N����ǂ̂��炢������(�v���C���[�̒��S���瓖���蔻��̒��S�̋�����)
					transform_.position_.x = (stage_->GetboxColl(b)->Getcenter().x - (stage_->GetboxColl(b)->Getsize().x * HALF)) - dist;
					jumpSpeed_ = -sqrtf(gravity_ * jumpHeight_);//�W�����v������
					isGround_ = false;
				}
			}

			if (isJump_) {
				if (boxColl_[UP]->IsHit(stage_->GetboxColl(b))) {
					jumpSpeed_ = 0;//����������W�����v���~�߂�
					float dist = (abs)(collPos[UP].y);//�u���b�N����ǂ̂��炢������(�v���C���[�̒��S���瓖���蔻��̒��S�̋�����)
					ceiling_ = (stage_->GetboxColl(b)->Getcenter().y - (stage_->GetboxColl(b)->Getsize().y * HALF)) - (dist);
					isJump_ = false;
				}
			}

			if (boxColl_[DOWN]->IsHit(stage_->GetboxColl(b))) {
				float dist = (abs)(collPos[DOWN].y);//�u���b�N����ǂ̂��炢������(�v���C���[�̒��S���瓖���蔻��̒��S�̋�����)
				ground_ = (stage_->GetboxColl(b)->Getcenter().y + (stage_->GetboxColl(b)->Getsize().y * HALF)) + (dist);//�n�ʂɏ悹��
				isGround_ = true;
			}
		}
	}

	if (!player_->GetIsEnd())
	{
		for (int i = 0; i < DETECT_BOX_NUM; i++)
		{
			//�v���C���[�̒ǐՔ͈́@����ɓ���������v���C���[��ǂ�
			if (detectBox_[i]->IsHit(player_->GetPBoxColl(CENTER)))
			{
				isChase_[i] = true;
			}
			else
			{
				isChase_[i] = false;
			}
		}

		//�v���C���[�ɓ���������HP�����炵�ăm�b�N�o�b�N
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

void Beast::MoveBeast()
{
	for (int i = 0; i < DETECT_BOX_NUM; i++)
	{
		if (isChase_[i] == true)//isChase_�̂ǂꂩ��ł�true��������ǐՂ���
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