#ifndef GAME_H
#define GAME_H

#include "tools.h"

using namespace std;

enum GameState {S_ChoosePlayerBox = 0, S_OpeningBoxes, S_BankCall, S_BankOffer, S_Dealing, S_AcceptedDeal, S_GameOver};
extern GameState gameState;

enum GameAtmo {A_good, A_neutral, A_bad};
extern GameAtmo gameAtmo;

namespace game
{
    const int maxBoxes = 20;

    struct box {
        int insideBox;
        bool opened;
    };
    extern box boxes[maxBoxes];

    struct playerinfo {
        int playerBox;
        int choosenBox;
        int bankGain;
    };
    extern playerinfo player;

    extern void initGame();
    extern int openCount(bool remaining = false);
    extern bool allOpened();
    extern int bankOffer();

    extern void loadDialogs();
    extern string mainDialog;
    extern vector<string> introDialog;
    extern vector<string> chooseBoxDialog;
    extern vector<string> earlyGameWinDialog;
    extern vector<string> earlyGameMidDialog;
    extern vector<string> earlyGameLossDialog;
    extern vector<string> midGameWinDialog;
    extern vector<string> midGameMidDialog;
    extern vector<string> midGameLossDialog;
    extern vector<string> endGameWinDialog;
    extern vector<string> endGameMidDialog;
    extern vector<string> endGameLossDialog;
    extern vector<string> bankerCall;

    extern void handleGame(SDL_Event &event, SDL_Point &mousePoint);
}

namespace gui // menus and splash/loading screens
{
    extern void handleMenus(SDL_Event &event, SDL_Point &mousePoint);
    extern void renderSplashScreen(string text);
    extern void renderMenu();
}

namespace render // render game
{
    extern int boxWidth, boxHeight;
    extern int boxSpacing;
    extern int boxesgridX();
    extern int boxesgridY();
    extern void renderGame();

    extern SDL_Rect yesRect, noRect;
}

#endif
