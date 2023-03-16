#ifndef MAIN_H
#define MAIN_H

#include "tools.h"

using namespace std;

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

namespace gl
{
    extern void glInit();
    extern bool glLoop();
    extern void glQuit();
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
