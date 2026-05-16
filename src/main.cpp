#include <iostream>
#include "Vec2.h"
#include <MiniFB.h>

uint32_t WINDOW_WIDTH = 800;
uint32_t WINDOW_HEIGHT = 600;
char *DISPLAY_NAME = "RASTERIZER";


int main() {
    struct mfb_window *window = mfb_open_ex(DISPLAY_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, MFB_WF_RESIZABLE);

    uint32_t *frame_buffer = (uint32_t *)malloc(WINDOW_WIDTH*WINDOW_HEIGHT*sizeof(uint32_t));

    mfb_update_state state;
    do {
        
        for (size_t i = 0; i < WINDOW_WIDTH*WINDOW_HEIGHT; i++)
        {
            frame_buffer[i] = 0x00FF0000;
        }
        
        state = mfb_update_ex(window, frame_buffer, WINDOW_WIDTH, WINDOW_HEIGHT);

        if (state != MFB_STATE_OK)
            break;

    } while(mfb_wait_sync(window));

    free(frame_buffer);
    frame_buffer = NULL;
    window = NULL;

    return 0;
}