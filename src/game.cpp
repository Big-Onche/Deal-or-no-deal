#include "main.h"

using namespace std;

namespace game
{
    const int maxBoxes = 16;
    const int boxValues[maxBoxes] = {0, 1, 10, 30, 69, 100, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 250000, 500000};

    struct box
    {
        int insideBox;
        bool opened;
    };
    box boxes[maxBoxes];

    struct playerinfo
    {
        int playerBox;
        int choosenBox;
        int bankGain;
    };
    struct playerinfo player;

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

    void assignBoxes()
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(0, maxBoxes-1);

        unordered_set<int> usedValues;

        loopi(maxBoxes)
        {
            boxes[i].opened = false;

            drawProgressBar((float)i / maxBoxes * 100);

            int value;
            bool uniqueValueFound = false;
            while (!uniqueValueFound)
            {
                value = dis(gen);
                uniqueValueFound = usedValues.insert(value).second;
            }

            boxes[i].insideBox = boxValues[value];
            clearConsole();
        }
    }

    void initGame() // (re)initialize everything for a new game
    {
        player.bankGain = 0;
        player.playerBox = 0;
        assignBoxes();
    }

    void runGame() // main loop, initializing game and continue/quit choice after each game
    {
        for(;;)
        {
            printf("You want to continue? (Y/N)\n");
            string response;

            while(response!="Y" || response!="N")
            {
                getline(cin, response);
                if(response=="Y")
                {
                    initGame();
                    break;
                }
                else if(response=="N") quit();
            }
            playGame();
        }
    }

    int openCount(bool remaining = false) // count the number of opened boxes (or the number of remaining)
    {
        int num = 0;
        loopi(maxBoxes) if(boxes[i].opened) num++;
        return remaining ? maxBoxes-num : num;
    }

    bool allOpened() // check if all boxes are opened
    {
        return openCount()>=maxBoxes-1; //-1 because of player box
    }

    void bankCall()
    {
        int bankOffer = 0;

        sound::playSound("bank_offer");

        loopi(maxBoxes) if(!boxes[i].opened) bankOffer+=boxes[i].insideBox;
        if(openCount(true)) bankOffer/=((float)openCount(true)*1.25);

        if(player.bankGain)
        {
            printf("At this time, the bank would make another offer: %d$, %s", bankOffer, player.bankGain>=bankOffer ? "well done!\n\n" : "bad luck!\n\n");
        }
        else
        {
            printf("The bank has an offer for you: %d$ Deal or no deal? (Y/N)\n", bankOffer);
            string response;
            while(response!="Y" || response!="N")
            {
                getline(cin, response);
                if(response=="Y")
                {
                    player.bankGain = bankOffer;
                    sound::playSound("victory");
                    printf("You selled your box for %d$\n\n", player.bankGain);
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

    void drawBoxes() // drawing boxes numbers and content of opened boxes
    {
        printf("\n");
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                int index = row * 4 + col;

                char boxTop[32];

                if(!boxes[index].opened && !allOpened()) sprintf(boxTop, "###########");
                else sprintf(boxTop, "[%s%d$%s]",
                             boxes[index].insideBox < 10 ? "    " :
                             boxes[index].insideBox < 100 ? "   " :
                             boxes[index].insideBox < 1000 ? "   " :
                             boxes[index].insideBox < 10000 ? "  " :
                             boxes[index].insideBox < 100000 ? "  " : " ",
                             boxes[index].insideBox,
                             boxes[index].insideBox < 10 ? "   " :
                             boxes[index].insideBox < 100 ? "   " :
                             boxes[index].insideBox < 1000 ? "  " :
                             boxes[index].insideBox < 10000 ? "  " :
                             boxes[index].insideBox < 100000 ? " " : " ");

                conoutf(index+1==player.playerBox ? C_GREEN : (boxes[index].opened ? C_MAGENTA : C_RED), C_WHITE, "%s\033[0m ", boxTop);
            }

            printf("\n");

            for (int col = 0; col < 4; col++)
            {
                int index = row * 4 + col;
                conoutf(index+1==player.playerBox ? C_GREEN : (boxes[index].opened ? C_MAGENTA : C_RED), C_WHITE, "#####%d%s####\033[0m ", index+1, index+1>9 ? "" : "#");
            }
            printf("\n\n");
        }
    }

    void drawRemainingPrices()
    {
        int values[maxBoxes];
        int numValues = 0; // keep track of number of non-zero values

        loopi(maxBoxes)
        {
            if(!boxes[i].opened && boxes[i].insideBox != 0)
            {
                values[numValues++] = boxes[i].insideBox;
            }
        }

        sort(values, values + numValues); // sort only non-zero values

        if(numValues > 0)
        {
            conoutf(C_DEF, C_DEF, "Remaining boxes: %d\n", openCount(true));
            loopi(numValues)
            {
                conoutf(C_WHITE, values[i]==69 ? C_MAGENTA : values[i]<2000 ? C_BLUE : values[i]<20000 ? C_YELLOW : C_RED, "%d$", values[i]);
                if(i < numValues - 1) conoutf(C_DEF, C_DEF, " ");
            }
            conoutf(C_DEF, C_DEF, "\n\n");
        }
    }

    void playGame() // run a game
    {
        while(player.playerBox<1 || player.playerBox>maxBoxes)
        {
            printf("Please select a box from 1 to %d:\n", maxBoxes);
            scanf("%d", &player.playerBox);
        }
        clearConsole();
        bool redraw = true;

        for(;;)
        {
            if(redraw)
            {
                drawBoxes();
                if(!allOpened()) drawRemainingPrices();
            }

            if(openCount())printf("There was %d$ in the %d box!\n", boxes[player.choosenBox-1].insideBox, player.choosenBox);

            if(allOpened())
            {
                if(player.bankGain) printf("You won %d$ and there was %d$ in your box, %s", player.bankGain, boxes[player.playerBox-1].insideBox, player.bankGain>=boxes[player.playerBox-1].insideBox ? "well done!\n\n" : "bad luck!\n\n");
                else printf("You won %d$ with your box %d.\n\n", boxes[player.playerBox-1].insideBox, player.playerBox);
                break;
            }

            switch(openCount()) { case 6: case 10: case 14: bankCall(); }

            printf("Please choose a box to open:\n");
            scanf("%d", &player.choosenBox);
            if(player.choosenBox==player.playerBox)
            {
                printf("That's your box, please choose another one.\n");
                redraw = false;
                continue;
            }
            else if(player.choosenBox<1 || player.choosenBox>maxBoxes)
            {
                player.choosenBox=0;
                printf("Invalid box, please choose between 1 and %d.\n", maxBoxes);
                redraw = false;
                continue;
            }
            else if(boxes[player.choosenBox-1].opened)
            {
                printf("That box is already opened, please choose another one.\n");
                redraw = false;
                continue;
            }
            else
            {
                boxes[player.choosenBox-1].opened = true;
                clearConsole();
                if(boxes[player.choosenBox-1].insideBox>=20000) sound::playSound("money_loss");
                else sound::playSound("box_open");
                redraw = true;
            }
        }
    }
}
