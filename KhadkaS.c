#include <sys/time.h>
#include <stdio.h>

#define KB 1024
#define MB KB * KB
#define dataSize 64 * MB
#define minSize 4 * KB

char x[dataSize];

struct timeval startTime, endTime;
int i, repeat;
void (*printer)(int, double, int);

double getTimeDifference(struct timeval* start, struct timeval* end)
{
 return  (end->tv_sec + end->tv_usec / 1000000.0) - (start->tv_sec + start->tv_usec / 1000000.0);
}

void printCacheSize(int sizeMoved, double time, int bytesJumped)
{
printf("%d\t\t%lf\n", sizeMoved, time);
}

void printBlockSize(int sizeMoved, double time, int bytesJumped)
{
printf("%d\t\t%lf\n", bytesJumped, time);
}

void loop(int numOfLoop, int numBytesToMove, int bytesToJump)
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
printer(numBytesToMove, getTimeDifference(&startTime, &endTime), bytesToJump);
}

int main()
{ 
  struct timeval totalExecStart, totalExecEnd;

  gettimeofday(&totalExecStart, NULL);

  int i, j;
  
int numBytesToMove, blockSize,  numOfLoop;
  
  double timeforFiftyLoops;
  
  int loopFactor;
/*
printf("Cache size");

  gettimeofday(&startTime, NULL);
  
   for(j = 1; j <= 50; j++)
    {
      for(i = 0; i < dataSize; i += 64)
	{
	  x[i]++;
	}
    }

  gettimeofday(&endTime, NULL);
  
  timeforFiftyLoops = getTimeDifference( &startTime, &endTime);

  loopFactor = (int) 1000 / timeforFiftyLoops; //adjust so that each jump will take around 20 seconds
  
  printf("%d\n", loopFactor);
  
  numBytesToMove = dataSize;
  
printer = printCacheSize;

  while(numBytesToMove >= minSize)
  {
      numOfLoop = (dataSize/ numBytesToMove) * loopFactor;
       
loop(numOfLoop, numBytesToMove, 64);

      if(numBytesToMove > 8 * MB || numBytesToMove <= 1 * MB)
	numBytesToMove /= 2;
      else
	numBytesToMove -= MB;
   } 

//loop(1, 64 * MB, 64);
*/
printf("%s\n", "Block Size");

printer = printBlockSize;

  //BLOCK SIZE
numBytesToMove = 64 * KB; //L3 cache for now
  blockSize = 256;
   
  while(blockSize >= 4)
  {
numOfLoop = blockSize * 800;
   loop(numOfLoop, numBytesToMove, blockSize);
   blockSize /= 2;
  }

gettimeofday(&totalExecEnd, NULL);
  
  printf("Total exec time: %lf\n", getTimeDifference(&totalExecStart, &totalExecEnd));
}
