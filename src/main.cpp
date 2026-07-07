#include <MiniFB.h>
#include <cfloat>
#include <cstdio>

#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Mat4.h"
#include "Mesh.h"
#include "Renderer.h"
#include "ObjLoader.h"
#include "TgaLoader.h"

uint32_t FRAME_WIDTH = 1920;
uint32_t FRAME_HEIGHT = 1080;

uint32_t BACKGROUND_COLOR = 0x00000000;

const char *DISPLAY_NAME = "RASTERIZER";


int main(int arg_c, char **arg_v) {
    struct mfb_window *window = mfb_open_ex(DISPLAY_NAME, FRAME_WIDTH, FRAME_HEIGHT, MFB_WF_RESIZABLE);

    // Init frame buffer
    uint32_t *frame_buffer = (uint32_t *)malloc(FRAME_WIDTH*FRAME_HEIGHT*sizeof(uint32_t)); 
    for (size_t i = 0; i < FRAME_WIDTH*FRAME_HEIGHT; i++)
        {
            frame_buffer[i] = BACKGROUND_COLOR;
        }

    // Init depth buffer
    float *depth_buffer = (float *)malloc(FRAME_WIDTH*FRAME_HEIGHT*sizeof(float));
    for (size_t i = 0; i < FRAME_WIDTH*FRAME_HEIGHT; i++)
        {
            depth_buffer[i] = -FLT_MAX;
        }

    // Test load and draw obj wireframe
    Mesh obj = load_obj(arg_v[1]);
    Texture tex;

    try {
            tex = load_tga(arg_v[2]);
        } catch (std::exception& e) {
            printf("%s\n", e.what());
        }

        float angle = -(3.14159f / 4.0f); //45 degrees as radians
        Mat4 model = Mat4::rotation_y(angle) * Mat4::rotation_z(angle/2) * Mat4::rotation_x(-angle/2);
        Mat4 proj = Mat4::perspective(6.0f);
        Mat4 viewport = Mat4::viewport(FRAME_WIDTH, FRAME_HEIGHT);
        Mat4 transform = viewport * proj * model;

    for (size_t i = 0; i < obj.f_count; i++)
    {
        auto project = [&](Vec3 v) -> Vec3 {
            float scale = (1 / (1 - v.z / 3));
            float sx = (v.x * scale + 1) / 2 * FRAME_WIDTH;
            float sy = (1 - (v.y * scale + 1) / 2) * FRAME_HEIGHT;
            return {sx, sy, -v.z};
        };

        Vec4 a4 = transform * Vec4{obj.vertices[obj.faces[i].v0].x, obj.vertices[obj.faces[i].v0].y, obj.vertices[obj.faces[i].v0].z, 1};
        Vec4 b4 = transform * Vec4{obj.vertices[obj.faces[i].v1].x, obj.vertices[obj.faces[i].v1].y, obj.vertices[obj.faces[i].v1].z, 1};
        Vec4 c4 = transform * Vec4{obj.vertices[obj.faces[i].v2].x, obj.vertices[obj.faces[i].v2].y, obj.vertices[obj.faces[i].v2].z, 1};

        Vec3 v0 = {a4.x/a4.w, a4.y/a4.w, a4.z/a4.w};
        Vec3 v1 = {b4.x/b4.w, b4.y/b4.w, b4.z/b4.w};
        Vec3 v2 = {c4.x/c4.w, c4.y/c4.w, c4.z/c4.w};

        Vec2 uv0 = obj.UV_coords[obj.UV_faces[i].v0];
        Vec2 uv1 = obj.UV_coords[obj.UV_faces[i].v1];
        Vec2 uv2 = obj.UV_coords[obj.UV_faces[i].v2];

        draw_textured_triangle(frame_buffer, depth_buffer, FRAME_WIDTH, FRAME_HEIGHT, v0, v1, v2, uv0, uv1, uv2, tex);
    }
    
    // Main loop
    mfb_update_state state;
    do {        
        state = mfb_update_ex(window, frame_buffer, FRAME_WIDTH, FRAME_HEIGHT);

        if (state != MFB_STATE_OK)
            break;

    } while(mfb_wait_sync(window));

    // Cleanup
    free(frame_buffer);
    frame_buffer = NULL;
    window = NULL;

    return 0;
}