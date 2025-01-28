#ifndef GRID_H
#define GRID_H

#include "raylib.h"
#include <vector>

#define GRID_SIZE 9
#define CELL_SIZE 100

class Grid {
private:
    std::vector<std::vector<int>> grid;
public:
    Grid();
    void SetValue(int row, int col, int value);
    void Draw();
    void ClearGrid();
};

#endif
