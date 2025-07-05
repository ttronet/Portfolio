#pragma once
#include "utils.h"

class IHudElement
{
protected:
	static TTF_Font* font;

public:
	IHudElement();
	virtual void draw();
	virtual void process(float deltaTime);
	virtual BUTTON_TYPE getType();
	virtual SDL_Rect* getRect();
	virtual ~IHudElement() {}
	static void setFont(TTF_Font* font);
	static void goBackTo3d();
	static void goBackTo2d();
};

