#include "solver.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

int main() {
    int continueFlag = 1;
    do {
        // Вибір режиму введення
        std::cout << "Select input method:\n"
                  << "1) Use preset puzzle\n"
                  << "2) Load puzzle from file\n"
                  << "-> ";
        int mode;
        std::cin >> mode;

        // Заздалегідь визначені головоломки
        std::vector<std::vector<std::vector<char>>> presets = {
            {
                {'.','.','.','.','W','.','.','B','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.','.','.','.','.','.','.'},
                {'.','.','W','.','B','.','.','W','.','.','W','.','B','.'},
                {'.','.','.','.','.','.','.','.','.','.','.','.','.','.'},
                {'.','.','B','.','.','B','.','.','.','B','.','.','W','.'},
                {'.','.','.','B','.','.','.','W','.','.','.','W','.','.'},
                {'.','W','.','.','.','.','.','B','.','W','.','.','B','.'},
                {'.','.','.','.','.','.','W','.','.','.','B','.','.','.'},
                {'.','.','.','.','B','.','.','B','.','W','.','.','.','.'},
                {'.','.','W','.','.','.','.','.','.','.','.','W','.','.'}
            },
            {
                {'.','W','.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','W','.','.','B','.'},
                {'.','W','.','.','W','.','.','B','.','.'},
                {'.','.','.','W','.','.','B','W','.','.'},
                {'.','.','B','.','.','.','.','.','W','.'},
                {'.','B','.','.','.','.','.','W','.','.'},
                {'.','.','W','W','.','.','.','.','.','.'},
                {'.','.','B','.','.','W','.','.','B','.'},
                {'.','W','.','.','W','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.','W','.'}
            },
            {
                {'.','W','.','.','B','W','.','.','B'},
                {'.','.','B','B','W','.','.','W','W'},
                {'.','.','.','.','.','B','.','.','.'},
                {'B','.','.','W','.','.','.','.','.'},
                {'.','W','.','.','W','.','.','.','W'},
                {'B','.','.','.','.','B','W','.','.'},
                {'W','.','B','W','B','.','W','B','W'},
                {'B','.','.','.','B','.','.','B','.'},
                {'.','.','.','B','W','.','.','.','.'}
            },
            {
                {'B','W','.','.','.','B','W','B'},
                {'.','B','.','B','.','.','W','.'},
                {'B','.','B','.','.','.','.','.'},
                {'W','.','W','.','B','.','B','.'},
                {'B','.','.','B','.','.','.','.'},
                {'.','.','.','B','W','.','W','B'},
                {'.','B','.','.','.','.','.','.'},
                {'.','.','W','B','.','.','.','B'}
            },
            {
                {'.','.','W','B'},
                {'.','.','.','.'},
                {'W','B','.','.'}
            }
        };

        std::vector<std::vector<char>> puzzle;
        if (mode == 1) {
            int n = static_cast<int>(presets.size());
            std::cout << "Available presets:\n";
            for (int i = 0; i < n; ++i) {
                std::cout << "  " << (i+1) << ") Preset " << (i+1) << "\n";
                Solver::printGridFrame(presets[i]);
            }

            int choice;
            do {
                std::cout << "Choose preset (1-" << n << "): ";
                std::cin >> choice;

                if (choice < 1 || choice > n)
                    std::cout << "Invalid preset\n";

            } while (choice < 1 || choice > n);
            puzzle = presets[choice - 1];
        }
        else if (mode == 2) {
            std::ifstream fin;
            std::string filename;

            do {
                std::cout << "Enter filename: ";
                std::cin >> filename;
                fin.open(filename);
                if (!fin) {
                    std::cout << "Cannot open file\n";
                }
            } while (!fin);

            int R, C;
            fin >> R >> C;
            puzzle.assign(R, std::vector<char>(C));
            for (int i = 0; i < R; ++i)
                for (int j = 0; j < C; ++j)
                    fin >> puzzle[i][j];
        }
        else {
            std::cerr << "Invalid mode\n";
            return 1;
        }

        Solver solver(puzzle);
        solver.printGridFrame();
        solver.findSolution();

        const auto& path = solver.getSolutionPathPoints();
        if (path.empty()) {
            std::cout << "No solution found.\n";
        } else {
            solver.printPathVisualization();
            int saveOpt;
            do {
                std::cout << "Save solution to file? (1=yes, 0=no): ";
                std::cin >> saveOpt;

                if (saveOpt == 1) {
                    std::cout << "Enter filename (without extension): ";
                    std::string outFile;
                    std::cin >> outFile;
                    solver.PrintSolutionToFile(outFile);
                }
            } while (saveOpt != 0 && saveOpt != 1);
        }
        std::cout << "Continue? (1=yes, 0=no): ";
        std::cin >> continueFlag;
    } while (continueFlag != 0);

    return 0;
}
