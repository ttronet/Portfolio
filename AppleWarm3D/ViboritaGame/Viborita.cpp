#include "Viborita.h"
#include "GameController.h"

Viborita::Viborita(Vec3 gridIndexes, Vec3 position, GLfloat colors[24]) : IGameEntity(gridIndexes,position) {
	for (int i = 0; i < 24; i++) {
		this->viboritaColors[i] = colors[i]; 
	}

	ViboritaPart* head = new ViboritaPart;
	head->gridIndex = gridIndexes;
	head->position = position;
	head->dirToFront = { 0,0,1 };
	head->orientationToFront = { 0,0,1 };
	head->next = nullptr;
	this->falling = 0;
	this->fallen = false;

	body.head = head;
	body.tail = head;
	body.size = 1;

	headDirection = { 0,0,0 };

	movingProgress = 1;
}

void Viborita::setGameContext(GamePlay* context)
{
	this->gameContext = context;
}

void translateAndRotateBodyPart(Vec2 effectiveAngle, Vec3 posOffset) {
	glTranslatef(posOffset.x, posOffset.y, posOffset.z);
	glRotatef(effectiveAngle.y, 0, 1, 0); //ROTATE FROM y FIRST
	glRotatef(effectiveAngle.x, 1, 0, 0);
}
//Este es mi bebe, mi funcioncita de manejar las rotaciones del cuerpo de la v[ibora <3
//dir: es un vector con valores -1 0 o 1, las posiciones x y z son exclusivas entre si, si una es distinta de 0, la otra es 0
//y puede tener valor junto a x y z, de hecho se espera, as[i adem[as de mirar para arriba se orienta correctamente la parte del cuerpo con respecto al plano x z
//rf: inicialmente era rotationFactor, pero al final no se us[o como factor pers[e, es la diferencia de la direcci[on hacia la parte de adelante, y la parte de atr[as de esa parte del cuerpo
//ej, si tenemos una vibora de 3 partes, rf tiene la diferencia de: hacia donde va la cabeza con respecto al torso y hacia donde va la cola con respecto al torso.
//rf nos permite detectar las curvas, ya que si las dos partes van en la misma direcci[on, va a venir como 0
void calculatePartRotation(Vec3 dir,Vec3 dirToBack,Vec3 orientation, bool end,	 Vec2 &finalAngle, Vec3 &finalOffset) {  //Los params de retorno nos permiten almacenar para el pr[oximo frame la rotaci[on
	Vec3 rf = subtractV3(dir,dirToBack); //dirToBack viene negado, por eso se resta, que en realidad se tendr[ia que hacer una suma

	int xAngle = 0;
	int xDir = 0;
	if (dir.y != 0) {
		xAngle = 90;
		if (rf.y == -1)
			xAngle += dir.y*45;
		else if (rf.y == 1)
			xAngle -= dir.y*45;

		xDir = -dir.y;
	}else if (dirToBack.y != 0) {
		xAngle = 90;
		if (rf.y == -1)
			xAngle -= dirToBack.y * 45;
		else if (rf.y == 1)
			xAngle += dirToBack.y * 45;

		xDir = -dirToBack.y;
	}
	int yAngle = 0;
	int yDir = 0;
	if (orientation.x != 0) {
		yAngle = 90;
		if (rf.z == 1)
			yAngle += 45;
		if (rf.z == -1)
			yAngle -= 45;
		
		yDir = orientation.x;
	}
	else if (orientation.z != 0) {
		yAngle = orientation.z == 1 ? 0 : 180;
		if (rf.x == -1)
			yAngle += 45;
		if (rf.x == 1)
			yAngle -= 45;

		yDir = orientation.z;
	}

	float offset = GameController::getInstance()->TILE_SIZE / 2.0f;
	Vec3 posOffset = { rf.y != 0 ? -rf.x * offset : 0,
						rf.y != 0 ? dir.y * offset : 0,
						rf.y != 0 ? -rf.z * offset : 0};

	if (!end) {
		posOffset.x = rf.x * offset;
		posOffset.z = rf.z * offset;
	}

	Vec2 effectiveAngle = { xDir * xAngle, yDir * yAngle };
	finalAngle = { effectiveAngle.x,effectiveAngle.y };
	finalOffset = { posOffset.x,posOffset.y,posOffset.z };
}

