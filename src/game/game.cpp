#include "main.h"
#include "game.h"
#include "sounds.h"

GameState gameState;

namespace game
{
    //const int maxBoxes = 16;
    const int boxValues[maxBoxes] = {0, 1, 10, 30, 69, 100, 420, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 250000, 500000};

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
        switch(rnd(3))
        {
            case 0: popDialog("Please choose your box!"); break;
            case 1: popDialog("It's time to choose your box wisely! But remember: It's all about chance."); break;
            case 2: popDialog("So, what's your box today? Pick the one that smells like bucks!");
        }
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
        player.playerBox = id;
        gameState = S_OpeningBoxes;
        switch(rnd(3))
        {
            case 0: popDialog("You choosed the box %d, I hope that it's a good number for you today!", id+1); break;
            case 1: popDialog("Box %d? I don't trust this number. But hey: maybe it's a lucky one for you!", id+1); break;
            case 2: popDialog("Let's go with box number %d. I think this is the one too! Or maybe not.", id+1);
        }
    }

    void openBox(int id)
    {
        SoundManager& SoundManager = SoundManager::getInstance();

        if(game::boxes[id].opened || id==player.playerBox) return;

        game::boxes[id].opened = true;
        int boxValue = game::boxes[id].insideBox;

        bool soundTrigger = (openCount() < 10 && boxValue>=50000) ? true :
                openCount() > 10 && boxValue>=10000 ? true : false;

        soundTrigger ? SoundManager.play("MoneyLoss") : SoundManager.play("BoxOpen");

        switch(game::openCount())
        {
            case 1:
                if(boxValue >= 50000) popDialog("What a bad start! $%d, in the first box. Let's try to forget that.", boxValue);
                else popDialog("Only $%d, nice! Let's continue like this.", boxValue);
                break;

            default:
                if(boxValue >= 50000)  popDialog("Holy shit! $%d. Maybe it was the last big one.", boxValue);
                else popDialog("Nice, only $%d! You're doing well, please choose another box.", boxValue);
                break;
        }
    }

    void handleGameEvents(SDL_Event &event, SDL_Point &mousePoint)
    {
        SDL_RenderGetScale(renderer, &scalew, &scaleh);

        switch(gameState)
        {
            case S_OpeningBoxes: case S_ChoosePlayerBox: // handling clicks on boxes
                loopi(4) {
                    loopj(4) {
                        int id = i * 4 + j;
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
                break;

            case S_AcceptedDeal:
                break;

            case S_GameOver:
                break;
        }
    }

    void bankCall()
    {
        int bankOffer = 0;

        //SoundManager.playSound("bank_offer");

        loopi(maxBoxes) if(!boxes[i].opened) bankOffer+=boxes[i].insideBox;
        if(openCount(true)) bankOffer/= (openCount(true)<3 ? (float)openCount(true)*1.3 : (openCount(true)*3) + rnd(2)) ;

        if(player.bankGain)
        {
            printf("At this time, the bank would make another offer: %d$, %s", bankOffer, player.bankGain>=bankOffer ? "well done!\n\n" : "bad luck!\n\n");
        }
        else
        {
            bool exchangeOffer = !rnd(3);
            printf("The bank has an offer for you: %s%s Deal or no deal? (Y/N)\n", exchangeOffer ? "Box exchange," : to_string(bankOffer).c_str(), exchangeOffer? "" : "$");

            string response;
            while(response!="Y" || response!="N")
            {
                getline(cin, response);
                if(response=="Y")
                {
                    if(exchangeOffer)
                    {
                        printf("Please enter the box you want:\n");
                        //playerInput(&player.playerBox);
                        //clearConsole();
                        //render::drawBoxes(player, boxes);
                        //if(!allOpened()) render::drawRemainingPrices(player, boxes);
                        printf("Your box is now the number %d!:\n", player.playerBox);
                    }
                    else
                    {
                        player.bankGain = bankOffer;
                        //sound::playSound("victory");
                        printf("You selled your box for %d$\n\n", player.bankGain);
                    }
                    break;
                }
                else if(response=="N")
                {
                    printf("You declined bank's offer.\n\n");
                        break;
                }
            }
        }
    }

    void playGame() // run a game
    {
        while(player.playerBox < 1 || player.playerBox > maxBoxes)
        {
            printf("Please select a box from 1 to %d:\n", maxBoxes);
            //playerInput(&player.playerBox);
        }
        //clearConsole();
        bool nextStep = true;

        for(;;)
        {
            if(nextStep)
            {
                //render::drawBoxes(player, boxes);
                //if(!allOpened()) render::drawRemainingPrices(player, boxes);
            }

            if(openCount() && nextStep) printf("There was %d$ in the %d box!\n", boxes[player.choosenBox-1].insideBox, player.choosenBox);

            if(allOpened())
            {
                if(player.bankGain) printf("You won %d$ and there was %d$ in your box, %s", player.bankGain, boxes[player.playerBox-1].insideBox, player.bankGain>=boxes[player.playerBox-1].insideBox ? "well done!\n\n" : "bad luck!\n\n");
                else printf("You won %d$ with your box %d.\n\n", boxes[player.playerBox-1].insideBox, player.playerBox);
                break;
            }

            if(nextStep) switch(openCount()) { case 6: case 10: case 14: bankCall(); }

            printf("Please choose a box to open:\n");
            //playerInput(&player.choosenBox);

            if(player.choosenBox==player.playerBox)
            {
                printf("That's your box, please choose another one.\n");
                nextStep = false;
                continue;
            }
            else if(player.choosenBox<1 || player.choosenBox>maxBoxes)
            {
                player.choosenBox=0;
                printf("Invalid box, please choose between 1 and %d.\n", maxBoxes);
                nextStep = false;
                continue;
            }
            else if(boxes[player.choosenBox-1].opened)
            {
                printf("That box is already opened, please choose another one.\n");
                nextStep = false;
                continue;
            }
            else
            {
                boxes[player.choosenBox-1].opened = true;
                //clearConsole();

                if(allOpened()) boxes[player.playerBox-1].opened = true;

                nextStep = true;
            }
        }
    }
}
