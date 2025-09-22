#pragma once
#include "Engine/GameObject.h"
#include "Scene.h"

class OverScene :
    public GameObject, public Scene
{
public:
	OverScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override {};
};

