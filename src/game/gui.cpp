#include "main.h"
#include "game.h"
#include "textures.h"

namespace gui
{
    vector<sdl::RectInfo> menuItems;

    void handleMenus(SDL_Event &event, SDL_Point &mousePoint)
    {
        if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            loopi(menuItems.size())
            {
                if(SDL_PointInRect(&mousePoint, &menuItems[i].rect))
                {
                    switch(i)
                    {
                        case 0:
                            engineState = S_LoadingScreen;
                            game::initGame();
                            break;

                        case 1:
                            // options
                            break;

                        case 2: quit();
                    }
                }
            }
        }
    }

    void renderSplashScreen(string text) // showing splash screen
    {
        TextureManager::getInstance().drawShadowedTex("GameLogo", (screenw - 500) / 2, (screenh - 500) / 2, 500, 500, renderer, 0xFFFFFF, 0x191919, 20, 20, 100);

        int tw, th; // text width, text height
        int textSize = 3;

        getTextSize(font[MainFont], text, tw, th, textSize);

        int x = (screenw - tw) / 2;
        int y = (screenh - th) / 1.05f;

        renderText(MainFont, text, x, y, textSize);
    }

    void initMenuItems(int textSize, int x, int y)
    {
        array<string, 3> menuItemTexts = {"New Game", "Options", "Exit"};

        for (const auto& itemText : menuItemTexts)
        {
            menuItems.emplace_back(x, y, static_cast<int>(itemText.length()) * cw[MainFont] * textSize, ch[MainFont] * textSize, itemText, "Button");
            y += 75;
        }
    }

    void renderMenu()
    {
        int textSize = 4;
        int x = 100, y = 350;

        if(menuItems.empty()) initMenuItems(textSize, x, y);

        TextureManager& textureManager = TextureManager::getInstance();

        textureManager.drawShadowedTex("GameLogo", 90, 100, 210, 210, renderer, 0xFFFFFF, 0x191919, 5, 5, 100);

        for(const sdl::RectInfo& menuItem : menuItems) // Iterate through the menu items and render the
        {
            textureManager.drawShadowedTex(menuItem.textureID, menuItem.rect.x, menuItem.rect.y, menuItem.rect.w, menuItem.rect.h, renderer, 0x888888, 0x222222);
            renderShadowedText(font[MainFont], menuItem.text, menuItem.rect.x, menuItem.rect.y, textSize, 0xFFFFFF, 0x444444);
            y += 75;
        }
    }
}

