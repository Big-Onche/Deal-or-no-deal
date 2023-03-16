#include "main.h"
#include <SDL.h>
#include <SDL_image.h>
#include <functional>

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

namespace gl
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *gameLogo = nullptr;
    SDL_Texture *fontTexture = nullptr;

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

            if (SDL_PointInRect(&mousePoint, &newGameRect))
            {
                sound::playSound("money_loss"); // just a test
            }
            else if (SDL_PointInRect(&mousePoint, &optionsRect))
            {
            }
            else if (SDL_PointInRect(&mousePoint, &exitRect))
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
        renderText(renderer, fontTexture, newGameText, x, y, textSize);
        newGameRect = {x, y, static_cast<int>(newGameText.length()) * cw * textSize, ch * textSize}; // Play

        y+=75;
        std::string optionsText = "Options";
        renderText(renderer, fontTexture, optionsText, x, y, textSize);
        optionsRect = {x, y, static_cast<int>(optionsText.length()) * cw * textSize, ch * textSize}; // Options

        y+=75;
        std::string exitText = "Exit";
        renderText(renderer, fontTexture, exitText, x, y, textSize);
        exitRect = {x, y, static_cast<int>(exitText.length()) * cw * textSize, ch * textSize}; // Exit
    }

    bool splashScreen = true;
    bool mainMenu = false;

    bool glLoop() // renderer loop
    {
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT) return false;
            if(event.type == SDL_KEYDOWN && splashScreen) {splashScreen = false; mainMenu = true;}
            handleMouseEvents(event);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if(splashScreen) // game intro splash screen
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
        else if(mainMenu) // main menu
        {
            renderMenu(renderer, fontTexture);
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

