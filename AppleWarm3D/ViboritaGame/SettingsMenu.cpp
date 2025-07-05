#include "SettingsMenu.h"

void goBackToGameCallback(std::string s) {
	SettingsMenu* sm = (SettingsMenu*)GameController::getInstance()->getState();

	sm->goBackToGame();
}
void setSpeedCallback(float speed) {
	SettingsMenu* sm = (SettingsMenu*)GameController::getInstance()->getState();
	sm->changeGameSpeed(speed);
}
void setVolumeCallback(float volume) {
	SettingsMenu* sm = (SettingsMenu*)GameController::getInstance()->getState();
	sm->changeVolume(volume);
}
void setLightAngleCallback(float angle) {
	SettingsMenu* sm = (SettingsMenu*)GameController::getInstance()->getState();
	sm->changeLightAngle(angle);
}
void setWireframeCallback(std::string wireframe) {
	SettingsMenu* sm = (SettingsMenu*)GameController::getInstance()->getState();
	sm->setWireframe(wireframe == "true");
}
void setTexturesCallback(std::string textures) {
	SettingsMenu* sm = (SettingsMenu*)GameController::getInstance()->getState();
	sm->setTextures(textures == "true");
}
void setFacetadoCallback(std::string textures) {
	SettingsMenu* sm = (SettingsMenu*)GameController::getInstance()->getState();
	sm->setFacetado(textures == "true");
}
void setInterpoladoCallback(std::string textures) {
	SettingsMenu* sm = (SettingsMenu*)GameController::getInstance()->getState();
	sm->setInterpolado(textures == "true");
}
void setTexSettingsCallback(std::string setting) {
	SettingsMenu* sm = (SettingsMenu*)GameController::getInstance()->getState();
	if(setting == "interpolado")
		sm->setTexSettings(INTERPOLADO);
	else
		sm->setTexSettings(FACETADO);
}
void goToMainMenuFromSettingsCallback(std::string z) {
	SettingsMenu* sm = (SettingsMenu*)GameController::getInstance()->getState();
	sm->goBackToMainMenu();
}
void setLightRedCallback(float red) {
	SettingsMenu* sm = (SettingsMenu*)GameController::getInstance()->getState();
	sm->changeLightRed(red);
}
void setLightGreenCallback(float green) {
	SettingsMenu* sm = (SettingsMenu*)GameController::getInstance()->getState();
	sm->changeLightGreen(green);
}
void setLightBlueCallback(float blue) {
	SettingsMenu* sm = (SettingsMenu*)GameController::getInstance()->getState();
	sm->changeLightBlue(blue);
}
void setLightAlphaCallback(float alpha) {
	SettingsMenu* sm = (SettingsMenu*)GameController::getInstance()->getState();
	sm->changeLightAlpha(alpha);
}

