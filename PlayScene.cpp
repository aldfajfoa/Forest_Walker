#include "PlayScene.h"
#include "Engine/Audio.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"
#include "Arrow.h"
#include "HP.h"
#include "Player.h"
#include "Stage.h"

PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent, "PlayScene"), enemys_(nullptr),hBgm_(NULL_NUM),stageFileName_("")
{
	vines_.clear();
}

void PlayScene::Initialize()
{
	hBgm_ = Audio::Load("Sound\\BGM1.wav");
	assert(hBgm_ > NULL_NUM);

	enemys_ = new Enemys();
	Stage* pField = Instantiate<Stage>(this);
	Instantiate<Player>(this);
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	stageFileName_ = pSceneManager->GetStageFileName();
	pField->SetMap(stageFileName_);
	Instantiate<HP>(this);
}

void PlayScene::Update()
{
	//Audio::Play(hBgm_);//BGM‚ğÄ¶
}