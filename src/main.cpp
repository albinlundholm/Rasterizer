#include <iostream>
#include <MiniFB.h>

#include "Vec2.h"
#include "Renderer.h"

uint32_t WINDOW_WIDTH = 800;
uint32_t WINDOW_HEIGHT = 600;
char *DISPLAY_NAME = "RASTERIZER";


int main() {
    struct mfb_window *window = mfb_open_ex(DISPLAY_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, MFB_WF_RESIZABLE);

    uint32_t *frame_buffer = (uint32_t *)malloc(WINDOW_WIDTH*WINDOW_HEIGHT*sizeof(uint32_t));

    // Init frame buffer
    uint32_t background_color = 0x00000000;
    for (size_t i = 0; i < WINDOW_WIDTH*WINDOW_HEIGHT; i++)
        {
            /* Gradient
            int x = i % WINDOW_WIDTH;
            int y = i / WINDOW_WIDTH;
            uint8_t R = (x * 255) / WINDOW_WIDTH;
            uint8_t G = (y * 255) / WINDOW_HEIGHT;
            uint8_t B = 0x00;
            color = 0x00000000 | (R << 16) | (G << 8) | B;
            */
            frame_buffer[i] = background_color;
        }
    
    // Some test lines
    // draw_line(frame_buffer, WINDOW_WIDTH, 100, 100, 400, 100, 0x00FFFFFF); // horizontal
    // draw_line(frame_buffer, WINDOW_WIDTH, 200, 50,  200, 400, 0x00FFFFFF); // vertical
    // draw_line(frame_buffer, WINDOW_WIDTH, 50,  50,  300, 300, 0x00FFFFFF); // 45 degrees
    // draw_line(frame_buffer, WINDOW_WIDTH, 50,  200, 700, 250, 0x00FFFFFF); // shallow
    // draw_line(frame_buffer, WINDOW_WIDTH, 600, 100, 650, 500, 0x00FFFFFF); // steep
    // draw_line(frame_buffer, WINDOW_WIDTH, 700, 100, 100, 400, 0x00FFFFFF); // right to left

    draw_line(frame_buffer, WINDOW_WIDTH, 100, 100, 600, 300, 0x00FFFFFF);
    draw_line(frame_buffer, WINDOW_WIDTH, 600, 300, 700, 100, 0x00FFFFFF);
    draw_line(frame_buffer, WINDOW_WIDTH, 700, 100, 100, 100, 0x00FFFFFF);


    mfb_update_state state;
    do {        
        state = mfb_update_ex(window, frame_buffer, WINDOW_WIDTH, WINDOW_HEIGHT);

        if (state != MFB_STATE_OK)
            break;

    } while(mfb_wait_sync(window));

    free(frame_buffer);
    frame_buffer = NULL;
    window = NULL;

    return 0;
}