#pragma once
#include "Engine/GameObject.h"
#include "Engine/Audio.h"
#include "Engine/VFX.h"
#include "assert.h"
#include "Share.h"
#include <vector>
using std::vector;
class BoxCollider;
class Timer;

class Enemys
{
	//ポインタ
	vector<Enemys*> enemys_;//敵のポインタ達
protected:
	//定数
	const int ENEMY_HP_DEFAULT = 1;
	static const int ENEMY_COLL_NUM = DIR::DIR_MAX;//本体当たり判定の数

	//ポインタ
	BoxCollider* attackedColl_ = nullptr;//攻撃を受ける当たり判定
	Timer* blinkTime_ = nullptr;//点滅させる間隔を管理

	////ハンドル////
    //音
	int hAttackSound_ = NULL_NUM;//プレイヤーに当たった時の効果音
	int hDieSound_ = NULL_NUM;//死ぬ時の効果音

	//その他の変数
	int currentHP_ = ENEMY_HP_DEFAULT;//現在HP
	bool isRight_ = false;//右向きかどうか
	bool isBlinking_ = false;//点滅させるかどうか
	bool isAffectedSlash = true;//斬撃を受けるか
	bool isAlive_ = true;//生死
	bool isDraw_ = true;//描画するかどうか
	EmitterData attackEf_;//プレイヤーに当たった時に出るエフェクト
	EmitterData dieEf_;//死んだ時に出るエフェクト

public:
	Enemys() {
		//デフォルトの音をセット
		hAttackSound_ = Audio::Load("Sound\\Impact.wav");
		assert(hAttackSound_ > NULL_NUM);
		hDieSound_ = Audio::Load("Sound\\EnemyDie.wav");
		assert(hDieSound_ > NULL_NUM);

		//デフォルトのエフェクトをセット
		attackEf_ = VFX::SetEffect("AttackEffectData.csv");
		dieEf_ = VFX::SetEffect("DieEffectData.csv");
	}

	////セッター////
	void SetisBlinking(const bool& is) { isBlinking_ = is; }
	void SetisAffectedSlash(const bool& is) { isAffectedSlash = is; }
	void SetisRight(const bool& isRight) { isRight_ = isRight; }

	////ゲッター////
	int GetCurrentHP() { return currentHP_; }
	bool GetisAffectedSlash() { return isAffectedSlash; }
	vector<Enemys*> GetEnemys() { return enemys_; }
	BoxCollider* GetAttackedColl() { return attackedColl_; }

	//配列に敵を追加
	void AddEnemy(Enemys* enemy) { enemys_.push_back(enemy); }

	//HPを減らす
	void DecreaseHP(const int& DecreaseNum) {
		currentHP_ -= DecreaseNum;
		if (currentHP_ <= 0) {
			isAlive_ = false;
		}
	}

	void DeleteEnemy()
	{
		for (int e = 0; e < enemys_.size(); e++)
		{
			//死んでいる敵は配列から削除
			if (enemys_[e]->isAlive_ == false)
			{
				enemys_.erase(enemys_.begin() + e);
			}
		}
	}
};

