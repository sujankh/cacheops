#include <sys/time.h>
#include <stdio.h>
#define dataSize 64 * 1024 * 1024

char x[dataSize];

int main()
{ 
  struct timeval totalExecStart, totalExecEnd, startTime, endTime;
  
  gettimeofday(&totalExecStart, NULL);
  double t1, t2;
  int i, sizeFactor, repeat ;

  int maxSizeFactor = 1024 * 16;
  
  int numBytesToMove, numTimesToRepeat;

  for(sizeFactor = 1; sizeFactor <= maxSizeFactor; sizeFactor *= 2)
  {
      gettimeofday(&startTime, NULL);
      t1 = startTime.tv_sec + startTime.tv_usec / 1000000.0;
      
      numTimesToRepeat = sizeFactor * 2500;
      numBytesToMove = dataSize / sizeFactor;

    for(repeat = 1; repeat <= numTimesToRepeat; repeat++)
      {
       for(i = 0; i < numBytesToMove; i += 32)
       {	  
	  x[i]++;	 
	}
      }
      gettimeofday(&endTime, NULL);
      t2 = endTime.tv_sec + endTime.tv_usec / 1000000.0;
      printf(" %d\t\t%lf\n", dataSize/sizeFactor, t2 - t1);

    } 
  gettimeofday(&totalExecEnd, NULL);
  
  printf("Total exec time: %lf\n", (totalExecEnd.tv_sec + totalExecEnd.tv_usec / 1000000.0) - (totalExecStart.tv_sec + totalExecStart.tv_usec / 1000000.0));
}
