#include "IGameState.h"

void IGameState::process(float deltaTime)
{
}

void IGameState::draw()
{
}

void IGameState::resize()
{
}

std::vector<IHudElement*> IGameState::getHudElements()
{
	std::vector<IHudElement*> buttons;
	return buttons;
}
