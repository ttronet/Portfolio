#include "LevelButton.h"

void calculatePreviewPos(Vec3 cameraPos, Vec3 center, int xOffset, float scale ,Vec3& previewPos) {

	//Normalized directions with respect to the vector from your face to the center of the viewport
	Vec3 forward = normalize({ center.x - cameraPos.x,center.y - cameraPos.y ,center.z - cameraPos.z });
	Vec3 right = normalize(crossProduct(forward, { 0,1,0 }));
	Vec3 up = normalize(crossProduct(right, forward));

	//Offsets
	float distance = 50 * scale;
	float yOffset = 0;

	previewPos = {
		cameraPos.x + forward.x * distance + right.x * xOffset + up.x * yOffset,
		cameraPos.y + forward.y * distance + right.y * xOffset + up.y * yOffset,
		cameraPos.z + forward.z * distance + right.z * xOffset + up.z * yOffset
	};
}

LevelButton::LevelButton(int x, Level* level,void(*callback)(std::string))
{
	this->level = level;
	this->rect = new SDL_Rect();
	this->x = x;
	resize();
	this->callback = callback;
	loadTextTexture(levelTextId,level->getName().c_str(),IHudElement::font,textSize.x,textSize.y);
	loadTextTexture(timerTextId, "-", IHudElement::font, timerSize.x, timerSize.y);
	loadTextTexture(scoreTextId, "-", IHudElement::font, scoreSize.x, scoreSize.y);
	GameController* gc = GameController::getInstance();
	SaveFile* sf = gc->getSaveFile();
	beat = false;
	if (level->getNo() <= 0 || !sf->hasBeatLevel(level->getNo()))
		return;
	
	beat = true; // :D
	int time = sf->getLevelTime(level->getNo());
	int minutes = time / 60;
	int seconds = time - minutes * 60;
	std::string mString = minutes < 10 ? "0" + std::to_string(minutes) : std::to_string(minutes);
	std::string sString = seconds < 10 ? "0" + std::to_string(seconds) : std::to_string(seconds);
	std::string text = mString + ":" + sString;
	loadTextTexture(timerTextId, text.c_str(), IHudElement::font, timerSize.x, timerSize.y);
	loadTextTexture(scoreTextId, std::to_string(sf->getLevelScore(level->getNo())).c_str(), IHudElement::font, scoreSize.x, scoreSize.y);

}

