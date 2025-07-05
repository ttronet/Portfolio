#include "MainMenu.h"
#include "LevelButton.h"

void goToLevelCreatorFromMainMenuCallback(std::string z) {
	MainMenu* mm = (MainMenu*)GameController::getInstance()->getState();
	mm->goToLevelCreator();
}


void goToSettingsFromMainMenuCallback(std::string z) {
	MainMenu* mm = (MainMenu*)GameController::getInstance()->getState();
	mm->goToSettings();
}

void closeFromMenuCallback(std::string z) {
	MainMenu* mm = (MainMenu*)GameController::getInstance()->getState();
	mm->closeGame();
}

void callbackForSlider(float newValue) {
	printf("%f\n",newValue);
}

void callbackShowLevelCarousel(std::string z) {
	MainMenu* mm = (MainMenu*)GameController::getInstance()->getState();
	mm->setLevelCarousel();
}
void callbackShowCustomLevelCarousel(std::string z) {
	MainMenu* mm = (MainMenu*)GameController::getInstance()->getState();
	mm->setCustomLevelCarousel();
}

void callbackCarouselRight() {
	MainMenu* mm = (MainMenu*)GameController::getInstance()->getState();
	mm->moveCarouselRight();
}
void callbackCarouselLeft() {
	MainMenu* mm = (MainMenu*)GameController::getInstance()->getState();
	mm->moveCarouselLeft();
}
void callbackCloseCarousel(std::string z) {
	MainMenu* mm = (MainMenu*)GameController::getInstance()->getState();
	mm->closeCarousel();
}
void loadLevel(std::string levelName) {
	MainMenu* mm = (MainMenu*)GameController::getInstance()->getState();

	mm->startLevel(levelName);
}

MainMenu::MainMenu() {
	GameController* gc = GameController::getInstance();

	showLevelCarousel = new Button("Niveles", 0, 150 * gc->getHeightScale(), callbackShowLevelCarousel, "", false, false);
	showLevelCarousel->center(0, gc->getWindowSize().x);
	
	showCustomLevelCarousel = new Button("Niveles personalizados", 0, 200 * gc->getHeightScale(), callbackShowCustomLevelCarousel, "", false,false);
	showCustomLevelCarousel->center(0, gc->getWindowSize().x);
	
	levelCreatorButton = new Button("Creador de niveles", 0, 250 * gc->getHeightScale(), goToLevelCreatorFromMainMenuCallback, "", false, false);
	levelCreatorButton->center(0, gc->getWindowSize().x);

	settingsButton = new Button("Opciones", 0, 300 * gc->getHeightScale(), goToSettingsFromMainMenuCallback, "", false, false);
	settingsButton->center(0, gc->getWindowSize().x);

	closeButton = new Button("Salir", 0, 400 * gc->getHeightScale(), closeFromMenuCallback, "", false, false);
	closeButton->center(0, gc->getWindowSize().x);


	leftArrowCarousel = new Button("images/carousel-left.png", "images/carousel-left-hover.png",10, gc->getWindowSize().y /2 - 30/2, 30 * gc->getWidthScale(),30 * gc->getHeightScale(),callbackCarouselLeft);
	rightArrowCarousel = new Button("images/carousel-right.png", "images/carousel-right-hover.png", gc->getWindowSize().x - gc->getWidthScale() * (30 + 10) / 2, gc->getWindowSize().y / 2 - gc->getWidthScale()* 30 / 2, 30 * gc->getWidthScale(), 30 * gc->getHeightScale(), callbackCarouselRight);
	closeCarouselBtn = new Button("X", gc->getWindowSize().x - 10 - 30 / 2, 10, callbackCloseCarousel, "", false, false);

	loadLevels();
	gc->setSaveFile(levels.size() + 1);

	std::vector<LevelButton*> btns;
	int i = 0;
	for (Level* level : levels) {
		LevelButton* levelBtn = new LevelButton(i%3,level,loadLevel);
		btns.push_back(levelBtn);
		i++;
	}
	this->levelButtons = btns;
	
	std::vector<LevelButton*> btnsForCustom;
	i = 0;
	for (Level* level : customLevels) {
		LevelButton* levelBtn = new LevelButton(i%3, level, loadLevel);
		btnsForCustom.push_back(levelBtn);
		i++;
	}
	this->customLevelsButtons = btnsForCustom;

	this->showCarousel = false;
	this->levelCarousel = btnsForCustom;
	this->carouselIndex = 0;
	this->cameraPosTime = 0;
	this->cameraY = 0;
	this->cameraX = 0;
	this->animationProgress = 0;
	this->animationLevel.resize(3);
	this->animationLevel[0] = new GamePlay(this->customLevels[0]);
	this->animationLevel[1] = new GamePlay(this->customLevels[1]);
	this->animationLevel[2] = new GamePlay(this->levels[6]);
	this->move = new Vec3[3];
}