void Viborita::draw() {
	float TILE_SIZE = GameController::getInstance()->TILE_SIZE;
	ViboritaPart* bodyPart = this->body.head;
	ViboritaPart* frontPart = NULL;
	Vec3 cero = { 0,0,0 };
	Vec3 headColor = { 0.0f, 0.25f, 0.3f };
	Vec3 viboritaColor = { 0.0f, 0.40f, 0.0f };
	glColor3f(0.0f, 0.50f, 0.0f);

	while(bodyPart != NULL)
	{
		glPushMatrix();

		Vec3 posOffset = { 0,0,0 };
		if (movingProgress != 1) {
			float cmovingProgress = 1 - movingProgress;
			posOffset = { bodyPart->lastMovementDir.x * cmovingProgress * TILE_SIZE, bodyPart->lastMovementDir.y * cmovingProgress * TILE_SIZE, bodyPart->lastMovementDir.z * cmovingProgress * TILE_SIZE };
		}

		glTranslatef(bodyPart->position.x - posOffset.x, bodyPart->position.y - posOffset.y - (TILE_SIZE * this->falling), bodyPart->position.z - posOffset.z);

		glTranslatef(TILE_SIZE/2, TILE_SIZE/6,TILE_SIZE/2);

		Vec2 currentRotation = { 0,0 };
		Vec3 currentOffset = {0,0,0};

		glScalef(0.5f * TILE_SIZE , 0.5f * TILE_SIZE, 0.5f * TILE_SIZE);
		if (bodyPart == this->body.head) {
			calculatePartRotation(this->body.head->dirToFront, this->body.head->dirToFront, bodyPart->orientationToFront, true, currentRotation, currentOffset);
			
			Vec2 rotationDiff = subtractV2(currentRotation, bodyPart->lastRotation);
			Vec3 offsetDiff = subtractV3(currentOffset,bodyPart->lastOffset);

			float xRotation = rotationDiff.x * movingProgress;
			float yRotation = rotationDiff.y * movingProgress;
			offsetDiff = { offsetDiff.x * movingProgress,offsetDiff.y * movingProgress,offsetDiff.z * movingProgress };

			translateAndRotateBodyPart(sumV2(bodyPart->lastRotation,{(int)xRotation,(int)yRotation}),sumV3(bodyPart->lastOffset, offsetDiff));
			//translateAndRotateBodyPart(currentRotation, currentOffset);

			drawModel(this->reachedPortal ? this->body.size == 1 ? WORM_TAIL_MODEL : WORM_BODY_MODEL : this->dead ? WORM_DEAD_HEAD_MODEL : WORM_HEAD_MODEL, GameController::getInstance()->getSettings()->hasTextures());
		}
		else if (bodyPart == this->body.tail) {
			calculatePartRotation(this->body.tail->dirToFront, this->body.tail->dirToFront, bodyPart->orientationToFront, true, currentRotation, currentOffset);
			Vec2 rotationDiff = subtractV2(currentRotation, bodyPart->lastRotation);
			Vec3 offsetDiff = subtractV3(currentOffset, bodyPart->lastOffset);

			float xRotation = rotationDiff.x * movingProgress;
			float yRotation = rotationDiff.y * movingProgress;
			offsetDiff = { offsetDiff.x * movingProgress,offsetDiff.y * movingProgress,offsetDiff.z * movingProgress };

			translateAndRotateBodyPart(sumV2(bodyPart->lastRotation, { (int)xRotation,(int)yRotation }), sumV3(bodyPart->lastOffset, offsetDiff));
			//translateAndRotateBodyPart(currentRotation, currentOffset);
			if (this->body.head->gridIndex == bodyPart->gridIndex) {

			}else
				drawModel(WORM_TAIL_MODEL, GameController::getInstance()->getSettings()->hasTextures());
		}else {
			Vec3 dirToBack = subtractV3(bodyPart->gridIndex, { bodyPart->next->gridIndex.x,bodyPart->next->gridIndex.y,bodyPart->next->gridIndex.z });
			calculatePartRotation(bodyPart->dirToFront,dirToBack,bodyPart->orientationToFront,false, currentRotation, currentOffset);
			Vec2 rotationDiff = subtractV2(currentRotation, bodyPart->lastRotation);
			Vec3 offsetDiff = subtractV3(currentOffset, bodyPart->lastOffset);

			float xRotation = rotationDiff.x * movingProgress;
			float yRotation = rotationDiff.y * movingProgress;
			offsetDiff = { offsetDiff.x * movingProgress,offsetDiff.y * movingProgress,offsetDiff.z * movingProgress };

			translateAndRotateBodyPart(sumV2(bodyPart->lastRotation, { (int)xRotation,(int)yRotation }), sumV3(bodyPart->lastOffset, offsetDiff));
			//translateAndRotateBodyPart(currentRotation, currentOffset);
			if (this->body.head->gridIndex == bodyPart->gridIndex) {

			}
			else
				drawModel(WORM_BODY_MODEL, GameController::getInstance()->getSettings()->hasTextures());
		}
		if (movingProgress == 1) {
			bodyPart->lastOffset = currentOffset;
			bodyPart->lastRotation = currentRotation;
		}
		if (GameController::getInstance()->isShowFps()) {
			glPopMatrix();
			
			glPushMatrix();
			glTranslatef(GameController::getInstance()->getGridPosition(bodyPart->gridIndex.x),
				GameController::getInstance()->getGridPosition(bodyPart->gridIndex.y),
				GameController::getInstance()->getGridPosition(bodyPart->gridIndex.z));
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			drawCubeWithNormals(this->body.head == bodyPart ? headColor : viboritaColor,false, -1);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			
		}
		
		glPopMatrix();
		frontPart = bodyPart;
		bodyPart = bodyPart->next;	
	}
	if (this->falling >= 1) {
		this->falling = 0;
	}
	glColor3f(0, 0, 0);
}

