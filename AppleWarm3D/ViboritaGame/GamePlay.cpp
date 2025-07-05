#include "GamePlay.h"
#include "LevelCreator.h"

void openSettings() {
	GameController::getInstance()->setState(new SettingsMenu(GameController::getInstance()->getSettings()));
	GameController::getInstance()->setPaused(true);
}
void resetLevelCallback() {
	GamePlay* gp = (GamePlay*)GameController::getInstance()->getState();
	gp->resetLevel();
}
void firstPersonCallback() {
	GamePlay* gp = (GamePlay*)GameController::getInstance()->getState();
	gp->setFirstPerson();
}
void levelPerspectiveCallback() {
	GamePlay* gp = (GamePlay*)GameController::getInstance()->getState();
	gp->setLevelPerspective();
}
void freePerspectiveCallback() {
	GamePlay* gp = (GamePlay*)GameController::getInstance()->getState();
	gp->setFreePerspective();
}
GamePlay::GamePlay(Level* level)
{
	GameController* gc = GameController::getInstance();
	this->level = level;
	settings = new Button("images/settings.png", "images/settingsHover.png", gc->getWindowSize().x - 60, 10, 50, 50, openSettings);
	reset = new Button("images/restart.png", "images/restartHover.png", gc->getWindowSize().x - 120, 10, 50, 50, resetLevelCallback);
	std::string sText = "0/" + std::to_string(level->getMaxScore());
	scoreText = new Button(sText.c_str(), 10, 10);
	timerText = new Button("00:00", 0, 10);
	timerText->center(0, gc->getWindowSize().x);
	resetWarning = new Button("R.I.P viborita, presiona R para reiniciar", 0, 0);
	SDL_Rect* rect = resetWarning->getRect();
	rect->x = gc->getWindowSize().x / 2 - rect->w / 2;
	rect->y = gc->getWindowSize().y / 2 - rect->h / 2;

	int btnSize = 20;
	cameraIcon = new Button("images/camera.png", GameController::getInstance()->getCameraType() == WORM_CAMERA ? 10 : 40 , gc->getWindowSize().y - 60 - btnSize, btnSize, btnSize, NULL);
	btnSize = 30;
	firstPersonPerspective = new Button("images/snake.png", 10, gc->getWindowSize().y - 10 - btnSize*1.5f, btnSize, btnSize, firstPersonCallback);
	levelPerspective = new Button("images/global.png", 20 + btnSize, gc->getWindowSize().y - 10 - btnSize * 1.5f, btnSize, btnSize, levelPerspectiveCallback);
	freePerspective = new Button("images/bird.png", 20 + btnSize*2, gc->getWindowSize().y - 10 - btnSize*1.5f, btnSize, btnSize, freePerspectiveCallback);

	for (int i = 0; i < 10;i++)
		tutorials[i] = NULL;

	tutorials[1] = new Button("images/Level1Tutorial.png", 10, 60, "");
	tutorials[1]->center(0, gc->getWindowSize().x);
	tutorials[2] = new Button("images/Level2Tutorial.png", 10, 60, "");
	tutorials[2]->center(0, gc->getWindowSize().x);
	tutorials[3] = new Button("images/Level3Tutorial.png",10,60,"");
	tutorials[3]->center(0, gc->getWindowSize().x);
	tutorials[4] = new Button("images/Level4Tutorial.png", 10, 60, "");
	tutorials[4]->center(0, gc->getWindowSize().x);
	tutorials[5] = new Button("images/Level5Tutorial.png", 10, 60, "");
	tutorials[5]->center(0, gc->getWindowSize().x);
	tutorials[6] = new Button("images/Level6Tutorial.png", 10, 60, "");
	tutorials[6]->center(0, gc->getWindowSize().x);
	tutorials[8] = new Button("images/Level8Tutorial.png", 10, 60, "");
	tutorials[8]->center(0, gc->getWindowSize().x);

	startLevel();
}

