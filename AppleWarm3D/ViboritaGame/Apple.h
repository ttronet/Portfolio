#pragma once
#include "GameController.h"
#include "IGameEntity.h"
#include "utils.h"

class Apple :
    public IGameEntity
{
private:
    float angle;
public:
    Apple(Vec3 gridIndexes, Vec3 position);
    void process(float deltaTime);
    void draw();
    void eatApple();
    GAME_ENTITY_TYPE getType();
};

