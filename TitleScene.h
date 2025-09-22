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
	////定数////
	const int INITIAL_COLUMN = 1;//初期列数
	const int PLAY_SIMULTANEOUSLY = 2;//同時に選択音を鳴らせる数
	const float AJM_POS_NUM = 0.0015f;
	const float IMAGE_AXIS_MAX = 1.0f;//軸の最大値

	////ポインタ////
	Text* stageName_;//ステージ名を表示

	////ハンドル////
    //画像
	int hStageButtons_;//ステージのボタン
	int hSelectFrame_;//選択されているボタンの上から表示する枠
	//音
	int hSelectSound_;//ステージを選択するときの音

	////csvから値を受け取る変数////
	int maxRow_;//ステージ一覧を表示するときの列数
	float btnDefaultPosY_;
	float btnSpaceY_;//Y座標のボタン同士の隙間
	float stageNameSize_;//ステージ名の文字サイズ
	XMFLOAT2 btnImageSize_;//ボタンの画像サイズ
	XMFLOAT2 btnSize_, halfBtnSize_;//ボタンのサイズ

	////その他の変数////
	int btnRow_, btnColumn_;//行数、列数
	int btnLastRow_;//ボタン配列の最後の行数(MAX_ROWで割り切れない数の場合最後の行を均等に配置するのに必要)
	pair<int, int> currentSel_;//現在選択されてるボタン
	float HalfBtnLength_;//1列のボタン全体の横幅の半分
	XMFLOAT3 selectFramePos_;
	vector<vector<XMFLOAT3>> btnPoss_;//ボタンの位置
	vector<vector<string>> stageNames_;//表示するステージの名前
public:
	TitleScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override {};
};