void LevelButton::draw()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glBegin(GL_QUADS);
	//glTexCoord2f(0.0f, 0.0f); glVertex2i(rect->x, rect->y);
	//glTexCoord2f(1.0f, 0.0f); glVertex2i(rect->x + rect->w, rect->y);
	//glTexCoord2f(1.0f, 1.0f); glVertex2i(rect->x + rect->w, rect->y + rect->h);
	//glTexCoord2f(0.0f, 1.0f); glVertex2i(rect->x, rect->y + rect->h);
	//glEnd();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindTexture(GL_TEXTURE_2D,levelTextId);
	glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			int offsetToCenterX = rect->w/2 - textSize.x/2;
			int offsetY = 0;
			int x0 = offsetToCenterX + rect->x;
			int x1 = offsetToCenterX + rect->x + textSize.x;
			int y0 = rect->y + rect->h;
			int y1 = rect->y + rect->h + textSize.y;

			glTexCoord2f(0.0f, 0.0f); glVertex2i(x0,y0);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(x1, y0);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(x1, y1);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(x0,y1);
		glEnd();
	if (timerTextId != NULL) {
		glBindTexture(GL_TEXTURE_2D, timerTextId);
		glBegin(GL_QUADS);
			offsetToCenterX = rect->w / 2 - timerSize.x / 2;
			offsetY += textSize.y + 10;
			x0 = offsetToCenterX + rect->x;
			x1 = offsetToCenterX + rect->x + timerSize.x;
			y0 = offsetY + rect->y + rect->h;
			y1 = offsetY + rect->y + rect->h + timerSize.y;

			glTexCoord2f(0.0f, 0.0f); glVertex2i(x0, y0);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(x1, y0);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(x1, y1);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(x0, y1);
		glEnd();
	}
	if (scoreTextId != NULL) {
		glBindTexture(GL_TEXTURE_2D, scoreTextId);
		glBegin(GL_QUADS);
			offsetToCenterX = rect->w / 2 - scoreSize.x / 2;
			offsetY += timerSize.y + 10;
			x0 = offsetToCenterX + rect->x;
			x1 = offsetToCenterX + rect->x + scoreSize.x;
			y0 = offsetY + rect->y + rect->h;
			y1 = offsetY + rect->y + rect->h + scoreSize.y;

			glTexCoord2f(0.0f, 0.0f); glVertex2i(x0, y0);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(x1, y0);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(x1, y1);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(x0, y1);
		glEnd();
	}
	glBindTexture(GL_TEXTURE_2D, beat ? textures.at("images/levelCompleted.png") : textures.at("images/levelNotCompleted.png"));
	glBegin(GL_QUADS);
	glColor3f(beat ? 0 : 0.7,beat ? 0.7 : 0,0);
	offsetToCenterX = rect->w / 2 - 15 / 2;
	offsetY += scoreSize.y + 10;
	x0 = offsetToCenterX + rect->x;
	x1 = offsetToCenterX + rect->x + 15;
	y0 = offsetY + rect->y + rect->h;
	y1 = offsetY + rect->y + rect->h + 15;

	glTexCoord2f(0.0f, 0.0f); glVertex2i(x0, y0);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(x1, y0);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(x1, y1);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(x0, y1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glColor3f(0,0,0);

	GameController* gc = GameController::getInstance();
	IHudElement::goBackTo3d();
		Vec3 offset;
		int xOffset = x == 0 ? -16 - gc->getWidthScale() * 2 : x == 1 ? 0 : 16 + gc->getWidthScale() * 2;
		float scale = 1 + aProgress;
		calculatePreviewPos(GameController::getInstance()->getCameraPos(),{0,0,0},xOffset,tileScale, offset);
		glPushMatrix();
			glScalef(scale, scale, scale);
			glTranslatef(offset.x, offset.y, offset.z);
			glRotatef(angle,0,1,0);
			level->drawPreview(GameController::getInstance()->GRID_SIZE);
		glPopMatrix();
	IHudElement::goBackTo2d();
}

void LevelButton::process(float deltaTime)
{
	if (hover)
	{
		this->angle += 40 * deltaTime;
		if (this->aProgress < 0.3)
			this->aProgress += 2 * deltaTime;
	}
	else {
		if(this->aProgress > 0)
			this->aProgress -= 2 * deltaTime;
	}
}

BUTTON_TYPE LevelButton::getType()
{
	return LEVELBUTTON;
}

SDL_Rect* LevelButton::getRect()
{
	return rect;
}

void LevelButton::handleHover()
{
	this->hover = !hover;
	if (hover)
		playSound(HOVER);
}

bool LevelButton::isHovering()
{
	return hover;
}

void LevelButton::click()
{
	callback(level->getName());
}

void LevelButton::resize()
{
	GameController* gc = GameController::getInstance();
	switch (x)
	{
	case 0:
		rect->x = 50 * gc->getWidthScale();
		break;
	case 1:
		rect->x = gc->getWindowSize().x / 2 - 75 * gc->getWidthScale();
		break;
	case 2:
		rect->x = gc->getWindowSize().x - 200 * gc->getWidthScale();
		break;
	default:
		break;
	}
	rect->y = gc->getWindowSize().y / 2 - 75 * gc->getHeightScale();
	rect->h = 150 * gc->getHeightScale();
	rect->w = 150 * gc->getWidthScale();
}
