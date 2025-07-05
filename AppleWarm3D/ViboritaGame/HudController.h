#pragma once
#include "utils.h"
#include <iostream>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_opengl.h"
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include <SDL_image.h>
#include <string>
#include "GameController.h"
#include "Button.h"
#include "Slider.h"
#include "TextField.h"

class HudController
{
private:
	static HudController* instance;
	HudController();
	TTF_Font* mainFont;
public:
	static HudController* getInstance();
	void process(float deltaTime); //Draw hud and check for button clicks
	void setFont(TTF_Font* mainFont);
	TTF_Font* getFont();
};

