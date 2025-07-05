#include "utils.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <GL/glu.h>
#include <iostream>
#include "GameController.h"
#include "LevelCreator.h"


void calculateArrowKeysPos(Vec3 cameraPos, Vec3 center, Vec3& arrowKeyPos) {
	
	//Normalized directions with respect to the vector from your face to the center of the viewport
	Vec3 forward = normalize({ center.x - cameraPos.x,center.y - cameraPos.y ,center.z - cameraPos.z });
	Vec3 right = normalize(crossProduct(forward,{0,1,0}));
	Vec3 up = normalize(crossProduct(right, forward));

	//Offsets
	float distance = 30;
	float xOffset = 12;
	float yOffset = -10;

	arrowKeyPos = {
		cameraPos.x + forward.x*distance + right.x*xOffset + up.x*yOffset,
		cameraPos.y + forward.y * distance + right.y * xOffset + up.y * yOffset,
		cameraPos.z + forward.z * distance + right.z * xOffset + up.z * yOffset
	};
}
void loadFonts() {
	TTF_Init();
	TTF_Font* mainFont = TTF_OpenFont("fonts/Sans Serif Shaded.ttf", 24);
	if (mainFont == NULL)
		printf("Failed to load font");
	HudController::getInstance()->setFont(mainFont);
}

void loadModelsAndTextures() {
	std::string filePath = "models/apple.obj";
	std::string name = "";
	GLuint textureId;
	GLuint grassTextureId;
	GLuint dirtTextureId;
	GLuint portalTexId;
	cargarModelo(filePath, name, 0);// Se le pasa el filePath, el nombre del modelo y la posición que ocupará en el modelsInfo
	filePath = "models/worm_head.obj";
	cargarModelo(filePath, name, 1);
	filePath = "models/worm_body.obj";
	cargarModelo(filePath, name, 2);
	filePath = "models/worm_tail.obj";
	cargarModelo(filePath, name, 3);
	filePath = "models/portal.obj";
	cargarModelo(filePath, name, 4);
	filePath = "models/peaks_apple.obj";
	cargarModelo(filePath, name, 5);
	filePath = "models/title.obj";
	cargarModelo(filePath, name, 6);
	filePath = "models/erasor.obj";
	cargarModelo(filePath, name, 7);
	filePath = "models/world.obj";
	cargarModelo(filePath, name, 8);
	filePath = "models/worm_dead_head.obj";
	cargarModelo(filePath, name, 9);
	loadTexture(grassTextureId, "textures/grass.png");
	loadTexture(dirtTextureId, "textures/dirt.png");
	loadTexture(portalTexId, "textures/portal_texture.jpg");
	modelsInfo[4].textureId = portalTexId;
	grassTexture = grassTextureId;
	dirtTexture = dirtTextureId;

	GLuint tId = 0;
	loadTexture(tId, "textures/arrows/right.png");
	arrowTextures[0] = tId;
	loadTexture(tId, "textures/arrows/down.png");
	arrowTextures[1] = tId;
	loadTexture(tId, "textures/arrows/left.png");
	arrowTextures[2] = tId;
	loadTexture(tId, "textures/arrows/up.png");
	arrowTextures[3] = tId;

	loadTexture(tId, "images/levelCompleted.png");
	loadTexture(tId, "images/levelNotCompleted.png");
}

void setupLighting() {
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_NORMALIZE);

	GLfloat lightPos[] = { 0.0f, 10.0f, 10.0f, 0.0f };
	GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

void setupSounds() {
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		SDL_Log("Failed to init SDL: %s", SDL_GetError());
		return;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		SDL_Log("Failed to init SDL_mixer: %s", Mix_GetError());
		return;
	}

	GameController::getInstance()->setSoundEnabled(true);

	Mix_Chunk* hoverSound = Mix_LoadWAV("sounds/hover.wav");
	if (!hoverSound) {
		SDL_Log("Failed to load sound: %s", Mix_GetError());
	}
	else {
		sounds[0] = hoverSound;
	}
	Mix_Chunk* movingSound = Mix_LoadWAV("sounds/moving.wav");
	if (!movingSound) {
		SDL_Log("Failed to load sound: %s", Mix_GetError());
	}
	else {
		sounds[1] = movingSound;
	}
	Mix_Chunk* crunch1 = Mix_LoadWAV("sounds/appleCrunch1.wav");
	if (!crunch1) {
		SDL_Log("Failed to load sound: %s", Mix_GetError());
	}
	else {
		sounds[2] = crunch1;
	}
	Mix_Chunk* crunch2 = Mix_LoadWAV("sounds/appleCrunch2.wav");
	if (!crunch2) {
		SDL_Log("Failed to load sound: %s", Mix_GetError());
	}
	else {
		sounds[3] = crunch2;
	}

}

