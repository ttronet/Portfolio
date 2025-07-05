#include "LevelCreator.h"


//TODO: agregar peaks_apple al creador de niveles
void selectEntityCallback(std::string entity) {
	LevelCreator* lc = (LevelCreator*)GameController::getInstance()->getState();
	if (entity == "block") {
		lc->setEntityTipe(BLOCK);
	}
	else if(entity == "apple"){
		lc->setEntityTipe(APPLE);
	}
	else if (entity == "goal") {
		lc->setEntityTipe(GOAL);
	}
	else if (entity == "viborita") {
		lc->setEntityTipe(VIBORITA);
	}
	else if (entity == "spiked_apple") {
		lc->setEntityTipe(SPIKED_APPLE);
	}
	
}
void selectEraseCallback(std::string z) {
	LevelCreator* lc = (LevelCreator*)GameController::getInstance()->getState();
	lc->setErase(true);
}
void goToMainMenuFromLevelCreatorCallback(std::string z) {
	LevelCreator* lc = (LevelCreator*)GameController::getInstance()->getState();
	lc->goBackToMainMenu();
}
void saveLevelCallback(std::string z) {
	LevelCreator* lc = (LevelCreator*)GameController::getInstance()->getState();
	lc->saveLevel();
}
void changeNameCallback(std::string name) {
	LevelCreator* lc = (LevelCreator*)GameController::getInstance()->getState();
	lc->setName(name);
}
void tryLevelCallback(std::string name) {
	LevelCreator* lc = (LevelCreator*)GameController::getInstance()->getState();
	lc->toggleTryLevel();
}
LevelCreator::LevelCreator()
{
	noOfApples = 0;
	this->name = "";
	createButtons();
	selectedGridIndex = { 0,0,0 };
	selectedEntityType = BLOCK;
	this->testGameplay = NULL;
	this->viborita = NULL;
	for (int x = 0; x < 8;x++)
		for (int y = 0; y < 8;y++)
			for (int z = 0; z < 8;z++)
				grid[x][y][z] = NULL;
}
void LevelCreator::createButtons()
{	
	GameController* gc = GameController::getInstance();
	nameField = new TextField("Nombre", gc->getWindowSize().x / 2 - 200 / 2, 10, 200, changeNameCallback, "");
	saveButton = new Button("Guardar", 10, 10, saveLevelCallback, "",false,false);
	mainMenuButton = new Button("Menu", gc->getWindowSize().x - 100, 10, goToMainMenuFromLevelCreatorCallback, "", false, false);
	tryButton = new Button("Probar nivel", 10, 50, tryLevelCallback, "", false, false);
	
	for (int i = 0;i < 8;i++) {
		selectBar[i] = NULL;
		selectBarBoxes[i] = NULL;
		selectAngles[i] = 0;
	}

	int leftOffset = 175;
	int bottomOffset = 20;
	int buttonSize = 50 * gc->getWidthScale();
	selectBar[0] = new Block({0,0,0},{0,0,0});
	selectBarBoxes[0] = new Button(leftOffset * gc->getWidthScale(), gc->getWindowSize().y - bottomOffset - buttonSize, buttonSize, buttonSize, selectEntityCallback, "block");
	selectBar[1] = new Apple({ 0,0,0 }, { 0,0,0 });
	selectBarBoxes[1] = new Button((leftOffset + 60) * gc->getWidthScale(), gc->getWindowSize().y- bottomOffset - buttonSize, buttonSize, buttonSize, selectEntityCallback, "apple");
	selectBar[2] = new Goal({ 0,0,0 }, { 0,0,0 });
	selectBarBoxes[2] = new Button((leftOffset + 60 + 60) * gc->getWidthScale(), gc->getWindowSize().y- bottomOffset - buttonSize, buttonSize, buttonSize, selectEntityCallback, "goal");
	selectBar[3] = new Viborita({ 0,0,0 }, { 0,0,0 }, baseViboritaColors);
	selectBarBoxes[3] = new Button((leftOffset + 60 + 60 + 60) * gc->getWidthScale(), gc->getWindowSize().y - bottomOffset - buttonSize, buttonSize, buttonSize, selectEntityCallback, "viborita");
	selectBar[4] = new SpikedApple({ 0,0,0 }, { 0,0,0 });
	selectBarBoxes[4] = new Button((leftOffset + 60 + 60 + 60 + 60) * gc->getWidthScale(), gc->getWindowSize().y - bottomOffset - buttonSize, buttonSize, buttonSize, selectEntityCallback, "spiked_apple");
	selectBar[5] = new Erasor({ 0,0,0 }, { 0,0,0 });
	selectBarBoxes[5] = new Button((leftOffset + 60 + 60 + 60 + 60 + 60) * gc->getWidthScale(), gc->getWindowSize().y- bottomOffset - buttonSize, buttonSize, buttonSize, selectEraseCallback, "");
}
void LevelCreator::process(float deltaTime)
{
	if (testGameplay != NULL) {
		try {

			testGameplay->process(deltaTime);
		}
		catch (const std::runtime_error& error) { //Had some weird crashes, so this is a safeguard so the game does not close
			toggleTryLevel();
			std::cerr << "Error: " << error.what() << std::endl;
		}
		return;
	}

	GameController* gc = GameController::getInstance();
	handleSelectedIndexMovement();
	handlePlaceEntity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPushMatrix();
	GLfloat bigCube[24];
	for (int i = 0;i < 24;i++) {
		bigCube[i] = baseCubeVertices[i] * gc->TILE_SIZE * gc->GRID_SIZE;
	}
	float offset = -gc->GRID_SIZE*gc->TILE_SIZE/2;
	glTranslatef(offset,offset,offset);
	drawCube(bigCube, baseCubeColors, baseCubeIndices);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(gc->getGridPosition(selectedGridIndex.x), gc->getGridPosition(selectedGridIndex.y), gc->getGridPosition(selectedGridIndex.z));
	drawCubeWithNormals({0,0,0}, false, 0);
	glPopMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_LIGHTING);
	for (int x = 0; x < gc->GRID_SIZE;x++) 
		for (int y = 0; y < gc->GRID_SIZE;y++) 
			for (int z = 0; z < gc->GRID_SIZE;z++) 
				if (grid[x][y][z] != NULL && grid[x][y][z] != this->viborita) 
					grid[x][y][z]->draw();
				
	if(this->viborita != NULL)
		this->viborita->draw();	
	
	selectAngles[selectedIndex] += 50.0f * deltaTime;
	this->draw();
	glDisable(GL_LIGHTING);
}

