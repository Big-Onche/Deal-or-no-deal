#ifndef MAIN_H
#define MAIN_H

#include "tools.h"

using namespace std;

extern void quit();
extern void clearConsole();

namespace game
{
    extern void initGame();
    extern void runGame();
    extern void playGame();
}

namespace sound
{
    extern bool initAudio();
    extern void playSound(const char *soundName);
    extern void unInitAudio();
}

#endif