bool MainMenu::getCarouselState() {
	return this->showCarousel;
}

void MainMenu::startLevel(std::string levelName)
{
	for (Level* level : levels) {
		if (level->getName() == levelName) {
			GamePlay* gp = new GamePlay(level);
			GameController::getInstance()->setState(gp);
			GameController::getInstance()->setGamePlay(gp);
			return;
		}
	}
	for (Level* level : customLevels) {
		if (level->getName() == levelName) {
			GamePlay* gp = new GamePlay(level);
			GameController::getInstance()->setState(gp);
			GameController::getInstance()->setGamePlay(gp);
			return;
		}
	}

	//yqs error de que no encontr[o el nivel :D

}

void MainMenu::goToLevelCreator()
{
	LevelCreator* lc = new LevelCreator();
	GameController::getInstance()->setState(lc);
}

void MainMenu::goToSettings()
{
	SettingsMenu* s = new SettingsMenu(GameController::getInstance()->getSettings());
	GameController::getInstance()->setState(s);
}

void MainMenu::closeGame()
{
	GameController::getInstance()->close();
}

void MainMenu::moveCarouselRight()
{
	if (levelCarousel.size() <= (carouselIndex+1) * 3)
		return;

	carouselIndex++;
}

void MainMenu::moveCarouselLeft()
{
	if (carouselIndex == 0)
		return;

	carouselIndex--;
}

void parseCoordsFromXMLNode(pugi::xml_node node, Vec3 &gridIndex, Vec3 &position) {
	float x = std::stoi(node.attribute("x").value());
	float y = std::stoi(node.attribute("y").value());
	float z = std::stoi(node.attribute("z").value());
	gridIndex = { x,y,z };
	position = { GameController::getInstance()->getGridPosition(x),GameController::getInstance()->getGridPosition(y),GameController::getInstance()->getGridPosition(z) };
}

void MainMenu::loadLevels()
{
	std::vector<std::string> levelFiles;
	std::vector<std::string> customLevelsFiles;

	for (const auto& entry : std::filesystem::directory_iterator("levels")) {
		if (entry.is_regular_file()) { 
			levelFiles.push_back(entry.path().filename().string());
		}
	}
	for (const auto& entry : std::filesystem::directory_iterator("customLevels")) {
		if (entry.is_regular_file()) {
			customLevelsFiles.push_back(entry.path().filename().string());
		}
	}

	pugi::xml_document levelXMLReader;
	int i = 1;
	for (std::string levelFile:levelFiles) {
		std::string fullPath = "levels/" + levelFile;
		pugi::xml_parse_result result = levelXMLReader.load_file(fullPath.c_str());
		Level * l = loadFromXML(levelXMLReader.child("Level"));
		l->setNo(i);
		levels.push_back(l);
		i++;
	}
	for (std::string levelFile : customLevelsFiles) {
		std::string fullPath = "customLevels/" + levelFile;
		pugi::xml_parse_result result = levelXMLReader.load_file(fullPath.c_str());
		Level* l =loadFromXML(levelXMLReader.child("Level"));
		customLevels.push_back(l);
	}
}

