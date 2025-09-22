#pragma once
#include <string>
using std::string;
#include<map>
using std::map;
#include <vector>
using std::vector;
using std::pair;

const int HP_DEAD_LINE = 0;//HP�������܂Ō������玀��

const int NULL_NUM = -1;//�n���h������ł��邱�Ƃ�\��

const int INDEX_AJM = 1;//�C���f�b�N�X�̒����Ɏg��

const int MAX_ALPHA = 255;//�����x�̍ő�l

//�f�t�H���g�̕���
const int OBJ_RIGHT_ANGLE = 90;
const int OBJ_LEFT_ANGLE = 360 - OBJ_RIGHT_ANGLE;
const int OBJ_FRONT_ANGLE = 180;

const int DEFAULT_GROUND = -999;//���ɉ������ꂪ�Ȃ�����Ground
const int DEFAULT_CEILING = 999;//��ɉ����u���b�N���Ȃ�����Ceiling

const float HALF = 0.5f;//���l�𔼕��ɂ��������ɂ�����

const float WINDOW_WIDTH = 1280.0f;//��ʉ���
const float WINDOW_HEIGHT = 720.0f;//��ʏc��
const pair<int, int> IMAGE_ORIZIN = { WINDOW_WIDTH * HALF,WINDOW_HEIGHT * HALF };//�摜�̍��W���_

const float BLINKING_INTERVAL = 1.0f;//�_�ł̊֐����Ăяo���Ԋu

const XMFLOAT2 HP_VIB_RANGE = { 0.1f ,0.05f };//HP�̐U����

const XMFLOAT3 ORIZIN = { 0.0f,0.0f,0.0f };//���_

const XMFLOAT3 DEFAULT_BLOCK_SIZE = { 1.0f,1.0f,0.1f };//�u���b�N��̃f�t�H���g�T�C�Y

inline const XMFLOAT3& operator +(XMFLOAT3& a, const XMFLOAT3& b) 
{
	XMFLOAT3 result = { a.x + b.x, a.y + b.y, a.z + b.z };
	return result;
}

inline const bool& operator ==(XMFLOAT3& a, const XMFLOAT3& b)
{
	if (a.x == b.x && a.y == b.y && a.z == b.z)
	{
		return true;
	}
	return false;
}

//�Q�[����ʂŕ\������X�e�[�W��
const string STAGE_VIEW_NAMES[] = {"STAGE","BEAST","BEENEST","FLOWER","VINE","BROCK"};

//�X�e�[�W�f�[�^�̃t�@�C����
const map<string, string> STAGE_FILE_NAMES = {
	{STAGE_VIEW_NAMES[0], "Stage.csv"},
	{STAGE_VIEW_NAMES[1], "StageBeast.csv"},
	{STAGE_VIEW_NAMES[2], "StageBeeNest.csv"},
	{STAGE_VIEW_NAMES[3], "StageFlower.csv"},
	{STAGE_VIEW_NAMES[4], "StageVine.csv"},
	{STAGE_VIEW_NAMES[5], "StageBrock.csv"}
};
const int STAGE_NUM = STAGE_FILE_NAMES.size();//�X�e�[�W��

enum BLOCK//�u���b�N�̎��
{
	GROUND = 100,//�n��
	GRASS = 101, //���t���n��
	ROCK = 102,//��
	POISON = 103,//��
	HALF_GROUND = 105,//�����̒n��
	BLOCK_MAX,
};

//�u���b�N�̃T�C�Y�����z��
const map<BLOCK, XMFLOAT3> blockSizes = {
	{ GROUND,      DEFAULT_BLOCK_SIZE },
	{ GRASS,       DEFAULT_BLOCK_SIZE },
	{ ROCK,        DEFAULT_BLOCK_SIZE },
	{ POISON,      DEFAULT_BLOCK_SIZE },
	{ HALF_GROUND, {1.0f, 0.5f,0.1f} }
};

enum OBJECT//�I�u�W�F�N�g�̎��
{
	PLAYER = 99,//�v���C���[
	BEAST = 1,//�r�[�X�g
	FLOWER = 2,//��
	NEST = 3,//�n�`�̑�
	NORMAL_VINE = 30,//�ʏ�̃c�^
	SPINY_VINE = 31,//���t���c�^
	LEAF_VINE = 32,//�t�t���c�^
	GOAL = 20,//�S�[��
};

enum DIR//����,�ʒu
{
	LEFT,  //��
	RIGHT, //�E
	UP,   //��
	DOWN, //��
	CENTER,//���S
	DIR_MAX,
};
const XMFLOAT2 NUM_DIR[DIR_MAX] = { {-1,0},{1,0},{0,1},{0,-1},{0,0} };

enum VINETYPE//�c�^�̎��
{
	NORMAL,//�ʏ�
	SPINY, //���t��
	LEAF,  //�t�t��
	V_MAX
};

enum HPSTATE//HP�̏��
{
	GREEN,
	YELLOW,
	RED,
	HP_POISON,
	HP_MAX
};

struct BlockData 
{
	int blockID = 0;
	BoxCollider* boxColl = nullptr;
	bool isActive = true;//�`��ⓖ���蔻������邩
};