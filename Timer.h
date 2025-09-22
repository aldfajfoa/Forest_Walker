#pragma once
#include "Engine/GameObject.h"

class Timer :
    public GameObject
{
	////���̑��̕ϐ�////
	float currentTime_;//�o�ߎ��Ԃ�ێ�����ϐ�
public:
	Timer(GameObject* parent);
	void Initialize() override {};
	void Update() override {};
	void Draw() override {};
	void Release() override {};

	////�Z�b�^�[////
	//�^�C�}�[�̃��Z�b�g
	void ResetTime(const float& time) { currentTime_ = time; }

	//�w�肵���b�����o������true��Ԃ�
	bool TimeElapsed(const float& time);
	//�w�肵���b�����o������true��Ԃ�
	//����Ƀ��Z�b�g����Ȃ��^�C�}�[
	bool NoResetTimeElapsed();
};

