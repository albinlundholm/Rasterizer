#include <iostream>
#include <MiniFB.h>
#include <cfloat>

#include "Vec2.h"
#include "Mesh.h"
#include "Renderer.h"
#include "ObjLoader.h"
#include "TgaLoader.h"

uint32_t FRAME_WIDTH = 1920;
uint32_t FRAME_HEIGHT = 1080;

uint32_t BACKGROUND_COLOR = 0x00000000;

char *DISPLAY_NAME = "RASTERIZER";


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
            depth_buffer[i] = FLT_MAX;
        }

    // Test load and draw obj wireframe
    Mesh obj = load_obj(arg_v[1]);
    Texture tex;

    try {
            tex = load_tga(arg_v[2]);
        } catch (std::exception& e) {
            printf("%s\n", e.what());
        }

    for (size_t i = 0; i < obj.f_count; i++)
    {
        auto project = [&](Vec3 v) -> Vec3 {
            float sx = (v.x + 1) / 2 * FRAME_WIDTH;
            float sy = (1 - (v.y + 1) / 2) * FRAME_HEIGHT;
            return {sx, sy, -v.z};
        };

        Vec3 a = obj.vertices[obj.faces[i].v0];
        Vec3 b = obj.vertices[obj.faces[i].v1];
        Vec3 c = obj.vertices[obj.faces[i].v2];

        Vec3 v0 = project(a);
        Vec3 v1 = project(b);
        Vec3 v2 = project(c);

        Vec2 uv0 = obj.UV_coords[obj.UV_faces[i].v0];
        Vec2 uv1 = obj.UV_coords[obj.UV_faces[i].v1];
        Vec2 uv2 = obj.UV_coords[obj.UV_faces[i].v2];
        
        // Switched input order of vertices to get correct vertex winding because screen space Y is pointing down, mirroring winding
        // Revert to original order when viewport transform is implemented, where we'll flip Y
        draw_textured_triangle(frame_buffer, depth_buffer, FRAME_WIDTH, FRAME_HEIGHT, v2, v1, v0, uv0, uv1, uv2, tex);
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