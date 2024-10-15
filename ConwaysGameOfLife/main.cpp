#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include "grid.h"

using namespace std;


// Function to rotate a pattern 90 degrees clockwise
vector<vector<bool>> rotatePattern(const vector<vector<bool>>& pattern) {
    int rows = pattern.size();
    int cols = pattern[0].size();
    vector<vector<bool>> rotated(cols, vector<bool>(rows, false));

    // Find the bounding box of the live cells
    int minRow = rows, maxRow = 0, minCol = cols, maxCol = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (pattern[i][j]) {
                minRow = min(minRow, i);
                maxRow = max(maxRow, i);
                minCol = min(minCol, j);
                maxCol = max(maxCol, j);
            }
        }
    }

    // Add padding around the bounding box
    minRow = max(0, minRow - 1);
    maxRow = min(rows - 1, maxRow + 1);
    minCol = max(0, minCol - 1);
    maxCol = min(cols - 1, maxCol + 1);

    // Rotate the pattern within the padded bounding box
    for (int i = minRow; i <= maxRow; ++i) {
        for (int j = minCol; j <= maxCol; ++j) {
            int newCol = rows - 1 - i;
            int newRow = j;
            rotated[newRow][newCol] = pattern[i][j];
        }
    }

    return rotated;
}

// todo: 用operator overloading重写方法
// Function to check if two patterns are different
bool patternsAreDifferent(const vector<vector<bool>>& p1, const vector<vector<bool>>& p2) {
    if (p1.size() != p2.size() || p1[0].size() != p2[0].size()) return true;
    for (size_t i = 0; i < p1.size(); ++i) {
        for (size_t j = 0; j < p1[0].size(); ++j) {
            if (p1[i][j] != p2[i][j]) return true;
        }
    }
    return false;
}

// Function to load pattern from file
bool loadPattern(const string& filename, vector<vector<vector<bool>>>& patterns) {
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

    int steps;
    file >> steps;
    file >> ws; // Consume newline

    int sequenceIndex;
    file >> sequenceIndex;
    file >> ws; // Consume newline

    vector<vector<bool>> pattern(rows, vector<bool>(cols, false));

    for (int i = 0; i < rows; ++i) {
        getline(file, line);
        for (int j = 0; j < cols; ++j) {
            pattern[i][j] = (line[j * 2 + 1] == 'O');
        }
    }

    file.close();

    // Add original pattern
    patterns.push_back(pattern);

    // Add rotated patterns
    for (int i = 1; i <= 3; ++i) {
        vector<vector<bool>> rotated = rotatePattern(patterns.back());
        if (patternsAreDifferent(rotated, pattern)) {
            patterns.push_back(rotated);
        }
    }

    return !patterns.empty();
}

// Function to find pattern in grid
bool findPattern(const Grid& grid, const vector<vector<vector<bool>>>& patterns, int& row, int& col, int& patternIndex) {
    int gridRows = grid.getRows();
    int gridCols = grid.getCols();

    for (size_t p = 0; p < patterns.size(); ++p) {
        const auto& pattern = patterns[p];
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
                    patternIndex = p;
                    return true;
                }
            }
        }
    }
    return false;
}

// Function to get current date and time as string
string getCurrentDateTime() {
    auto now = chrono::system_clock::now();
    auto in_time_t = chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(localtime(&in_time_t), "%Y%m%d-%H%M%S");
    return ss.str();
}

// Function to save experiment to file
bool saveExperiment(const Grid& grid, const string& patternName, int currentStep) {
    string filename = getCurrentDateTime() + "-" + patternName + "-" + to_string(currentStep) + ".txt";
    return grid.saveToFile(filename, currentStep);
}


// todo: 记录初始grid的state或随机种子，在发现匹配pattern后保存文件时记录初始状态和最终状态，以便实验重现
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
    vector<vector<vector<bool>>> patterns;
    string patternName;

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

        steps = 1000;  // Set a large number of steps for pattern finding

        // Load pattern
        string patternFilename;
        cout << "Enter the filename of the pattern to find: ";
        cin >> patternFilename;
        if (!loadPattern(patternFilename, patterns)) {
            return 1;  // Exit if pattern loading fails
        }

        // Extract pattern name from filename
        size_t lastSlash = patternFilename.find_last_of("/\\");
        size_t lastDot = patternFilename.find_last_of(".");
        patternName = patternFilename.substr(lastSlash + 1, lastDot - lastSlash - 1);

        bool patternFound = false;
        int experimentCount = 0;

        while (!patternFound) {
            experimentCount++;
            cout << "\nStarting experiment #" << experimentCount << endl;

            grid = new Grid(rows, cols);
            grid->initialize(aliveCount);
            currentStep = 0;

            // Output initial grid
            cout << "\nInitial grid for experiment #" << experimentCount << ":\n";
            cout << grid->toString();

            // Simulation loop
            for (int step = 0; step < steps && !patternFound; step++) {
                // Update grid
                grid->update();
                currentStep++;

                // Output current grid
                cout << "\nStep " << currentStep << " for experiment #" << experimentCount << ":\n";
                cout << grid->toString();

                // Check for patterns
                int row, col, patternIndex;
                if (findPattern(*grid, patterns, row, col, patternIndex)) {
                    cout << "Pattern found at position (" << row << ", " << col << ")" << endl;
                    cout << "Rotation: " << patternIndex * 90 << " degrees" << endl;
                    cout << "Found in experiment #" << experimentCount << " at step " << currentStep << endl;
                    patternFound = true;

                    // Ask user if they want to save the experiment
                    char saveChoice;
                    cout << "Do you want to save this experiment? (y/n): ";
                    cin >> saveChoice;
                    if (saveChoice == 'y' || saveChoice == 'Y') {
                        if (saveExperiment(*grid, patternName, currentStep)) {
                            cout << "Experiment saved successfully." << endl;
                        } else {
                            cout << "Failed to save experiment." << endl;
                        }
                    }
                }
            }

            if (!patternFound) {
                cout << "Pattern not found in experiment #" << experimentCount << endl;
                // Clear the screen (note: this is platform-dependent)
                #ifdef _WIN32
                system("cls");
                #else
                system("clear");
                #endif
            }

            delete grid;
        }
    }
    else {
        cout << "Invalid choice. Exiting." << endl;
        return 1;
    }

    // Simulation loop for choices 1 and 2
    if (choice == 1 || choice == 2) {
        for (int step = 0; step < steps; step++) {
            // Update grid
            grid->update();
            currentStep++;

            // Display current step
            cout << "\nStep " << currentStep << ":\n";
            cout << grid->toString();

            // Prompt user for action
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
                    cout << "Enter the number of additional steps to simulate: ";
                    cin >> steps;
                    step = 0;  // Reset step counter
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

        cout << "\nSimulation completed." << endl;
        delete grid;
    }

    return 0;
}