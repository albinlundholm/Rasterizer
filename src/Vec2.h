#pragma once
#include <cmath>

struct Vec2 {
    float x, y;
    
    float length(){
        return std::sqrt(x*x + y*y);
    }

    float cross(const Vec2& b) const {
        return x*b.y - y*b.x;
    }

    Vec2 operator+(const Vec2& other) const { return {x + other.x, y + other.y}; }
    Vec2 operator-(const Vec2& other) const { return {x - other.x, y - other.y}; }
};