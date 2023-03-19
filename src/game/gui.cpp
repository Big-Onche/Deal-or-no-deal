#include "main.h"
#include "game.h"
#include "textures.h"
#include <functional>

using namespace std;

namespace gui
{
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
        TextureManager::getInstance().drawShadowedTex("GameLogo", (screenw - 500) / 2, (screenh - 500) / 2, 500, 500, renderer, 0xFFFFFF, 0x191919, 20, 20, 100);

        int tw, th; // text width, text height
        int textSize = 3;

        getTextSize(font[MainFont], text, tw, th, textSize);

        int x = (screenw - tw) / 2;
        int y = (screenh - th) / 1.05f;

        renderText(MainFont, text, x, y, textSize);
    }

    void renderMenu() // yeah yeah: arrays for menu items, loops and shit, let me make a playable game first
    {
        TextureManager& textureManager = TextureManager::getInstance();

        textureManager.drawShadowedTex("GameLogo", 90, 100, 210, 210, renderer, 0xFFFFFF, 0x191919, 5, 5, 100);

        int textSize = 3;

        int x = 100, y = 350;
        string newGameText = "New Game";
        newGameRect = {x, y, static_cast<int>(newGameText.length()) * cw[MainFont] * textSize, ch[MainFont] * textSize}; // Play
        textureManager.drawShadowedTex("RemainingPrices", x, y, static_cast<int>(newGameText.length()) * cw[MainFont] * textSize, ch[MainFont] * textSize, renderer, 0x888888, 0x222222);
        renderShadowedText(font[MainFont], newGameText, x, y, textSize, 0xFFFFFF, 0x444444);

        y+=75;
        string optionsText = "Options";
        optionsRect = {x, y, static_cast<int>(optionsText.length()) * cw[MainFont] * textSize, ch[MainFont] * textSize}; // Options
        textureManager.drawShadowedTex("RemainingPrices", x, y, static_cast<int>(optionsText.length()) * cw[MainFont] * textSize, ch[MainFont] * textSize, renderer, 0x888888, 0x222222);
        renderShadowedText(font[MainFont], optionsText, x, y, textSize, 0xFFFFFF, 0x444444);

        y+=75;
        string exitText = "Exit";

        exitRect = {x, y, static_cast<int>(exitText.length()) * cw[MainFont] * textSize, ch[MainFont] * textSize}; // Exit
        textureManager.drawShadowedTex("RemainingPrices", x, y, static_cast<int>(exitText.length()) * cw[MainFont] * textSize, ch[MainFont] * textSize, renderer, 0x888888, 0x222222);
        renderShadowedText(font[MainFont], exitText, x, y, textSize, 0xFFFFFF, 0x444444);
    }
}

