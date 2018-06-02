# Election Debates

This task is about implementation of [AVL Tree](https://en.wikipedia.org/wiki/AVL_tree) with a minor modification to be able to find the [median](https://en.wikipedia.org/wiki/Median) value of given two nodes in the tree.

Implementation was done without STL.

## Task Description

For several weeks, there has been an excited mood in a country very far away. The cause of all this are the upcoming elections. The whole country tensely hopes the long awaited change to the better might be right around the corner. This atmosphere stresses politicians more than anyone, as in the upcoming days they will have to participate in rough battles during television debates. In particular, difficult challenges await members of the cabinet, who would very much like to retain their mandates. That is, because above all they will be judged by the successes or failures of the cabinet in the past tenure.

Minister of finance is in the hardest position, as it is no secret his actions during his tenure were inadequate at best. Specifically, he is responsible for the decrease in people's incomes, which put many people below the subsistence level. By taking this fact into consideration, there is little to no chance the minister would make a good impression on voters. Therefore, he and his advisers devised a rescue plan. If the minister were able to present in debates a value, which is true, is based on real incomes, doesn't look that bad at the first glance, but whose calculation is difficult for voters to grasp, that would be great, wouldn't it ? The first choice was the arithmetic mean of incomes, but survey showed voters understand the mean rather well. Therefore, the advisers have chosen the median, which is not as widespread. In particular, to succeed in debates and thus in the elections, the minister needs the following - every time a host or political opponent asks him a question about incomes in the given financial range, the minister's answer will be the median of incomes of all people, that have income in the given range. However, the situation gets harder by the fact the incomes of people often change. This means some incomes might get deleted from the list of all people's incomes, or some incomes might be inserted into the list of all people's incomes (a change of an income can be described by a combination of a deletion of the old value and an insertion of the new value). It is thus needed the minister has quickly available medians for the given ranges of incomes, with respect to dynamically changing list of people's incomes.

As this is a very risky plan to sway the favor of voters to the minister's side, an experienced programmer, who is not scared of such challenge, had to be found. The responsibility has been bestowed upon you. You have to create a program, that for a given sequence of insertions/deletions of incomes and ranges for median search, works in a way described above. Given the maximal classification, you are allowed to use little to none code you haven't produced yourself. This measure also includes STL library and so it is not possible to use it to solve this task.

#### Input and Output Format:

Input consists of several lines, where one line encodes one command (see the commands in quotes below):

**"1 X"** where X is a positive integer, 1 ≤ X ≤ 109, encodes the insertion of an income of value X to the list of incomes.
**"2 X"** where X is a positive integer, 1 ≤ X ≤ 109, encodes the deletion of an income of value X from the list of incomes.
**"3 X Y"** where X, Y are positive integers, 1 ≤ X, Y ≤ 109, X ≤ Y, encodes the search of the median (see definition below) of numbers in range from X to Y (inclusive) in the current list of incomes. The median has to be output on a single line.
**"4"** encodes the start of elections, so there will be no more commands on input; it is guaranteed this is the last command on input.

You can assume all inputs are valid.

##### Moreover:
The median of numbers in the range from X to Y from the current list of incomes is defined as follows: if, in the income list, there are k incomes of value in the range X to Y (inclusive), the median is ⌈k⁄2⌉-th smallest of these incomes.
Should there be no income in the list for the given range, output a single line with text "notfound" (without quotes).
It is possible, due to a bureaucratic mistake, that your program will be given a command to delete a value of income not present in the list of incomes. If this happens, such command should be ignored.
On the contrary, no two incomes of the same value will be present in the list of incomes at the same time. However, after a deletion of a certain value of income, it is possible this value will be inserted to the list again.

#### Classification Conditions:

To pass the basic compulsory tests, your solution has to return correct answers for inputs of size at most 1 000 commands. Issued commands are only of types 1, 3 and 4.
To pass the test on large dataset #1, your solution has to output correct answers within the time limit for inputs of size at most 1 000 000 commands. Issued commands are only of types 1, 3 and 4.
To pass the test on large dataset #2, your solution has to output correct answers within the time limit for inputs of size at most 1 000 000 commands. All types of commands may be issued.

Tests based on the examples are divided into two different tests because of the limitations on the types of commands. First example test examines the submitted program on the Input Examples 1 and 2 and is mandatory. The second example test checks the program on the Input Example 3 and is not mandatory.

### Examples:
##### Input Example 1:

	1 13
	1 4
	1 17
	1 6
	1 9
	1 14
	3 4 9
	3 3 10
	3 14 14
	3 1 20
	4

##### Output Example 1:

	6
	6
	14
	9

##### Input Example 2:

	1 2
	1 3
	1 1
	3 15 20
	3 2 2
	3 4 4
	4

##### Output Example 2:

	notfound
	2
	notfound

##### Input Example 3:

	1 4
	1 8
	1 6
	1 12
	1 7
	2 10
	3 10 13
	3 8 15
	3 1 100
	3 2 7
	2 6
	3 2 7
	2 4
	3 2 7
	2 7
	3 2 7
	4

##### Output Example 3:

	12
	8
	7
	6
	4
	7
	notfound

