#include "SpikedApple.h"

SpikedApple::SpikedApple(Vec3 gridIndexes, Vec3 position) : IGameEntity(gridIndexes, position)
{
}

void SpikedApple::process(float deltaTime)
{
	this->angle += 10.0f * deltaTime;
}

void SpikedApple::draw()
{
	float TILESIZE = GameController::getInstance()->TILE_SIZE;
	float animProgress = cos(this->angle/5);
	glPushMatrix();
	glTranslatef(position.x + TILESIZE/2, position.y + TILESIZE/6 * animProgress + TILESIZE / 3, position.z + TILESIZE / 2);
	glRotatef(angle*5,0,1,0);
	glScalef(0.2f * TILESIZE, 0.2f * TILESIZE, 0.2f * TILESIZE);
	drawModel(SPIKED_APPLE_MODEL, false);
	glPopMatrix();
}

GAME_ENTITY_TYPE SpikedApple::getType() {
	return SPIKED_APPLE;
}