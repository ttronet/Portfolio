#pragma once
#include "GL/glew.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <GL/glu.h>

#include "HudController.h"
#include "GameStats.h"
#include "Settings.h"
#include "Button.h"
#include "TextField.h"
#include "Level.h"

#include "IGameEntity.h"
#include "Block.h"
#include "Apple.h"
#include "Viborita.h"
#include "Goal.h"
#include "IGameState.h"
#include "GamePlay.h"
#include "MainMenu.h"
#include "LoadLevel.h"
#include "SettingsMenu.h"
#include "ParticleSystem.h"
#include "random"
#include <ctime>
#include "SaveFile.h"

class LevelCreator;
class MainMenu;
class Cloud;

class GameController
{
private:
	GameController();
	static GameController* instance;
	Settings* settings;
	
	GamePlay* game;
	IGameState* state;
	SaveFile* saveFile;

	std::list<ParticleSystem*> particles;

	bool paused = false;

	Vec2 mousePos;
	float timeCounter;
	
	bool showFps = false;
	int fps = 0;
	Button* fpsBtn;

	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool z = false;
	bool x = false;
	bool click = false;
	bool space = false;
	bool mouseDown = false;
	bool mouseUp = false;
	bool moveCamera = true;
	std::string keyPressed = "";

	
	float sensitivity = 0.5f;

	bool closeFlag = false;
	bool soundEnabled = false;
	CAMERA_ENUM cameraType = WORLD_CAMERA;
	//World and free camera stuff
	Vec3 cameraPos = { 0,0,0 };
	Vec3 cameraProps = { -M_PI / 2 , M_PI / 4 , 20 }; //theta, phi, radius
	//Free camera stuff
	bool w = false;
	bool s = false;
	bool a = false;
	bool d = false;

	//First person stuff
	bool normalControl = true;
	bool izqControl = false;
	bool derControl = false;


	Vec2 windowSize;
	float widthScale;
	float heightScale;
public:
	int GRID_SIZE;
	int TILE_SIZE;
	float GRID_OFFSET;
	float getGridPosition(float a);
	static GameController* getInstance();
	void processFrame(float deltaTime);
	virtual std::vector<IHudElement*> getHudElements();

	void setMousePos(Vec2 mousePos);
	void setArrowUp(bool up);
	void setArrowDown(bool down);
	void setArrowLeft(bool left);
	void setArrowRight(bool right);
	void setZKey(bool z);
	void setXKey(bool x);
	void setSpaceKey(bool space);
	void setClick(bool click);
	void setMouseDown(bool click);
	void setMouseUp(bool click);
	void setShowFps(bool show);
	void setKeyPressed(std::string key);
	void setWKey(bool x) { this->w = x; }
	void setSKey(bool x) { this->s = x; }
	void setDKey(bool x) { this->d = x; }
	void setAKey(bool x) { this->a = x; }

	Settings* getSettings();
	void setSettings(Settings* settings);
	IGameState* getState();
	void setState(IGameState* state);
	Vec2 getMousePos();
	void setGamePlay(GamePlay* game);
	GamePlay* getGamePlay();
	bool isArrowUp();
	bool isArrowDown();
	bool isArrowLeft();
	bool isArrowRight();
	bool isZKey();
	bool isXKey();
	bool isSpaceKey();
	bool clicked();
	bool isMouseDown();
	bool isMouseUp();
	bool isShowFps();
	bool isWKey() { return w; }
	bool isSKey() { return s; }
	bool isDKey() { return d; }
	bool isAKey() { return a; }
	std::string getKeyPressed();

	void setMoveCamera(bool move);
	bool getMoveCamera();

	Vec3 getCameraProps();
	float getSensitivity();
	bool getNormalControl();
	void setNormalControl(bool normalControl);
	bool getIzqControl();
	void setIzqControl(bool izqControl);
	bool getDerControl();
	void setDerControl(bool derControl);
	bool isPaused();

	void setPaused(bool paused);
	void setCameraCoordinates(float theta, float phi);
	void setCameraRadius(float radius);
	void setSensitivity(float sensitivity);

	void setCameraPos(Vec3 pos);
	Vec3 getCameraPos();

	void close();
	bool shouldClose();

	void addParticles(ParticleSystem* ps);
	void removeParticles(ParticleSystem* ps);

	void setSoundEnabled(bool);
	bool getSoundEnabled();

	CAMERA_ENUM getCameraType();
	void setCameraType(CAMERA_ENUM cameraType);

	void setWindowSize(Vec2 size);
	Vec2 getWindowSize();

	float getWidthScale();
	float getHeightScale();

	void setSaveFile(int levelCount);
	SaveFile* getSaveFile();
	void saveSaveFile();
	void removeAllParticles();
};

