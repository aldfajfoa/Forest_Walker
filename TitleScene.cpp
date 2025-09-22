#include "TitleScene.h"
#include "Engine/Input.h"
#include "Engine/Audio.h"
#include "Engine/CsvReader.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"

TitleScene::TitleScene(GameObject* parent)
	:GameObject(parent, "TitleScene"), stageName_(nullptr), hStageButtons_(NULL_NUM), hSelectFrame_(NULL_NUM), hSelectSound_(NULL_NUM),
	maxRow_(0), btnDefaultPosY_(0.0f), btnSpaceY_(0.0f), stageNameSize_(0.0f), btnImageSize_({ 0,0 }), btnSize_({ 0,0 }), halfBtnSize_({ 0,0 }),
	btnRow_(0), btnColumn_(0), btnLastRow_(0), currentSel_(0, 0), HalfBtnLength_(0.0f), selectFramePos_(0, 0, 0)
{
	btnPoss_.clear();
	stageNames_.clear();
}

void TitleScene::Initialize()
{
	//csvからデータ読み込み
	CsvReader csv;//データを読むクラスのインスタンスを作成
	bool ret = csv.Load("CsvData\\TitleSceneData.csv");
	assert(ret);
	int row = 1, column = 0;

	//ハンドルにデータをセット
	hImage_ = Image::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hImage_ > NULL_NUM);
	hStageButtons_ = Image::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hStageButtons_ > NULL_NUM);
	hSelectFrame_ = Image::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hSelectFrame_ > NULL_NUM);
	hSelectSound_ = Audio::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))), false, PLAY_SIMULTANEOUSLY);
	assert(hSelectSound_ > NULL_NUM);

	maxRow_ = csv.extractInt(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	btnDefaultPosY_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	btnSpaceY_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	stageNameSize_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	btnImageSize_ = csv.extractFloat2(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	btnSize_ = { btnImageSize_.x * AJM_POS_NUM,btnImageSize_.y * AJM_POS_NUM };
	halfBtnSize_ = { btnSize_.x * HALF,btnSize_.y * HALF };

	btnRow_ = STAGE_NUM;//行数
	btnColumn_ = INITIAL_COLUMN;//列数
	btnLastRow_ = maxRow_;

	if (STAGE_NUM >= maxRow_)
	{
		btnRow_ = maxRow_;
		btnColumn_ = STAGE_NUM / maxRow_;
		btnLastRow_ = STAGE_NUM % maxRow_;

		if (btnLastRow_ > 0)
		{
			btnColumn_++;//最後に余りがあるなら列を1つ追加
		}
		else
		{
			btnLastRow_ = maxRow_; //余りが0の時は、最後の列もMAX_ROW行分ある（満タン）
		}
		//配列サイズを変更
		btnPoss_.resize(maxRow_, vector<XMFLOAT3>(btnColumn_));
		stageNames_.resize(maxRow_, vector<string>(btnColumn_));
	}

	HalfBtnLength_ = (btnSize_.x * maxRow_) * HALF;//1列のボタン全体の横幅の半分

	int sNameNum = 0;
	for (int c = 0; c < btnColumn_; c++)
	{
		if (c == btnColumn_ - INDEX_AJM)
		{
			HalfBtnLength_ = (btnSize_.x * btnLastRow_) * HALF;
			for (int r = 0; r < btnLastRow_; r++)
			{
				btnPoss_[r][c] = { (float)((btnSize_.x * r) - (HalfBtnLength_ - halfBtnSize_.x)),(float)(btnDefaultPosY_ - btnSpaceY_ * (c + INDEX_AJM)),0 };
				stageNames_[r][c] = STAGE_VIEW_NAMES[sNameNum];
				sNameNum++;
			}
		}
		else
		{
			for (int r = 0; r < btnRow_; r++)
			{
				btnPoss_[r][c] = { (float)((btnSize_.x * r) - (HalfBtnLength_ - halfBtnSize_.x)),(float)(btnDefaultPosY_ - btnSpaceY_ * (c + INDEX_AJM)),0 };
				stageNames_[r][c] = STAGE_VIEW_NAMES[sNameNum];
				sNameNum++;
			}
		}
	}

	selectFramePos_ = btnPoss_[currentSel_.first][currentSel_.second];

	stageName_ = new Text;
	stageName_->Initialize();
}

void TitleScene::Update()
{
	if (Input::IsKey(DIK_P) || Input::IsPadButton(XINPUT_GAMEPAD_START))
	{
		isFadeOut_ = true;
	}

	//フェードアウトしてPlaySceneへ
	if (isFadeOut_ && alpha_ <= MAX_ALPHA)
	{
		Audio::Play(hFadeOutSound_);
		alpha_ += FADE_SPEED;
		if (alpha_ >= MAX_ALPHA)
		{
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->SetStageFileName(STAGE_FILE_NAMES.at(stageNames_[currentSel_.first][currentSel_.second]));
			pSceneManager->ChangeScene(SCENE_ID_PLAY);
		}
	}

	if (!isFadeOut_)
	{
		pair<int, int> beforeSel = { currentSel_.first, currentSel_.second };
		if (Input::IsKeyDown(DIK_LEFT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT))
		{
			currentSel_.first--;
		}
		else if (Input::IsKeyDown(DIK_RIGHT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT))
		{
			currentSel_.first++;
		}

		if (Input::IsKeyDown(DIK_UP) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_UP))
		{
			currentSel_.second--;
		}
		else if (Input::IsKeyDown(DIK_DOWN) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_DOWN))
		{
			currentSel_.second++;
		}

		if (beforeSel.first != currentSel_.first || beforeSel.second != currentSel_.second)
		{
			Audio::Play(hSelectSound_);
		}
	}

	//配列の範囲外に出ないようにする
	if (currentSel_.first < 0)
	{
		currentSel_.first = 0;
	}
	if (currentSel_.first >= btnRow_)
	{
		currentSel_.first = btnRow_ - INDEX_AJM;
	}

	if (currentSel_.second < 0)
	{
		currentSel_.second = 0;
	}
	if (currentSel_.second >= btnColumn_)
	{
		currentSel_.second = btnColumn_ - INDEX_AJM;
	}

	if(currentSel_.second == btnColumn_ - INDEX_AJM)
	{
		if (currentSel_.first >= btnLastRow_)
		{
			currentSel_.first = btnLastRow_ - INDEX_AJM;
		}
	}

	selectFramePos_ = btnPoss_[currentSel_.first][currentSel_.second];
}

