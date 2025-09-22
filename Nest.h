#pragma once
#include "Engine/GameObject.h"
#include "Enemys.h"
class BoxCollider;
class Enemys;
class Player;
class PlayScene;
class Timer;

class Nest :
	public GameObject, public Enemys
{
	///ポインタ////
	BoxCollider* boxColl_;
	Enemys* enemys_;
	Player* player_;
	PlayScene* playScene_;
	Timer* birthIntervalTime_;

	////ハンドル////
	//モデル
	int hNest_;

	////csvから値を受け取る変数////
	int maxHP_;//体力最大値
	float birthInterval_;//ハチを生成する間隔
	float beeDist_;//ハチを生成した時自分からどのくらい離すか

	////その他の変数////
	bool isBirthBee_;//ハチを生成するか
public:
	Nest(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	////セッター////
	void SetisBirthBee(const bool& isBirthBee) { isBirthBee_ = isBirthBee; }
};

