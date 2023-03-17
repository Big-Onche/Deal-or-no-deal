#define SDL_MAIN_HANDLED
#include "main.h"

EngineState engineState;

int main(int argc, char *argv[])
{
    logoutf("init: main");
    gl::glInit();
    sound::initAudio();
    clearConsole();
    sound::playSound("theme");
    conoutf(C_YELLOW, C_BLACK, "Welcome to Deal or no Deal!\n\n");
    logoutf("init: game");

    for(;;) // main loop
    {
        if(!gl::glLoop()) break; // break if SDL_QUIT is called
    }

    quit();
}

void quit() // cleanup and quit
{
    engineState = S_ShuttingDown;
    gl::glQuit();
    sound::unInitAudio();
    logoutf("shutdown: exit success\n\n");
    exit(EXIT_SUCCESS);
}
