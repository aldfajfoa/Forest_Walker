#pragma once
#include "Engine/GameObject.h"
#include "Engine/VFX.h"
#include "Share.h"
class BoxCollider;
class Enemys;
class PlayScene;
class Timer;

class Arrow :
	public GameObject
{
	////ポインタ////
	BoxCollider* boxColl_;
	PlayScene* playscene_;
	Timer* eraseTimer_;//止まってから消えるまでの時間

	////ハンドル////
    //モデル
	int hArrow_;
	//音
	int hArrowHitSound_;//矢が当たった時の効果音

	////csvから値を受け取る変数////
	int attack_;//攻撃力
	float moveSpeed_;//移動速度
	float gravity_;//重力
	float eraseTime_;//止まってから消えるまでの秒数
	float magniDist_;//矢の中心と当たり判定の距離の倍率
	float defaultDirPos_;
	float maxDir_;//回転が止まる角度
	float rotateSpeed_;//回転速度の倍率

	////その他の変数////
	float dirY_;//Y軸の角度
	bool isMove_;
	DIR objDir_;//オブジェクトの向き
	EmitterData arrowHitEf_;//矢が敵に当たった時に出るエフェクト

	////関数////
	//当たり判定
	void ArrowCollision();
public:
	Arrow(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	
	////セッター////
	void SetArrowDir(const bool& PisRight);
};

