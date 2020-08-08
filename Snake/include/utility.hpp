#pragma once

typedef enum {
    DIR_LEFT = 0,
    DIR_UP = 1,
    DIR_RIGHT = 2,
    DIR_DOWN = 3
} Direction;

/// Game settings structure.
typedef struct Settings {
    int columns;
    int rows;
    float speed;    // Seconds per cell
} Settings;
