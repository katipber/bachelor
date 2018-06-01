# Scoreboard



## Task Description

The task is to develop a generic class CContest that computes the overall scoreboard based on the results of individual matches.

Assume a group of contestants. The contestants compete in matches, there are two contestants in each match, the result of the match is recorded (a simple boolean flag, points, times, ...). Our class needs to be flexible, thus the result is represented using some generic data type. The matches (i.e. the names of contestants and the result) are stored in our CContest instance. Subsequently, the scoreboard needs to be computed based on the individual matches. The trick is that our scoreboard must be computed just based on the results of the known matches, moreover, we want to know whether the known matches are enough to positively compute the scoreboard.

A scoreboard is unambiguous if each contestant Xi on position i holds:

    Xi defeated all contestants Xj, i+1 ≤ j ≤ n. The contestants were defeated either directly in a match, or indirectly (Xi defeats Xa, Xa defeats Xb, ..., Xz defeats Xj, i+1 ≤ a < b < ... < j ≤ n),
    Xi neither won nor drawn any match with any contestant Xk, 0 ≤ k ≤ i-1.

For example let A defeats B and B defeats C. Then the scoreboard is unambiguous A B C. On the other hand, if:

A defeats B
B defeats C
C defeats A

or:

A defeats B
A defeats C

or:

A defeats B
B draws with C

then the scoreboard cannot be established unambiguously (we would have to add further matches, or we would have to establish the scoreboard on other criteria; we are not going to do so in this homework).

It was noted that a match is described as a triplet (contestant1, contestant2, result) where result is some data type to describe the winner/score/times/... The generic result needs to be transformed into one of win/draw/lose. That conversion is accomplished by a comparator passed to the scoreboard computation method. The comparator is one of function/functor/lambda-expression that given an instance of result returns an integer:

    negative, if contestant1 lost,
    zero for draw,
    positive if contestant1 won.

The interface of class template CContest shall be:

the template is parametrized by a generic parameter _M
    this parameter describes the type used to describe the result of a match. Data type _M guarantees moving, copying, and destroying (CopyConstructibe, MoveConstructible, CopyAssignable, MoveAssignable and Destructible). No further interface is guaranteed, in particular, there is not default constructor guaranteed for _M (i.e. does not have to be DefaultConstructible).
default constructor
    initializes an empty instance of CContest,
AddMatch(contestant1, contestant2, result)
    the method adds a new match to the storage in the instance. The contestants (contestant1 and contestant2) are strings, the result is of type _M. Calls to AddMatch may be chained. The method must throw exception DuplicateMatchException if the same match has been already registered.
IsOrdered (comparator)
    the method decides whether the final scoreboard may be unambiguously established based on the so-far registered matches. The result is true if the scoreboard is unambiguous, false otherwise. The parameter is comparator - a function, functor, or a lambda-expression to call when a match result need to be converted into the win/draw/lost information (i.e. transforms _M into positive/zero/negative integer, see above). The method must not modify CContest instance and in must not throw any exceptions. 
Results (comparator)
    the method computes the scoreboard. Return value is STL list (std::list) of strings, each string represents the name of a contestant. The list is ordered from the absolute winner (first string in the list) to the last contestant. The method must throw OrderingDoesNotExistException if the list cannot be established unambiguously. Parameter comparator is similar to the IsOrdered method.

Submit a source file with the implementation of the CContest class template. Use the attached source code as a basis for your implementation. Please preserve the conditional compile directives - if present, the source may be locally tested and submitted to Progtest without manual modifications.

The assessment is divided into mandatory and bonus tests. The mandatory tests add only a few contestant/matches, thus naive algorithm working in time #contestants x #matches passes the time limits. An advanced algorithm is needed to pass the bonus test where the number of contestants/matches is high.

A naive approach may need to find all contestants directly or indirectly defeated by a particular contestant X. Use BFS (breadth first search) algorithm for this search. Further inspiration: PA1, proseminar #11, labyrinth problem.

The example source code may be found in the attached archive.
