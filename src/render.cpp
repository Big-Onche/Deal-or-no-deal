#include "main.h"
#include "textures.h"

namespace render
{
    int boxWidth = 140, boxHeight = 125;
    int boxSpacing = 15;

    int boxesgridX() { return (screenw - (4 * boxWidth + 3 * boxSpacing)) / 2; }
    int boxesgridY() { return ((4 * boxHeight + 3 * boxSpacing)) - screenh / 1.3f; }

    void drawBox(TextureManager& textureManager, int id, int x, int y, int boxvalue, bool opened, bool ownBox = false) // draw one box
    {
        int bwidth = boxWidth, bheight = boxHeight, shadowOffset = 4, textsize = 2;

        if(ownBox) { bwidth *= 1.5f, bheight *= 1.5f, shadowOffset *= 2, textsize*=2; }

        textureManager.drawShadowedTex(opened ? "OpenedBox" : "ClosedBox", x, y, bwidth, bheight, renderer, 0x000000, shadowOffset, shadowOffset, 50, 75);

        if(opened)
        {
            string text = "$" + to_string(boxvalue);
            int tw, th;
            sdl::getTextSize(text, tw, th, ownBox ? 3 : 2);
            int textX = (x + (bwidth - tw) / 2) + 8, textY = y + (bheight - th) / 5;

            sdl::renderText(text, textX, textY, textsize, 0xFFFFFF);
        }
        sdl::renderText(to_string(id + 1), x + bwidth / 2, y + bheight / 1.5f, textsize, opened ? 0x999999 : 0xFFFFFF);
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
            drawBox(textureManager, id, (screenw-boxWidth*1.5f)-20, 20+(screenh-boxWidth*1.5f), game::boxes[id].insideBox, game::allOpened(), true);
        }
    }

    void drawRemainingPrices(TextureManager& textureManager)
    {
        int textSize = 3;
        int values[game::maxBoxes], numValues = 0;

        loopi(game::maxBoxes) if (!game::boxes[i].opened) values[numValues++] = game::boxes[i].insideBox;

        sort(values, values + numValues);

        int splitIndex = numValues/2;
        int lineHeight = static_cast<int>(ch * textSize) + 11;

        loopi(numValues)
        {
            int val = values[i];
            string text = "$" + to_string(val);
            int x, y, tw, th;

            sdl::getTextSize(text, tw, th, textSize);

            if (i < splitIndex) { x = 10; y = 10 + i * lineHeight; }
            else { x = screenw - tw; y = 10 + (i - splitIndex) * lineHeight;}

            uint32_t bgrdColor = values[i]==69 ? 0xCC33CC : values[i]==420 ? 0x00CC00 : values[i] < 2000 ? 0x3333FF : values[i] < 50000 ? 0xCCCC33 : 0xFF3333;

            textureManager.setColorMod("RemainingPrices", (bgrdColor >> 16) & 0xFF, (bgrdColor >> 8) & 0xFF, bgrdColor & 0xFF);
            textureManager.draw("RemainingPrices", x - 4, y - 6, tw + 8, th + 8, renderer);

            sdl::renderOutlinedText(text, x, y, textSize, 0xFFFFFF, 0x333333);
        }
    }

    void drawDialogs(TextureManager& textureManager)
    {
        textureManager.drawShadowedTex("Presenter", -30, screenh-230, 200, 300, renderer, 0x000000, 15, 15, 50, 75);
        textureManager.draw("Bubble", 133, screenh-165, 640, 160, renderer);

        if(gameState == S_ChoosePlayerBox) game::presenterDialog = "Please choose your box!";
        sdl::renderText(game::presenterDialog, 225, screenh-120, 2.5f, 0x000000, 500);
    }

    void renderGame() // rendering a game
    {
        TextureManager& textureManager = TextureManager::getInstance();
        drawDialogs(textureManager);
        drawBoxes(textureManager);
        drawRemainingPrices(textureManager);
    }
}
