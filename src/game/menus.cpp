#include "main.h"
#include "game.h"

namespace menus
{
    Button *menuButton;
    enum {Btn_NewGame, Btn_Options, Btn_Quit, numMenuButtons};

    void menuQuit() { quit(); } // wrapper

    void initMainMenuButtons() // called when initializing the engine
    {
        int y = 350;

        menuButton = createButton(100, y, "New game", 4, 0x888888, game::initGame);
        addButton(menuButton);

        y += 70;

        menuButton = createButton(100, y, "Options", 4, 0x888888, game::initGame);
        addButton(menuButton);

        y += 70;

        menuButton = createButton(100, y, "Quit", 4, 0x888888, menuQuit);
        addButton(menuButton);
    }

    void renderMenu(TextureManager &textureManager)
    {
        textureManager.draw("MenuBackground", 0, 0, screenw, screenh, renderer);
        textureManager.drawShadowedTex("GameLogo", 90, 90, 220, 220, renderer, 0xFFFFFF, 0x191919, 5, 5, 100);

        loopi(numMenuButtons) drawButton(buttons[i], renderer); // iterate trough Btn_NewGame, Btn_Options, Btn_Quit
    }

    void renderSplashScreen(string text, TextureManager &textureManager) // showing splash screen
    {
        textureManager.drawShadowedTex("GameLogo", (screenw - 500) / 2, (screenh - 500) / 2, 500, 500, renderer, 0xFFFFFF, 0x191919, 20, 20, 100);
        int tw, th; // text width, text height
        int textSize = 3;
        getTextSize(font[MainFont], text, tw, th, textSize);
        int x = (screenw - tw) / 2;
        int y = (screenh - th) / 1.05f;
        renderText(MainFont, text, x, y, textSize);
    }

    void handleMenus(SDL_Event &event, SDL_Point &mousePoint)
    {
        loopi(numMenuButtons)
        {
            Button *button = buttons[i];
            if(button != nullptr)
            {
                bool isMouseOver = isMouseOverButton(button, mousePoint);

                button->isHover = isMouseOver;

                if(event.type == SDL_MOUSEBUTTONDOWN && isMouseOver)
                {
                    if (button->onClick != nullptr) button->onClick();
                }
            }
        }
    }
}