Vec3* Viborita::getMovementDirection()
{
	Vec3* movementDir = new Vec3;
	movementDir->x = 0;
	movementDir->y = 0;
	movementDir->z = 0;
	if (GameController::getInstance()->getNormalControl()) {

		if (GameController::getInstance()->isArrowUp())
		{
			movementDir->z = 1;
		}
		else if (GameController::getInstance()->isArrowDown())
		{
			movementDir->z = -1;
		}
		else if (GameController::getInstance()->isArrowLeft())
		{
			movementDir->x = 1;
		}
		else if (GameController::getInstance()->isArrowRight())
		{
			movementDir->x = -1;
		}
		else if (GameController::getInstance()->isZKey())
		{
			movementDir->y = 1;
		}
		else if (GameController::getInstance()->isXKey())
		{
			movementDir->y = -1;
		}
	}

	if(GameController::getInstance()->getCameraType() == WORM_CAMERA)
		if (headDirection.x == 1 || this->body.head->orientationToFront.x == 1) {
			GameController::getInstance()->setNormalControl(false);
			if (GameController::getInstance()->isArrowUp())
			{
				movementDir->x = 1;
			}
			else if (GameController::getInstance()->isArrowLeft())
			{
				movementDir->z = -1;
			}
			else if (GameController::getInstance()->isArrowRight())
			{
				movementDir->z = 1;
				GameController::getInstance()->setNormalControl(true);
			}
			else if (GameController::getInstance()->isZKey()) {
				movementDir->y = 1;
			}
			else if (GameController::getInstance()->isXKey()) {
				movementDir->y = -1;

			}

		}
		else if (headDirection.z == -1 || this->body.head->orientationToFront.z == -1) {
			GameController::getInstance()->setNormalControl(false);
			if (GameController::getInstance()->isArrowUp())
			{
				movementDir->z = -1;
			}
			else if (GameController::getInstance()->isArrowLeft())
			{
				movementDir->x = -1;
			}
			else if (GameController::getInstance()->isArrowRight())
			{
				movementDir->x = 1;
			}
			else if (GameController::getInstance()->isZKey()) {
				movementDir->y = 1;
			}
			else if (GameController::getInstance()->isXKey()) {
				movementDir->y = -1;

			}

		}
		else if (headDirection.x == -1 || this->body.head->orientationToFront.x == -1) {
			GameController::getInstance()->setNormalControl(false);
			if (GameController::getInstance()->isArrowUp())
			{
				movementDir->x = -1;
			}
			else if (GameController::getInstance()->isArrowLeft())
			{
				movementDir->z = 1;
				GameController::getInstance()->setNormalControl(true);
			}
			else if (GameController::getInstance()->isArrowRight())
			{
				movementDir->z = -1;
			}
			else if (GameController::getInstance()->isZKey()) {
				movementDir->y = 1;
			}
			else if (GameController::getInstance()->isXKey()) {
				movementDir->y = -1;

			}

		}
		else if (headDirection.z == 1 || this->body.head->orientationToFront.z == 1)
			GameController::getInstance()->setNormalControl(true);

	return movementDir;
}

