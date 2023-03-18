#include "main.h"
#include "textures.h"

int screenw = 1280;
int screenh = 720;
float scalew = 1280.f;
float scaleh = 720.f;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

namespace sdl
{
    void sdlInit() // initing SDL
    {
        logoutf("init: gl");
        SDL_SetMainReady();

        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            logoutf("Unable to initialize SDL (%s)\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0)
        {
            logoutf("Unable to initialize SDL_image (%s)\n", IMG_GetError());
            exit(EXIT_FAILURE);
        }

        window = SDL_CreateWindow("Deal or no Deal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenw, screenh, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if(window == NULL)
        {
            logoutf("Unable to create window (%s)\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if(renderer == NULL)
        {
            logoutf("Unable to initialize renderer (%s)\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        TextureManager::getInstance().preloadTextures();
    }

    bool sdlLoop() // renderer loop
    {
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT) return false; // quit

            switch(event.type)
            {
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    {
                        int newWidth, newHeight;
                        SDL_GetWindowSize(window, &newWidth, &newHeight);
                        scalew = static_cast<float>(newWidth) / 1280;
                        scaleh = static_cast<float>(newHeight) / 720;
                        SDL_RenderSetScale(renderer, scalew, scaleh);
                    }
                    break;
            }

            gui::handleKeyboardEvents(event);
            gui::handleMouseEvents(event);
        }

        SDL_RenderClear(renderer);

        switch(engineState)
        {
            case S_Initialization: // game intro splash screen
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                gui::renderSplashScreen("Press any key to continue.");
                break;

            case S_MainMenu: // main menu
                gui::renderMenu();
                break;

            case S_LoadingScreen: // loading screen
                gui::renderSplashScreen("Loading...");
                break;

            case S_InGame: // in game
                SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
                render::renderGame();
                break;

            case S_ShuttingDown:
                break;
        }

        SDL_RenderPresent(renderer);
        return true;
    }

    void renderCenteredTexture(SDL_Renderer *renderer, SDL_Texture *texture, int screenw, int screenh) // render an image at the center of the window
    {
        int imgw, imgh;
        SDL_QueryTexture(texture, NULL, NULL, &imgw, &imgh);

        SDL_Rect destRect;
        destRect.x = (screenw - imgw) / 2;
        destRect.y = (screenh - imgh) / 2;
        destRect.w = imgw;
        destRect.h = imgh;

        SDL_RenderCopy(renderer, texture, NULL, &destRect);
    }

    void sdlQuit()
    {
        logoutf("shutdown: gl");
        TextureManager::getInstance().clearTextures();
        IMG_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}

