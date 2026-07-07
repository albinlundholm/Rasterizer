#include <algorithm>
#include <cmath>

#include "Renderer.h"
#include "Texture.h"
#include "Vec2.h"
#include "Vec3.h"

void draw_line(uint32_t *frame_buffer, int frame_width, int frame_height, Vec2 start, Vec2 end, uint32_t color)
{
    const float dx = end.x - start.x;
    const float dy = end.y - start.y;

    if (std::abs(dx) >= std::abs(dy)) {
        // Step along x
        // y = kx + m
        const int x_step = (dx > 0) ? 1 : -1;
        for (int x = (int)start.x; x != (int)end.x; x += x_step)
        {
            float y = dy/dx * (x - start.x) + start.y;
            int px = x;
            int py = (int)round(y);
            if (px < 0 || px >= frame_width || py < 0 || py >= frame_height) continue;
            frame_buffer[py * frame_width + px] = color;
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
            frame_buffer[py * frame_width + px] = color;
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

void draw_shaded_triangle(uint32_t *frame_buffer, float * depth_buffer, int frame_width, int frame_height, Vec3 v0, Vec3 v1, Vec3 v2, uint32_t c0, uint32_t c1, uint32_t c2)
{   
    // Get triangle bounding box
    float x_min = std::min({v0.x, v1.x, v2.x});
    float x_max = std::max({v0.x, v1.x, v2.x});
    float y_min = std::min({v0.y, v1.y, v2.y});
    float y_max = std::max({v0.y, v1.y, v2.y});

    // Clamp to screen
    x_min = std::max(x_min, 0.0f);
    x_max = std::min(x_max, (float)(frame_width - 1));
    y_min = std::max(y_min, 0.0f);
    y_max = std::min(y_max, (float)(frame_height - 1));

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
            // w_i = weight of vertex i (named w_i here since b0..b2 hold the blue channels)
            Vec2 P = {(float)x, (float)y};
            float w1 = (P-v2_0).cross(v2_2-v2_0) / denom;
            float w2 = (v2_1-v2_0).cross(P-v2_0) / denom;
            float w0 = 1 - w1 - w2;

            float z = w0 * v0.z + w1 * v1.z + w2 * v2.z;

            if (z > depth_buffer[(size_t)y * frame_width + x])
            {
                if (w0 >= 0 && w1 >= 0 && w2 >= 0){
                    uint8_t r = (uint8_t)(w0*r0 + w1*r1 + w2*r2);
                    uint8_t g = (uint8_t)(w0*g0 + w1*g1 + w2*g2);
                    uint8_t b = (uint8_t)(w0*b0 + w1*b1 + w2*b2);
                    uint32_t blended = (r << 16) | (g << 8) | b;
                    frame_buffer[(size_t)y * frame_width + x] = blended;
                    depth_buffer[(size_t)y * frame_width + x] = z;
                }
            }
        }
    }
}

void draw_textured_triangle(uint32_t *frame_buffer, float *depth_buffer, int frame_width, int frame_height, Vec3 v0, Vec3 v1, Vec3 v2, Vec2 uv0, Vec2 uv1, Vec2 uv2, Texture texture)
{
    // Get triangle bounding box
    float x_min = std::min({v0.x, v1.x, v2.x});
    float x_max = std::max({v0.x, v1.x, v2.x});
    float y_min = std::min({v0.y, v1.y, v2.y});
    float y_max = std::max({v0.y, v1.y, v2.y});

    // Clamp to screen
    x_min = std::max(x_min, 0.0f);
    x_max = std::min(x_max, (float)(frame_width - 1));
    y_min = std::max(y_min, 0.0f);
    y_max = std::min(y_max, (float)(frame_height - 1));


    // Flatten vectors to 2D
    Vec2 v2_0 = {v0.x, v0.y};
    Vec2 v2_1 = {v1.x, v1.y};
    Vec2 v2_2 = {v2.x, v2.y};

    float denom = (v2_1-v2_0).cross(v2_2-v2_0);
    
    if (denom >= 0) return;

    for (int y = (int)y_min; y <= (int)y_max; y++)
    {
        for (int x = (int)x_min; x <= (int)x_max; x++)
        {
            // Find barycentric coords for point
            // b_i = weight of vertex i = signed area of the sub-triangle opposite vertex i / total area
            Vec2 P = {(float)x, (float)y};
            float b1 = (P-v2_0).cross(v2_2-v2_0) / denom;
            float b2 = (v2_1-v2_0).cross(P-v2_0) / denom;
            float b0 = 1 - b1 - b2;

            float z = b0 * v0.z + b1 * v1.z + b2 * v2.z;

            if (b0 >= 0 && b1 >= 0 && b2 >= 0)
            {
                if (z > depth_buffer[(size_t)y * frame_width + x]){
                    float tex_u = b0 * uv0.x + b1 * uv1.x + b2 * uv2.x;
                    float tex_v = b0 * uv0.y + b1 * uv1.y + b2 * uv2.y;

                    int tx = (int)(tex_u * texture.width);
                    int ty = (int)(tex_v * texture.height);

                    if (tx < 0 || tx >= texture.width || ty < 0 || ty >= texture.height) continue;

                    frame_buffer[(size_t)y * frame_width + x] = texture.pixels[(int)ty * texture.width + (int)tx];
                    depth_buffer[(size_t)y * frame_width + x] = z;
                }
            }
        }
    }
}