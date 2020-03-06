/* Parallel Computing Homework#2: MultiThreading programming */
/* 
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
*/

#include <pthread.h>    /* Needed for threaded libraries */
#include <stdio.h>      /* Needed for C libraries */
#include <stdlib.h>     /* Needed for C libraries */
#include <time.h>       /* Needed for clock_gettime() function */ 


#define BILLION 1000000000.0  // For time function 
#define  listSize  100000
   // we chose this size to note the speed-up in threaded.
// if we chose small size, the thread be slower than sequential. 

int  unSortedList[listSize]; // list which has unsorted entry 
int  sortedList[listSize];   // resulted  list 
int finalSortedList[listSize];   // final resulted list 



//function to compare array elements , return zero if they equall. 
int compareArray(int a[],int b[],int size)	{
	int i;
	for(i=0;i<size;i++){
		if(a[i]!=b[i])
			return 1;
	}
	return 0;
}


int listSortedOrNot(int arr[], int arrSize){ 
   /* 
     This function used to check if we sorted our list successfuly or not:  
      -The basic idea for this function (recursive approach):
        * If size of array is zero or one, return true. 
        * Check last two elements of array, if they are sorted,
          perform a recursive call with arrSize -1,
          else, return false.
        * If all the elements will be found sorted, arrSize will finally be one, 
          so, satisfying the first note.  
   */
    if (arrSize == 1 || arrSize ==0)
         return 1; 
    if (arr[arrSize-1] < arr[arrSize-2])
         return 0;
   return listSortedOrNot(arr, arrSize -1);
}

  //Struct for sort threaded parameters. 
struct args1 {
   int startEndix;
   int endEndix; 
};


  //Struct for marge threaded parameters. 
struct args2 {
   int startEndix;
   int endEndix; 
   int  num; // num of threads (2 or 4)  
};



void *threadSort (void *input){
   struct args1 *in = (struct args1 *)input;
   int start = in->startEndix; 
   int end = in->endEndix; 
//  printf("%d   %d \n",start,end);
   int m ,i, j, temp;
/* printf ("Printing recived Array:\n" );  
 for(m=start; m<end+1; m++)
    printf(" %d\n", unSortedList[m]);
  */ 
//sorted function
     for(i=start; i<=end; i++){
         for(j=start; j< end; j++){
             if(unSortedList[j] > unSortedList[j+1]){
                temp = unSortedList[j];
                unSortedList[j] = unSortedList[j+1];
                unSortedList[j+1] = temp;
             }
         }
      } 
      pthread_exit(0); //kill the thread 
}//End of thread sort function


void *threadMerge (void *input){
   // in this function, we use: 
   // Merge Sort algorithem: focuses on how to merge together two pre-sorted arrays
   // such that the resulting array is also sorted. 
   struct args2 *in = (struct args2 *)input;
   int start = in->startEndix; 
   int end = in->endEndix; 
   int  n = in->num;
    int i,j, k; 
        i = k = start; 
        j = end/2 ;
       /* 
          Check if current element of first list is smaller than current element 
          of second list. If yes, store first list element and increment first list 
          index. Otherwise do same with second list. (here i used same list, but divided
          them by indexes as i work in 2 lists. )
        */
     int diff; 
     if (start != 0){
       diff = (end - start)/2 ; 
       j = j +diff;
     }else 
       diff = 0;
 
      while (i < ((end/2)+diff) && j < end ){
           if (unSortedList[i] < unSortedList[j]){
             sortedList[k]= unSortedList[i];
             i++;
            }else {
             sortedList[k]= unSortedList[j];
             j++;
            }
          k++;  
      }   

    // Store remaining elements of first list
     while (i < ((end/2)+diff)){
        sortedList[k]= unSortedList[i];
             i++;
             k++;
     }

    // Store remaining elements of second list
     while (j < end){
        sortedList[k]= unSortedList[j];
             j++;
             k++;
     } 
}// End of marge function 

void *threadFinalMerge (void *input){
   //This function for marge  final result .  
   struct args2 *in = (struct args2 *)input;
   int start = in->startEndix; 
   int end = in->endEndix; 
   int  n = in->num;
    int i,j, k; 
        i = k = start; 
        j = end/2 ;

 while (i < (end/2) && j < end ){
           if (sortedList[i] < sortedList[j]){
             finalSortedList[k]= sortedList[i];
             i++;
            }else {
             finalSortedList[k]= sortedList[j];
             j++;
            }
          k++;  
      }   

    // Store remaining elements of first list
     while (i < (end/2)){
        finalSortedList[k]= sortedList[i];
             i++;
             k++;
     }

// Store remaining elements of second list
     while (j < end){
        finalSortedList[k]= sortedList[j];
             j++;
             k++;
     } 
}// End of FinalMarge function 

