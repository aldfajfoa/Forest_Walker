#pragma once
#include "Engine/GameObject.h"
#include "Share.h"
#include <map>
using std::map;
#include <vector>
using std::vector;
class BoxCollider;
class Enemys;
class Player;

class Stage :
    public GameObject
{
	////�|�C���^////
	vector<BoxCollider*> stageboxColls_;
	Player* player_;

	////�n���h��////
    //���f��
	map<BLOCK, int> hBlocks_;//�u���b�N�̃n���h�������z��
	//�摜
	int hActionUI_;//������@�̉摜
	int hBack_;//�w�i

	////csv����l���󂯎��ϐ�////
	int blockDamage_;//�_���[�W���󂯂�u���b�N�łǂ̂��炢�_���[�W���󂯂邩
	float notDrawDist_;//�u���b�N�����̋����v���C���[���痣�ꂽ��`�悵�Ȃ��Ȃ�
	XMFLOAT2 actionUIPos_;//������@�̕\���ʒu
	XMFLOAT2 backSize_;//�w�i�T�C�Y

	////���̑��̕ϐ�////
	int width_, height_; //�X�e�[�W��,����
	Transform actionUIpos_;//������@�̕\���ʒu
	vector<BlockData> blocks_;//�}�b�v�̃f�[�^�����z��

	////�֐�////
	bool DecideIsRight(const string& object);//�E�������݂̂ŕԂ�
	DIR DecideIsDir(const string& object);//�l�����̂ǂꂩ��Ԃ�
public:
	Stage(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	////�Z�b�^�[////
	void SetMap(const string& stageFileName);//�X�e�[�W�̐���

	////�Q�b�^�[////
	int Getwidth() { return width_; }
	int Getheight() { return height_; }
	int GetBlockID(const int& index) { return blocks_[index].blockID; }
	int GetBlocksSize() { return blocks_.size(); }
	int GetBlockDamage() { return blockDamage_; }
	bool GetisActive(const int& index) { return blocks_[index].isActive; }
	BoxCollider* GetboxColl(const int& index) { return blocks_[index].boxColl; }
};

