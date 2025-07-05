#include "Block.h"
#include "GameController.h"

Block::Block(Vec3 gridIndexes, Vec3 position) : IGameEntity(gridIndexes,position) {
	this->context = NULL;
}

void Block::setContext(GamePlay* context)
{
	this->context = context;
}

void Block::draw() {
	glPushMatrix();
	glTranslatef(position.x,position.y,position.z);
	float TILESIZE = GameController::getInstance()->TILE_SIZE;
	glScalef(TILESIZE, TILESIZE, TILESIZE);
	bool blockAbove = context != NULL && context->hasSolidBlock({ gridIndexes.x,gridIndexes.y + 1,gridIndexes.z });
	drawCubeWithNormals({0.3,0.3,0.3}, GameController::getInstance()->getSettings()->hasTextures(), blockAbove ? dirtTexture : grassTexture);
	glPopMatrix();
}

GAME_ENTITY_TYPE Block::getType()
{
	return BLOCK;
}

void Block::process(float deltaTime) {

}