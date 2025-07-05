#include "HudController.h"
#include "LevelButton.h"

HudController* HudController::instance = NULL;

HudController::HudController() {

}

HudController* HudController::getInstance()
{
	if (instance == NULL)
		instance = new HudController();

	return instance;
}

void HudController::process(float deltaTime)
{
    GameController* gc =  GameController::getInstance();
    Vec2 mousePos = gc->getMousePos();
    std::vector<IHudElement*> hud = gc->getHudElements();
    SDL_Rect mouse;
    mouse.h = 1;
    mouse.w = 1;
    mouse.x = mousePos.x;
    mouse.y = mousePos.y;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();                    
    glLoadIdentity();
    glOrtho(0, gc->getWindowSize().x, gc->getWindowSize().y, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();                     
    glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(0,0,0);
    for (IHudElement* hudElement : hud) {
        bool mouseOver = SDL_HasIntersection(&mouse, hudElement->getRect());
        Button* btn;
        LevelButton* lvlbtn;
        Slider* slider;
        TextField* tf;
        hudElement->process(deltaTime);
        switch (hudElement->getType())
        {
        case BUTTON:
            btn = (Button*)hudElement;
            if (mouseOver && gc->clicked())
                btn->handleClick();
            if ((!mouseOver && btn->isHovering()) || (mouseOver && !btn->isHovering()))
                btn->handleHover();
            break;
        case SLIDER:
            slider = (Slider*)hudElement;
            mouseOver |= SDL_HasIntersection(&mouse, slider->getSelectorRect());
            if (mouseOver && gc->isMouseDown())
                slider->mouseDown(mousePos);
            if (gc->isMouseUp())
                slider->mouseUp();
            slider->processMousePos(mousePos);           
            break;
        case CHECKBOX:
            break;
        case TEXTFIELD:
            tf = (TextField*)hudElement;
            if (gc->clicked())
                mouseOver ? tf->click(mousePos) : tf->clickOutside();
            tf->setKeyPressed(gc->getKeyPressed());
            tf->processKeyPress();
            tf->setKeyPressed("");
            break;
        case LEVELBUTTON:
            lvlbtn = (LevelButton*)hudElement;
            if (mouseOver && gc->clicked())
                lvlbtn->click();
            if ((!mouseOver && lvlbtn->isHovering()) || (mouseOver && !lvlbtn->isHovering()))
                lvlbtn->handleHover();
            break;
        default:
            break;
        }
        hudElement->draw();
    }
    glColor3f(0, 0, 0);
    glDisable(GL_BLEND);
    glPopMatrix();                      
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();                      
    glMatrixMode(GL_MODELVIEW);        
}

void HudController::setFont(TTF_Font* mainFont)
{
    this->mainFont = mainFont;
    IHudElement::setFont(mainFont);
}

TTF_Font* HudController::getFont()
{
    return mainFont;
}