Level* MainMenu::loadFromXML(pugi::xml_node levelNode)
{
	if (!levelNode) {
		printf("Error loading level: %s. A Level node is required as root of the XML file");
		return NULL;
	}
	std::string levelName = levelNode.attribute("name").value();
	int maxScore = std::stoi(levelNode.attribute("maxScore").value());
	std::string nextLevel = "";
	if (levelNode.attribute("nextLevel"))
		nextLevel = levelNode.attribute("nextLevel").value();

	pugi::xml_node viboritaNode = levelNode.child("Viborita");
	pugi::xml_node gridNode = levelNode.child("Grid");

	if (!viboritaNode) {
		printf("Error loading level: %s. A Vivorita node is required inside the main Level node");
		return NULL;
	}
	if (!gridNode) {
		printf("Error loading level: %s. A Grid node is required inside the main Level node");
		return NULL;
	}

	IGameEntity* grid[8][8][8];
	for (int x = 0; x < 8;x++)
		for (int y = 0; y < 8;y++)
			for (int z = 0; z < 8;z++)
				grid[x][y][z] = NULL;

	for (pugi::xml_node gameEntity = gridNode.child("GameEntity"); gameEntity; gameEntity = gameEntity.next_sibling("GameEntity")) {
		IGameEntity* gi = NULL;
		std::string type = gameEntity.attribute("type").value();

		Vec3 gridIndex = { 0,0,0 };
		Vec3 position = { 0,0,0 };
		parseCoordsFromXMLNode(gameEntity, gridIndex, position);
		if (type == "block") {
			gi = new Block(gridIndex, position);
		}
		else if (type == "apple") {
			gi = new Apple(gridIndex, position);
		}
		else if (type == "goal") {
			gi = new Goal(gridIndex, position);
		}
		else if (type == "spiked_apple") {
			gi = new SpikedApple(gridIndex, position);
		}
		grid[(int)gridIndex.x][(int)gridIndex.y][(int)gridIndex.z] = gi;
	}

	pugi::xml_node viboritaHeadNode = viboritaNode.child("BodyPart");
	Vec3 gridIndex = { 0,0,0 };
	Vec3 position = { 0,0,0 };
	parseCoordsFromXMLNode(viboritaHeadNode, gridIndex, position);

	Viborita* viborita = new Viborita(gridIndex, position, baseViboritaColors);
	Vec3 voidVec = { 0,0,0 };
	for (pugi::xml_node viboritaBodyPartNode = viboritaHeadNode.next_sibling(); viboritaBodyPartNode; viboritaBodyPartNode = viboritaBodyPartNode.next_sibling("BodyPart")) {
		Vec3 partGridIndex = { 0,0,0 };
		parseCoordsFromXMLNode(viboritaBodyPartNode, partGridIndex, voidVec);
		viborita->addTail(partGridIndex);
	}

	Level* level = new Level(levelName, nextLevel, grid, maxScore, viborita);
	
	return level;
}

void MainMenu::resize()
{
	GameController* gc = GameController::getInstance();
	showLevelCarousel->getRect()->y = 150 * gc->getHeightScale();
	showLevelCarousel->center(0, gc->getWindowSize().x);

	showCustomLevelCarousel->getRect()->y = 200 * gc->getHeightScale();
	showCustomLevelCarousel->center(0, gc->getWindowSize().x);

	levelCreatorButton->getRect()->y = 250 * gc->getHeightScale();
	levelCreatorButton->center(0, gc->getWindowSize().x);

	settingsButton->getRect()->y = 300 * gc->getHeightScale();
	settingsButton->center(0, gc->getWindowSize().x);

	closeButton->getRect()->y = 400 * gc->getHeightScale();
	closeButton->center(0, gc->getWindowSize().x);


	leftArrowCarousel->getRect()->x = 10;
	leftArrowCarousel->getRect()->y = gc->getWindowSize().y / 2 - gc->getHeightScale()*30 / 2;
	leftArrowCarousel->getRect()->w = 30 * gc->getWidthScale();
	leftArrowCarousel->getRect()->h = 30 * gc->getHeightScale();

	rightArrowCarousel->getRect()->x = gc->getWindowSize().x - 10 - gc->getWidthScale() * 30;
	rightArrowCarousel->getRect()->y = gc->getWindowSize().y / 2 - gc->getHeightScale() * 30 / 2;
	rightArrowCarousel->getRect()->w = 30 * gc->getWidthScale();
	rightArrowCarousel->getRect()->h = 30 * gc->getHeightScale();

	closeCarouselBtn->getRect()->x = gc->getWindowSize().x - 10 - gc->getWidthScale() * 30 / 2;
	closeCarouselBtn->getRect()->y = 10;
	for (LevelButton* level : levelButtons)
		level->resize();
	for (LevelButton* level : customLevelsButtons) 
		level->resize();
}

