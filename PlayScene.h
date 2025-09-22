#pragma once
#include "Engine/GameObject.h"
#include "Enemys.h"
class Arrow;
class Vine;

class PlayScene :
    public GameObject
{
	////�|�C���^////
	Enemys* enemys_;
	vector<Vine*> vines_;

	////�n���h��////
    //��
	int hBgm_;//�X�e�[�WBGM�̃n���h��

	////���̑��̕ϐ�////
	string stageFileName_;//Stage�ɓn���X�e�[�W�t�@�C����

public:
	PlayScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override {};
	void Release() override {};

	////�Z�b�^�[////
	void SetStageFileName(const string& stageFileName) { stageFileName_ = stageFileName; }

	////�Q�b�^�[////
	Enemys* GetPlayScene_enemys() { return enemys_; }
	vector<Vine*> Getvines() { return vines_; }

	////���̑�////
	void AddVine(Vine* vine) { vines_.push_back(vine); }
};

