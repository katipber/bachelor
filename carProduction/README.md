# Car Production



## Task Description

 Although the prototype of a new car model pleases designers and managers of the local car factory, it brings nothing but nightmares to engineers responsible for planning the production process. That is because designers let their imagination go wild and thus to complete the production of a single car, it is required to successively assemble a large amount of partial components. Moreover, it often happens that a particular component cannot be assembled earlier than some other components, as the assembly might require another components to be already assembled beforehand (e.g. the assembly of a car door requires a lock, a handle, a window mechanism, a window, a gasket, etc.).

Due to the complexity of the construction of the new car model, it is needed to precisely schedule its production process. Otherwise it would be imminent that the time needed to produce a single car would be unprofitable in the long term. As this is no task that could be solved by hand (due to the large number of components), you were asked to help the factory. In particular, for a given production plan, which specifies the dependencies during the assembly, and for given times needed to assembly each component, you are asked to schedule when should the assembly of each of the components begin, in order to minimize the total time needed to produce a car (to produce a car, it is needed to assembly all components specified in the production plan).

In the input you are given the number of components, the times needed to assembly each component, and the production plan, which for each component specifies the components that are to be assembled before the assembly of such component can begin. It is not possible to start the assembly of a component before all of the components, on whose its assembly depends, are assembled. Should the assembly of a component depend on no other component, it is possible to start its assembly immediately. If a component is assembled at time x, it is possible to immediately use it to assembly another component also at time x. Assume there is an infinite number of workers available at all times. This means it is possible to assembly arbitrary many components at the same time. There might occur a situation, when designers make a mistake in the production plan, such that it is, for some reason, impossible to schedule the production of a car. You have to detect such cases. It is also possible there exist multiple solutions that minimize the total time needed to produce a car, but the assembly times for components differ. In such case output an arbitrary optimal solution.

In the light of the recent dieselgate affair, all software used to manipulate the results of emissions testing is forbidden to be used in car factories. This measure also includes STL library and so it is not possible to use it to solve this task.

#### Input Format:

    On the first line there is a single number N specifying the number of components needed to produce a car.
    The numbering of components is zero-based; their numbers are thus 0, 1, ..., N - 1.
    On the next line there are N numbers t0, t1, ..., tn-1, 0 < ti < 109. The i-th number ti specifies the time needed to assembly the i-th component.
    After that N lines follow, successively for components 0 to N - 1. In the beginning of each row is a number describing the number of components needed to assembly the corresponding component (this number may also be 0), followed by a space-separated list of numbers of those components.
    No component is listed in its own dependency list and no component is listed multiple times in a particular dependency list.
    The production plan always consists of at least a single component, but there might be no dependencies in between components whatsoever. This implies there might exist pairs of components that do not depend on each other (not even transitively through another components)
    You can assume the input is valid.

#### Output Format:

    If it is not possible to schedule the production of a car for a given production plan, your program should output a single line with text "No solution." (without quotes).
    If a solution exists, the output should consist of two rows. On the first one output the minimal total time needed to assembly a car. On the second line output N space-separated numbers, where the i-th such number specifies the time, when the assembly of the i-th component should begin.

#### Classification Conditions:

    To pass the compulsory tests (basic and small data tests), your solution has to return correct answers for production plans of at most 10 components and 20 dependencies.
    To pass the test on medium dataset, your solution has to output correct answers within the time and memory limit for production plans of at most 100 components and 1 000 dependencies.
    To pass the test on large dataset, your solution has to output correct answers within the time and memory limit for production plans of at most 100 000 components and 1 000 000 dependencies.

#### Examples:
Input Example 1:

6             
3 5 7 2 2 4 
0           
0           
2 0 1       
2 2 1       
0           
1 4 

Output Example 1:

14
0 0 5 12 0 2

Input Example 2:

4
6 3 5 4
0
2 0 3
1 1
1 2

Output Example 2:

No solution.


