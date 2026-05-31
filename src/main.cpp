#include <iostream>
#include <MiniFB.h>
#include <cfloat>

#include "Vec2.h"
#include "Mesh.h"
#include "Renderer.h"
#include "ObjLoader.h"

uint32_t FRAME_WIDTH = 800;
uint32_t FRAME_HEIGHT = 600;

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
    
    for (size_t i = 0; i < obj.f_count; i++)
    {
        float screen_x = (obj.vertices[obj.faces[i].v0].x + 1) / 2 * FRAME_WIDTH;
        float screen_y = (1 - (obj.vertices[obj.faces[i].v0].y + 1) / 2) * FRAME_HEIGHT;
        Vec2 v0 = {screen_x, screen_y};

        screen_x = (obj.vertices[obj.faces[i].v1].x + 1) / 2 * FRAME_WIDTH;
        screen_y = (1 - (obj.vertices[obj.faces[i].v1].y + 1) / 2) * FRAME_HEIGHT;
        Vec2 v1 = {screen_x, screen_y};

        screen_x = (obj.vertices[obj.faces[i].v2].x + 1) / 2 * FRAME_WIDTH;
        screen_y = (1 - (obj.vertices[obj.faces[i].v2].y + 1) / 2) * FRAME_HEIGHT;
        Vec2 v2 = {screen_x, screen_y};
        
        draw_line(frame_buffer, FRAME_WIDTH, FRAME_HEIGHT, v0, v1, 0x00FFFFFF);
        draw_line(frame_buffer, FRAME_WIDTH, FRAME_HEIGHT, v1, v2, 0x00FFFFFF);
        draw_line(frame_buffer, FRAME_WIDTH, FRAME_HEIGHT, v2, v0, 0x00FFFFFF);
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