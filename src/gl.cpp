#include "main.h"

using namespace std;

int screenw = 1280;
int screenh = 720;
bool fullscreen = false;

namespace gl
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    //time to use an array or some shit like that
    SDL_Texture *gameLogo = nullptr;
    SDL_Texture *fontTexture = nullptr;

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

        window = SDL_CreateWindow("Deal or no Deal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenw, screenh, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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
        if(!surface) { logoutf("Error loading image: %s\n", IMG_GetError()); return nullptr; }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if(!texture) { logoutf("Error creating texture: %s\n", SDL_GetError()); return nullptr; }

        return texture;
    }

    void preloadTextures() // preload images used in the game
    {
        gameLogo = loadTexture(renderer, "data/gui/logo.jpg");
        fontTexture = loadTexture(renderer, "data/gui/font.png");
    }

    void freeTextures() // free images before stopping gl
    {
        if(gameLogo != nullptr) { SDL_DestroyTexture(gameLogo);gameLogo = nullptr; }
        if(fontTexture != nullptr) { SDL_DestroyTexture(fontTexture); fontTexture = nullptr; }
    }

    void getTextSize(const string &text, int &width, int &height, int fontSize)
    {
        int charWidth = cw * fontSize;
        int charHeight = ch * fontSize;
        width = static_cast<int>(text.length()) * charWidth;
        height = charHeight;
    }

    void renderText(SDL_Renderer *renderer, SDL_Texture *fontTexture, const string &text, int x, int y, float fontSize)
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

    bool glLoop() // renderer loop
    {
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT) return false; // quit
            if(event.type == SDL_KEYDOWN && engineStatus==S_SplashScreen) { engineStatus=S_MainMenu; } // exit splash screen
            else if (event.key.keysym.sym == SDLK_F11 && event.type == SDL_KEYDOWN) // toggle fullscreen
            {
                fullscreen = !fullscreen;
                if(fullscreen) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                else SDL_SetWindowFullscreen(window, 0);
                SDL_GetWindowSize(window, &screenw, &screenh);
            }
            if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) // window resize
            {
                screenw = event.window.data1; screenh = event.window.data2;
            }
            gui::handleMouseEvents(event);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        switch(engineStatus)
        {
            case S_SplashScreen: // game intro splash screen
                gui::showSplashScreen("Press any key to continue.");
                break;

            case S_MainMenu: // main menu
                gui::renderMenu();
                break;

            case S_LoadingScreen: case S_InGame: // loading screen
                gui::showSplashScreen("Loading...");
                break;
        }

        SDL_RenderPresent(renderer);
        return true;
    }

    void glQuit()
    {
        logoutf("shutdown: gl");
        freeTextures();
        IMG_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}

