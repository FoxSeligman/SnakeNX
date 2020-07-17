#include <unistd.h>

#include <switch.h>

#include <antimony>

#include "snake_game.hpp"

#define GAME_COLUMNS 40
#define GAME_ROWS 22    // SCREEN_H / SCREEN_W * GAME_COLUMNS

// Main program entrypoint
int main(int argc, char** argv) {
    socketInitializeDefault();              // Initialize sockets
    nxlinkStdio();                          // Redirect stdout and stderr over the network to nxlink

    // Initialize system
    atmy::System::Init();
    SnakeGame* game = new SnakeGame(GAME_COLUMNS, GAME_ROWS, 0.05f);

    // Loop until exit requested
    while(game->Update());

    // Clean up system
    delete game;
    atmy::System::Terminate();

    socketExit();                           // Cleanup

    return 0;
}
