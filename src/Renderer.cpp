#include "Renderer.h"
#include <cstdint>
#include <cmath>

void draw_line(uint32_t *frame_buffer, int frame_width, int start_x, int start_y, int end_x, int end_y, uint32_t color)
{
    int dx = end_x - start_x;
    int dy = end_y - start_y;

    if (abs(dx) >= abs(dy)) {
        // Step along x
        // y = kx + m
        int x_step = (dx > 0) ? 1 : -1;
        for (int x = start_x; x != end_x; x += x_step)
        {
            float y = (float)dy/(float)dx * (x - start_x) + start_y;
            frame_buffer[(int)round(y) * frame_width + x] = color;
        }
        
    } else {
        // Step along y 
                int y_step = (dy > 0) ? 1 : -1;
        for (int y = start_y; y != end_y; y += y_step)
        {
            float x = (float)dx/(float)dy * (y - start_y) + start_x;
            frame_buffer[y * frame_width + (int)round(x)] = color;
        }
    }

}