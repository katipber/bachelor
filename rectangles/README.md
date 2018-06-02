# Rectangles

When i was solving this task i did not know any searching algorithm. First i implemented [BFS](https://en.wikipedia.org/wiki/Breadth-first_search) but that implementation failed memory restrictions. Then i came up with something similar to [DFS](https://en.wikipedia.org/wiki/Depth-first_search) with some heuristic to narrow down the search tree.

## Task Description

The problem is to develop an efficient program that solves "rectangle" puzzle.

The puzzle is a rectangular board with a grid. A cell is either empty, or is filled with a number. The problem is to divide the board into rectangles such that:

* the rectangles cover entire board,
* the rectangles do not overlap,
* each rectangle contains exactly one cell with number
* the size of the rectangle (area) is equal to the number it contains.

The size of the board is limited by 32x32 cells, there is at most 200 cells filled inside the board, and the numbers in the cells are in the range 1-99 inclusive.

The input of the program is the puzzle, the exact format is shown below.

The output of the program is the solution of the input puzzle. The program shall distinguish four answers:

* the input is invalid,
* the puzzle has exactly one solution. The solution is shown in this case.
* the puzzle does not have any solution. The program just displays the no solution answer.
* there are several different solutions of the input puzzle. The program shall count all possible unique solutions. Two puzzle solutions are considered unique if they differ in at least one rectangle placement and/or shape.

The program must validate the input data. If there are invalid, or inconsistent data in the input, the program shall detect it, display an error message, and terminate. The format of the error message is shown in the sample run below. The following is considered invalid:

* missing, incomplete, or irregular puzzle border,
* a cell contents different from spaces and integers 1-99 inclusive,
* missing grid inside the board,
* the puzzle exceeds 32x32 cells in size,
* there is more than 200 filled cells in the input.

The program is tested in a limited environment. The program is limited in both time and memory space. Both limits are shown in the reference solution testing log. In general, the program does not need much memory. On the other hand, the program may require a lot of CPU time to test all possible permutations, especially if there are many solutions of the puzzle. An efficient algorithm and careful implementation is required to pass the time limits. In particular, design the algorithm to avoid unnecessary branches which do not lead to the solution.

The problem is a contest problem. It means it is more complicated than the regular problems. The solution requires both programming and algorithmic skills. The problem is intended for gifted students who understand programming and who are bored by the simplicity of the regular problems. The assessment will consist of two parts. There are some points awarded once your program passes the non-contest tests. The remaining points will be awarded at the end of the contest, based on the speed and efficiency of your solution compared to the other submitted solutions.

### Sample program run:

	Enter the puzzle:
	+--+--+--+--+--+
	| 2  3        2|
	+  +  +  +  +  +
	|       2  3   |
	+  +  +  +  +  +
	|    3         |
	+  +  +  +  +  +
	|       2      |
	+  +  +  +  +  +
	| 2        3  3|
	+--+--+--+--+--+
	One solution:
	+--+--+--+--+--+
	| 2| 3      | 2|
	+  +--+--+--+  +
	|  |    2| 3|  |
	+--+--+--+  +--+
	|    3   |  |  |
	+--+--+--+  +  +
	|  |    2|  |  |
	+  +--+--+--+  +
	| 2|       3| 3|
	+--+--+--+--+--+

	Enter the puzzle:
	+--+--+--+--+--+--+--+--+--+--+
	| 4  2                        |
	+  +  +  +  +  +  +  +  +  +  +
	|          3                  |
	+  +  +  +  +  +  +  +  +  +  +
	|    2        3     8         |
	+  +  +  +  +  +  +  +  +  +  +
	|       4  2             10   |
	+  +  +  +  +  +  +  +  +  +  +
	|    2     2     3            |
	+  +  +  +  +  +  +  +  +  +  +
	|                         2  6|
	+  +  +  +  +  +  +  +  +  +  +
	|               12  6         |
	+  +  +  +  +  +  +  +  +  +  +
	| 2        4                 4|
	+  +  +  +  +  +  +  +  +  +  +
	|                  10     3   |
	+  +  +  +  +  +  +  +  +  +  +
	| 2  2  2                     |
	+--+--+--+--+--+--+--+--+--+--+
	One solution:
	+--+--+--+--+--+--+--+--+--+--+
	| 4| 2|  |  |  |     |     |  |
	+  +  +  +  +  +  +  +  +  +  +
	|  |  |  | 3|  |     |     |  |
	+  +--+  +  +  +  +  +  +  +  +
	|  | 2|  |  | 3|    8|     |  |
	+  +  +  +--+--+  +  +  +  +  +
	|  |  | 4| 2   |     |   10|  |
	+--+--+--+--+--+--+--+  +  +  +
	|    2|    2|    3   |     |  |
	+--+--+--+--+--+--+--+--+--+  +
	|                 |     | 2| 6|
	+  +  +  +  +  +  +  +  +  +--+
	|               12| 6   |  |  |
	+--+--+--+--+--+--+  +  +--+  +
	| 2   |    4      |     |  | 4|
	+--+--+--+--+--+--+--+--+  +  +
	|  |  |  |         10   | 3|  |
	+  +  +  +  +  +  +  +  +  +  +
	| 2| 2| 2|              |  |  |
	+--+--+--+--+--+--+--+--+--+--+

	Enter the puzzle:
	+--+--+--+--+--+--+--+--+--+
	| 2        3        2      |
	+  +  +  +  +  +  +  +  +  +
	|          3  2  2  1      |
	+  +  +  +  +  +  +  +  +  +
	|                7         |
	+  +  +  +  +  +  +  +  +  +
	|             7        3   |
	+  +  +  +  +  +  +  +  +  +
	|    4     4  3     3     7|
	+  +  +  +  +  +  +  +  +  +
	|                3         |
	+  +  +  +  +  +  +  +  +  +
	|    4              3      |
	+--+--+--+--+--+--+--+--+--+
	One solution:
	+--+--+--+--+--+--+--+--+--+
	| 2|       3|  |  | 2   |  |
	+  +--+--+--+  +  +--+--+  +
	|  |       3| 2| 2| 1|  |  |
	+--+--+--+--+--+--+--+  +  +
	|                7   |  |  |
	+--+--+--+--+--+--+--+  +  +
	|             7      | 3|  |
	+--+--+--+--+--+--+--+--+  +
	|    4|    4| 3|    3   | 7|
	+  +  +  +  +  +--+--+--+  +
	|     |     |  | 3      |  |
	+--+--+--+--+  +--+--+--+  +
	|    4      |  |    3   |  |
	+--+--+--+--+--+--+--+--+--+

	Enter the puzzle:
	+--+--+--+--+
	| 4         |
	+  +  +  +  +
	|       4   |
	+  +  +  +  +
	|    4      |
	+  +  +  +  +
	|          4|
	+--+--+--+--+
	Total solutions: 9

	Enter the puzzle:
	+--+--+--+--+--+--+--+--+--+
	|    3        3            |
	+  +  +  +  +  +  +  +  +  +
	| 4     4  3  2     2  3   |
	+  +  +  +  +  +  +  +  +  +
	|                2         |
	+  +  +  +  +  +  +  +  +  +
	|    4     2           3   |
	+  +  +  +  +  +  +  +  +  +
	| 3                    6   |
	+  +  +  +  +  +  +  +  +  +
	|          2     2     2   |
	+  +  +  +  +  +  +  +  +  +
	|       2     2     2     8|
	+  +  +  +  +  +  +  +  +  +
	|    8                     |
	+--+--+--+--+--+--+--+--+--+
	Total solutions: 39

	Enter the puzzle:
	+--+--+--+--+--+--+
	|                 |
	+  +  +  +  +  +  +
	|            12   |
	+  +  +  +  +  +  +
	|       2     4   |
	+  +  +  +  +  +  +
	|    4            |
	+  +  +  +  +  +  +
	|    2            |
	+  +  +  +  +  +  +
	|                 |
	+  +  +  +  +  +  +
	|    2  8         |
	+  +  +  +  +  +  +
	| 3           5  6|
	+--+--+--+--+--+--+
	No solution.

	Enter the puzzle:
	+--+--+--+--+--+
	|          2   |
	+  +  +  +  +  +
	| 3     2      |
	+  +  +  +  +  +
	|          3   |
	+  +  +  +  +  +
	|              |
	+  +  +  +  +  +
	| 2        2   7|
	+  +  +  +  +  +
	|       3      |
	+  +  +  +  +  +
	| 2  7  2      |
	+--+--+--+--+--+
	Invalid input.

