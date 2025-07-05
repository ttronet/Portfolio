#include "Slider.h"

Slider::Slider(std::string text, int x, int y, int length, float min, float max, void(*callback)(float newValue),float initialValue)
{
	this->girth = 10;
	this->value = initialValue;
	this->min = min;
	this->max = max;
	this->length = length;

	this->sliderLine = new SDL_Rect();
	sliderLine->x = x;
	sliderLine->y = y;
	sliderLine->w = length;
	sliderLine->h = girth;

	float xProgress = (value - min) / (max - min);
	int xProgresPixels = (length - girth) * xProgress;

	this->progressLine = new SDL_Rect();
	progressLine->x = x;
	progressLine->y = y;
	progressLine->w = xProgresPixels;
	progressLine->h = girth;
	
	
	this->selector = new SDL_Rect();
	
	selector->x = x + xProgresPixels;
	selector->y = y-girth;
	selector->w = girth;
	selector->h = girth*3;
	this->callback = callback;
}

void Slider::draw()
{
	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.6f, 0.6f);
	glVertex2i(selector->x, selector->y);
	glVertex2i(selector->x + selector->w, selector->y);
	glVertex2i(selector->x + selector->w, selector->y + selector->h);
	glVertex2i(selector->x, selector->y + selector->h);
	glColor3f(0,0,0);
	glVertex2i(progressLine->x, progressLine->y);
	glVertex2i(progressLine->x + progressLine->w, progressLine->y);
	glVertex2i(progressLine->x + progressLine->w, progressLine->y + progressLine->h);
	glVertex2i(progressLine->x, progressLine->y + progressLine->h);
	glColor3f(0.8f, 0.8f, 0.8f);
	glVertex2i(sliderLine->x, sliderLine->y);
	glVertex2i(sliderLine->x + sliderLine->w, sliderLine->y);
	glVertex2i(sliderLine->x + sliderLine->w, sliderLine->y + sliderLine->h);
	glVertex2i(sliderLine->x, sliderLine->y + sliderLine->h);
	glColor3f(0, 0, 0);
	glEnd();
}

void Slider::mouseDown(Vec2 mousePos)
{
	this->movingSelector = true;
	GameController::getInstance()->setMoveCamera(false);
}

void Slider::mouseUp()
{
	this->movingSelector = false;
	GameController::getInstance()->setMoveCamera(true);
}

void Slider::processMousePos(Vec2 mousePos)
{
	if (!movingSelector)
		return;

	float newProgress = (float)(mousePos.x - progressLine->x)/(float)(length-girth);

	if (newProgress > 1)
		newProgress = 1;
	else if (newProgress < 0)
		newProgress = 0;

	progressLine->w = (length - girth) * newProgress;
	selector->x = progressLine->x + (length-girth) * newProgress;
	this->value = min + (max - min) * newProgress;
	callback(this->value);
}

SDL_Rect* Slider::getRect()
{
	return sliderLine;
}

SDL_Rect* Slider::getSelectorRect()
{
	return selector;
}

BUTTON_TYPE Slider::getType()
{
	return SLIDER;
}

void Slider::setLength(int length)
{
	//Does not work jeje
	//this->length = length;
	//sliderLine->w = length;
	//float xProgress = (value - min) / (max - min);
	//int xProgresPixels = (length - girth) * xProgress;
	//progressLine->w = xProgresPixels;
	//selector->x = sliderLine->x + xProgresPixels;
}
