#include "main.h"
#include "game.h"
#include "sounds.h"

namespace game
{
    int lastOffer;

    int bankOffer(float offerMod)
    {
        int offer = 0;
        loopi(maxBoxes) if(!boxes[i].opened) offer+=boxes[i].insideBox;
        if(openCount(true)) offer /= (openCount(true) < 3 ? (float)openCount(true)*(0.7f+offerMod) : (openCount(true)*(1.7f+(offerMod/2.f))));
        lastOffer = offer;
        return offer;
    }

    void acceptDeal()
    {
        popDialog("Maybe the banker trapped you, but let's see what's inside the other boxes.");
        if(!player.bankGain)
        {
            player.bankGain = lastOffer;
            gameState=S_AcceptedDeal;
        }
        else gameState=S_OpeningBoxes;
    }

    void refuseDeal()
    {
        popDialog(getRandomDialogue(DialogueType::DealRefused).c_str());
        Mix_FadeOutMusic(1000);
        gameState=S_OpeningBoxes;
    }

    void continueGame()
    {
        //popDialog(dealRefused[rnd(dealRefused.size())].c_str());
        if(gameState==S_BankCall)
        {
            gameState=S_BankOffer;
        }
        else if (gameState==S_Dealing)
        {
            Mix_FadeOutMusic(1000);
            gameState=S_OpeningBoxes;
        }
    }


    Button *dealButton;

    void initDealButtons() // called when initializing the engine
    {
        int y = 600;

        dealButton = createButton(810, y, "Accept", 4, 0x22CC00, acceptDeal);
        addButton(dealButton);

        y += 55;

        dealButton = createButton(810, y, "Refuse", 4, 0xFF3300, refuseDeal);
        addButton(dealButton);

        dealButton = createButton(810, y, "Continue", 4, 0xFFAA00, continueGame);
        addButton(dealButton);
    }
}