bool Viborita::handleMovement(Vec3* movementDir, bool die) {
	Vec3 nextGridIndex = { this->body.head->gridIndex.x + movementDir->x,this->body.head->gridIndex.y + movementDir->y,this->body.head->gridIndex.z + movementDir->z };

	if (gameContext->hasSolidBlock(nextGridIndex)) //Se choca con un bloque
		return false;
	bool ateItself = gameContext->hasViborita(nextGridIndex) && !equalsV3(this->body.tail->gridIndex, nextGridIndex);
	bool ateSpikedApple = gameContext->hasSpikedApple(nextGridIndex);
	if (die && (ateItself || ateSpikedApple)) {
		dying = true;
	}
	if (gameContext->hasGoal(nextGridIndex)) {
		reachedPortal = true;
	}
	playSound(MOVING);
	headDirection = { movementDir->x,movementDir->y,movementDir->z };
	Vec3 dirCopy = {movementDir->x,movementDir->y,movementDir->z};
	Vec3 verticalHeadDir = { this->body.head->orientationToFront.x, 0, this->body.head->orientationToFront.z };
	this->body.head->dirToFront = { movementDir->x,movementDir->y,movementDir->z };
	this->body.head->lastMovementDir = headDirection;
	this->body.head->orientationToFront = dirCopy.y != 0 ? verticalHeadDir : dirCopy;

	Vec3* prevPos = getVec3FromVec3(this->body.head->position);
	Vec3* prevGrid = getVec3FromVec3(this->body.head->gridIndex);

	this->body.head->position.x += GameController::getInstance()->TILE_SIZE * movementDir->x;
	this->body.head->position.y += GameController::getInstance()->TILE_SIZE * movementDir->y;
	this->body.head->position.z += GameController::getInstance()->TILE_SIZE * movementDir->z;
	gameContext->clearTile(this->body.head->gridIndex);
	this->body.head->gridIndex = nextGridIndex;

	ViboritaPart* aux = this->body.head->next;
	ViboritaPart* prev = this->body.head;
	while (aux != NULL) {
		Vec3 auxPos = { aux->position.x,aux->position.y,aux->position.z };
		Vec3 auxGrid = { aux->gridIndex.x,aux->gridIndex.y,aux->gridIndex.z };
		
		aux->position = { prevPos->x,prevPos->y,prevPos->z };
		gameContext->clearTile(aux->gridIndex);
		aux->gridIndex = { prevGrid->x,prevGrid->y,prevGrid->z };
		gameContext->addViborita(aux->gridIndex);

		Vec3 movementDir = subtractV3({ prevGrid->x, prevGrid->y ,prevGrid->z }, auxGrid);
		aux->lastMovementDir = movementDir;

		Vec3 newDirToFront = subtractV3({ prev->gridIndex.x,prev->gridIndex.y,prev->gridIndex.z }, aux->gridIndex);
		Vec3 nDirCopy = { newDirToFront.x,newDirToFront.y,newDirToFront.z };
		Vec3 newOrientation = { aux->orientationToFront.x, 0, aux->orientationToFront.z };
		aux->dirToFront = newDirToFront;
		aux->orientationToFront = newDirToFront.y != 0 ? newOrientation : nDirCopy;

		prevPos = getVec3FromVec3(auxPos);
		prevGrid = getVec3FromVec3(auxGrid);
		prev = aux;
		aux = aux->next;
	}
	movingProgress = 0;
	return true;
}

void Viborita::handleEatApple(Vec3* lastTailPos, Vec3* lastTailGrid)
{
	ViboritaPart* newTail = new ViboritaPart;
	newTail->position = { lastTailPos->x,lastTailPos->y,lastTailPos->z };
	newTail->gridIndex = { lastTailGrid->x,lastTailGrid->y,lastTailGrid->z };
	newTail->dirToFront = subtractV3(this->body.tail->gridIndex, newTail->gridIndex);
	newTail->orientationToFront = { this->body.tail->orientationToFront.x,this->body.tail->orientationToFront.y,this->body.tail->orientationToFront.z};
	newTail->next = NULL;
	this->body.tail->next = newTail;
	this->body.tail = newTail;
	this->body.size++;
	gameContext->addViborita(newTail->gridIndex);
	gameContext->ateApple();
}

