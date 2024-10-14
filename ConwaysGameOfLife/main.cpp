#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;


/* todo:
 * 1. 尝试用类或者面向对象(类，继承，多态，模版)的形式存储grid
 * 2. 用类表示grid
 * 3. 分析并优化updateGrid()的效率
 * 4. 添加二叉搜索树
 * 5. 实现自动化和pattern识别算法
 * 6. 每个方法不超过15行
 */


// Function to display the grid with each cell represented as a space surrounded by four dots
// displayGrid额外处理了显示输出的逻辑，Grid中的数据存储无需关注输出的格式
void displayGrid(const vector<vector<bool>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();

    for (int i = 0; i < cols; i++) {
        cout << ". ";
    }
    cout << "." << endl;

    for (int i = 0; i < rows; i++) {
        cout << ".";
        for (int j = 0; j < cols; j++) {
            cout << (grid[i][j] ? "O." : " .");
        }
        cout << endl;
    }
}

// Function to count the alive neighbors of a cell
int countAliveNeighbors(const vector<vector<bool>>& grid, int x, int y) {
    int aliveNeighbors = 0;
    int rows = grid.size();
    int cols = grid[0].size();

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;  // Skip the cell itself
            int nx = x + i;
            int ny = y + j;
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                aliveNeighbors += grid[nx][ny];
            }
        }
    }
    return aliveNeighbors;
}

// Function to update the grid
vector<vector<bool>> updateGrid(const vector<vector<bool>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    vector<vector<bool>> newGrid(rows, vector<bool>(cols, false));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int aliveNeighbors = countAliveNeighbors(grid, i, j);
            if (grid[i][j] && (aliveNeighbors == 2 || aliveNeighbors == 3)) {
                newGrid[i][j] = true;  // Survival
            } else if (!grid[i][j] && aliveNeighbors == 3) {
                newGrid[i][j] = true;  // Birth
            } else {
                newGrid[i][j] = false;  // Death
            }
        }
    }
    return newGrid;
}

// Function to initialize the grid with random alive cells
void initializeGrid(vector<vector<bool>>& grid, int aliveCount) {
    const int rows = grid.size();
    const int cols = grid[0].size();
    int placed = 0;

    while (placed < aliveCount) {
        int x = rand() % rows;
        int y = rand() % cols;
        if (!grid[x][y]) {
            grid[x][y] = true;
            placed++;
        }
    }
}

// Function to save the current grid state to a file
bool saveGridToFile(const string& filename, const vector<vector<bool>>& grid, int currentStep) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error: Unable to open file for saving." << endl;
        return false;
    }

    int rows = grid.size();
    int cols = grid[0].size();
    int aliveCount = 0;

    // Count alive cells
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j]) aliveCount++;
        }
    }

    // Write grid size
    outFile << rows << " " << cols << endl;
    // Write alive count
    outFile << aliveCount << endl;
    // Write current step
    outFile << currentStep << endl;

    string line = "";
    for (int i = 0; i < cols; i++) {
        line += ". ";
    }
    line += ".";
    outFile << line << endl;

    for (int i = 0; i < rows; i++) {
        line = ".";
        for (int j = 0; j < cols; j++) {
            line += (grid[i][j] ? "O." : " .");
        }
        outFile << line << endl;
    }

    outFile.close();
    cout << "Grid saved successfully to " << filename << endl;
    return true;
}

// Function to load the grid state from a file
// todo: might be problems when loading from file
/**
 * @brief 从文件中加载游戏网格
 *
 * @param filename 文件名
 * @param grid 网格二维布尔向量
 * @param currentStep 当前步骤数
 * @return true 加载成功
 * @return false 加载失败
 */
