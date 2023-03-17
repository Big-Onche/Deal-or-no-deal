#include "main.h"
#include <functional>

using namespace std;

namespace gui
{
    void showSplashScreen(string text) // showing splash screen
    {
        if(!gl::gameLogo) return;
        SDL_SetRenderDrawColor(gl::renderer, 0, 0, 0, 255);
        SDL_RenderClear(gl::renderer);
        gl::renderCenteredTexture(gl::renderer, gl::gameLogo, screenw, screenh);

        int tw, th; // text width, text height
        int textSize = 3;

        gl::getTextSize(text, tw, th, textSize);

        int x = (screenw - tw) / 2;
        int y = (screenh - th) / 1.05f;

        gl::renderText(gl::renderer, gl::fontTexture, text, x, y, textSize);
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
            SDL_Point mousePoint = {mouseX, mouseY};

            if(SDL_PointInRect(&mousePoint, &newGameRect))
            {
                engineStatus = S_LoadingScreen;
                game::initGame();
            }
            else if(SDL_PointInRect(&mousePoint, &optionsRect))
            {
            }
            else if(SDL_PointInRect(&mousePoint, &exitRect))
            {
                quit(); // the only useful btn atm
            }
        }
    }

    void renderMenu() // yeah yeah: arrays for menu items, loops and shit, let me make a playable game first
    {
        int textSize = 3;

        int x = 100, y = 200;
        std::string newGameText = "New Game";
        gl::renderText(gl::renderer, gl::fontTexture, newGameText, x, y, textSize);
        newGameRect = {x, y, static_cast<int>(newGameText.length()) * gl::cw * textSize, gl::ch * textSize}; // Play

        y+=75;
        std::string optionsText = "Options";
        gl::renderText(gl::renderer, gl::fontTexture, optionsText, x, y, textSize);
        optionsRect = {x, y, static_cast<int>(optionsText.length()) * gl::cw * textSize, gl::ch * textSize}; // Options

        y+=75;
        std::string exitText = "Exit";
        gl::renderText(gl::renderer, gl::fontTexture, exitText, x, y, textSize);
        exitRect = {x, y, static_cast<int>(exitText.length()) * gl::cw * textSize, gl::ch * textSize}; // Exit
    }

    void showLoadingScreen()
    {
        if(!gl::gameLogo) return;
        SDL_SetRenderDrawColor(gl::renderer, 0, 0, 0, 255);
        SDL_RenderClear(gl::renderer);
        gl::renderCenteredTexture(gl::renderer, gl::gameLogo, screenw, screenh);

        int tw, th; // text width, text height
        int textSize = 3;

        string text = "Loading...";
        gl::getTextSize(text, tw, th, textSize);

        int x = (screenw - tw) / 2;
        int y = (screenh - th) / 1.05f;

        gl::renderText(gl::renderer, gl::fontTexture, text, x, y, textSize);
    }
}

