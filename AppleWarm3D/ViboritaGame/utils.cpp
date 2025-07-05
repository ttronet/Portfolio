#include "utils.h"

float distance(const Vec3& a, const Vec3& b) {
	float dx = b.x - a.x;
	float dy = b.y - a.y;
	float dz = b.z - a.z;
	return std::sqrt(dx * dx + dy * dy + dz * dz);
}

GLfloat cubeTextureCoords[] = {
	// FRONT face (column=1, row=1)
	texW * 1, texH * 2,   // Bottom-left
	texW * 2, texH * 2,   // Bottom-right
	texW * 2, texH * 1,   // Top-right
	texW * 1, texH * 1,   // Top-left

	// BACK face (column=3, row=1)
	texW * 3, texH * 2,
	texW * 4, texH * 2,
	texW * 4, texH * 1,
	texW * 3, texH * 1,

	// LEFT face (column=0, row=1)
	texW * 0, texH * 2,
	texW * 1, texH * 2,
	texW * 1, texH * 1,
	texW * 0, texH * 1,

	// RIGHT face (column=2, row=1)
	texW * 2, texH * 2,
	texW * 3, texH * 2,
	texW * 3, texH * 1,
	texW * 2, texH * 1,

	// TOP face (column=1, row=0)
	texW * 1, texH * 1,
	texW * 2, texH * 1,
	texW * 2, texH * 0,
	texW * 1, texH * 0,

	// BOTTOM face (column=1, row=2)
	texW * 1, texH * 3,
	texW * 2, texH * 3,
	texW * 2, texH * 2,
	texW * 1, texH * 2,
};

void drawCube(GLfloat vertices[24], GLfloat colors[24], GLubyte indices[24]) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colors);
	for (int i = 0; i < 6; ++i) {
		glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, &indices[i * 4]);
	}
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

Vec3* getVec3FromVec3(Vec3 vecPrev) {
	Vec3* vec = new Vec3;
	vec->x = vecPrev.x;
	vec->y = vecPrev.y;
	vec->z = vecPrev.z;
	return vec;
}
GLuint grassTexture = 0;
GLuint dirtTexture = 0;
std::map<std::string, GLuint> textures;

void loadTexture(GLuint& textureId, const char* path, float &w,float &h) {
	if (textures.count(path) > 0) {
		textureId = textures.at(path);
		SDL_Surface* surface = IMG_Load(path);
		w = surface->w;
		h = surface->h;
		SDL_FreeSurface(surface);
		return;
	}

	SDL_Surface* surface = IMG_Load(path);
	w = surface->w;
	h = surface->h;
	SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
	SDL_FreeSurface(surface);

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	GLenum format = (converted->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, converted->w, converted->h, 0,
		format, GL_UNSIGNED_BYTE, converted->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	SDL_FreeSurface(converted);
	textures.insert({path,textureId});
}

void loadTexture(GLuint& textureId, const char* path) {
	float w = 0;
	float h = 0;
	loadTexture(textureId, path, w, h);
}

void loadTextTexture(GLuint& textureId, const char* text,TTF_Font* font, int &width, int &height) {
	SDL_Surface* surfaceText = TTF_RenderText_Blended(font, text, {255,255,255});
	width = surfaceText->w;
	height = surfaceText->h;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surfaceText->w, surfaceText->h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, surfaceText->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	SDL_FreeSurface(surfaceText);
}

GLfloat baseCubeVertices[] = {
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f
};
GLfloat baseKeyboardKeyVertices[] = {
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.5f, 0.0f,
	0.0f, 0.5f, 0.0f,

	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.5f, 1.0f,
	0.0f, 0.5f, 1.0f
};

GLfloat baseCubeColors[] = {
	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,

};

GLubyte baseCubeIndices[] = {
	0, 1, 2, 3, // Front face
	4, 5, 6, 7, // Back face
	0, 1, 5, 4, // Left face
	1, 2, 6, 5, // Right face
	2, 3, 7, 6, // Top face
	0, 3, 7, 4  // Bottom face
};


void drawPyramid(GLfloat vertices[12], GLfloat colors[12], GLuint indices[12]) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colors);
	for (int i = 0; i < 4; ++i) {
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &indices[i * 3]);
	}

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

GLfloat* basePyramidVertices = new GLfloat[12]{
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 
	0.5f, 0.0f, 0.866f,
	0.5f, 1.0f, 0.288f 
};

GLfloat* basePyramidColors = new GLfloat[12]{
	1.0f, 0.388f, 0.278f,
	1.0f, 0.288f, 0.278f,
	1.0f, 0.388f, 0.278f,
	1.0f, 0.288f, 0.278f,
};