bool Viborita::hasFloor()
{
	ViboritaPart* aux = this->body.head;
	while (aux != NULL)
	{
		Vec3 indexUnderPart = { aux->gridIndex.x,aux->gridIndex.y - 1,aux->gridIndex.z };
		if (gameContext->hasSolidBlock(indexUnderPart) || gameContext->tileHasApple(indexUnderPart))
			return true;
		aux = aux->next;
	}
	return false;
}


void Viborita::handleFall()
{
	ViboritaPart* aux = this->body.head;
	while (aux != NULL)
	{
		gameContext->clearTile(aux->gridIndex);
		Vec3 positionUnderPart = { aux->position.x,aux->position.y - GameController::getInstance()->TILE_SIZE,aux->position.z};
		Vec3 indexUnderPart = { aux->gridIndex.x,aux->gridIndex.y - 1,aux->gridIndex.z };
		if (gameContext->hasSpikedApple(indexUnderPart)) { //Se cae en los pinchos :c
			dying = true;
			return; 
		}
		aux->position = positionUnderPart;
		aux->gridIndex = indexUnderPart;

		if (gameContext->hasGoal(aux->gridIndex)) { //Si te caes arriba de la meta xd
			if(aux == this->body.head)
				this->reachedPortal = true;
		}else
			gameContext->addViborita(aux->gridIndex);

		aux = aux->next;
	}
	if (this->body.head->gridIndex.y < 0 && this->gameContext != NULL) {
		GameController::getInstance()->setNormalControl(true);
		this->gameContext->resetLevel();
	}
}

void Viborita::handleDeath()
{
	/*ViboritaPart* aux = this->body.head;
	while (aux != NULL)
	{
		GameController::getInstance()->clearTile(aux->gridIndex);
		ViboritaPart* toDelete = aux;
		aux = aux->next;
		delete toDelete;
	}

	delete this;*/
	/*GameController::getInstance()->setNormalControl(true);
	gameContext->resetLevel();*/
	float TILESIZE = GameController::getInstance()->TILE_SIZE;
	Vec3 position = this->body.head->position;
	ParticleSystem* ps = new ParticleSystem(0.001f, 0.2f, 150);
	ps->setLifeSpan(0.2f, 0.2f);
	ps->setVelocityRange({ -3,1,-3 }, { 5,5,5 });
	ps->setAccelerationRange({ 0,-0.2f,0 }, { 0,-0.2,0 });
	ps->setPosRange({ position.x,position.y,position.z }, { position.x + TILESIZE, position.y + TILESIZE, position.z + TILESIZE });
	ps->setAlphaRange(0.5f, 1);
	ps->setColorRange({ 0,0.3f,0 }, { 0,0.8f,0 });
	ps->setSizeRange(4, 8);
	GameController::getInstance()->addParticles(ps);
	this->dead = true;
	this->gameContext->viboritaDied();
}

Viborita* Viborita::deepCopy()
{
	Viborita* copy = new Viborita(body.head->gridIndex,body.head->position,viboritaColors);
	ViboritaPart* aux = body.head->next;
	while (aux != NULL)
	{
		copy->addTail(aux->gridIndex);
		aux = aux->next;
	}
	return copy;
}

GAME_ENTITY_TYPE Viborita::getType()
{
	return VIBORITA;
}

float getCameraAngle(Vec3 dir) {
	if (dir.x == -1)
		return 0;
	else if (dir.x == 1)
		return M_PI;
	else if (dir.z == -1)
		return M_PI/2;
	else if(dir.z == 1)
		return -M_PI / 2;
}

