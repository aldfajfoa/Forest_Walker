#pragma once
#include "Engine/GameObject.h"
#include "Enemys.h"
#include "Share.h"
class BoxCollider;
class Player;
class Stage;
class Timer;

class Flower :
    public GameObject, public Enemys
{
	////ポインタ////
	BoxCollider* boxColl_[ENEMY_COLL_NUM];//ビースト当たり判定
	BoxCollider* checkScaffoldColl_;//進行方向に足場があるかどうかを調べる当たり判定
	Player* player_;
	Stage* stage_;

	////ハンドル////
	//モデル
	int hFlower_;

	////csvから値を受け取る変数////
	int maxHP_;//体力の最大値
	int attack_;//攻撃力
	float moveSpeed_;//移動速度
	float gravity_;//重力加速度
	float pBlowDist_;//プレイヤーをノックバックさせる距離
	float rotSpeed_;//左右を向く時の回転速度
	float rightAngle_,leftAngle_;//右向きの時の角度
	XMFLOAT3 checkScaffoldPosL_, checkScaffoldPosR_, checkScaffoldSize_;

	////その他の変数////
	float ground_;//地面
	bool isLeftMove_, isRightMove_;

	////関数////
	//動きの処理
	void MoveFlower();
	//向きを変える
	void RotFlower(const bool& isRight);
	//反転させるか判定
	void DetermineRot();
public:
	Flower(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//当たり判定 何かに当たったら呼ばれる
	void OnCollision(GameObject* pTarget) override;

	////セッター////
	void SetGround(const float& ground) { ground_ = ground; }
	
	////ゲッター////
	float GetFlowerDir(const bool& isRight)
	{
		if (isRight) {
			return OBJ_RIGHT_ANGLE;
		}
		else {
			return OBJ_LEFT_ANGLE;
		}
	}
};

