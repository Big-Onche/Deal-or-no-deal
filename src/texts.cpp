#include "main.h"
#include "textures.h"

int font[MaxFonts] = {MainFont, DialFont};
int cw[MaxFonts] = {8, 7}; // char width
int ch[MaxFonts] = {12, 9}; // height
int cpr[MaxFonts] = {16, 18}; // char per rows

void getTextSize(int font, const string &text, int &width, int &height, int fontSize, int maxWidth)
{
    int charWidth = cw[font] * fontSize;
    int charHeight = ch[font] * fontSize;
    int currentWidth = 0;
    int currentHeight = charHeight;
    int lineWidth = 0;

    for (char c : text)
    {
        if(c == ' ')
        {
            currentWidth += charWidth;
            if (currentWidth + charWidth > maxWidth)
            {
                currentWidth = 0;
                    currentHeight += charHeight;
            }
        }
        else currentWidth += charWidth;

        lineWidth = std::max(lineWidth, currentWidth);
        width = lineWidth;
        height = currentHeight;
    }
}

void renderText(int font, const string &text, int x, int y, float fontSize, uint32_t fontColor, int maxWidth)
{
    TextureManager& textureManager = TextureManager::getInstance();

    string fontID = font==MainFont ? "MainFont" : "DialFont";

    SDL_Rect srcRect = {0, 0, cw[font], ch[font]};
    SDL_Rect dstRect = {x, y, static_cast<int>(cw[font] * fontSize), static_cast<int>(ch[font] * fontSize)};

    textureManager.setColorMod(fontID, fontColor);

    stringstream ss(text); string word;

    while (ss >> word)
    {
        int wordWidth = word.length() * static_cast<int>(cw[font] * fontSize);
        int spaceWidth = static_cast<int>(cw[font] * fontSize);

        if(maxWidth > 0 && (dstRect.x + wordWidth + spaceWidth) - x > maxWidth)
        {
            dstRect.x = x;
            dstRect.y += static_cast<int>(ch[font] * fontSize);
        }

        for (char c : word)
        {
            int charIndex = static_cast<int>(c) - 32;
            srcRect.x = (charIndex % cpr[font]) * cw[font];
            srcRect.y = (charIndex / cpr[font]) * ch[font];

            textureManager.drawFrame(fontID, dstRect.x, dstRect.y, cw[font], ch[font], srcRect.x, srcRect.y, srcRect.w, srcRect.h, fontSize, renderer);

            dstRect.x += static_cast<int>(cw[font] * fontSize);
        }

        if(!ss.eof()) // render a space after each word (except the last one)
        {
            int charIndex = static_cast<int>(' ') - 32;
            srcRect.x = (charIndex % cpr[font]) * cw[font];
            srcRect.y = (charIndex / cpr[font]) * ch[font];

            textureManager.drawFrame(fontID, dstRect.x, dstRect.y, cw[font], ch[font], srcRect.x, srcRect.y, srcRect.w, srcRect.h, fontSize, renderer);

            dstRect.x += static_cast<int>(cw[font] * fontSize);
        }
    }
}

void renderShadowedText(int font, const string &text, int x, int y, float fontSize, uint32_t textColor, uint32_t shadowColor, int maxWidth)
{
    renderText(font, text, x+2, y+2, fontSize, shadowColor, maxWidth);
    renderText(font, text, x, y, fontSize, textColor, maxWidth);
}

void renderOutlinedText(int font, const string &text, int x, int y, float fontSize, uint32_t textColor, uint32_t outlineColor, int maxWidth)
{
    renderText(font, text, x+2, y, fontSize, outlineColor, maxWidth);
    renderText(font, text, x-2, y, fontSize, outlineColor, maxWidth);
    renderText(font, text, x, y+2, fontSize, outlineColor, maxWidth);
    renderText(font, text, x, y-2, fontSize, outlineColor, maxWidth);
    renderText(font, text, x, y, fontSize, textColor, maxWidth);
}


void logoutf(const char* format, ...) // Print to log file
{
    FILE* logfile = fopen("log.txt", "a"); // Open log file in append mode

    // Print formatted text
    va_list args;
    va_start(args, format);
    vfprintf(logfile, format, args);
    vfprintf(logfile, "\n", args); //newline
    va_end(args);

    fclose(logfile); // Close log file
}
