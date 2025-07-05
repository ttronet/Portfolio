#pragma once
#include "IGameState.h"
#include <set>
#include "GameController.h"
#include "GamePlay.h"
#include "Button.h"
#include "Level.h"
#include "pugixml.hpp"
#include <iostream>
#include <filesystem>
#include "LevelCreator.h"
class LevelButton;

class MainMenu :
    public IGameState
{
private:
	Button* goToTutorial;
	Button* showLevelCarousel;
	Button* showCustomLevelCarousel;
	Button* levelCreatorButton;
	Button* closeButton;
	Button* settingsButton;

	bool showCarousel;
	std::vector<LevelButton*> levelCarousel;
	int carouselIndex;
	Button* leftArrowCarousel;
	Button* rightArrowCarousel;
	Button* closeCarouselBtn;
	float cameraY;
	float cameraX;
	float cameraPosTime;
	int animationProgress;
	Vec3* move;
	std::vector<GamePlay*> animationLevel;

	std::vector<LevelButton*> levelButtons;
	std::vector<LevelButton*> customLevelsButtons;
	
	std::vector<Level*> levels;
	std::vector<Level*> customLevels;
	void loadLevels();
	Level* loadFromXML(pugi::xml_node root);

	void resize();
public:
	MainMenu();
	void process(float deltaTime);
	void draw();
	void startLevel(std::string levelName);
	void goToLevelCreator();
	void goToSettings();
	void closeGame();
	void moveCarouselRight();
	void moveCarouselLeft();
	bool getCarouselState();
	virtual std::vector<IHudElement*> getHudElements();
	void setLevelCarousel();
	void setCustomLevelCarousel();
	void closeCarousel();
};

