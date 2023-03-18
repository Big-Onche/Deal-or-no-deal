#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include "main.h"

namespace sound
{
    ma_result result;
    ma_engine engine;

    bool initAudio()
    {
        result = ma_engine_init(NULL, &engine);
        if(result != MA_SUCCESS)
        {
            logoutf("Failed to initialize audio module (miniaudio)");
            return result;  // Failed to initialize the engine.
        }
        logoutf("init: audio (miniaudio version: %s)", MA_VERSION_STRING);

        return true;
    }

    void playSound(const char *soundName)
    {
        char path[64]; sprintf(path, "data/audio/%s.wav", soundName);
        result = ma_engine_play_sound(&engine, path, NULL);
        if(result != MA_SUCCESS) logoutf("Failed to play sound (%s)", path);
    }

    void unInitAudio()
    {
        logoutf("shutdown: miniaudio");
        ma_engine_uninit(&engine);
    }
}




