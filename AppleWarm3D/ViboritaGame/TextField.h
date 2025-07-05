#pragma once
#include "IHudElement.h"

class TextField : public IHudElement
{
private:
	int height;
	SDL_Rect* rect;
	SDL_Rect* indicator;
	SDL_Rect* textRect;
	SDL_Rect* placeholderRect;
	bool selected;
	GLuint textTextureId;
	GLuint placeHolderTextureId;
	std::string keyPressed;
	std::string value;
	int maxValueSize;
	float angle = 0;
	int charactedLength;
	void (*callback)(std::string newValue);

	void changeText(std::string newText);
public:
	TextField(std::string text,int x,int y,int length, void (*callback)(std::string newValue), std::string initialValue);
	void draw();
	void process(float deltaTime);
	void processKeyPress();
	BUTTON_TYPE getType();
	SDL_Rect* getRect();

	void click(Vec2 mousePos);
	void clickOutside();
	void setKeyPressed(std::string key);

	bool isSelected();
};

