#include "solver.hpp"
#include <iostream>
#include <fstream>
#include <thread>

using namespace std;

// Конструктор класу Solver
Solver::Solver(const vector<vector<char>>& initialGrid)
    : Rows(static_cast<int>(initialGrid.size())),
    Cols(Rows ? static_cast<int>(initialGrid[0].size()) : 0),
    puzzleGrid(initialGrid),
    isSolutionFound(false) {
    solutionPathPoints.clear(); // Ініціалізація списку з точками шляху
}

// Статичний метод для виводу довільної сітки у вигляді рамки
void Solver::printGridFrame(const vector<vector<char>>& frameGrid) {
    int frameRows = static_cast<int>(frameGrid.size());
    if (frameRows == 0) return;

    int frameCols = static_cast<int>(frameGrid[0].size());

    // Виводимо верхню межу
    cout << "Size: " << frameRows << "x" << frameCols << endl;
    cout << "+";
    for (int j = 0; j < frameCols; ++j) cout << "---+";
    cout << "\n";

    // Вивід кожного рядка з роздільниками
    for (int i = 0; i < frameRows; ++i) {
        cout << "|";
        for (int j = 0; j < frameCols; ++j) {
            cout << " " << frameGrid[i][j] << " |";
        }
        cout << "\n+";
        for (int j = 0; j < frameCols; ++j) cout << "---+";
        cout << "\n";
    }
}

// Вивід поточної сітки (основної головоломки)
void Solver::printGridFrame() const {
    printGridFrame(puzzleGrid);
}

// Перевірка, чи координати в межах поля
bool Solver::isValidCell(int row, int col) const {
    return row >= 0 && row < Rows && col >= 0 && col < Cols;
}

// Якщо поточна клітинка типу 'B', зміна напрямку дозволена лише якщо він дійсно змінюється
bool Solver::isTurnRequired(int row, int col, int prevDir, int nextDir) const {
    if (puzzleGrid[row][col] == 'B') return prevDir != -1 && prevDir != nextDir;
    return true;
}

// Якщо клітинка 'W', то зміна напрямку заборонена
bool Solver::isTurnForbidden(int row, int col, int prevDir, int nextDir) const {
    if (puzzleGrid[row][col] == 'W') return prevDir != -1 && prevDir != nextDir;
    return false;
}

// Рекурсивна заливка (флуд-філ) для перевірки досяжності порожніх клітинок
void Solver::floodFill(int row, int col,
                       const vector<vector<bool>>& visitedCells,
                       vector<vector<bool>>& reachableCells) const {
    if (!isValidCell(row, col) || visitedCells[row][col] || reachableCells[row][col]) return;

    char cellValue = puzzleGrid[row][col];
    if (cellValue != '.' && cellValue != 'B' && cellValue != 'W') return;

    reachableCells[row][col] = true;

    static int rowDelta[4] = {-1, 0, 1, 0}, colDelta[4] = {0, 1, 0, -1};
    for (int direction = 0; direction < 4; ++direction) {
        floodFill(row + rowDelta[direction], col + colDelta[direction], visitedCells, reachableCells);
    }
}

// Перевірка, чи всі порожні клітинки доступні для проходження
bool Solver::allEmptyReachable(const vector<vector<bool>>& visitedCells) const {
    vector<vector<bool>> reachableCells(Rows, vector<bool>(Cols, false));
    bool foundEmpty = false;

    for (int r = 0; r < Rows && !foundEmpty; ++r) {
        for (int c = 0; c < Cols && !foundEmpty; ++c) {
            if (puzzleGrid[r][c] == '.' && !visitedCells[r][c]) {
                floodFill(r, c, visitedCells, reachableCells);
                foundEmpty = true;
            }
        }
    }

    for (int r = 0; r < Rows; ++r)
        for (int c = 0; c < Cols; ++c)
            if (puzzleGrid[r][c] == '.' && !visitedCells[r][c] && !reachableCells[r][c])
                return false;

    return true;
}

