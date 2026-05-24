#pragma once
#include <cmath>

struct Vec3 {
    float x, y, z;
    
    float length(){
        return std::sqrt(x*x + y*y + z*z);
    }

    Vec3 cross(const Vec3& b) const {
        return { y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x };
    }

    Vec3 operator+(const Vec3& other) const { return {x + other.x, y + other.y, z + other.z}; }
    Vec3 operator-(const Vec3& other) const { return {x - other.x, y - other.y, z - other.z}; }
};