float angle = M_PI;
void MainMenu::process(float deltaTime)
{
	Vec3 pos = { 0,0,0 };
	calc3dCoordsForHud(GameController::getInstance()->getCameraPos(), { 0,0,0 }, 50, 0, 13, pos);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(pos.x,pos.y,pos.z);
	drawModel(TITLE_MODEL, GameController::getInstance()->getSettings()->hasTextures());
	glPopMatrix();
	glColor3f(0.0f, 0, 0.0f);
	glDisable(GL_LIGHTING);

	if (showCarousel)
		return;

	this->cameraPosTime +=deltaTime;
	if (this->cameraPosTime >= 0 && this->cameraPosTime < 5.0f) {
		cameraX += 10 * deltaTime;
		cameraY += 10 * deltaTime;
		if (this->cameraPosTime >= 1 && this->cameraPosTime < 1.2f) {
			this->move[0] = { 0,1,0 };
			this->animationLevel[0]->getViborita()->processAnimation(move, deltaTime);

		}
		else if (this->cameraPosTime >= 1.2f && this->cameraPosTime < 1.4f) {
			this->move[0] = { 1,0,0 };
			this->animationLevel[0]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 1.4f && this->cameraPosTime < 1.8f) {
			this->move[0] = { 1,0,0 };
			this->animationLevel[0]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 1.8 && this->cameraPosTime < 2.0) {
			this->move[0] = { 1,0,0 };
			this->animationLevel[0]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 2.0f && this->cameraPosTime < 2.2f) {
			this->move[0] = { 1,0,0 };
			this->animationLevel[0]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 2.2f && this->cameraPosTime < 2.4f) {
			this->move[0] = { 0,0,1 };
			this->animationLevel[0]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 2.4f && this->cameraPosTime < 2.6f) {
			this->move[0] = { -1,0,0 };
			this->animationLevel[0]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 2.6f && this->cameraPosTime < 2.8f) {
			this->move[0] = { -1,0,0 };
			this->animationLevel[0]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 2.8f && this->cameraPosTime < 3.0f) {
			this->move[0] = { -1,0,0 };
			this->animationLevel[0]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 3.0f && this->cameraPosTime < 3.2f) {
			this->move[0] = { 0,0,-1 };
			this->animationLevel[0]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 3.2f && this->cameraPosTime < 3.4f) {
			this->move[0] = { 1,0,0 };
			this->animationLevel[0]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 3.4f && this->cameraPosTime < 3.6f) {
			this->move[0] = { 0,1,0 };
			this->animationLevel[0]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 3.6f && this->cameraPosTime < 3.8f) {
			this->move[0] = { 0,1,0 };
			this->animationLevel[0]->getViborita()->processAnimation(move, deltaTime);

		}
		else if (this->cameraPosTime >= 3.8f && this->cameraPosTime < 4.0f) {
			this->move[0] = { 0,1,0 };
			this->animationLevel[0]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 4.0f && this->cameraPosTime < 4.2f) {
			this->move[0] = { 0,1,0 };
			this->animationLevel[0]->getViborita()->processAnimation(move, deltaTime);

		}
		else if (this->cameraPosTime >= 4.2f && this->cameraPosTime < 4.4f) {
			this->move[0] = { 1,0,0 };
			this->animationLevel[0]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 4.4f && this->cameraPosTime < 4.6f) {
			this->move[0] = { 1,0,0 };
			this->animationLevel[0]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 4.6f && this->cameraPosTime < 4.8f) {
			this->move[0] = { 0,0,1 };
			this->animationLevel[0]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 4.8f && this->cameraPosTime < 5.0f) {
			this->move[0] = { 0,0,1 };
			this->animationLevel[0]->getViborita()->processAnimation(move, deltaTime);
		}
	}
	else if (this->cameraPosTime >= 5.0f && this->cameraPosTime < 9.0f) {
		this->animationProgress = 1;
		cameraX += 10 * deltaTime;
		cameraY += 10 * deltaTime;
		if (this->cameraPosTime > 5.0f && this->cameraPosTime < 5.2f) {
			this->move[0] = { 1,0,0 };
			this->animationLevel[1]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 5.2f && this->cameraPosTime < 5.4f) {
			this->move[0] = { 0,0,-1 };
			this->animationLevel[1]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 5.4f && this->cameraPosTime < 5.6f) {
			this->move[0] = { 0,0,-1 };
			this->animationLevel[1]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 5.6f && this->cameraPosTime < 5.8f) {
			this->move[0] = { 0,0,-1 };
			this->animationLevel[1]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 5.8f && this->cameraPosTime < 6.0f) {
			this->move[0] = { 0,0,-1 };
			this->animationLevel[1]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 6.0f && this->cameraPosTime < 6.2f) {
			this->move[0] = { -1,0,0 };
			this->animationLevel[1]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 6.2f && this->cameraPosTime < 6.4f) {
			this->move[0] = { -1,0,0 };
			this->animationLevel[1]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 6.4f && this->cameraPosTime < 6.6f) {
			this->move[0] = { -1,0,0 };
			this->animationLevel[1]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 6.6f && this->cameraPosTime < 6.8f) {
			this->move[0] = { 0,0,-1 };
			this->animationLevel[1]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 6.8f && this->cameraPosTime < 7.0f) {
			this->move[0] = { 0,0,-1 };
			this->animationLevel[1]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 7.0f && this->cameraPosTime < 7.2f) {
			this->move[0] = { 1,0,0 };
			this->animationLevel[1]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 7.2f && this->cameraPosTime < 7.4f) {
			this->move[0] = { 1,0,0 };
			this->animationLevel[1]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 7.4f && this->cameraPosTime < 7.6f) {
			this->move[0] = { 1,0,0 };
			this->animationLevel[1]->getViborita()->processAnimation(move, deltaTime);

		}
		else if (this->cameraPosTime >= 7.6f && this->cameraPosTime < 7.8f) {

			this->move[0] = { 1,0,0 };
			this->animationLevel[1]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 7.8f && this->cameraPosTime < 8.0f) {
			this->move[0] = { 0,0,1 };
			this->animationLevel[1]->getViborita()->processAnimation(move, deltaTime);

		}
		else if (this->cameraPosTime >= 8.0f && this->cameraPosTime < 8.2f) {
			this->move[0] = { -1,0,0 };
			this->animationLevel[1]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 8.2f && this->cameraPosTime < 8.4f) {
			this->move[0] = { -1,0,0 };
			this->animationLevel[1]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 8.4f && this->cameraPosTime < 8.6f) {
			this->move[0] = { -1,0,0 };
			this->animationLevel[1]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 8.6f && this->cameraPosTime < 8.8f) {
			this->move[0] = { -1,0,0 };
			this->animationLevel[1]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 8.8f && this->cameraPosTime <= 9.0f) {
			this->move[0] = { -1,0,0 };
			this->animationLevel[1]->getViborita()->processAnimation(move, deltaTime);
		}
	}
	else if (this->cameraPosTime > 9.0f && this->cameraPosTime < 13.0f) {
		this->animationProgress = 2;
		cameraX += 10 * deltaTime;
		cameraY += 10 * deltaTime;
		if (this->cameraPosTime >= 9.2f && this->cameraPosTime < 9.4f) {
			this->move[0] = { 0,0,1 };
			this->animationLevel[2]->getViborita()->processAnimation(move, deltaTime);

		}
		else if (this->cameraPosTime >= 9.4f && this->cameraPosTime < 9.6f) {
			this->move[0] = { 1,0,0 };
			this->animationLevel[2]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 9.6f && this->cameraPosTime < 9.8f) {
			this->move[0] = { 0,0,-1 };
			this->animationLevel[2]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 9.8f && this->cameraPosTime < 10.0f) {
			this->move[0] = { 0,0,-1 };
			this->animationLevel[2]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 10.0f && this->cameraPosTime < 10.2f) {
			this->move[0] = { 0,-1,0 };
			this->animationLevel[2]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 10.2f && this->cameraPosTime < 10.4f) {
			this->move[0] = { 0,-1,0 };
			this->animationLevel[2]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 10.4f && this->cameraPosTime < 10.6f) {
			this->move[0] = { 0,-1,0 };
			this->animationLevel[2]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 10.6f && this->cameraPosTime < 10.8f) {
			this->move[0] = { 0,0,1 };
			this->animationLevel[2]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 10.8f && this->cameraPosTime < 11.0f) {
			this->move[0] = { -1,0,0 };
			this->animationLevel[2]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 11.0f && this->cameraPosTime < 11.2f) {
			this->move[0] = { 0,0,1 };
			this->animationLevel[2]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 11.2f && this->cameraPosTime < 11.4f) {
			this->move[0] = { -1,0,0 };
			this->animationLevel[2]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 11.4f && this->cameraPosTime < 11.6f) {
			this->move[0] = { -1,0,0 };
			this->animationLevel[2]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 11.6f && this->cameraPosTime < 11.8f) {
			this->move[0] = { 0,-1,0 };
			this->animationLevel[2]->getViborita()->processAnimation(move, deltaTime);

		}
		else if (this->cameraPosTime >= 11.8f && this->cameraPosTime < 12.0f) {
			this->move[0] = { 0,-1,0 };
			this->animationLevel[2]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 12.0f && this->cameraPosTime < 12.2f) {
			this->move[0] = { 0,-1,0 };
			this->animationLevel[2]->getViborita()->processAnimation(move, deltaTime);

		}
		else if (this->cameraPosTime >= 12.2f && this->cameraPosTime < 12.4f) {
			this->move[0] = { 0,0,-1 };
			this->animationLevel[2]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 12.4f && this->cameraPosTime < 12.6f) {
			this->move[0] = { 1,0,0 };
			this->animationLevel[2]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 12.6f && this->cameraPosTime < 12.8f) {
			this->move[0] = { 1,0,0 };
			this->animationLevel[2]->getViborita()->processAnimation(move, deltaTime);
		}
		else if (this->cameraPosTime >= 12.8f && this->cameraPosTime <= 13.0f) {
			this->move[0] = { 1,0,0 };
			this->animationLevel[2]->getViborita()->processAnimation(move, deltaTime);
		}
	}
	else if (this->cameraPosTime > 13.0f) {
		this->cameraPosTime = 0;
		this->cameraY = 0;
		this->cameraX = 0;
		this->animationProgress = 0;
		animationLevel[0]->resetLevel();
		animationLevel[1]->resetLevel();
		animationLevel[2]->resetLevel();
	}

	draw();
}

