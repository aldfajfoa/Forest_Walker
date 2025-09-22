#pragma once
#include "Engine//GameObject.h"
#include <string>
using std::string;
#include<vector>
using std::vector;
#include<map>
using std::map;

class GameData :
    public GameObject
{
	vector<string> stageViewNames_;
	map<string, string> stageFileNames_;
public:
	GameData(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override {};

	////ÉQÉbÉ^Å[////
	vector<string> GetStageViewNames(){return stageViewNames_;}
	map<string, string> StageFileNames(){return stageFileNames_;}
};

