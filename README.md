# 🧩 Masyu Solver (DFS, Multithreaded C++)

A multithreaded C++ console application that solves Masyu puzzles (a Nikoli logic game) using depth-first search (DFS). Designed with clear Module–Controller–View architecture, and built for performance with parallel threads.

## 🚀 Features

- Solves Masyu puzzles using DFS
- Multithreaded solver for speed on large puzzles
- Modular design separating puzzle logic and orchestration
- Easy to extend with different puzzle types or algorithms

## 📁 Project Structure

MasyuSolverDFS_Multithread/
├── src/
│ ├── masyu_solver.cpp/h # Core DFS solving logic
│ ├── puzzle.cpp/h # Puzzle representation & validation
│ └── main.cpp # Orchestrates multithreading & I/O
├── puzzles/
│ └── sample1.txt # Sample input puzzles
└── CMakeLists.txt # Build system

## ✏️ Input & Output Format

- **Input**: plain-text puzzle files (e.g. in `puzzles/`, one puzzle description per file)
- **Output**: solved puzzle printed to console with path and loop visuals

## ⚙️ How to Build & Run

git clone https://github.com/CeH99/MasyuSolverDFS_Multithread.git
cd MasyuSolverDFS_Multithread
mkdir build && cd build
cmake ..
make
./MasyuSolverDFS_Multithread ../puzzles/sample1.txt
🏎 Multithreading Details
Splits initial DFS branches across threads

Uses C++11 <thread> for concurrency

Thread-safe results aggregation and early exit on solution

🛠 How to Extend
Plug in other solving strategies (e.g., BFS, backtracking, heuristics)

Support additional puzzles (Slitherlink, Nurikabe, etc.)

Add a GUI or visualization module

Integrate unit tests (e.g. with GoogleTest)

✅ TODO Ideas
Add command-line parameters for thread count, time limits

Profile performance and optimize locking or data structures

Add support for multiple puzzles in batch

Improve input parser with custom file formats or GUI
