#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "grid.h"

using namespace std;

// Function to load pattern from file
bool loadPattern(const string& filename, vector<vector<bool>>& pattern) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return false;
    }

    string line;
    getline(file, line); // Skip the [pattern] line

    int rows, cols;
    file >> rows >> cols;
    file >> ws; // Consume newline

    pattern.clear();
    pattern.resize(rows, vector<bool>(cols, false));

    for (int i = 0; i < rows; ++i) {
        getline(file, line);
        int col = 0;
        for (size_t j = 1; j < line.length(); j += 2) {
            if (col >= cols) break;
            pattern[i][col] = (line[j] == 'O');
            ++col;
        }
    }

    file.close();
    return !pattern.empty();
}

// Function to find pattern in grid
bool findPattern(const Grid& grid, const vector<vector<bool>>& pattern, int& row, int& col) {
    int gridRows = grid.getRows();
    int gridCols = grid.getCols();
    int patternRows = pattern.size();
    int patternCols = pattern[0].size();

    for (int i = 0; i <= gridRows - patternRows; ++i) {
        for (int j = 0; j <= gridCols - patternCols; ++j) {
            bool match = true;
            for (int pi = 0; pi < patternRows && match; ++pi) {
                for (int pj = 0; pj < patternCols && match; ++pj) {
                    if (pattern[pi][pj] != grid.getState()[i + pi][j + pj]) {
                        match = false;
                    }
                }
            }
            if (match) {
                row = i;
                col = j;
                return true;
            }
        }
    }
    return false;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator

    int rows, cols, aliveCount, steps;
    int currentStep = 0;

    // Initial choice: new simulation or load from file
    cout << "Conway's Game of Life Simulation\n";
    cout << "---------------------------------\n";
    cout << "1. Start a new simulation\n";
    cout << "2. Load simulation from file\n";
    cout << "3. Run until find a life pattern\n";
    cout << "Enter your choice (1 - 3): ";
    int choice;
    cin >> choice;

    Grid* grid = nullptr;
    vector<vector<bool>> pattern;

    if (choice == 1) {
        // Input for grid size, number of alive cells, and steps
        cout << "Enter the number of rows (minimum 3): ";
        cin >> rows;
        cout << "Enter the number of columns (minimum 3): ";
        cin >> cols;
        cout << "Enter the number of alive cells: ";
        cin >> aliveCount;
        cout << "Enter the number of steps: ";
        cin >> steps;

        // Validate inputs
        if (rows < 3 || cols < 3) {
            cout << "Grid size must be at least 3x3." << endl;
            return 1;
        }
        if (aliveCount < 0 || aliveCount > rows * cols) {
            cout << "Invalid number of alive cells. Must be between 0 and " << rows * cols << "." << endl;
            return 1;
        }
        if (steps < 0) {
            cout << "Number of steps must be non-negative." << endl;
            return 1;
        }

        // Create and initialize the grid
        grid = new Grid(rows, cols);
        grid->initialize(aliveCount);
        cout << "Grid initialized" << endl;

        // Display initial grid
        cout << "\nInitial grid:\n";
        cout << grid->toString();
    }
    else if (choice == 2) {
        // Load from file
        string filename;
        cout << "Enter the filename to load: ";
        cin >> filename;

        grid = new Grid(1, 1);  // Temporary grid, will be resized when loading
        if (!grid->loadFromFile(filename, currentStep)) {
            delete grid;
            return 1;  // Exit if loading fails
        }

        // Optionally, ask for number of steps to continue
        cout << "Enter the number of additional steps to simulate: ";
        cin >> steps;

        // Display loaded grid
        cout << "\nLoaded grid (Step " << currentStep << "):\n";
        cout << grid->toString();
    }
    else if (choice == 3) {
        cout << "Enter the number of rows (minimum 30): ";
        cin >> rows;
        cout << "Enter the number of columns (minimum 30): ";
        cin >> cols;
        cout << "Enter the number of alive cells (minimum 25): ";
        cin >> aliveCount;

        if (rows < 30 || cols < 30 || aliveCount < 25) {
            cout << "Invalid input. Grid size must be at least 30x30 and alive cells at least 25." << endl;
            return 1;
        }

        grid = new Grid(rows, cols);
        grid->initialize(aliveCount);
        steps = 1000;  // Set a large number of steps for pattern finding

        // Load pattern
        string patternFilename;
        cout << "Enter the filename of the pattern to find: ";
        cin >> patternFilename;
        if (!loadPattern(patternFilename, pattern)) {
            delete grid;
            return 1;  // Exit if pattern loading fails
        }
    }
    else {
        cout << "Invalid choice. Exiting." << endl;
        return 1;
    }

    // Simulation loop
    bool patternFound = false;
    for (int step = 0; step < steps && !patternFound; step++) {
        // Update grid
        grid->update();
        currentStep++;

        // Display current step
        cout << "\nStep " << currentStep << ":\n";
        cout << grid->toString();

        // Check for patterns if in pattern finding mode
        if (choice == 3) {
            int row, col;
            if (findPattern(*grid, pattern, row, col)) {
                cout << "Pattern found at position (" << row << ", " << col << ")" << endl;
                patternFound = true;
                break;
            }
        }

        // Prompt user for action
        if (choice != 3) {
            cout << "\nChoose an action:\n";
            cout << "1. Continue to next step\n";
            cout << "2. Save and exit\n";
            cout << "3. Load from file\n";
            cout << "4. Exit without saving\n";
            cout << "Enter your choice (1-4): ";
            int action;
            cin >> action;

            if (action == 1) {
                continue;  // Proceed to next step
            }
            else if (action == 2) {
                // Save and exit
                string saveFilename;
                cout << "Enter filename to save the current grid: ";
                cin >> saveFilename;
                if (grid->saveToFile(saveFilename, currentStep)) {
                    cout << "Simulation saved. Exiting." << endl;
                    delete grid;
                    return 0;
                }
                else {
                    cout << "Failed to save. Continuing simulation." << endl;
                }
            }
            else if (action == 3) {
                // Load from file
                string loadFilename;
                cout << "Enter filename to load the grid: ";
                cin >> loadFilename;
                Grid* newGrid = new Grid(1, 1);  // Temporary grid, will be resized when loading
                if (newGrid->loadFromFile(loadFilename, currentStep)) {
                    delete grid;
                    grid = newGrid;
                    // Display loaded grid
                    cout << "\nLoaded grid (Step " << currentStep << "):\n";
                    cout << grid->toString();
                    // Adjust remaining steps if desired
                }
                else {
                    cout << "Failed to load. Continuing simulation." << endl;
                    delete newGrid;
                }
            }
            else if (action == 4) {
                // Exit without saving
                cout << "Exiting without saving." << endl;
                delete grid;
                return 0;
            }
            else {
                cout << "Invalid action. Continuing simulation." << endl;
            }
        }
    }

    if (choice == 3 && !patternFound) {
        cout << "Pattern not found within " << steps << " steps." << endl;
    } else if (choice != 3) {
        cout << "\nSimulation completed." << endl;
    }

    delete grid;
    return 0;
}


/*
 * Run in terminal window:
 *
 * cd "/Users/libangyu/Dev/csc8501coursework/conwaysgameoflife"
 *
 * clang++ -std=c++17 -o gameoflife main.cpp
 * or
 * clang++ -std=c++17 -o gameoflife main.cpp grid.cpp
 *
 * ./gameoflife
 */