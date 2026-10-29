/*   EECS 348 Assignment 5
    Lumo AI Sudoku Solver UPDATED & Extended for File Output
    Program Description:
        A program that solves Sudoku puzzles using an object-oriented recursive search with backtracking,
        prints all solutions, and exports solutions to respective solutionN.txt files.
    Collaborators on this program:
        Caleb Schild (Original Assignment 4 Author)
        Lumo AI (Original AI Collaborator)
    Sources for code:
        Caleb Schild, Lumo AI, and EECS 348 Course Materials
    Author: Caleb Schild
    Creation Date: 10/10/2026
    Revised by: Prabesh Sapkota
    Revision Date: 10/29/2026
    Revisions:
        10/29/2026 - Added file output functionality to export solutions to solutionN.txt files 
                     meeting Assignment 5 output specifications, updated prologue comments and line-by-line attribution.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

// ---------------------------------------------------------------
// Board configuration: single source of truth for grid parameters
// ---------------------------------------------------------------
constexpr int GRID_SIZE = 9;     // Cells per row/column [Author: Caleb Schild / Lumo AI]
constexpr int BLOCK_SIZE = 3;    // Sub-grid dimensions (BLOCK_SIZE x BLOCK_SIZE) [Author: Caleb Schild / Lumo AI]
constexpr int CELL_COUNT = GRID_SIZE * GRID_SIZE; // Total cells on board [Author: Caleb Schild / Lumo AI]
constexpr int EMPTY_CELL = 0;    // Sentinel for an unsolved cell [Author: Caleb Schild / Lumo AI]

class SudokuBoard { // initiates the Sudoku board class [Author: Caleb Schild / Lumo AI]
private: 
    int grid[GRID_SIZE][GRID_SIZE]; // 9x9 grid for Sudoku [Author: Caleb Schild / Lumo AI]

    // Checks if a value exists in the specified row [Author: Caleb Schild / Lumo AI]
    bool valueExistsInRow(int row, int value) const {
        for (int col = 0; col < GRID_SIZE; col++) { // Iterate through columns in row [Author: Caleb Schild / Lumo AI]
            if (grid[row][col] == value) return true; // Match found [Author: Caleb Schild / Lumo AI]
        }
        return false; // No match found [Author: Caleb Schild / Lumo AI]
    }

    // Checks if a value exists in the specified column [Author: Caleb Schild / Lumo AI]
    bool valueExistsInColumn(int col, int value) const {
        for (int row = 0; row < GRID_SIZE; row++) { // Iterate through rows in column [Author: Caleb Schild / Lumo AI]
            if (grid[row][col] == value) return true; // Match found [Author: Caleb Schild / Lumo AI]
        }
        return false; // No match found [Author: Caleb Schild / Lumo AI]
    }

    // Checks if a value exists in the 3x3 block containing (row, col) [Author: Caleb Schild / Lumo AI]
    bool valueExistsInBlock(int row, int col, int value) const {
        const int startRow = (row / BLOCK_SIZE) * BLOCK_SIZE; // Determine block top-left row [Author: Caleb Schild / Lumo AI]
        const int startCol = (col / BLOCK_SIZE) * BLOCK_SIZE; // Determine block top-left col [Author: Caleb Schild / Lumo AI]
        for (int i = 0; i < BLOCK_SIZE; i++) {
            for (int j = 0; j < BLOCK_SIZE; j++) {
                if (grid[startRow + i][startCol + j] == value) return true; // Match found in block [Author: Caleb Schild / Lumo AI]
            }
        }
        return false; // No match in block [Author: Caleb Schild / Lumo AI]
    }

public: 
    // Converts a linear index [0, CELL_COUNT) into row/col coordinates [Author: Caleb Schild / Lumo AI]
    static void indexToCoords(int index, int& row, int& col) {
        row = index / GRID_SIZE; // Compute row coordinate [Author: Caleb Schild / Lumo AI]
        col = index % GRID_SIZE; // Compute column coordinate [Author: Caleb Schild / Lumo AI]
    }

    SudokuBoard() { // Constructor initializes the grid to empty cells [Author: Caleb Schild / Lumo AI]
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                grid[i][j] = EMPTY_CELL; // Set cell to empty [Author: Caleb Schild / Lumo AI]
            }
        }
    }

    // Loads a Sudoku puzzle from a file [Author: Caleb Schild / Lumo AI]
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename); // Open input file stream [Author: Caleb Schild / Lumo AI]
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl; // Error handling [Author: Caleb Schild / Lumo AI]
            return false;
        }
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                char ch;
                if (!(file >> ch)) { // Read character token [Author: Caleb Schild / Lumo AI]
                    std::cerr << "Error: file '" << filename << "' has insufficient data" << std::endl;
                    return false;
                }
                if (ch >= '1' && ch <= '9') {
                    grid[i][j] = ch - '0'; // Convert char digit to int [Author: Caleb Schild / Lumo AI]
                } else {
                    grid[i][j] = EMPTY_CELL; // Treat non-digits as empty [Author: Caleb Schild / Lumo AI]
                }
            }
        }
        return true;
    }

    // Displays the Sudoku board to standard output [Author: Caleb Schild / Lumo AI]
    void display() const {
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                if (grid[i][j] != EMPTY_CELL) {
                    std::cout << grid[i][j];
                } else {
                    std::cout << "_";
                }
                std::cout << " ";
                if ((j + 1) % BLOCK_SIZE == 0 && j != GRID_SIZE - 1) {
                    std::cout << " ";
                }
            }
            std::cout << std::endl;
            if ((i + 1) % BLOCK_SIZE == 0 && i != GRID_SIZE - 1) {
                std::cout << std::endl;
            }
        }
    }

    // Writes the Sudoku board to a specified output file in puzzle format [Revised by: Your Name]
    void saveToFile(std::ofstream& file) const {
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                if (grid[i][j] != EMPTY_CELL) {
                    file << grid[i][j]; // Write digit [Revised by: Your Name]
                } else {
                    file << '_'; // Write empty cell sentinel [Revised by: Your Name]
                }
                if (j < GRID_SIZE - 1) file << " "; // Space separator [Revised by: Your Name]
            }
            file << "\n"; // Newline after row [Revised by: Your Name]
        }
    }

    int getCell(int row, int col) const { return grid[row][col]; } // Accessor [Author: Caleb Schild / Lumo AI]
    void setCell(int row, int col, int value) { grid[row][col] = value; } // Mutator [Author: Caleb Schild / Lumo AI]

    // Checks if placing a value at (row, col) is valid [Author: Caleb Schild / Lumo AI]
    bool isValid(int row, int col, int value) const {
        return !valueExistsInRow(row, value)
            && !valueExistsInColumn(col, value)
            && !valueExistsInBlock(row, col, value);
    }

    // Finds the next empty cell at or after 'startIndex' [Author: Caleb Schild / Lumo AI]
    int findNextEmptyIndex(int startIndex) const {
        for (int index = startIndex; index < CELL_COUNT; index++) {
            int row, col;
            indexToCoords(index, row, col);
            if (grid[row][col] == EMPTY_CELL) {
                return index; // Return first empty cell index found [Author: Caleb Schild / Lumo AI]
            }
        }
        return CELL_COUNT; // Return cell count if none remain [Author: Caleb Schild / Lumo AI]
    }
};

class SudokuSolver { // initiates the Sudoku solver class [Author: Caleb Schild / Lumo AI]
private:
    SudokuBoard board; // Active board instance [Author: Caleb Schild / Lumo AI]
    std::vector<SudokuBoard> solutions; // Storage for solutions [Author: Caleb Schild / Lumo AI]

    // Recursive backtracking solver function [Author: Caleb Schild / Lumo AI]
    void solveRecursive(int startIndex) {
        const int index = board.findNextEmptyIndex(startIndex);
        if (index == CELL_COUNT) {
            solutions.push_back(board); // Solution reached, store copy [Author: Caleb Schild / Lumo AI]
            return;
        }

        int row, col;
        SudokuBoard::indexToCoords(index, row, col);
        for (int value = 1; value <= GRID_SIZE; value++) {
            if (board.isValid(row, col, value)) {
                board.setCell(row, col, value); // Place value [Author: Caleb Schild / Lumo AI]
                solveRecursive(index + 1);      // Recurse to next cell [Author: Caleb Schild / Lumo AI]
                board.setCell(row, col, EMPTY_CELL); // Backtrack [Author: Caleb Schild / Lumo AI]
            }
        }
    }

public:
    void setBoard(const SudokuBoard& b) {
        board = b;
        solutions.clear();
    }

    void solve() {
        solutions.clear();
        solveRecursive(0);
    }

    const std::vector<SudokuBoard>& getSolutions() const { return solutions; }
    size_t getSolutionCount() const { return solutions.size(); }
};

int main() { // Main function to drive solver and export results [Revised by: Your Name]
    const std::vector<std::string> puzzleFiles = {
        "puzzle1.txt", "puzzle2.txt", "puzzle3.txt",
        "puzzle4.txt", "puzzle5.txt"
    };

    SudokuSolver solver; // Solver instance [Author: Caleb Schild / Lumo AI]

    for (size_t p = 0; p < puzzleFiles.size(); p++) {
        const std::string& filename = puzzleFiles[p];
        std::string solFilename = "solution" + std::to_string(p + 1) + ".txt"; // Solution filename [Revised by: Your Name]

        std::cout << "========================================" << std::endl;
        std::cout << "Processing: " << filename << std::endl;
        std::cout << "========================================" << std::endl;

        SudokuBoard puzzle;
        if (!puzzle.loadFromFile(filename)) {
            continue; // Skip if load fails [Author: Caleb Schild / Lumo AI]
        }

        solver.setBoard(puzzle);
        solver.solve(); // Run solver [Author: Caleb Schild / Lumo AI]

        // Open solution output file [Revised by: Your Name]
        std::ofstream outFile(solFilename);
        if (!outFile.is_open()) {
            std::cerr << "Error opening output file: " << solFilename << std::endl;
            continue;
        }

        size_t count = solver.getSolutionCount();
        std::cout << "\nSolutions found: " << count << std::endl << std::endl;

        if (count == 0) {
            outFile << "No solution found." << std::endl; // Required format for no solutions [Assignment 5 Spec]
            std::cout << "No solution found" << std::endl;
        } else {
            int solutionNumber = 1;
            for (const auto& solution : solver.getSolutions()) {
                std::cout << "Solution #" << solutionNumber++ << ":" << std::endl;
                solution.display();
                std::cout << std::endl;

                // Write each solution to solutionN.txt [Revised by: Your Name]
                solution.saveToFile(outFile);
                outFile << "\n"; // Blank line between solutions if multiple
            }
            outFile << "Solutions found: " << count << std::endl; // Required format footer [Assignment 5 Spec]
        }
        outFile.close(); // Close file stream [Revised by: Your Name]
        std::cout << std::endl;
    }

    return 0;
}