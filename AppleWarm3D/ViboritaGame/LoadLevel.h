#pragma once
#include "IGameState.h"
#include "Button.h"

class LoadLevel :
    public IGameState
{
private:
	bool loading;
	void readLevelFile();
	void processLevel();
	void startGameplay();
public:
	void process(float deltaTime);
	void draw();
	virtual std::vector<Button*> getHudButtons();
};

