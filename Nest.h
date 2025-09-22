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
	///�|�C���^////
	BoxCollider* boxColl_;
	Enemys* enemys_;
	Player* player_;
	PlayScene* playScene_;
	Timer* birthIntervalTime_;

	////�n���h��////
	//���f��
	int hNest_;

	////csv����l���󂯎��ϐ�////
	int maxHP_;//�̗͍ő�l
	float birthInterval_;//�n�`�𐶐�����Ԋu
	float beeDist_;//�n�`�𐶐���������������ǂ̂��炢������

	////���̑��̕ϐ�////
	bool isBirthBee_;//�n�`�𐶐����邩
public:
	Nest(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	////�Z�b�^�[////
	void SetisBirthBee(const bool& isBirthBee) { isBirthBee_ = isBirthBee; }
};

