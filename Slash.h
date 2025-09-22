#pragma once
#include "Engine/GameObject.h"
#include "Engine/VFX.h"
class BoxCollider;
class Player;
class Timer;

class Slash :
	public GameObject
{
	////ポインタ////
	BoxCollider* boxColl_;
	Player* player_;
	Timer* slashIntervalTime_;

	////ハンドル////
    //音
	int hSlashHitSound_;//斬撃が当たった時の効果音

	////csvから値を受け取る変数////
	int attack_;//攻撃力
	int rotateYRight_;//右向きの時のY軸角度
	int	rotateYLeft_;//左向きの時のY軸角度
	float slashInterval_;//連続で斬撃を出せる間隔
	XMFLOAT3 slashDist_;//プレイヤー中心としてどのくらい離すか(斬撃)
	XMFLOAT3 slashShineDist_;//プレイヤー中心としてどのくらい離すか(光)
	XMFLOAT3 slashBoxSize_;//当たり判定サイズ

	////その他の変数////
	bool isSlash_;
	bool isSlashCollision_;//当たり判定をするか
	EmitterData slashEf_;//近接攻撃エフェクト(斬撃)
	EmitterData shineEf_;//近接攻撃エフェクト(光)
	EmitterData slashHitEf_;//斬撃が敵に当たった時に出るエフェクト

	////関数////
	//当たり判定
	void Collision();
public:
	Slash(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override {};
	void Release() override {};
};