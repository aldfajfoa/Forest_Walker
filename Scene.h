#pragma once
#include "Engine/Audio.h"
#include "Engine/Image.h"
#include "Share.h"
#include "assert.h"

class Scene
{
protected:
	////定数////
	const int FADE_SPEED = 3;//フェードアウトする速度

	////ハンドル////
	//画像
	int hImage_ = NULL_NUM;
	int hFade_ = NULL_NUM;
	//音
	int hFadeOutSound_ = NULL_NUM;//フェードアウトする時の音

	////その他の変数////
	int alpha_ = 0;//透明度
	bool isFadeOut_ = false;//フェードアウトするかどうか

public:
	Scene() {
		hFade_ = Image::Load("Image\\Black.png");
		assert(hFade_ > NULL_NUM);

		hFadeOutSound_ = Audio::Load("Sound\\FadeOutStart.wav");
		assert(hFadeOutSound_ > NULL_NUM);
	}
};

