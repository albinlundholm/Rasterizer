#pragma once
#include "Vec2.h"
#include <cstdint>

void draw_line(uint32_t *frame_buffer, int frame_width, Vec2 start, Vec2 end, uint32_t color);

void draw_filled_triangle(uint32_t *frame_buffer, int frame_width, Vec2 point_0, Vec2 point_1, Vec2 point_2, uint32_t color);