float prevCameraAngle = 0;
const float ANGLEEPSILON = 0.0001f;
void Viborita::getFPCamera(Vec3& pos, Vec3& center)
{
	float TILESIZE = GameController::getInstance()->TILE_SIZE;
	Vec3 posOffset = { 0,0,0 };
	ViboritaPart* head = this->body.head;
	float cameraAngle = getCameraAngle(head->orientationToFront);
	if (movingProgress != 1) {
		float cmovingProgress = 1 - movingProgress;
		posOffset = { 
			head->lastMovementDir.x * cmovingProgress * TILESIZE, 
			head->lastMovementDir.y * cmovingProgress * TILESIZE, 
			head->lastMovementDir.z * cmovingProgress * TILESIZE 
		};

		if (fabs(cameraAngle - M_PI) < ANGLEEPSILON && fabs(prevCameraAngle + M_PI / 2) < ANGLEEPSILON)
			cameraAngle *= -1;
		else if (fabs(prevCameraAngle - M_PI) < ANGLEEPSILON && fabs(cameraAngle + M_PI / 2) < ANGLEEPSILON) 
			prevCameraAngle *= -1;
		

		cameraAngle += (prevCameraAngle - cameraAngle) * cmovingProgress;
	}
	else {
		prevCameraAngle = cameraAngle;
	}

	center.x = head->position.x - posOffset.x - headDirection.x * TILESIZE +  TILESIZE / 2;
	center.y = head->position.y - posOffset.y - headDirection.y * TILESIZE +  TILESIZE / 2;
	center.z = head->position.z - posOffset.z - headDirection.z * TILESIZE +  TILESIZE / 2;

	float radius = TILESIZE * 4;
	
	pos.x = center.x + cos(cameraAngle) * radius;
	pos.y = center.y + TILESIZE;
	pos.z = center.z + sin(cameraAngle) * radius;
	
	//Para visualizar
	if (GameController::getInstance()->isShowFps()) {
		glPointSize(8);
		glBegin(GL_POINTS);
		glColor4f(0, 0, 0, 1);
		glVertex3f(pos.x, pos.y, pos.z);
		glEnd();
		glBegin(GL_POINTS);
		glColor4f(0, 1, 0, 1);
		glVertex3f(center.x, center.y, center.z);
		glEnd();
		glColor3f(1, 1, 1);
	}
	
}

void Viborita::process(float deltaTime) {
	if (movingProgress != 1)
	{
		this->movingProgress += 10.0f * deltaTime;
		if (movingProgress > 1.0f)
			movingProgress = 1.0f;
		return;
	}
	if (dead) {
		return;
	}
	if (dying) {
		this->handleDeath();
		return;
	}
	if (reachedPortal) {
		if (this->body.size == 1) {
			this->gameContext->beatLevel();
			return;
		}
		else {
			this->body.head = this->body.head->next;
			this->body.size--;
			Vec3* dir = new Vec3;
			dir->x = headDirection.x;
			dir->y = headDirection.y;
			dir->z = headDirection.z;
			handleMovement(dir,false);
			return;
		}
	}
	
	if (!this->fallen && !hasFloor()) {
		this->falling = 0.00000001f;
		this->fallen = true;
		return;
	}

	if (this->fallen && falling != 0) {
		this->falling += 10.0f* deltaTime;
		if (falling > 1.0f)
			falling = 1.0f;
		return;
	}
	else {
		this->fallen = false;
	}

	if (!hasFloor()) {
		handleFall();
		return;
	}

	Vec3* movementDir = this->getMovementDirection();

	if (movementDir->x == 0 && movementDir->y == 0 && movementDir->z == 0)  //The user didn't press a movement key
		return;
	if (movementDir->x + headDirection.x == 0 && movementDir->y + headDirection.y == 0 && movementDir->z + headDirection.z == 0) //The user tried to move in the opossite direction
		return;

	Vec3* oldTailPos = getVec3FromVec3(this->body.tail->position);
	Vec3* oldTailGrid = getVec3FromVec3(this->body.tail->gridIndex);

	bool moved = this->handleMovement(movementDir, true);

	if (moved) {
		if (gameContext->tileHasApple(body.head->gridIndex))
		{
			gameContext->eatAppleAt(body.head->gridIndex);
			this->handleEatApple(oldTailPos, oldTailGrid);
		}
		if(!reachedPortal)
			gameContext->addViborita(this->body.head->gridIndex);
	}
}

ViboritaPart* Viborita::getHead() {
	return this->body.head;
}

int Viborita::getSize() {
	return this->body.size;
}

void Viborita::setHead() {
	if (this->body.head != NULL) {
		ViboritaPart* aux = this->body.head;
		this->body.head = this->body.head->next;
		if (this->body.head != NULL) {
			this->body.head->gridIndex = aux->gridIndex;
			this->body.head->position = this->body.head->position;
			this->body.size--;
		}
		delete aux;
	}
	else {
		std::cout << "No hay partes en el cuerpo de la viborita" << std::endl;
	}
}

