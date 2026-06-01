#include "TgaLoader.h"
#include "Texture.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

Texture load_tga(char *path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error(std::string("could not open file: ") + path);
    }


    // Byte 2 = image type
    file.seekg(2);
    uint8_t img_type = file.get();
    if (img_type != 2 && img_type != 10){
        throw std::runtime_error("unsupported TGA format");
    }

    // Byte 12-13 = width
    file.seekg(12);
    uint8_t lo = file.get();
    uint8_t hi = file.get();
    uint16_t width = lo | (hi << 8);

    // Byte 14-15 = height
    lo = file.get();
    hi = file.get();
    uint16_t height = lo | (hi << 8);

    // Byte 16 = bits per pixel
    uint8_t bits_per_pixel = file.get();

    // Byte 17 = image descriptor
    uint8_t img_desc = file.get();

    // From byte 18: raw pixel data
    uint32_t *pixels = (uint32_t *)malloc(width * height * sizeof(uint32_t));

    if (img_type == 2)
    {
        for (int i = 0; i < (width * height); i++)
        {
            uint8_t b = file.get();
            uint8_t g = file.get();
            uint8_t r = file.get();
            uint32_t pixel = (r << 16) | (g << 8) | b;

            if (bits_per_pixel == 32)
            {
                uint8_t a = file.get();
                pixel = (a << 24) | (r << 16) | (g << 8) | b;
            }

            pixels[i] = pixel;
        }
    }
    if (img_type == 10)
    {
        int i = 0;

        while (i < width * height)
        {
            uint8_t header = file.get();
            uint8_t count = (header & 0x7F) + 1; // count is lower 7 bits of header. Add 1 to  get total count

            // Bit 8 = 1: run. Repeat next pixel count times 
            if (header & 0x80)
            {
                uint8_t b = file.get();
                uint8_t g = file.get();
                uint8_t r = file.get();
                uint32_t pixel = (r << 16) | (g << 8) | b;

                if (bits_per_pixel == 32)
                {
                    uint8_t a = file.get();
                    pixel = (a << 24) | (r << 16) | (g << 8) | b;
                }

                for (int p = 0; p < count; p++)
                {
                    pixels[i + p] = pixel;
                }
            } else 
            {
                // Bit 8 = 2: raw. Read pixels individually
                for (int p = 0; p < count; p++)
                {
                    uint8_t b = file.get();
                    uint8_t g = file.get();
                    uint8_t r = file.get();
                    uint32_t pixel = (r << 16) | (g << 8) | b;

                    if (bits_per_pixel == 32)
                    {
                        uint8_t a = file.get();
                        pixel = (a << 24) | (r << 16) | (g << 8) | b;
                    }

                    pixels[i + p] = pixel;
                }
            }
            i += count;
        }
        
    }
    
    file.close();

    return {width, height, pixels};
}