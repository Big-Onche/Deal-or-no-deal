#include "main.h"
#include "textures.h"

int screenw = 1280;
int screenh = 720;

namespace gl
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

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
        if(window == NULL)
        {
            logoutf("Unable to create window (%s)\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(renderer == NULL)
        {
            logoutf("Unable to initialize renderer (%s)\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        preloadTextures();
    }

    bool glLoop() // renderer loop
    {
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT) return false; // quit
            gui::handleKeyboardEvents(event);
            gui::handleMouseEvents(event);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        switch(engineState)
        {
            case S_Initialization: // game intro splash screen
                gui::renderSplashScreen("Press any key to continue.");
                break;

            case S_MainMenu: // main menu
                gui::renderMenu();
                break;

            case S_LoadingScreen: // loading screen
                gui::renderSplashScreen("Loading...");
                break;

            case S_InGame: // in game
                render::renderGame();
                break;

            case S_ShuttingDown:
                break;
        }

        SDL_RenderPresent(renderer);
        return true;
    }

    SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *file) // load an image
    {
        SDL_Surface *surface = IMG_Load(file);
        if(!surface) { logoutf("Error loading image: %s", IMG_GetError()); return nullptr; }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if(!texture) { logoutf("Error creating texture: %s", SDL_GetError()); return nullptr; }

        return texture;
    }

    void preloadTextures() // preload images used in the game
    {
        TextureManager& textureManager = TextureManager::getInstance();
        if(!textureManager.load("data/gui/font.png", "MainFont", renderer)) fatal("Unable to load font texture!");
        textureManager.load("data/gui/logo.jpg", "GameLogo", renderer);
        textureManager.load("data/gui/price.png", "RemainingPrices", renderer);
        textureManager.load("data/images/box_closed.png", "ClosedBox", renderer);
        textureManager.load("data/images/box_opened.png", "OpenedBox", renderer);
        textureManager.load("data/images/presenter.png", "Presenter", renderer);
        textureManager.load("data/images/bubble.png", "Bubble", renderer);
    }

    void getTextSize(const string &text, int &width, int &height, int fontSize, int maxWidth)
    {
        int charWidth = cw * fontSize;
        int charHeight = ch * fontSize;
        int currentWidth = 0;
        int currentHeight = charHeight;
        int lineWidth = 0;

        for (char c : text)
        {
            if (c == ' ')
            {
                currentWidth += charWidth;
                if (currentWidth + charWidth > maxWidth)
                {
                    currentWidth = 0;
                    currentHeight += charHeight;
                }
            }
            else
            {
                currentWidth += charWidth;
            }

            lineWidth = std::max(lineWidth, currentWidth);
        }

        width = lineWidth;
        height = currentHeight;
    }

    void renderText(const string &text, int x, int y, float fontSize, uint32_t fontColor, int maxWidth)
    {
        TextureManager& textureManager = TextureManager::getInstance();

        SDL_Rect srcRect = {0, 0, cw, ch};
        SDL_Rect dstRect = {x, y, static_cast<int>(cw * fontSize), static_cast<int>(ch * fontSize)};

        textureManager.setColorMod("MainFont", (fontColor >> 16) & 0xFF, (fontColor >> 8) & 0xFF, fontColor & 0xFF);

        stringstream ss(text); string word;

        while (ss >> word)
        {
            int wordWidth = word.length() * static_cast<int>(cw * fontSize);
            int spaceWidth = static_cast<int>(cw * fontSize);

            if(maxWidth > 0 && (dstRect.x + wordWidth + spaceWidth) - x > maxWidth)
            {
                dstRect.x = x;
                dstRect.y += static_cast<int>(ch * fontSize);
            }

            for (char c : word)
            {
                int charIndex = static_cast<int>(c) - 32;
                srcRect.x = (charIndex % cpr) * cw;
                srcRect.y = (charIndex / cpr) * ch;

                textureManager.drawFrame("MainFont", dstRect.x, dstRect.y, cw, ch, srcRect.x, srcRect.y, srcRect.w, srcRect.h, fontSize, gl::renderer);

                dstRect.x += static_cast<int>(cw * fontSize);
            }

            if(!ss.eof()) // render a space after each word (except the last one)
            {
                int charIndex = static_cast<int>(' ') - 32;
                srcRect.x = (charIndex % cpr) * cw;
                srcRect.y = (charIndex / cpr) * ch;

                textureManager.drawFrame("MainFont", dstRect.x, dstRect.y, cw, ch, srcRect.x, srcRect.y, srcRect.w, srcRect.h, fontSize, gl::renderer);

                dstRect.x += static_cast<int>(cw * fontSize);
            }
        }
    }

    void renderShadowedText(const string &text, int x, int y, float fontSize, uint32_t textColor, uint32_t shadowColor, int maxWidth)
    {
        renderText(text, x+3, y+3, fontSize, shadowColor, maxWidth);
        renderText(text, x, y, fontSize, textColor, maxWidth);
    }

    void renderOutlinedText(const string &text, int x, int y, float fontSize, uint32_t textColor, uint32_t outlineColor, int maxWidth)
    {
        renderText(text, x+2, y, fontSize, outlineColor, maxWidth);
        renderText(text, x-2, y, fontSize, outlineColor, maxWidth);
        renderText(text, x, y+2, fontSize, outlineColor, maxWidth);
        renderText(text, x, y-2, fontSize, outlineColor, maxWidth);
        renderText(text, x, y, fontSize, textColor, maxWidth);
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

    void glQuit()
    {
        logoutf("shutdown: gl");
        TextureManager::getInstance().clearTextures();
        IMG_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}

