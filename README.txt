Ansly Jeyaseelan
811641166

to compile this file, just type in make
to run the file the usage is ./proj4.out inputFile outputFile s t
s is the sum you want to find and t is the thread you want to use
ex : ./proj4 input.txt 15 3

Findings:
Small Grid: A 4x4 or 5x5 grid.
Medium Grid: An 8x8 or 10x10 grid.


Grid Size	1 Thread Time	2 Thread Time	3 Thread Time
Small (4x4)	   0.000002	  0.000191s	  0.000154s
Medium (10x10)	   0.000007s	  0.000125s	  0.000265s



Small Grids (4x4):
For small grid sizes, the use of multiple threads does not significantly improve performance. In fact, the program with a single thread performs faster than those with two or three threads. This is due to the overhead associated with thread creation and management, which outweighs the benefits of parallelization for small computational tasks.

Medium Grids (10x10):
For medium grid sizes, using two threads provides a slight performance improvement compared to a single thread. However, using three threads results in a longer execution time than using two threads. This suggests that for this grid size, the overhead of managing three threads might be counterproductive, or the workload distribution across threads is not optimized.

Project 4: Multithreaded Diagonal Sum Finder
Project Overview
This is a C programming project that implements a multithreaded algorithm for finding all diagonals in a numerical grid that sum to a specified target value. The project demonstrates advanced concepts in concurrent programming and memory management, utilizing POSIX threads (pthreads) to achieve parallel computation across multiple cores.
Core Functionality
The application reads an n×n grid of single-digit numbers (1-9) from an input file and systematically searches through all possible diagonal sequences—both left-to-right and right-to-left diagonals—to identify those whose digits sum to a user-specified target value. When a matching diagonal is found, those specific digit positions are preserved in an output grid, while all other positions are filled with zeros. This creates a visual representation showing exactly which diagonals in the original grid satisfy the sum requirement.
The program accepts four command-line arguments: an input file containing the grid, an output file for results, a target sum value, and the number of threads to use (1-3). For example, running ./proj4.out in1.txt out1.txt 10 2 would search for all diagonals summing to 10 using 2 threads.
Technical Architecture
The implementation is structured around a modular design with separate header and implementation files. The core data structure is a grid struct containing a dynamically allocated 2D array of unsigned characters and its dimension. The main API consists of four functions: initializeGrid for parsing input files and allocating memory, diagonalSums for the parallel computation, writeGrid for file output, and freeGrid for proper memory cleanup.
The diagonalSums function orchestrates the parallel computation by dividing the workload among threads. Each thread processes a subset of rows, examining all possible diagonal sequences starting from positions within its assigned range. The algorithm checks both right-descending diagonals (upper-left to lower-right) and left-descending diagonals (upper-right to lower-left). An important optimization exits early from diagonal traversal when the running sum exceeds the target, avoiding unnecessary computation.
Threading Implementation
The multithreading strategy uses a custom ThreadArgs struct to pass parameters to worker threads. When multiple threads are requested, the program uses pthread_create to spawn workers and pthread_join to synchronize completion. The workload is distributed by dividing grid rows approximately evenly among threads. For single-threaded execution, the program bypasses thread creation overhead and executes computation directly in the main thread.
Performance Analysis
The project includes empirical performance testing with microsecond-precision timing measurements. The findings reveal interesting insights about threading efficiency:
Small grids (4×4): Single-threaded execution (0.000002s) outperforms multithreaded versions (0.000191s for 2 threads, 0.000154s for 3 threads) because thread creation overhead exceeds computational benefits.
Medium grids (10×10): Two threads (0.000125s) show slight improvements over one thread (0.000007s), but three threads (0.000265s) perform worse, suggesting diminishing returns and possible overhead from thread management.
Build and Testing
The project includes a Makefile for compilation using gcc with -pthread for thread support, -Wall for warnings, and -pedantic-errors for strict standards compliance. Five test cases with corresponding input and output files verify correctness across various grid sizes and target sums.
This project effectively demonstrates systems programming concepts including dynamic memory management, file I/O with error handling, concurrent programming with thread synchronization, and performance profiling for parallel algorithms—all essential skills for modern software development.
