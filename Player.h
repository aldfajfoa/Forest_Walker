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
	////�萔////
	static const int BROCK_HIT_NUM = 4;
	static const int PBOX_NUM = DIR::DIR_MAX;//�{�̓����蔻��̐�
	
	////�|�C���^////
	BoxCollider* boxColl_[PBOX_NUM];
	PlayScene* playScene_;
	Stage* stage_;
	Timer* bDamageTime_;//�u���b�N�ɂ���ă_���[�W���󂯂�Ԋu
	Timer* arrowIntervalTime_;
	Timer* blinkTime_;
	Timer* gameOverStopTime_;//�Q�[���I�[�o�[�ɂȂ����ۈ�u�ł܂鎞��
	Timer* gameOverChangeTime_;//�Q�[���I�[�o�[�ɂȂ��Ă���GameOverScene�֑J�ڂ���܂ł̎���

	////�n���h��////
    //���f��
	int hPlayer_;
	//�摜
	int hBlack_;//�Q�[���I�[�o�[�̍ۂ̈Ó]
	//��
	int hPoisonSound_;//�ł��󂯂����̉�
	int hDeadSound_, hDeadFallSound_;//���񂾏u�Ԃ̉��Ɨ����Ă����Ƃ��̉�

	////csv����l���󂯎��ϐ�////
	float moveSpeed_;//�ړ����x
	float gravity_;//�d�͉����x
	float rotSpeed_;//���E���������̉�]���x
	int blackAlpha_;//�Q�[���I�[�o�[���̈Ó]�̓����x
	float deathGroundDist_;//�������玀�ʍ��W���J�������~�܂���W����ǂ̂��炢������
	float blowSpeed_, blowJumpHeight_;//������΂���鎞�̑��x�ƃW�����v�̍���
	float arrowInterval_;//���A���Ō��Ă�Ԋu
	float maxJumpHoldTime_;//�������ŃW�����v�������Ȃ�ő剟������
	float initialJumpSpeed_;//�ŏ��̃W�����v���x
	float jumpHoldBoost_;//�����������ꍇ�̒ǉ��W�����v���x
	float frameTime_;//�t���[�����Ƃ̎��Ԍo��
	float brockDamageTime_;//�u���b�N�ɂ��_���[�W���󂯂�Ԋu
	float deadJumpHeight_;//�Q�[���I�[�o�[���o�ł̃W�����v�̍���
	float gameOverPosZ_;//�Q�[���I�[�o�ɂȂ����炱�̐��l�������đ��̃I�u�W�F�N�g���O�ɏo��
	float gameOverRotSpeed_;//�Q�[���I�[�o�[���o�ł̃v���C���[��]���x
	float playStopTime_;//�Q�[���I�[�o�[�ɂȂ����ۈ�u�ł܂鎞�ԁA���ꂪ�߂����牉�o���n�܂�
	float gameOverTime_;//�Q�[���I�[�o�[���o�̎���
	XMFLOAT3 cameraDistTarget_;
	XMFLOAT3 cameraDistPos_;


	////���̑��̕ϐ�////
	float jumpSpeed_, jumpHeight_;//�W�����v�̑��x�A����
	float ground_, ceiling_;//�n��,�V��
	float cameraStopGround_;//�����艺�ɂ����ƃJ�������v���C���[��ǂ�Ȃ��Ȃ�
	float deathGround_;//�����܂ŗ�������Q�[���I�[�o�[
	float dist_;
	bool isDraw_;//�`�悷�邩�ǂ���
	bool isRight_;//�E�������ǂ���
	bool isGround_,isCeiling_;//�n�ʂɏ���Ă��邩
	bool isJump_;//�W�����v���ł��邩
	bool isPdamage_[BROCK_HIT_NUM];//���̂ǂꂩ��true�̊Ԃ̓_���[�W���󂯂�
	bool isBrockHit_[BROCK_HIT_NUM];//�u���b�N�Ɠ������Ă��邩
	bool isVineHit_[BROCK_HIT_NUM];//�c�^�Ɠ������Ă��邩
	bool isfirst_;//�_���[�W���󂯂�u���b�N�ɐG�ꂽ�u�Ԃ��ǂ���
	bool isBlinking_;//�_�ł����邩�ǂ���
	bool isHorizontal_, isVertical_;//���A�c�̓����蔻������邩�ǂ���
	bool isfirstGameOver_;//�֐�GameOver���ň�x�����s�������Ŏg��
	DIR pushDir_;//�v���C���[���΂�����
	EmitterData poisonEf_;//�Ńu���b�N�ɓ������Ă�v���C���[����o��G�t�F�N�g

	////�֐�////
	//�v���C���[�ړ�
	void MovePlayer();
	void DiscernBrock(const int& index, const DIR& dir);

public:

	Player(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//�����蔻�� �����ɓ���������Ă΂��
	void OnCollision(GameObject* pTarget) override;

	////�Z�b�^�[////
	void SetGround(const float& ground) { ground_ = ground; }
	//CameraStopGround_��ݒ肵���������DeathGround_���ݒ�
	void SetCameraStopandDeathGround_(const float& Ground) { cameraStopGround_ = Ground; deathGround_ = cameraStopGround_ - deathGroundDist_;}
	void SetDistandDir(const float& dist, const DIR& dir) {dist_ = dist; pushDir_ = dir;}
	void SetisBlinking(const bool& isBlinking) { isBlinking_ = isBlinking; }
	void SetisRight(const bool& isRight) { isRight_ = isRight; }

	////�Q�b�^�[////
	bool GetisRight_() { return isRight_; }
	BoxCollider* GetPBoxColl(const DIR& dir) { return boxColl_[dir]; }//�v���C���[�̓����蔻����擾

	void RotPlayer(const bool& isRight);//�v���C���[�̌�����ς���
	void PushAwayPlayer();//�v���C���[�𐁂���΂�
	void GameOver();//�Q�[���I�[�o�[�̍ۂ̓����̏���
};

