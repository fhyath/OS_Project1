#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void calculate(int list[], int totalNumberOfProcess, FILE *file, int processorID, int pid[], int remBurstTime[], int burstTimeList[]);

void main()
{
  FILE *file;
  float avg_WaitTime, avg_TurnaroundTime,
      avg_cycles, avg_MemoryFootprint,
      totalWaiting = 0, totalTurnAround = 0,
      totalCycle = 0, totalMemoryFootprint = 0;

  printf("This program is utilizing RR algorithm:\n\n");

  int totalNumberOfProcess = 250;

  // System has 250 processes
  int processorID[totalNumberOfProcess];
  int burstTimeList[totalNumberOfProcess];
  int waitingTime[totalNumberOfProcess];
  int turnAroundTime[totalNumberOfProcess];
  int memFoot[totalNumberOfProcess];
  int remBurstTime[totalNumberOfProcess];

  int burstTimeMax = (int)(pow(10, 6)); // x 10 ^ 6 cycles
  int burstTimeMin = 10;                // x 10 ^ 6 cycles
  int memoryMax = 16000;                // in MB
  int memoryMin = 1;                    // in MB

  int processor1[totalNumberOfProcess / 6];
  int processor2[totalNumberOfProcess / 6];
  int processor3[totalNumberOfProcess / 6];
  int processor4[totalNumberOfProcess / 6];
  int processor5[totalNumberOfProcess / 6];
  int processor6[totalNumberOfProcess / 6];

  int readyList[totalNumberOfProcess];
  // Just initializing the processor array.
  for (int i = 0; i < (totalNumberOfProcess / 6); i++)
  {
    processor1[i] = -1;
    processor2[i] = -1;
    processor3[i] = -1;
    processor4[i] = -1;
    processor5[i] = -1;
    processor6[i] = -1;
  }

  // Generates 250 processes
  for (int i = 0; i < totalNumberOfProcess; i++)
  {
    // the following generating random numbers in a range was found on:
    // "https://www.geeksforgeeks.org/generating-random-number-range-c/"
    int memory = (rand() % (memoryMax - memoryMin + 1)) + memoryMin;
    int burstTime = (rand() % (burstTimeMax - burstTimeMin + 1)) + burstTimeMin;
    remBurstTime[i] = burstTime;
    // index = pid, value = burstTime.
    readyList[i] = burstTime;
  }
  printf("readyList[]", readyList[0]);

  int pid1[totalNumberOfProcess / 6];
  int pid2[totalNumberOfProcess / 6];
  int pid3[totalNumberOfProcess / 6];
  int pid4[totalNumberOfProcess / 6];
  int pid5[totalNumberOfProcess / 6];
  int pid6[totalNumberOfProcess / 6];

  int i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0, i6 = 0;

  int readyIndex = 0;
  int index = 0;
  while (readyIndex < totalNumberOfProcess)
  {
    if (index >= totalNumberOfProcess / 6)
    {
      index = 0;
    }

    if (processor1[index] == -1)
    {

      processor1[index] = readyList[readyIndex];
      pid1[i1] = readyIndex;
      i1++;

      printf("\nprocessor1[index] = %d", processor1[index]);
    }
    else if (processor2[index] == -1)
    {

      processor2[index] = readyList[readyIndex];
      pid2[i2] = readyIndex;
      i2++;
      printf("\nprocessor2[index] = %d", processor2[index]);
    }
    else if (processor3[index] == -1)
    {
      processor3[index] = readyList[readyIndex];
      pid3[i3] = readyIndex;
      i3++;
      printf("\nprocessor3[index] = %d", processor3[index]);
    }
    else if (processor4[index] == -1)
    {
      processor4[index] = readyList[readyIndex];
      pid4[i4] = readyIndex;
      i4++;
      printf("\nprocessor4[index] = %d", processor4[index]);
    }
    else if (processor5[index] == -1)
    {
      processor5[index] = readyList[readyIndex];
      pid5[i5] = readyIndex;
      i5++;
      printf("\nprocessor5[index] = %d", processor5[index]);
    }
    else
    {
      processor6[index] = readyList[readyIndex];
      pid6[i6] = readyIndex;
      i6++;
      printf("\nprocessor6[index] = %d", processor6[index]);
    }
    index++;
    readyIndex++;
  }
  file = fopen("Q1C.txt", "w");
  if (file == NULL)
  {
    printf("Unable to create file.\n");
    exit(EXIT_FAILURE);
  }

  // At this point, every processor should have a list of processes
  // waiting to be run

  for (int j = 1; j <= 6; j++)
  {
    if (j == 1)
    {
      // void calculate(processor1, 'processor1', totalNumberOfProcess, file);
      printf("\n\nstart porcessor1 and value of size = %d", (totalNumberOfProcess / 6));

      calculate(processor1, totalNumberOfProcess, file, j, pid1, remBurstTime, processor1);
      printf("\n\n finished processor1");
    }
    else if (j == 2)
    {

      calculate(processor2, totalNumberOfProcess, file, j, pid2, remBurstTime, processor2);
    }
    else if (j == 3)
    {
      calculate(processor3, totalNumberOfProcess, file, j, pid3, remBurstTime, processor3);
    }
    else if (j == 4)
    {
      calculate(processor4, totalNumberOfProcess, file, j, pid4, remBurstTime, processor4);
    }
    else if (j == 5)
    {
      calculate(processor5, totalNumberOfProcess, file, j, pid5, remBurstTime, processor5);
    }
    else
    {
      calculate(processor6, totalNumberOfProcess, file, j, pid6, remBurstTime, processor6);
    }
  }

  fclose(file);
}

