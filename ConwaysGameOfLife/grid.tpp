#include "grid.h"
#include <random>
#include <fstream>
#include <iostream>

template <typename CellType>
Grid<CellType>::Grid(int rows, int cols) : rows(rows), cols(cols) {
    cells.assign(rows, std::vector<CellType>(cols, CellType()));
}

template <typename CellType>
void Grid<CellType>::initialize(int aliveCount) {
    initialAliveCount = aliveCount;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rowDist(0, rows - 1);
    std::uniform_int_distribution<> colDist(0, cols - 1);

    int placed = 0;
    while (placed < aliveCount) {
        int x = rowDist(gen);
        int y = colDist(gen);
        if (!cells[x][y]) {
            cells[x][y] = CellType(true);
            placed++;
        }
    }
    initialState = cells;
}

template <typename CellType>
void Grid<CellType>::update() {
    std::vector<std::vector<CellType>> newCells(rows, std::vector<CellType>(cols, CellType()));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int aliveNeighbors = countAliveNeighbors(i, j);
            if (cells[i][j] && (aliveNeighbors == 2 || aliveNeighbors == 3)) {
                newCells[i][j] = CellType(true);  // Survival
            } else if (!cells[i][j] && aliveNeighbors == 3) {
                newCells[i][j] = CellType(true);  // Birth
            }
        }
    }
    cells = std::move(newCells);
}

template <typename CellType>
int Grid<CellType>::countAliveNeighbors(int row, int col) const {
    int aliveNeighbors = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int nx = row + i;
            int ny = col + j;
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                aliveNeighbors += cells[nx][ny] ? 1 : 0;
            }
        }
    }
    return aliveNeighbors;
}

template <typename CellType>
std::string Grid<CellType>::toString() const {
    std::string result;
    for (int i = 0; i < cols; i++) {
        result += ". ";
    }
    result += ".\n";

    for (int i = 0; i < rows; i++) {
        result += ".";
        for (int j = 0; j < cols; j++) {
            result += cells[i][j] ? "O." : " .";
        }
        result += "\n";
    }
    return result;
}

template <typename CellType>
std::string Grid<CellType>::toString(const std::vector<std::vector<CellType>>& printGrid) const {
    std::string result;
    for (int i = 0; i < cols; i++) {
        result += ". ";
    }
    result += ".\n";

    for (int i = 0; i < rows; i++) {
        result += ".";
        for (int j = 0; j < cols; j++) {
            result += printGrid[i][j] ? "O." : " .";
        }
        result += "\n";
    }
    return result;
}

template <typename CellType>
bool Grid<CellType>::saveExperiment(const std::string& filename, int currentStep, const std::vector<std::vector<CellType>>& initialState) const {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error: Unable to open file for saving." << std::endl;
        return false;
    }

    int aliveCount = 0;
    for (const auto& row : cells) {
        for (const auto& cell : row) {
            if (cell) aliveCount++;
        }
    }

    outFile << rows << " " << cols << std::endl;
    outFile << initialAliveCount << std::endl;
    outFile << currentStep << std::endl;
    outFile << toString(initialState) << std::endl;

    outFile << "Final State:" << std::endl;
    outFile << toString(cells);

    outFile.close();
    std::cout << "Experiment saved successfully to " << filename << std::endl;
    return true;
}

template <typename CellType>
bool Grid<CellType>::saveToFile(const std::string& filename, int currentStep) const {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error: Unable to open file for saving." << std::endl;
        return false;
    }

    int aliveCount = 0;
    for (const auto& row : cells) {
        for (const auto& cell : row) {
            if (cell) aliveCount++;
        }
    }

    outFile << rows << " " << cols << std::endl;
    outFile << aliveCount << std::endl;
    outFile << currentStep << std::endl;
    outFile << toString();

    outFile.close();
    std::cout << "Grid saved successfully to " << filename << std::endl;
    return true;
}

template <typename CellType>
bool Grid<CellType>::loadFromFile(const std::string& filename, int& currentStep) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error: Unable to open file for loading." << std::endl;
        return false;
    }

    int fileRows, fileCols, aliveCount;
    inFile >> fileRows >> fileCols >> aliveCount >> currentStep;

    rows = fileRows;
    cols = fileCols;

    cells.assign(rows, std::vector<CellType>(cols, CellType()));

    std::string line;
    std::getline(inFile, line); // Consume the newline
    std::getline(inFile, line); // Skip the top border

    for (int i = 0; i < rows; i++) {
        std::getline(inFile, line);
        for (int j = 0; j < cols; j++) {
            cells[i][j] = (line[2*j+1] == 'O') ? CellType(true) : CellType(false);
        }
    }

    inFile.close();
    std::cout << "Grid loaded successfully from " << filename << std::endl;
    return true;
}