SettingsMenu::SettingsMenu(Settings* settings) {
	GameController* gc = GameController::getInstance();
	this->settings = settings;

	speedText = new Button("Velocidad",20 * gc->getWidthScale(),20 * gc->getHeightScale());
	speedSlider = new Slider("Prueba", 30 * gc->getWidthScale(), 70 * gc->getHeightScale(), 150 * gc->getWidthScale(), 0.5f, 3.0f, setSpeedCallback, settings->getGameSpeed());
	volumeText = new Button("Volumen", 20 * gc->getWidthScale(), 100 * gc->getHeightScale());
	volumeSlider = new Slider("Prueba", 30 * gc->getWidthScale(), 150 * gc->getHeightScale(), 150 * gc->getWidthScale(), 0, 128, setVolumeCallback, settings->getVolume());

	graphicsText = new Button("Graficos", 20 * gc->getWidthScale(), 200 * gc->getHeightScale());
	wireFrameText = new Button("Wireframe", 30 * gc->getWidthScale(), (120 + 80 + 50) * gc->getHeightScale(),setWireframeCallback,"", true, settings->isWireframe());
	wireFrameText->center(30 * gc->getWidthScale(),230 * gc->getWidthScale());
	texturesText = new Button("Textures", 30 * gc->getWidthScale(), (170 + 80 + 50) * gc->getHeightScale(),setTexturesCallback,"",true, settings->hasTextures());
	texturesText->center(30 * gc->getWidthScale(), 230 * gc->getWidthScale());
	interpoladoText = new Button("Interpolado", 30 * gc->getWidthScale(), (220 + 80 + 50) * gc->getHeightScale(), setInterpoladoCallback,"",true,settings->hasInterpolado());
	interpoladoText->center(30 * gc->getWidthScale(), 230 * gc->getWidthScale());
	facetadoText = new Button("Facetado", 30 * gc->getWidthScale(), (270 + 80 + 50) * gc->getHeightScale(),  setFacetadoCallback,"",true,!settings->hasInterpolado());
	facetadoText->center(30 * gc->getWidthScale(), 230 * gc->getWidthScale());


	lightText = new Button("Luz",440 * gc->getWidthScale(), 20 * gc->getHeightScale());
	lightDirText = new Button("Direccion", 450 * gc->getWidthScale(), 20 + 40 * gc->getHeightScale());
	lightDirSlider = new Slider("", 460 * gc->getWidthScale(), (70 + 40) * gc->getHeightScale(), 150 * gc->getWidthScale(), 0, 2*M_PI, setLightAngleCallback, settings->getLightAngle());
	//TODO: agregar para seleccionar qu[e tipo de luz que cambi[as el color ambient, difuse,specular, usar el settings->setSelectedLight
	lightRText = new Button("Rojo", 450 * gc->getWidthScale(), (120 + 40) * gc->getHeightScale());
	lightRSlider = new Slider("", 460 * gc->getWidthScale(), (160 + 40) * gc->getHeightScale(), 150 * gc->getWidthScale(), 0, 1, setLightRedCallback, settings->getLightColor().x);
	lightGText = new Button("Verde", 450 * gc->getWidthScale(), (220 + 40) * gc->getHeightScale());
	lightGSlider = new Slider("", 460 * gc->getWidthScale(), (260 + 40) * gc->getHeightScale(), 150 * gc->getWidthScale(), 0, 1, setLightGreenCallback, settings->getLightColor().y);
	lightBText = new Button("Azul", 450 * gc->getWidthScale(), (320 + 40) * gc->getHeightScale());
	lightBSlider = new Slider("", 460 * gc->getWidthScale(), (360 + 40) * gc->getHeightScale(), 150 * gc->getWidthScale(), 0, 1, setLightBlueCallback, settings->getLightColor().z);
	//lightAlphaText = new Button("Alfa", 450, 420 + 50);
	//lightAlphaSlider = new Slider("", 450, 460 + 50, 150, 0, 1, setLightAlphaCallback, settings->getLightAlpha());


	backToMenu = new Button("Volver al menu", 170 * gc->getWidthScale(), 440 * gc->getHeightScale(), goToMainMenuFromSettingsCallback, "", false,false);
	backToMenu->center(0, gc->getWindowSize().x);
	backToGame = new Button("X", 0, 10 * gc->getHeightScale(), goBackToGameCallback,"", false,false);
	backToGame->center(0, gc->getWindowSize().x);
}

void SettingsMenu::changeGameSpeed(float speed)
{
	this->settings->setGameSpeed(speed);
}

void SettingsMenu::changeVolume(int volume)
{
	this->settings->setVolume(volume);
}

void SettingsMenu::setWireframe(bool wireframe)
{
	this->settings->setWireframe(wireframe);
}

void SettingsMenu::setTextures(bool texture)
{
	this->settings->setTextures(texture);
}

void SettingsMenu::setFacetado(bool val)
{
	settings->setInterpolado(false);
	facetadoText->setSelected(true);
	interpoladoText->setSelected(false);
}

void SettingsMenu::setInterpolado(bool val)
{
	settings->setInterpolado(true);
	facetadoText->setSelected(false);
	interpoladoText->setSelected(true);
}

void SettingsMenu::setTexSettings(TEX_SETTINGS texSettings)
{
	this->settings->setTexSettings(texSettings);
}

void SettingsMenu::changeLightRed(float red)
{
	Vec3 currentLight = settings->getLightColor();
	float alpha = settings->getLightAlpha();
	settings->setLightColor({red,currentLight.y,currentLight.z},alpha);
}

void SettingsMenu::changeLightGreen(float green)
{
	Vec3 currentLight = settings->getLightColor();
	float alpha = settings->getLightAlpha();
	settings->setLightColor({ currentLight.x, green,currentLight.z },alpha);
}

void SettingsMenu::changeLightBlue(float blue)
{
	Vec3 currentLight = settings->getLightColor();
	float alpha = settings->getLightAlpha();
	settings->setLightColor({ currentLight.x,currentLight.y,blue },alpha);
}

void SettingsMenu::changeLightAngle(int angle)
{
	this->settings->setLightAngle(angle);
}

void SettingsMenu::changeLightAlpha(float alpha)
{
	printf("%f\n",alpha);
	Vec3 currentLight = settings->getLightColor();
	settings->setLightColor({ currentLight.x,currentLight.y,currentLight.z },alpha);
}

void SettingsMenu::goBackToGame()
{
	GameController::getInstance()->setState(GameController::getInstance()->getGamePlay());
	GameController::getInstance()->setPaused(false);
}

