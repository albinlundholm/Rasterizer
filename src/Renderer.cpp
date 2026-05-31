#include "Renderer.h"
#include <algorithm>
#include <cmath>

void draw_line(uint32_t *frame_buffer, int frame_width, int frame_height, Vec2 start, Vec2 end, uint32_t color)
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
            int px = x;
            int py = (int)round(y);
            if (px < 0 || px >= frame_width || py < 0 || py >= frame_height) continue;
            frame_buffer[(int)round(y) * frame_width + x] = color;
        }
        
    } else {
        // Step along y 
        const int y_step = (dy > 0) ? 1 : -1;
        for (int y = (int)start.y; y != (int)end.y; y += y_step)
        {
            float x = dx/dy * (y - start.y) + start.x;
            int px = (int)round(x);
            int py = y;
            if (px < 0 || px >= frame_width || py < 0 || py >= frame_height) continue;
            frame_buffer[y * frame_width + (int)round(x)] = color;
        }
    }
}

void draw_filled_triangle(uint32_t *frame_buffer, int frame_width, Vec2 p0, Vec2 p1, Vec2 p2, uint32_t c)
{
    // Sort the three vertices by Y (top to bottom): v0, v1, v2
    Vec2 verts[] = {p0, p1, p2};
    std::sort(verts, verts + 3, [](Vec2 a, Vec2 b){ return a.y < b.y; });
    Vec2 v0 = verts[0], v1 = verts[1], v2 = verts[2];

    // Split triangle in two halves at the middle vertex's Y:
    // Top half: from v0.y to v1.y
    // Bottom half: from v1.y to v2.y
    // For each scanline y in each half, compute the X where the left and right edges intersect that row, then draw a horizontal span between them
    // Iterate over top half
    if ((int)v1.y != (int)v0.y) {
        for (int y = (int)v0.y; y <= (int)v1.y; y++){
            // from v0 to v1:
            float x1 = v0.x + (v1.x - v0.x) * (y - v0.y) / (v1.y - v0.y);
            // from v0 to v2:
            float x2 = v0.x + (v2.x - v0.x) * (y - v0.y) / (v2.y - v0.y);

            int x_left  = (int)std::min(x1, x2);
            int x_right = (int)std::max(x1, x2);
            for (int x = x_left; x <= x_right; x++)
            {
                frame_buffer[(size_t)y * frame_width + x] = c;
            }
        }
    }

    // Iterate over bottom half
    if ((int)v2.y != (int)v1.y) {
        for (int y = (int)v1.y; y <= (int)v2.y; y++){
            // from v1 to v2:
            float x1 = v1.x + (v2.x - v1.x) * (y - v1.y) / (v2.y - v1.y);
            // from v0 to v2:
            float x2 = v0.x + (v2.x - v0.x) * (y - v0.y) / (v2.y - v0.y);

            int x_left  = (int)std::min(x1, x2);
            int x_right = (int)std::max(x1, x2);
            for (int x = x_left; x <= x_right; x++)
            {
                frame_buffer[(size_t)y * frame_width + x] = c;
            }
        }
    }
}

void draw_shaded_triangle(uint32_t *frame_buffer, float * depth_buffer,  int frame_width, Vec3 v0, Vec3 v1, Vec3 v2, uint32_t c0, uint32_t c1, uint32_t c2)
{   
    // Get triangle bounding box
    float x_min = std::min({v0.x, v1.x, v2.x});
    float x_max = std::max({v0.x, v1.x, v2.x});
    float y_min = std::min({v0.y, v1.y, v2.y});
    float y_max = std::max({v0.y, v1.y, v2.y});

    // Flatten vectors to 2D
    Vec2 v2_0 = {v0.x, v0.y};
    Vec2 v2_1 = {v1.x, v1.y};
    Vec2 v2_2 = {v2.x, v2.y};

    // Unpack colors to be blended later
    uint8_t r0 = (c0 >> 16) & 0xFF;
    uint8_t g0 = (c0 >> 8)  & 0xFF;
    uint8_t b0 = (c0)       & 0xFF;
    uint8_t r1 = (c1 >> 16) & 0xFF;
    uint8_t g1 = (c1 >> 8)  & 0xFF;
    uint8_t b1 = (c1)       & 0xFF;
    uint8_t r2 = (c2 >> 16) & 0xFF;
    uint8_t g2 = (c2 >> 8)  & 0xFF;
    uint8_t b2 = (c2)       & 0xFF;

    float denom = (v2_1-v2_0).cross(v2_2-v2_0);
    
    for (int y = (int)y_min; y <= (int)y_max; y++)
    {
        for (int x = (int)x_min; x <= (int)x_max; x++)
        {
            // Find barycentric coords for point to interpolate color
            Vec2 P = {(float)x, (float)y};
            float u = (v2_1-v2_0).cross(P-v2_0) / denom;
            float v = (P-v2_0).cross(v2_2-v2_0) / denom;
            float w = 1 - u - v;

            float z = u * v0.z + v * v1.z + w * v2.z;

            if (z < depth_buffer[(size_t)y * frame_width + x])
            {
                if (u >= 0 && v >= 0 && w >= 0){
                    uint8_t r = (uint8_t)(u*r0 + v*r1 + w*r2);
                    uint8_t g = (uint8_t)(u*g0 + v*g1 + w*g2);
                    uint8_t b = (uint8_t)(u*b0 + v*b1 + w*b2);
                    uint32_t blended = (r << 16) | (g << 8) | b;
                    frame_buffer[(size_t)y * frame_width + x] = blended;
                    depth_buffer[(size_t)y * frame_width + x] = z;
                }
            }
        }
    }
}