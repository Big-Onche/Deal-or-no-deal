#include "main.h"
#include "textures.h"
#include <functional>

using namespace std;

namespace gui
{
    bool fullscreen = false;

    void handleKeyboardEvents(SDL_Event &event)
    {
        if(event.type == SDL_KEYDOWN && engineState==S_Initialization) { engineState=S_MainMenu; } // exit splash screen
        else if (event.key.keysym.sym == SDLK_F11 && event.type == SDL_KEYDOWN) // toggle fullscreen (rescaling is called in sdlLoop(), SDL_WINDOWEVENT)
        {
            fullscreen = !fullscreen;
            if(fullscreen) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            else SDL_SetWindowFullscreen(window, 0);
        }
    }

    SDL_Rect newGameRect;
    SDL_Rect optionsRect;
    SDL_Rect exitRect;

    void handleMouseEvents(SDL_Event &event)
    {
        if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            SDL_RenderGetScale(renderer, &scalew, &scaleh);
            SDL_Point mousePoint = { static_cast<int>(mouseX / scalew), static_cast<int>(mouseY / scaleh) };

            switch(engineState)
            {
                case S_MainMenu:
                    if(SDL_PointInRect(&mousePoint, &newGameRect))
                    {
                        engineState = S_LoadingScreen;
                        game::initGame();
                    }
                    else if(SDL_PointInRect(&mousePoint, &optionsRect))
                    {
                    }
                    else if(SDL_PointInRect(&mousePoint, &exitRect))
                    {
                        quit();
                    }
                    break;

                case S_InGame:
                    game::handleGameEvents(event, mousePoint);
                    break;

                default: break;
            }
        }
    }

    void renderSplashScreen(string text) // showing splash screen
    {
        TextureManager& textureManager = TextureManager::getInstance();
        textureManager.draw("GameLogo", (screenw - 500) / 2, (screenh - 500) / 2, 500, 500, renderer);

        int tw, th; // text width, text height
        int textSize = 3;

        getTextSize(font[MainFont], text, tw, th, textSize);

        int x = (screenw - tw) / 2;
        int y = (screenh - th) / 1.05f;

        renderText(MainFont, text, x, y, textSize);
    }

    void renderMenu() // yeah yeah: arrays for menu items, loops and shit, let me make a playable game first
    {
        int textSize = 3;

        int x = 100, y = 200;
        string newGameText = "New Game";
        renderText(font[MainFont], newGameText, x, y, textSize);
        newGameRect = {x, y, static_cast<int>(newGameText.length()) * cw[MainFont] * textSize, ch[MainFont] * textSize}; // Play

        y+=75;
        string optionsText = "Options";
        renderText(font[MainFont], optionsText, x, y, textSize);
        optionsRect = {x, y, static_cast<int>(optionsText.length()) * cw[MainFont] * textSize, ch[MainFont] * textSize}; // Options

        y+=75;
        string exitText = "Exit";
        renderText(font[MainFont], exitText, x, y, textSize);
        exitRect = {x, y, static_cast<int>(exitText.length()) * cw[MainFont] * textSize, ch[MainFont] * textSize}; // Exit
    }
}

