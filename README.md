# Multithreading-programming
 How our code work ?
   A) we have unsorted list called: (unSortedList), contain a random integer. 
   B)Firs step: we need to sort it sequantaly, then:
      -compute execution time: using clock_gettime(). 
      -prove that our unsorted list has become sorted: using recursiveapproach.  
   C)Second step: we need to sort it using thread, then:
      -compute execution time: using clock_gettime(). 
      -prove that our unsorted list has become sorted: using recursiveapproach.   
   D)Result: 
      -Compute speed-up.
      
NOTES: 
1) you can compile the code using: ( gcc -pthread -o parallel_HW#2  parallel_HW#2.c )
2) you can run the code using: ./parallel_HW#2 X, which X = number of sorted threads you want to use (2 / 4). 
3) We chose a very big size for our unsortedList (100000 elements), to note the speed-up in threaded, because if we chose small size,
   the thread program maybe slower than sequential program due to overhead for syncronization.
