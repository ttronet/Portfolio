#include "TextField.h"

void TextField::changeText(std::string newText)
{
	if(textTextureId != NULL)
		glDeleteTextures(1, &textTextureId);
	int width = 0;
	int height = 0;
	loadTextTexture(this->textTextureId, newText.c_str(), font, width, height);
	this->textRect->w = width;
	this->textRect->h = height;
}

TextField::TextField(std::string text, int x, int y, int length, void (*callback)(std::string newValue), std::string initialValue) {
	this->height = 32;
	this->charactedLength = 18;
	this->value = initialValue;
	this->textTextureId = NULL;
	this->rect = new SDL_Rect();
	rect->x = x;
	rect->y = y;
	rect->h = height;
	rect->w = length;
	this->indicator = new SDL_Rect();
	indicator->x = x;
	indicator->y = y;
	indicator->h = height;
	indicator->w = 2;

	this->textRect = new SDL_Rect();
	textRect->x = x;
	textRect->y = y+2;
	textRect->w = 0;
	textRect->h = 0;

	this->placeholderRect = new SDL_Rect();
	placeholderRect->x = x;
	placeholderRect->y = y + 2;
	placeholderRect->w = 0;
	placeholderRect->h = 0;
	if(initialValue != "")
		changeText(initialValue);

	if (text != "") {
		int width = 0;
		int height = 0;
		loadTextTexture(this->placeHolderTextureId, text.c_str(), font, width, height);
		this->placeholderRect->w = width;
		this->placeholderRect->h = height;
	}

	this->maxValueSize = 11;

	this->callback = callback;
}

void TextField::draw()
{
	SDL_Rect* rectForText = textRect;
	GLuint textId = textTextureId;
	float alpha = 1;
	if (textTextureId == NULL) {
		rectForText = placeholderRect;
		textId = placeHolderTextureId;
		alpha = 0.5;
	}

	if (selected && cos(angle) > 0) {
		glBegin(GL_QUADS);
		//Renderizo el rectangulo del indicador
		glColor3f(0.2f, 0.2f, 0.2f);
		int wordSize = this->value.size() == 0 ? 0 : rectForText->w;
		glVertex2i(wordSize + indicator->x, indicator->y);
		glVertex2i(wordSize + indicator->x + indicator->w, indicator->y);
		glVertex2i(wordSize + indicator->x + indicator->w, indicator->y + height - 3);
		glVertex2i(wordSize + indicator->x, indicator->y + height - 3);
		glEnd();
	}

	
	glColor4f(0,0,0,alpha);
	glBindTexture(GL_TEXTURE_2D, textId);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	//Renderizo el texto

	glTexCoord2f(0.0f, 0.0f); glVertex2i(rectForText->x, rectForText->y);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(rectForText->x + rectForText->w, rectForText->y);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(rectForText->x + rectForText->w, rectForText->y + rectForText->h);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(rectForText->x, rectForText->y + rectForText->h);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glColor4f(0, 0, 0, 1);

	/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);*/
	//Renderizo el rectangulo del input
	//glColor3f(0.8f, 0.8f, 0.8f);
	//glVertex2i(rect->x, rect->y);
	//glVertex2i(rect->x + rect->w, rect->y);
	//glVertex2i(rect->x + rect->w, rect->y + rect->h);
	//glVertex2i(rect->x, rect->y + rect->h);
	//glColor3f(0, 0, 0);
	//glEnd();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBegin(GL_QUADS);
		//Renderizo el rectangulo del input
		glVertex2i(rect->x, rect->y + height );
		glVertex2i(rect->x + rect->w, rect->y + height);
		glVertex2i(rect->x + rect->w, rect->y +height + 3);
		glVertex2i(rect->x, rect->y+height + 3);
	glEnd();


}

void TextField::process(float deltaTime)
{
	angle += 5.0f * deltaTime;
}

void TextField::processKeyPress()
{
	if (!selected || keyPressed == "")
		return;
	
	if (keyPressed == "ENTER") {
		clickOutside();
	}else if (keyPressed == "BACKSPACE" ) {
		if (this->value.length() > 0) {
			this->value = this->value.substr(0, this->value.length() - 1);
			if (value == "") {
				glDeleteTextures(1, &textTextureId);
				textTextureId = NULL;
			}
			else
				changeText(this->value);
		}
	}else if(this->value.length() < maxValueSize) {
		this->value += keyPressed;
		changeText(this->value);
	}
	callback(this->value);
}

BUTTON_TYPE TextField::getType()
{
	return TEXTFIELD;
}

SDL_Rect* TextField::getRect()
{
	return rect;
}

void TextField::click(Vec2 mousePos)
{
	this->selected = true; //TODO: agregar que use la mouse pos para calcular el [indice del string en el que va a seguir escribiendo
}

void TextField::clickOutside()
{
	this->selected = false;
}

void TextField::setKeyPressed(std::string key)
{
	this->keyPressed = key;
}

bool TextField::isSelected()
{
	return selected;
}



