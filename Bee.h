#pragma once
#include "Engine/GameObject.h"
#include "Enemys.h"
#include "Share.h"
class BoxCollider;
class Player;
class Nest;
class Stage;
class Timer;

class Bee :
    public GameObject, public Enemys
{
	////定数////
	static const int IS_MOVE_NUM = 4;

	////ポインタ////
	BoxCollider* boxColl_[ENEMY_COLL_NUM];//当たり判定
	BoxCollider* detectBox_;//プレイヤーの検知範囲
	Nest* nest_;
	Player* player_;
	Stage* stage_;
	Timer* blowInterval_;//プレイヤーに当たってる時にノックバックさせる間隔

	////ハンドル////
	//モデル
	int hBee_;

	////csvから値を受け取る変数////
	int maxHP_;//体力の最大値
	int attack_;//攻撃力
	float pBlowDist_;//プレイヤーをノックバックさせる距離
	float rotSpeed_;//左右を向く時の回転速度
	float acceleration_, maxSpeed_;//加速度,最大速度
	float addSizeAngleNum_;
	float blowIntervalTime_;//ノックバックさせれる間隔
	XMFLOAT3 detectBoxPosL_,detectBoxPosR_, detectBoxSize_;//プレイヤーの検知範囲

	////その他の変数////
	float sinAngle_;
	float baseY_;//生成された瞬間のY座標を持つ変数
	float speed_;//移動速度
	bool isMove_[IS_MOVE_NUM];//左右上下移動ができるか
	bool isHovering_;//浮遊状態かどうか
	bool isUp_;//上下移動の際上がってるか下がってるか
	bool isBlow_;//ノックバックさせるか

	////関数////
	//向きを変える
	void RotBee();
	//動きの処理
	void MoveBee();
public:
	Bee(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//当たり判定 何かに当たったら呼ばれる
	void OnCollision(GameObject* pTarget) override;

	////セッター////
	void SetNestPointer(Nest* nest) { nest_ = nest; }

};

