#pragma once
#include "utils.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_ttf.h"
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include <SDL_image.h>
#include "IHudElement.h"
#include "GameController.h"

//class GameController;

class Slider :
	public IHudElement
{
private:
	std::string text;
	SDL_Rect* area;
	SDL_Rect* sliderLine; //Estos dos son los dos rectangulos del slider:
	SDL_Rect* progressLine; //Estos dos son los dos rectangulos del slider:
	SDL_Rect* selector;		// 0 ------|---- 100
	int girth;
	int length;
	bool movingSelector;
	float min;
	float max;
	float value;
	void (*callback)(float newValue);
public:
	Slider(std::string text,int x, int y, int length, float min, float max, void (*callback)(float newValue), float initialValue);
	void draw();
	void mouseDown(Vec2 mousePos);
	void mouseUp();
	void processMousePos(Vec2 mousePos);
	virtual SDL_Rect* getRect();
	virtual SDL_Rect* getSelectorRect();
	BUTTON_TYPE getType();
	void setLength(int length);
};

