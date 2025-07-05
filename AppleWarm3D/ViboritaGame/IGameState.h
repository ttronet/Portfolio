#pragma once
#include <iostream>
#include <set>
#include "Button.h"
#include "IHudElement.h"

class IGameState {
public:
	virtual void process(float deltaTime);
	virtual void draw();
	virtual void resize();
	virtual std::vector<IHudElement*> getHudElements();
	virtual ~IGameState(){}
};
