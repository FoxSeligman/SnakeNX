// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include additional headers
#include <sys/time.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

#include "game.h"

#ifdef DISPLAY_IMAGE
#include "image_bin.h"//Your own raw RGB888 1280x720 image at "data/image.bin" is required.
#endif

// See also libnx display/framebuffer.h.

// Define the desired framebuffer resolution (here we set it to 720p).
#define FB_WIDTH  1280
#define FB_HEIGHT 720

// Remove above and uncomment below for 1080p
//#define FB_WIDTH  1920
//#define FB_HEIGHT 1080

#define GAME_COLUMNS 80
#define GAME_ROWS 45    // FB_HEIGHT / FB_WIDTH * GAME_COLUMNS

#define CELL_SIZE FB_WIDTH / GAME_COLUMNS
#define CELL_SIZE_HALF CELL_SIZE / 2


bool determineDirection(Direction* dir, u64 kDown)
{
    if (kDown & KEY_LSTICK_UP) {
        *dir = DIR_UP;
    } else if (kDown & KEY_LSTICK_RIGHT) {
        *dir = DIR_RIGHT;
    } else if (kDown & KEY_LSTICK_DOWN) {
        *dir = DIR_DOWN;
    } else if (kDown & KEY_LSTICK_LEFT) {
        *dir = DIR_LEFT;
    } else {
        return false;
    }
    return true;
}

// TODO: Not implied that last_timestamp will update
void getDeltaTime(float* out_delta_time, struct timeval* last_timestamp) 
{
    struct timeval timestamp;
    gettimeofday(&timestamp, NULL);

    time_t deltaSeconds = timestamp.tv_sec - last_timestamp->tv_sec;
    suseconds_t deltaUsec = (timestamp.tv_usec - last_timestamp->tv_usec + (deltaSeconds * 1000000));
    
    *out_delta_time = deltaUsec / 1e6;
    *last_timestamp = timestamp;
}

// Main program entrypoint
int main(int argc, char* argv[])
{
    // Retrieve the default window
    NWindow* win = nwindowGetDefault();

    // Create a linear double-buffered framebuffer
    Framebuffer fb;
    framebufferCreate(&fb, win, FB_WIDTH, FB_HEIGHT, PIXEL_FORMAT_RGBA_8888, 2);
    framebufferMakeLinear(&fb);

#ifdef DISPLAY_IMAGE
    u8* imageptr = (u8*)image_bin;
#endif

    // Create new game
    Game game;
    game.settings.speed = 0.05f;
    gameInit(&game);

    // Time
    struct timeval lastTimestamp;
    float deltaTime;

    // Main loop
    while (appletMainLoop())
    {
        // Scan all the inputs. This should be done once for each frame
        hidScanInput();

        // hidKeysDown returns information about which buttons have been
        // just pressed in this frame compared to the previous one
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS)
            break; // break in order to return to hbmenu

        Direction nextDir;
        if (determineDirection(&nextDir, kDown)) {
            if (!game.has_started) {
                gameBegin(&game, nextDir);
            } else {
                gameSetNextDir(&game, nextDir);
            }
        }

        getDeltaTime(&deltaTime, &lastTimestamp);
        gameUpdate(&game, deltaTime);

        // Retrieve the framebuffer
        u32 stride;
        u32* framebuf = (u32*) framebufferBegin(&fb, &stride);

        Blip* blip = &game.root_blip;
        double blip_cell_y = game.progress * (blip->target_row - blip->row) + blip->row;
        double blip_cell_x = game.progress * (blip->target_column - blip->column) + blip->column;

        blip_cell_y = blip->row;
        blip_cell_x = blip->column;

        u32 blip_y = (int)(FB_HEIGHT - blip_cell_y * CELL_SIZE);    // Flip vertically
        u32 blip_x = (int)(blip_cell_x * CELL_SIZE);

        // Black out framebuffer TODO: Quick and dirty
        for (u32 y = 0; y < FB_HEIGHT; y ++)
        {
            for (u32 x = 0; x < FB_WIDTH; x ++)
            {
                u32 pos = y * stride / sizeof(u32) + x;
                framebuf[pos] = 0;
            }
        }

        // Each pixel is 4-bytes due to RGBA8888.
        for (u32 y = blip_y - CELL_SIZE_HALF; y < blip_y + CELL_SIZE_HALF && y >= 0 && y < FB_HEIGHT; y ++)
        {
            for (u32 x = blip_x - CELL_SIZE_HALF; x < blip_x + CELL_SIZE_HALF && x >= 0 && x < FB_WIDTH; x ++)
            {
                u32 pos = y * stride / sizeof(u32) + x;
#ifdef DISPLAY_IMAGE
                framebuf[pos] = RGBA8_MAXALPHA(imageptr[pos*3+0]+(cnt*4), imageptr[pos*3+1], imageptr[pos*3+2]);
#else
                framebuf[pos] = 0xFFFFFFFF;//Set framebuf to white
#endif
            }
        }

        // We're done rendering, so we end the frame here.
        framebufferEnd(&fb);
    }

    framebufferClose(&fb);

    return 0;
}
