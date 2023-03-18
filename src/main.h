#ifndef MAIN_H
#define MAIN_H

#include "tools.h"

using namespace std;

enum EngineState {S_Initialization = 0, S_MainMenu, S_LoadingScreen, S_InGame, S_ShuttingDown};
extern EngineState engineState;

enum GameState {S_ChoosePlayerBox = 0, S_OpeningBoxes, S_BankCall, S_AcceptedDeal, S_GameOver};
extern GameState gameState;

extern void quit(bool fatal = false);
extern void fatal(const string &message);

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

    extern string presenterDialog;

    extern box boxes[maxBoxes];
    extern playerinfo player;

    extern int openCount(bool remaining = false);
    extern bool allOpened();

    extern void handleGameEvents(SDL_Event &event, SDL_Point &mousePoint);

    extern void initGame();
}

namespace gui // menus and splash/loading screens
{
    extern void handleKeyboardEvents(SDL_Event &event);
    extern void handleMouseEvents(SDL_Event &event);
    extern void renderSplashScreen(string text);
    extern void renderMenu();
}

//sdl things
extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern int screenw, screenh;
extern float scalew, scaleh;

// bitmap font
const int cw = 8; // char width
const int ch = 12; // height
const int cpr = 16; // char per rows

namespace sdl
{
    // main sdl funcs
    extern void sdlInit();
    extern bool sdlLoop();
    extern void sdlQuit();

    // other useful funcs
    extern void preloadTextures();
    extern void renderText(const string &text, int x, int y, float fontSize = 2.f, uint32_t hexColor = 0xFFFFFF, int maxWidth = screenw/1.5f);
    extern void renderShadowedText(const string &text, int x, int y, float fontSize, uint32_t = 0xFFFFFF, uint32_t = 0x333333, int maxWidth = screenw/1.5f);
    extern void renderOutlinedText(const string &text, int x, int y, float fontSize, uint32_t = 0xFFFFFF, uint32_t = 0x333333, int maxWidth = screenw/1.5f);
    extern void getTextSize(const string &text, int &width, int &height, int fontSize, int maxWidth = screenw/1.5f);
    extern void renderCenteredTexture(SDL_Renderer *renderer, SDL_Texture *texture, int screenw, int screenh);
}

namespace render // render game
{
    extern int boxWidth, boxHeight;
    extern int boxSpacing;
    extern int boxesgridX();
    extern int boxesgridY();
    extern void renderGame();
}

namespace sound
{
    extern bool initAudio();
    extern void playSound(const char *soundName);
    extern void unInitAudio();
}

#endif
