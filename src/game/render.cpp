#include "main.h"
#include "game.h"
#include "particles.h"

namespace render
{
    int boxWidth = 140, boxHeight = 125;
    int boxSpacingX = 20, boxSpacingY = 12;

    int boxesgridX() { return (screenw - (5 * boxWidth + 3 * boxSpacingX)) / 2; }
    int boxesgridY() { return ((4 * boxHeight + 3 * boxSpacingY)) - screenh / 1.37f; }

    void drawBox(TextureManager& textureManager, int id, int x, int y, int boxvalue, bool opened, bool ownBox = false) // draw one box
    {
        int bwidth = boxWidth, bheight = boxHeight, shadowOffset = 4, textsize = 2;

        if(ownBox) { bwidth *= 1.5f, bheight *= 1.5f, shadowOffset = 10, textsize= 3; }

        textureManager.drawShadowedTex(opened ? "OpenedBox" : "ClosedBox", x, y, bwidth, bheight, renderer, 0xFFFFFF, 0x000000, shadowOffset, shadowOffset, 75);

        if(opened)
        {
            string text = "$" + to_string(boxvalue);
            int tw, th;
            getTextSize(font[MainFont], text, tw, th, ownBox ? 3 : 2);
            int textX = (x + (bwidth - tw) / 2) + 8, textY = y + (bheight - th) / 5;

            renderText(font[MainFont], text, textX, textY, textsize, 0xFFFFFF);
        }
        renderText(font[MainFont], to_string(id + 1), x + bwidth / 2, y + bheight / 1.5f, textsize, opened ? 0x999999 : 0xFFFFFF);
    }

    void drawBoxes(TextureManager& textureManager) // draw all boxes in a grid
    {
        loopi(4)
        {
            loopj(5)
            {
                int id = i * 5 + j;
                int x = boxesgridX() + j * (boxWidth + boxSpacingX);
                int y = boxesgridY() + i * (boxHeight + boxSpacingY);
                if(gameState==S_ChoosePlayerBox || id!=game::player.playerBox) drawBox(textureManager, id, x, y, game::boxes[id].insideBox, game::boxes[id].opened);
            }
        }

        if(gameState>S_ChoosePlayerBox)
        {
            int id = game::player.playerBox;
            drawBox(textureManager, id, (screenw-boxWidth*1.5f)-20, 10+(screenh-boxWidth*1.5f), game::boxes[id].insideBox, game::allOpened(), true);
        }
    }

    uint32_t backgroundColor(int value)
    {
        return value==69 ? 0xCC33CC : value==420 ? 0x00CC00 : value < 5000 ? 0x3333FF : value < 50000 ? 0xCCCC33 : 0xFF3333;
    }

    void drawRemainingPrices(TextureManager& textureManager)
    {
        int textSize = 3;
        int values[game::maxBoxes], numValues = 0;

        loopi(game::maxBoxes) if (!game::boxes[i].opened) values[numValues++] = game::boxes[i].insideBox;

        sort(values, values + numValues);

        int splitIndex = numValues/2;
        int lineHeight = static_cast<int>(ch[MainFont] * textSize) + 11;

        loopi(numValues)
        {
            int val = values[i];
            string text = "$" + to_string(val);
            int x, y, tw, th;

            getTextSize(font[MainFont], text, tw, th, textSize);

            if (i < splitIndex) { x = 10; y = 10 + i * lineHeight; }
            else { x = screenw - tw; y = 10 + (i - splitIndex) * lineHeight;}

            textureManager.setColorMod("RemainingPrices", backgroundColor(values[i]));
            textureManager.draw("RemainingPrices", x - 4, y - 6, tw + 8, th + 8, renderer);

            renderOutlinedText(font[MainFont], text, x, y, textSize, 0xFFFFFF, 0x333333);
        }
    }

    void drawDialogs(TextureManager& textureManager)
    {
        textureManager.drawShadowedTex("Presenter", -30, screenh-280, 252, 373, renderer, 0xFFFFFF, 0x000000, 15, 15, 75);
        textureManager.draw("Bubble", 135, screenh-170, 652, 163, renderer);

        renderShadowedText(font[DialFont], game::mainDialog, 217, screenh-137, 3, 0x000000, 0xCCCCCC, 550);
    }

