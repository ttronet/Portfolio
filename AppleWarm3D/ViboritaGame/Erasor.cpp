#include "Erasor.h"

Erasor::Erasor(Vec3 girdIndex, Vec3 position) : IGameEntity(gridIndexes,position)
{
}

GAME_ENTITY_TYPE Erasor::getType()
{
	return ERASOR;
}

void Erasor::process(float deltaTime)
{
	this->angle += 10.0f * deltaTime;

}

void Erasor::draw()
{
	float TILESIZE = GameController::getInstance()->TILE_SIZE;
	float animProgress = cos(this->angle / 5);
	glPushMatrix();
	glTranslatef(position.x + TILESIZE / 2, position.y + TILESIZE / 6 * animProgress + TILESIZE / 3, position.z + TILESIZE / 2);
	glRotatef(angle * 5, 0, 1, 0);
	glScalef(0.5f * TILESIZE, 0.5f * TILESIZE, 0.5f * TILESIZE);
	drawModel(ERASOR_MODEL, false);
	glPopMatrix();
}
