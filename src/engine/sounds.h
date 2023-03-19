#ifndef SOUNDS_H
#define SOUNDS_H

#include <map>
#include <string>
#include "SDL_mixer.h"

class SoundManager
{
    public:
        static SoundManager& getInstance()
        {
            static SoundManager instance;
            return instance;
        }

        bool init();
        void quit();

        bool load(const string &fileName, const string &id);
        void preloadSounds();
        void play(const string &id, int loops = 0);
        void stop(const string &id);
        void playMusic(const string &path);

    private:
        SoundManager() {}
        SoundManager(const SoundManager&) = delete;
        SoundManager& operator=(const SoundManager&) = delete;

        map<string, Mix_Chunk*> m_soundMap;
};

#endif