bool loadGridFromFile(const string& filename, vector<vector<bool>>& grid, int& currentStep) {
    // 打开文件
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Error: Unable to open file for loading." << endl;
        return false;
    }

    int rows, cols, aliveCount;
    string line;

    // 读取网格行数和列数
    if (!(inFile >> rows >> cols)) {
        cerr << "Error: Invalid file format (grid size)." << endl;
        return false;
    }

    // 读取存活细胞数量
    if (!(inFile >> aliveCount)) {
        cerr << "Error: Invalid file format (alive count)." << endl;
        return false;
    }

    // 读取当前步骤数
    if (!(inFile >> currentStep)) {
        cerr << "Error: Invalid file format (current step)." << endl;
        return false;
    }

    // 初始化网格，全部设为死亡
    grid.assign(rows, vector<bool>(cols, false));

    // 读取网格状态
    getline(inFile, line); // 消耗当前行的剩余换行符
    getline(inFile, line); // 消耗当前行的剩余换行符
    for (int i = 0; i < rows; i++) {
        if (!getline(inFile, line)) {
            cerr << "Error: Incomplete grid data." << endl;
            return false;
        }

        // 输出当前读取的行内容，便于调试
        // cout << "Reading row " << i + 1 << ": " << line << endl;

        int j = 0; // 列索引
        for (char ch : line) {
            if (ch == '.') {
                continue; // 忽略分隔符和边界的点
            }
            if (ch == 'O') {
                if (j < cols) {
                    grid[i][j] = true; // 存活
                    j++;
                } else {
                    cerr << "Error: Too many cells in row " << i + 1 << "." << endl;
                    return false;
                }
            }
            else if (ch == ' ') {
                if (j < cols) {
                    grid[i][j] = false; // 死亡
                    j++;
                } else {
                    cerr << "Error: Too many cells in row " << i + 1 << "." << endl;
                    return false;
                }
            }
            else {
                // 处理其他可能的字符
                cerr << "Error: Invalid character '" << ch << "' in grid data at row " << i + 1 << ", column " << j + 1 << "." << endl;
                return false;
            }
        }

        if (j < cols) {
            cerr << "Error: Not enough cells in row " << i + 1 << "." << endl;
            return false;
        }
    }

    inFile.close();
    cout << "Grid loaded successfully from " << filename << endl;
    return true;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator

    int rows, cols, aliveCount, steps, lifePattern;
    vector<vector<bool>> grid;
    int currentStep = 0;

    // Initial choice: new simulation or load from file
    cout << "Conway's Game of Life Simulation\n";
    cout << "---------------------------------\n";
    cout << "1. Start a new simulation\n";
    cout << "2. Load simulation from file\n";;
    cout << "3. Run until find a life\n";
    cout << "Enter your choice (1 - 3): ";
    int choice;
    cin >> choice;

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

        // Create the grid
        grid.assign(rows, vector<bool>(cols, false));
        cout << "Grid assigned" << endl;

        // Initialize grid with random alive cells
        initializeGrid(grid, aliveCount);
        cout << "Grid initialized" << endl;

        // Display initial grid
        cout << "\nInitial grid:\n";
        displayGrid(grid);
    }
    else if (choice == 2) {
        // Load from file
        string filename;
        cout << "Enter the filename to load: ";
        cin >> filename;

        if (!loadGridFromFile(filename, grid, currentStep)) {
            return 1;  // Exit if loading fails
        }

        // Optionally, ask for number of steps to continue
        cout << "Enter the number of additional steps to simulate: ";
        cin >> steps;

        // Display loaded grid
        cout << "\nLoaded grid (Step " << currentStep << "):\n";
        displayGrid(grid);
    }
    else if (choice == 3) {
        cout << "Choose a life pattern: ";
        cin >> lifePattern;
        cout << "Enter the number of rows (minimum 3): ";
        cin >> rows;
        cout << "Enter the number of columns (minimum 3): ";
        cin >> cols;
        cout << "Enter the number of alive cells: ";
        cin >> aliveCount;
        cout << "Enter the number of steps: ";
        cin >> steps;
        // todo: pattern matching
    }
    else {
        cout << "Invalid choice. Exiting." << endl;
        return 1;
    }

    // Simulation loop
    for (int step = 0; step < steps; step++) {
        // Update grid
        grid = updateGrid(grid);
        currentStep++;

        // Display current step
        cout << "\nStep " << currentStep << ":\n";
        displayGrid(grid);

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
            if (saveGridToFile(saveFilename, grid, currentStep)) {
                cout << "Simulation saved. Exiting." << endl;
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
            if (loadGridFromFile(loadFilename, grid, currentStep)) {
                // Display loaded grid
                cout << "\nLoaded grid (Step " << currentStep << "):\n";
                displayGrid(grid);
                // Adjust remaining steps if desired
            }
            else {
                cout << "Failed to load. Continuing simulation." << endl;
            }
        }
        else if (action == 4) {
            // Exit without saving
            cout << "Exiting without saving." << endl;
            return 0;
        }
        else {
            cout << "Invalid action. Continuing simulation." << endl;
        }
    }

    cout << "\nSimulation completed." << endl;
    return 0;
}



/*
 * Run in terminal window:
 * cd "/Users/libangyu/Dev/csc8501coursework/conwaysgameoflife"
 * clang++ -std=c++17 -o gameoflife main.cpp
 * ./gameoflife
 */