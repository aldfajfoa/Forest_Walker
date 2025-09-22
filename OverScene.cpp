#include "OverScene.h"
#include "Engine/Input.h"
#include "Engine/Audio.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"
#include "Share.h"

OverScene::OverScene(GameObject* parent)
	:GameObject(parent, "OverScene")
{
}

void OverScene::Initialize()
{
	hImage_ = Image::Load("Image\\OverScene.png");
	assert(hImage_ > NULL_NUM);

	hFadeOutSound_ = Audio::Load("Sound\\FadeOutTitle.wav");
	assert(hFadeOutSound_ > NULL_NUM);
}

void OverScene::Update()
{
	if (Input::IsKey(DIK_T) || Input::IsPadButton(XINPUT_GAMEPAD_BACK))
	{
		isFadeOut_ = true;
	}

	if (isFadeOut_ && alpha_ <= MAX_ALPHA)
	{
		Audio::Play(hFadeOutSound_);
		alpha_ += FADE_SPEED;
		if (alpha_ >= MAX_ALPHA)
		{
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_TITLE);
		}
	}
}

void OverScene::Draw()
{
	Image::SetTransform(hImage_, transform_);
	Image::Draw(hImage_);

	Image::SetAlpha(hFade_, alpha_);
	Image::SetTransform(hFade_, transform_);
	Image::Draw(hFade_);
}
