#include "main.h"

namespace render
{
    void drawProgressBar(float progress)
    {
        const int barWidth = 40;
        int numBars = (int)(progress / 100.0f * barWidth);
        conoutf(C_GREEN, C_BLACK, "Please wait, we are assigning boxes...\n", progress);
        printf("[");
        loopi(barWidth)
        {
            if (i < numBars) printf("=");
            else printf(" ");
        }
        printf("] %3.1f%%\r", progress);
        fflush(stdout);
    }

    const char *boxcolor(int id, int playerBox, bool opened) // color of the box, based on: your box / opened / not opened
    {
        return id + 1 == playerBox ? "\033[1;31m" : (opened ? "\033[1;35m" : "\033[1;32m");
    }

    string openedBoxTop(int insideBox) // build the top of the opened box with centered price
    {
        stringstream ss;
        int valueLength = to_string(insideBox).length();
        int leftPadding = (12 - valueLength) / 2;
        int rightPadding = 12 - valueLength - leftPadding;

        ss << "[" << setw(leftPadding) << "" << insideBox << "$" << setw(rightPadding) << "]";
        return ss.str();
    }

    void drawBoxTop(int id, int playerBox, bool opened, int insideBox) // draw closed or opened top of the box
    {
        if(!opened) printf("%s%c%s%c\033[0m   ", boxcolor(id, playerBox, opened), 218, "------------", 191);
        else printf("%s%s\033[0m   ", boxcolor(id, playerBox, opened), openedBoxTop(insideBox).c_str());
    }

    void drawBoxBottom(int id, int playerBox, bool opened) // bottom of the box with centered box number
    {
        printf("%s|_____%s%d_____|\033[0m   ", boxcolor(id, playerBox, opened), id + 1 > 9 ? "" : "_", id + 1);
    }

    void drawBoxes(const game::playerinfo &player, game::box boxes[]) // rendering all boxes
    {
        loopi(4)
        {
            loopj(4) { // top
                int id = i * 4 + j;
                drawBoxTop(id, player.playerBox, boxes[id].opened, boxes[id].insideBox);
            }
            printf("\n");

            loopj(4) { // middle (always the same)
                int id = i * 4 + j;
                printf("%s%c%s%c\033[0m   ", boxcolor(id, player.playerBox, boxes[id].opened), 218, "------------", 191);
            }
            printf("\n");
            loopj(4) { // bottom
                int id = i * 4 + j;
                drawBoxBottom(id, player.playerBox, boxes[id].opened);
            }
            printf("\n\n\n");
        }
    }

    void drawRemainingPrices(const game::playerinfo &player, game::box boxes[]) // rendering all remaining prices
    {
        conoutf(C_DEF, C_DEF, "Remaining boxes: %d\n", game::openCount(true));

        int values[game::maxBoxes];
        int numValues = 0;

        loopi(game::maxBoxes)
        {
            if (!boxes[i].opened) values[numValues++] = boxes[i].insideBox;
        }

        sort(values, values + numValues);

        for (int i = 0; i < numValues; i += 8) {
            for (int j = i; j < i + 8 && j < numValues; j++) {
                int val = values[j];
                conoutf(C_WHITE, values[j]==69 ? C_MAGENTA : values[j]<2000 ? C_BLUE : values[j]<50000 ? C_YELLOW : C_RED, "$%-11d\033[0m ", val);
            }
            printf("\n\n");
        }
    }
}
