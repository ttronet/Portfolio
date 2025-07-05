#include "IGameEntity.h"

void IGameEntity::draw() {

}

GAME_ENTITY_TYPE IGameEntity::getType()
{
	return GAME_ENTITY_TYPE();
}

IGameEntity::IGameEntity(Vec3 gridIndexes, Vec3 position)
{
	this->gridIndexes = gridIndexes;
	this->position = position;
}

void IGameEntity::process(float deltaTime) {

}