GLuint basePyramydIndices[] = {
	0, 1, 2,  // Base triangle (ABC)

	0, 1, 3,  // Side triangle (AB + top)
	1, 2, 3,  // Side triangle (BC + top)
	2, 0, 3   // Side triangle (CA + top)
};

GLuint arrowTextures[4];

void drawArrowKeys(Vec3 position) {
	glEnable(GL_LIGHTING);
	Vec3 color = { 0.82f, 0.80f, 0.72f };
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	glScalef(1, 0.5, 1);
	glTranslatef(1.2f,0,0);
	drawCubeWithNormals(color, true, arrowTextures[1]); // ^
	
	glPopMatrix();

	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	glScalef(1, 0.5, 1);
	glTranslatef(0, 0, -1.2f);
	drawCubeWithNormals(color, true, arrowTextures[2]); // <
	
	glTranslatef(1.2f, 0, 0);
	drawCubeWithNormals(color, true, arrowTextures[3]); // \/

	glTranslatef(1.2f, 0, 0);
	drawCubeWithNormals(color, true, arrowTextures[0]); // >
	glPopMatrix();
	glDisable(GL_LIGHTING);
}

Vec3 crossProduct(Vec3 a, Vec3 b) {
	return {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

Vec3 normalize(Vec3 v) {
	float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (length == 0.0f) return { 0, 0, 0 }; // avoid division by zero
	return { v.x / length, v.y / length, v.z / length };
}

GLfloat baseViboritaColors[24] = {
			0.0f, 0.25f, 0.0f,
			0.0f, 0.30f, 0.0f,
			0.0f, 0.35f, 0.0f,
			0.0f, 0.40f, 0.0f,
			0.0f, 0.45f, 0.0f,
			0.0f, 0.50f, 0.0f,
			0.0f, 0.55f, 0.0f,
			0.0f, 0.60f, 0.0f
};
GLfloat headColors[24] = {
			0.0f, 0.25f, 0.3f,
			0.0f, 0.30f, 0.3f,
			0.0f, 0.35f, 0.3f,
			0.0f, 0.40f, 0.3f,
			0.0f, 0.45f, 0.3f,
			0.0f, 0.50f, 0.3f,
			0.0f, 0.55f, 0.3f,
			0.0f, 0.60f, 0.3f
};

std::vector<modelInfo> modelsInfo;
GLuint cubeTexture;

void cargarModelo(std::string& filePath, std::string name,int pos) {
	// Implementación de cargarModelo
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath,
		aiProcess_Triangulate |
		aiProcess_GenNormals |
		aiProcess_FlipUVs | // Opcional: puede ser necesario para algunos formatos/texturas
		aiProcess_JoinIdenticalVertices);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "Error al cargar el archivo OBJ con Assimp: " << importer.GetErrorString() << std::endl;
		return ;
	}

	std::vector<subMesh> subMeshes;
	subMeshes.resize(scene->mNumMeshes);

	// Procesar cada malla en la escena
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
		const aiMesh* mesh = scene->mMeshes[i];;

		// Procesar vértices
		for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
			VertexData vertex;

			// Posición
			vertex.position.x = mesh->mVertices[j].x;
			vertex.position.y = mesh->mVertices[j].y;
			vertex.position.z = mesh->mVertices[j].z;

			// Normales (asegurarse de que existan)
			if (mesh->HasNormals()) {
				vertex.normal.x = mesh->mNormals[j].x;
				vertex.normal.y = mesh->mNormals[j].y;
				vertex.normal.z = mesh->mNormals[j].z;

				subMeshes[i].normals.push_back(mesh->mNormals[j].x);
				subMeshes[i].normals.push_back(mesh->mNormals[j].y);
				subMeshes[i].normals.push_back(mesh->mNormals[j].z);
			}
			else {
				vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f); // Si no hay normales, ponerlas a cero
			}

			// Coordenadas de Textura (UVs) - Tomamos el primer conjunto si existe
			if (mesh->HasTextureCoords(i)) {
				vertex.texCoord.x = mesh->mTextureCoords[i][j].x;
				vertex.texCoord.y = mesh->mTextureCoords[i][j].y;
			}
			else {
				vertex.texCoord = glm::vec2(0.0f, 0.0f);
			}

			// Colores de Vértice - Tomamos el primer conjunto si existe
			if (mesh->HasVertexColors(i)) {
				vertex.color.r = mesh->mColors[i][j].r;
				vertex.color.g = mesh->mColors[i][j].g;
				vertex.color.b = mesh->mColors[i][j].b;
				vertex.color.a = mesh->mColors[i][j].a;
			}
			else {
				vertex.color = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f); // Color blanco por defecto
			}

			subMeshes[i].vertices.push_back(vertex);
		}

		// Procesar caras (índices)
		for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
			const aiFace& face = mesh->mFaces[j];
			for (unsigned int k = 0; k < face.mNumIndices; ++k) {
				subMeshes[i].indices.push_back(face.mIndices[k]);
			}
			const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			aiColor4D diffuse;
			if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
				subMeshes[i].materialDiffuseColor = glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
			}
			else {
				subMeshes[i].materialDiffuseColor = glm::vec4(1.0f); // Default white
			}
		}

	}

	if (modelsInfo.size() <= pos) {
		modelsInfo.resize(pos + 1);
	}

	// Guardar el VAO para usarlo en el bucle de renderizado
	modelsInfo[pos].name = name;
	modelsInfo[pos].subMeshes = subMeshes;
}

