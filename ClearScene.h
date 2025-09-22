#pragma once
#include "Engine/GameObject.h"
#include "Scene.h"

class ClearScene:
	public GameObject,public Scene
{
public:
	ClearScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override {};
};