void TitleScene::Draw()
{
	Image::SetTransform(hImage_, transform_);
	Image::Draw(hImage_);

	HalfBtnLength_ = (btnSize_.x * STAGE_NUM) * HALF;//1列のボタン全体の横幅の半分

	Transform btnPos;
	int sNameNum = 0;
	for (int c = 0; c < btnColumn_; c++)
	{
		for (int r = 0; r < btnRow_; r++)
		{
			if (!(btnPoss_[r][c] == ORIZIN))//btnPoss_が未設定(初期値 ORIZIN)じゃなければ描画
			{
				btnPos.position_ = btnPoss_[r][c];
				Image::SetTransform(hStageButtons_, btnPos);
				Image::Draw(hStageButtons_);

				//ステージ名の位置を設定
				float nameAdjustX = (stageName_->GetTextWidth() * STAGE_VIEW_NAMES[sNameNum].length()) * HALF;//ボタンの真ん中に表示するための調整
				XMFLOAT2 namePos = { ((btnPoss_[r][c].x + IMAGE_AXIS_MAX) * HALF * WINDOW_WIDTH) - nameAdjustX,
									 ((IMAGE_AXIS_MAX - btnPoss_[r][c].y) * HALF * WINDOW_HEIGHT) };//IMAGE_AXIS_MAXを使って座標を反転
				stageName_->Draw(namePos.x, namePos.y, STAGE_VIEW_NAMES[sNameNum].c_str(), stageNameSize_);//ステージ名を表示
				sNameNum++;
			}
		}
	}

	Transform selectPos;
	selectPos.position_ = selectFramePos_;
	Image::SetTransform(hSelectFrame_, selectPos);
	Image::Draw(hSelectFrame_);

	Image::SetAlpha(hFade_, alpha_);
	Image::SetTransform(hFade_, transform_);
	Image::Draw(hFade_);
}