void drawModel(MODEL_TYPE modelType, bool textures) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	int indexModel;
	switch (modelType)
	{
	case APPLE_MODEL:
		indexModel = 0;
		break;
	case WORM_HEAD_MODEL:
		indexModel = 1;
		break;
	case WORM_BODY_MODEL:
		indexModel = 2;
		break;
	case WORM_TAIL_MODEL:
		indexModel = 3;
		break;
	case GOAL_MODEL:
		indexModel = 4;
		break;
	case SPIKED_APPLE_MODEL:
		indexModel = 5;
		break;
	case TITLE_MODEL:
		indexModel = 6;
		break;
	case ERASOR_MODEL:
		indexModel = 7;
		break;
	case WORLD_MODEL:
		indexModel = 8;
		break;
	case WORM_DEAD_HEAD_MODEL:
		indexModel = 9;
		break;
	default:
		break;
	}
	//if (textures) {
	//	glEnable(GL_TEXTURE_2D);
	//	glBindTexture(GL_TEXTURE_2D, modelsInfo[indexModel].textureId);
	//}
	
	for (const subMesh& mesh : modelsInfo[indexModel].subMeshes) {
		glColor4f(
			mesh.materialDiffuseColor.r,
			mesh.materialDiffuseColor.g,
			mesh.materialDiffuseColor.b,
			mesh.materialDiffuseColor.a
		);
		const VertexData* vertexDataPtr = mesh.vertices.data();
		const GLuint* indexData = mesh.indices.data();
		const GLfloat* normalsData = mesh.normals.data();

		glVertexPointer(3, GL_FLOAT, sizeof(VertexData), &vertexDataPtr[0].position);
		glNormalPointer(GL_FLOAT, 0, normalsData);
		glTexCoordPointer(2, GL_FLOAT, sizeof(VertexData), &vertexDataPtr[0].texCoord);

		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, indexData);
	}
	glColor4f(0,0,0,1);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//if (textures) {
	//	glDisable(GL_TEXTURE_2D);
	//}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

GLfloat cubeNormalsColors[] = {
	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,

	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,

	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,

	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,

	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,

	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,

};


GLfloat normalCubeVertices[] = {
	// Front face
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,

	// Back face
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,

	// Left face
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f,

	// Right face
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,

	// Top face
	0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	// Bottom face
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
};

GLfloat cubeNormals[] = {
	// Front face normals
	 0.0f,  0.0f,  1.0f,
	 0.0f,  0.0f,  1.0f,
	 0.0f,  0.0f,  1.0f,
	 0.0f,  0.0f,  1.0f,

	 // Back face normals
	  0.0f,  0.0f, -1.0f,
	  0.0f,  0.0f, -1.0f,
	  0.0f,  0.0f, -1.0f,
	  0.0f,  0.0f, -1.0f,

	  // Left face normals
	  -1.0f,  0.0f,  0.0f,
	  -1.0f,  0.0f,  0.0f,
	  -1.0f,  0.0f,  0.0f,
	  -1.0f,  0.0f,  0.0f,

	  // Right face normals
	   1.0f,  0.0f,  0.0f,
	   1.0f,  0.0f,  0.0f,
	   1.0f,  0.0f,  0.0f,
	   1.0f,  0.0f,  0.0f,

	   // Top face normals
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,

		// Bottom face normals
		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
};

