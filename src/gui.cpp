#include "main.h"
#include <functional>

using namespace std;

namespace gui
{
    bool fullscreen = false;

    void handleKeyboardEvents(SDL_Event &event)
    {
        if(event.type == SDL_KEYDOWN && engineState==S_Initialization) { engineState=S_MainMenu; } // exit splash screen
        else if (event.key.keysym.sym == SDLK_F11 && event.type == SDL_KEYDOWN) // toggle fullscreen
        {
            fullscreen = !fullscreen;
            if(fullscreen) SDL_SetWindowFullscreen(gl::window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            else SDL_SetWindowFullscreen(gl::window, 0);
            SDL_GetWindowSize(gl::window, &screenw, &screenh);
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
            SDL_Point mousePoint = {mouseX, mouseY};

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
                        quit(); // the only useful btn atm
                    }
                    break;

                case S_InGame:
                    game::handleGameEvents(event, mousePoint);
                    break;

                default: break;
            }
        }
        else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) // window resize
        {
            screenw = event.window.data1; screenh = event.window.data2;
        }
    }

    void renderSplashScreen(string text) // showing splash screen
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

        gl::renderText(text, x, y, textSize);
    }

    void renderMenu() // yeah yeah: arrays for menu items, loops and shit, let me make a playable game first
    {
        int textSize = 3;

        int x = 100, y = 200;
        std::string newGameText = "New Game";
        gl::renderText(newGameText, x, y, textSize);
        newGameRect = {x, y, static_cast<int>(newGameText.length()) * gl::cw * textSize, gl::ch * textSize}; // Play

        y+=75;
        std::string optionsText = "Options";
        gl::renderText(optionsText, x, y, textSize);
        optionsRect = {x, y, static_cast<int>(optionsText.length()) * gl::cw * textSize, gl::ch * textSize}; // Options

        y+=75;
        std::string exitText = "Exit";
        gl::renderText(exitText, x, y, textSize);
        exitRect = {x, y, static_cast<int>(exitText.length()) * gl::cw * textSize, gl::ch * textSize}; // Exit
    }
}

