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

    struct box { // store boxes states
        int insideBox;
        bool opened;
    };
    extern box boxes[maxBoxes];

    struct playerinfo { // store player states
        int playerBox;
        int choosenBox;
        int bankGain;
    };
    extern playerinfo player;

    // main game
    extern void initGame();
    extern int openCount(bool remaining = false);
    extern bool allOpened();
    extern int boxCombo[3];
    //bank
    extern int lastOffer;
    extern int bankOffer(float offerMod);
    //dialogs
    extern void popDialog(const char* format, ...);
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
    extern vector<string> dealRefused;

    extern Button *dealButton;
    enum {Btn_Accept, Btn_Refuse, Btn_Continue, numDealButtons};
    extern void initDealButtons();
    extern void handleGame(SDL_Event &event, SDL_Point &mousePoint);
}

namespace menus // menus and splash/loading screens
{
    extern void initMainMenuButtons();
    extern void renderMenu();
    extern void renderSplashScreen(string text);
    extern void handleMenus(SDL_Event &event, SDL_Point &mousePoint);
}

namespace render // render game
{
    extern int boxWidth, boxHeight;
    extern int boxSpacing;
    extern int boxesgridX();
    extern int boxesgridY();
    extern void renderGame();
    extern void checkGameAtmo();
}

#endif
