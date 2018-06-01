# Advent Rush

This task demonstrates implementation of [Minimum Spanning Tree](https://en.wikipedia.org/wiki/Minimum_spanning_tree) by [Kruskal](https://en.wikipedia.org/wiki/Kruskal%27s_algorithm) algorithm and [Vertex Cover](https://en.wikipedia.org/wiki/Vertex_cover).

Since STL was not allowed [Linked List](https://en.wikipedia.org/wiki/Linked_list) and [Heap](https://en.wikipedia.org/wiki/Heap_(data_structure)) was also implemented.

## Task Description

Although Advent has already started several days ago, there has been a huge underestimation of preparations for Christmas madness in one of European capitals. To this day, there is not a single Christmas decoration on street lightning lamps, squares are deserted and coniferous trees only sadly try to recall last year's Christmas. This situation obviously doesn't appeal well to the general public, as very popular Christmas markets are not being held this year. Local inhabitants thus decided to take the situation into their own hands by trying to at least in some sense induce Christmas atmosphere.

In order to transform the state of the city to an Advent-like one, two essential steps are to be undertaken: firstly, it is needed to decorate enough number of streets with Christmas decorations. Secondly, it is needed to commence Christmas markets on some of the squares, which are connected by streets. All this is to be done knowing everything is already delayed by many days and that it will be needed to cover all expenses by joint funding. Therefore, both steps will have to be implemented as quickly and as cheaply as possible. In particular, for the first step it will be needed to choose only such subset of streets, for which (in order to save money for decorations) the sum of lengths of chosen streets will be as small as possible. As during Advent nobody wants to walk across gloomy undecorated streets, it also has to be possible to get from every square to any other by using only decorated streets. After the ideal subset of streets to be decorated will be found, the second step will be taken. It will consist of selection of squares on which Christmas markets will be commenced. Naturally, in order to minimize expenses, it would be clever to commence markets on as few squares as possible. However, to make markets close to everyone, every decorated street must lead to at least one square with Christmas market.

Because it is not easy just to read the requirements above, city inhabitants selected you to help them with this task. You will be given a map of the city, which contains a drawing of squares connected by streets and a description of each street's length. Streets are bidirectional and their lengths are positive. No two streets are of the same length. There is no street connecting a square with itself. There is at most single street connecting two squares. It is possible to get from every square to any other by using streets.

During Christmas small elves in your computer, who usually physically interpret operations of STL library, have to help with packing of presents. Therefore it is not possible to use STL library to solve this task.

#### Input Format:

On the first line there are two numbers N and M, specifying the number of squares and the number of streets in the city map, respectively.
The numbering of both squares and streets are zero-based; their numbers are thus 0, 1, ..., N - 1 and 0, 1, ..., M - 1.
After that M lines follow, successively for every street. On the i-th line there are three space-separated numbersxi, yi and ki, 0 ≤ xi, yi < N, xi ≠ yi; 0 < ki ≤ 109. These numbers specify that street i connects squares xi and yi and its length is ki.
There are always at least two squares and therefore at least a single street in the city.
You can assume the input is valid.

#### Output Format:

The output consists of three lines. On the first line output two space-separated numbers: the minimal sum of lengths of decorated streets and the minimal number of squares, on which Christmas markets are to be commenced, respectively. On the second row output, in any order, a space-separated list of numbers of streets that are to be decorated. On the third row output, in any order, a space-separated list of numbers of squares, on which Christmas markets are to be commenced.
Should there be multiple solutions, output an arbitrary one.

#### Classification Conditions:

To pass the compulsory tests (basic and small data tests), your solution has to return correct answers for city maps of at most 10 squares and 20 streets.
To pass the test on medium dataset, your solution has to output correct answers within the time and memory limit for city maps of at most 100 squares and 1 000 streets.
To pass the test on large dataset, your solution has to output correct answers within the time and memory limit for city maps of at most 100 000 squares and 1 000 000 streets.

### Examples:
##### Input Example 1:

	4 5
	1 0 3
	3 1 7
	0 3 2
	0 2 4
	2 3 1

##### Output Example 1:

	6 2
	0 2 4
	0 3

##### Input Example 2:

	7 10
	0 4 4
	0 3 1
	0 2 6
	1 3 7
	1 2 9
	2 3 5
	2 5 8
	3 4 2
	4 5 3
	4 6 10

##### Output Example 2:

	28 2
	1 7 8 5 3 9
	3 4

