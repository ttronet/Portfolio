#pragma once
#include "IGameEntity.h"
#include "GameController.h"
#include "utils.h"

class SpikedApple :
    public IGameEntity
{
private:
	float angle = 0;
public:
	SpikedApple(Vec3 girdIndex, Vec3 position);
	GAME_ENTITY_TYPE getType();
	void process(float deltaTime);
	void draw();
};

