#pragma once
#include "Engine/GameObject.h"
#include "Engine/VFX.h"
#include "Share.h"
class BoxCollider;
class Enemys;
class PlayScene;
class Timer;

class Arrow :
	public GameObject
{
	////�|�C���^////
	BoxCollider* boxColl_;
	PlayScene* playscene_;
	Timer* eraseTimer_;//�~�܂��Ă��������܂ł̎���

	////�n���h��////
    //���f��
	int hArrow_;
	//��
	int hArrowHitSound_;//������������̌��ʉ�

	////csv����l���󂯎��ϐ�////
	int attack_;//�U����
	float moveSpeed_;//�ړ����x
	float gravity_;//�d��
	float eraseTime_;//�~�܂��Ă��������܂ł̕b��
	float magniDist_;//��̒��S�Ɠ����蔻��̋����̔{��
	float defaultDirPos_;
	float maxDir_;//��]���~�܂�p�x
	float rotateSpeed_;//��]���x�̔{��

	////���̑��̕ϐ�////
	float dirY_;//Y���̊p�x
	bool isMove_;
	DIR objDir_;//�I�u�W�F�N�g�̌���
	EmitterData arrowHitEf_;//��G�ɓ����������ɏo��G�t�F�N�g

	////�֐�////
	//�����蔻��
	void ArrowCollision();
public:
	Arrow(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	
	////�Z�b�^�[////
	void SetArrowDir(const bool& PisRight);
};

