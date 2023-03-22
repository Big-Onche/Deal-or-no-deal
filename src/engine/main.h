#ifndef MAIN_H
#define MAIN_H

#include "tools.h"
#include "textures.h"

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
const int maxfps = 60;

extern int screenw, screenh;
extern float scalew, scaleh;

namespace sdl
{
    struct RectInfo
    {
        SDL_Rect rect;
        string text;
        string textureID;

        RectInfo(int x, int y, int w, int h, const std::string& text, const std::string& textureID)
            : rect{x, y, w, h}, text{text}, textureID{textureID} {}
    };

    // main sdl funcs
    extern void sdlInit();
    extern bool handleEvents();
    extern void render(TextureManager &textureManager);
    extern void sdlQuit();

    // other useful funcs
    extern void preloadTextures();
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


// buttons
const int MAX_BUTTONS = 32;

typedef struct Button
{
    SDL_Rect rect;
    string texture;
    const char *text;
    int fontSize;
    uint32_t color;
    bool isHover;
    void (*onClick)(void);
} Button;

extern Button *buttons[];

extern Button *createButton(int x, int y, const char *text, int fontSize, uint32_t color, void (*onClick)(void));
extern void drawButton(Button *button, SDL_Renderer *renderer);
extern void addButton(Button *button);
extern bool isMouseOverButton(const Button *button, const SDL_Point &mousePoint);
extern void freeButtons();

#endif
