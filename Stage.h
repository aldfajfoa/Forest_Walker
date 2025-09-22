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
	////ポインタ////
	vector<BoxCollider*> stageboxColls_;
	Player* player_;

	////ハンドル////
    //モデル
	map<BLOCK, int> hBlocks_;//ブロックのハンドルを持つ配列
	//画像
	int hActionUI_;//操作方法の画像
	int hBack_;//背景

	////csvから値を受け取る変数////
	int blockDamage_;//ダメージを受けるブロックでどのくらいダメージを受けるか
	float notDrawDist_;//ブロックがこの距離プレイヤーから離れたら描画しなくなる
	XMFLOAT2 actionUIPos_;//操作方法の表示位置
	XMFLOAT2 backSize_;//背景サイズ

	////その他の変数////
	int width_, height_; //ステージ幅,高さ
	Transform actionUIpos_;//操作方法の表示位置
	vector<BlockData> blocks_;//マップのデータを持つ配列

	////関数////
	bool DecideIsRight(const string& object);//右か左かのみで返す
	DIR DecideIsDir(const string& object);//四方向のどれかを返す
public:
	Stage(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	////セッター////
	void SetMap(const string& stageFileName);//ステージの生成

	////ゲッター////
	int Getwidth() { return width_; }
	int Getheight() { return height_; }
	int GetBlockID(const int& index) { return blocks_[index].blockID; }
	int GetBlocksSize() { return blocks_.size(); }
	int GetBlockDamage() { return blockDamage_; }
	bool GetisActive(const int& index) { return blocks_[index].isActive; }
	BoxCollider* GetboxColl(const int& index) { return blocks_[index].boxColl; }
};

