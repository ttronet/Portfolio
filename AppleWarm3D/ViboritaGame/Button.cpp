#include "Button.h"

Button::Button()
{
}

Button::Button(const char* texturePath, const char* hoverPath, const char* selectedPath, int x, int y, int width, int height, void(*callback)())
{
	hover = false;
	selected = false;

	loadTexture(this->textureId, texturePath);
	loadTexture(this->hoverTextureId, hoverPath);
	loadTexture(this->selectedTextureId, selectedPath);

	this->rectangle = new SDL_Rect();
	this->rectangle->x = x;
	this->rectangle->y = y;
	this->rectangle->w = width;
	this->rectangle->h = height;

	this->callback = callback;
	this->strcallback = NULL;
}

Button::Button(const char* texturePath, const char* hoverPath, int x, int y, int width, int height, void (*callback)())
{//Btn con hover
	hover = false;
	selected = false;

	loadTexture(this->textureId, texturePath);
	loadTexture(this->hoverTextureId, hoverPath);
	this->selectedTextureId = NULL;

	this->rectangle = new SDL_Rect();
	this->rectangle->x = x;
	this->rectangle->y = y;
	this->rectangle->w = width;
	this->rectangle->h = height;

	this->callback = callback;
	this->strcallback = NULL;
}

Button::Button(const char* texturePath, const char* hoverPath, const char* selectedPath, int x, int y, int width, int height, void(*callback)(std::string arg), std::string arg)
{
	hover = false;
	selected = false;

	loadTexture(this->textureId, texturePath);
	loadTexture(this->hoverTextureId, hoverPath);
	loadTexture(this->selectedTextureId, selectedPath);

	this->rectangle = new SDL_Rect();
	this->rectangle->x = x;
	this->rectangle->y = y;
	this->rectangle->w = width;
	this->rectangle->h = height;

	this->callback = NULL;
	this->strcallback = callback;
	this->arg = arg;
}

Button::Button(const char* texturePath, const char* hoverPath, int x, int y, int width, int height, void(*callback)(std::string arg),std::string arg)
{
	hover = false;
	selected = false;

	loadTexture(this->textureId, texturePath);
	loadTexture(this->hoverTextureId, hoverPath);
	this->selectedTextureId = NULL;

	this->rectangle = new SDL_Rect();
	this->rectangle->x = x;
	this->rectangle->y = y;
	this->rectangle->w = width;
	this->rectangle->h = height;

	this->callback = NULL;
	this->strcallback = callback;
	this->arg = arg;
}

Button::Button(const char* texturePath, int x, int y, int width, int height, void(*callback)())
{//Btn sin hover
	hover = false;
	selected = false;

	loadTexture(this->textureId, texturePath);
	this->hoverTextureId = NULL;
	this->selectedTextureId = NULL;

	this->rectangle = new SDL_Rect();
	this->rectangle->x = x;
	this->rectangle->y = y;
	this->rectangle->w = width;
	this->rectangle->h = height;

	this->callback = callback;
	this->strcallback = NULL;
}

Button::Button(int x, int y, int width, int height, void(*callback)(std::string arg), std::string arg)
{
	this->textureId = NULL;
	this->hoverTextureId = NULL;
	this->selectedTextureId = NULL;
	this->rectangle = new SDL_Rect();
	this->rectangle->x = x;
	this->rectangle->y = y;
	this->rectangle->w = width;
	this->rectangle->h = height;

	this->callback = NULL;
	this->strcallback = callback;
	this->arg = arg;
}

Button::Button(const char* texturePath, int x, int y, std::string kiki)
{
	hover = false;
	selected = false;
	float width = 0;
	float height = 0;
	
	loadTexture(this->textureId, texturePath,width,height);

	this->hoverTextureId = NULL;
	this->selectedTextureId = NULL;

	this->rectangle = new SDL_Rect();
	this->rectangle->x = x;
	this->rectangle->y = y;
	this->rectangle->w = width;
	this->rectangle->h = height;

	this->callback = callback;
	this->strcallback = NULL;
}

