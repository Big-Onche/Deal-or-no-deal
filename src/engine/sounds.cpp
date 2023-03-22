#include "main.h"
#include "sounds.h"

using namespace std;

bool SoundManager::init() //sdl2_mixer init
{
    logoutf("init: sound");

    SDL_DisplayMode desktop;
    SDL_GetDesktopDisplayMode(0, &desktop); // getting desktop sample freq

    if(Mix_OpenAudio(desktop.refresh_rate, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        logoutf("Failed to initialize SDL_mixer: (%s)", Mix_GetError());
        return false;
    }
    Mix_VolumeMusic(96);
    preloadSounds(); // if initialized, we preload all sounds in sounds.cfg
    return true;
}

bool SoundManager::load(const string &fileName, const string &id) // load one sound
{
    Mix_Chunk* sound = Mix_LoadWAV(fileName.c_str());
    if(sound == nullptr)
    {
        logoutf("warning: failed to load sound (%s)", fileName.c_str());
        return false;
    }

    m_soundMap[id] = sound;
    return true;
}

void SoundManager::preloadSounds() // preload all sounds soft-coded in sounds.cfg
{
    SoundManager& soundManager = SoundManager::getInstance();

    ifstream configFile("config/sounds.cfg");
    if(!configFile.is_open()) fatal("Unable to open sounds.cfg!");

    string line;
    while(getline(configFile, line))
    {
        istringstream iss(line);
        string command, filePath, soundID;

        if(iss >> command >> filePath >> soundID)
        {
            if(command == "sound") soundManager.load(filePath, soundID);
        }
    }
    configFile.close();
}

void SoundManager::play(const string &id, int loops) // play a sound
{
    auto it = m_soundMap.find(id);
    if (it == m_soundMap.end())
    {
        logoutf("warning: failed to play sound (%s)", id.c_str());
        return;
    }

    Mix_PlayChannel(-1, it->second, loops);
}

void SoundManager::stop(const string &id) // stop a sound
{
    auto it = m_soundMap.find(id);
    if (it == m_soundMap.end())
    {
        logoutf("warning: failed to stop sound (%s)", id);
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

Mix_Music* music;

void SoundManager::playMusic(const string &path, int loops) // cleanup
{
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if(music == nullptr)
    {
        logoutf("warning: failed to play music (%s)", Mix_GetError());
        return;
    }

    if(Mix_PlayMusic(music, loops) == -1)
    {
        logoutf("warning: failed to play music (%s)", Mix_GetError());
        return;
    }
}

void SoundManager::quit() // cleanup
{
    for (auto& sound : m_soundMap) Mix_FreeChunk(sound.second);
    m_soundMap.clear();
    Mix_FreeMusic(music);
    Mix_CloseAudio();
}

