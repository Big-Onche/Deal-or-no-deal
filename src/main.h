#ifndef MAIN_H
#define MAIN_H

#include "tools.h"

using namespace std;

extern Uint32 elapsedTime, seconds, milliseconds;

enum EngineState {S_Initialization = 0, S_MainMenu, S_LoadingScreen, S_InGame, S_ShuttingDown};
extern EngineState engineState;

enum GameState {S_ChoosePlayerBox = 0, S_OpeningBoxes, S_BankCall, S_AcceptedDeal, S_GameOver};
extern GameState gameState;

extern void quit(bool fatal = false);
extern void fatal(const string &message);

extern void logoutf(const char* format, ...);

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

    extern string mainDialog;
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

namespace sdl
{
    // main sdl funcs
    extern void sdlInit();
    extern bool sdlLoop();
    extern void sdlQuit();

    // other useful funcs
    extern void preloadTextures();
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

// bitmap font and text rendring
extern Uint32 lastDialogTime;

enum {MainFont = 0, DialFont, MaxFonts};
extern int font[MaxFonts];
extern int cw[MaxFonts];
extern int ch[MaxFonts];
extern int cpr[MaxFonts];

extern void renderText(int font, const string &text, int x, int y, float fontSize = 2.f, uint32_t hexColor = 0xFFFFFF, int maxWidth = screenw/1.5f);
extern void renderShadowedText(int font, const string &text, int x, int y, float fontSize, uint32_t = 0xFFFFFF, uint32_t = 0x333333, int maxWidth = screenw/1.5f);
extern void renderOutlinedText(int font, const string &text, int x, int y, float fontSize, uint32_t = 0xFFFFFF, uint32_t = 0x333333, int maxWidth = screenw/1.5f);
extern void getTextSize(int font, const string &text, int &width, int &height, int fontSize, int maxWidth = screenw/1.5f);

#endif
