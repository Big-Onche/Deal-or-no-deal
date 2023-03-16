#include "main.h"
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

namespace gl
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    void glInit()
    {
        logoutf("init: gl");
        SDL_SetMainReady();

        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            logoutf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        SDL_Window *window = SDL_CreateWindow("Deal or no Deal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

        if (window == NULL) {
            logoutf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL) {
            logoutf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }

    bool glLoop()
    {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // stuff goes here

        SDL_RenderPresent(renderer);
        return true;
    }

    void glQuit()
    {
        logoutf("gl: uninit");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}

