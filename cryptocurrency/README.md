# Cryptocurrency


## Task Description

The aim is to develop a class to quickly mine cryptocurrencies.

Cryptocurrencies are very popular nowadays, thus even our university and our faculty decided to start an ICO of their own cryptocurrencies. Computation-expensive operations are used to secure and mine the currency. Your task is to implement a program to mine the currency efficiently, that is, using threads and all available CPUs.

Unfortunately, there is a disagreement between the CVUT and FIT, thus there are two different mining algorithms resulting in two variants of the currency - fitCoin and cvutCoin.

fitCoin
    is given an array of 32 bit unsigned integers - bit vectors. Next, there is given a distance dist. The mining algorithm searches for 32 bit integers x such that the number of mismatched bits between x and any input vector is smaller than dist. The output of the computation is the count of such x. The computation is demonstrated in the following example (the example uses just 4 bits instead of 32 bits). The input vectors are:

      0011
      0101
      0110
      

    The the number of mismatched bits of the possible values x is:

              ≤0     ≤1     ≤2     ≤3     ≤4
      0000                   *      *      *
      0001                          *      *
      0010                          *      *
      0011                   *      *      *
      0100                          *      *
      0101                   *      *      *
      0110                   *      *      *
      0111            *      *      *      *
      1000                          *      *
      1001                                 *
      1010                                 *
      1011                          *      *
      1100                                 *
      1101                          *      *
      1110                          *      *
      1111                   *      *      *
      Count:   0      1      6     13     16
      

    That is, if the above input vectors are given, and the dist is set to 3, the result is 13 (this is of course limited to the 4-bit version).
cvutCoin
    is given an array of bytes. The array is processed as a sequence of bits, starting from the LSB (least significant bit) of the first byte to the MSB (most significant bit) of the last byte. If the input array is n bytes long, the sequence length is 8n bits. Next, the algorithm is given two integers distMin and distMax. The problem is to take all non-empty prefixes and all non-empty suffixes and form pairs (prefix ; suffix). Then the edit distance is established for each such pair. The result is the count of all such pairs where the edit distance fits into the closed interval [ distMin ; distMax ]. Edit distance is measured as the least number of insert/delete/replace operations that convert one string into the other.

Your task is to implement class CRig, which can solve such tasks. Both tasks demand computing resources and both need to be solved quickly. Thus we will use threads to split the load to more CPUs and solve task asynchronously.

Class CRig should compute solutions of both tasks perpetually. The tasks are placed by customers (instances of class CCustomer, created by the test environment and passed to the CRig instance). Customers pass tasks (i.e. instances of classes CFITCoin and CCVUTCoin). Your implementation of CRig picks up instances of the tasks, processes them, and returns them solved to the appropriate customer.

Your implementation should create work threads, the number of work threads is given as a parameter at the start. Then create three helper threads per each customer, these threads will pick up the tasks, i.e., these threads will call corresponding methods of CCustomer. The first thread will call delivery method for tasks of type fitCoin, the second for tasks of type cvutCoin. Finally, the third thread will pass the solved problems back to the customer. The problems are placed as instances of class CFITCoin and CCVUTCoin (as smart pointers - shared_ptr<CFITCoin> and shared_ptr<CCVUTCoin>, respectively. There exist aliases AFITCoin and ACVUTCoin for shortening). The purpose of helper threads is to pick up the tasks, but they are not designated to solve them. Instead, they pass them to the work threads.

You should create several work threads (based on the initialization parameter. Work threads solve given task instances and place the results into the fields of AFITCoin / ACVUTCoin instance. Having these fields filled, the work threads pass the instance to the submission thread of the corresponding customer. The submission thread then passes the solved instance to the CCustomer interface. Moreover, the submission needs to serialize the calls to the CCustomer methods.

As it was already mentioned, helper threads should get requests from customers and pass them to the work threads. The total number of helper threads is three times the number of customers being served. If helper thread solved tasks directly and there were a huge amount of customers, there would be a huge CPU overhead (too many open tasks, frequent context switches, ...). The expected solution is to process computations in work threads only, there is only a given fixed number of them, thus there is a better control of the overhead.

The class CRig is an interface to your application. The class should contain methods as described below (there can exist other private methods required for your implementation):

implicit constructor
    It initializes a new class instance.
destructor
    It frees resources allocated by the instance of CRig.
Start (thr)
    It starts the computation. You should create required work threads for the computation in this method. The number of work threads is given by thr parameter. The threads start processing of the requests from the customers. However, Start method finishes immediately after it starts the work threads, i.e., it does not wait for their completion.
Stop ()
    This method informs about the end of processing. It is required to pick up the remaining requests from the existing customers, wait for their completion, and return of results. Stop method finishes and returns to the caller after the termination of all helper and work threads. Stop method must return to the caller. Don't stop the program (don't call exit or similar functions), if you terminate the whole program, your evaluation would be 0 points.
