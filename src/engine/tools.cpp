#include "tools.h"

uint32_t divideHexColor(uint32_t hex_color, float divisor)
{
    uint8_t r = (hex_color >> 16) & 0xFF;
    uint8_t g = (hex_color >> 8) & 0xFF;
    uint8_t b = hex_color & 0xFF;

    uint8_t new_r = static_cast<uint8_t>(r / divisor);
    uint8_t new_g = static_cast<uint8_t>(g / divisor);
    uint8_t new_b = static_cast<uint8_t>(b / divisor);

    return (new_r << 16) | (new_g << 8) | new_b;
}
