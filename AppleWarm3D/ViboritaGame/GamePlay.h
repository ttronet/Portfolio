#pragma once
#include "IGameState.h"
#include "Button.h"
#include "Level.h"
#include "GameStats.h"
#include "GameController.h"

class LevelCreator;

class GamePlay :
    public IGameState
{
private:
	Button* settings;
	Button* reset;
	Button* scoreText;
	Button* timerText;
	Level* level;
	IGameEntity* grid[8][8][8];
	Viborita* viborita;
	GameStats* stats;

	Button* cameraIcon;
	Button* firstPersonPerspective;
	Button* levelPerspective;
	Button* freePerspective;

	bool showWarning = false;
	Button* resetWarning;
	float angle;

	Button* tutorials[20];

	LevelCreator* lc = NULL;
public:
	GamePlay(Level* level);
	GamePlay(Level* level, LevelCreator* levelCreator);
	void process(float deltaTime);
	void changeTimer(int time);
	void beatLevel();
	void startLevel();
	void resetLevel();
	void viboritaDied();
	void addSecond();
	void draw();
	std::vector<IHudElement*> getHudElements();

	Viborita* getViborita();
	bool tileHasApple(Vec3 indices);
	void clearTile(Vec3 indices);
	bool validTile(Vec3 indices);
	bool hasSolidBlock(Vec3 indices);
	bool hasViborita(Vec3 indices);
	bool hasSpikedApple(Vec3 indices);
	bool hasGoal(Vec3 indices);
	void addViborita(Vec3 indices);
	void eatAppleAt(Vec3 indices);
	void ateApple();

	void setFirstPerson();
	void setLevelPerspective();
	void setFreePerspective();

	std::vector<IHudElement*> getLevelTutorials();
	
	void resize();
	
	~GamePlay();
};

