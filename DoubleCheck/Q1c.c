#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
  int pid;
  int burstime;
  int memory;
} process;

void calculate(process list[], int totalNumberOfProcess, FILE *file, int processorID, process remBurstTime[], float *avgTR, float *avgWT);

void main()
{
  // File Variables
  FILE *file;
  FILE *fp;
  float avgTR = 0.0;
  float avgWT = 0.0;
  float avg_WaitTime, avg_TurnaroundTime,
      avg_cycles, avg_MemoryFootprint,
      totalWaiting = 0, totalTurnAround = 0,
      totalCycle = 0, totalMemoryFootprint = 0;

  int totalNumberOfProcess = 250;

  // Variables for process array
  process pArray[totalNumberOfProcess];
  int count = 0;
  printf("This program is utilizing RR algorithm:\n\n");

  process remBurstTime1[totalNumberOfProcess / 6];
  process remBurstTime2[totalNumberOfProcess / 6];
  process remBurstTime3[totalNumberOfProcess / 6];
  process remBurstTime4[totalNumberOfProcess / 6];
  process remBurstTime5[totalNumberOfProcess / 6];
  process remBurstTime6[totalNumberOfProcess / 6];

  process processor1[totalNumberOfProcess / 6];
  process processor2[totalNumberOfProcess / 6];
  process processor3[totalNumberOfProcess / 6];
  process processor4[totalNumberOfProcess / 6];
  process processor5[totalNumberOfProcess / 6];
  process processor6[totalNumberOfProcess / 6];

  // Just initializing the processor array with -1 PID
  for (int i = 0; i < totalNumberOfProcess / 6; i++)
  {
    process temp = {-1, -1, -1};
    processor1[i] = temp;
    processor2[i] = temp;
    processor3[i] = temp;
    processor4[i] = temp;
    processor5[i] = temp;
    processor6[i] = temp;
    remBurstTime1[i] = temp;
    remBurstTime2[i] = temp;
    remBurstTime3[i] = temp;
    remBurstTime4[i] = temp;
    remBurstTime5[i] = temp;
    remBurstTime6[i] = temp;
  }

  // Open CSV with processes and store it into an array of structs

  fp = fopen("processes.csv", "r");
  if (fp == NULL)
  {
    fprintf(stderr, "Error reading file\n");
  }

  while (fscanf(fp, " %d,%d,%d", &pArray[count].pid, &pArray[count].burstime, &pArray[count].memory) == 3)
  {
    count++;
  }
  /*
    for (int i = 0; i < count; i++)
    {
      printf("%d,%d,%d\n", pArray[i].pid, pArray[i].burstime, pArray[i].memory);
    }
  */
  fclose(fp);
  // printf("%d",pArray[3].burstime);

  int readyIndex = 0;
  int index = 0;
  while (readyIndex < totalNumberOfProcess)
  {
    if (index >= totalNumberOfProcess / 6)
    {
      index = 0;
    }

    if (processor1[index].pid == -1)
    {

      processor1[index] = pArray[readyIndex];
      remBurstTime1[index] = processor1[index];
    }
    else if (processor2[index].pid == -1)
    {

      processor2[index] = pArray[readyIndex];
      remBurstTime2[index] = processor2[index];
    }
    else if (processor3[index].pid == -1)
    {
      processor3[index] = pArray[readyIndex];
      remBurstTime3[index] = processor3[index];
    }
    else if (processor4[index].pid == -1)
    {
      processor4[index] = pArray[readyIndex];
      remBurstTime4[index] = processor4[index];
    }
    else if (processor5[index].pid == -1)
    {
      processor5[index] = pArray[readyIndex];
      remBurstTime5[index] = processor5[index];
    }
    else if (processor6[index].pid == -1)
    {
      processor6[index] = pArray[readyIndex];
      remBurstTime6[index] = processor6[index];
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

  for (int j = 1; j <= 6; j++)
  {
    if (j == 1)
    {
      // void calculate(processor1, 'processor1', totalNumberOfProcess, file);
      calculate(processor1, totalNumberOfProcess, file, j, remBurstTime1, &avgTR, &avgWT);
    }
    else if (j == 2)
    {

      calculate(processor2, totalNumberOfProcess, file, j, remBurstTime2, &avgTR, &avgWT);
    }
    else if (j == 3)
    {
      calculate(processor3, totalNumberOfProcess, file, j, remBurstTime3, &avgTR, &avgWT);
    }
    else if (j == 4)
    {
      calculate(processor4, totalNumberOfProcess, file, j, remBurstTime4, &avgTR, &avgWT);
    }
    else if (j == 5)
    {
      calculate(processor5, totalNumberOfProcess, file, j, remBurstTime5, &avgTR, &avgWT);
    }
    else
    {
      calculate(processor6, totalNumberOfProcess, file, j, remBurstTime6, &avgTR, &avgWT);
    }
  }

  printf("Overall WT AVG: %f", avgWT / (totalNumberOfProcess / 6));
    printf("\tOverall TR AVG: %f", avgTR / (totalNumberOfProcess / 6));
    fprintf(file, "Overall WT AVG: %f", avgWT / (totalNumberOfProcess / 6));
    fprintf(file, "\tOverall TR AVG: %f", avgTR / (totalNumberOfProcess / 6));
  fclose(file);
}

// PROBLEM is that the value of the remBurstTime or the

void calculate(process list[], int totalNumberOfProcess, FILE *file, int processorID, process remBurstTime[], float *avgTR, float *avgWT)
{
  int totalNumber = 0;
  int length = totalNumberOfProcess / 6;
  int quantum = 5000;

  int timeStamp, count, tr = 0, wt = 0;
  int remain = length;

  int terminationFlag = 0;

  /*
    for(int i = 0; i < length; i++){
      printf("\nPID = %d", list[i].pid);
      printf("\tWith index = %d", i);
      printf("\tBT = %d", list[i].burstime);
    }

    printf("\n----------------------------------------\n");
  */

  for (timeStamp = 0, count = 0; remain != 0;)
  {

    if (remBurstTime[count].burstime <= quantum && remBurstTime[count].burstime > 0 && remBurstTime[count].pid != -1)
    {
      // remBurstTime is just the list of burst Time but we use remBurstTime to keep track of
      // which burstime we have looked at.
      timeStamp += remBurstTime[count].burstime;

      remBurstTime[count].burstime = -1;

      terminationFlag = 1;
    }
    else if (remBurstTime[count].burstime > 0 && remBurstTime[count].pid != -1) // it's greater than the quantum time, so lets say quantum time is 50, and our burstime = 100,
    {

      remBurstTime[count].burstime -= quantum; // difference between quantum and burstTime, so 100 - 50

      timeStamp += quantum;
    }

    if (remBurstTime[count].burstime == -1 && terminationFlag == 1 && remBurstTime[count].pid != -1)
    {

      remain--;
      // TurnAround Time = TimeStamp of time the processor terminated x number
      // of quantum time units added to the remaining burstime of the processor.
      tr += timeStamp;

      // Waiting time = TR - Processor's burst time.

      wt += (timeStamp - list[count].burstime);
      printf("p%d: \t|\t%d\t|\t%d\n", list[count].pid, timeStamp, timeStamp - list[count].burstime);
      fprintf(file, "p%d: \t|\t%d\t|\t%d\n", list[count].pid, timeStamp, timeStamp - list[count].burstime);
      terminationFlag = 0;
    }

    if (count >= (totalNumberOfProcess / 6) - 1)
    { // if the counter reaches to the last process, we reset.
      count = 0;
      // timeStamp = 0;
    }
    else
    {
      count++;
    }
  }

  float wtAVG = ((float)wt) / length;

  float trAVG = ((float)tr) / length;
  fprintf(file, "\nProcessor %d", processorID);
  fprintf(file, "\nWait Time AVG = %f", wtAVG);
  fprintf(file, "\nTurnaround Time AVG = %f\n\n", trAVG);
  printf("\nProcessor %d", processorID);
  printf("\nWait Time AVG = %f", wtAVG);
  printf("\nTurnaround Time AVG = %f\n\n", trAVG);

  *avgTR += trAVG;
  *avgWT += wtAVG;
}
