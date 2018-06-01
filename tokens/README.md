# Tokens

## Task Description

The task is to develop a program that solves logical game - "maximum token sum".

Two players A and B play the game. There is a row of tokens on the table. The row is between 1 and 100 tokens. Each token holds an integer number - the value of the token. The value may be positive, zero, or negative. The players alternate, the game is started by player A. The player on move removes one or two tokens. The player may choose the tokens, however, the tokens may only be removed from the beginning or the end of the row. Even more, if the player decides to remove two tokens from the same end of the row, then the first/last token must be removed first and the (now unblocked) token may be removed second. The game ends when the last token is removed. The player with the greater sum of tokens wins the game.

We assume both players play the best strategy. That is, each player removes the tokens such that his sum is the highest possible. The task is to develop a program that finds the moves and decides who wins the game under these assumptions.

The input of the program is a sequence of integers. The sequence defines the row of tokens. The tokens are referred to by integer indices, index 0 represents the first token, index 1 second, ...

The output of the program is the list of moves. Each move is described on a single output line. A move starts with player id (A or B, alternating), then there is the index of the token being removed (two indices if two tokens are removed) and the value(s) of the token(s) being removed. The last output line lists the sum of tokens for both players.

The program must validate input data. If the input is invalid, the program must detect it, it shall output an error message (see below) and terminate. If displayed, the error message must be displayed to the standard output (do not send it to the error output) and the error message must be terminated by a newline (\n). The input is considered invalid, if:

    the input is an empty sequence, or the sequence exceeds 100 integers,
    a value in the sequence is not an integer.

### Sample program runs:

Tokens:
3 5 3 6 1
A 0 (3)
B 1, 2 (5,3)
A 4, 3 (1,6)
A: 10, B: 8

Tokens:
-11 5 3 6 -1
A 4 (-1)
B 3, 2 (6,3)
A 1 (5)
B 0 (-11)
A: 4, B: -2

Tokens:
1 5 3 6 2 1 8 2 3
A 0 (1)
B 1, 8 (5,3)
A 7, 6 (2,8)
B 2, 3 (3,6)
A 5, 4 (1,2)
A: 14, B: 17

Tokens:
-57 87 -75 117 44 96
A 5, 4 (96,44)
B 3 (117)
A 0, 1 (-57,87)
B 2 (-75)
A: 170, B: 42

Tokens:
73 -7 118 86 11 -11 83
A 0, 6 (73,83)
B 1, 2 (-7,118)
A 3, 4 (86,11)
B 5 (-11)
A: 253, B: 100

Tokens:
-34 -1 94 111 43 78 -79 13
A 7 (13)
B 6, 5 (-79,78)
A 4, 3 (43,111)
B 2, 1 (94,-1)
A 0 (-34)
A: 133, B: 92

Tokens:
-72 36 -2 42 139 116 -59 -31 -34
A 0, 1 (-72,36)
B 8 (-34)
A 7 (-31)
B 2 (-2)
A 3, 4 (42,139)
B 5 (116)
A 6 (-59)
A: 55, B: 80

Tokens:
131 8 97 -68 -79 135 -93 -61 -4 -61
A 0, 1 (131,8)
B 2 (97)
A 9, 8 (-61,-4)
B 3 (-68)
A 4, 5 (-79,135)
B 7 (-61)
A 6 (-93)
A: 37, B: -32

Tokens:
32 119 7 -34 -21 92 -73 92 76 -45 90
A 10, 9 (90,-45)
B 8 (76)
A 0, 1 (32,119)
B 2, 7 (7,92)
A 6, 5 (-73,92)
B 4 (-21)
A 3 (-34)
A: 181, B: 154

Tokens:
2 62 132 25 71 9 52 44 58 15 58 -65
A 11, 10 (-65,58)
B 9, 8 (15,58)
A 7, 6 (44,52)
B 5 (9)
A 4 (71)
B 3, 2 (25,132)
A 1, 0 (62,2)
A: 224, B: 239

Tokens:
140 -63 11 -18 138 -10 0 -74 138 146 43 116 -62
A 0 (140)
B 12, 11 (-62,116)
A 10, 9 (43,146)
B 8 (138)
A 1, 7 (-63,-74)
B 2 (11)
A 3, 4 (-18,138)
B 6 (0)
A 5 (-10)
A: 302, B: 203

Tokens:
127 62 -84 138 141 96 101 42 -29 140 44 9 -6 130
A 0, 13 (127,130)
B 1, 12 (62,-6)
A 2, 3 (-84,138)
B 4, 5 (141,96)
A 6 (101)
B 7 (42)
A 8, 9 (-29,140)
B 11, 10 (9,44)
A: 523, B: 388

Tokens:
-92 92 80 47 6 60 86 -82 -86 93 75 -69 -24 -81 71
A 14 (71)
B 13, 12 (-81,-24)
A 11, 10 (-69,75)
B 9, 8 (93,-86)
A 7, 6 (-82,86)
B 5, 4 (60,6)
A 3, 2 (47,80)
B 1 (92)
A 0 (-92)
A: 116, B: 60

Tokens:
hello
Invalid input.

