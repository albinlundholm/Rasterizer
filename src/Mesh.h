#pragma once

#include "Vec3.h"

struct Face
{
    int v0, v1, v2;
};

struct Mesh
{
    Vec3 *vertices;
    Face *faces;
    int v_count;
    int f_count;
};
