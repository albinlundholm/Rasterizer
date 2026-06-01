#pragma once
#include <cstdint>

struct Texture 
{
    int width, height;

    uint32_t *pixels;
};