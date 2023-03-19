#include "main.h"
#include "sounds.h"

bool SoundManager::init()
{
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
    {
        std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void SoundManager::quit()
{
    for (auto& sound : m_soundMap)
    {
        Mix_FreeChunk(sound.second);
    }
    m_soundMap.clear();
    Mix_CloseAudio();
}

bool SoundManager::load(const std::string& fileName, const std::string& id)
{
    Mix_Chunk* chunk = Mix_LoadWAV(fileName.c_str());
    if (!chunk)
    {
        std::cerr << "Failed to load sound: " << fileName << " | SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    m_soundMap[id] = chunk;
    return true;
}

void SoundManager::play(const std::string& id, int loops)
{
    auto it = m_soundMap.find(id);
    if (it == m_soundMap.end())
    {
        std::cerr << "Failed to play sound: " << id << " not found." << std::endl;
        return;
    }

    Mix_PlayChannel(-1, it->second, loops);
}

void SoundManager::stop(const std::string& id)
{
    auto it = m_soundMap.find(id);
    if (it == m_soundMap.end())
    {
        std::cerr << "Failed to stop sound: " << id << " not found." << std::endl;
        return;
    }

    int channelCount = Mix_AllocateChannels(-1);
    for (int i = 0; i < channelCount; i++)
    {
        if (Mix_GetChunk(i) == it->second)
        {
            Mix_HaltChannel(i);
        }
    }
}
