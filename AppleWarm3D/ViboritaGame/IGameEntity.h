#pragma once
#include <set>
#include "utils.h"

class IGameEntity
{
protected:
	Vec3 gridIndexes;
	Vec3 position;
public:
	IGameEntity(Vec3 gridIndexes, Vec3 position);
	virtual void process(float deltaTime);
	virtual void draw();
	virtual GAME_ENTITY_TYPE getType();
	virtual ~IGameEntity() {}
};