AddCustomer ( c )
    This method adds a further customer to the list of customers for this instance. The parameter c is a smart pointer (shared_ptr<CCustomer>, ACustomer shortened) to the new customer. The method AddCustomer must create three helper threads for getting customer's requests and for the submission of the completed requests. Attention: the method can be called either before Start method call (i.e. customers are served, but work threads do not exist yet), or after Start (the new customer is added to the existing list of customers while the other customers are already being served and their problems are being computed).
Solve (fitCoin)
    The method sequentially calculates one task of type AFITCoin (parameter). The test environment tests sequential solution first. This simplifies the testing of the underlying algorithm.
Solve (cvutCoin)
    The method calculates sequentially one task of type ACVUTCoin (parameter). The test environment tests sequential solution first. This simplifies the testing of the underlying algorithm.

Class CCustomer defines the interface of a customer. The customer is implemented is the test environment. Its instance is passed to your implementation as a smart pointer (shared_ptr<CCustomer> alias ACustomer). The CCustomer interface has methods:

destructor
    It frees resources allocated by the instance,
FITCoinGen() / CVUTCoinGen()
    These methods return another instance of the fitCoin/cvutCoin task to be processed. They return a smart pointer (shared_ptr<CFITCoin> alias AFITCoin or shared_ptr<CCVUTCoin> alias ACVUTCoin), which contain parameters of the problem. If an empty smart pointer is returned (i.e. it contains NULL value), it means, that the current customer does not have any other task of the corresponding type (but it still can supply tasks of the other type). An empty pointer indicates that you can finish the corresponding helper thread.
FITCoinAccept ( fitCoin ) / CVUTCoinAccept ( cvutCoin )
    This method is the interface to return the solved task of the corresponding type back to the customer. It is required that the solved task is returned to the customer, who assigned it. Next, it is also a must to use the instance passed by the method FITCoinGen / CVUTCoinGen, it is just necessary to fill in the calculated values. Finally, the calls to the accept methods must be synchronized from the submission thread, the submission thread must be the same for the life time of the customer.

The class CFITCoin is declared and implemented in the test environment. It is included in the conditional compilation block for your own tests (leave it as it is). It has fields defined:

m_Vectors
    a list of 32 bit vectors that define the problem instance. The value is set by the testing environment, your implementation can read it, but is not allowed to modify it.
m_DistMax
    is the maximum distance from the bit vectors. The value is set by the testing environment, your implementation can read it, but is not allowed to modify it.
m_Count
    is the result of the computation - the number of 32 bit values x such that their distance is at most m_DistMax from any of the bit vectors. The value is filled by the worker thread.
default constructor, destructor
    helper code to simplify the creation and testing.

The class CCVUTCoin is declared and implemented in the test environment. It is included in the conditional compilation block for your own tests (leave it as it is). It has fields defined:

m_Data
    the bytes that form the bit sequence to analyze. The bits are read from LSB to MSB. The value is set by the testing environment, your implementation can read it, but is not allowed to modify it.
m_DistMin, m_DistMax
    the range of distances in the comparison. The values are set by the testing environment, your implementation can read them, but is not allowed to modify them.
m_Count
    is the result of the computation filled by the worker thread. The value is the number of pairs ( prefix ; suffix ) from the input bit sequence such that the edit distance of prefix and suffix fits into the interval [ m_DistMin ; m_DistMax ].
default constructor, destructor
    helper code to simplify the creation and testing. 

