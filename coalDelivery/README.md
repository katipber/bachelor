# Coal Delivery

This task is about implementation of [Ford-Fulkerson](https://en.wikipedia.org/wiki/Ford%E2%80%93Fulkerson_algorithm) algorithm on a [Directed Graph](https://en.wikipedia.org/wiki/Directed_graph). 

Solution was obtained by discretization of time steps, iteratively building the map and applyting Ford-Fulkerson on each step.

## Task Description

Approximately quarter of European electrical energy is obtained in coal fired power plants. Coal is delivered into power plants by freight trains that travel through European railroad network. These trains are tens of railway cars long and usually deliver at least hundreds tons of coal at once. Therefore, it is needed to precisely plan the route of each such train.

Last month European Union ratified a directive requiring higher degree of coordination among companies delivering coal. One of its requirements is that the companies plan coal deliveries mutually. As the directive comes into effect in just 16 days, there is little to none time to make appropriate changes. All coal-delivering companies thus rapidly began to invite applications for competitive tendering in the matter of required route scheduling. Because you see a possible easy profit in this area, you applied yourself.

The directive incorporates the following requirements:

* All trains will be required to transport the exact standardized amount of freight, which will allow to replenish reserves of any power plant by an arbitrary train.
* All trains will also be required to report in a network junction every hour on the hour, in order to ensure fulfillment of the delivery schedule.
* Every railroad between two junctions is operated in a single way only and it takes a train exactly one hour to travel through a single railroad.
* As there are usually multiple trains present in the network at the same time, for safety measures it is not allowed for two trains to meet at a single network junction at the same time.
* Mutual plan of deliveries must minimize the time it takes the last train to deliver its freight to a power plant.
* Companies must publish the schedule of all trains at their website.

Companies demand from you a program that creates a delivery schedule compliant with the requirements of the directive. Also, there must be exactly a single train ending its route at a particular power plant (this train will replenish coal resources of the power plant). To not complicate the task any further, there will be the same number of power plants as freight trains.

European Parliament provided companies a grant called "Sponsored Transport Logistics" and so it is possible to use STL library in your solutions.

#### Input format:

On the first line, there are three numbers N,M and K, specifying the number of network junctions, the number of railroads and the number of trains and power plants.
The numbering of network junctions is zero-based; their identifiers are thus 0, 1, ..., N-1.
On the second line there are K pairwise distinct numbers—starting locations of trains.
On the third line there are another K pairwise distinct numbers—locations of power plants.
After that M lines follow. Each such line contains two distinct numbers A_i, B_i from range 0 to N-1, which describe a railroad between two junctions. A railroad can be traversed only in the direction from junction A_i to junction B_i.
Railroad network doesn't contain multiple railroads between two junctions in the same direction. However, it is possible it contains railroads between two junctions in both directions.
It is not guaranteed every train can reach every power plant.
You can assume the input is valid.

#### Output format:

Output consists of K+1 lines.
On the first line, there is the minimal possible time T required to deliver coal supplies to power plants.
On the i-th of the remaining K lines, there is a travel schedule of the i-th train. Each such schedule consists of T+1 numbers -- identifiers of junctions in which the train was located at times 0, 1, ..., T.
Should there be multiple solutions, output an arbitrary one.
Should the solution not exist, output -1.

##### Classification Conditions:

To pass the compulsory tests (basic and small data tests), your solution has to return correct answers for railroad networks with at most 10 network junctions. (5 points)
To pass the test on large dataset, your solution has to return correct answers within the time and space limit for railroad networks of at most 300 network junctions and 1 000 railroads. (+10 points, 15 points total)

### Sample inputs and outputs:
##### Input example 1:

	4 5 2
	0 1
	2 3
	0 1
	0 2
	1 2
	1 3
	2 3

##### Output example 1:

	1
	0 2
	1 3

##### Input example 2:

	6 5 2
	0 1
	0 5
	0 1
	1 2
	2 3
	3 4
	4 5

##### Output example 2:

	4
	0 0 0 0 0
	1 2 3 4 5

##### Input example 3:

	6 6 3
	0 2 3
	2 4 5
	0 1
	1 2
	2 3
	3 4
	4 5
	5 0

##### Output example 3:

	2
	0 1 2
	2 3 4
	3 4 5

##### Input example 4:

	2 1 1
	0
	1
	1 0

##### Output example 4:

	-1

