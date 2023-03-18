#ifndef TEXTURES_H
#define TEXTURES_H

#include <map>
#include "tools.h"

class TextureManager
{
    public:
        static TextureManager& getInstance();
        bool load(const string& fileName, const string& id, SDL_Renderer* pRenderer);
        void preloadTextures();
        void draw(const string& id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void drawShadowedTex(const string& textureID, int x, int y, int width, int height, SDL_Renderer* renderer, uint32_t originalColor, uint32_t shadowColor, int offsetX = 5, int offsetY = 5, Uint8 shadowAlpha = 64);
        void drawFrame(const string& textureID, int x, int y, int width, int height, int srcX, int srcY, int srcW, int srcH, int scale, SDL_Renderer* renderer);
        void setColorMod(const string& textureID, uint32_t colorMod);
        void setAlpha(const string& textureID, Uint8 alpha);
        void clearTextures();

    private:
        TextureManager() {}
        TextureManager(const TextureManager&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;

        map<string, SDL_Texture*> m_textureMap;
};

#endif