int main (int argc, char *argv[]){
    
    int numOfThreads; 
// check for command line entering ... 
    if (argc == 2)
       numOfThreads =atoi(argv[1]);
    else{
      printf("--------------------------------------------------\n");
      printf("You must enter just two argument in command line:\n *The first one is \\.filename\n *The second one is number of threads.\n ");    
      printf("--------------------------------------------------\n");
      printf(" Bye :P\n");
      exit(0);
      }

   /*
     - Fill the global unsorted list with random integers
     - make temporary list to sort it sequentially 
   */  
     printf("\n Hello ...\n");
     printf("--------------------------------------------------\n");
//Started sequantial part. 
     printf(" Sequantial part started!\n");
     int i, tempList[listSize];
     for (i=0; i<listSize; i++ ){
          unSortedList[i] = tempList[i] = (rand() % 1000) +1;
          }

  /* Sort temporary list sequentially */
     int j,temp;
     /*
       Find execution time of our sequential code: 
        - we use clock_gettime() function, which take 2 arguments, the first one is the 
          clock type, and the second argument is a pointer to timespec structute. 
        - timespec structure is contain time metured in secound & nanosecounds, defined as:
         
           struct timespec {
                time_t   tv_sec;      // seconds 
                long     tv_nsec;     // nanoseconds 
              }; 
      */

     struct timespec startTimeSeq, finishTimeSeq;//variables for compute the execution time.
     double totalTimeSeq; // total execution time. 
     clock_gettime(CLOCK_REALTIME,&startTimeSeq); // start compute real time. 
     // Sort function 
     for(i=0; i< listSize; i++){
         for(j=0; j< listSize-i-1; j++){
             if(tempList[j] > tempList[j+1]){
                temp = tempList[j];
                tempList[j] = tempList[j+1];
                tempList[j+1] = temp;
             }
         }
      } 
     clock_gettime(CLOCK_REALTIME,&finishTimeSeq); //Finish compute real time.
     // total = finish - start  
     totalTimeSeq = ((finishTimeSeq.tv_sec - startTimeSeq.tv_sec) + ((finishTimeSeq.tv_nsec - startTimeSeq.tv_nsec) / BILLION));
     printf("  *Execution time for sequential code = %f seconds\n", totalTimeSeq);
/*     
   printf("printing tempList  after sorting:\n");
    for (i=0; i<listSize; i++ ){
          printf("%d\n",tempList[i]);
          }
*/
      
      /* Check if the list sorted or not without print it */ 
      if (listSortedOrNot(tempList,listSize))
         printf("  *Successfully sorted ^_^\n");
      else 
         printf("Not sorted :(\n");

  
     printf(" Sequantial part finished!\n");
     printf("--------------------------------------------------\n");
//End of sequential part.

/*----------------------------------------------------------------------------------------------------------------------------------------------------------*/

//Start of threaded part. 
     printf(" Threaded part started!\n");
     struct timespec startTimeThr, finishTimeThr;//variables for compute the execution time.
     double totalTimeThr; // total threaded execution time. 
     // check the number of threads, which you entered! (2 or 4)
     // if 2 threads, we also need  1 thread to mearge result.
     if (numOfThreads == 2){
         printf("  *You work with 2 thread for sorting, so we need also 1 thread for marge the result.\n "); 
         pthread_t threadID[numOfThreads + 1]; // 3 threads id's
         struct args1 *data1 = (struct args1 *)malloc(sizeof(struct args1));//create struct contain threads parameters
         data1->startEndix =0;//start index (from unSortedList) to thread 0
         data1->endEndix = (listSize/numOfThreads) -1;//end index (from unSortedList) to thread 0 
         clock_gettime(CLOCK_REALTIME,&startTimeThr); // start compute real time. 
         for (i=0; i<numOfThreads; i++){
         pthread_create(&threadID[i], NULL, threadSort, (void *)data1); //creat  threads 0 and 1 
         data1 = (struct args1 *)malloc(sizeof(struct args1));
         data1->startEndix = listSize/numOfThreads; //start index (from unSortedList) to thread 1
         data1->endEndix = listSize -1; //end index (from unSortedList) to thread 1
         }

         /* waiting for the 2 sorting threads to finish (0 and 1) */
         for (i = 0; i < 2 ; i++)
         pthread_join(threadID[i], NULL);

         /* establish the merge thread (2)*/
          struct args2 *data2 = (struct args2 *)malloc(sizeof(struct args2));
          data2->startEndix = 0;
          data2->endEndix = listSize;
          data2->num = numOfThreads;
          pthread_create(&threadID[2], NULL, threadMerge,(void *) data2);

         /* wait for the merge thread to finish */
          pthread_join(threadID[2], NULL);   
      
          clock_gettime(CLOCK_REALTIME,&finishTimeThr); //Finish compute real time.
          // total = finish - start  
          totalTimeThr = ((finishTimeThr.tv_sec - startTimeThr.tv_sec) + ((finishTimeThr.tv_nsec - startTimeThr.tv_nsec) / BILLION));
          printf(" *Execution time for threaded code = %f seconds\n", totalTimeThr);

          /* Check if the list sorted or not without print it */ 
          if (listSortedOrNot(sortedList,listSize))
          printf("  *Successfully sorted ^_^\n");
          else 
          printf("  *Not sorted :(\n");

         printf(" Threaded part finished!\n");
         printf("--------------------------------------------------\n");

      }
      // if 4 threads, we also need 2 threads to mearge the result for every 2 threads, then we need 1 thread to marge final result.
      else if (numOfThreads == 4){
         printf(" *You work with 4 thread for sorting,\n  so we need also 2 threads to marge the result for every 2 thrads,\n  Then we need 1 thread to marge the final result.\n");
         pthread_t threadID[numOfThreads + 3]; // 7 threads id's
         struct args1 *data1 = (struct args1 *)malloc(sizeof(struct args1));//create struct contain threads parameters.
         data1->startEndix =0;//start index (from unSortedList) to thread 0 
         data1->endEndix = (listSize/numOfThreads) -1;//end index (from unSortedList) to thread 0 
         int diff = (data1->endEndix - data1->startEndix); //diff+1: number of entry for each thread 
         clock_gettime(CLOCK_REALTIME,&startTimeThr); // start compute real time. 
         for (i=0; i<numOfThreads; i++){
         pthread_create(&threadID[i], NULL, threadSort, (void *)data1); //creat  threads
         data1 = (struct args1 *)malloc(sizeof(struct args1));
         data1->startEndix = listSize-(listSize-((i+1)*(diff+1)));
         data1->endEndix = (listSize-(listSize-((i+1)*(diff+1)))) +diff ; 
         }
         /* waiting for the 4 sorting threads to finish */
         for (i = 0; i < 4 ; i++)
          pthread_join(threadID[i], NULL);


          /* establish first merge thread */
         struct args2  *data2 = (struct args2 *)malloc(sizeof(struct args2));//create struct c$
         data2->startEndix =0;
         data2->endEndix = (listSize/2);
         data2->num = numOfThreads;
         pthread_create(&threadID[4], NULL, threadMerge, (void *)data2); //creat  threads
         
         /* establish second  merge thread */
         data2 = (struct args2 *)malloc(sizeof(struct args2));
         data2->startEndix = listSize/2;
         data2->endEndix = listSize;
         pthread_create(&threadID[5], NULL, threadMerge, (void *)data2); //creat  threads

         /* waiting for the 2 merge threads to finish */
         for (i = 4; i < 6 ; i++)
         pthread_join(threadID[i], NULL);

         /* establish final  merge thread */
         data2 = (struct args2 *)malloc(sizeof(struct args2));
         data2->startEndix = 0;
         data2->endEndix = listSize; 
         pthread_create(&threadID[6], NULL, threadFinalMerge, (void *)data2); //creat  threads

        /* waiting for final merge threads to finish */
        pthread_join(threadID[6], NULL);
        
          clock_gettime(CLOCK_REALTIME,&finishTimeThr); //Finish compute real time.
          // total = finish - start  
          totalTimeThr = ((finishTimeThr.tv_sec - startTimeThr.tv_sec) + ((finishTimeThr.tv_nsec - startTimeThr.tv_nsec) / BILLION));
          printf(" *Execution time for threaded code = %f seconds\n", totalTimeThr);

          /* Check if the list sorted or not without print it */ 
          if (listSortedOrNot(finalSortedList,listSize))
          printf("  *Successfully sorted ^_^\n");
          else 
          printf("  *Not sorted :(\n");


         printf(" Threaded part finished!\n");
         printf("--------------------------------------------------\n");
      }
      else {
          printf(" You can use just 2 or 4 threads, retry!\n ");
          printf("--------------------------------------------------\n");
      }

/*
  printf("printing sortedList:\n");
    for (i=0; i<listSize; i++ ){
          printf("%d\n",finalSortedList[i]);
          }

*/

      if (numOfThreads == 2 || numOfThreads == 4){
         // conclution
           printf(" Result:\n ");

            //  check if sorted list sequantally = the sorted list using multithreading
     int check; 
      if (numOfThreads == 2)  
           check =  compareArray(tempList, sortedList, listSize);
       else if (numOfThreads == 4)
           check =  compareArray(tempList, finalSortedList, listSize);

           if (check == 0 )
            printf("*Same sorting result in parallel and sequantial.\n  ");  
           else 
           printf("*Not same sorting result in parallel and sequantial !!!\n  ");

           //speed-up 
           printf("  *We chose a very big size for our list (%d elements), to note the speed-up in threaded.\n   Because if we chose small size, the thread maybe slower than sequential.\n",listSize) ;
           printf("  *Threaded Speedup = %f\n ",(totalTimeSeq/totalTimeThr));
           printf("--------------------------------------------------\n");
       }
       
       printf(" Bye :P\n");
  
 return 0;
}//End of main 
