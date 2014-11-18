#include <sys/time.h>
#include <stdio.h>

#define KB 1024
#define MB KB * KB
#define dataSize 64 * MB
#define minSize 4 * KB

char x[dataSize];

struct timeval startTime, endTime;
int i, repeat;

double getTimeDifference(struct timeval* start, struct timeval* end)
{
 return  (end->tv_sec + end->tv_usec / 1000000.0) - (start->tv_sec + start->tv_usec / 1000000.0);
}

void printSizeTime(int size, double time)
{
printf("%d\t\t%lf", size, time);
}

double loop(int numOfLoop, int numBytesToMove, int bytesToJump)
{
    gettimeofday(&startTime, NULL);      

    for(repeat = 1; repeat <= numOfLoop; repeat++)
      {
       for(i = 0; i < numBytesToMove; i += bytesToJump)
       {	  
	  x[i]++;	 
	}
      }

      gettimeofday(&endTime, NULL);
      
return  getTimeDifference(&startTime, &endTime);
}

int main()
{ 
  struct timeval totalExecStart, totalExecEnd;

  double times[20], sizes[20];

  gettimeofday(&totalExecStart, NULL);

int i, j, cacheSize[4];
  
 double numBytesToMove;
 int blockSize,  numOfLoop;
 int recentCacheIndex; 
  double timeforFiftyLoops;
  
  int loopFactor;
    double sizeFactor, k;

printf("Cache size\n");
  
timeforFiftyLoops = loop(50, dataSize, 64);

 loopFactor = (int) 750 / timeforFiftyLoops; //adjust so that each jump will take around 20 seconds
  
  printf("%d\n", loopFactor);
  
  numBytesToMove = dataSize;
  
  double timeTaken, prevTime, difference;

 sizeFactor = 3.0/4;
 k = 8.0/9;

 i = 0;

  while(numBytesToMove >= minSize)
  {
      numOfLoop = (dataSize/ numBytesToMove) * loopFactor;
     
      timeTaken = loop(numOfLoop, (int)numBytesToMove, 64);

      times[i] = timeTaken;
      sizes[i] = numBytesToMove;
	i++;
      
      if(numBytesToMove > 2 * MB)
	{
	  numBytesToMove *= sizeFactor;
	  sizeFactor *= k;
	  k = 1 / k;
	}	
      else
	{
	  numBytesToMove /= 2;
	}
   } 

  prevTime = times[0];
  recentCacheIndex = 0;

  for(i = 0; i < 20; i++)
  {
     difference = (prevTime - times[i]) / prevTime * 100;
     printSizeTime(sizes[i], times[i]); printf("\t%lf\n", difference);
     prevTime = times[i];

     if(difference > 23 && recentCacheIndex != i - 1)
       {
       printf("Is it a cache %lf\n", sizes[i]);
       recentCacheIndex = i;
       }
  }
  printf("%s\n", "Block Size");

/*
  //BLOCK SIZE
cacheSize[0] = 32 * KB;
cacheSize[1] = 2048 * KB;
cacheSize[2] = 3 * MB;
cacheSize[3] = 0;

//numBytesToMove = 64 * KB; //L3 cache for now

for(i = 0; i < 2; i++)
  {
printf("L%d Cache\n", i + 1);
numBytesToMove = cacheSize[i];

 blockSize = 256;
timeTaken = 0, prevTime = 0;   
loopFactor = (10000 / (i + 1)) / ((i + 1) * (i + 1) / (i + 1)) *  1000.0 * KB / numBytesToMove;
printf("%d\n", loopFactor);

  while(blockSize >= 4)
  {
   numOfLoop = blockSize * loopFactor;

   prevTime = timeTaken;
   timeTaken = loop(numOfLoop, numBytesToMove, blockSize);
   
   printSizeTime(blockSize, timeTaken); printf("\t%lf\n", prevTime / timeTaken);

   blockSize /= 2;
  }
  }*/
/*numBytesToMove = 64 * MB;
numOfBlocksInCache = 3 * MB / 64;

for(setAssoc = 2; setAssoc <= 64; setAssoc *= 2)
  {
   numOfSetsInCache = numOfBlocksInCache / setAssoc;

   for(i = 1; i <= setAssoc + 1; i++)  
   {
     
   }
  }
*/
gettimeofday(&totalExecEnd, NULL);
  
  printf("Total exec time: %lf\n", getTimeDifference(&totalExecStart, &totalExecEnd));
}
