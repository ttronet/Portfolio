#include "Level.h"

Level::Level(std::string name, std::string nextLevelName, IGameEntity* initialGrid[8][8][8], int maxScore, Viborita* initialViborita)
{
	this->name = name;
	this->nextLevelName = nextLevelName;
	this->initialViborita = initialViborita;
	this->maxScore = maxScore;
	for (int x = 0; x < 8;x++) {
		for (int y = 0; y < 8;y++) {
			for (int z = 0; z < 8;z++) {
				this->initialGrid[x][y][z] = initialGrid[x][y][z];
			}
		}
	}
}

IGameEntity* Level::getInitialGridPosition(int x, int y, int z)
{
	return initialGrid[x][y][z];
}

Viborita* Level::getInitialViborita()
{
	return initialViborita->deepCopy();
}

int Level::getMaxScore()
{
	return maxScore;
}

std::string Level::getName()
{
	return name;
}

std::string Level::getNextLevelName()
{
	return nextLevelName;
}

void Level::drawPreview(int gridSize)
{
	glEnable(GL_LIGHTING);
	glNormal3f(0.0f, 1.0f, 0.0f);
	for (int x = 0; x < gridSize;x++)
		for (int y = 0; y < gridSize;y++)
			for (int z = 0; z < gridSize;z++)
				if (initialGrid[x][y][z] != NULL && initialGrid[x][y][z] != this->initialViborita)
					initialGrid[x][y][z]->draw();

	this->initialViborita->draw();
	glDisable(GL_LIGHTING);
}

void Level::setNo(int no)
{
	this->no = no;
}

int Level::getNo()
{
	return no;
}

