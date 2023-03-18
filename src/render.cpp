#include "main.h"
#include "textures.h"

namespace render
{
    int boxWidth = 140, boxHeight = 125;
    int boxSpacing = 40;

    int boxesgridX() { return (screenw - (4 * boxWidth + 3 * boxSpacing)) / 2; }
    int boxesgridY() { return (screenh - (4 * boxHeight + 3 * boxSpacing)) / 3; }

    void drawBox(TextureManager& textureManager, int id, int x, int y, float width, float height, int boxvalue, bool opened, bool ownBox = false) // draw one box
    {
        textureManager.draw(opened ? "OpenedBox" : "ClosedBox", x, y, static_cast<int>(width), static_cast<int>(height), gl::renderer);

        if(opened)
        {
            string text = "$" + to_string(boxvalue);
            int tw, th;
            gl::getTextSize(text, tw, th, ownBox ? 5 : 2);
            int textX = (x + (width - tw) / 2) + 8, textY = y + (height - th) / 5;

            gl::renderText(text, textX, textY, ownBox ? 5 : 2, 0xFFFFFF);
        }
        gl::renderText(to_string(id + 1), x + width / 2, y + height / 1.5f, ownBox ? 6 : 3, opened ? 0x999999 : 0xFFFFFF);
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
                if(gameState==S_ChoosePlayerBox || id!=game::player.playerBox) drawBox(textureManager, id, x, y, boxWidth, boxHeight, game::boxes[id].insideBox, game::boxes[id].opened);
            }
        }

        if(gameState>S_ChoosePlayerBox)
        {
            int id = game::player.playerBox;
            drawBox(textureManager, id, (screenw-boxWidth*2.5f)-30, 30+(screenh-boxWidth*2.5f), boxWidth*2.5f, boxHeight*2.5f, game::boxes[id].insideBox, game::allOpened(), true);
        }
    }

    void drawRemainingPrices(TextureManager& textureManager)
    {
        int textSize = 3;
        int values[game::maxBoxes], numValues = 0;

        loopi(game::maxBoxes) if (!game::boxes[i].opened) values[numValues++] = game::boxes[i].insideBox;

        sort(values, values + numValues);

        int splitIndex = numValues/2;
        int lineHeight = static_cast<int>(gl::ch * textSize) + 11;

        loopi(numValues)
        {
            int val = values[i];
            string text = "$" + to_string(val);
            int x, y, tw, th;

            gl::getTextSize(text, tw, th, textSize);

            if (i < splitIndex) { x = 10; y = 10 + i * lineHeight; }
            else { x = screenw - tw; y = 10 + (i - splitIndex) * lineHeight;}

            uint32_t bgrdColor = values[i]==69 ? 0xCC33CC : values[i]==420 ? 0x00CC00 : values[i] < 2000 ? 0x3333FF : values[i] < 50000 ? 0xCCCC33 : 0xFF3333;

            textureManager.setColorMod("RemainingPrices", (bgrdColor >> 16) & 0xFF, (bgrdColor >> 8) & 0xFF, bgrdColor & 0xFF);
            textureManager.draw("RemainingPrices", x - 4, y - 6, tw + 8, th + 8, gl::renderer);

            gl::renderOutlinedText(text, x, y, textSize, 0xFFFFFF, 0x333333);
        }
    }

    void drawDialogs(TextureManager& textureManager)
    {
        int textSize = 3;

        textureManager.draw("Presenter", -50, screenh-350, 300, 450, gl::renderer);
        textureManager.draw("Bubble", 200, screenh-250, 960, 240, gl::renderer);

        if(gameState == S_ChoosePlayerBox) game::presenterDialog = "Please choose your box!";
        gl::renderShadowedText(game::presenterDialog, 340, screenh-180, textSize, 0x000000, 0xCCCCCC, 750);
    }

    void renderGame() // rendering a game
    {
        TextureManager& textureManager = TextureManager::getInstance();
        drawDialogs(textureManager);
        drawBoxes(textureManager);
        drawRemainingPrices(textureManager);
    }
}
