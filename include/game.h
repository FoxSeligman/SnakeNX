#include <switch.h>

typedef enum {
    DIR_UP = 0,
    DIR_RIGHT = 1,
    DIR_DOWN = 2,
    DIR_LEFT = 3
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
    float speed;    // Seconds per cell
} Settings;

/// Game structure.
typedef struct Game {
    Settings settings;
    bool has_started;
    Blip root_blip;
    float progress;
    Direction next_dir;
} Game;

void gameInit(Game* game);
void gameBegin(Game* game, Direction next);
void gameSetNextDir(Game* game, Direction next);
void gameUpdate(Game* game, float delta_time);
