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
