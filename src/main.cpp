#include <iostream>
#include <MiniFB.h>
#include <cfloat>

#include "Vec2.h"
#include "Renderer.h"

uint32_t WINDOW_WIDTH = 800;
uint32_t WINDOW_HEIGHT = 600;

uint32_t BACKGROUND_COLOR = 0x00000000;

char *DISPLAY_NAME = "RASTERIZER";


int main() {
    struct mfb_window *window = mfb_open_ex(DISPLAY_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, MFB_WF_RESIZABLE);

    // Init frame buffer
    uint32_t *frame_buffer = (uint32_t *)malloc(WINDOW_WIDTH*WINDOW_HEIGHT*sizeof(uint32_t)); 
    for (size_t i = 0; i < WINDOW_WIDTH*WINDOW_HEIGHT; i++)
        {
            frame_buffer[i] = BACKGROUND_COLOR;
        }

    // Init depth buffer
    float *depth_buffer = (float *)malloc(WINDOW_WIDTH*WINDOW_HEIGHT*sizeof(float));
    for (size_t i = 0; i < WINDOW_WIDTH*WINDOW_HEIGHT; i++)
        {
            depth_buffer[i] = FLT_MAX;
        }

    // Test interpolated triangle and z buffer
    draw_shaded_triangle(frame_buffer, depth_buffer, WINDOW_WIDTH, {250, 150, 1}, {550, 150, 1}, {400, 450, 1}, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF);
    draw_shaded_triangle(frame_buffer, depth_buffer, WINDOW_WIDTH, {400, 100, 0}, {100, 500, 0}, {700, 300, 0}, 0x00FF0000, 0x0000FF00, 0x000000FF);

    // Main loop
    mfb_update_state state;
    do {        
        state = mfb_update_ex(window, frame_buffer, WINDOW_WIDTH, WINDOW_HEIGHT);

        if (state != MFB_STATE_OK)
            break;

    } while(mfb_wait_sync(window));

    // Cleanup
    free(frame_buffer);
    frame_buffer = NULL;
    window = NULL;

    return 0;
}