void Viborita::processAnimation(Vec3* movementDir, float deltaTime) {
	if (movingProgress != 1)
	{
		this->movingProgress += 4*deltaTime;
		if (movingProgress > 1.0f)
			movingProgress = 1.0f;
		return;
	}
	if (reachedPortal) {
		if (this->body.size == 1) {
			this->gameContext->beatLevel();
			return;
		}
		else {
			this->body.head = this->body.head->next;
			this->body.size--;
			Vec3* dir = new Vec3;
			dir->x = headDirection.x;
			dir->y = headDirection.y;
			dir->z = headDirection.z;
			handleMovement(dir, false);
			return;
		}
	}

	if (!this->fallen && !hasFloor()) {
		this->falling = 0.00000001f;
		this->fallen = true;
		return;
	}

	if (this->fallen && falling != 0) {
		this->falling += 10.0f;
		if (falling > 1.0f)
			falling = 1.0f;
		return;
	}
	else {
		this->fallen = false;
	}

	if (!hasFloor()) {
		handleFall();
		return;
	}

	if (movementDir->x == 0 && movementDir->y == 0 && movementDir->z == 0)  //The user didn't press a movement key
		return;
	if (movementDir->x + headDirection.x == 0 && movementDir->y + headDirection.y == 0 && movementDir->z + headDirection.z == 0) //The user tried to move in the opossite direction
		return;

	Vec3* oldTailPos = getVec3FromVec3(this->body.tail->position);
	Vec3* oldTailGrid = getVec3FromVec3(this->body.tail->gridIndex);

	bool moved = this->handleMovement(movementDir,true);

	if (moved) {
		if (gameContext->tileHasApple(body.head->gridIndex))
		{
			gameContext->eatAppleAt(body.head->gridIndex);
			this->handleEatApple(oldTailPos, oldTailGrid);
		}
		if (!reachedPortal)
			gameContext->addViborita(this->body.head->gridIndex);
	}
}

void Viborita::addTail(Vec3 gridIndex)
{
	Vec3 dir = subtractV3(this->body.tail->gridIndex, gridIndex);
	float dirSum = (dir.x + dir.y + dir.z);
	if (abs(dirSum) != 1) //Una direcci[on tiene que ser x o y o z = -1 1 y el resto 0
		return;

	ViboritaPart* newTail = new ViboritaPart();
	newTail->next = NULL;
	newTail->gridIndex = gridIndex;
	newTail->position = { GameController::getInstance()->getGridPosition(gridIndex.x),GameController::getInstance()->getGridPosition(gridIndex.y) ,GameController::getInstance()->getGridPosition(gridIndex.z) };
	newTail->dirToFront = dir;
	if (this->body.head == this->body.tail) {
		this->headDirection = { newTail->dirToFront.x ,newTail->dirToFront.y ,newTail->dirToFront.z };
		this->body.head->dirToFront = { newTail->dirToFront.x ,newTail->dirToFront.y ,newTail->dirToFront.z };
		this->body.head->orientationToFront = { newTail->dirToFront.x ,newTail->dirToFront.y ,newTail->dirToFront.z };
	}
	Vec3 dirCopy = { newTail->dirToFront.x,newTail->dirToFront.y,newTail->dirToFront.z };
	Vec3 verticalOr = { this->body.tail->orientationToFront.x,dirCopy.y,this->body.tail->orientationToFront.z };
	newTail->orientationToFront = newTail->dirToFront.y != 0 ? verticalOr: dirCopy;
	this->body.tail->next = newTail;
	this->body.tail = newTail;
	this->body.size++;
}

void Viborita::removeBodyFromGridIndex(int x, int y, int z) {
	ViboritaPart* prev = this->body.head;
	ViboritaPart* aux = this->body.head->next;
	bool removeBody = false;

	if (prev->gridIndex.x == x && prev->gridIndex.z == z && prev->gridIndex.y == y) {
		removeBody = true;
		this->body.head = NULL;
		prev->next = NULL;
	}
	while (aux != NULL)
	{
		if (removeBody) {
			this->body.size--;
			//delete prev; TODO: agregar esto sin que crashee
		}
		if (aux->gridIndex.x == x && aux->gridIndex.z == z && aux->gridIndex.y == y){
			removeBody = true;
			prev->next = NULL;
			this->body.tail = prev;
		}		
		prev = aux;
		aux = aux->next;
	}
	if (removeBody) {
		this->body.size--;
		//delete prev; TODO: agregar esto sin que crashee
	}
}


void Viborita::loadInGrid()
{
	ViboritaPart* aux = body.head;
	while (aux != NULL)
	{
		gameContext->addViborita(aux->gridIndex);
		aux = aux->next;
	}
}
