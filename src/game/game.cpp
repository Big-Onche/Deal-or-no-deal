#include "main.h"
#include "game.h"
#include "sounds.h"

GameState gameState;
GameAtmo gameAtmo;

namespace game
{
    const int boxValues[maxBoxes] = {0, 1, 5, 10, 30, 69, 100, 420, 1000, 2000, 5000, 7500, 10000, 15000, 20000, 50000, 75000, 100000, 250000, 500000};

    box boxes[maxBoxes];
    struct playerinfo player;

    string mainDialog;
    void popDialog(const char* format, ...)
    {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, 256, format, args);
        va_end(args);

        mainDialog = buffer;

        lastDialogTime = elapsedTime;
    }

    void assignBoxes() // random distribution of boxes
    {
        random_device rd;
        mt19937 gen(rd());

        vector<int> tempBoxValues(boxValues, boxValues + maxBoxes);
        shuffle(tempBoxValues.begin(), tempBoxValues.end(), gen);

        loopi(maxBoxes)
        {
            boxes[i].opened = false;
            boxes[i].insideBox = tempBoxValues[i]; // assign the shuffled value directly to the boxes array
        }
    }

    void initGame() // (re)initialize everything for a new game
    {
        player.bankGain = 0;
        player.playerBox = 0;
        assignBoxes();
        gameState = S_ChoosePlayerBox;
        engineState = S_InGame;

        popDialog(introDialog[rnd(introDialog.size())].c_str());
    }

    int openCount(bool remaining) // count the number of opened boxes (or the number of remaining)
    {
        int num = 0;
        loopi(maxBoxes) if(boxes[i].opened) num++;
        return remaining ? maxBoxes-num : num;
    }

    bool allOpened() // check if all boxes are opened
    {
        return openCount()>=maxBoxes-1; //-1 because of player box not opened
    }

    void chooseBox(int id)
    {
        popDialog(chooseBoxDialog[rnd(chooseBoxDialog.size())].c_str(), id + 1);
        player.playerBox = id;
        gameState = S_OpeningBoxes;
    }

    int boxCombo[3];

    void checkGameAtmo()
    {
        size_t maxIndex = std::max_element(boxCombo, boxCombo + 3) - boxCombo;

        switch (maxIndex) { // Set gameAtmo based on the index
            case A_neutral: gameAtmo = A_neutral; break;
            case A_bad: gameAtmo = A_bad; break;
            case A_good: gameAtmo = A_good; break;
        }
    }

    void openBox(int id)
    {
        SoundManager& SoundManager = SoundManager::getInstance();

        if(boxes[id].opened || id==player.playerBox) return;

        boxes[id].opened = true;
        int boxValue = boxes[id].insideBox;

        if(openCount()<=3)
        {
            if(boxValue >= 50000)
            {
                SoundManager.play("MoneyLoss");
                boxCombo[A_bad]++;
                popDialog(earlyGameLossDialog[rnd(earlyGameLossDialog.size())].c_str(), boxValue);
            }
            else if(boxValue >= 5000)
            {
                SoundManager.play("BoxOpen");
                boxCombo[A_neutral]++;
                popDialog(earlyGameMidDialog[rnd(earlyGameMidDialog.size())].c_str(), boxValue);
            }
            else
            {
                SoundManager.play("BoxOpen");
                boxCombo[A_good]++;
                popDialog(earlyGameWinDialog[rnd(earlyGameWinDialog.size())].c_str(), boxValue);
            }
        }
        else if(openCount()>=4 && openCount()<=14)
        {
            if(boxValue >= 20000)
            {
                SoundManager.play("MoneyLoss");
                boxCombo[A_bad]++;
                popDialog(midGameLossDialog[rnd(midGameLossDialog.size())].c_str(), boxValue);
            }
            else if(boxValue >= 2000)
            {
                SoundManager.play("BoxOpen");
                boxCombo[A_neutral]++;
                popDialog(midGameMidDialog[rnd(midGameMidDialog.size())].c_str(), boxValue);
            }
            else
            {
                SoundManager.play("BoxOpen");
                boxCombo[A_good]++;
                popDialog(midGameWinDialog[rnd(midGameWinDialog.size())].c_str(), boxValue);
            }
        }
        else
        {
            if(boxValue >= 5000)
            {
                SoundManager.play("MoneyLoss");
                boxCombo[A_bad]++;
                popDialog(endGameLossDialog[rnd(endGameLossDialog.size())].c_str(), boxValue);
            }
            else if(boxValue >= 1000)
            {
                SoundManager.play("BoxOpen");
                boxCombo[A_neutral]++;
                popDialog(endGameMidDialog[rnd(endGameMidDialog.size())].c_str(), boxValue);
            }
            else
            {
                SoundManager.play("BoxOpen");
                boxCombo[A_good]++;
                popDialog(endGameWinDialog[rnd(endGameWinDialog.size())].c_str(), boxValue);
            }
        }

        if(openCount()==5) SoundManager.playMusic("data/songs/mid_game.ogg");

        if(openCount()==5 || openCount()==9 || openCount()==12 || openCount()==15 || openCount()==18)
        {
            SoundManager.play("BankCall");
            loopi(3) boxCombo[i]=0;
            gameAtmo = A_neutral;
            gameState = S_BankCall;
        }
        else if(allOpened())
        {
            gameState = S_GameOver;
            SoundManager.playMusic("data/songs/jingle.ogg");
        }
    }

    int lastOffer;
    int bankOffer(float offerMod)
    {
        int offer = 0;
        loopi(maxBoxes) if(!boxes[i].opened) offer+=boxes[i].insideBox;
        if(openCount(true)) offer /= (openCount(true) < 3 ? (float)openCount(true)*(0.7f+offerMod) : (openCount(true)*(1.5f+offerMod)));
        lastOffer = offer;
        return offer;
    }

    void handleGame(SDL_Event &event, SDL_Point &mousePoint)
    {
        if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            switch(gameState)
            {
                case S_OpeningBoxes: case S_ChoosePlayerBox: // handling clicks on boxes
                    loopi(4) {
                        loopj(5) {
                            int id = i * 5 + j;
                            // set the click zone at the same sizes of the box displayed in drawBox() with the same loop
                            SDL_Rect boxRect = {render::boxesgridX() + j * (render::boxWidth + render::boxSpacing),
                                                render::boxesgridY() + i * (render::boxHeight + render::boxSpacing),
                                                render::boxWidth, render::boxHeight};

                            if(SDL_PointInRect(&mousePoint, &boxRect))
                            {
                                if(gameState==S_ChoosePlayerBox) chooseBox(id); // just choose a box (early game or bank exchange)
                                else openBox(id); // else we open a box when clicking on it
                            }
                        }
                    }
                    break;

                case S_BankCall:
                    popDialog(bankerCall[rnd(bankerCall.size())].c_str());
                    gameState=S_BankOffer;
                    break;

                case S_BankOffer:
                    popDialog("He want to buy your box number %d for $%d", player.playerBox+1, bankOffer(rnd(10)/10.f));
                    gameState=S_Dealing;
                    break;

                case S_Dealing:
                    popDialog("So, you take the bucks or you risk the bust?");
                    if(SDL_PointInRect(&mousePoint, &render::yesRect))
                    {
                        popDialog("Maybe the banker trapped you, but let's see what's inside the other boxes.");
                        player.bankGain = bankOffer(rnd(10)/10.f);
                        gameState=S_AcceptedDeal;
                    }
                    if(SDL_PointInRect(&mousePoint, &render::noRect))
                    {
                        popDialog("It's risky: I like that! Let's continue.");
                        gameState=S_OpeningBoxes;
                    }
                    break;

                case S_AcceptedDeal:
                    popDialog("Well done! You just won $%d. Let's see if you've made a good choice.", player.bankGain);
                    break;

                case S_GameOver:
                    popDialog("Well done! You just won $%d!", (player.bankGain ? player.bankGain : game::boxes[game::player.playerBox].insideBox));
                    break;
            }
        }
    }
}
