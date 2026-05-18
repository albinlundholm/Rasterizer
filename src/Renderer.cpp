#include "Renderer.h"
#include <algorithm>
#include <cmath>

void draw_line(uint32_t *frame_buffer, int frame_width, Vec2 start, Vec2 end, uint32_t color)
{
    const float dx = end.x - start.x;
    const float dy = end.y - start.y;

    if (abs(dx) >= abs(dy)) {
        // Step along x
        // y = kx + m
        const int x_step = (dx > 0) ? 1 : -1;
        for (int x = (int)start.x; x != (int)end.x; x += x_step)
        {
            float y = dy/dx * (x - start.x) + start.y;
            frame_buffer[(int)round(y) * frame_width + x] = color;
        }
        
    } else {
        // Step along y 
        const int y_step = (dy > 0) ? 1 : -1;
        for (int y = (int)start.y; y != (int)end.y; y += y_step)
        {
            float x = dx/dy * (y - start.y) + start.x;
            frame_buffer[y * frame_width + (int)round(x)] = color;
        }
    }
}

void draw_filled_triangle(uint32_t *frame_buffer, int frame_width, Vec2 point_0, Vec2 point_1, Vec2 point_2, uint32_t color)
{
    // Sort the three vertices by Y (top to bottom): v0, v1, v2
    Vec2 verts[] = {point_0, point_1, point_2};
    std::sort(verts, verts + 3, [](Vec2 a, Vec2 b){ return a.y < b.y; });
    Vec2 v0 = verts[0], v1 = verts[1], v2 = verts[2];

    // The triangle naturally splits into two halves at the middle vertex's Y:
    // Top half: from v0.y to v1.y
    // Bottom half: from v1.y to v2.y
    
    // For each scanline y in each half, compute the X where the left and right edges intersect that row, then draw a horizontal span between them
    // Iterate over top half
    for (int y = (int)v0.y; y != (int)v1.y; y++)
    {
        // from v0 to v1:
        float x1 = v0.x + (v1.x - v0.x) * (y - v0.y) / (v1.y - v0.y);
        // from v0 to v2:
        float x2 = v0.x + (v2.x - v0.x) * (y - v0.y) / (v2.y - v0.y);

        int x_left  = (int)std::min(x1, x2);
        int x_right = (int)std::max(x1, x2);
        for (int x = x_left; x <= x_right; x++)
        {
            frame_buffer[(size_t)y * frame_width + x] = color;
        }
    }

    // Iterate over bottom half
    for (int y = (int)v1.y; y != (int)v2.y; y++)
    {
        // from v1 to v2:
        float x1 = v1.x + (v2.x - v1.x) * (y - v1.y) / (v2.y - v1.y);
        // from v0 to v2:
        float x2 = v0.x + (v2.x - v0.x) * (y - v0.y) / (v2.y - v0.y);

        int x_left  = (int)std::min(x1, x2);
        int x_right = (int)std::max(x1, x2);
        for (int x = x_left; x <= x_right; x++)
        {
            frame_buffer[(size_t)y * frame_width + x] = color;
        }
    }
    


}