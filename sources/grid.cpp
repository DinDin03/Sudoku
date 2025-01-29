#include "sudoku.h"
#include <fstream>
#include <iostream>
#include <random>

Grid::Grid() {
    grid = std::vector<std::vector<int>>(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
    userGrid = std::vector<std::vector<int>>(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
}

void Grid::SetValue(int row, int col, int value) {
    if (grid[row][col] == 0) { // Allow modification only if it's not a predefined number
        userGrid[row][col] = value;
    }
}

int Grid::GetValue(int row, int col) {
    return (userGrid[row][col] != 0) ? userGrid[row][col] : grid[row][col];
}

void Grid::ClearGrid() {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            userGrid[i][j] = 0;
        }
    }
}

void Grid::SetGrid(const std::vector<std::vector<int>>& newGrid) {
    grid = newGrid;
    userGrid = std::vector<std::vector<int>>(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
}

bool Grid::IsValidPlacement(int row, int col, int num) const {
    for (int i = 0; i < GRID_SIZE; ++i) {
        if (GetValue(row, i) == num || GetValue(i, col) == num) {
            return false;
        }
    }
    
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (GetValue(startRow + i, startCol + j) == num) {
                return false;
            }
        }
    }
    return true;
}

void Grid::CheckViolations() {
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            if (userGrid[row][col] != 0 && !IsValidPlacement(row, col, userGrid[row][col])) {
                std::cout << "Violation at (" << row << ", " << col << ") with value " << userGrid[row][col] << std::endl;
            }
        }
    }
}

void Grid::ShuffleNumbers(std::vector<int>& numbers) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(numbers.begin(), numbers.end(), g);
}

void Grid::GenerateFullGrid() {
    ClearGrid();
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    ShuffleNumbers(numbers);
    for (int i = 0; i < GRID_SIZE; i++) {
        grid[i][(i * 3) % GRID_SIZE] = numbers[i];
    }
    SolveSudoku();
}

void Grid::RemoveNumbers(int difficulty) {
    int attempts = difficulty * 10;
    while (attempts > 0) {
        int row = rand() % GRID_SIZE;
        int col = rand() % GRID_SIZE;
        if (grid[row][col] != 0) {
            grid[row][col] = 0;
            attempts--;
        }
    }
}

void Grid::GenerateSudoku(int difficulty) {
    GenerateFullGrid();
    RemoveNumbers(difficulty);
}

bool Grid::SaveGrid(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    for (const auto& row : grid) {
        for (size_t col = 0; col < row.size(); col++) {
            file << row[col] << (col < row.size() - 1 ? " " : "\n");
        }
    }
    return true;
}

bool Grid::LoadGrid(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    for (auto& row : grid) {
        for (auto& cell : row) {
            file >> cell;
            if (file.fail()) return false;
        }
    }
    return true;
}

bool Grid::SolveSudoku() {
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            if (grid[row][col] == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (IsValidPlacement(row, col, num)) {
                        grid[row][col] = num;
                        if (SolveSudoku()) return true;
                        grid[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}
