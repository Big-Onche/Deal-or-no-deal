#include "main.h"
#include "game.h"
#include "particles.h"

bool fullscreen;
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
        logoutf("init: image");
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

    int mouseX, mouseY;

    bool handleEvents() // events then render loop
    {
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT) return false; // quit

            if(event.type == SDL_KEYDOWN && engineState==S_Initialization && event.key.keysym.sym!=SDLK_F11) { engineState=S_MainMenu; } // exit splash screen
            else if (event.key.keysym.sym == SDLK_F11 && event.type == SDL_KEYDOWN) // toggle fullscreen (rescaling is called in sdlLoop(), SDL_WINDOWEVENT)
            {
                fullscreen = !fullscreen;
                if(fullscreen) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                else SDL_SetWindowFullscreen(window, 0);
            }

            switch(event.type) // window resized
            {
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    {
                        int newWidth, newHeight;
                        SDL_GetWindowSize(window, &newWidth, &newHeight);
                        scalew = static_cast<float>(newWidth) / 1280;
                        scaleh = static_cast<float>(newHeight) / 720;
                        SDL_RenderSetScale(renderer, scalew, scaleh); // set scale to get a responsive 16:9 screen
                    }
                    break;
            }

            SDL_GetMouseState(&mouseX, &mouseY);
            SDL_RenderGetScale(renderer, &scalew, &scaleh);
            SDL_Point mousePoint = { static_cast<int>(mouseX / scalew), static_cast<int>(mouseY / scaleh) };

            switch(engineState)
            {
                case S_MainMenu: menus::handleMenus(event, mousePoint); break;
                case S_InGame: game::handleGame(event, mousePoint);
                default: break;
            }
        }
        return true;
    }

    void render(TextureManager& textureManager)
    {
        SDL_RenderClear(renderer); // cleaning all rendered things

        render::drawBackground(textureManager);

        particlesManager.update(1.f/maxfps);
        particlesManager.render(renderer);

        switch(engineState)
        {
            case S_Initialization: // game intro splash screen
                SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
                menus::renderSplashScreen("Press any key to continue.", textureManager);
                break;

            case S_MainMenu: // main menu
                menus::renderMenu(textureManager);
                break;

            case S_LoadingScreen: // loading screen
                menus::renderSplashScreen("Loading...", textureManager);
                break;

            case S_InGame: // in game
                SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
                render::renderGame(textureManager);
                break;

            case S_ShuttingDown:
                break;
        }

        SDL_RenderPresent(renderer); // rendering all things
    }

    void sdlQuit()
    {
        logoutf("shutdown: sdl");
        IMG_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}

