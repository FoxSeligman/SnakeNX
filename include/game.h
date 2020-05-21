#include <switch.h>

typedef enum {
    DIR_LEFT = 0,
    DIR_UP = 1,
    DIR_RIGHT = 2,
    DIR_DOWN = 3
} Direction;

/// Blip structure.
typedef struct Blip {
    u32 target_column;
    u32 target_row;
    u32 column;
    u32 row;
    struct Blip* next;
} Blip;

/// Game settings structure.
typedef struct Settings {
    int columns;
    int rows;
    float speed;    // Seconds per cell
} Settings;

/// Game structure.
typedef struct Game {
    Settings settings;
    bool has_started;
    Blip root_blip;
    Blip loose_blip;
    float progress;
    Direction next_dir;
} Game;

void gameInit(Game* game, u32 columns, u32 rows, float speed);
void gameStart(Game* game, Direction next);
void gameSetNextDir(Game* game, Direction next);
void gameUpdate(Game* game, float delta_time);
void gameCleanup(Game* game);
