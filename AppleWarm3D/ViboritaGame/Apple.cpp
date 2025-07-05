#include "Apple.h"

Apple::Apple(Vec3 gridIndexes, Vec3 position) : IGameEntity(gridIndexes,position) {
	this->angle = 0;
}

void Apple::draw() {
	Material appleMaterial = { {0.8f, 0.8f, 0.8f, 1.0f} , {0.1f, 0.1f, 0.1f, 1.0f} ,  {0.4f, 0.4f, 0.4f, 1.0f}, {100.0f} };
	applyMaterial(appleMaterial);
	float TILESIZE = GameController::getInstance()->TILE_SIZE;
	float animProgress = cos(this->angle / 5);
	glPushMatrix();
	glTranslatef(position.x + TILESIZE / 2, position.y + TILESIZE / 6 * animProgress + TILESIZE / 3, position.z + TILESIZE / 2);
	glRotatef(angle * 5, 0, 1, 0);
	glScalef(0.2f * TILESIZE, 0.2f * TILESIZE, 0.2f * TILESIZE);
	drawModel(APPLE_MODEL, false);
	glPopMatrix();

	applyMaterial(defaultMaterial);
}

void Apple::eatApple()
{
	playSound(CRUNCH);
	float TILESIZE = GameController::getInstance()->TILE_SIZE; 
	ParticleSystem* ps = new ParticleSystem(0.001f, 0.2f, 50);
	ps->setLifeSpan(0.2f,0.2f);
	ps->setVelocityRange({ -3,1,-3 }, { 5,5,5 });
	ps->setAccelerationRange({ 0,-0.1f,0 }, { 0,-0.1,0 });
	ps->setPosRange({ position.x,position.y,position.z}, { position.x + TILESIZE, position.y + TILESIZE, position.z + TILESIZE });
	ps->setAlphaRange(0.5f, 1);
	ps->setColorRange({ 0.7f,0,0 }, { 0.9f,0,0 });
	ps->setSizeRange(4, 8);
	GameController::getInstance()->addParticles(ps);
	ParticleSystem* ps2 = new ParticleSystem(0.001f, 0.2f, 50);
	ps2->setLifeSpan(0.2f, 0.2f);
	ps2->setVelocityRange({ -3,1,-3 }, { 5,5,5 });
	ps2->setAccelerationRange({ 0,-0.1f,0 }, { 0,-0.1,0 });
	ps2->setPosRange({ position.x,position.y,position.z }, { position.x + TILESIZE, position.y + TILESIZE, position.z + TILESIZE });
	ps2->setAlphaRange(0.5f, 1);
	ps2->setColorRange({ 0.82f, 0.80f, 0.72f }, { 0.82f, 0.80f, 0.72f });
	ps2->setSizeRange(4, 8);
	GameController::getInstance()->addParticles(ps2);
}

GAME_ENTITY_TYPE Apple::getType()
{
	return APPLE;
}

void Apple::process(float deltaTime) {
	this->angle += 10*deltaTime;
}