#ifndef SOUNDS_H
#define SOUNDS_H

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

        bool load(const std::string &fileName, const std::string &id);
        void preloadSounds();
        void play(const std::string &id, int loops = 0);
        void stop(const std::string &id);
        void playMusic(const std::string &path, int loops = 0);

    private:
        SoundManager() {}
        SoundManager(const SoundManager&) = delete;
        SoundManager& operator=(const SoundManager&) = delete;

        std::map<std::string, Mix_Chunk*> m_soundMap;
};

#endif
