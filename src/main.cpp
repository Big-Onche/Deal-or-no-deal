#define SDL_MAIN_HANDLED
#include "main.h"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char *argv[])
{
    logoutf("init: main");
    gl::glInit();
    sound::initAudio();
    clearConsole();
    sound::playSound("theme");
    conoutf(C_YELLOW, C_BLACK, "Welcome to Deal or no Deal!\n\n");
    logoutf("init: game");

    for(;;) // main loop, initializing game and continue/quit choice after each game
    {
        gl::glLoop();
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
    gl::glQuit();
    sound::unInitAudio();
    logoutf("shutdown: exit success\n\n");
    exit(EXIT_SUCCESS);
}