Button::Button(const char* text, int x, int y)
{//Renderizador de texto xdxd
	hover = false;
	selected = false;
	textBtn = true;
	int width = 0;
	int height = 0;
	loadTextTexture(this->textureId, text, font,width,height);
	this->hoverTextureId = NULL;

	this->rectangle = new SDL_Rect();
	this->rectangle->x = x;
	this->rectangle->y = y;
	this->rectangle->w = width;
	this->rectangle->h = height;
	this->callback = NULL;
	this->strcallback = NULL;
}

Button::Button(const char* text, int x, int y, void(*callback)(std::string arg), std::string arg,bool selectable, bool selected)
{
	this->selectable = selectable;
	textBtn = true;
	hover = false;
	this->selected = selected;
	int width = 0;
	int height = 0;
	loadTextTexture(this->textureId, text, font, width,height);
	this->hoverTextureId = NULL;

	this->rectangle = new SDL_Rect();
	this->rectangle->x = x;
	this->rectangle->y = y;
	
	GLuint id = textureId;
	this->rectangle->w = width;
	this->rectangle->h = height;
	this->callback = NULL;
	this->strcallback = callback;
	this->arg = arg;
}

void Button::draw()
{
	if (textureId != NULL) {
		glBindTexture(GL_TEXTURE_2D, hoverTextureId != NULL && isHovering() ? hoverTextureId : textureId);
		glEnable(GL_TEXTURE_2D);
	}
	else
		return;
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //debug rectangle

	
	float alpha = selectable && !selected ? 0.3f : 1;

	int yOffset = 0;
	glColor4f(hoverProgress, hoverProgress, hoverProgress,alpha);
	yOffset = -rectangle->h * hoverProgress;
	
	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(rectangle->x, yOffset + rectangle->y);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(rectangle->x + rectangle->w, yOffset + rectangle->y);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(rectangle->x + rectangle->w, yOffset + rectangle->y + rectangle->h);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(rectangle->x, yOffset + rectangle->y + rectangle->h);
	glEnd();
	glColor4f(0, 0, 0, 1);

	if (textureId != NULL) 
		glDisable(GL_TEXTURE_2D);
	//else
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Button::process(float deltaTime)
{
	if (hover) 
		hoverProgress += hoverProgress < 0.3 ? 2.0f * deltaTime : 0;
	else if(hoverProgress > 0)
		hoverProgress -= 2.0f * deltaTime;
	
}

void Button::handleClick()
{
	if(callback != NULL)
		this->callback();
	if (this->selectable)
	{
		this->setSelected(!selected);
		this->strcallback(selected ? "true" : "false");
	}
	else if (strcallback != NULL)
		this->strcallback(arg);

}

void Button::handleHover()
{
	if(callback != NULL || strcallback != NULL)
	{
		hover = !hover;
		if (hover)
			playSound(HOVER);
	}
}

bool Button::isHovering()
{
	return hover;
}

float Button::getHoverProgress()
{
	return hoverProgress;
}

void Button::updateText(const char* newText)
{
	glDeleteTextures(1, &textureId);
	int width = 0;
	int height = 0;
	loadTextTexture(this->textureId, newText, font,width,height);
	this->rectangle->w = width;
	this->rectangle->h = height;
}

SDL_Rect* Button::getRect()
{
	return this->rectangle;
}

void Button::setSelected(bool selected)
{
	this->selected = selected;
}

void Button::center(int begin, int end)
{
	rectangle->x = begin + (end - begin) / 2 - rectangle->w / 2;
}

Button::~Button()
{
	if(textBtn && textureId != NULL)
		glDeleteTextures(1, &textureId);
}

BUTTON_TYPE Button::getType()
{
	return BUTTON;
}
