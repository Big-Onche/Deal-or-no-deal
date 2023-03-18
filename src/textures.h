#ifndef TEXTURES_H
#define TEXTURES_H

#include <map>
#include "tools.h"

class TextureManager
{
    public:
        static TextureManager& getInstance();
        bool load(const string& fileName, const string& id, SDL_Renderer* pRenderer);
        void draw(const string& id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void drawFrame(const string& textureID, int x, int y, int width, int height, int srcX, int srcY, int srcW, int srcH, int scale, SDL_Renderer* renderer);
        void setColorMod(const string& textureID, Uint8 r, Uint8 g, Uint8 b);
        void clearTextures();

    private:
        TextureManager() {}
        TextureManager(const TextureManager&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;

        map<string, SDL_Texture*> m_textureMap;
};

#endif
