#pragma once
#include <cmath>

struct Vec2 {
    float x, y;
    
    float length() {
        return std::sqrt(x*x + y*y);
    }
};