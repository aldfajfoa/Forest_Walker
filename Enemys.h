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
	//�|�C���^
	vector<Enemys*> enemys_;//�G�̃|�C���^�B
protected:
	//�萔
	const int ENEMY_HP_DEFAULT = 1;
	static const int ENEMY_COLL_NUM = DIR::DIR_MAX;//�{�̓����蔻��̐�

	//�|�C���^
	BoxCollider* attackedColl_ = nullptr;//�U�����󂯂铖���蔻��
	Timer* blinkTime_ = nullptr;//�_�ł�����Ԋu���Ǘ�

	////�n���h��////
    //��
	int hAttackSound_ = NULL_NUM;//�v���C���[�ɓ����������̌��ʉ�
	int hDieSound_ = NULL_NUM;//���ʎ��̌��ʉ�

	//���̑��̕ϐ�
	int currentHP_ = ENEMY_HP_DEFAULT;//����HP
	bool isRight_ = false;//�E�������ǂ���
	bool isBlinking_ = false;//�_�ł����邩�ǂ���
	bool isAffectedSlash = true;//�a�����󂯂邩
	bool isAlive_ = true;//����
	bool isDraw_ = true;//�`�悷�邩�ǂ���
	EmitterData attackEf_;//�v���C���[�ɓ����������ɏo��G�t�F�N�g
	EmitterData dieEf_;//���񂾎��ɏo��G�t�F�N�g

public:
	Enemys() {
		//�f�t�H���g�̉����Z�b�g
		hAttackSound_ = Audio::Load("Sound\\Impact.wav");
		assert(hAttackSound_ > NULL_NUM);
		hDieSound_ = Audio::Load("Sound\\EnemyDie.wav");
		assert(hDieSound_ > NULL_NUM);

		//�f�t�H���g�̃G�t�F�N�g���Z�b�g
		attackEf_ = VFX::SetEffect("AttackEffectData.csv");
		dieEf_ = VFX::SetEffect("DieEffectData.csv");
	}

	////�Z�b�^�[////
	void SetisBlinking(const bool& is) { isBlinking_ = is; }
	void SetisAffectedSlash(const bool& is) { isAffectedSlash = is; }
	void SetisRight(const bool& isRight) { isRight_ = isRight; }

	////�Q�b�^�[////
	int GetCurrentHP() { return currentHP_; }
	bool GetisAffectedSlash() { return isAffectedSlash; }
	vector<Enemys*> GetEnemys() { return enemys_; }
	BoxCollider* GetAttackedColl() { return attackedColl_; }

	//�z��ɓG��ǉ�
	void AddEnemy(Enemys* enemy) { enemys_.push_back(enemy); }

	//HP�����炷
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
			//����ł���G�͔z�񂩂�폜
			if (enemys_[e]->isAlive_ == false)
			{
				enemys_.erase(enemys_.begin() + e);
			}
		}
	}
};