// Основний алгоритм пошуку в глибину з умовами гри
bool Solver::dfs(int currentRow, int currentCol,
                 int startRow, int startCol,
                 int prevDirection,
                 vector<vector<bool>>& visitedCells,
                 vector<PathPoint>& currentPath,
                 int visitedEmptyCount,
                 int totalEmptyCount) {
    if (isSolutionFound) return false;

    static int rowDelta[4] = {-1, 0, 1, 0}, colDelta[4] = {0, 1, 0, -1};

    for (int direction = 0; direction < 4; ++direction) {
        int nextRow = currentRow + rowDelta[direction];
        int nextCol = currentCol + colDelta[direction];

        if (!isValidCell(nextRow, nextCol)) continue;

        // Якщо повернулися в початкову точку і виконано всі умови — рішення знайдено
        if (nextRow == startRow && nextCol == startCol) {
            if (visitedEmptyCount == totalEmptyCount && currentPath.size() > 1
                && isTurnRequired(currentRow, currentCol, prevDirection, direction)
                && !isTurnForbidden(currentRow, currentCol, prevDirection, direction)) {
                currentPath.push_back({nextRow, nextCol, direction});
                return true;
            }
            continue;
        }

        if (visitedCells[nextRow][nextCol] ||
            !isTurnRequired(currentRow, currentCol, prevDirection, direction) ||
            isTurnForbidden(currentRow, currentCol, prevDirection, direction)) {
            continue;
        }

        visitedCells[nextRow][nextCol] = true;
        currentPath.push_back({nextRow, nextCol, direction});
        int addedEmpty = (puzzleGrid[nextRow][nextCol] == '.');

        if (allEmptyReachable(visitedCells) &&
            dfs(nextRow, nextCol, startRow, startCol, direction,
                visitedCells, currentPath, visitedEmptyCount + addedEmpty, totalEmptyCount)) {
            return true;
        }

        // Відкат
        currentPath.pop_back();
        visitedCells[nextRow][nextCol] = false;
    }
    return false;
}

// Потік для паралельного пошуку рішення з окремої стартової точки
void Solver::worker(int startRow, int startCol, int totalEmptyCount) {
    vector<vector<bool>> visitedCells(Rows, vector<bool>(Cols, false));
    visitedCells[startRow][startCol] = true;
    vector<PathPoint> currentPath = {{startRow, startCol, -1}};

    if (dfs(startRow, startCol, startRow, startCol, -1,
            visitedCells, currentPath, 1, totalEmptyCount)) {
        lock_guard<mutex> lock(outputMutex);
        if (!isSolutionFound) {
            isSolutionFound = true;
            solutionPathPoints = currentPath;
        }
    }
}

// Ініціація багатопотокового пошуку
void Solver::findSolution() {
    vector<pair<int,int>> startingPoints;
    int totalEmptyCount = 0;

    for (int r = 0; r < Rows; ++r) {
        for (int c = 0; c < Cols; ++c) {
            if (puzzleGrid[r][c] == '.') {
                startingPoints.emplace_back(r, c);
                ++totalEmptyCount;
            }
        }
    }

    vector<thread> threads;
    for (auto& pt : startingPoints) {
        threads.emplace_back(&Solver::worker, this, pt.first, pt.second, totalEmptyCount);
    }

    for (auto& t : threads) t.join();
}

// Візуалізація шляху на консолі
void Solver::printPathVisualization() const {
    int vizRows = Rows * 2;
    int vizCols = Cols * 4;
    vector<string> canvas(vizRows, string(vizCols, ' '));

    for (int r = 0; r < Rows; ++r)
        for (int c = 0; c < Cols; ++c)
            canvas[r * 2][c * 4] = puzzleGrid[r][c];

    for (size_t i = 1; i < solutionPathPoints.size(); ++i) {
        const auto& prev = solutionPathPoints[i - 1];
        const auto& curr = solutionPathPoints[i];
        int y0 = prev.row * 2, x0 = prev.col * 4;
        int y1 = curr.row * 2, x1 = curr.col * 4;

        if (y0 == y1) {
            for (int x = min(x0, x1) + 1; x < max(x0, x1); ++x) canvas[y0][x] = '-';
        } else {
            for (int y = min(y0, y1) + 1; y < max(y0, y1); ++y) canvas[y][x0] = '|';
        }
    }

    cout << "\n--- Path Visualization ---\n";
    for (const auto& rowStr : canvas) cout << rowStr << "\n";
}

// Збереження візуалізації у файл
void Solver::PrintSolutionToFile(const string& outFileBase) const {
    string filename = outFileBase + ".txt";
    ofstream fout(filename);
    if (!fout) {
        cerr << "Cannot create file " << filename << "\n";
        return;
    }

    int vizRows = Rows * 2;
    int vizCols = Cols * 4;
    vector<string> canvas(vizRows, string(vizCols, ' '));

    for (int r = 0; r < Rows; ++r)
        for (int c = 0; c < Cols; ++c)
            canvas[r * 2][c * 4] = puzzleGrid[r][c];

    for (size_t i = 1; i < solutionPathPoints.size(); ++i) {
        const auto& prev = solutionPathPoints[i - 1];
        const auto& curr = solutionPathPoints[i];
        int y0 = prev.row * 2, x0 = prev.col * 4;
        int y1 = curr.row * 2, x1 = curr.col * 4;

        if (y0 == y1) {
            for (int x = min(x0, x1) + 1; x < max(x0, x1); ++x) canvas[y0][x] = '-';
        } else {
            for (int y = min(y0, y1) + 1; y < max(y0, y1); ++y) canvas[y][x0] = '|';
        }
    }

    for (const auto& rowStr : canvas) fout << rowStr << "\n";
    cout << "Saved visualization to " << filename << "\n";
}

// Отримання маршруту рішення
const vector<Solver::PathPoint>& Solver::getSolutionPathPoints() const {
    return solutionPathPoints;
}
