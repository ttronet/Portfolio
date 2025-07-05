#pragma once
#include "utils.h"
#include "IGameEntity.h"
#include "ParticleSystem.h"

class Goal :
    public IGameEntity
{
private:
	Vec3 position;
	Vec3 gridIndex;
	float angle;
	GLfloat goal[12];
	ParticleSystem* ps;
public:
	Goal(Vec3 gridIndexes, Vec3 position);
	void process(float deltaTime);
	void draw();
	GAME_ENTITY_TYPE getType();
	Vec3 getPosition() { return this->position; }
	Vec3 getGridIndexes() { return this->gridIndexes; }
	~Goal();
};

