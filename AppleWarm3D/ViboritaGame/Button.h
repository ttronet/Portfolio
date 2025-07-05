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
#include <string>
#include "IHudElement.h"

//class HudController;

class Button : public IHudElement
{
private:
	GLuint textureId;
	GLuint hoverTextureId;
	GLuint selectedTextureId;
	SDL_Rect* rectangle;
	
	bool textBtn = false;
	
	bool hover;
	float hoverProgress = 0;

	bool selectable;
	bool selected;
	void (*callback)();
	void (*strcallback)(std::string arg);
	std::string arg;
public:
	Button();
	Button(const char* texturePath, const char* hoverPath, const char* selectedPath, int x, int y, int width, int height, void (*callback)()); //Button ocn hover and selected
	Button(const char* texturePath, const char* hoverPath, int x, int y, int width, int height, void (*callback)()); //Button ocn hover
	Button(const char* texturePath, const char* hoverPath, const char* selectedPath, int x, int y, int width, int height, void (*callback)(std::string arg), std::string arg); //Button ocn hover y un callback con string
	Button(const char* texturePath, const char* hoverPath, int x, int y, int width, int height, void (*callback)(std::string arg), std::string arg); //Button ocn hover y un callback con string
	Button(const char* texturePath, int x, int y, int width, int height, void (*callback)()); //Button sin hover
	Button(int x, int y, int width, int height, void (*callback)(std::string arg),std::string arg); //Button invisible
	Button(const char* texturePath, int x, int y,std::string kiki); //Renderiza imagen ocn tama;o de la imagen

	Button(const char* text, int x, int y); //Renderiza solo texto
	Button(const char* text, int x, int y, void (*callback)(std::string arg), std::string arg, bool selectable, bool selected); //Button texto y callback con string
	void draw();
	void process(float deltaTime);
	BUTTON_TYPE getType();
	void handleClick();
	void handleHover();
	bool isHovering();
	float getHoverProgress();
	void updateText(const char* texturePath); //Only when text rendering
	SDL_Rect* getRect();
	void setSelected(bool selected);
	void center(int begin, int end);
	~Button();
};

