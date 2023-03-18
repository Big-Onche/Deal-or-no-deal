#include "main.h"
#include "textures.h"

TextureManager& TextureManager::getInstance()
{
    static TextureManager instance;
    return instance;
}

bool TextureManager::load(const string& fileName, const string& id, SDL_Renderer* pRenderer) // load a texture
{
    SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());
    if(pTempSurface == nullptr)
    {
        logoutf("warning: could not load texture (%s)", fileName.c_str());
        return false;
    }

    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pTempSurface);
    SDL_FreeSurface(pTempSurface);

    if(pTexture != nullptr)
    {
        m_textureMap[id] = pTexture;
        return true;
    }
    return false;
}

void TextureManager::draw(const string& id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip) //draw an entire texture
{
    SDL_Rect srcRect;
    SDL_Rect destRect;

    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = destRect.w = width;
    srcRect.h = destRect.h = height;
    destRect.x = x;
    destRect.y = y;

    SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, 0, nullptr, flip);
}

void TextureManager::drawFrame(const string& textureID, int x, int y, int width, int height, int srcX, int srcY, int srcW, int srcH, int scale, SDL_Renderer* renderer) // draw choosen part of a texture
{
    SDL_Rect srcRect = {srcX, srcY, srcW, srcH};
    SDL_Rect destRect = {x, y, width * scale, height * scale};
    SDL_RenderCopy(renderer, m_textureMap[textureID], &srcRect, &destRect);
}

void TextureManager::setColorMod(const string& textureID, Uint8 r, Uint8 g, Uint8 b) // change color of a texture
{
    auto it = m_textureMap.find(textureID);
    if(it != m_textureMap.end()) SDL_SetTextureColorMod(it->second, r, g, b);
    else logoutf("warning: texture not found (%d)", textureID);
}

void TextureManager::clearTextures() // free all textures ressources before quitting
{
    for(auto& texture : m_textureMap) SDL_DestroyTexture(texture.second);
    m_textureMap.clear();
}
