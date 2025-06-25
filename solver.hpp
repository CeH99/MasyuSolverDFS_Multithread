#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <vector>
#include <string>
#include <mutex>

// Solver class implements an algorithm to find a closed path through a puzzle
class Solver {
public:
    // Structure representing a path point:
    // cell coordinates (row, col) and movement direction (0=up, 1=right, 2=down, 3=left)
    struct PathPoint {
        int row;
        int col;
        int direction;
    };

    // Constructor takes the initial puzzle grid
    Solver(const std::vector<std::vector<char>>& initialGrid);

    // Starts the solution search
    void findSolution();

    // Prints the current puzzle grid in a framed format (main method)
    void printGridFrame() const;

    // Static method: prints any given grid with a frame
    static void printGridFrame(const std::vector<std::vector<char>>& frameGrid);

    // Prints the solution path as ASCII art to the console
    void printPathVisualization() const;

    // Saves the solution visualization to a text file
    void PrintSolutionToFile(const std::string& outFileBase) const;

    // Returns the vector of points representing the found path
    const std::vector<PathPoint>& getSolutionPathPoints() const;

private:
    int Rows;  // Number of rows in the grid
    int Cols;  // Number of columns in the grid
    std::vector<std::vector<char>> puzzleGrid; // The puzzle grid itself
    bool isSolutionFound; // Flag indicating whether a solution has been found
    std::vector<PathPoint> solutionPathPoints; // The discovered path

    // Synchronization mutex for thread-safe operations
    mutable std::mutex outputMutex;

    // Checks whether given coordinates are within grid bounds
    bool isValidCell(int row, int col) const;

    // Determines if a direction change is required based on the current cell
    bool isTurnRequired(int row, int col, int prevDir, int nextDir) const;

    // Determines if a direction change is forbidden based on the current cell
    bool isTurnForbidden(int row, int col, int prevDir, int nextDir) const;

    // Recursive flood fill to check cell reachability
    void floodFill(int row, int col,
                   const std::vector<std::vector<bool>>& visitedCells,
                   std::vector<std::vector<bool>>& reachableCells) const;

    // Checks if all empty cells are reachable (optimization)
    bool allEmptyReachable(const std::vector<std::vector<bool>>& visitedCells) const;

    // Core depth-first search function
    bool dfs(int currentRow, int currentCol,
             int startRow, int startCol,
             int prevDirection,
             std::vector<std::vector<bool>>& visitedCells,
             std::vector<PathPoint>& currentPath,
             int visitedEmptyCount,
             int totalEmptyCount);

    // Worker thread function for a specific start point
    void worker(int startRow, int startCol, int totalEmptyCount);
};

#endif // SOLVER_HPP
