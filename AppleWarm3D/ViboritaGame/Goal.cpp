#include "Goal.h"
#include "GameController.h"

Goal::Goal(Vec3 gridIndexes, Vec3 position) : IGameEntity(gridIndexes, position)
{
	this->position = position;
	this->gridIndex = gridIndexes;
	this->angle = 0.0f;
	this->angle += 0.05f;
	

	ps = NULL;
}

void Goal::draw()
{
	//TODO: arreglar giro de la meta
	glPushMatrix();
	Material goalMaterial = { {0.8f, 0.8f, 0.8f, 1.0f} , {0.1f, 0.1f, 0.1f, 1.0f} ,  {0.4f, 0.4f, 0.4f, 1.0f}, {100.0f} };
	applyMaterial(goalMaterial);
	glTranslatef(position.x + 0.5, position.y-1.2f, position.z); //TODO sacar estos valores hardcodeados, que sean en base a TILE_SIZE
	float scale = cos(angle) * 0.1;
	glScalef(0.5 + scale, 0.5, 0.5 + scale);
	glRotatef(90, 0, 1, 0);
	//glColor3f(0, 0, 0); //color negro
	drawModel(GOAL_MODEL, GameController::getInstance()->getSettings()->hasTextures());
	glPopMatrix();
	applyMaterial(defaultMaterial);
}

GAME_ENTITY_TYPE Goal::getType()
{
	return GOAL;
}

Goal::~Goal()
{
	GameController::getInstance()->removeParticles(ps);
}

void Goal::process(float deltaTime){  
	this->angle += 1.0f * deltaTime; 
	if (ps == NULL) {
		float TILESIZE = GameController::getInstance()->TILE_SIZE;
		ps = new ParticleSystem(0.3f, 300.0f, 30);
		ps->setVelocityRange({ -0.5,0.5f,-0.5 }, { 0.5,1.5f,0.5 });
		ps->setAccelerationRange({ 0,-0.004f,0 }, { 0,-0.004,0 });
		ps->setPosRange({ position.x,position.y,position.z + TILESIZE / 2 }, { position.x + TILESIZE, position.y + TILESIZE, position.z + TILESIZE / 2 });
		ps->setAlphaRange(0.5f, 1);
		ps->setColorRange({ 0,0,0 }, { 0.5f,0,0.5f });
		ps->setSizeRange(1, 6);
		GameController::getInstance()->addParticles(ps);
	}
}