void LevelCreator::draw()
{
	GameController* gc = GameController::getInstance();
	for (int i = 0;i < 8;i++)
		if (selectBar[i] != NULL)
		{
			glPushMatrix();
			Button* hitBox = selectBarBoxes[i];
			Vec3 pos;
			float TILE_SIZE = GameController::getInstance()->TILE_SIZE;
			float windowFactor = gc->getWidthScale() > 1.5 ? gc->getWidthScale() / 2 : gc->getWidthScale();
			float xOffset = i - 2 == 0 ? 0 : (i - 2) * 2.5f * windowFactor;

			calc3dCoordsForHud(GameController::getInstance()->getCameraPos(),{0,0,0},25,xOffset,-8.5 + 2*hitBox->getHoverProgress(), pos);
			glTranslatef(pos.x,pos.y,pos.z);
			glRotatef(selectAngles[i],0,1,0);
			glTranslatef(-TILE_SIZE / 2, -TILE_SIZE / 2, -TILE_SIZE / 2);
			selectBar[i]->draw();
			glPopMatrix();
			//if (selectedIndex == i) { //TODO: indicator de que est[a seleccionado
			//	glPushMatrix();
			//	glScalef(0.7f,0.7f,0.7f);
			//	calc3dCoordsForHud(GameController::getInstance()->getCameraPos(), { 0,0,0 }, 25, (i - 2) * 2.5, -10, pos);
			//	glTranslatef(pos.x, pos.y, pos.z);
			//	drawPyramid(basePyramidVertices,basePyramidColors,basePyramydIndices);
			//	glPopMatrix();
			//}
		}
}

