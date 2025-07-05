#include "GameController.h"

GameController* GameController::instance = NULL;

std::mt19937 generador(std::time(nullptr));
std::uniform_int_distribution<> angles(1, 100);
std::uniform_int_distribution<> radios(15, 20);
std::uniform_int_distribution<> cloudModel(1, 3);

GameController::GameController() {
	GRID_SIZE = 8;
	TILE_SIZE = 1;
	for (int i = 0; i < 24;i++)
		baseCubeVertices[i] = baseCubeVertices[i] * TILE_SIZE;
	for (int i = 0; i < 12;i++)
		basePyramidVertices[i] = basePyramidVertices[i] * TILE_SIZE;

	GRID_OFFSET = GRID_SIZE / 2;

	game = NULL;
	state = NULL;
	settings = NULL;
	timeCounter = 0.0f;
	fps = 0;
	fpsBtn = new Button("60", 10, 400);
	
	saveFile = NULL;
}

GameController* GameController::getInstance() {
	if (instance == NULL)
		instance = new GameController();

	return instance;
}

void GameController::processFrame(float deltaTime) {
	fps++;
	timeCounter += deltaTime;
	if (timeCounter >= 1.0) {
		timeCounter -= 1.0;
		fpsBtn->updateText(std::to_string(fps).c_str());
		fps = 1;
		if (game != NULL)
			game->addSecond();
	}

	if (settings->isWireframe())
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	state->process(deltaTime);

	std::list<ParticleSystem*>::iterator it;
	it = particles.begin();
	while (!particles.empty() && it != particles.end()) {
		ParticleSystem* ps = (*it);
		paused ? ps->draw() : ps->process(deltaTime);
		if (ps->shouldDissapear()) {
			delete ps;
			it = particles.erase(it);
		}else
			it++;
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	HudController::getInstance()->process(deltaTime);

}

float GameController::getGridPosition(float a) {
	return a * TILE_SIZE - GRID_OFFSET;
}

std::vector<IHudElement*> GameController::getHudElements()
{
	std::vector<IHudElement*> elements = state->getHudElements();
	if (showFps)
		elements.push_back(fpsBtn);
	return elements;
}

void GameController::setMousePos(Vec2 mousePos)
{
	this->mousePos = mousePos;
}

void GameController::setArrowUp(bool up)
{
	this->up = up;
}

void GameController::setArrowDown(bool down)
{
	this->down = down;
}

void GameController::setArrowLeft(bool left)
{
	this->left = left;
}

void GameController::setArrowRight(bool right)
{
	this->right = right;
}

void GameController::setZKey(bool z)
{
	this->z = z;
}
void GameController::setXKey(bool x)
{
	this->x = x;
}

void GameController::setSpaceKey(bool space)
{
	this->space = space;
}

void GameController::setClick(bool click)
{
	this->click = click;
}

void GameController::setMouseDown(bool click)
{
	this->mouseDown = click;
}
void GameController::setMouseUp(bool click)
{
	this->mouseUp = click;
}

void GameController::setShowFps(bool show)
{
	this->showFps = show;
}

void GameController::setKeyPressed(std::string key)
{
	this->keyPressed = key;
}

void GameController::setGamePlay(GamePlay* game)
{
	this->game = game;
}

GamePlay* GameController::getGamePlay()
{
	return game;
}

Settings* GameController::getSettings()
{
	return settings;
}

void GameController::setSettings(Settings* settings)
{
	this->settings = settings;
}

IGameState* GameController::getState()
{
	return state;
}

void GameController::setState(IGameState* state)
{
	this->state = state;
}

Vec2 GameController::getMousePos()
{
	return this->mousePos;
}

bool GameController::isArrowUp()
{
	return up;
}
bool GameController::isArrowDown()
{
	return down;
}
bool GameController::isArrowLeft()
{
	return left;
}
bool GameController::isArrowRight()
{
	return right;
}

bool GameController::isZKey()
{
	return z;
}

bool GameController::isXKey()
{
	return x;
}

bool GameController::isSpaceKey()
{
	return space;
}

bool GameController::clicked()
{
	return click;
}

bool GameController::isMouseDown()
{
	return mouseDown;
}

bool GameController::isMouseUp()
{
	return mouseUp;
}

bool GameController::isShowFps()
{
	return showFps;
}

std::string GameController::getKeyPressed()
{
	return keyPressed;
}

void GameController::setMoveCamera(bool move)
{
	this->moveCamera = move;
}

bool GameController::getMoveCamera()
{
	return moveCamera;
}

Vec3 GameController::getCameraProps()
{
	return cameraProps;
}

float GameController::getSensitivity()
{
	return sensitivity;
}

bool GameController::getNormalControl()
{
	return this->normalControl;
}

bool GameController::getIzqControl()
{
	return this->izqControl;
}

bool GameController::getDerControl()
{
	return this->derControl;
}

void GameController::setNormalControl(bool normalControl)
{
	this->normalControl = normalControl;
}

void GameController::setIzqControl(bool izqControl)
{
	this->izqControl = izqControl;
}

void GameController::setDerControl(bool derControl)
{
	this->derControl = derControl;
}

bool GameController::isPaused()
{
	return paused;
}

void GameController::setPaused(bool paused)
{
	this->paused = paused;
}

void GameController::setCameraCoordinates(float theta, float phi)
{
	cameraProps.x = theta;
	cameraProps.y = phi;
}

void GameController::setCameraRadius(float radius)
{
	cameraProps.z = radius;
}

void GameController::setSensitivity(float sensitivity)
{
	this->sensitivity = sensitivity;
}

void GameController::setCameraPos(Vec3 pos)
{
	cameraPos.x = pos.x;
	cameraPos.y = pos.y;
	cameraPos.z = pos.z;
}

Vec3 GameController::getCameraPos()
{
	return cameraPos;
}

void GameController::close()
{
	this->closeFlag = true;
}

bool GameController::shouldClose()
{
	return closeFlag;
}

void GameController::addParticles(ParticleSystem* ps)
{
	if (particles.size() < 20)
		particles.push_back(ps);
	else
		delete ps;
}

void GameController::removeParticles(ParticleSystem* ps)
{
	std::list<ParticleSystem*>::iterator it;
	it = particles.begin();
	while (!particles.empty() && it != particles.end()) {
		ParticleSystem* toDel = (*it);
		if(toDel != ps)
		{
			it++;
			continue;
		}
		delete ps;
		particles.erase(it);
		break;
	}
}

void GameController::removeAllParticles() {
	std::list<ParticleSystem*>::iterator it;
	it = particles.begin();
	while (!particles.empty() && it != particles.end()) {
		ParticleSystem* toDel = (*it);
		delete toDel;
		particles.erase(it);
		break;
	}
}

void GameController::setSoundEnabled(bool e)
{
	this->soundEnabled = e;
}

bool GameController::getSoundEnabled()
{
	return soundEnabled;
}

CAMERA_ENUM GameController::getCameraType()
{
	return cameraType;
}

void GameController::setCameraType(CAMERA_ENUM type)
{
	this->cameraType = type;
	this->moveCamera = type != WORM_CAMERA;
}

void GameController::setWindowSize(Vec2 size)
{
	this->windowSize = { size.x,size.y };
	this->widthScale = (float)size.x / 640.0f;
	this->heightScale = (float)size.y / 480.0f;
}

Vec2 GameController::getWindowSize()
{
	return this->windowSize;
}

float GameController::getWidthScale()
{
	return widthScale;
}

float GameController::getHeightScale()
{
	return heightScale;
}

std::string readFileToString(FILE* file) {
	std::string contents;
	char buffer[1024];

	while (fgets(buffer, sizeof(buffer), file)) {
		contents += buffer;
	}

	return contents;
}
void GameController::setSaveFile(int levelCount)
{
	if (this->saveFile != NULL)
		return;


	this->saveFile = new SaveFile(levelCount);

	std::string path = std::filesystem::current_path().string() + "/save.snaked";
	FILE* fp = nullptr;
	errno_t err = fopen_s(&fp, path.c_str(), "r");

	if (err != 0 || fp == nullptr) { //Doesn't have a save file
		return;
	}
	
	std::string saveString = readFileToString(fp);
	fclose(fp);

	std::vector<std::string> lines;
	int start = 0;
	int end;

	while ((end = saveString.find('\n', start)) != std::string::npos) {
		lines.push_back(saveString.substr(start, end - start));
		start = end + 1;
	}
	lines.push_back(saveString.substr(start, end - start));

	for (int i = 0; i < lines.size();i++) {
		std::string line = lines.at(i);
		int j = 0;
		start = 0;
		end = 0;
		while ((end = line.find(',', start)) != std::string::npos) {
			try {
				std::string val = line.substr(start, end - start);
				start = end + 1;
				switch (i) {
				case 0:
					if (val == "true")
						saveFile->beatLevel(j);
					break;
				case 1:
					saveFile->setLevelBestTime(j, std::stoi(val));
					break;
				case 2:
					saveFile->setLevelBestScore(j, std::stoi(val));
					break;
				}
				j++;
			}
			catch (const std::exception& ex) {
				std::cerr << "Failed to load level no: "+std::to_string(j)+" in line "+std::to_string(i) << std::endl;
				j++;
			}
		}
	}
}

SaveFile* GameController::getSaveFile()
{
	return saveFile;
}

void GameController::saveSaveFile()
{
	std::string path = std::filesystem::current_path().string() + "/save.snaked";
	std::ofstream file(path);
	if (!file) {
		std::cerr << "Failed to create the file." << std::endl;
		return;
	}
	file << saveFile->toString();
	file.close(); 
	std::cout << "Saved progress correctly." << std::endl;
}






