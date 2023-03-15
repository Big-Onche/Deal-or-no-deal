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
    game::runGame();
}

void quit()
{
    sound::unInitAudio();
    logoutf("uninit: exit success\n\n");
    exit(EXIT_SUCCESS);
}
