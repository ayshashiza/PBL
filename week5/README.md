# Week 5 PBL: Page Replacement Algorithm Simulator

## 1. Project Overview
This project simulates and compares three page replacement algorithms used in operating systems: FIFO, LRU, and Optimal.

## 2. Algorithms Implemented
- **FIFO (First-In, First-Out):** Replaces the page that entered memory first.
- **LRU (Least Recently Used):** Replaces the page that has not been used for the longest time.
- **Optimal:** Replaces the page whose next use is farthest in the future, or which will not be used again.

## 3. Technologies Used
- Programming language: C
- Graphical interface: GTK 3
- Development environment: Ubuntu Linux
- Compiler: GCC

## 4. Project Structure
- `src/page_replacement.h` — Shared definitions and function declarations
- `src/fifo.c` — FIFO algorithm
- `src/lru.c` — LRU algorithm
- `src/optimal.c` — Optimal algorithm
- `src/main.c` — Console entry point
- `gui/gui.c` — Graphical interface

## 5. Compilation

From the `week5` directory, run:

```bash
gcc -Wall -Wextra src/fifo.c src/lru.c src/optimal.c gui/gui.c -o page_replacement_gui $(pkg-config --cflags --libs gtk+-3.0)
```

## 6. Running the Application

```bash
./page_replacement_gui
```

Enter the page reference string, number of frames, and desired algorithm, then click **Run Simulation**.

## 7. Sample Test Case
- Page reference string: `7 0 1 2 0 3 0 4 2 3`
- Number of frames: `3`
- Selection: Compare all

| Algorithm | Hits | Page Faults | Hit Ratio | Fault Ratio |
|---|---:|---:|---:|---:|
| FIFO | 1 | 9 | 10% | 90% |
| LRU | 2 | 8 | 20% | 80% |
| Optimal | 4 | 6 | 40% | 60% |

## 8. Conclusion
The simulator demonstrates how different page replacement algorithms manage memory. For the sample input, Optimal produces the fewest page faults.
