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
	//csv����f�[�^�ǂݍ���
	CsvReader csv;//�f�[�^��ǂރN���X�̃C���X�^���X���쐬
	bool ret = csv.Load("CsvData\\ArrowData.csv");
	assert(ret);
	int row = 1, column = 0;

	//�n���h���Ƀf�[�^���Z�b�g
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
		//�d�͂ŏ������p�x�ƍ��x��������
		transform_.position_.y += dirY_;
		if (transform_.rotate_.x < maxDir_)
		{
			transform_.rotate_.x -= dirY_ * rotateSpeed_;
		}
		dirY_ -= gravity_;

		//�p�x�i�x�P�ʁj�����W�A���ɕϊ�
		float angleInDegrees = transform_.rotate_.x;
		float angleInRadians = angleInDegrees * DEG_TO_RAD;

		//�x�N�g���̌v�Z
		float unitVectorX = cos(angleInRadians) * magniDist_;
		float unitVectorY = sin(angleInRadians) * magniDist_;

		//�v���C���[�̌����Ői�ޕ�����ς���
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
	//�u���b�N�Ƃ̓����蔻��
	Stage* stage = GetParent()->FindGameObject<Stage>();
	for (int b = 0; b < stage->GetBlocksSize(); b++)
	{
		if (stage->GetboxColl(b) != nullptr)//�u���b�N�̓����蔻�肪nullptr(��)����Ȃ������瓖���蔻��
		{
			if (boxColl_->IsHit(stage->GetboxColl(b))) {
				isMove_ = false;
			}
		}
	}

	//�������~�܂��ĂȂ���ΓG�Ƃ̓����蔻��
	if (isMove_)
	{
		Enemys* eColl = playscene_->GetPlayScene_enemys();
		for (int e = 0; e < eColl->GetEnemys().size(); e++)
		{
			if (eColl->GetEnemys()[e]->GetCurrentHP() > 0 && eColl->GetEnemys()[e]->GetAttackedColl() != nullptr)
			{
				//�G�̗̑͂��c���Ă邩���ɍU�����󂯂Ă��Ȃ���
				if (boxColl_->IsHit(eColl->GetEnemys()[e]->GetAttackedColl())) 
				{
					//�ʒu��ݒ肵�ăG�t�F�N�g�Đ�
					arrowHitEf_.position = transform_.position_;
					VFX::Start(arrowHitEf_);
					Audio::Play(hArrowHitSound_);//���Đ�

					eColl->GetEnemys()[e]->SetisBlinking(true);
					eColl->GetEnemys()[e]->DecreaseHP(attack_);

					KillMe();
				}
			}
		}
	}
}
