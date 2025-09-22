#pragma once
#include "Engine/GameObject.h"
#include "Enemys.h"
#include "Share.h"
class BoxCollider;
class Player;
class Stage;
class Timer;

class Beast :
    public GameObject,public Enemys
{
	////定数////
	static const int DETECT_BOX_NUM = 2;//プレイヤーの検知範囲

	////ポインタ////
	BoxCollider* boxColl_[ENEMY_COLL_NUM];//当たり判定
	BoxCollider* detectBox_[DETECT_BOX_NUM];//プレイヤーの検知範囲
	Player* player_;
	Stage* stage_;

	////ハンドル////
	//モデル
	int hBeast_;

	////csvから値を受け取る変数////
	int maxHP_;//体力最大値
	int attack_;//攻撃力
	float moveSpeed_;//移動速度
	float jumpHeight_;//ジャンプの高さ
	float gravity_;//重力加速度
	float pBlowDist_;//プレイヤーをノックバックさせる距離
	float rotSpeed_;//左右を向く時の回転速度

	////その他の変数////
	float jumpSpeed_;//ジャンプ速度
	float ground_, ceiling_;//地面,天井
	bool isGround_;//地面に乗っているか
	bool isLeftMove_, isRightMove_, isJump_;//左右移動とジャンプができるかの判定
	bool isChase_[DETECT_BOX_NUM];//プレイヤーを追いかけるか

	////関数////
	//動きの処理
	void MoveBeast();
	//向きを変える
	void RotBeast();
public:
	Beast(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//当たり判定 何かに当たったら呼ばれる
	void OnCollision(GameObject* pTarget) override;

	////セッター////
	//地面を設定
	void SetGround(const float& ground) { ground_ = ground; }
};