    void drawBank(TextureManager& textureManager)
    {
        textureManager.drawShadowedTex("Banker", (screenw - 500) / 2, (screenh - 500) / 3, 500, 500, renderer, 0xFFFFFF, 0x000000, 15, 15, 75);

        if(gameState==S_Dealing)
        {
            if(game::player.bankGain) drawButton(buttons[game::Btn_Continue], renderer);
            else
            {
                drawButton(buttons[game::Btn_Accept], renderer);
                drawButton(buttons[game::Btn_Refuse], renderer);
            }
            // draw offer from banker
            int tw, th;
            string offerText = "Offer:$" + to_string(game::lastOffer);
            getTextSize(font[DialFont], offerText, tw, th, 5);
            textureManager.drawShadowedTex("RemainingPrices", (screenw - tw-8) / 2, (screenh - th-4) / 1.35, tw + 4, th + 8, renderer, 0xFFCC11, 0x000000, 10, 10, 75);
            renderOutlinedText(font[DialFont], offerText, (screenw - tw) / 2, (screenh - th) / 1.35, 5, 0xFFFFFF, 0x333333);
        }
        int id = game::player.playerBox;
        drawBox(textureManager, id, (screenw-boxWidth*1.5f)-20, 10+(screenh-boxWidth*1.5f), game::boxes[id].insideBox, game::allOpened(), true);
    }

    void drawGameOver(TextureManager& textureManager)
    {
        if(!rnd(5) && game::player.finalGain >= game::boxes[game::player.playerBox].insideBox)
            particlesManager.spawnParticle("Confetti", 15, rnd(screenw), -10, (rand() % 50 - 25) / 1.0f, 100, 7.f, 0xFFD700);

        int id = game::player.playerBox;
        drawBox(textureManager, id, (screenw-boxWidth*1.5f)-20, 10+(screenh-boxWidth*1.5f), game::boxes[id].insideBox, game::allOpened(), true);

        int tw, th;
        string priceText = "You won: $" + to_string(game::player.finalGain);
        getTextSize(font[DialFont], priceText, tw, th, 5);
        textureManager.drawShadowedTex("RemainingPrices", (screenw - tw-8) / 2, (screenh - th-4) / 2, tw + 4, th + 8, renderer, 0xFFCC11, 0x000000, 10, 10, 75);
        renderOutlinedText(font[DialFont], priceText, (screenw - tw) / 2, (screenh - th) / 2, 5, 0xFFFFFF, 0x333333);
    }

    void checkGameAtmo()
    {
        size_t maxIndex = max_element(game::boxCombo, game::boxCombo + 3) - game::boxCombo;

        switch (maxIndex)
        { // Set gameAtmo based on the index
            case A_neutral: gameAtmo = A_neutral; break;
            case A_bad: gameAtmo = A_bad; break;
            case A_good: gameAtmo = A_good; break;
        }
    }

    int alphaBlendBad = 0, alphaBlendGood = 0;

    void drawBackground(TextureManager& textureManager)
    {
        switch(gameAtmo)
        {
            case A_good:
                if(alphaBlendGood<200) alphaBlendGood+=2;
                if(alphaBlendBad>=3) alphaBlendBad-=2;
                break;
            case A_bad:
                if(alphaBlendBad<150) alphaBlendBad+=2;
                if(alphaBlendGood>=3) alphaBlendGood-=2;
                break;
            case A_neutral:
                if(alphaBlendBad>=3) alphaBlendBad-=2;
                if(alphaBlendGood>=3) alphaBlendGood-=2;
        }

        textureManager.draw("NeutralBackground", 0, 0, screenw, screenh, renderer);
        textureManager.drawAlphaTex("BadBackground", 0, 0, screenw, screenh, renderer, alphaBlendBad);
        textureManager.drawAlphaTex("GoodBackground", 0, 0, screenw, screenh, renderer, alphaBlendGood);
    }

    void renderGame(TextureManager &textureManager) // rendering a game
    {
        if(gameState==S_ChoosePlayerBox || gameState==S_OpeningBoxes || gameState==S_BankCall) drawBoxes(textureManager);
        else if (gameState==S_BankOffer || gameState==S_Dealing) drawBank(textureManager);
        else if (gameState==S_GameOver) drawGameOver(textureManager);

        if(game::lastBoxOpeningTime+1500 > elapsedTime && game::openCount())
        {
            int tw, th;
            string valueText = "$" + to_string(game::lastBoxValue);
            getTextSize(font[DialFont], valueText, tw, th, 5);
            textureManager.drawShadowedTex("RemainingPrices", (screenw - tw-8) / 2, (screenh - th-4) / 1.35, tw + 4, th + 8, renderer, backgroundColor(game::lastBoxValue), 0x000000, 10, 10, 75);
            renderOutlinedText(font[DialFont], valueText, (screenw - tw) / 2, (screenh - th) / 1.35, 5, 0xFFFFFF, 0x333333);
        }

        drawDialogs(textureManager);
        drawRemainingPrices(textureManager);
    }
}