GamePlay::GamePlay(Level* level, LevelCreator* levelCreator)
{
	GameController* gc = GameController::getInstance();
	this->level = level;
	this->lc = levelCreator;

	settings = new Button("images/settings.png", "images/settingsHover.png", gc->getWindowSize().x - 60, 10, 50, 50, openSettings);
	reset = new Button("images/restart.png", "images/restartHover.png", gc->getWindowSize().x - 120, 10, 50, 50, resetLevelCallback);
	std::string sText = "0/" + std::to_string(level->getMaxScore());
	scoreText = new Button(sText.c_str(), 10, 10);
	timerText = new Button("00:00", 0, 10);
	timerText->center(0, gc->getWindowSize().x);

	int btnSize = 20;
	cameraIcon = new Button("images/camera.png", GameController::getInstance()->getCameraType() == WORM_CAMERA ? 10 : 40, gc->getWindowSize().y - 60 - btnSize, btnSize, btnSize, NULL);
	btnSize = 30;
	firstPersonPerspective = new Button("images/snake.png", 10, gc->getWindowSize().y - 10 - btnSize * 1.5f, btnSize, btnSize, firstPersonCallback);
	levelPerspective = new Button("images/global.png", 20 + btnSize, gc->getWindowSize().y - 10 - btnSize * 1.5f, btnSize, btnSize, levelPerspectiveCallback);

	for (int i = 0; i < 10;i++)
		tutorials[i] = NULL;

	startLevel();
}

void GamePlay::changeTimer(int time)
{
	int minutes = time / 60;
	int seconds = time - minutes * 60;
	std::string mString = minutes < 10 ? "0" + std::to_string(minutes) : std::to_string(minutes);
	std::string sString = seconds < 10 ? "0"+std::to_string(seconds) : std::to_string(seconds) ;
	std::string text = mString + ":"+ sString;
	timerText->updateText(text.c_str());
}

void GamePlay::beatLevel()
{
	if (this->lc != NULL) {
		this->lc->toggleTryLevel();
		return;
	}
	std::string nextLevelName = level->getNextLevelName();
	MainMenu* mm = new MainMenu();
	GameController::getInstance()->setGamePlay(NULL);
	GameController::getInstance()->setState(mm);

	for (int x = 0; x < 8;x++) 
		for (int y = 0; y < 8;y++) 
			for (int z = 0; z < 8;z++) {
				if(this->grid[x][y][z] != NULL && this->grid[x][y][z] != this->viborita)
				{
					IGameEntity* ge = this->grid[x][y][z];
					delete ge;
					this->grid[x][y][z] = NULL;
				}
			}

	if (level->getNo() > 0) {
		SaveFile* sf = GameController::getInstance()->getSaveFile();
		sf->beatLevel(level->getNo());
		sf->setLevelBestScore(level->getNo(), stats->getScore());
		sf->setLevelBestTime(level->getNo(),stats->getTimer());
		GameController::getInstance()->saveSaveFile();
	}

	if (nextLevelName != "") 
		mm->startLevel(nextLevelName);
}

void GamePlay::startLevel()
{
	std::string sText = "0/" + std::to_string(level->getMaxScore());
	scoreText->updateText(sText.c_str());
	timerText->updateText("00:00");

	this->stats = new GameStats(level->getMaxScore(), false);

	for (int x = 0; x < 8;x++) {
		for (int y = 0; y < 8;y++) {
			for (int z = 0; z < 8;z++) {
				this->grid[x][y][z] = level->getInitialGridPosition(x, y, z);
				if (grid[x][y][z] != NULL && grid[x][y][z]->getType() == BLOCK)
					((Block*)grid[x][y][z])->setContext(this);
			}
		}
	}
	this->viborita = level->getInitialViborita();
	this->viborita->setGameContext(this);
	this->viborita->loadInGrid();
}

void GamePlay::resetLevel()
{
	showWarning = false;
	for (int x = 0; x < 8;x++) {
		for (int y = 0; y < 8;y++) {
			for (int z = 0; z < 8;z++) {
				this->grid[x][y][z] = NULL;
			}
		}
	}
	startLevel();
}

void GamePlay::viboritaDied()
{
	showWarning = true;
}

