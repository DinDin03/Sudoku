#include "grid.h"
#include <fstream>
#include <iostream>
#include <random>

Grid::Grid() : grid(GRID_SIZE, std::vector<int>(GRID_SIZE, 0)) {}

void Grid::SetValue(int row, int col, int value) {
    grid[row][col] = value;
}

int Grid::GetValue(int row, int col) {
    return grid[row][col];
}

void Grid::Draw() {
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            DrawRectangleLines(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
            if (grid[row][col] != 0) {
                DrawText(TextFormat("%d", grid[row][col]), col * CELL_SIZE + 35, row * CELL_SIZE + 35, 50, BLACK);
            }
        }
    }
    for (int i = 0; i <= GRID_SIZE; i += 3) {
        DrawLineEx({0, static_cast<float>(i * CELL_SIZE)}, {static_cast<float>(GRID_SIZE * CELL_SIZE), static_cast<float>(i * CELL_SIZE)}, 5.0f, BLACK);
        DrawLineEx({static_cast<float>(i * CELL_SIZE), 0}, {static_cast<float>(i * CELL_SIZE), static_cast<float>(GRID_SIZE * CELL_SIZE)}, 5.0f, BLACK);
    }
}

void Grid::ClearGrid() {
    for (auto& row : grid) {
        std::fill(row.begin(), row.end(), 0);
    }
}

void Grid::SetGrid(const std::vector<std::vector<int>>& newGrid) {
    grid = newGrid;
}

bool Grid::IsValidPlacement(int row, int col, int num) const {
    for (int x = 0; x < GRID_SIZE; x++) {
        if (grid[row][x] == num || grid[x][col] == num) return false;
    }
    int startRow = (row / 3) * 3, startCol = (col / 3) * 3;
    for (int r = startRow; r < startRow + 3; r++) {
        for (int c = startCol; c < startCol + 3; c++) {
            if (grid[r][c] == num) return false;
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
            int backup = grid[row][col];
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