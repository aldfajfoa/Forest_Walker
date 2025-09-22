#pragma once
#include "Engine/Audio.h"
#include "Engine/Image.h"
#include "Share.h"
#include "assert.h"

class Scene
{
protected:
	////�萔////
	const int FADE_SPEED = 3;//�t�F�[�h�A�E�g���鑬�x

	////�n���h��////
	//�摜
	int hImage_ = NULL_NUM;
	int hFade_ = NULL_NUM;
	//��
	int hFadeOutSound_ = NULL_NUM;//�t�F�[�h�A�E�g���鎞�̉�

	////���̑��̕ϐ�////
	int alpha_ = 0;//�����x
	bool isFadeOut_ = false;//�t�F�[�h�A�E�g���邩�ǂ���

public:
	Scene() {
		hFade_ = Image::Load("Image\\Black.png");
		assert(hFade_ > NULL_NUM);

		hFadeOutSound_ = Audio::Load("Sound\\FadeOutStart.wav");
		assert(hFadeOutSound_ > NULL_NUM);
	}
};