void MainMenu::draw()
{
	if (this->animationProgress == 0) {
		glPushMatrix();
		glRotatef(this->cameraY, 0, 2, 0);
		this->animationLevel[0]->draw();
		glPopMatrix();
	}
	else if (this->animationProgress == 1) {
		glPushMatrix();
		glRotatef(this->cameraY, 0, 1, 0);
		this->animationLevel[1]->draw();
		glPopMatrix();
	}
	else if (this->animationProgress == 2) {
		glPushMatrix();
		glRotatef(this->cameraY, 0, 1, 0);
		this->animationLevel[2]->draw();
		glPopMatrix();
	}
}

std::vector<IHudElement*> MainMenu::getHudElements()
{
	std::vector<IHudElement*> buttons;
	if (showCarousel) {
		buttons.push_back(levelCarousel.at(carouselIndex * 3));
		if (levelCarousel.size() > carouselIndex * 3 + 1)
			buttons.push_back(levelCarousel.at(carouselIndex * 3 + 1));
		if (levelCarousel.size() > carouselIndex * 3 + 2)
			buttons.push_back(levelCarousel.at(carouselIndex * 3 + 2));

		buttons.push_back(leftArrowCarousel);
		buttons.push_back(rightArrowCarousel);
		buttons.push_back(closeCarouselBtn);
	}
	else {
		buttons.push_back(showLevelCarousel);
		buttons.push_back(showCustomLevelCarousel);
		buttons.push_back(levelCreatorButton);
		buttons.push_back(closeButton);
		buttons.push_back(settingsButton);
	}

	
	return buttons;
}

void MainMenu::setLevelCarousel()
{
	this->showCarousel = true;
	this->carouselIndex = 0;
	this->levelCarousel = this->levelButtons;
}

void MainMenu::setCustomLevelCarousel()
{
	this->showCarousel = true;
	this->carouselIndex = 0;
	this->levelCarousel = this->customLevelsButtons;
}

void MainMenu::closeCarousel()
{
	this->showCarousel = false;
}
