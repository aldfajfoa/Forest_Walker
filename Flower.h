#pragma once
#include "Engine/GameObject.h"
#include "Enemys.h"
#include "Share.h"
class BoxCollider;
class Player;
class Stage;
class Timer;

class Flower :
    public GameObject, public Enemys
{
	////�|�C���^////
	BoxCollider* boxColl_[ENEMY_COLL_NUM];//�r�[�X�g�����蔻��
	BoxCollider* checkScaffoldColl_;//�i�s�����ɑ��ꂪ���邩�ǂ����𒲂ׂ铖���蔻��
	Player* player_;
	Stage* stage_;

	////�n���h��////
	//���f��
	int hFlower_;

	////csv����l���󂯎��ϐ�////
	int maxHP_;//�̗͂̍ő�l
	int attack_;//�U����
	float moveSpeed_;//�ړ����x
	float gravity_;//�d�͉����x
	float pBlowDist_;//�v���C���[���m�b�N�o�b�N�����鋗��
	float rotSpeed_;//���E���������̉�]���x
	float rightAngle_,leftAngle_;//�E�����̎��̊p�x
	XMFLOAT3 checkScaffoldPosL_, checkScaffoldPosR_, checkScaffoldSize_;

	////���̑��̕ϐ�////
	float ground_;//�n��
	bool isLeftMove_, isRightMove_;

	////�֐�////
	//�����̏���
	void MoveFlower();
	//������ς���
	void RotFlower(const bool& isRight);
	//���]�����邩����
	void DetermineRot();
public:
	Flower(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//�����蔻�� �����ɓ���������Ă΂��
	void OnCollision(GameObject* pTarget) override;

	////�Z�b�^�[////
	void SetGround(const float& ground) { ground_ = ground; }
	
	////�Q�b�^�[////
	float GetFlowerDir(const bool& isRight)
	{
		if (isRight) {
			return OBJ_RIGHT_ANGLE;
		}
		else {
			return OBJ_LEFT_ANGLE;
		}
	}
};

