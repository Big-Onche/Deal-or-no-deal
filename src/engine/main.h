#ifndef MAIN_H
#define MAIN_H

#include "tools.h"

using namespace std;

// time
extern Uint32 elapsedTime, seconds, milliseconds;

// engine states
enum EngineState {S_Initialization = 0, S_MainMenu, S_LoadingScreen, S_InGame, S_ShuttingDown};
extern EngineState engineState;

// useful global things
extern void quit(bool fatal = false);
extern void fatal(const string &message);
extern void logoutf(const char* format, ...);

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
