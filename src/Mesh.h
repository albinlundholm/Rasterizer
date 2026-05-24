#pragma once

#include "Vec3.h"

struct Face
{
    int x, y, z;
};

struct Mesh
{
    Vec3 *vertices;
    Face *faces;
    int v_count;
    int f_count;
};
