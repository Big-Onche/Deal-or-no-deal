#ifndef MAIN_H
#define MAIN_H

#include "tools.h"

using namespace std;

extern int screenw, screenh;

extern int engineStatus;
enum {S_Initialization = 0, S_SplashScreen, S_MainMenu, S_LoadingScreen, S_InGame, S_ShuttingDown};

extern void quit();

// console
enum ConsoleColors
{
    C_DEF = 0,
    C_BLACK = 30,
    C_RED, //31, etc.
    C_GREEN,
    C_YELLOW,
    C_BLUE,
    C_MAGENTA,
    C_CYAN,
    C_WHITE
};

extern void playerInput(int *value);
extern void logoutf(const char* format, ...);
extern void conoutf(ConsoleColors background, ConsoleColors text, const char* format, ...);
extern void clearConsole();

namespace game
{
    const int maxBoxes = 16;

    struct box {
        int insideBox;
        bool opened;
    };

    struct playerinfo {
        int playerBox;
        int choosenBox;
        int bankGain;
    };

    extern box boxes[maxBoxes];
    extern playerinfo player;

    extern int openCount(bool remaining = false);
    extern bool allOpened();

    extern void initGame();
    extern void playGame();
}

namespace gui
{
    extern void handleMouseEvents(SDL_Event &event);
    extern void showSplashScreen(string text);
    extern void renderMenu();
}

namespace gl
{
    extern SDL_Window *window;
    extern SDL_Renderer *renderer;
    //time to use an array or some shit like that
    extern SDL_Texture *gameLogo;
    extern SDL_Texture *fontTexture;

    // bitmap font
    const int cw = 8; // char width
    const int ch = 12; // height
    const int cpr = 16; // char per rows

    // main gl funcs
    extern void glInit();
    extern bool glLoop();
    extern void glQuit();

    // other useful funcs
    extern void preloadTextures();
    extern void renderText(SDL_Renderer *renderer, SDL_Texture *fontTexture, const string &text, int x, int y, float fontSize = 2.f);
    extern void getTextSize(const string &text, int &width, int &height, int fontSize);
    extern void renderCenteredTexture(SDL_Renderer *renderer, SDL_Texture *texture, int screenw, int screenh);
}

namespace render
{
    extern void drawProgressBar(float progress);
    extern void drawBoxes(const game::playerinfo &player, game::box boxes[]);
    extern void drawRemainingPrices(const game::playerinfo &player, game::box boxes[]);
}

namespace sound
{
    extern bool initAudio();
    extern void playSound(const char *soundName);
    extern void unInitAudio();
}

#endif
