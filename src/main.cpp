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
    printf("Welcome to Deal or no Deal!\n");
    printf("Press \"y\" to continue or \"n\" to quit.\n");
    string response;

    while(response!="y" || response!="n")
    {
        getline(cin, response);
        if(response=="y")
        {
            game::initGame();
            game::runGame(); // main loop here
            break;
        }
        else if(response=="n") quit();
    }
}

void quit()
{
    sound::unInitAudio();
    exit(EXIT_SUCCESS);
}

void clearConsole()
{
    #if __linux__
      system("clear");
    #elif _WIN32
      system("cls");
    #endif
}