GLuint cubeNormalsIndices[] = {
	0, 1, 2, 3,     // Front face
	4, 5, 6, 7,     // Back face
	8, 9,10,11,     // Left face
   12,13,14,15,     // Right face
   16,17,18,19,     // Top face
   20,21,22,23      // Bottom face
};


void drawCubeWithNormals(Vec3 color, bool textures, GLuint textureId) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	if (textures && textureId != -1) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexCoordPointer(2, GL_FLOAT, 0, cubeTextureCoords);
	}
	glColor3f(color.x,color.y,color.z);
	glVertexPointer(3, GL_FLOAT, 0, normalCubeVertices);
	glNormalPointer(GL_FLOAT, 0, cubeNormals);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, &cubeNormalsIndices);
	if (textures) {
		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glColor3f(0, 0, 0);
}

Light defaultLight = { { 0.0f, 10.0f, 0.0f, 1.0f } ,{ 0.2f, 0.2f, 0.2f, 1.0f },{ 0.8f, 0.8f, 0.8f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } };
Material defaultMaterial = { {0.2f, 0.2f, 0.2f, 1.0f} , {0.8f, 0.8f, 0.8f, 1.0f} ,  {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f} };

void applyMaterial(Material m) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, m.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, m.shininess);
}


//glLightfv(GL_LIGHT0, GL_POSITION, defaultLight.position);
//glLightfv(GL_LIGHT0, GL_AMBIENT, defaultLight.ambient);
//glLightfv(GL_LIGHT0, GL_DIFFUSE, defaultLight.diffuse);
//glLightfv(GL_LIGHT0, GL_SPECULAR, defaultLight.specular);

Vec3 subtractV3(Vec3 v,Vec3 u) {
	float x = v.x - u.x;
	float y = v.y - u.y;
	float z = v.z - u.z;

	return { x,y,z };
}

Vec2 subtractV2(Vec2 v, Vec2 u)
{
	int x = v.x - u.x;
	int y = v.y - u.y;
	return { x,y };
}
Vec3 sumV3(Vec3 v, Vec3 u) {
	float x = v.x + u.x;
	float y = v.y + u.y;
	float z = v.z + u.z;

	return { x,y,z };
}

Vec2 sumV2(Vec2 v, Vec2 u)
{
	int x = v.x + u.x;
	int y = v.y + u.y;
	return { x,y };
}

Vec3 scalarProdV3(float scalar, Vec3 v)
{
	return {v.x * scalar, v.y * scalar, v.z * scalar};
}

bool equalsV3(Vec3 v, Vec3 u)
{
	return v.x == u.x && v.y == u.y && v.z == u.z;
}

//Used for Hud elements that are rendered as 3d objects, it returns a pos in 3d that is always in the same position in the screen depending on the offsets and distance
void calc3dCoordsForHud(Vec3 cameraPos, Vec3 center, float distance, float xOffset, float yOffset, Vec3& pos) {

	//Normalized directions with respect to the vector from your face to the center of the viewport
	Vec3 forward = normalize({ center.x - cameraPos.x,center.y - cameraPos.y ,center.z - cameraPos.z });
	Vec3 right = normalize(crossProduct(forward, { 0,1,0 }));
	Vec3 up = normalize(crossProduct(right, forward));

	pos = {
		cameraPos.x + forward.x * distance + right.x * xOffset + up.x * yOffset,
		cameraPos.y + forward.y * distance + right.y * xOffset + up.y * yOffset,
		cameraPos.z + forward.z * distance + right.z * xOffset + up.z * yOffset
	};
}

Mix_Chunk* sounds[10] = {NULL,NULL,NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

float getRandomFloat(float min, float max)
{
	if (min >= max) {
		std::swap(min, max); // ← This avoids the crash
	}
	static std::random_device rd;  // Non-deterministic random device
	static std::mt19937 gen(rd()); // Mersenne Twister engine
	std::uniform_real_distribution<float> dist(min, max);
	return dist(gen);
}

int getRandomInt(int min, int max) {
	if (min >= max) {
		std::swap(min, max); // ← This avoids the crash
	}
	static std::random_device rd;  // Seed
	static std::mt19937 gen(rd()); // Random number generator
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}

void playSound(SOUND_ENUM sound)
{
	int i = -1;
	switch (sound) {
	case HOVER:
		i = 0;
		break;
	case MOVING:
		i = 1;
		break;
	case CRUNCH:
		i = getRandomInt(2, 3);
		break;
	}
	if (i != -1 && sounds[i] != NULL)
		Mix_PlayChannel(-1, sounds[i], 0);
}

GLuint skyboxTextures[6];