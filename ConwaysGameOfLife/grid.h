#pragma once
#include <vector>
#include <string>
#include <random>
#include <iostream>
#include <fstream>

template <typename CellType>
class Grid {
public:
    Grid(int rows, int cols);
    void initialize(int aliveCount);
    void update();
    std::string toString() const;
    std::string toString(const std::vector<std::vector<CellType>>& printGrid) const;
    bool saveToFile(const std::string& filename, int currentStep) const;
    bool saveExperiment(const std::string& filename, int currentStep, const std::vector<std::vector<CellType>>& initialState) const;
    bool loadFromFile(const std::string& filename, int& currentStep);
    const std::vector<std::vector<CellType>>& getState() const { return cells; }
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    std::vector<std::vector<CellType>> getInitialState() const { return initialState; }

private:
    std::vector<std::vector<CellType>> cells;
    std::vector<std::vector<CellType>> initialState;
    int rows, cols;
    int initialAliveCount;
    int countAliveNeighbors(int row, int col) const;
};

#include "grid.tpp"