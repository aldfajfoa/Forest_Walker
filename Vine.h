#pragma once
#include "Engine/GameObject.h"
#include "Engine/CsvReader.h"
#include "Engine/VFX.h"
#include "Stage.h"
#include "Share.h"
class BoxCollider;
class Player;
class Stage;
class Timer;

class Vine :
	public GameObject
{
	////�萔////
	const int DEFAULT_VINE_NUM = 1;//Vines_�̏����l

	////�|�C���^////
	BoxCollider* vineColl_;//�c�^�{�̂̓����蔻��
	BoxCollider* vTipColl_;//�c�^��[�̓����蔻��
	BoxCollider* vBlockColl_;//�u���b�N�Ƃ̓����蔻��
	Player* player_;
	Stage* stage_;
	Timer* timer_;

	////�n���h��////
	//���f��
	int hVine_;//�L�т镪�̃c��
	int hTipVine_;//�c�^�̐�[
	//��
	int hSpinyHitSound_;//�g�Q�t���c�^���v���C���[�ɓ����������̌��ʉ�

	////csv����l���󂯎��ϐ�////
	int attack_;//�U����
	float moveSpeed_;//�����X�s�[�h
	float pBlowDist_;//�v���C���[���m�b�N�o�b�N�����鋗��
	float vineTimer_;
	XMFLOAT2 vineSize_;//�c�^��̃T�C�Y
	XMFLOAT3 vCollPos_, vCollSize_;//�c�^�{�̂̓����蔻��̈ʒu,�T�C�Y
	XMFLOAT3 vTipCollPos_[V_MAX], vTipCollSize_[V_MAX];//�c�^��[�̓����蔻��̈ʒu,�T�C�Y
	XMFLOAT3 vBlockCollPos_, vBlockCollSize_;//�u���b�N�Ƃ̓����蔻��̈ʒu,�T�C�Y


	////���̑��̕ϐ� ////
	int vines_;//�c�^�̐�
	int addVinesLine_;
	int vineCollTall_;//�����蔻��Box�̍���
	float moveNum_;//�ړ���
	bool isGrow_;//�c�����オ���Ă��邩�������Ă��邩
	bool isMove_;//�����Ă��邩
	bool isPCollision_;//�v���C���[�Ɠ����蔻�肷�邩
	VINETYPE vType_;//�c�^�̎��
	DIR dir_;//����
	EmitterData spinyHitEf_;//�g�Q���v���C���[�ɓ����������ɏo��G�t�F�N�g
	CsvReader csv;//�f�[�^��ǂރN���X�̃C���X�^���X

	////�֐�////
	void GrowVine(const bool& isGrow);
public:
	Vine(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override {};

	//�����蔻��
	void Collision();

	////�Z�b�^�[////
	void SetvDir(const DIR& dir);
	void SetvType(const VINETYPE& vType);

	////�Q�b�^�[////
	bool GetisPCollision() { return isPCollision_; }
	BoxCollider* GetvTipColl() { return vTipColl_; }
	BoxCollider* GetvineColl() { return vineColl_; }
	XMFLOAT2 GetVINE_SIZE() { return vineSize_; }
};

