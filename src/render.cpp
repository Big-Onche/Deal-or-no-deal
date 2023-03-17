#include "main.h"

namespace render
{
    int boxSize = 75;
    int boxSpacing = 30;

    int boxesgridX() { return (screenw - (4 * boxSize + 3 * boxSpacing)) / 2; }
    int boxesgridY() { return (screenh - (4 * boxSize + 3 * boxSpacing)) / 2; }

    void drawBox(int id, int x, int y, float size, bool opened) // draw one box
    {
        SDL_Rect boxRect = {x, y, static_cast<int>(size), static_cast<int>(size)};

        uint32_t boxColor = opened ? 0x443333 : 0x666666FF;
        SDL_SetRenderDrawColor(gl::renderer, (boxColor >> 16) & 0xFF, (boxColor >> 8) & 0xFF, boxColor & 0xFF, 255);

        SDL_RenderFillRect(gl::renderer, &boxRect);

        gl::renderText(to_string(id + 1), x + size / 2, y + size / 2, 2.5f, 0xFFFFFF);
    }

    void drawBoxes() // draw all boxes in a grid
    {
        loopi(4)
        {
            loopj(4)
            {
                int id = i * 4 + j;
                int x = boxesgridX() + j * (boxSize + boxSpacing);
                int y = boxesgridY() + i * (boxSize + boxSpacing);
                drawBox(id, x, y, boxSize, game::boxes[id].opened);
            }
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

            SDL_SetTextureColorMod(gl::priceTexture, (bgrdColor >> 16) & 0xFF, (bgrdColor >> 8) & 0xFF, bgrdColor & 0xFF);
            SDL_RenderCopy(gl::renderer, gl::priceTexture, nullptr, &rectDst);

            gl::renderOutlinedText(text, x, y, textSize, 0xFFFFFF, 0x333333);
        }
    }

    void renderGame() // rendering a game
    {
        drawBoxes();
        drawRemainingPrices();
    }
}
