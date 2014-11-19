#include <sys/time.h>
#include <stdio.h>

#define KB 1024
#define MB KB * KB
#define dataSize 64 * MB
#define minSize 4 * KB

char x[dataSize];
struct timeval startTime, endTime;
int  numOfLoop, loopFactor, repeat, cacheSize[4], blockSize;
double numBytesToMove, timeTaken, prevTime, difference;

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
  int i;
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

double getPercentageDifference(double prevTime, double currentTime)
{
  return (prevTime - currentTime) / prevTime * 100;
}

void calculateCacheSize()
{
  double times[20], sizes[20];
  double  sizeFactor, k, notedPercentDrop, timeforFiftyLoops;
  int   threshold, nextCacheLikely = 1;
  int i, j;
printf("Cache size\n");  
timeforFiftyLoops = loop(50, dataSize, 64);
 loopFactor = (int) 1000 / timeforFiftyLoops; //adjust so that each jump will take around 20 seconds
  
 sizeFactor = 3.0/4; k = 8.0/9;
 i = 0;
 numBytesToMove = dataSize;

  while(numBytesToMove >= minSize)
  {
      numOfLoop = (dataSize/ numBytesToMove) * loopFactor;
      timeTaken = loop(numOfLoop, (int)numBytesToMove, 64);
      times[i] = timeTaken;
      sizes[i] = numBytesToMove; i++;
      
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
  threshold = 20;
  cacheSize[0] = cacheSize[1] = cacheSize[2] = cacheSize[3] = 0;
  j = 0;
  for(i = 0; i < 20; i++)
  {
    difference = getPercentageDifference(prevTime, times[i]);//(prevTime - times[i]) / prevTime * 100;
     printSizeTime(sizes[i], times[i]); printf("\t%lf\n", difference);
     prevTime = times[i];
     
     if(nextCacheLikely)
       {
	 if(difference > threshold)
	   {
	     printf("Is it a cache %lf\n", sizes[i]);
	     cacheSize[j++] = sizes[i];
	     notedPercentDrop = difference;
	     nextCacheLikely = 0;
	   }
       }
     else
       {
	 if(notedPercentDrop - difference > 10)
	   {
	     nextCacheLikely = 1;
	     threshold = 10;
	   }
       }
  }

}


int calculateBlockSize()
{
  int i, j, currentBlock, sizes[8];
  double ratio = 0;
  printf("%s\n", "Block Size");
  
 currentBlock = 256;
 timeTaken = 0, prevTime = 0;   
 numOfLoop = 1000;
 i  = 0;

  while(currentBlock >= 4)
    {
      prevTime = timeTaken;
      timeTaken = loop(numOfLoop, dataSize, currentBlock);
      
      if(prevTime != 0)
	{
	  ratio = prevTime / timeTaken;
	  if(ratio > 1.8)
	    {
	      blockSize = sizes[i - 1];
	      return;	       
	    }
	}
      printSizeTime(currentBlock, timeTaken); printf("\t%lf\n", ratio);
      sizes[i++] = currentBlock;
      currentBlock /= 2;
      numOfLoop /= 2;
    }
}

void calculateSetAssociativity()
{
  int n, setAssoc;
  int numBlocks = cacheSize[0] / blockSize;
  int numLoop = 10000;

  printf("Num blocks: %d\n", numBlocks);

  numBytesToMove = dataSize;//cacheSize[0];

 double sizeFactor[2] = { 3.0 / 2, 4.0 / 3 };

 prevTime = timeTaken = 0;

for(setAssoc = 2; setAssoc <= 32; setAssoc *= 2)
  {
    n = numBlocks / setAssoc; // num of sets
    
    prevTime = timeTaken;
   timeTaken = loop(numLoop, numBytesToMove, blockSize * n);
   

   printSizeTime(setAssoc, timeTaken); 
   if(prevTime != 0)
     {
   printf("\t%lf\n", getPercentageDifference(prevTime, timeTaken));
     }
   else
     {
       printf("\n");
     }
   numLoop /= 2;
   loop(1, dataSize, 64);
  }
  
}

int main()
{ 
  struct timeval totalExecStart, totalExecEnd;
  int i, j;
  gettimeofday(&totalExecStart, NULL);

  //calculateCacheSize();
   cacheSize[0] = 32 * KB; cacheSize[1] = 256 * KB; cacheSize[2] = 3 * MB;
   blockSize = 64;
// blockSize =  calculateBlockSize();
   
   // printf("Block Size = %d\n", blockSize);
   // for(i = 3; i >= 0; i--)
   // printf("%d\n", cacheSize[i]);
   //calculateSetAssociativity();
gettimeofday(&totalExecEnd, NULL);
  
  printf("Total exec time: %lf\n", getTimeDifference(&totalExecStart, &totalExecEnd));
}
