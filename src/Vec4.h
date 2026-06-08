#pragma once
#include <cmath>

struct Vec4 {
    float x, y, z, w;
    
    float length(){
        return std::sqrt(x*x + y*y + z*z + w*w);
    }

    Vec4 operator+(const Vec4& other) const { return {x + other.x, y + other.y, z + other.z, w + other.w}; }
    Vec4 operator-(const Vec4& other) const { return {x - other.x, y - other.y, z - other.z, w - other.w}; }
};