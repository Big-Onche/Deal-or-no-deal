#include "main.h"

using namespace std;

int main()
{
    #ifdef WIN32
      SetConsoleTitle("Deal or no Deal");
    #endif
    logoutf("init: main");
    sound::initAudio();
    clearConsole();
    sound::playSound("theme");
    conoutf(C_YELLOW, C_BLACK, "Welcome to Deal or no Deal!\n\n");
    logoutf("init: game");

    for(;;) // main loop, initializing game and continue/quit choice after each game
    {
        printf("You want to continue? (Y/N)\n");
        string response;

        while(response!="Y" || response!="N")
        {
            getline(cin, response);
            if(response=="Y")
            {
                game::initGame();
                break;
            }
            else if(response=="N") quit();
        }
        game::playGame();
    }
}

void quit()
{
    sound::unInitAudio();
    logoutf("uninit: exit success\n\n");
    exit(EXIT_SUCCESS);
}