void updatePerspective(Vec2 windowSize) {
	glMatrixMode(GL_PROJECTION);

	gluPerspective(45, windowSize.x / (float)windowSize.y, 0.1, 100);

	glMatrixMode(GL_MODELVIEW);
}

float skyAngle = 0;
void drawSkybox(float size) {
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glScalef(15, 15, 15);
	glRotatef(skyAngle,0,1,0);
	drawModel(WORLD_MODEL,false); //Literally not a skybox
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

using namespace std;
int main(int argc, char* argv[]) {

	//	Init sld
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		exit(1);
	}
	
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); //	THE GOD LINE

	Vec2 windowSize = { 640,480 };

	SDL_Window* win = SDL_CreateWindow("Snak3d",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowSize.x, windowSize.y,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_GLContext context = SDL_GL_CreateContext(win);

	IMG_Init(IMG_INIT_PNG);

	loadFonts();

	setupSounds();

	if (glewInit() != GLEW_OK)
	{
		// Manejar error de inicialización de GLEW
		return -1;
	}

	GameController* gc = GameController::getInstance();

	gc->setWindowSize(windowSize);

	//Camera setup
	glMatrixMode(GL_PROJECTION);

	glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
	gluPerspective(45, windowSize.x / (float)windowSize.y, 0.1, 100);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);

	gc->setState(new MainMenu());
	gc->setSettings(new Settings());

	loadModelsAndTextures();
	
	bool moveCamera = false;
	bool alreadyMoved = false;
	SDL_Event event;
	Vec3 cameraPos = { 0,0,0 };
	Vec3 center = { 0,0,0 };
	Uint64 currentTick = SDL_GetPerformanceCounter();
	Uint64 lastTick = currentTick;

	cameraPos.x = gc->getCameraProps().z * sin(gc->getCameraProps().y) * cos(gc->getCameraProps().x);
	cameraPos.y = gc->getCameraProps().z * cos(gc->getCameraProps().y);
	cameraPos.z = gc->getCameraProps().z * sin(gc->getCameraProps().y) * sin(gc->getCameraProps().x);
	gc->setCameraPos(cameraPos);
	Vec3 arrowKeysPos = { 0,0,0 };
	Vec3 lookDirection = { 0,0,0 };

	calculateArrowKeysPos(cameraPos,center, arrowKeysPos);

	float timeFromDown = 100;
	float CLICK_TIME = 0.8f;

	setupLighting();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		currentTick = SDL_GetPerformanceCounter();

		float deltaTime = (currentTick - lastTick) / (float)SDL_GetPerformanceFrequency();
		deltaTime *= gc->getSettings()->getGameSpeed();
		lastTick = currentTick;
		timeFromDown += deltaTime/gc->getSettings()->getGameSpeed();
		
		skyAngle += 1.0f * deltaTime;

		//son 3 vector3, donde me paro, donde miro, y donde est[a arriba
		gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, center.x, center.y, center.z, 0, 1, 0);
		
		drawSkybox(0.5f);

		gc->processFrame(deltaTime);

		if (gc->getGamePlay() != NULL) {
			float vel = 0;
			switch (gc->getCameraType()) {
			case WORLD_CAMERA:
				gc->setNormalControl(true);
				cameraPos.x = gc->getCameraProps().z * sin(gc->getCameraProps().y) * cos(gc->getCameraProps().x);
				cameraPos.y = gc->getCameraProps().z * cos(gc->getCameraProps().y);
				cameraPos.z = gc->getCameraProps().z * sin(gc->getCameraProps().y) * sin(gc->getCameraProps().x);
				center.x = 0;
				center.y = 0;
				center.z = 0;
				break;
			case FREE_CAMERA:
				cameraPos = gc->getCameraPos();
				vel = 10;
				Vec3 front = normalize(subtractV3(cameraPos,center));
				Vec3 right = normalize(crossProduct(front, { 0,1,0 }));
				if (gc->isWKey()) {
					cameraPos = sumV3(cameraPos, scalarProdV3(-vel*deltaTime,front)); //Front is back...
				}
				if (gc->isSKey()) {
					cameraPos = sumV3(cameraPos, scalarProdV3(vel * deltaTime, front));
				}
				if (gc->isAKey()) {
					cameraPos = sumV3(cameraPos, scalarProdV3(vel * deltaTime, right)); //... and right is left???
				}
				if (gc->isDKey()) {
					cameraPos = sumV3(cameraPos, scalarProdV3(-vel * deltaTime, right));
				}

				center.x = cameraPos.x + sin(gc->getCameraProps().y) * cos(gc->getCameraProps().x);
				center.y = cameraPos.y + cos(gc->getCameraProps().y);
				center.z = cameraPos.z + sin(gc->getCameraProps().y) * sin(gc->getCameraProps().x);
				gc->setCameraPos(cameraPos);
				break;
			case WORM_CAMERA:
				gc->getGamePlay()->getViborita()->getFPCamera(cameraPos, center);
				break;
			}
		}
		else {
			cameraPos.x = gc->getCameraProps().z * sin(gc->getCameraProps().y) * cos(gc->getCameraProps().x);
			cameraPos.y = gc->getCameraProps().z * cos(gc->getCameraProps().y);
			cameraPos.z = gc->getCameraProps().z * sin(gc->getCameraProps().y) * sin(gc->getCameraProps().x);
			center.x = 0;
			center.y = 0;
			center.z = 0;
		}

		gc->setArrowRight(false);
		gc->setArrowLeft(false);
		gc->setArrowDown(false);
		gc->setArrowUp(false);
		gc->setZKey(false);
		gc->setXKey(false);
		gc->setClick(false);
		gc->setMouseUp(false);
		gc->setSpaceKey(false);
		gc->setMouseDown(false);
		gc->setKeyPressed("");

		if(gc->getGamePlay() == gc->getState() && gc->getCameraType() != WORM_CAMERA)
			drawArrowKeys({ arrowKeysPos.x-1.7f,arrowKeysPos.y,arrowKeysPos.z });		
	
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				gc->close();
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym >= SDLK_SPACE && event.key.keysym.sym <= SDLK_z) {
					std::string key;
					key.push_back(static_cast<char>(event.key.keysym.sym));
					gc->setKeyPressed(key);
				}
				else {
					switch (event.key.keysym.sym) {
						case SDLK_RETURN: gc->setKeyPressed("ENTER"); break;
						case SDLK_BACKSPACE:gc->setKeyPressed("BACKSPACE"); break;
						default: gc->setKeyPressed(""); break;
					}
				}
				switch (event.key.keysym.sym) {
					case SDLK_p:
						if (!gc->isPaused() && gc->getGamePlay() != gc->getState())
							break;

						gc->setPaused(!gc->isPaused());
						if (gc->isPaused()) {
							gc->setState(new SettingsMenu(gc->getSettings()));
						}
						else {
							gc->setState(gc->getGamePlay());
						}
						break;
					case SDLK_ESCAPE:
						gc->close();
						break;
					case SDLK_UP:
					case SDLK_DOWN:
					case SDLK_LEFT:
					case SDLK_RIGHT:
					case SDLK_x:
					case SDLK_z:
					case SDLK_SPACE:
						alreadyMoved = false;
						break;
					case SDLK_TAB:
						gc->setShowFps(!gc->isShowFps());
						break;
					case SDLK_v:
						gc->setCameraType(static_cast<CAMERA_ENUM>((gc->getCameraType() + 1) % CAMERA_COUNT));
						break;
					case SDLK_r:
						if (gc->getGamePlay() == gc->getState())
							gc->getGamePlay()->resetLevel();
						break;
					case SDLK_w:
						gc->setWKey(false);
						break;
					case SDLK_s:
						gc->setSKey(false);
						break;
					case SDLK_a:
						gc->setAKey(false);
						break;
					case SDLK_d:
						gc->setDKey(false);
						break;
				}
				break;
			
			case SDL_KEYDOWN:
				if (alreadyMoved)
					break;

				switch (event.key.keysym.sym) {
				case SDLK_UP:
					gc->setArrowUp(true);
					alreadyMoved = true;
					break;
				case SDLK_DOWN:
					gc->setArrowDown(true);
					alreadyMoved = true;
					break;
				case SDLK_LEFT:
					gc->setArrowLeft(true);
					alreadyMoved = true;
					break;
				case SDLK_RIGHT:
					gc->setArrowRight(true);
					alreadyMoved = true;
					break;
				case SDLK_z:
					gc->setZKey(true);
					alreadyMoved = true;
					break;
				case SDLK_x:
					gc->setXKey(true);
					alreadyMoved = true;
					break;
				case SDLK_SPACE:
					gc->setSpaceKey(true);
					alreadyMoved = true;
					break;
				case SDLK_w:
					gc->setWKey(true);
					break;
				case SDLK_s:
					gc->setSKey(true);
					break;
				case SDLK_a:
					gc->setAKey(true);
					break;
				case SDLK_d:
					gc->setDKey(true);
					break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				moveCamera = true;
				timeFromDown = 0.0f;
				gc->setMouseDown(true);
				SDL_SetRelativeMouseMode(SDL_TRUE);
				break;
			case SDL_MOUSEBUTTONUP:
				moveCamera = false;
				if(timeFromDown <= CLICK_TIME)
					gc->setClick(true);
				gc->setMouseUp(true);
				SDL_SetRelativeMouseMode(SDL_FALSE);
				break;
			case SDL_MOUSEMOTION:
				gc->setMousePos({ event.motion.x, event.motion.y });
				if (moveCamera && gc->getMoveCamera()) {
					int deltaX = event.motion.xrel;
					int deltaY = event.motion.yrel;

					/*int horDir = deltaX != 0 ? deltaX / abs(deltaX) : 0;
					int verDir = deltaY != 0 ? deltaY / abs(deltaY) : 0;*/ //Noble but flawed idea sadly :c
					float sensitivity = gc->getSensitivity();
					float cameraRadius = gc->getCameraProps().z;
					float theta = gc->getCameraProps().x + sensitivity * deltaTime * deltaX;
					float phi = gc->getCameraProps().y + sensitivity * sin(deltaTime * deltaY);

					float epsilon = 0.5f;
					if (phi < epsilon) phi = epsilon;
					if (phi > M_PI - epsilon) phi = M_PI - epsilon;

					if (gc->getCameraType() == WORLD_CAMERA) {
						cameraPos.x = cameraRadius * sin(phi) * cos(theta);
						cameraPos.y = cameraRadius * cos(phi);
						cameraPos.z = cameraRadius * sin(phi) * sin(theta);

						gc->setCameraPos(cameraPos);
						calculateArrowKeysPos(cameraPos, center, arrowKeysPos);
					}	

					gc->setCameraCoordinates(theta,phi);
				}

				break;
			case SDL_WINDOWEVENT:
				if (event.window.event != SDL_WINDOWEVENT_RESIZED)
					break;
				printf("resized");
				SDL_GetWindowSize(win, &windowSize.x, &windowSize.y);
				gc->setWindowSize(windowSize);

				glViewport(0, 0, windowSize.x, windowSize.y);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				gluPerspective(45.0, windowSize.x / (float)windowSize.y, 0.1, 100.0);
				glMatrixMode(GL_MODELVIEW);
				gc->getState()->resize();
				break;
			case SDL_MOUSEWHEEL:
				int scrollY = event.wheel.y;
				if (scrollY != 0 && gc->getCameraType() == WORLD_CAMERA) {
					float sensitivity = gc->getSensitivity();
					float theta = gc->getCameraProps().x;
					float phi = gc->getCameraProps().y;
					float cameraRadius = gc->getCameraProps().z;

					float movement = deltaTime * sensitivity * scrollY * 40;
					cameraRadius += -movement;
					if (cameraRadius < 0.5 || cameraRadius > 40)
						cameraRadius += movement;


					cameraPos.x = cameraRadius * sin(phi) * cos(theta);
					cameraPos.y = cameraRadius * cos(phi);
					cameraPos.z = cameraRadius * sin(phi) * sin(theta);
					
					gc->setCameraPos(cameraPos);
					gc->setCameraRadius(cameraRadius);
					calculateArrowKeysPos(cameraPos, center, arrowKeysPos);
				}

				break;
			}

		}

		SDL_GL_SwapWindow(win);
	} while (!gc->shouldClose());

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}