void GamePlay::addSecond()
{
	stats->addSecond();
	changeTimer(stats->getTimer());
}

void GamePlay::process(float deltaTime)
{
	angle += 5.0f*deltaTime;
	glEnable(GL_LIGHTING);
	glNormal3f(0.0f, 1.0f, 0.0f);
	for (int x = 0; x < GameController::getInstance()->GRID_SIZE;x++) {
		for (int y = 0; y < GameController::getInstance()->GRID_SIZE;y++) {
			for (int z = 0; z < GameController::getInstance()->GRID_SIZE;z++) {
				if (grid[x][y][z] != NULL && grid[x][y][z] != this->viborita) {
					grid[x][y][z]->process(deltaTime);
					grid[x][y][z]->draw();
				}
			}
		}
	}
	this->viborita->process(deltaTime);
	this->viborita->draw();
	glDisable(GL_LIGHTING);

	switch (GameController::getInstance()->getCameraType()) {
		case WORLD_CAMERA:
			cameraIcon->getRect()->x = 40;
			break;
		case WORM_CAMERA:
			cameraIcon->getRect()->x = 10;
			break;
		case FREE_CAMERA:
			cameraIcon->getRect()->x = 70;
			break;
	}
}

void GamePlay::draw()
{
	glEnable(GL_LIGHTING);
	glNormal3f(0.0f, 1.0f, 0.0f);
	for (int x = 0; x < GameController::getInstance()->GRID_SIZE;x++) 
		for (int y = 0; y < GameController::getInstance()->GRID_SIZE;y++) 
			for (int z = 0; z < GameController::getInstance()->GRID_SIZE;z++) 
				if (grid[x][y][z] != NULL && grid[x][y][z] != this->viborita) 
					grid[x][y][z]->draw();

	this->viborita->draw();
	glDisable(GL_LIGHTING);
}

std::vector<IHudElement*> GamePlay::getHudElements()
{
	std::vector<IHudElement*> buttons;
	if(this->lc == NULL)
		buttons.push_back(settings);
	buttons.push_back(reset);
	buttons.push_back(scoreText);
	buttons.push_back(timerText);
	buttons.push_back(cameraIcon);
	buttons.push_back(firstPersonPerspective);
	buttons.push_back(levelPerspective);
	buttons.push_back(freePerspective);
	if(showWarning && cos(angle) > 0)
		buttons.push_back(resetWarning);
	if (level->getNo() != NULL && level->getNo() < 10 && tutorials[level->getNo()] != NULL)
		buttons.push_back(tutorials[level->getNo()]);
	return buttons;
}

Viborita* GamePlay::getViborita()
{
	return viborita;
}

bool GamePlay::tileHasApple(Vec3 indices)
{
	int x = indices.x;
	int y = indices.y;
	int z = indices.z;
	return validTile(indices) && this->grid[x][y][z] != NULL && this->grid[x][y][z]->getType() == APPLE;
}

bool GamePlay::hasGoal(Vec3 indices)
{
	int x = indices.x;
	int y = indices.y;
	int z = indices.z;
	return validTile(indices) && this->grid[x][y][z] != NULL && this->grid[x][y][z]->getType() == GOAL;
}

void GamePlay::clearTile(Vec3 indices)
{
	if (!validTile(indices))
		return;
	int x = indices.x;
	int y = indices.y;
	int z = indices.z;
	this->grid[x][y][z] = NULL;
}

bool GamePlay::validTile(Vec3 indices)
{
	bool xOutside = indices.x < 0 || indices.x >= GameController::getInstance()->GRID_SIZE;
	bool yOutside = indices.y < 0 || indices.y >= GameController::getInstance()->GRID_SIZE;
	bool zOutside = indices.z < 0 || indices.z >= GameController::getInstance()->GRID_SIZE;
	return !(xOutside || yOutside || zOutside);
}

bool inRange(int i) {
	return i >= 0 && i <= 8;
}
bool GamePlay::hasSolidBlock(Vec3 indices)
{
	int x = indices.x;
	int y = indices.y;
	int z = indices.z;
	return validTile(indices) && grid[x][y][z] != NULL && grid[x][y][z]->getType() == BLOCK;
}

