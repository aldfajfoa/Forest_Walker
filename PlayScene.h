#pragma once
#include "Engine/GameObject.h"
#include "Enemys.h"
class Arrow;
class Vine;

class PlayScene :
    public GameObject
{
	////ポインタ////
	Enemys* enemys_;
	vector<Vine*> vines_;

	////ハンドル////
    //音
	int hBgm_;//ステージBGMのハンドル

	////その他の変数////
	string stageFileName_;//Stageに渡すステージファイル名

public:
	PlayScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override {};
	void Release() override {};

	////セッター////
	void SetStageFileName(const string& stageFileName) { stageFileName_ = stageFileName; }

	////ゲッター////
	Enemys* GetPlayScene_enemys() { return enemys_; }
	vector<Vine*> Getvines() { return vines_; }

	////その他////
	void AddVine(Vine* vine) { vines_.push_back(vine); }
};

