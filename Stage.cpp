#include "Stage.h"
#include "Engine/BoxCollider.h"
#include "Engine/CsvReader.h"
#include "Engine/Image.h"
#include "Engine/Model.h"
#include "Beast.h"
#include "Enemys.h"
#include "Flower.h"
#include "Nest.h"
#include "Player.h"
#include "PlayScene.h"
#include "Goal.h"
#include "Vine.h"

bool Stage::DecideIsRight(const string& object)
{
	CsvReader csv;
	return (csv.extractString(object) == "R");
}

DIR Stage::DecideIsDir(const string& object)
{
	CsvReader csv;
	if (csv.extractString(object) == "L")
	{
		return LEFT;
	}
	else if (csv.extractString(object) == "R")
	{
		return RIGHT;
	}
	else if (csv.extractString(object) == "U")
	{
		return UP;
	}
	else if (csv.extractString(object) == "D")
	{
		return DOWN;
	}
	else
	{
		return LEFT;
	}
}

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage"), player_(nullptr), hActionUI_(NULL_NUM), hBack_(NULL_NUM), blockDamage_(0), notDrawDist_(0.0f),
	actionUIPos_({ 0,0 }), backSize_({ 0,0 }), width_(0), height_(0)
{
	hBlocks_.clear();
	blocks_.clear();
}

