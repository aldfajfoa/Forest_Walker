#pragma once
#include "Engine/GameObject.h"
#include "Enemys.h"
#include "Share.h"
class BoxCollider;
class Player;
class Nest;
class Stage;
class Timer;

class Bee :
    public GameObject, public Enemys
{
	////�萔////
	static const int IS_MOVE_NUM = 4;

	////�|�C���^////
	BoxCollider* boxColl_[ENEMY_COLL_NUM];//�����蔻��
	BoxCollider* detectBox_;//�v���C���[�̌��m�͈�
	Nest* nest_;
	Player* player_;
	Stage* stage_;
	Timer* blowInterval_;//�v���C���[�ɓ������Ă鎞�Ƀm�b�N�o�b�N������Ԋu

	////�n���h��////
	//���f��
	int hBee_;

	////csv����l���󂯎��ϐ�////
	int maxHP_;//�̗͂̍ő�l
	int attack_;//�U����
	float pBlowDist_;//�v���C���[���m�b�N�o�b�N�����鋗��
	float rotSpeed_;//���E���������̉�]���x
	float acceleration_, maxSpeed_;//�����x,�ő呬�x
	float addSizeAngleNum_;
	float blowIntervalTime_;//�m�b�N�o�b�N�������Ԋu
	XMFLOAT3 detectBoxPosL_,detectBoxPosR_, detectBoxSize_;//�v���C���[�̌��m�͈�

	////���̑��̕ϐ�////
	float sinAngle_;
	float baseY_;//�������ꂽ�u�Ԃ�Y���W�����ϐ�
	float speed_;//�ړ����x
	bool isMove_[IS_MOVE_NUM];//���E�㉺�ړ����ł��邩
	bool isHovering_;//���V��Ԃ��ǂ���
	bool isUp_;//�㉺�ړ��̍ۏオ���Ă邩�������Ă邩
	bool isBlow_;//�m�b�N�o�b�N�����邩

	////�֐�////
	//������ς���
	void RotBee();
	//�����̏���
	void MoveBee();
public:
	Bee(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//�����蔻�� �����ɓ���������Ă΂��
	void OnCollision(GameObject* pTarget) override;

	////�Z�b�^�[////
	void SetNestPointer(Nest* nest) { nest_ = nest; }

};

