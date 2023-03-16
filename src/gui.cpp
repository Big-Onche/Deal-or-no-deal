#include "main.h"
#include <SDL.h>
#include <SDL_image.h>
#include <functional>

using namespace std;

namespace gui
{
    void showSplashScreen(SDL_Renderer *renderer, SDL_Texture *fontTexture, SDL_Texture *gameLogo) // showing splash screen
    {
        if(!gameLogo) return;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        gl::renderCenteredTexture(renderer, gameLogo, SCREEN_WIDTH, SCREEN_HEIGHT);

        int tw, th; // text width, text height
        int textSize = 3;

        string text = "Press any key to continue";
        gl::getTextSize(text, tw, th, textSize);

        int x = (SCREEN_WIDTH - tw) / 2;
        int y = (SCREEN_HEIGHT - th) / 1.05f;

        gl::renderText(renderer, fontTexture, text, x, y, textSize);
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
                sound::playSound("money_loss"); // just a test
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

    void renderMenu(SDL_Renderer *renderer, SDL_Texture *fontTexture) // yeah yeah: arrays for menu items, loops and shit, let me make a playable game first
    {
        int textSize = 3;

        int x = 100, y = 200;
        std::string newGameText = "New Game";
        gl::renderText(renderer, fontTexture, newGameText, x, y, textSize);
        newGameRect = {x, y, static_cast<int>(newGameText.length()) * gl::cw * textSize, gl::ch * textSize}; // Play

        y+=75;
        std::string optionsText = "Options";
        gl::renderText(renderer, fontTexture, optionsText, x, y, textSize);
        optionsRect = {x, y, static_cast<int>(optionsText.length()) * gl::cw * textSize, gl::ch * textSize}; // Options

        y+=75;
        std::string exitText = "Exit";
        gl::renderText(renderer, fontTexture, exitText, x, y, textSize);
        exitRect = {x, y, static_cast<int>(exitText.length()) * gl::cw * textSize, gl::ch * textSize}; // Exit
    }
}

