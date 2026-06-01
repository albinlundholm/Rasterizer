#pragma once

#include "Vec2.h"
#include "Vec3.h"

struct Face
{
    int v0, v1, v2;
};

struct Mesh
{
    Vec3 *vertices;
    int v_count;
    
    Face *faces;
    int f_count;

    Vec2 *UV_coords;
    int UV_count;

    Face *UV_faces;
};
