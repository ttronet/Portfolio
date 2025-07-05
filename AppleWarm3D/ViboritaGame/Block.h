#pragma once
#include "utils.h"
#include "IGameEntity.h"
#include "GamePlay.h"

class Block :
	public IGameEntity
{
private:
	GamePlay* context;
public:
	Block(Vec3 gridIndexes, Vec3 position);
	void setContext(GamePlay* context);
	void process(float deltaTime);
	void draw();
	GAME_ENTITY_TYPE getType();
};

