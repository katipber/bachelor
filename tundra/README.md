# Tundra



## Task Description

Residents of the northernmost regions of the tundra protest. Traditionally, small villages are able to communicate with each other due to the complicated cable network. News are spreading rapidly in this region, and everyone knows what is going on immediately on the next day. But tough winter of this year and occasional snow storms are too much for old communication technologies which do not satisfy the communication requirements of the village residents.

Not for the first time, the storm had caused an avalanche, and one village was under the snow. All the inhabitants of such village are busy for a full week of snow throwing, defrosting the equipment and strolling the roads, which in turn means that the village is cut off from the news of the outside world. Such a disaster indirectly affects the entire region, because all villages rely on news from the neighbouring villages and cannot communicate with each other in case some village is afflicted by such disaster.

Counterintelligence reports that future storms can destabilize the region, and residents are likely to revolt. The region is governed by a fearless despot that does not want any riots, so he has decided to invest in new satellite communication system TUNDRANET. This technology is based on the similar ideas as the wired connection, but it allows all villages with TUNDRANET to communicate.

The cost of this modern device installation is not negligible, therefore you have been called for forced labor. It is necessary to find out what is the minimum number of installed devices necessary for the communication between any pair of villages.

For the programming purposes the dictator provided you with the handbook "State-Tolerated Liquidation" which describes how to proceed in the current situation. This means you can use STL in your solutions.

#### Input format:

    On the first line there are two numbers N and M, specifying the number of villages and the number of communication channels in the region, respectively.
    The numbering of villages is zero-based; their identifiers are thus 0, 1, ..., N - 1.
    After that M lines follow, where on each line a pair of villages, which are bidirectionally connected, is specified.
    No channel connects the village with itself.
    There is at most one channel between every pair of villages.
    The villages are able to communicate without TUNDRANET if no village is "under the snow".
    You can assume the input is valid.

#### Output Format:

    The output consists of two lines. On the first line, there is one output number: the minimum number of villages that has to be equipped by TUNDRANET. On the second line, print, in any order, a space-separated list of the village identifiers that has to be equipped by TUNDRANET.
    Should there be more than one solution, print an arbitrary one.

#### Classification Conditions:

    To pass the compulsory tests (basic and small data tests), your solution has to return correct answers for region with at most 100 villages.
    To pass the test on large dataset, your solution has to return correct answers within the time and space limit for region of at most 50 000 villages and 1 000 000 channels.

### Examples:
Input Example #1:

5 5
1 2
2 3
3 1
4 1
0 2

Output Example #1:

2
0 4

Input Example #2:

11 13
1 2
2 3
1 3
4 3
5 4
3 5
5 6
6 7
8 7
7 10
8 9
10 9
7 0

Output Example #2:

3
2 0 10

