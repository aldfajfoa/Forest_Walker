#include "Player.h"
#include "Engine/BoxCollider.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Engine/Model.h"
#include "Engine/Audio.h"
#include "Engine/CsvReader.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"
#include "Arrow.h"
#include "HP.h"
#include "Slash.h"
#include "PlayScene.h"
#include "Goal.h"
#include "Stage.h"
#include "Vine.h"
#include "Timer.h"

Player::Player(GameObject* parent)
	:GameObject(parent, "Player"), playScene_(nullptr), stage_(nullptr), bDamageTime_(nullptr), arrowIntervalTime_(nullptr), 
	blinkTime_(nullptr),hPlayer_(NULL_NUM), hBlack_(NULL_NUM), hPoisonSound_(NULL_NUM), hDeadSound_(NULL_NUM), hDeadFallSound_(NULL_NUM), 
	moveSpeed_(0.0f), gravity_(0.0f), rotSpeed_(0.0f), blackAlpha_(0), deathGroundDist_(0.0f), blowSpeed_(0.0f), blowJumpHeight_(0.0f), arrowInterval_(0.0f),
	maxJumpHoldTime_(0.0f), initialJumpSpeed_(0.0f), jumpHoldBoost_(0.0f), frameTime_(0.0f), brockDamageTime_(0.0f), deadJumpHeight_(0.0f), gameOverPosZ_(0.0f), 
	gameOverRotSpeed_(0.0f), playStopTime_(0.0f), gameOverTime_(0.0f), cameraDistTarget_({0,0,0}), cameraDistPos_({ 0,0,0 }),jumpSpeed_(0.0f), 
	jumpHeight_(0.0f), ground_(0.0f), ceiling_(0.0f),cameraStopGround_(0.0f), deathGround_(0.0f), dist_(0.0f), isDraw_(true), isRight_(true), 
	isGround_(true), isCeiling_(true), isJump_(true), isfirst_(true),isBlinking_(false), isHorizontal_(true), isVertical_(true), isfirstGameOver_(true),
	pushDir_(RIGHT),gameOverStopTime_(nullptr), gameOverChangeTime_(nullptr)
{
	for (int b = 0; b < PBOX_NUM; b++)
	{
		boxColl_[b] = nullptr;
	}

	for (int i = 0; i < BROCK_HIT_NUM; i++)
	{
		isPdamage_[i] = false;
		isBrockHit_[i] = false;
		isVineHit_[i] = false;
	}
}

void Player::Initialize()
{
	//csvからデータ読み込み
	CsvReader csv;//データを読むクラスのインスタンスを作成
	bool ret = csv.Load("CsvData\\Chara\\PlayerData.csv");
	assert(ret);
	int row = 1, column = 0;

	//ハンドルにデータをセット
	hPlayer_ = Model::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hPlayer_ > NULL_NUM);
	hBlack_ = Image::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hBlack_ > NULL_NUM);
	Image::SetTransform(hBlack_, transform_);
	hPoisonSound_ = Audio::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hPoisonSound_ > NULL_NUM);
	hDeadSound_ = Audio::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hDeadSound_ > NULL_NUM);
	hDeadFallSound_ = Audio::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hDeadFallSound_ > NULL_NUM);

	moveSpeed_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	gravity_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	rotSpeed_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	blackAlpha_ = csv.extractInt(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	Image::SetAlpha(hBlack_, blackAlpha_);
	deathGroundDist_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	blowSpeed_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	blowJumpHeight_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	arrowInterval_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	maxJumpHoldTime_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	initialJumpSpeed_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	jumpHoldBoost_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	frameTime_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	brockDamageTime_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	deadJumpHeight_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	gameOverPosZ_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	gameOverRotSpeed_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	playStopTime_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	gameOverTime_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	cameraDistTarget_ = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	cameraDistPos_ = csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column)));

	//当たり判定の設定
	for (int i = 0; i < PBOX_NUM; i++)
	{
		boxColl_[i] = new BoxCollider({ 0,0,0 }, { 0,0,0 });
		boxColl_[i]->Setcenter(csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column))));//位置設定
	}
	for (int i = 0; i < PBOX_NUM; i++)
	{
		boxColl_[i]->Setsize(csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column))));//サイズ設定
		AddCollider(boxColl_[i]);
	}


	Slash* slash = Instantiate<Slash>(FindObject("PlayScene"));
	playScene_ = (PlayScene*)GetParent();
	stage_ = GetParent()->FindGameObject<Stage>();
	bDamageTime_ = Instantiate<Timer>(this);
	arrowIntervalTime_ = Instantiate<Timer>(this);
	arrowIntervalTime_->ResetTime(0.0f);
	blinkTime_ = Instantiate<Timer>(this);
	gameOverChangeTime_ = Instantiate<Timer>(this);
	gameOverStopTime_ = Instantiate<Timer>(this);
	gameOverStopTime_->ResetTime(playStopTime_);

	poisonEf_ = VFX::SetEffect("PlayerPoisonEffectData.csv");

	transform_.rotate_.y = OBJ_RIGHT_ANGLE;
}

