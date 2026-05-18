#include <iostream>
#include <MiniFB.h>

#include "Vec2.h"
#include "Renderer.h"

uint32_t WINDOW_WIDTH = 800;
uint32_t WINDOW_HEIGHT = 600;
char *DISPLAY_NAME = "RASTERIZER";


int main() {
    struct mfb_window *window = mfb_open_ex(DISPLAY_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, MFB_WF_RESIZABLE);

    // Init frame buffer
    uint32_t *frame_buffer = (uint32_t *)malloc(WINDOW_WIDTH*WINDOW_HEIGHT*sizeof(uint32_t));
    uint32_t background_color = 0x00000000;
    for (size_t i = 0; i < WINDOW_WIDTH*WINDOW_HEIGHT; i++)
        {
            frame_buffer[i] = background_color;
        }

    // Testing drawing filled triangle
    // draw_filled_triangle(frame_buffer, WINDOW_WIDTH, {100, 50},  {300, 250}, {50,  250}, 0x00FF0000); // red
    // draw_filled_triangle(frame_buffer, WINDOW_WIDTH, {350, 50},  {550, 250}, {400, 250}, 0x0000FF00); // green
    // draw_filled_triangle(frame_buffer, WINDOW_WIDTH, {600, 50},  {780, 250}, {620, 250}, 0x000000FF); // blue
    // draw_filled_triangle(frame_buffer, WINDOW_WIDTH, {100, 350}, {300, 550}, {200, 300}, 0x00FFFF00); // yellow
    // draw_filled_triangle(frame_buffer, WINDOW_WIDTH, {400, 300}, {600, 500}, {500, 300}, 0x00FF00FF); // magenta

    // Test interpolated triangle
    draw_shaded_triangle(frame_buffer, WINDOW_WIDTH, {400, 100}, {100, 500}, {700, 300}, 0x00FF0000, 0x0000FF00, 0x000000FF);

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