#pragma once
#include "Button.h"
#include "GameController.h"
#include "IGameState.h"
#include "Settings.h"
//#include "Slider.h"

class Slider;

class SettingsMenu :
    public IGameState
{
private:
	Settings* settings;

	Button* speedText;
	Slider* speedSlider;

	Button* volumeText;
	Slider* volumeSlider;

	Button* graphicsText;

	Button* wireFrameText;

	Button* texturesText;

	Button* backToGame;
	Button* backToMenu;

	Button* interpoladoText;
	Button* facetadoText;
	

	Button* lightText;

	Button* lightDirText;
	Slider* lightDirSlider;
	Button* lightRText;
	Slider* lightRSlider;
	Button* lightGText;
	Slider* lightGSlider;
	Button* lightBText;
	Slider* lightBSlider;
	Button* lightAlphaText;
	Slider* lightAlphaSlider;

public:
	SettingsMenu(Settings* settings);
	void process(float deltaTime);
	void draw();
	void resize();
	void changeGameSpeed(float gameSpeed);
	void changeVolume(int volume);
	void setWireframe(bool wireframe);
	void setTextures(bool texture);
	void setFacetado(bool val);
	void setInterpolado(bool val);
	void setTexSettings(TEX_SETTINGS texSettings);
	void changeLightRed(float red);
	void changeLightGreen(float green);
	void changeLightBlue(float blue);
	void changeLightAngle(int angle);
	void changeLightAlpha(float alpha);
	void goBackToGame();
	void goBackToMainMenu();
	virtual std::vector<IHudElement*> getHudElements();
};

