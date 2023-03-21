#include "main.h"
#include "textures.h"

int buttonCount = 0;

Button *buttons[MAX_BUTTONS] = {nullptr};

Button *createButton(int x, int y, const char *text, int fontSize, uint32_t color, void (*onClick)(void))
{
    Button *button = (Button *)malloc(sizeof(Button));

    int tw, th;
    getTextSize(font[DialFont], text, tw, th, fontSize);

    button->rect.x = x;
    button->rect.y = y;
    button->rect.w = tw;
    button->rect.h = th;
    button->text = text;
    button->fontSize = fontSize;
    button->color = color;
    button->onClick = onClick;
    button->isHover = false;
    return button;
}

void addButton(Button *button)
{
    if(buttonCount < MAX_BUTTONS) buttons[buttonCount++] = button;
    else logoutf("warning: Maximum number of buttons reached. (%d)", buttonCount);
}

void drawButton(Button *button, SDL_Renderer *renderer) // draw a button
{
    int x = button->rect.x;
    int y = button->rect.y;
    int w = button->rect.w;
    int h = button->rect.h;
    uint32_t color = button->color;

    if(button->isHover) color = divideHexColor(color, 1.33f);

    TextureManager::getInstance().drawShadowedTex("Button", x - 4, y - 4, w + 4, h + 8, renderer, color, 0x222222, 10, 10, 75);
    renderOutlinedText(font[DialFont], button->text, x, y, button->fontSize, 0xFFFFFF, 0x333333);
}

bool isMouseOverButton(const Button *button, const SDL_Point &mousePoint) // check if mouse is over a button
{
    return (mousePoint.x >= button->rect.x) &&
           (mousePoint.x <= button->rect.x + button->rect.w) &&
           (mousePoint.y >= button->rect.y) &&
           (mousePoint.y <= button->rect.y + button->rect.h);
}

void freeButtons() // clean up
{
    loopi(MAX_BUTTONS)
    {
        if (buttons[i] != nullptr)
        {
            free(buttons[i]);
            buttons[i] = nullptr;
        }
    }
    buttonCount = 0; // Reset the button count
}
