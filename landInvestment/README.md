# Land Investment

This task is about iterating over a 2x2 array efficiently.

Implementation probably can be improved using [Dynamic Programming](https://en.wikipedia.org/wiki/Dynamic_programming).

## Task Description

The task is to develop a program to optimize parcel investments.

We assume a grid of land parcels. The size of the grid is known (1 to 2000 rows/columns), moreover, the price of the individual parcels is known. The price is an integer, it may be positive, zero, or negative. A negative (zero) price is acceptable, e.g. the land is an easement.

We want to invest money into the parcels. The investment is tricky, however. We may only invest into adjacent parcels that form a rectangular (square) shape. Moreover, we are searching for an investment where the sum of parcel prices is exactly equal to the invested money.

The program must understand two kinds of query. First, a query of type count x finds the total number of possible investments. Second, query of type list x is an extension of count x. It lists the individual parcels that match the query and the total number of possible investments.

The input of the program is:

* the size of the grid (width, height), both dimensions in the range 1 to 2000,
* the price of individual parcels in the grid. The prices are listed in a row-major order in the input,
* the queries.

A query is either count x or list x, where x is the amount of money to invest.

The output of the program are answers to the queries:

* **count x** answer is the total number of possible parcel allocations such that the allocations form a rectangle/square in the grid and the sum of parcel prices equals to x,
* **list x** delivers the list of possible parcel allocations plus the total number of allocations (like count x). The list of possible allocations consists of lines, each line represents one possible allocation:

x @ (x1,y1) - (x2,y2) 
        
where x1,y1 is upper left corner of the allocation and x2,y2 is the lower right corner of the allocation.

The program must validate input data. If the input is invalid, the program must detect it, it shall output an error message (see below) and terminate. If displayed, the error message must be displayed to the standard output (do not send it to the error output) and the error message must be terminated by a newline (\n). The input is considered invalid, if:

* grid dimensions are not numbers, or is outside the boundaries (1 to 2000),
* parcel price is not an integer,
* query is neither 'count' nor 'list',
* query parameter is not an integer.

Invest some time to think up the structure of the program. There is an upper limit of the grid size, thus static allocation may be adequate for the task. Next, the searching. A naive searching algorithm may require up to n6 time, there are improvements that decrease the searching time dramatically. The time limits in the testing environment are very benevolent. A sensible implementation of the naive algorithm passes all but bonus tests.

###Sample program runs:

	Map size:
	5 6
	Price map:
	-13   9 -16  14   3
	 11   7  -5  14   9
	 -2  -5   9  -9  -4
	 11  13   3   8 -16
	 -1 -15   1  -4   2
		3   7  -8   0  27
	Queries:
	list 12
	12 @ (0,3) - (2,4)
	12 @ (0,4) - (4,5)
	12 @ (2,2) - (2,3)
	12 @ (4,0) - (4,1)
	Total: 4
	count 12
	Total: 4
	count 11
	Total: 10
	list 11
	11 @ (0,1) - (0,1)
	11 @ (0,3) - (0,3)
	11 @ (0,2) - (0,5)
	11 @ (0,1) - (1,2)
	11 @ (0,2) - (1,5)
	11 @ (0,2) - (3,5)
	11 @ (1,0) - (1,2)
	11 @ (1,1) - (3,2)
	11 @ (2,2) - (3,3)
	11 @ (2,3) - (3,3)
	Total: 10
	count -3
	Total: 2
	list -3
	-3 @ (0,0) - (4,0)
	-3 @ (2,4) - (3,4)
	Total: 2
	list 9
	9 @ (0,1) - (0,2)
	9 @ (0,2) - (0,3)
	9 @ (0,0) - (0,5)
	9 @ (0,0) - (2,5)
	9 @ (0,2) - (3,4)
	9 @ (1,0) - (1,0)
	9 @ (1,0) - (1,4)
	9 @ (1,2) - (4,5)
	9 @ (2,2) - (2,2)
	9 @ (2,1) - (3,1)
	9 @ (2,1) - (3,2)
	9 @ (2,1) - (3,5)
	9 @ (2,0) - (4,4)
	9 @ (2,1) - (4,3)
	9 @ (2,2) - (4,5)
	9 @ (3,1) - (3,4)
	9 @ (3,1) - (3,5)
	9 @ (4,1) - (4,1)
	9 @ (4,2) - (4,5)
	Total: 19
	list 0
	0 @ (1,1) - (1,4)
	0 @ (1,2) - (1,5)
	0 @ (1,0) - (2,5)
	0 @ (1,2) - (3,5)
	0 @ (2,1) - (2,5)
	0 @ (2,2) - (3,2)
	0 @ (2,2) - (3,5)
	0 @ (2,3) - (3,5)
	0 @ (3,5) - (3,5)
	0 @ (3,1) - (4,4)
	Total: 10
	count 10000
	Total: 0

	Map size:
	4 3
	Price map:
	1 -1 1 -1
	-1 1 -1 1
	1 -1 1 -1
	Queries:
	count 0
	Total: 36
	list 0
	0 @ (0,0) - (1,0)
	0 @ (0,0) - (3,0)
	0 @ (2,0) - (3,0)
	0 @ (1,0) - (2,0)
	0 @ (0,0) - (0,1)
	0 @ (0,0) - (1,1)
	0 @ (1,0) - (1,1)
	0 @ (0,0) - (2,1)
	0 @ (1,0) - (2,1)
	0 @ (2,0) - (2,1)
	0 @ (0,0) - (3,1)
	0 @ (1,0) - (3,1)
	0 @ (2,0) - (3,1)
	0 @ (3,0) - (3,1)
	0 @ (0,0) - (1,2)
	0 @ (0,0) - (3,2)
	0 @ (2,0) - (3,2)
	0 @ (1,0) - (2,2)
	0 @ (1,1) - (2,1)
	0 @ (0,1) - (1,1)
	0 @ (0,1) - (3,1)
	0 @ (2,1) - (3,1)
	0 @ (0,1) - (0,2)
	0 @ (0,1) - (1,2)
	0 @ (1,1) - (1,2)
	0 @ (0,1) - (2,2)
	0 @ (1,1) - (2,2)
	0 @ (2,1) - (2,2)
	0 @ (0,1) - (3,2)
	0 @ (1,1) - (3,2)
	0 @ (2,1) - (3,2)
	0 @ (3,1) - (3,2)
	0 @ (0,2) - (1,2)
	0 @ (0,2) - (3,2)
	0 @ (2,2) - (3,2)
	0 @ (1,2) - (2,2)
	Total: 36

	Map size:
	1 12
	Price map:
	1 2 3
	4 1 2 3 4 1 2
	3 4
	Queries:
	count 0
	Total: 0
	list 9
	9 @ (0,1) - (0,3)
	9 @ (0,5) - (0,7)
	9 @ (0,9) - (0,11)
	Total: 3
	count 10
	Total: 9
	list 10
	10 @ (0,0) - (0,3)
	10 @ (0,1) - (0,4)
	10 @ (0,2) - (0,5)
	10 @ (0,3) - (0,6)
	10 @ (0,4) - (0,7)
	10 @ (0,5) - (0,8)
	10 @ (0,6) - (0,9)
	10 @ (0,7) - (0,10)
	10 @ (0,8) - (0,11)
	Total: 9
	buy 10
	Invalid input.

	Map size:
	2 2
	Price map:
	1 2 3 test
	Invalid input.


