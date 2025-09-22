#pragma once
#include <string>
using std::string;
#include<map>
using std::map;
#include <vector>
using std::vector;
using std::pair;

const int HP_DEAD_LINE = 0;//HPがここまで減ったら死ぬ

const int NULL_NUM = -1;//ハンドルが空であることを表す

const int INDEX_AJM = 1;//インデックスの調整に使う

const int MAX_ALPHA = 255;//透明度の最大値

//デフォルトの方向
const int OBJ_RIGHT_ANGLE = 90;
const int OBJ_LEFT_ANGLE = 360 - OBJ_RIGHT_ANGLE;
const int OBJ_FRONT_ANGLE = 180;

const int DEFAULT_GROUND = -999;//下に何も足場がない時のGround
const int DEFAULT_CEILING = 999;//上に何もブロックがない時のCeiling

const float HALF = 0.5f;//数値を半分にしたい時にかける

const float WINDOW_WIDTH = 1280.0f;//画面横幅
const float WINDOW_HEIGHT = 720.0f;//画面縦幅
const pair<int, int> IMAGE_ORIZIN = { WINDOW_WIDTH * HALF,WINDOW_HEIGHT * HALF };//画像の座標原点

const float BLINKING_INTERVAL = 1.0f;//点滅の関数を呼び出す間隔

const XMFLOAT2 HP_VIB_RANGE = { 0.1f ,0.05f };//HPの振動幅

const XMFLOAT3 ORIZIN = { 0.0f,0.0f,0.0f };//原点

const XMFLOAT3 DEFAULT_BLOCK_SIZE = { 1.0f,1.0f,0.1f };//ブロック一つのデフォルトサイズ

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

//ゲーム画面で表示するステージ名
const string STAGE_VIEW_NAMES[] = {"STAGE","BEAST","BEENEST","FLOWER","VINE","BROCK"};

//ステージデータのファイル名
const map<string, string> STAGE_FILE_NAMES = {
	{STAGE_VIEW_NAMES[0], "Stage.csv"},
	{STAGE_VIEW_NAMES[1], "StageBeast.csv"},
	{STAGE_VIEW_NAMES[2], "StageBeeNest.csv"},
	{STAGE_VIEW_NAMES[3], "StageFlower.csv"},
	{STAGE_VIEW_NAMES[4], "StageVine.csv"},
	{STAGE_VIEW_NAMES[5], "StageBrock.csv"}
};
const int STAGE_NUM = STAGE_FILE_NAMES.size();//ステージ数

enum BLOCK//ブロックの種類
{
	GROUND = 100,//地面
	GRASS = 101, //草付き地面
	ROCK = 102,//岩
	POISON = 103,//毒
	HALF_GROUND = 105,//半分の地面
	BLOCK_MAX,
};

//ブロックのサイズを持つ配列
const map<BLOCK, XMFLOAT3> blockSizes = {
	{ GROUND,      DEFAULT_BLOCK_SIZE },
	{ GRASS,       DEFAULT_BLOCK_SIZE },
	{ ROCK,        DEFAULT_BLOCK_SIZE },
	{ POISON,      DEFAULT_BLOCK_SIZE },
	{ HALF_GROUND, {1.0f, 0.5f,0.1f} }
};

enum OBJECT//オブジェクトの種類
{
	PLAYER = 99,//プレイヤー
	BEAST = 1,//ビースト
	FLOWER = 2,//花
	NEST = 3,//ハチの巣
	NORMAL_VINE = 30,//通常のツタ
	SPINY_VINE = 31,//棘付きツタ
	LEAF_VINE = 32,//葉付きツタ
	GOAL = 20,//ゴール
};

enum DIR//向き,位置
{
	LEFT,  //左
	RIGHT, //右
	UP,   //上
	DOWN, //下
	CENTER,//中心
	DIR_MAX,
};
const XMFLOAT2 NUM_DIR[DIR_MAX] = { {-1,0},{1,0},{0,1},{0,-1},{0,0} };

enum VINETYPE//ツタの種類
{
	NORMAL,//通常
	SPINY, //棘付き
	LEAF,  //葉付き
	V_MAX
};

enum HPSTATE//HPの状態
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
	bool isActive = true;//描画や当たり判定をするか
};