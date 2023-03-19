#ifndef GAME_H
#define GAME_H

#include "tools.h"

using namespace std;

enum GameState {S_ChoosePlayerBox = 0, S_OpeningBoxes, S_BankCall, S_AcceptedDeal, S_GameOver};
extern GameState gameState;

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

#endif