std::vector<IHudElement*> LevelCreator::getHudElements()
{
	std::vector<IHudElement*> btns;

	btns.push_back(saveButton);
	btns.push_back(mainMenuButton);
	btns.push_back(nameField);
	btns.push_back(tryButton);

	for (int i = 0;i < 8;i++) 
		if (selectBarBoxes[i] != NULL)
			btns.push_back(selectBarBoxes[i]);
	
	return btns;
}

void LevelCreator::setEntityTipe(GAME_ENTITY_TYPE type)
{
	this->selectedEntityType = type;
	switch (type)
	{
	case BLOCK:
		selectedIndex = 0;
		break;
	case VIBORITA:
		selectedIndex = 3;
		break;
	case APPLE:
		selectedIndex = 1;
		break;
	case GOAL:
		selectedIndex = 2;
		break;
	case SPIKED_APPLE:
		selectedIndex = 4;
		break;
	default:
		selectedIndex = 0;
		break;
	}
	this->erase = false;
}

void LevelCreator::setErase(bool erase)
{
	this->erase = erase;
	if(erase)
		selectedIndex = 5;
}

std::string entityTypeString(GAME_ENTITY_TYPE type) {
	switch (type) {
	case BLOCK:
		return "block";
	case VIBORITA:
		return "viborita";
	case APPLE:
		return "apple";
	case GOAL:
		return "goal";
	case SPIKED_APPLE:
		return "spiked_apple";
	default:
		return "";
	}
}
void LevelCreator::saveLevel()
{
	if (this->viborita == NULL)
		return;
	if (this->name == "")
		return;
	GameController* gc = GameController::getInstance();

	pugi::xml_document* root = new pugi::xml_document();
	pugi::xml_node levelNode = root->append_child("Level");
	levelNode.append_attribute("name").set_value(this->name);
	levelNode.append_attribute("maxScore").set_value(noOfApples);
	levelNode.append_attribute("nextLevel").set_value("");
	
	

	pugi::xml_node gridNode = levelNode.append_child("Grid");
	for (int x = 0; x < gc->GRID_SIZE;x++)
		for (int y = 0; y < gc->GRID_SIZE;y++)
			for (int z = 0; z < gc->GRID_SIZE;z++) {
				if (grid[x][y][z] != NULL && grid[x][y][z] != this->viborita) {
					pugi::xml_node entityNode = gridNode.append_child("GameEntity");
					entityNode.append_attribute("type").set_value(entityTypeString(grid[x][y][z]->getType()));
					entityNode.append_attribute("x").set_value(x);
					entityNode.append_attribute("y").set_value(y);
					entityNode.append_attribute("z").set_value(z);
				}
			}

	pugi::xml_node viboritaNode = levelNode.append_child("Viborita");
	ViboritaPart* aux = this->viborita->getHead();
	while (aux != NULL) {
		pugi::xml_node bodyPartNode =  viboritaNode.append_child("BodyPart");
		bodyPartNode.append_attribute("x").set_value(aux->gridIndex.x);
		bodyPartNode.append_attribute("y").set_value(aux->gridIndex.y);
		bodyPartNode.append_attribute("z").set_value(aux->gridIndex.z);
		aux = aux->next;
	}

	std::string filePath = "customLevels/" + name + ".xml";
	
	root->save_file(filePath.c_str());
}

void LevelCreator::clearGrid()
{
}

void LevelCreator::goBackToMainMenu()
{
	MainMenu* mm = new MainMenu();
	GameController::getInstance()->setState(mm);
}

void LevelCreator::setName(std::string name)
{
	this->name = name;
}

