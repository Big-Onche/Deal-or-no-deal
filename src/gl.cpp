#include "main.h"
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

namespace gl
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    void glInit() // initing SDL
    {
        logoutf("init: gl");
        SDL_SetMainReady();

        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            logoutf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0)
        {
            logoutf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
            exit(EXIT_FAILURE);
        }

        window = SDL_CreateWindow("Deal or no Deal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            logoutf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL) {
            logoutf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }

    SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *file) // load an image
    {
        SDL_Surface *surface = IMG_Load(file);
        if(!surface) { logoutf("Error loading image: %s\n", IMG_GetError()); return NULL; }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if(!texture) { logoutf("Error creating texture: %s\n", SDL_GetError()); return NULL; }

        return texture;
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

    void showSplashScreen() // showing splash screen
    {
        SDL_Texture *logo = loadTexture(renderer, "data/gui/logo.jpg");
        if(!logo) logoutf("Error loading logo texture\n");
        else
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            renderCenteredTexture(renderer, logo, SCREEN_WIDTH, SCREEN_HEIGHT);
            SDL_RenderPresent(renderer);
        }
    }

    bool glLoop() // (future) renderer loop
    {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        showSplashScreen();

        SDL_RenderPresent(renderer);
        return true;
    }

    void glQuit()
    {
        logoutf("shutdown: gl");
        IMG_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}

