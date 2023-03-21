#ifndef GAME_H
#define GAME_H

#include "tools.h"
#include "dialogs.h"
#include <map>

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
    extern string mainDialog;
    enum class DialogueType {
        Invalid = -1,
        Intro,
        ChooseBox,
        EarlyGameWin,
        EarlyGameMid,
        EarlyGameLoss,
        MidGameWin,
        MidGameMid,
        MidGameLoss,
        EndGameWin,
        EndGameMid,
        EndGameLoss,
        BankerCall,
        DealRefused
    };
    extern map<DialogueType, vector<string>> dialogues;
    extern void loadDialogs();
    const string &getRandomDialogue(DialogueType type);
    extern void popDialog(const char* format, ...);
    //buttons and game handling
    //extern Button *dealButton;
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
