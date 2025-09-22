#pragma once
#include "Engine/GameObject.h"
#include "Engine/VFX.h"
class BoxCollider;
class Timer;

class Goal :
	public GameObject
{
	////�|�C���^////
	BoxCollider* boxColl_;
	Timer* timer_;

	////�n���h��////
    //���f��
	int hGoal_;
	//�G�t�F�N�g
	int hSpark_;//�S�[���ɒH�蒅�������̃G�t�F�N�g
	//��
	int hGoalSound_;//�S�[���ɒH�蒅�������̉�

	////csv����l���󂯎��ϐ�////
	float gLeftAngle_,gRightAngle_;//�I�u�W�F�N�g�̍��E�̌���
	float goalSec_;//�S�[���ɒ����Ă���N���A�V�[���ɑJ�ڂ���܂ł̕b��

	////���̑��̕ϐ�////
	bool isfirstSpark_,isfirstGoal_;
	bool isRight_;//�E�������ǂ���
public:
	Goal(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override {};

	////�Z�b�^�[////
	void SetisRight(const bool& isRight) { isRight_ = isRight; }

	void EndEf(){ VFX::End(hSpark_); }
};