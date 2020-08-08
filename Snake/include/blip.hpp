#pragma once

#include <memory>

#include "utility.hpp"

struct Cell {
    int column, row;

    bool operator==(Cell other) {
        return column == other.column &&
            row == other.row;
    }
} ;

class Blip {
public:
    Blip(Cell cell);

    void setCell(Cell cell);
    void setNextTargets(Direction next);

    Cell target_cell;
    Cell cell;
    std::shared_ptr<Blip> next;

private:
    void setNextTargetsRecursive(Cell target_cell);
};
