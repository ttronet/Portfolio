#pragma once
#include "utils.h"
#include "IGameEntity.h"
#include "Viborita.h"
class Level
{
private:
	int no = 0;

	std::string name;
	std::string nextLevelName;
	IGameEntity* initialGrid[8][8][8];
	int maxScore;
	Viborita* initialViborita;
public:
	Level(std::string name, std::string nextLevelName, IGameEntity* initialGrid[8][8][8],int maxScore, Viborita* initialViborita);

	Viborita* getInitialViborita();
	IGameEntity* getInitialGridPosition(int x, int y, int z);
	int getMaxScore();
	std::string getName();
	std::string getNextLevelName();
	void drawPreview(int gridSize);

	void setNo(int no);
	int getNo();
};

