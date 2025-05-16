#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <vector>
#include <string>
#include <mutex>

// Клас Solver реалізує алгоритм пошуку замкнутого шляху по головоломці
class Solver {
public:
    // Структура, що описує одну точку шляху:
    // координати клітинки (row, col) та напрям руху (0=вгору, 1=право, 2=вниз, 3=вліво)
    struct PathPoint {
        int row;
        int col;
        int direction;
    };

    // Конструктор, приймає початкову сітку головоломки
    Solver(const std::vector<std::vector<char>>& initialGrid);

    // Запускає пошук рішення
    void findSolution();

    // Виводить поточну сітку у вигляді рамки (основний метод)
    void printGridFrame() const;

    // Статичний метод: вивід будь-якої сітки з рамкою
    static void printGridFrame(const std::vector<std::vector<char>>& frameGrid);

    // Вивід маршруту рішення у вигляді ASCII-графіки в консолі
    void printPathVisualization() const;

    // Зберігає результат рішення у текстовий файл (із візуалізацією)
    void PrintSolutionToFile(const std::string& outFileBase) const;

    // Повертає вектор точок знайденого шляху
    const std::vector<PathPoint>& getSolutionPathPoints() const;

private:
    int Rows;  // Кількість рядків у сітці
    int Cols;  // Кількість стовпців
    std::vector<std::vector<char>> puzzleGrid; // Сітка головоломки
    bool isSolutionFound; // Флаг: чи знайдено рішення
    std::vector<PathPoint> solutionPathPoints; // Знайдений маршрут

    // Для синхронізації доступу з різних потоків
    mutable std::mutex outputMutex;

    // Перевірка, чи координати в межах сітки
    bool isValidCell(int row, int col) const;

    // Чи потрібна зміна напрямку (залежно від клітинки)
    bool isTurnRequired(int row, int col, int prevDir, int nextDir) const;

    // Чи заборонена зміна напрямку (залежно від клітинки)
    bool isTurnForbidden(int row, int col, int prevDir, int nextDir) const;

    // Рекурсивна заливка, щоб перевірити досяжність
    void floodFill(int row, int col,
                   const std::vector<std::vector<bool>>& visitedCells,
                   std::vector<std::vector<bool>>& reachableCells) const;

    // Чи всі порожні клітинки досяжні (оптимізація)
    bool allEmptyReachable(const std::vector<std::vector<bool>>& visitedCells) const;

    // Головна функція пошуку в глибину (DFS)
    bool dfs(int currentRow, int currentCol,
             int startRow, int startCol,
             int prevDirection,
             std::vector<std::vector<bool>>& visitedCells,
             std::vector<PathPoint>& currentPath,
             int visitedEmptyCount,
             int totalEmptyCount);

    // Один потік обробки з певної стартової точки
    void worker(int startRow, int startCol, int totalEmptyCount);
};

#endif // SOLVER_HPP
