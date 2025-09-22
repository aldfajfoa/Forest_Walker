#include "Goal.h"
#include "Engine/BoxCollider.h"
#include "Engine/Audio.h"
#include "Engine/CsvReader.h"
#include "Engine/Model.h"
#include "Engine/SceneManager.h"
#include "Player.h"
#include "Timer.h"

Goal::Goal(GameObject* parent)
	:GameObject(parent, "Gool"), boxColl_(nullptr), timer_(nullptr), hGoal_(NULL_NUM), hSpark_(NULL_NUM), hGoalSound_(NULL_NUM),
	gLeftAngle_(0.0f), gRightAngle_(0.0f), goalSec_(0.0f), isfirstSpark_(true), isfirstGoal_(true), isRight_(false)
{
}

void Goal::Initialize()
{
	//csvからデータ読み込み
	CsvReader csv;//データを読むクラスのインスタンスを作成
	bool ret = csv.Load("CsvData\\Stage\\GoalData.csv");
	assert(ret);
	int row = 1, column = 0;

	//ハンドルにデータをセット
	hGoal_ = Model::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hGoal_ > NULL_NUM);
	hGoalSound_ = Audio::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hGoalSound_ > NULL_NUM);

	gLeftAngle_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	gRightAngle_ = 360.0f - gLeftAngle_;
	goalSec_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));

	boxColl_ = new BoxCollider({ 0,0,0 }, { 0,0,0 });
	boxColl_->Setcenter(csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column))));//位置設定
	boxColl_->Setsize(csv.extractFloat3(csv.GetString(row, csv.ReturnAndAddIndex(column))));//サイズ設定
	AddCollider(boxColl_);

	timer_ = Instantiate<Timer>(this);
}

void Goal::Update()
{
	if (isSet_)
	{
		if (isRight_)
		{
			transform_.rotate_.y = gRightAngle_;
		}
		else
		{
			transform_.rotate_.y = gLeftAngle_;
		}
		isSet_ = false;
	}

	if (isfirstSpark_)
	{
		EmitterData SparklingEf = VFX::SetEffect("SparklingEffectData.csv");;//常に出続けるキラキラエフェクト
		SparklingEf.position = transform_.position_;

		hSpark_ = VFX::Start(SparklingEf);
		isfirstSpark_ = false;
	}

	//プレイヤーと当たったらクリア画面へ
	Player* player = (Player*)GetParent()->GetParent()->FindGameObject<Player>();
	if (boxColl_->IsHit(player->GetPBoxColl(CENTER)))
	{
		player->SetIsEnd(true);
		Audio::Play(hGoalSound_);
		VFX::End(hSpark_);
		if(isfirstGoal_)
		{
			isfirstGoal_ = false;

			//ゴールした時に出るエフェクト(キラキラ)
			EmitterData GoalSparklingEf = VFX::SetEffect("GoalSparklingEffectData.csv");
			GoalSparklingEf.position = transform_.position_;
			VFX::Start(GoalSparklingEf);
			//ゴールした時に出るエフェクト(粒子)
			EmitterData GoalParticleEf = VFX::SetEffect("GoalParticleEffectData.csv");;
			GoalParticleEf.position = transform_.position_;
			VFX::Start(GoalParticleEf);
		}
		if (timer_->TimeElapsed(goalSec_))
		{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_CLEAR);
		}
	}
}

void Goal::Draw()
{
	Model::SetTransform(hGoal_, transform_);
	Model::Draw(hGoal_);
}