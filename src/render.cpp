#include "main.h"

namespace render
{
    int boxWidth = 140, boxHeight = 125;
    int boxSpacing = 40;

    int boxesgridX() { return (screenw - (4 * boxWidth + 3 * boxSpacing)) / 2; }
    int boxesgridY() { return (screenh - (4 * boxHeight + 3 * boxSpacing)) / 3; }

    void drawBox(int id, int x, int y, float width, float height, int boxvalue, bool opened) // draw one box
    {
        SDL_Rect boxRect = {x, y, static_cast<int>(width), static_cast<int>(height)};

        SDL_RenderCopy(gl::renderer, opened ? gl::openedBoxTex : gl::closedBoxTex, nullptr, &boxRect);

        if(opened)
        {
            string text = "$" + to_string(boxvalue);

            int tw, th;
            float textSize = 2;
            gl::getTextSize(text, tw, th, textSize);
            int textX = (x + (width - tw) / 2) + 8, textY = y + (height - th) / 5;

            gl::renderText(text, textX, textY, textSize, 0xFFFFFF);
        }

        gl::renderText(to_string(id + 1), x + width / 2, y + height / 1.5f, 2.5f, opened ? 0x999999 : 0xFFFFFF);
    }

    void drawBoxes() // draw all boxes in a grid
    {
        loopi(4)
        {
            loopj(4)
            {
                int id = i * 4 + j;
                int x = boxesgridX() + j * (boxWidth + boxSpacing);
                int y = boxesgridY() + i * (boxHeight + boxSpacing);
                if(gameState==S_ChoosePlayerBox || id!=game::player.playerBox) drawBox(id, x, y, boxWidth, boxHeight, game::boxes[id].insideBox, game::boxes[id].opened);
            }
        }

        if(gameState>S_ChoosePlayerBox)
        {
            int id = game::player.playerBox;
            drawBox(id, (screenw-boxWidth*2.5f)-30, 30+(screenh-boxWidth*2.5f), boxWidth*2.5f, boxHeight*2.5f, id, game::allOpened());
        }
    }

    void drawRemainingPrices()
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

            SDL_Rect rectDst = {x - 4, y - 6, tw + 8, th + 8};
            uint32_t bgrdColor = values[i]==69 ? 0xCC33CC : values[i]==420 ? 0x00CC00 : values[i] < 2000 ? 0x3333FF : values[i] < 50000 ? 0xCCCC33 : 0xFF3333;

            SDL_SetTextureColorMod(gl::priceTex, (bgrdColor >> 16) & 0xFF, (bgrdColor >> 8) & 0xFF, bgrdColor & 0xFF);
            SDL_RenderCopy(gl::renderer, gl::priceTex, nullptr, &rectDst);

            gl::renderOutlinedText(text, x, y, textSize, 0xFFFFFF, 0x333333);
        }
    }

    void drawDialogs()
    {
        int textSize = 3;
        int tw, th;

        if(gameState == S_ChoosePlayerBox) game::presentatorDialog = "Please choose your box!";

        gl::renderText(game::presentatorDialog, 125, screenh-125, textSize, 0xFFFFFF, screenw/2);
    }

    void renderGame() // rendering a game
    {
        drawDialogs();
        drawBoxes();
        drawRemainingPrices();
    }
}