void LevelCreator::toggleTryLevel()
{
	if (testGameplay != NULL) {
		testGameplay = NULL;
		GameController::getInstance()->removeAllParticles();
		tryButton->updateText("Probar nivel");
	}
	else if(this->viborita != NULL){
		Level* level = new Level(name, "", grid, noOfApples, this->viborita);
		testGameplay = new GamePlay(level, this);
		tryButton->updateText("Editar nivel");
	}
}

void LevelCreator::handleSelectedIndexMovement()
{
	GameController* gc = GameController::getInstance();
	if (gc->isArrowUp() && selectedGridIndex.z + 1 < gc->GRID_SIZE)
		selectedGridIndex.z += 1;
	else if (gc->isArrowDown() && selectedGridIndex.z - 1 >= 0)
		selectedGridIndex.z += -1;
	else if (gc->isArrowLeft() && selectedGridIndex.x + 1 < gc->GRID_SIZE)
		selectedGridIndex.x += 1;
	else if (gc->isArrowRight() && selectedGridIndex.x - 1 >= 0)
		selectedGridIndex.x += -1;
	else if (gc->isZKey() && selectedGridIndex.y + 1 < gc->GRID_SIZE)
		selectedGridIndex.y += 1;
	else if (gc->isXKey() && selectedGridIndex.y - 1 >= 0)
		selectedGridIndex.y += -1;
}

void LevelCreator::handlePlaceEntity()
{
	if (nameField->isSelected())
		return;

	GameController* gc = GameController::getInstance();
	if (!gc->isSpaceKey())
		return;

	int x = selectedGridIndex.x;
	int y = selectedGridIndex.y;
	int z = selectedGridIndex.z;
	/*if (grid[x][y][z] != NULL) {
		delete grid[x][y][z];
	}*/
	if (this->erase && grid[x][y][z] != NULL) {
		IGameEntity* toDelete = grid[x][y][z];
		grid[x][y][z] = NULL;
		if(toDelete->getType() == APPLE)
			noOfApples--;
		else if(toDelete->getType() == VIBORITA) {
			this->viborita->removeBodyFromGridIndex(x,y,z);
			if (this->viborita->getSize() == 0)
			{
				this->viborita = NULL;
				//TODO: agregar el delete
			}

			return;
		}
		//TODO: ver por qu[e crashea con el delete toDelete;
		return;
	}
	Vec3 gridIndex = { x,y,z };
	Vec3 position = {gc->getGridPosition(x),gc->getGridPosition(y) ,gc->getGridPosition(z) };
	switch (selectedEntityType)
	{
	case BLOCK:
		grid[x][y][z] = new Block(gridIndex,position);
		break;
	case VIBORITA:
		if (this->viborita == NULL) {
			this->viborita = new Viborita(gridIndex,position,baseViboritaColors);
		}
		else if(grid[x][y][z] != this->viborita){
			this->viborita->addTail(gridIndex);
		}
		grid[x][y][z] = this->viborita;
		break;
	case APPLE:
		grid[x][y][z] = new Apple(gridIndex, position);
		noOfApples++;
		break;
	case GOAL:
		grid[x][y][z] = new Goal(gridIndex, position);
		break;
	case SPIKED_APPLE:
		grid[x][y][z] = new SpikedApple(gridIndex, position);
		break;
	default:
		break;
	}
}

void LevelCreator::resize()
{
	GameController* gc = GameController::getInstance();

	int leftOffset = 175;
	int bottomOffset = 20;
	int buttonSize = 50 * gc->getWidthScale();
	for (int i = 0; i < 6; i++) {
		selectBarBoxes[i]->getRect()->x = (leftOffset + 60*i) * gc->getWidthScale();
		selectBarBoxes[i]->getRect()->y = gc->getWindowSize().y - bottomOffset - buttonSize;
		selectBarBoxes[i]->getRect()->h = buttonSize;
		selectBarBoxes[i]->getRect()->w = buttonSize;
	}
}
