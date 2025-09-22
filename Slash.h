#pragma once
#include "Engine/GameObject.h"
#include "Engine/VFX.h"
class BoxCollider;
class Player;
class Timer;

class Slash :
	public GameObject
{
	////�|�C���^////
	BoxCollider* boxColl_;
	Player* player_;
	Timer* slashIntervalTime_;

	////�n���h��////
    //��
	int hSlashHitSound_;//�a���������������̌��ʉ�

	////csv����l���󂯎��ϐ�////
	int attack_;//�U����
	int rotateYRight_;//�E�����̎���Y���p�x
	int	rotateYLeft_;//�������̎���Y���p�x
	float slashInterval_;//�A���Ŏa�����o����Ԋu
	XMFLOAT3 slashDist_;//�v���C���[���S�Ƃ��Ăǂ̂��炢������(�a��)
	XMFLOAT3 slashShineDist_;//�v���C���[���S�Ƃ��Ăǂ̂��炢������(��)
	XMFLOAT3 slashBoxSize_;//�����蔻��T�C�Y

	////���̑��̕ϐ�////
	bool isSlash_;
	bool isSlashCollision_;//�����蔻������邩
	EmitterData slashEf_;//�ߐڍU���G�t�F�N�g(�a��)
	EmitterData shineEf_;//�ߐڍU���G�t�F�N�g(��)
	EmitterData slashHitEf_;//�a�����G�ɓ����������ɏo��G�t�F�N�g

	////�֐�////
	//�����蔻��
	void Collision();
public:
	Slash(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override {};
	void Release() override {};
};