void Stage::Initialize()
{
	//csvからデータ読み込み
	CsvReader csv;//データを読むクラスのインスタンスを作成
	bool ret = csv.Load("CsvData\\Stage\\StageData.csv");
	assert(ret);
	int row = 1, column = 0;

	//ハンドルにデータをセット
	//ブロックのモデルを設定
	hBlocks_[GROUND] = Model::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	hBlocks_[HALF_GROUND] = Model::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	hBlocks_[GRASS] = Model::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	hBlocks_[ROCK] = Model::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	hBlocks_[POISON] = Model::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	for (auto brock = hBlocks_.begin(); brock != hBlocks_.end(); ++brock)
	{
		assert(brock->second > NULL_NUM);
	}

	hActionUI_ = Image::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hActionUI_ > NULL_NUM);
	hBack_ = Image::Load(csv.extractString(csv.GetString(row, csv.ReturnAndAddIndex(column))));
	assert(hBack_ > NULL_NUM);

	blockDamage_ = csv.extractInt(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	notDrawDist_ = csv.extractFloat(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	actionUIPos_ = csv.extractFloat2(csv.GetString(row, csv.ReturnAndAddIndex(column)));
	actionUIpos_.position_.x = actionUIPos_.x;
	actionUIpos_.position_.y = actionUIPos_.y;
	backSize_ = csv.extractFloat2(csv.GetString(row, csv.ReturnAndAddIndex(column)));
}

void Stage::SetMap(const string& stageFileName)
{
	CsvReader csv;//データを読むクラスのインスタンスを作成
	bool ret = csv.Load(string("CsvData\\MapData\\") + stageFileName);
	assert(ret);

	width_ = csv.GetWidth(0);//横幅を取得
	height_ = csv.GetHeight();//縦幅を取得

	vector<vector<BlockData>> map;
	map.resize(height_, vector<BlockData>(width_));//サイズを変更

	int wholeheight = height_;//全体の縦幅
	height_ = (wholeheight - INDEX_AJM) * HALF;//マップの縦幅取得

	for (int h = 0; h < height_; h++)
	{
		for (int w = 0; w < width_; w++)
		{
			BlockData block;
			block.blockID = csv.GetInt(w, h);

			switch (block.blockID)
			{
			case GROUND:
			case GRASS:
			case ROCK:
			case POISON:
			case HALF_GROUND:
			{
				XMFLOAT3 bSize = blockSizes.at((BLOCK)block.blockID);
				block.boxColl = new BoxCollider({ (float)w,((float)-h - DEFAULT_BLOCK_SIZE.y * HALF) + (bSize.y * HALF),0 }, bSize);
				AddCollider(block.boxColl);
				blocks_.push_back(block);

				break;
			}
			default:
				break;
			}
		}
	}

	PlayScene* playscene = (PlayScene*)GetParent();
	Enemys* enemys = playscene->GetPlayScene_enemys();

	for (int h = 0; h < height_; h++)
	{
		for (int w = 0; w < width_; w++)
		{
			string object = csv.GetString(w, h + height_ + INDEX_AJM);
			int objNum = csv.extractNumber(object);
			bool isRight = DecideIsRight(object);

			switch (objNum)
			{
			case PLAYER://プレイヤー
			{
				player_ = GetParent()->FindGameObject<Player>();
				player_->SetPosition(w, -h);
				player_->SetGround(-h);
				player_->SetisRight(isRight);
				break;
			}
			case BEAST://ビースト
			{
				Beast *b = Instantiate<Beast>(this);
				b->SetPosition(w, -h);
				b->SetGround(-h);
				b->SetisRight(isRight);
				enemys->AddEnemy(b);
				break;
			}
			case FLOWER://花
			{
				Flower* f = Instantiate<Flower>(this);
				f->SetPosition(w, -h);
				f->SetGround(-h);
				f->SetisRight(isRight);
				enemys->AddEnemy(f);
				break;
			}
			case NEST://ハチの巣
			{
				Nest* n = Instantiate<Nest>(this);
				n->SetPosition(w, -h);
				n->SetisRight(isRight);
				enemys->AddEnemy(n);
				break;
			}
			case GOAL://ゴール
			{
				Goal* g = Instantiate<Goal>(this);
				g->SetPosition(w, -h);
				g->SetisRight(isRight);
				break;
			}
			case NORMAL_VINE://通常のツタ
			case SPINY_VINE://棘付きツタ
			case LEAF_VINE://葉付きツタ
			{
				Vine* v = Instantiate<Vine>(this);
				v->SetPosition(w, -h);
				if (objNum == SPINY_VINE){
					v->SetvType(SPINY);
				}
				else if (objNum == LEAF_VINE){
					v->SetvType(LEAF);
				}
				else{
					v->SetvType(NORMAL);
				}
				v->SetvDir(DecideIsDir(object));
				playscene->AddVine(v);
				break;
			}
			break;
			}
		}
	}

	player_->SetCameraStopandDeathGround_(-height_);
}

void Stage::Update()
{
	Transform t;
	for (int b = 0; b < blocks_.size(); b++)
	{
		t.position_.x = blocks_[b].boxColl->Getcenter().x;
		t.position_.y = blocks_[b].boxColl->Getcenter().y;
		//プレイヤーと一定以上離れているか
		if (CalculationDist(t.position_, player_->GetPosition()) >= notDrawDist_)
		{
			blocks_[b].isActive = false;
		}
		else
		{
			blocks_[b].isActive = true;
		}
	}
}

void Stage::Draw()
{
	//背景の描画
	Transform back;
	back.scale_.x = backSize_.x;
	back.scale_.y = backSize_.y;
	Image::SetTransform(hBack_, back);
	Image::Draw(hBack_);

	Transform t;
	for (int b = 0; b < blocks_.size(); b++)
	{
		switch (blocks_[b].blockID)
		{
		case GROUND:
		case GRASS:
		case ROCK:
		case POISON:
		case HALF_GROUND:
		{
			t.position_.x = blocks_[b].boxColl->Getcenter().x;
			t.position_.y = blocks_[b].boxColl->Getcenter().y;

			if (blocks_[b].isActive)//プレイヤーと一定以上離れてたら描画しない
			{
				Model::SetTransform(hBlocks_[(BLOCK)blocks_[b].blockID], t);
				Model::Draw(hBlocks_[(BLOCK)blocks_[b].blockID]);
			}
			break;
		}
		default:
			break;
		}
	}

	Image::SetTransform(hActionUI_, actionUIpos_);
	Image::Draw(hActionUI_);
}

void Stage::Release()
{
	hBlocks_.clear();
	stageboxColls_.clear();
	blocks_.clear();
}
