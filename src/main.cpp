#include "main.h"

using namespace std;

int main()
{
    #ifdef WIN32
      SetConsoleTitle("Deal or no Deal");
    #endif
    sound::initAudio();
    clearConsole();
    sound::playSound("theme");
    conoutf(C_YELLOW, C_BLACK, "Welcome to Deal or no Deal!\n\n");
    conoutf(C_DEF, C_DEF, "Press \"Y\" to continue or \"N\" to quit.\n");
    string response;

    while(response!="Y" || response!="N")
    {
        getline(cin, response);
        if(response=="Y")
        {
            game::initGame();
            game::runGame(); // main loop here
            break;
        }
        else if(response=="N") quit();
    }
}

void quit()
{
    sound::unInitAudio();
    exit(EXIT_SUCCESS);
}
