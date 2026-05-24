#pragma once
#include "Vec2.h"
#include "Vec3.h"
#include <cstdint>

void draw_line(uint32_t *frame_buffer, int frame_width, Vec2 start, Vec2 end, uint32_t color);

void draw_filled_triangle(uint32_t *frame_buffer, int frame_width, Vec2 p0, Vec2 p1, Vec2 p2, uint32_t c);

void draw_shaded_triangle(uint32_t *frame_buffer, float *depth_buffer, int frame_width, Vec3 v0, Vec3 v1, Vec3 v2, uint32_t c0, uint32_t c1, uint32_t c2);
