#pragma once
#include "Engine/GameObject.h"
#include "Engine/CsvReader.h"
#include "Engine/VFX.h"
#include "Stage.h"
#include "Share.h"
class BoxCollider;
class Player;
class Stage;
class Timer;

class Vine :
	public GameObject
{
	////定数////
	const int DEFAULT_VINE_NUM = 1;//Vines_の初期値

	////ポインタ////
	BoxCollider* vineColl_;//ツタ本体の当たり判定
	BoxCollider* vTipColl_;//ツタ先端の当たり判定
	BoxCollider* vBlockColl_;//ブロックとの当たり判定
	Player* player_;
	Stage* stage_;
	Timer* timer_;

	////ハンドル////
	//モデル
	int hVine_;//伸びる分のツル
	int hTipVine_;//ツタの先端
	//音
	int hSpinyHitSound_;//トゲ付きツタがプレイヤーに当たった時の効果音

	////csvから値を受け取る変数////
	int attack_;//攻撃力
	float moveSpeed_;//動くスピード
	float pBlowDist_;//プレイヤーをノックバックさせる距離
	float vineTimer_;
	XMFLOAT2 vineSize_;//ツタ一つのサイズ
	XMFLOAT3 vCollPos_, vCollSize_;//ツタ本体の当たり判定の位置,サイズ
	XMFLOAT3 vTipCollPos_[V_MAX], vTipCollSize_[V_MAX];//ツタ先端の当たり判定の位置,サイズ
	XMFLOAT3 vBlockCollPos_, vBlockCollSize_;//ブロックとの当たり判定の位置,サイズ


	////その他の変数 ////
	int vines_;//ツタの数
	int addVinesLine_;
	int vineCollTall_;//当たり判定Boxの高さ
	float moveNum_;//移動量
	bool isGrow_;//ツルが上がっているか下がっているか
	bool isMove_;//動いているか
	bool isPCollision_;//プレイヤーと当たり判定するか
	VINETYPE vType_;//ツタの種類
	DIR dir_;//向き
	EmitterData spinyHitEf_;//トゲがプレイヤーに当たった時に出るエフェクト
	CsvReader csv;//データを読むクラスのインスタンス

	////関数////
	void GrowVine(const bool& isGrow);
public:
	Vine(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override {};

	//当たり判定
	void Collision();

	////セッター////
	void SetvDir(const DIR& dir);
	void SetvType(const VINETYPE& vType);

	////ゲッター////
	bool GetisPCollision() { return isPCollision_; }
	BoxCollider* GetvTipColl() { return vTipColl_; }
	BoxCollider* GetvineColl() { return vineColl_; }
	XMFLOAT2 GetVINE_SIZE() { return vineSize_; }
};

