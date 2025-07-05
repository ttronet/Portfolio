#pragma once
#include "GameController.h"
#include "IHudElement.h"
#include "Level.h"
#include "utils.h"

class LevelButton : public IHudElement
{
private:
	SDL_Rect* rect;
	Level* level;
	int x = 1; 
	bool hover;
	float angle = 0;
	float tileScale = 1;
	float aProgress = 0;
	Vec2 textSize;
	GLuint levelTextId;
	Vec2 timerSize;
	GLuint timerTextId;
	Vec2 scoreSize;
	GLuint scoreTextId;
	bool beat;
	void (*callback)(std::string);
public:
	LevelButton(int x, Level* level, void (*callback)(std::string));
	void draw();
	void process(float deltaTime);
	BUTTON_TYPE getType();
	SDL_Rect* getRect();

	void handleHover();
	bool isHovering();
	void click();

	void resize();
};

