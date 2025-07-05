#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <cmath>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SDL_mixer.h>
#include <random>
#include <map>

struct Vec3 {  
   float x, y, z;  

   // Sobrecarga del operador == 
   bool operator==(const Vec3& other) const {  
       return x == other.x && y == other.y && z == other.z;  
   }  
};  
struct Vec2 {
	int x, y;
};

struct VertexData {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec4 color;

};

struct ViboritaPart {  
	Vec3 dirToFront; 
	Vec3 orientationToFront; // y = 0 , x,z = -1 | 1
	Vec3 gridIndex;
	Vec3 position;

	Vec2 lastRotation = { 0,0 };
	Vec3 lastOffset = { 0,0,0 };
	Vec3 lastMovementDir = { 0,0,0 };

	ViboritaPart* next;  
};  

struct ViboritaBody {  
   int size;  
   ViboritaPart* head;  
   ViboritaPart* tail;  
};  

struct subMesh {
	std::vector<VertexData> vertices;
	std::vector<unsigned int> indices;
	std::vector<float> normals;
	glm::vec4 materialDiffuseColor;
};

struct modelInfo {
	std::string name;
	std::vector<subMesh> subMeshes;
	GLuint textureId;
};

enum BUTTON_TYPE { BUTTON, SLIDER, CHECKBOX, TEXTFIELD, LEVELBUTTON, MESSAGE};
enum GAME_ENTITY_TYPE {BLOCK,VIBORITA,APPLE,GOAL,CLOUD, SPIKED_APPLE, ERASOR};
enum TEX_SETTINGS {FACETADO, INTERPOLADO};
enum MODEL_TYPE {APPLE_MODEL, WORM_HEAD_MODEL, WORM_BODY_MODEL, WORM_TAIL_MODEL, GOAL_MODEL, SPIKED_APPLE_MODEL, TITLE_MODEL, ERASOR_MODEL, WORLD_MODEL, WORM_DEAD_HEAD_MODEL};

Vec3* getVec3FromVec3(Vec3 vecPrev);

extern std::vector<modelInfo> modelsInfo;//Arreglo que contiene la información de cada modelo

extern GLuint grassTexture;
extern GLuint dirtTexture;

extern std::map<std::string, GLuint> textures;
void loadTexture(GLuint& textureId, const char* path, float& w, float& h);
void loadTexture(GLuint& textureId, const char* path);
void loadTextTexture(GLuint& textureId, const char* text, TTF_Font* font, int& width, int& height);

float distance(const Vec3& a, const Vec3& b);
const float texW = 1.0f / 4.0f;
const float texH = 1.0f / 3.0f;
extern GLfloat cubeTextureCoords[];

void drawCube(GLfloat vertices[24], GLfloat colors[24], GLubyte indices[24]);

extern GLfloat baseCubeVertices[];  

extern GLfloat baseCubeColors[];  

extern GLubyte baseCubeIndices[];  

void drawPyramid(GLfloat vertices[12], GLfloat colors[12], GLuint indices[12]);  

extern GLfloat* basePyramidVertices;  

extern GLfloat* basePyramidColors;  

extern GLuint basePyramydIndices[];

extern GLfloat baseKeyboardKeyVertices[];

extern GLuint arrowTextures[4];

void drawArrowKeys(Vec3 position);

Vec3 crossProduct(Vec3 a, Vec3 b);

Vec3 normalize(Vec3 v);

extern GLfloat baseViboritaColors[24];
extern GLfloat headColors[24];

void cargarModelo(std::string& filePath, std::string name, int pos);

void drawModel(MODEL_TYPE,bool textures);

extern GLfloat cubeNormals[];
extern GLfloat cubeNormalsColors[];
extern GLuint cubeNormalsIndices[];
void drawCubeWithNormals(Vec3 color, bool textures, GLuint textureId);

struct Light {
	GLfloat position[4];
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
};
extern Light defaultLight;
struct Material {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess[1];
};
extern Material defaultMaterial;

void applyMaterial(Material);

Vec3 subtractV3(Vec3 v, Vec3 u);
Vec2 subtractV2(Vec2 v, Vec2 u);

Vec3 sumV3(Vec3 v, Vec3 u);
Vec2 sumV2(Vec2 v, Vec2 u);

Vec3 scalarProdV3(float scalar, Vec3 v);

bool equalsV3(Vec3 v, Vec3 u);

void calc3dCoordsForHud(Vec3 cameraPos, Vec3 center, float distance, float xOffset, float yOffset, Vec3& arrowKeyPos);

enum SOUND_ENUM {
	HOVER,
	MOVING,
	CRUNCH
};

extern Mix_Chunk* sounds[10];

void playSound(SOUND_ENUM sound);

float getRandomFloat(float min, float max);

int getRandomInt(int min, int max);


enum MESSAGES_ENUM {
	level1MovementTutorial,
	level2MovementTutorial,
	level3MovementTutorial,
};

extern GLuint skyboxTextures[6];

enum CAMERA_ENUM {
	WORLD_CAMERA,
	WORM_CAMERA,
	FREE_CAMERA,
	CAMERA_COUNT
};