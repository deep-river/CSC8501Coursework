#pragma once
#include <vector>
#include <string>
#include <random>
#include <iostream>
#include <fstream>

class Grid {
public:
    Grid(int rows, int cols);
    void initialize(int aliveCount);
    void update();
    std::string toString() const;
    bool saveToFile(const std::string& filename, int currentStep) const;
    bool loadFromFile(const std::string& filename, int& currentStep);
    const std::vector<std::vector<bool>>& getState() const { return cells; }
    int getRows() const { return rows; }
    int getCols() const { return cols; }

private:
    std::vector<std::vector<bool>> cells;
    int rows, cols;

    int countAliveNeighbors(int row, int col) const;
};