#pragma once
#include "utils.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <GL/glu.h>
#include <vector>
#include "IGameEntity.h"
//#include "GamePlay.h"

class GamePlay;

class Viborita :
	public IGameEntity
{
private:
	GamePlay* gameContext;
	GLfloat viboritaColors[24];
	ViboritaBody body;
	Vec3 headDirection;
	Vec3 prevMovement;
	float falling;
	bool fallen;
	float movingProgress;
	bool dead = false;

	Vec3 movingIntoWallDir;

	bool reachedPortal = false;
	bool dying = false;
public:
	Viborita(Vec3 gridPosition,Vec3 position, GLfloat colors[24]);
	void setGameContext(GamePlay* context);
	void process(float deltaTime);
	void draw();
	ViboritaPart* getHead();
	int getSize();
	void setHead();
	void addTail(Vec3 gridPosition);
	void loadInGrid();
	Vec3* getMovementDirection();
	bool handleMovement(Vec3* movementDir, bool die);
	void handleEatApple(Vec3* lastTailPos,Vec3* lastTailGrid);
	bool hasFloor();
	void handleFall();
	void handleDeath();
	void removeBodyFromGridIndex(int x,int y, int z);
	void processAnimation(Vec3* movementDir, float deltaTime);
	Viborita* deepCopy();
	GAME_ENTITY_TYPE getType();

	void getFPCamera(Vec3& pos, Vec3& center);
};

