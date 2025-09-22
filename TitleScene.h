#pragma once
#include "Engine/GameObject.h"
#include "Engine/SceneManager.h"
#include "Engine/Text.h"
#include "Scene.h"
#include "Share.h"
#include <string>
using std::string;
#include <vector>
using std::vector;
using std::pair;

class TitleScene :
	public GameObject,public Scene
{
	////�萔////
	const int INITIAL_COLUMN = 1;//������
	const int PLAY_SIMULTANEOUSLY = 2;//�����ɑI������点�鐔
	const float AJM_POS_NUM = 0.0015f;
	const float IMAGE_AXIS_MAX = 1.0f;//���̍ő�l

	////�|�C���^////
	Text* stageName_;//�X�e�[�W����\��

	////�n���h��////
    //�摜
	int hStageButtons_;//�X�e�[�W�̃{�^��
	int hSelectFrame_;//�I������Ă���{�^���̏ォ��\������g
	//��
	int hSelectSound_;//�X�e�[�W��I������Ƃ��̉�

	////csv����l���󂯎��ϐ�////
	int maxRow_;//�X�e�[�W�ꗗ��\������Ƃ��̗�
	float btnDefaultPosY_;
	float btnSpaceY_;//Y���W�̃{�^�����m�̌���
	float stageNameSize_;//�X�e�[�W���̕����T�C�Y
	XMFLOAT2 btnImageSize_;//�{�^���̉摜�T�C�Y
	XMFLOAT2 btnSize_, halfBtnSize_;//�{�^���̃T�C�Y

	////���̑��̕ϐ�////
	int btnRow_, btnColumn_;//�s���A��
	int btnLastRow_;//�{�^���z��̍Ō�̍s��(MAX_ROW�Ŋ���؂�Ȃ����̏ꍇ�Ō�̍s���ϓ��ɔz�u����̂ɕK�v)
	pair<int, int> currentSel_;//���ݑI������Ă�{�^��
	float HalfBtnLength_;//1��̃{�^���S�̂̉����̔���
	XMFLOAT3 selectFramePos_;
	vector<vector<XMFLOAT3>> btnPoss_;//�{�^���̈ʒu
	vector<vector<string>> stageNames_;//�\������X�e�[�W�̖��O
public:
	TitleScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override {};
};