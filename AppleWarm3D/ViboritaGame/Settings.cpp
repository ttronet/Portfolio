#include "Settings.h"
#include "GameController.h"

Settings::Settings()
{
	//Setear configs por defecto
	this->gameSpeed = 1;
	this->textures = true;
	this->wireframe = false;
	this->texSettings = INTERPOLADO;
    this->lightColor = { 0.2f,0.2f,0.2f };
    this->lightAlpha = 1.0f;
    setInterpolado(true);
    setVolume(128);
    setLightAngle(0);
}

float Settings::getGameSpeed() const {
    return gameSpeed;
}

bool Settings::isWireframe() const {
    return wireframe;
}

bool Settings::hasTextures() const {
    return textures;
}

bool Settings::hasInterpolado() const
{
    return interpolado;
}

bool Settings::hasFacetado() const
{
    return facetado;
}

TEX_SETTINGS Settings::getTexSettings() const {
    return texSettings;
}

int Settings::getLightAngle() const {
    return lightAngle;
}

Vec3 Settings::getLightColor() const {
    return lightColor;
}

float Settings::getLightAlpha() const
{
    return lightAlpha;
}

void Settings::setGameSpeed(float speed) {
    gameSpeed = speed;
}

void Settings::setWireframe(bool value) {
    wireframe = value;
}

void Settings::setTextures(bool value) {
    textures = value;
}

void Settings::setFacetado(bool value)
{
    this->facetado = value;
}

void Settings::setInterpolado(bool value)
{
    this->interpolado = value;
    if (interpolado)
        glShadeModel(GL_SMOOTH);
    else
        glShadeModel(GL_FLAT);
}

void Settings::setTexSettings(TEX_SETTINGS settings) {
    texSettings = settings;
}

void Settings::setLightAngle(int angle) {
    lightAngle = angle;

    glPushMatrix();
    glRotatef(120, 0, 1.0f, 1.0f);
    GLfloat lightPos[] = { 10.0f * cos(lightAngle), 10.0f * sin(lightAngle), 0.0f, 0.0f }; // Last value: 1.0 = positional, 0.0 = directional
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glPopMatrix();
}

void Settings::setLightColor(Vec3 color, float alpha) {
    lightColor = color;
    lightAlpha = alpha;
    GLfloat lightArray[] = {color.x,color.y,color.z, alpha };

    glLightfv(GL_LIGHT0, SELECTED_LIGHT, lightArray);
}

void Settings::setSelectedLight(GLenum light)
{
    this->SELECTED_LIGHT = light; //Podr[ia validar si es uno de los 3 enums que espero, pero yo conf[io que si ;)
}

void Settings::setVolume(int volume)
{
    this->volume = volume;
    if (GameController::getInstance()->getSoundEnabled()) {
        Mix_Volume(-1, volume);
        Mix_VolumeMusic(volume);
    }
}

int Settings::getVolume() {
    return volume;
}
