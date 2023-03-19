#include "main.h"
#include "game.h"
#include "textures.h"

namespace render
{
    int boxWidth = 140, boxHeight = 125;
    int boxSpacing = 12;

    int boxesgridX() { return (screenw - (4 * boxWidth + 3 * boxSpacing)) / 2; }
    int boxesgridY() { return ((4 * boxHeight + 3 * boxSpacing)) - screenh / 1.35f; }

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
            loopj(4)
            {
                int id = i * 4 + j;
                int x = boxesgridX() + j * (boxWidth + boxSpacing);
                int y = boxesgridY() + i * (boxHeight + boxSpacing);
                if(gameState==S_ChoosePlayerBox || id!=game::player.playerBox) drawBox(textureManager, id, x, y, game::boxes[id].insideBox, game::boxes[id].opened);
            }
        }

        if(gameState>S_ChoosePlayerBox)
        {
            int id = game::player.playerBox;
            drawBox(textureManager, id, (screenw-boxWidth*1.5f)-20, 10+(screenh-boxWidth*1.5f), game::boxes[id].insideBox, game::allOpened(), true);
        }
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

            uint32_t bgrdColor = values[i]==69 ? 0xCC33CC : values[i]==420 ? 0x00CC00 : values[i] < 2000 ? 0x3333FF : values[i] < 50000 ? 0xCCCC33 : 0xFF3333;

            textureManager.setColorMod("RemainingPrices", bgrdColor);
            textureManager.draw("RemainingPrices", x - 4, y - 6, tw + 8, th + 8, renderer);

            renderOutlinedText(font[MainFont], text, x, y, textSize, 0xFFFFFF, 0x333333);
        }
    }

    void drawDialogs(TextureManager& textureManager)
    {
        textureManager.drawShadowedTex("Presenter", -30, screenh-230, 200, 300, renderer, 0xFFFFFF, 0x000000, 15, 15, 75);
        textureManager.draw("Bubble", 135, screenh-160, 652, 163, renderer);

        renderShadowedText(font[DialFont], game::mainDialog, 217, screenh-127, 3, 0x000000, 0xCCCCCC, 550);
    }

    SDL_Rect yesRect, noRect;

    void drawBank(TextureManager& textureManager)
    {
        textureManager.drawShadowedTex("BankVault", (screenw - 500) / 2, (screenh - 500) / 8, 500, 500, renderer, 0xFFFFFF, 0x000000, 15, 15, 75);
        textureManager.drawShadowedTex("Banker", (screenw - 500) / 2, (screenh - 500) / 3, 500, 500, renderer, 0xFFFFFF, 0x000000, 15, 15, 75);

        if(gameState==S_Dealing)
        {
            int x=820, y=600, tw, th;
            string acceptText = "Accept", refuseText = "Refuse";

            yesRect = {x, y, static_cast<int>(acceptText.length()) * cw[DialFont] * 4, ch[DialFont]*4};
            getTextSize(font[DialFont], acceptText, tw, th, 4);
            textureManager.drawShadowedTex("Button", x - 4, y - 6, tw + 8, th + 8, renderer, 0x00CC00, 0x000000, 5, 5, 75);
            renderOutlinedText(font[DialFont], acceptText, x, y, 4, 0xFFFFFF, 0x333333);

            y+=55;

            noRect = {x, y, static_cast<int>(refuseText.length()) * cw[DialFont] * 4, ch[DialFont]*4};
            getTextSize(font[DialFont], refuseText, tw, th, 4);
            textureManager.drawShadowedTex("Button", x - 4, y - 6, tw + 8, th + 8, renderer, 0xFF0000, 0x000000, 5, 5, 75);
            renderOutlinedText(font[DialFont], refuseText, x, y, 4, 0xFFFFFF, 0x333333);

            string offerText = "Offer: $" + to_string(game::bankOffer());
            getTextSize(font[DialFont], offerText, tw, th, 5);
            textureManager.drawShadowedTex("RemainingPrices", (screenw - tw-8) / 2, (screenh - th-4) / 1.35, tw + 4, th + 8, renderer, 0xFFCC11, 0x000000, 10, 10, 75);
            renderOutlinedText(font[DialFont], offerText, (screenw - tw) / 2, (screenh - th) / 1.35, 5, 0xFFFFFF, 0x333333);
        }

        int id = game::player.playerBox;
        drawBox(textureManager, id, (screenw-boxWidth*1.5f)-20, 10+(screenh-boxWidth*1.5f), game::boxes[id].insideBox, game::allOpened(), true);
    }

    void drawBackground(TextureManager& textureManager)
    {
        //textureManager.draw("GameBackground", 0, 0, screenw, screenh, renderer);
    }

    void renderGame() // rendering a game
    {
        TextureManager& textureManager = TextureManager::getInstance();

        if(gameState==S_ChoosePlayerBox || gameState==S_OpeningBoxes) drawBoxes(textureManager);
        else drawBank(textureManager);
        drawDialogs(textureManager);
        drawRemainingPrices(textureManager);
    }
}