// PROBLEM is that the value of the remBurstTime or the

void calculate(int list[], int totalNumberOfProcess, FILE *file, int processorID, int pid[], int remBurstTime[], int burstTimeList[])
{
  int length = totalNumberOfProcess / 6;
  int quantum = 20000;
  int timeStamp = 0, count = 0, tr = 0, wt = 0;
  int remain = length;
  // for(int i = 0; i < size; i++)
  int terminationFlag = 0;
  for (timeStamp = 0, count = 0; remain != 0;)
  {
    //printf("\ncount = %d,      list[count] = %d", count, list[count]);
    if (list[count] <= quantum && list[count] > 0)
    {
      // remBurstTime is just the list of burst Time but we use remBurstTime to keep track of
      // which burstime we have looked at.
      timeStamp += list[count];
      list[count] = -1;

      terminationFlag = 1;
    }
    else if (list[count] > 0 && list[count] > quantum) // it's greater than the quantum time, so lets say quantum time is 50, and our burstime = 100,
    {

      list[count] -= quantum; // difference between quantum and burstTime, so 100 - 50

      timeStamp += quantum;
    }
    /*
    else if (list[count] == 0 && terminationFlag == 0){
      remain--;
    }
    */
    if (list[count] == -1 && terminationFlag == 1)
    {
      remain--;
      // TurnAround Time = TimeStamp of time the processor terminated x number
      // of quantum time units added to the remaining burstime of the processor.
      tr += timeStamp;
      // Waiting time = TR - Processor's burst time.
      wt += tr - list[count];
      // printf("\np%d: \t|\t%d\t|\t%d",count,timeStamp,timeStamp-burstTimeList[count]);

      terminationFlag = 0;
    }
    if (count == totalNumberOfProcess - 1)
    { // if the counter reaches to the last process, we reset.
      count = 0;
      timeStamp = 0;
    }
    else
    {
      count++;
    }
  }

  float wtAVG = ((float)wt) / length;
  float trAVG = ((float)tr) / length;

  fprintf(file, "\nWait Time AVG = %f", wtAVG);
  fprintf(file, "\nTurnaround Time AVG = %f", trAVG);

  printf("\nWait Time AVG = %f", wtAVG);
  printf("\nTurnaround Time AVG = %f", trAVG);
}