bool GamePlay::hasViborita(Vec3 indices)
{
	int x = indices.x;
	int y = indices.y;
	int z = indices.z;
	return validTile(indices) && grid[x][y][z] != NULL && grid[x][y][z]->getType() == VIBORITA;
}

bool GamePlay::hasSpikedApple(Vec3 indices) {
	int x = indices.x;
	int y = indices.y;
	int z = indices.z;
	return validTile(indices) && grid[x][y][z] != NULL && grid[x][y][z]->getType() == SPIKED_APPLE;
}

void GamePlay::addViborita(Vec3 indices)
{
	if (this->viborita == NULL || !validTile(indices))
		return;
	int x = indices.x;
	int y = indices.y;
	int z = indices.z;
		grid[x][y][z] = this->viborita;
}

void GamePlay::eatAppleAt(Vec3 indices)
{
	int x = indices.x;
	int y = indices.y;
	int z = indices.z;
	IGameEntity* ge = grid[x][y][z];
	if (ge == NULL || ge->getType() != APPLE)
		return;

	((Apple*)ge)->eatApple();
}

void GamePlay::ateApple()
{
	stats->ateAnApple();
	int newScore = stats->getScore();
	int maxScore = stats->getMaxScore();

	std::string newText = std::to_string(newScore) + " / " + std::to_string(maxScore);

	scoreText->updateText(newText.c_str());
}

void GamePlay::setFirstPerson()
{
	GameController::getInstance()->setCameraType(WORM_CAMERA);
	cameraIcon->getRect()->x = 10;
}

void GamePlay::setLevelPerspective()
{
	GameController::getInstance()->setCameraType(WORLD_CAMERA);
	cameraIcon->getRect()->x = 40;
}

void GamePlay::setFreePerspective()
{
	GameController::getInstance()->setCameraType(FREE_CAMERA);
	cameraIcon->getRect()->x = 70;
}

std::vector<IHudElement*> GamePlay::getLevelTutorials()
{
	std::vector<IHudElement*> messages;
	if(level->getNo() == -1)
		return messages;

	switch (level->getNo())
	{
	default:
		break;
	}
	return messages;
}

void GamePlay::resize()
{
	GameController* gc = GameController::getInstance();
	settings->getRect()->x = gc->getWindowSize().x - 60;
	settings->getRect()->y = 10;

	reset->getRect()->x = gc->getWindowSize().x - 120;
	reset->getRect()->y = 10;

	timerText->center(0, gc->getWindowSize().x);

	int btnSize = 20;
	cameraIcon->getRect()->x = gc->getCameraType() == WORM_CAMERA ? 10 : 40;
	cameraIcon->getRect()->y = gc->getWindowSize().y - 60 - btnSize;
	btnSize = 30;
	firstPersonPerspective->getRect()->x = 10;
	firstPersonPerspective->getRect()->y = gc->getWindowSize().y - 10 - btnSize * 1.5f;

	levelPerspective->getRect()->x = 20 + btnSize;
	levelPerspective->getRect()->y = gc->getWindowSize().y - 10 - btnSize * 1.5f;

	tutorials[1]->center(0, gc->getWindowSize().x);
	tutorials[2]->center(0, gc->getWindowSize().x);
	tutorials[3]->center(0, gc->getWindowSize().x);
	tutorials[4]->center(0, gc->getWindowSize().x);
	tutorials[8]->center(0, gc->getWindowSize().x);
}

GamePlay::~GamePlay()
{
	for (int x = 0; x < 8;x++)
		for (int y = 0; y < 8;y++)
			for (int z = 0; z < 8;z++) {
				if (this->grid[x][y][z] != NULL && this->grid[x][y][z] != this->viborita)
				{
					IGameEntity* ge = this->grid[x][y][z];
					delete ge;
					this->grid[x][y][z] = NULL;
				}
			}

	delete settings;
	delete reset;
	delete scoreText;
	delete stats;
}
