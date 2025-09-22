#pragma once
#include "Engine/GameObject.h"
#include "Engine/VFX.h"
#include "Share.h"
using std::pair;
class Player;
class Timer;

class HP :
    public GameObject
{
	////定数////
	const float MAX_PERCENTAGE = 100.0f;

	////ポインタ////
	Player* player_;
	Timer* isDamageTime_;//連続でダメージを受ける間隔

	////ハンドル////
	//画像
	int hHPBars_[HP_MAX];
	int hBehindHP_;
	int hHPFrame_, hPoisonFrame_;

	////csvから値を受け取る変数////
	int maxHP_;//体力最大値
	int yellowLine_, redLine_;//HPが黄色、赤になる割合
	pair<int, int> hpBarImageSize_;//HPバーの画像サイズ
	pair<int, int> hpBarSize_;//HPバーの画像サイズ(空白の分を含まない大きさ)
	float vibrationSpeed_;//振動の速さ
	float vibDecreaseSpeed_;//振動が遅くなるスピード
	float isDamageInterval_;//連続でダメージを受けられる間隔
	XMFLOAT2 hpFrameDist_;
	XMFLOAT3 poisonEfPos_;//プレイヤーを中心として毒エフェクトを出す位置
	XMFLOAT3 hpBarPos_;//HPバーの位置
	XMFLOAT3 hpFramePos_, hpFrameScale_;//HP枠の位置,大きさ

	////その他の変数////
	float currentHP_, currentPercentage_;//現在HPとその割合
	float angle_;
	float cutNum_;//画像を切り抜く範囲
	XMFLOAT2 vibrationRange_;//揺れの大きさ
	XMFLOAT2 center_;//振動の時に使う元の中心位置を持っておく変数
	bool isDamage_;//ダメージを受けるか
	HPSTATE currentState_;
	Transform frameTrans_;//HP枠のトランスフォーム
	EmitterData poisonEf_;//毒ブロックに当たってるHPバーから出るエフェクト

	////関数////
	void ChangeState();
public:
	HP(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override {};

	////セッター////
	void SetVibrationRangeX(const float& vibrationRangeX)
	{
		vibrationRange_.x = vibrationRangeX;
		angle_ = 0;
	}
	void SetVibrationRangeY(const float& vibrationRangeY)
	{
		vibrationRange_.y = vibrationRangeY;
		angle_ = 0;
	}

	void SetCurrentState(const HPSTATE& hpState) { currentState_ = hpState; }
	void SetCurrentHP(const int& currentHp) { currentHP_ = currentHp; }

	////ゲッター////
	bool GetIsDamage() { return isDamage_; }

	//HPを減らす関数
	//引数は減らしたい値
	void DecreaseHP(const int& decreaseNum) {
		if (isDamage_)
		{
			isDamage_ = false;
			currentHP_ -= decreaseNum;
		}
	}

	void PlayDamageEffect(const HPSTATE& hpState);
};

