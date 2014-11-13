#include <sys/time.h>
#include <stdio.h>
#define dataSize 64 * 1024 * 1024

char x[dataSize];

double getTimeDifference(struct timeval* start, struct timeval* end)
{
 return  (end->tv_sec + end->tv_usec / 1000000.0) - (start->tv_sec + start->tv_usec / 1000000.0);
}

int main()
{ 
  struct timeval totalExecStart, totalExecEnd, startTime, endTime;

  gettimeofday(&totalExecStart, NULL);

  double t1, t2;
  int i,j, sizeFactor, repeat ;

  int maxSizeFactor = 1024 * 16;
  
  int numBytesToMove, numOfLoop;
  
  double timeforFiftyLoops;
  
  int repeatFactor;

  gettimeofday(&startTime, NULL);
  
  for(j = 1; j <= 50; j++)
    {
      for(i = 0; i < dataSize; i += 32)
	{
	  x[i]++;
	}
    }

  gettimeofday(&endTime, NULL);
  
  timeforFiftyLoops = getTimeDifference( &startTime, &endTime);

  repeatFactor = (int) 1000 / timeforFiftyLoops; //adjust so that each jump will take around 20 seconds
  
  printf("%d\n", repeatFactor);

  for(sizeFactor = 1; sizeFactor <= maxSizeFactor; sizeFactor *= 2)
  {
      numOfLoop = sizeFactor * repeatFactor;
      numBytesToMove = dataSize / sizeFactor;
      
      gettimeofday(&startTime, NULL);      

    for(repeat = 1; repeat <= numOfLoop; repeat++)
      {
       for(i = 0; i < numBytesToMove; i += 32)
       {	  
	  x[i]++;	 
	}
      }

      gettimeofday(&endTime, NULL);
      
      printf(" %d\t\t%lf\n", dataSize/sizeFactor, getTimeDifference(&startTime, &endTime));

    } 
  gettimeofday(&totalExecEnd, NULL);
  
  printf("Total exec time: %lf\n", getTimeDifference(&totalExecStart, &totalExecEnd));
}
