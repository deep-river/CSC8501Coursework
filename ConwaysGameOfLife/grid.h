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
    std::string toString(std::vector<std::vector<bool>>) const;
    bool saveToFile(const std::string& filename, int currentStep) const;
    bool saveExperiment(const std::string& filename, int currentStep, const std::vector<std::vector<bool>>& initialState) const;
    bool loadFromFile(const std::string& filename, int& currentStep);
    const std::vector<std::vector<bool>>& getState() const { return cells; }
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    std::vector<std::vector<bool>> getInitialState() const { return initialState; }

private:
    std::vector<std::vector<bool>> cells;
    std::vector<std::vector<bool>> initialState;
    int rows, cols;

    int countAliveNeighbors(int row, int col) const;
};