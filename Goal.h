#pragma once
#include "Engine/GameObject.h"
#include "Engine/VFX.h"
class BoxCollider;
class Timer;

class Goal :
	public GameObject
{
	////ポインタ////
	BoxCollider* boxColl_;
	Timer* timer_;

	////ハンドル////
    //モデル
	int hGoal_;
	//エフェクト
	int hSpark_;//ゴールに辿り着いた時のエフェクト
	//音
	int hGoalSound_;//ゴールに辿り着いた時の音

	////csvから値を受け取る変数////
	float gLeftAngle_,gRightAngle_;//オブジェクトの左右の向き
	float goalSec_;//ゴールに着いてからクリアシーンに遷移するまでの秒数

	////その他の変数////
	bool isfirstSpark_,isfirstGoal_;
	bool isRight_;//右向きかどうか
public:
	Goal(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override {};

	////セッター////
	void SetisRight(const bool& isRight) { isRight_ = isRight; }

	void EndEf(){ VFX::End(hSpark_); }
};