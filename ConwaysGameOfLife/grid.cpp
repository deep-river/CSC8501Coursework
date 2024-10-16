#include "grid.h"

Grid::Grid(int rows, int cols) : rows(rows), cols(cols) {
    cells.assign(rows, std::vector<bool>(cols, false));
}

void Grid::initialize(int aliveCount) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rowDist(0, rows - 1);
    std::uniform_int_distribution<> colDist(0, cols - 1);

    int placed = 0;
    while (placed < aliveCount) {
        int x = rowDist(gen);
        int y = colDist(gen);
        if (!cells[x][y]) {
            cells[x][y] = true;
            placed++;
        }
    }

    // Store the initial state
    initialState = cells;
}

void Grid::update() {
    std::vector<std::vector<bool>> newCells(rows, std::vector<bool>(cols, false));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int aliveNeighbors = countAliveNeighbors(i, j);
            if (cells[i][j] && (aliveNeighbors == 2 || aliveNeighbors == 3)) {
                newCells[i][j] = true;  // Survival
            } else if (!cells[i][j] && aliveNeighbors == 3) {
                newCells[i][j] = true;  // Birth
            }
        }
    }

    cells = std::move(newCells);
}

int Grid::countAliveNeighbors(int row, int col) const {
    int aliveNeighbors = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int nx = row + i;
            int ny = col + j;
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                aliveNeighbors += cells[nx][ny];
            }
        }
    }
    return aliveNeighbors;
}

std::string Grid::toString() const {
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

std::string Grid::toString(std::vector<std::vector<bool>> printGrid) const {
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

bool Grid::saveExperiment(const std::string& filename, int currentStep, const std::vector<std::vector<bool>>& initialState) const {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error: Unable to open file for saving." << std::endl;
        return false;
    }

    int aliveCount = 0;
    for (const auto& row : cells) {
        for (bool cell : row) {
            if (cell) aliveCount++;
        }
    }

    outFile << rows << " " << cols << std::endl;
    outFile << aliveCount << std::endl;
    outFile << currentStep << std::endl;

    // Save initial state
    /*outFile << "Initial State:" << std::endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            outFile << (initialState[i][j] ? "O" : ".");
        }
        outFile << std::endl;
    }*/
    outFile << toString(initialState) << std::endl;

    // Save final state
    outFile << "Final State:" << std::endl;
    outFile << toString(cells);

    outFile.close();
    std::cout << "Experiment saved successfully to " << filename << std::endl;
    return true;
}

bool Grid::saveToFile(const std::string& filename, int currentStep) const {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error: Unable to open file for saving." << std::endl;
        return false;
    }

    int aliveCount = 0;
    for (const auto& row : cells) {
        for (bool cell : row) {
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

bool Grid::loadFromFile(const std::string& filename, int& currentStep) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error: Unable to open file for loading." << std::endl;
        return false;
    }

    int fileRows, fileCols, aliveCount;
    inFile >> fileRows >> fileCols >> aliveCount >> currentStep;

    rows =  fileRows;
    cols = fileCols;

    cells.assign(rows, std::vector<bool>(cols, false));

    std::string line;
    std::getline(inFile, line); // Consume the newline
    std::getline(inFile, line); // Skip the top border

    for (int i = 0; i < rows; i++) {
        std::getline(inFile, line);
        for (int j = 0; j < cols; j++) {
            cells[i][j] = (line[2*j+1] == 'O');
        }
    }

    inFile.close();
    std::cout << "Grid loaded successfully from " << filename << std::endl;
    return true;
}