#pragma once
#include <cmath>
#include "Vec4.h"

struct Mat4 {
    float m[4][4];

    static Mat4 identity()
    {
        return 
        {{
            {1,0,0,0},
            {0,1,0,0},
            {0,0,1,0},
            {0,0,0,1}
        }};
    }

    static Mat4 rotation_x(float angle)
    {
        return {{
            { 1,           0,           0,  0 },
            { 0,  cos(angle), -sin(angle),  0 },
            { 0,  sin(angle),  cos(angle),  0 },
            { 0,           0,           0,  1 }
        }};
    }

    static Mat4 rotation_y(float angle)
    {
        return {{
            {  cos(angle),  0,  sin(angle),  0 },
            {           0,  1,           0,  0 },
            { -sin(angle),  0,  cos(angle),  0 },
            {           0,  0,           0,  1 }
        }};
    }

    static Mat4 rotation_z(float angle)
    {
        return {{
            {  cos(angle), -sin(angle),  0,  0 },
            {  sin(angle),  cos(angle),  0,  0 },
            {           0,           0,  1,  0 },
            {           0,           0,  0,  1 }
        }};
    }

    Mat4 operator*(const Mat4& other) const 
    {
        Mat4 result = {};
        for (size_t i = 0; i < 4; i++)
        {
            for (size_t j = 0; j < 4; j++)
            {
                result.m[i][j] = m[i][0]*other.m[0][j] + m[i][1]*other.m[1][j] + m[i][2]*other.m[2][j] + m[i][3]*other.m[3][j];
            }
        }
        return result;
    }

    Vec4 operator*(const Vec4& v) const 
    {
        Vec4 result = {};
        result.x = m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z + m[0][3]*v.w;
        result.y = m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z + m[1][3]*v.w;
        result.z = m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z + m[2][3]*v.w;
        result.w = m[3][0]*v.x + m[3][1]*v.y + m[3][2]*v.z + m[3][3]*v.w;
        return result;
    }
};