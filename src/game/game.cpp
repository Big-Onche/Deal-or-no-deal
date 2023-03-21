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
        engineState = S_LoadingScreen;
        freeButtons();
        initDealButtons();
        player.bankGain = 0;
        player.playerBox = 0;
        assignBoxes();
        gameState = S_ChoosePlayerBox;
        engineState = S_InGame;

        popDialog(getRandomDialogue(DialogueType::Intro).c_str());
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

    void chooseBox(int id) // when player is prompt to choose his box
    {
        popDialog(getRandomDialogue(DialogueType::ChooseBox).c_str(), id + 1);
        player.playerBox = id;
        gameState = S_OpeningBoxes;
    }

    int boxCombo[3]; // store streaks of positive, neutral or negative of box opening

    void openBox(int id)
    {
        SoundManager& SoundManager = SoundManager::getInstance();

        if(boxes[id].opened || id==player.playerBox) return; // stop if already opened or player's box
        else if(allOpened()) // check for game over
        {
            gameState = S_GameOver;
            SoundManager.playMusic("data/songs/jingle.ogg");
            return;
        }

        boxes[id].opened = true;
        int boxValue = boxes[id].insideBox;
        SoundManager.play("BoxOpen");
        render::checkGameAtmo();

        if(openCount()<=3) // early game
        {
            if(boxValue >= 50000)
            {
                SoundManager.play("BoxNegative");
                boxCombo[A_bad]++;
                popDialog(getRandomDialogue(DialogueType::EarlyGameLoss).c_str(), boxValue);
            }
            else if(boxValue >= 5000)
            {
                SoundManager.play("BoxPositive");
                boxCombo[A_neutral]++;
                popDialog(getRandomDialogue(DialogueType::EarlyGameMid).c_str(), boxValue);
            }
            else
            {
                SoundManager.play("BoxPositive");
                boxCombo[A_good]++;
                popDialog(getRandomDialogue(DialogueType::EarlyGameWin).c_str(), boxValue);
            }
        }
        else if(openCount()>=4 && openCount()<=14) // mid game
        {
            if(boxValue >= 20000)
            {
                SoundManager.play("BoxNegative");
                boxCombo[A_bad]++;
                popDialog(getRandomDialogue(DialogueType::MidGameLoss).c_str(), boxValue);
            }
            else if(boxValue >= 2000)
            {
                SoundManager.play("BoxPositive");
                boxCombo[A_neutral]++;
                popDialog(getRandomDialogue(DialogueType::MidGameMid).c_str(), boxValue);
            }
            else
            {
                SoundManager.play("BoxPositive");
                boxCombo[A_good]++;
                popDialog(getRandomDialogue(DialogueType::MidGameWin).c_str(), boxValue);
            }
        }
        else // end game
        {
            if(boxValue >= 5000)
            {
                SoundManager.play("BoxNegative");
                boxCombo[A_bad]++;
                popDialog(getRandomDialogue(DialogueType::EndGameLoss).c_str(), boxValue);
            }
            else if(boxValue >= 1000)
            {
                SoundManager.play("BoxPositive");
                boxCombo[A_neutral]++;
                popDialog(getRandomDialogue(DialogueType::EndGameMid).c_str(), boxValue);
            }
            else
            {
                SoundManager.play("BoxPositive");
                boxCombo[A_good]++;
                popDialog(getRandomDialogue(DialogueType::EndGameWin).c_str(), boxValue);
            }
        }

        if(openCount()==5 || openCount()==9 || openCount()==12 || openCount()==15 || openCount()==18)
        {
            SoundManager.play("BankCall");
            loopi(3) boxCombo[i]=0;
            gameAtmo = A_neutral;
            gameState = (player.bankGain ? S_BankOffer : S_BankCall);
        }
    }

    void handleGame(SDL_Event &event, SDL_Point &mousePoint)
    {
        if(gameState==S_Dealing) // accept/refuse or continue buttons handling
        {
            loopi(numDealButtons)
            {
                Button *button = buttons[i];
                if(button != nullptr)
                {
                    bool isMouseOver = isMouseOverButton(button, mousePoint);
                    button->isHover = isMouseOver;

                    if(event.type == SDL_MOUSEBUTTONDOWN && isMouseOver)
                    {
                        if(button->onClick != nullptr) button->onClick();
                    }
                }
            }
        }
        else if ((gameState==S_OpeningBoxes || gameState==S_ChoosePlayerBox) && event.type == SDL_MOUSEBUTTONDOWN) // handling clicks on boxes
        {
            loopi(4)
            {
                loopj(5)
                {
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
        }

        if(event.type == SDL_KEYDOWN && event.key.keysym.sym==SDLK_SPACE)
        {
            switch(gameState)
            {
                case S_BankCall:
                    popDialog(getRandomDialogue(DialogueType::BankerCall).c_str());
                    SoundManager::getInstance().playMusic("data/songs/bank.ogg", -1);
                    gameState=S_BankOffer;
                    break;

                case S_BankOffer:
                    if(player.bankGain) popDialog("At this point of the game, the banker would buy your box number %d for $%d", player.playerBox+1, bankOffer(rnd(10)/10.f));
                    else popDialog("He want to buy your box number %d for $%d", player.playerBox+1, bankOffer(rnd(10)/10.f));
                    gameState=S_Dealing;
                    break;

                case S_Dealing:
                    if(player.bankGain && player.bankGain>lastOffer) popDialog("Well done, you have accepted $%d just before!", player.bankGain);
                    else if(player.bankGain && player.bankGain<lastOffer) popDialog("You accepted $%d before! Maybe you accepted a bit too fast?", player.bankGain);
                    else popDialog("So, you take the bucks or you risk the bust?");
                    break;

                case S_AcceptedDeal:
                    popDialog("Well done! You just won $%d. Let's see if you've made a good choice.", player.bankGain);
                    gameState=S_OpeningBoxes;
                    break;

                case S_GameOver:
                    popDialog("Well done! You just won $%d!", (player.bankGain ? player.bankGain : game::boxes[game::player.playerBox].insideBox));
                    break;
            }
        }
    }



}
