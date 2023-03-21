#define SDL_MAIN_HANDLED
#include "main.h"
#include "sounds.h"
#include "game.h"
#include "textures.h"

EngineState engineState;

Uint32 elapsedTime, seconds, milliseconds = 0;

int frameTime;
const int maxfps = 60;
const int frameDelay = 1000 / maxfps;

int main(int argc, char *argv[])
{
    sdl::sdlInit();
    SoundManager::getInstance().init();
    logoutf("init: game");
    game::loadDialogs();
    menus::initMainMenuButtons();

    SoundManager::getInstance().playMusic("data/songs/theme.ogg");

    for(;;) // main loop
    {
        elapsedTime  = SDL_GetTicks();

        seconds = elapsedTime / 1000;
        milliseconds = elapsedTime % 1000;

        if(!sdl::sdlLoop()) break; // break if SDL_QUIT is called

        frameTime = SDL_GetTicks() - elapsedTime; // Calculate the time taken for one loop iteration
        if(frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
    }

    quit();
}

void logoutf(const char* format, ...) // print to log file
{
    FILE* logfile = fopen("log.txt", "a"); // open log file in append mode

    // Print formatted text
    va_list args;
    va_start(args, format);
    vfprintf(logfile, format, args);
    vfprintf(logfile, "\n", args); //newline
    va_end(args);

    fclose(logfile); // close log file
}

void quit(bool fatal) // cleanup and quit
{
    engineState = S_ShuttingDown;

    TextureManager::getInstance().clearTextures();
    SoundManager::getInstance().quit();
    freeButtons();
    sdl::sdlQuit();

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
    quit(true);
}
