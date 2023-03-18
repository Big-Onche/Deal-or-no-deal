#define SDL_MAIN_HANDLED
#include "main.h"

EngineState engineState;

const int maxfps = 30;
const int frameDelay = 1000 / maxfps;

Uint32 frameStart;
int frameTime;

int main(int argc, char *argv[])
{
    logoutf("init: main");
    sdl::sdlInit();
    sound::initAudio();
    sound::playSound("theme");
    logoutf("init: game");

    for(;;) // main loop
    {
        frameStart = SDL_GetTicks();

        if(!sdl::sdlLoop()) break; // break if SDL_QUIT is called

        frameTime = SDL_GetTicks() - frameStart; // Calculate the time taken for one loop iteration
        if(frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
    }

    quit();
}

void quit(bool fatal) // cleanup and quit
{
    engineState = S_ShuttingDown;
    sdl::sdlQuit();
    sound::unInitAudio();
    if(fatal)
    {
        logoutf("shutdown: fatal error\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        logoutf("shutdown: exit success\n");
        exit(EXIT_SUCCESS);
    }
}

void fatal(const std::string &message)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), nullptr);
    std::cerr << message << std::endl;
    quit(true);
}