void Player::Update()
{
	if (isSet_)
	{
		if (isRight_)
		{
			transform_.rotate_.y = OBJ_RIGHT_ANGLE;
		}
		else
		{
			transform_.rotate_.y = OBJ_LEFT_ANGLE;
		}
		isSet_ = false;
	}

	if (!isEnd_)
	{
		MovePlayer();
	}
	

	if (transform_.position_.y >= ceiling_) {
		isCeiling_ = true;
		jumpSpeed_ = 0;
		isJump_ = false;
		transform_.position_.y = ceiling_;
	}
	else
	{
		isCeiling_ = false;
	}

	if (transform_.position_.y <= ground_) {
		isGround_ = true;
		transform_.position_.y = ground_;
		jumpSpeed_ = 0.0f;
		isJump_ = true;
	}
	else
	{
		isGround_ = false;
	}
	
	if (!isEnd_ && transform_.position_.y > cameraStopGround_)//ここでカメラが止まる
	{
		XMFLOAT3 t = transform_.position_ + cameraDistTarget_;
		XMFLOAT3 p = transform_.position_ + cameraDistPos_;
		Camera::SetTarget(t);
		Camera::SetPosition(p);
	}
	else if (!isEnd_ && transform_.position_.y < deathGround_)//ゲームオーバー
	{
		Goal* goal = (Goal*)(stage_->FindGameObject<Goal>());
		goal->EndEf();
		HP* hp = GetParent()->FindGameObject<HP>();
		hp->SetCurrentHP(HP_DEAD_LINE);
	}

	//Fキーで矢を撃つ
	if (arrowIntervalTime_->NoResetTimeElapsed() && !isEnd_)
	{
		if (Input::IsKeyDown(DIK_F) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
		{
			Arrow* arrow = Instantiate<Arrow>(FindObject("PlayScene"));
			arrow->SetPosition(transform_.position_);
			arrow->SetArrowDir(isRight_);
			arrowIntervalTime_->ResetTime(arrowInterval_);
		}
	}
	
	PushAwayPlayer();

	if (isBlinking_)
	{
		if (!blinkTime_->TimeElapsed(BLINKING_INTERVAL))
		{
			Blinking(isDraw_);
		}
		else
		{
			isDraw_ = true;
			isBlinking_ = false;
		}
	}

	//isPdamage_のどれかがtrueだったらダメージを受ける
	if (!isEnd_)
	{
		if (isPdamage_[LEFT] || isPdamage_[RIGHT] || isPdamage_[UP] || isPdamage_[DOWN])
		{
			HP* hp = GetParent()->FindGameObject<HP>();
			if (isfirst_ || bDamageTime_->TimeElapsed(brockDamageTime_))//当たった瞬間は即ダメージを受ける
			{
				hp->SetVibrationRangeY(HP_VIB_RANGE.y);
				hp->DecreaseHP(stage_->GetBlockDamage());
				hp->SetCurrentState(HP_POISON);
				hp->PlayDamageEffect(HP_POISON);

				poisonEf_.position = transform_.position_;
				VFX::Start(poisonEf_);

				Audio::Play(hPoisonSound_);
				isfirst_ = false;
			}
		}
		else
		{
			isfirst_ = true;
			bDamageTime_->ResetTime(0.0f);
		}
	}


	if (isBrockHit_[LEFT] || isBrockHit_[RIGHT])
	{
		isHorizontal_ = false;//左右どちらかがブロックと当たっていたらツタとの左右判定もオフにする
	}
	if (isBrockHit_[UP] || isBrockHit_[DOWN])
	{
		isVertical_ = false;//上下どちらかがブロックと当たっていたらツタとの上下判定もオフにする
	}
	if (!isGround_ && !isVertical_)
	{
		isHorizontal_ = false;//落ちているかつツタとの上下判定がオフの時は左右判定もオフにする
	}
	if (isBrockHit_[DOWN] && (isVineHit_[UP] || isVineHit_[DOWN]))
	{
		isHorizontal_ = false;
	}

	if (!isVineHit_[LEFT] && !isVineHit_[RIGHT] && !isVineHit_[UP] && !isVineHit_[DOWN])
	{
		isHorizontal_ = true;
		isVertical_ = true;
	}

	ground_ = DEFAULT_GROUND;
	ceiling_ = DEFAULT_CEILING;
	for (int b = 0; b < BROCK_HIT_NUM; b++)
	{
		isPdamage_[b] = false;
		isBrockHit_[b] = false;
		isVineHit_[b] = false;
	}
}

void Player::Draw()
{
	if (isEnd_)
	{
		Image::Draw(hBlack_);
	}

	if (isDraw_)
	{
		Model::SetTransform(hPlayer_, transform_);
		Model::Draw(hPlayer_);
	}
}

void Player::Release()
{
	Model::Release(hPlayer_);
}

void Player::RotPlayer(const bool& isRight)
{
	if (isRight_ == false)
	{
		if (transform_.rotate_.y != OBJ_LEFT_ANGLE)
		{
			transform_.rotate_.y += rotSpeed_;
		}
	}
	else
	{
		if (transform_.rotate_.y != OBJ_RIGHT_ANGLE)
		{
			transform_.rotate_.y -= rotSpeed_;
		}
	}
}

void Player::PushAwayPlayer()
{
	if (!isEnd_) {
		if (dist_ > 00.0f)
		{
			if ((pushDir_ == LEFT) || (pushDir_ == RIGHT))
			{
				transform_.position_.x += blowSpeed_ * NUM_DIR[pushDir_].x;
			}
			jumpSpeed_ = -sqrtf(gravity_ * blowJumpHeight_);
			dist_--;
		}
		else if (dist_ <= 0.0f)
		{
			dist_ = 0.0f;
			pushDir_ = CENTER;
		}
	}
}

void Player::OnCollision(GameObject* pTarget)
{
	XMFLOAT3 collPos[5], collSize[5];
	//当たり判定の設定
	for (int i = 0; i < PBOX_NUM; i++)
	{
		collPos[i] = boxColl_[i]->Getcenter();
		collSize[i] = boxColl_[i]->Getsize();
	}

	if (!isEnd_)
	{
		for (int b = 0; b < stage_->GetBlocksSize(); b++)
		{
			if (stage_->GetboxColl(b) != nullptr && stage_->GetisActive(b))//ブロックの当たり判定がnullptr(空白)じゃなかったら当たり判定
			{
				if (boxColl_[LEFT]->IsHit(stage_->GetboxColl(b))) {
					isBrockHit_[LEFT] = true;
					float dist = (abs)(collPos[LEFT].x) + (abs)(collSize[LEFT].x * HALF);//ブロックからどのくらい離すか(プレイヤーの中心から当たり判定の中心の距離分)
					transform_.position_.x = (stage_->GetboxColl(b)->Getcenter().x + (stage_->GetboxColl(b)->Getsize().x * HALF)) + dist;
					DiscernBrock(b, LEFT);
				}

				if (boxColl_[RIGHT]->IsHit(stage_->GetboxColl(b))) {
					isBrockHit_[RIGHT] = true;
					float dist = (abs)(collPos[RIGHT].x) + (abs)(collSize[RIGHT].x * HALF);//ブロックからどのくらい離すか(プレイヤーの中心から当たり判定の中心の距離分)
					transform_.position_.x = (stage_->GetboxColl(b)->Getcenter().x - (stage_->GetboxColl(b)->Getsize().x * HALF)) - dist;
					DiscernBrock(b, RIGHT);
				}

				if (boxColl_[UP]->IsHit(stage_->GetboxColl(b))) {
					isBrockHit_[UP] = true;
					float dist = (abs)(collPos[UP].y);//ブロックからどのくらい離すか(プレイヤーの中心から当たり判定の中心の距離分)
					ceiling_ = (stage_->GetboxColl(b)->Getcenter().y - (stage_->GetboxColl(b)->Getsize().y * HALF)) - (dist);
					DiscernBrock(b, UP);
				}

				if (boxColl_[DOWN]->IsHit(stage_->GetboxColl(b))) {
					isBrockHit_[DOWN] = true;
					jumpHeight_ = 0;
					float dist = (abs)(collPos[DOWN].y);//ブロックからどのくらい離すか(プレイヤーの中心から当たり判定の中心の距離分)
					ground_ = (stage_->GetboxColl(b)->Getcenter().y + (stage_->GetboxColl(b)->Getsize().y * HALF)) + (dist);//地面に乗せる
					DiscernBrock(b, DOWN);
				}
			}
		}

		//ツタの当たり判定
		for (int v = 0; v < playScene_->Getvines().size(); v++)
		{
			Vine* vine = playScene_->Getvines()[v];
			//ツタの中心座標からツタの当たり判定中心の長さ
			XMFLOAT2 vineCenterDist = { vine->GetPosition().x + vine->GetvineColl()->Getcenter().x ,
				                        vine->GetPosition().y + vine->GetvineColl()->Getcenter().y };

			if (vine->GetisPCollision())
			{
				if (boxColl_[LEFT]->IsHit(vine->GetvineColl())) {
					isVineHit_[LEFT] = true;
					if (isHorizontal_)
					{
						float dist = (abs)(collPos[LEFT].x) + (abs)(collSize[LEFT].x * HALF);//ブロックからどのくらい離すか(プレイヤーの中心から当たり判定の中心の距離分)
						transform_.position_.x = (vineCenterDist.x + (vine->GetvineColl()->Getsize().x * HALF)) + (dist);
					}
				}
				if (boxColl_[RIGHT]->IsHit(vine->GetvineColl())) {
					isVineHit_[RIGHT] = true;
					if (isHorizontal_)
					{
						float dist = (abs)(collPos[RIGHT].x) + (abs)(collSize[RIGHT].x * HALF);//ブロックからどのくらい離すか(プレイヤーの中心から当たり判定の中心の距離分)
						transform_.position_.x = (vineCenterDist.x - (vine->GetvineColl()->Getsize().x * HALF)) - (dist);
					}
				}

				if (boxColl_[UP]->IsHit(vine->GetvineColl())) {
					isVineHit_[UP] = true;
					if (isVertical_)
					{
						float dist = (abs)(collPos[UP].y);//ブロックからどのくらい離すか(プレイヤーの中心から当たり判定の中心の距離分)
						ceiling_ = (vineCenterDist.y - (vine->GetvineColl()->Getsize().y * HALF)) - (dist);
					}
				}

				if (boxColl_[DOWN]->IsHit(vine->GetvineColl())) {
					isVineHit_[DOWN] = true;
					if (isVertical_)
					{
						float dist = (abs)(collPos[DOWN].y);//ブロックからどのくらい離すか(プレイヤーの中心から当たり判定の中心の距離分)
						ground_ = (vineCenterDist.y + (vine->GetvineColl()->Getsize().y * HALF)) + (dist);
					}
				}
			}
		}

		for (int v = 0; v < playScene_->Getvines().size(); v++)
		{
			Vine* vine = playScene_->Getvines()[v];
			//vTipCollの座標
			XMFLOAT2 vTipCenter = { vine->GetPosition().x + vine->GetvTipColl()->Getcenter().x ,
									vine->GetPosition().y + vine->GetvTipColl()->Getcenter().y };

			if (vine->GetisPCollision())
			{
				if (boxColl_[LEFT]->IsHit(vine->GetvTipColl())) {
					isVineHit_[LEFT] = true;
					if (isHorizontal_)
					{
						float dist = (abs)(collPos[LEFT].x) + (abs)(collSize[LEFT].x * HALF);//ブロックからどのくらい離すか(プレイヤーの中心から当たり判定の中心の距離分)
						transform_.position_.x = (vTipCenter.x + (vine->GetvTipColl()->Getsize().x * HALF)) + (dist);
					}
				}

				if (boxColl_[RIGHT]->IsHit(vine->GetvTipColl())) {
					isVineHit_[RIGHT] = true;
					if (isHorizontal_)
					{
						float dist = (abs)(collPos[RIGHT].x) + (abs)(collSize[RIGHT].x * HALF);//ブロックからどのくらい離すか(プレイヤーの中心から当たり判定の中心の距離分)
						transform_.position_.x = (vTipCenter.x - (vine->GetvTipColl()->Getsize().x * HALF)) - (dist);
					}
				}

				if (boxColl_[UP]->IsHit(vine->GetvTipColl())) {
					isVineHit_[UP] = true;
					if (isVertical_)
					{
						float dist = (abs)(collPos[UP].y);//ブロックからどのくらい離すか(プレイヤーの中心から当たり判定の中心の距離分)
						ceiling_ = (vTipCenter.y - (vine->GetvTipColl()->Getsize().y * HALF)) - (dist);
					}
				}

				if (boxColl_[DOWN]->IsHit(vine->GetvTipColl())) {
					isVineHit_[DOWN] = true;
					if (isVertical_)
					{
						float dist = (abs)(collPos[DOWN].y);//ブロックからどのくらい離すか(プレイヤーの中心から当たり判定の中心の距離分)
						ground_ = (vTipCenter.y + (vine->GetvTipColl()->Getsize().y * HALF)) + (dist);
					}
				}
			}
		}
	}
}

void Player::MovePlayer()
{
	//左右移動の処理
	if (Input::IsKey(DIK_LEFT) || Input::GetPadStickL().x < 0)//左向きにする
	{
		isRight_ = false;
		transform_.position_.x -= moveSpeed_;//左へ進む
	}

	if (Input::IsKey(DIK_RIGHT) || Input::GetPadStickL().x > 0)//右向きにする
	{
		isRight_ = true;
		transform_.position_.x += moveSpeed_;//右へ進む
	}

	RotPlayer(isRight_);

	// ジャンプ処理
	static float jumpHoldTime = 0.0f;

	// スペースキーを押した瞬間にジャンプを開始
	//if (isGround_ && isJump_ && (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B))) {
	if ((Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B))) {
		isGround_ = false;
		jumpSpeed_ = initialJumpSpeed_;
		isJump_ = false;
		jumpHoldTime = 0.0f;
	}

	// スペースを押し続けている間、一定時間だけ上昇を加速
	//if (!isJump_ && (Input::IsKey(DIK_SPACE) || Input::IsPadButton(XINPUT_GAMEPAD_B))) {
	if ((Input::IsKey(DIK_SPACE) || Input::IsPadButton(XINPUT_GAMEPAD_B))) {
		if (jumpHoldTime < maxJumpHoldTime_) {
			jumpSpeed_ += jumpHoldBoost_;//より高くジャンプ
			jumpHoldTime += frameTime_; //フレーム時間
		}
	}

	jumpSpeed_ += gravity_;
	transform_.position_.y -= jumpSpeed_;
}

void Player::DiscernBrock(const int& index, const DIR& dir)
{
	switch (stage_->GetBlockID(index))
	{
	case POISON:
		isPdamage_[dir] = true;
	}
}

void Player::GameOver()
{
	if (isfirstGameOver_)//一度だけ行う処理
	{
		Audio::Play(hDeadSound_);
		isEnd_ = true;
		transform_.position_.z -= gameOverPosZ_;
		transform_.rotate_.y = OBJ_FRONT_ANGLE;

		jumpSpeed_ = deadJumpHeight_;
		isfirstGameOver_ = false;
	}

	//回りながら落ちていく
	if (gameOverStopTime_->NoResetTimeElapsed())
	{
		Audio::Play(hDeadFallSound_);
		transform_.rotate_.z += gameOverRotSpeed_;

		jumpSpeed_ += gravity_;
		transform_.position_.y -= jumpSpeed_;
	}

	if (gameOverChangeTime_->TimeElapsed(gameOverTime_))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_OVER);
	}
}

