#pragma once
#include "Engine/GameObject.h"
#include "Engine/VFX.h"
#include "Share.h"
using std::pair;
class Player;
class Timer;

class HP :
    public GameObject
{
	////�萔////
	const float MAX_PERCENTAGE = 100.0f;

	////�|�C���^////
	Player* player_;
	Timer* isDamageTime_;//�A���Ń_���[�W���󂯂�Ԋu

	////�n���h��////
	//�摜
	int hHPBars_[HP_MAX];
	int hBehindHP_;
	int hHPFrame_, hPoisonFrame_;

	////csv����l���󂯎��ϐ�////
	int maxHP_;//�̗͍ő�l
	int yellowLine_, redLine_;//HP�����F�A�ԂɂȂ銄��
	pair<int, int> hpBarImageSize_;//HP�o�[�̉摜�T�C�Y
	pair<int, int> hpBarSize_;//HP�o�[�̉摜�T�C�Y(�󔒂̕����܂܂Ȃ��傫��)
	float vibrationSpeed_;//�U���̑���
	float vibDecreaseSpeed_;//�U�����x���Ȃ�X�s�[�h
	float isDamageInterval_;//�A���Ń_���[�W���󂯂���Ԋu
	XMFLOAT2 hpFrameDist_;
	XMFLOAT3 poisonEfPos_;//�v���C���[�𒆐S�Ƃ��ēŃG�t�F�N�g���o���ʒu
	XMFLOAT3 hpBarPos_;//HP�o�[�̈ʒu
	XMFLOAT3 hpFramePos_, hpFrameScale_;//HP�g�̈ʒu,�傫��

	////���̑��̕ϐ�////
	float currentHP_, currentPercentage_;//����HP�Ƃ��̊���
	float angle_;
	float cutNum_;//�摜��؂蔲���͈�
	XMFLOAT2 vibrationRange_;//�h��̑傫��
	XMFLOAT2 center_;//�U���̎��Ɏg�����̒��S�ʒu�������Ă����ϐ�
	bool isDamage_;//�_���[�W���󂯂邩
	HPSTATE currentState_;
	Transform frameTrans_;//HP�g�̃g�����X�t�H�[��
	EmitterData poisonEf_;//�Ńu���b�N�ɓ������Ă�HP�o�[����o��G�t�F�N�g

	////�֐�////
	void ChangeState();
public:
	HP(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override {};

	////�Z�b�^�[////
	void SetVibrationRangeX(const float& vibrationRangeX)
	{
		vibrationRange_.x = vibrationRangeX;
		angle_ = 0;
	}
	void SetVibrationRangeY(const float& vibrationRangeY)
	{
		vibrationRange_.y = vibrationRangeY;
		angle_ = 0;
	}

	void SetCurrentState(const HPSTATE& hpState) { currentState_ = hpState; }
	void SetCurrentHP(const int& currentHp) { currentHP_ = currentHp; }

	////�Q�b�^�[////
	bool GetIsDamage() { return isDamage_; }

	//HP�����炷�֐�
	//�����͌��炵�����l
	void DecreaseHP(const int& decreaseNum) {
		if (isDamage_)
		{
			isDamage_ = false;
			currentHP_ -= decreaseNum;
		}
	}

	void PlayDamageEffect(const HPSTATE& hpState);
};

