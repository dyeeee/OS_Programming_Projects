## Requirements:
Write a multithreaded program that implements the banker’s algorithm. Several customers request and release resources from the bank. The banker will grant a request only if it leaves the system in a safe state. A request that leaves the system in an unsafe state will be denied. 
Invoke the program by passing the number of resources of each type on the command line.

## Procedure：
1. Define array for available resources, maximum matrix, allocation matrix and need matrix.
2. Define threads for customers.
3. Declaration request_resources function, release_resources function and safety check function.
4. In main function:

a)Check the parameters from command line, if correct, pass to the available resources array.

b)Initialize mutex
  
c)Create Customers’ threads, pass threads to the customerBehaviour function.

d)Wait and join the threads.

5. In customerBehavior function:

a)Randomly create the maximum resources number(smaller than the maximum available resources)

b)Request the resources by request_resources function.

c)Until need equals to allocation, customers regenerate request.

6. In request_resources function:
a)Step 1: whether request is more than need (For get a shorter running time, when randomize the request, the requests would smaller than its need.  So step1 would not be used actually)

b)Step 2: whether request is more than available.

c)Step 3: Allocate the resources

d)Step 4: Check if there exist a safety sequences. If not roll back the statement before Step3.


7. In safety check function: (the process is the same from book)
a)Initialize Work and Finish

b)Find Pi for need[] <= work[]

c)Finish[i] = true, work[i] += allocation[i]

d)If all finish are true, there have a safety sequence.

8. Display function. Show the MAX matrix, allocation matrix and available array.
