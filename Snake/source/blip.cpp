#include "blip.hpp"

#include <stdlib.h>

Blip::Blip(Cell cell)
{
    setCell(cell);
}

void Blip::setCell(Cell cell)
{
    this->cell = this->target_cell = cell;
}

void Blip::setNextTargetsRecursive(Cell target_cell)
{
    this->cell = this->target_cell;

    this->target_cell = target_cell;

    if (this->next)
        this->next->setNextTargetsRecursive(this->cell);
}

void Blip::setNextTargets(Direction next)
{
    Cell target_cell = this->target_cell;

    switch(next) {
    case DIR_LEFT:
        target_cell.column -= 1;
        break;
    case DIR_UP:
        target_cell.row += 1;
        break;
    case DIR_RIGHT:
        target_cell.column += 1;
        break;
    case DIR_DOWN:
    default:
        target_cell.row -= 1;
    }

    setNextTargetsRecursive(target_cell);
}