void SettingsMenu::goBackToMainMenu()
{
	if (GameController::getInstance()->getGamePlay() != NULL) {
		GamePlay* gp = GameController::getInstance()->getGamePlay();
		delete gp;
		GameController::getInstance()->setGamePlay(NULL);
	}
	GameController::getInstance()->setPaused(false);
	MainMenu* mm = new MainMenu();
	GameController::getInstance()->setState(mm);
	GameController::getInstance()->setPaused(false);
	GameController::getInstance()->setGamePlay(NULL);
}

void SettingsMenu::process(float deltaTime)
{
	if(GameController::getInstance()->getGamePlay() != NULL)
		GameController::getInstance()->getGamePlay()->draw(); //Only draw but don't process (Viborita can't move)
}

void SettingsMenu::draw()
{
}

void SettingsMenu::resize()
{
	GameController* gc = GameController::getInstance();
	float ws = gc->getWidthScale();
	float hs = gc->getHeightScale();
	Vec2 winSize = gc->getWindowSize();

	speedText->getRect()->x = 20 * ws;
	speedText->getRect()->y = 20 * hs;

	speedSlider->getRect()->x = 30 * ws;
	speedSlider->getRect()->y = 70 * hs;
	speedSlider->setLength(150 * ws);

	volumeText->getRect()->x = 20 * ws;
	volumeText->getRect()->y = 100 * hs;

	volumeSlider->getRect()->x = 30 * ws;
	volumeSlider->getRect()->y = 150 * hs;
	volumeSlider->setLength(150 * ws);

	graphicsText->getRect()->x = 20 * ws;
	graphicsText->getRect()->y = 200 * hs;

	wireFrameText->getRect()->x = 30 * ws;
	wireFrameText->getRect()->y = (120 + 80 + 50) * hs;
	wireFrameText->center(30 * ws, 230 * ws);

	texturesText->getRect()->x = 30 * ws;
	texturesText->getRect()->y = (170 + 80 + 50) * hs;
	texturesText->center(30 * ws, 230 * ws);

	interpoladoText->getRect()->x = 30 * ws;
	interpoladoText->getRect()->y = (220 + 80 + 50) * hs;
	interpoladoText->center(30 * ws, 230 * ws);

	facetadoText->getRect()->x = 30 * ws;
	facetadoText->getRect()->y = (270 + 80 + 50) * hs;
	facetadoText->center(30 * ws, 230 * ws);

	lightText->getRect()->x = 440 * ws;
	lightText->getRect()->y = 20 * hs;

	lightDirText->getRect()->x = 450 * ws;
	lightDirText->getRect()->y = (20 + 40) * hs;

	lightDirSlider->getRect()->x = 460 * ws;
	lightDirSlider->getRect()->y = (70 + 40) * hs;
	lightDirSlider->setLength(150 * ws);

	lightRText->getRect()->x = 450 * ws;
	lightRText->getRect()->y = (120 + 40) * hs;

	lightRSlider->getRect()->x = 460 * ws;
	lightRSlider->getRect()->y = (160 + 40) * hs;
	lightRSlider->setLength(150 * ws);

	lightGText->getRect()->x = 450 * ws;
	lightGText->getRect()->y = (220 + 40) * hs;

	lightGSlider->getRect()->x = 460 * ws;
	lightGSlider->getRect()->y = (260 + 40) * hs;
	lightGSlider->setLength(150 * ws);

	lightBText->getRect()->x = 450 * ws;
	lightBText->getRect()->y = (320 + 40) * hs;

	lightBSlider->getRect()->x = 460 * ws;
	lightBSlider->getRect()->y = (360 + 40) * hs;
	lightBSlider->setLength(150 * ws);

	backToMenu->getRect()->x = 170 * ws;
	backToMenu->getRect()->y = 440 * hs;
	backToMenu->center(0, winSize.x);

	backToGame->getRect()->x = 0;
	backToGame->getRect()->y = 10 * hs;
}

std::vector<IHudElement*> SettingsMenu::getHudElements()
{
	std::vector<IHudElement*> buttons;
	buttons.push_back(speedText);
	buttons.push_back(speedSlider);
	buttons.push_back(volumeText);
	buttons.push_back(volumeSlider);
	buttons.push_back(wireFrameText);
	buttons.push_back(texturesText);
	if (GameController::getInstance()->getGamePlay() != NULL)
		buttons.push_back(backToGame);
	buttons.push_back(interpoladoText);
	buttons.push_back(facetadoText);
	buttons.push_back(backToMenu);
	buttons.push_back(lightDirText);
	buttons.push_back(lightDirSlider);
	buttons.push_back(lightRText);
	buttons.push_back(lightRSlider);
	buttons.push_back(lightGText);
	buttons.push_back(lightGSlider);
	buttons.push_back(lightBText);
	buttons.push_back(lightBSlider);
	buttons.push_back(graphicsText);
	buttons.push_back(lightText);
	//buttons.push_back(lightAlphaText); //No cambia nada en realidad
	//buttons.push_back(lightAlphaSlider);
	return buttons;
}