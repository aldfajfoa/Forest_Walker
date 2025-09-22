#pragma once
#include "Engine/GameObject.h"
#include "Engine/Transform.h"
#include "Engine/VFX.h"
#include "Share.h"
class BoxCollider;
class PlayScene;
class Stage;
class Timer;

class Player :
	public GameObject
{
	////定数////
	static const int BROCK_HIT_NUM = 4;
	static const int PBOX_NUM = DIR::DIR_MAX;//本体当たり判定の数
	
	////ポインタ////
	BoxCollider* boxColl_[PBOX_NUM];
	PlayScene* playScene_;
	Stage* stage_;
	Timer* bDamageTime_;//ブロックによってダメージを受ける間隔
	Timer* arrowIntervalTime_;
	Timer* blinkTime_;
	Timer* gameOverStopTime_;//ゲームオーバーになった際一瞬固まる時間
	Timer* gameOverChangeTime_;//ゲームオーバーになってからGameOverSceneへ遷移するまでの時間

	////ハンドル////
    //モデル
	int hPlayer_;
	//画像
	int hBlack_;//ゲームオーバーの際の暗転
	//音
	int hPoisonSound_;//毒を受けた時の音
	int hDeadSound_, hDeadFallSound_;//死んだ瞬間の音と落ちていくときの音

	////csvから値を受け取る変数////
	float moveSpeed_;//移動速度
	float gravity_;//重力加速度
	float rotSpeed_;//左右を向く時の回転速度
	int blackAlpha_;//ゲームオーバー時の暗転の透明度
	float deathGroundDist_;//落ちたら死ぬ座標をカメラが止まる座標からどのくらい離すか
	float blowSpeed_, blowJumpHeight_;//吹っ飛ばされる時の速度とジャンプの高さ
	float arrowInterval_;//矢を連続で撃てる間隔
	float maxJumpHoldTime_;//長押しでジャンプが高くなる最大押し時間
	float initialJumpSpeed_;//最初のジャンプ速度
	float jumpHoldBoost_;//押し続けた場合の追加ジャンプ速度
	float frameTime_;//フレームごとの時間経過
	float brockDamageTime_;//ブロックによるダメージを受ける間隔
	float deadJumpHeight_;//ゲームオーバー演出でのジャンプの高さ
	float gameOverPosZ_;//ゲームオーバになったらこの数値分引いて他のオブジェクトより前に出す
	float gameOverRotSpeed_;//ゲームオーバー演出でのプレイヤー回転速度
	float playStopTime_;//ゲームオーバーになった際一瞬固まる時間、これが過ぎたら演出が始まる
	float gameOverTime_;//ゲームオーバー演出の時間
	XMFLOAT3 cameraDistTarget_;
	XMFLOAT3 cameraDistPos_;


	////その他の変数////
	float jumpSpeed_, jumpHeight_;//ジャンプの速度、高さ
	float ground_, ceiling_;//地面,天井
	float cameraStopGround_;//これより下にいくとカメラがプレイヤーを追わなくなる
	float deathGround_;//ここまで落ちたらゲームオーバー
	float dist_;
	bool isDraw_;//描画するかどうか
	bool isRight_;//右向きかどうか
	bool isGround_,isCeiling_;//地面に乗っているか
	bool isJump_;//ジャンプができるか
	bool isPdamage_[BROCK_HIT_NUM];//このどれかがtrueの間はダメージを受ける
	bool isBrockHit_[BROCK_HIT_NUM];//ブロックと当たっているか
	bool isVineHit_[BROCK_HIT_NUM];//ツタと当たっているか
	bool isfirst_;//ダメージを受けるブロックに触れた瞬間かどうか
	bool isBlinking_;//点滅させるかどうか
	bool isHorizontal_, isVertical_;//横、縦の当たり判定をするかどうか
	bool isfirstGameOver_;//関数GameOver内で一度だけ行う処理で使う
	DIR pushDir_;//プレイヤーを飛ばす方向
	EmitterData poisonEf_;//毒ブロックに当たってるプレイヤーから出るエフェクト

	////関数////
	//プレイヤー移動
	void MovePlayer();
	void DiscernBrock(const int& index, const DIR& dir);

public:

	Player(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//当たり判定 何かに当たったら呼ばれる
	void OnCollision(GameObject* pTarget) override;

	////セッター////
	void SetGround(const float& ground) { ground_ = ground; }
	//CameraStopGround_を設定しそれを元にDeathGround_も設定
	void SetCameraStopandDeathGround_(const float& Ground) { cameraStopGround_ = Ground; deathGround_ = cameraStopGround_ - deathGroundDist_;}
	void SetDistandDir(const float& dist, const DIR& dir) {dist_ = dist; pushDir_ = dir;}
	void SetisBlinking(const bool& isBlinking) { isBlinking_ = isBlinking; }
	void SetisRight(const bool& isRight) { isRight_ = isRight; }

	////ゲッター////
	bool GetisRight_() { return isRight_; }
	BoxCollider* GetPBoxColl(const DIR& dir) { return boxColl_[dir]; }//プレイヤーの当たり判定を取得

	void RotPlayer(const bool& isRight);//プレイヤーの向きを変える
	void PushAwayPlayer();//プレイヤーを吹っ飛ばす
	void GameOver();//ゲームオーバーの際の動きの処理
};

