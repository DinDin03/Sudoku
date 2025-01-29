#ifndef GRID_H
#define GRID_H

#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include "raylib.h"

const int GRID_SIZE = 9; // Define the grid size
const int CELL_SIZE = 100; // Define each cell's size in pixels

class Grid {
private:
    std::vector<std::vector<int>> grid;

public:
    Grid();
    void SetValue(int row, int col, int value);
    int GetValue(int row, int col);
    void ClearGrid();
    void SetGrid(const std::vector<std::vector<int>>& newGrid);
    bool IsValidPlacement(int row, int col, int num) const;
    void Draw();
    bool SaveGrid(const std::string& filename) const;
    bool LoadGrid(const std::string& filename);
    void GenerateSudoku(int difficulty);
    bool SolveSudoku();
    void ShuffleNumbers(std::vector<int>& numbers);
    void GenerateFullGrid();
    void RemoveNumbers(int difficulty);
    bool HasUniqueSolution();
};

#endif