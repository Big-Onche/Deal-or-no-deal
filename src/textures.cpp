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

void TextureManager::preloadTextures() // preload images used in the game, soft-coded in textures.cfg (except font)
{
    TextureManager& textureManager = TextureManager::getInstance();

    if(!textureManager.load("data/gui/font.png", "MainFont", gl::renderer)) fatal("Unable to load font texture!");

    ifstream configFile("config/textures.cfg");
    if (!configFile.is_open()) fatal("Unable to open textures.cfg!");

    string line;
    while (getline(configFile, line))
    {
        istringstream iss(line);
        string command, filePath, textureID;

        if (iss >> command >> filePath >> textureID)
        {
            if (command == "texture") textureManager.load(filePath, textureID, gl::renderer);
        }
    }
    configFile.close();
}
/*
void TextureManager::preloadTextures() // preload images used in the game
{
    TextureManager& textureManager = TextureManager::getInstance();

    textureManager.load("data/gui/logo.jpg", "GameLogo", gl::renderer);
    textureManager.load("data/gui/price.png", "RemainingPrices", gl::renderer);
    textureManager.load("data/images/box_closed.png", "ClosedBox", gl::renderer);
    textureManager.load("data/images/box_opened.png", "OpenedBox", gl::renderer);
    textureManager.load("data/images/presenter.png", "Presenter", gl::renderer);
    textureManager.load("data/images/bubble.png", "Bubble", gl::renderer);
}
*/
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
