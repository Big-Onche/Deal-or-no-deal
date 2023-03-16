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
    SDL_Texture *gameLogo = NULL;
    SDL_Texture *fontTexture = NULL;

    extern void preloadTextures();

    void glInit() // initing SDL
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

        window = SDL_CreateWindow("Deal or no Deal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
            {
            logoutf("Unable to create window (%s)\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL)
        {
            logoutf("Unable to initialize renderer (%s)\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        preloadTextures();
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

    SDL_Texture* loadBitmapFont(SDL_Renderer* renderer, const char* file)
    {
        // Load the bitmap font image using SDL_image
        SDL_Surface* fontSurface = IMG_Load(file);
        if (!fontSurface) {
            logoutf("Error loading font image: %s", IMG_GetError());
            return nullptr;
        }

        // Convert the SDL_Surface to an SDL_Texture
        SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
        SDL_FreeSurface(fontSurface); // Free the surface, as we don't need it anymore
        if (!fontTexture) {
            logoutf("Error creating font texture: %s", SDL_GetError());
            return nullptr;
        }

        return fontTexture;
    }

    void preloadTextures()
    {
        gameLogo = loadTexture(renderer, "data/gui/logo.jpg");
        fontTexture = loadBitmapFont(renderer, "data/gui/font.png");
    }

    const int cw = 8; // char width
    const int ch = 12; // height
    const int cpr = 16; // char per rows

    void getTextSize(const string &text, int &width, int &height, int fontSize)
    {
        int charWidth = cw * fontSize;
        int charHeight = ch * fontSize;
        width = static_cast<int>(text.length()) * charWidth;
        height = charHeight;
    }

    void renderText(SDL_Renderer *renderer, SDL_Texture *fontTexture, const string &text, int x, int y, float fontSize = 2.f)
    {
        SDL_Rect srcRect = {0, 0, cw, ch};
        SDL_Rect dstRect = {x, y, static_cast<int>(cw * fontSize), static_cast<int>(ch * fontSize)};

        for (char c : text) {
            int charIndex = static_cast<int>(c) - 32;
            srcRect.x = (charIndex % cpr) * cw;
            srcRect.y = (charIndex / cpr) * ch;

            SDL_RenderCopy(renderer, fontTexture, &srcRect, &dstRect);

            dstRect.x += static_cast<int>(cw * fontSize);
        }
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
        if(!gameLogo) return;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        renderCenteredTexture(renderer, gameLogo, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    bool splashScreen = true;

    bool glLoop() // (future) renderer loop
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) return false;
            if (event.type == SDL_KEYDOWN) splashScreen = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if(splashScreen)
        {
            showSplashScreen();

            int tw, th; // text width, text height
            int textSize = 3;

            string text = "Press any key to continue";
            getTextSize(text, tw, th, textSize);

            int x = (SCREEN_WIDTH - tw) / 2;
            int y = (SCREEN_HEIGHT - th) / 1.05f;

            renderText(renderer, fontTexture, text, x, y, textSize);
        }
        else
        {

        }

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

