#pragma once
#include "Engine/GameObject.h"
#include "Enemys.h"
#include "Share.h"
class BoxCollider;
class Player;
class Stage;
class Timer;

class Beast :
    public GameObject,public Enemys
{
	////�萔////
	static const int DETECT_BOX_NUM = 2;//�v���C���[�̌��m�͈�

	////�|�C���^////
	BoxCollider* boxColl_[ENEMY_COLL_NUM];//�����蔻��
	BoxCollider* detectBox_[DETECT_BOX_NUM];//�v���C���[�̌��m�͈�
	Player* player_;
	Stage* stage_;

	////�n���h��////
	//���f��
	int hBeast_;

	////csv����l���󂯎��ϐ�////
	int maxHP_;//�̗͍ő�l
	int attack_;//�U����
	float moveSpeed_;//�ړ����x
	float jumpHeight_;//�W�����v�̍���
	float gravity_;//�d�͉����x
	float pBlowDist_;//�v���C���[���m�b�N�o�b�N�����鋗��
	float rotSpeed_;//���E���������̉�]���x

	////���̑��̕ϐ�////
	float jumpSpeed_;//�W�����v���x
	float ground_, ceiling_;//�n��,�V��
	bool isGround_;//�n�ʂɏ���Ă��邩
	bool isLeftMove_, isRightMove_, isJump_;//���E�ړ��ƃW�����v���ł��邩�̔���
	bool isChase_[DETECT_BOX_NUM];//�v���C���[��ǂ������邩

	////�֐�////
	//�����̏���
	void MoveBeast();
	//������ς���
	void RotBeast();
public:
	Beast(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//�����蔻�� �����ɓ���������Ă΂��
	void OnCollision(GameObject* pTarget) override;

	////�Z�b�^�[////
	//�n�ʂ�ݒ�
	void SetGround(const float& ground) { ground_ = ground; }
};

