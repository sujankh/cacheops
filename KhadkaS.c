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
      
//printf(" %d\t\t%lf\n", numBytesToMove, getTimeDifference(&startTime, &endTime));
//printer(numBytesToMove, getTimeDifference(&startTime, &endTime), bytesToJump);
return  getTimeDifference(&startTime, &endTime);
}

int main()
{ 
  struct timeval totalExecStart, totalExecEnd;

  gettimeofday(&totalExecStart, NULL);

int i, j, cacheSize[4];
  
int numBytesToMove, blockSize,  numOfLoop;
  
  double timeforFiftyLoops;
  
  int loopFactor;

printf("Cache size\n");
  
timeforFiftyLoops = loop(50, dataSize, 64);

 loopFactor = (int) 1000 / timeforFiftyLoops; //adjust so that each jump will take around 20 seconds
  
  printf("%d\n", loopFactor);
  
  numBytesToMove = dataSize;
  
double timeTaken, prevTime;

prevTime = 0, timeTaken = 0;

  while(numBytesToMove >= minSize)
  {
      numOfLoop = (dataSize/ numBytesToMove) * loopFactor;
prevTime = timeTaken; 
      timeTaken = loop(numOfLoop, numBytesToMove, 64);

      printSizeTime(numBytesToMove, timeTaken); printf("\t%lf\n", (prevTime - timeTaken) / prevTime * 100);
      
     if(numBytesToMove > 8 * MB || numBytesToMove <= 1 * MB)
	numBytesToMove /= 2;
      else
	numBytesToMove -= MB;
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
