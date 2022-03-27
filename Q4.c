#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

typedef struct
{
    int pid;
    int burstime;
    int memory;
} process;

void calculate(process list[], int totalNumberOfProcess, FILE *file, int processorID, float *avgTR, float *avgWT);
void main()
{
    float avgTR = 0.0;
    float avgWT = 0.0;
    float avg_WaitTime, avg_TurnaroundTime, avg_cycles, avg_MemoryFootprint, totalWaiting = 0, totalTurnAround = 0, totalCycle = 0, totalMemoryFootprint = 0;
    printf("This program is utilizing SJF algorithm:\n\n");

    // File Variables
    FILE *file;
    FILE *fp;

    int totalNumberOfProcess = 250;
    int numOfProcessors = 6;

    int processorID[totalNumberOfProcess];
    int burstTimeList[totalNumberOfProcess];
    int waitingTime[totalNumberOfProcess];
    int turnAroundTime[totalNumberOfProcess];

    process pArray[totalNumberOfProcess];
    int count = 0;

    process processor1[totalNumberOfProcess / numOfProcessors];
    process processor2[totalNumberOfProcess / numOfProcessors];
    process processor3[totalNumberOfProcess / numOfProcessors];
    process processor4[totalNumberOfProcess / numOfProcessors];
    process processor5[totalNumberOfProcess / numOfProcessors];
    process processor6[totalNumberOfProcess / numOfProcessors];

    int readyList[totalNumberOfProcess];

    // Just initializing the processor array.
    for (int i = 0; i < totalNumberOfProcess / numOfProcessors; i++)
    {
        process temp = {-1, -1, -1};
        processor1[i] = temp;
        processor2[i] = temp;
        processor3[i] = temp;
        processor4[i] = temp;
        processor5[i] = temp;
        processor6[i] = temp;
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
    fclose(fp);

    int readyIndexTop = 0;
    int readyIndexBottom = totalNumberOfProcess - 1;
    int indexTop = 0;
    int indexBottom = 0;

    int smallMemory = 0;
    int largerMemory = 0;

    int p1Index = 0;
    int p2Index = 0;
    int p3Index = 0;
    int p4Index = 0;
    int p5Index = 0;
    int p6Index = 0;

    bool fastProcessorFull = false;
    bool slowProcessorFull = false;

    int numberOfBig = 0;
    int numberOfSmall = 0;


    int pointer1 = 0;
    int pointer2 = totalNumberOfProcess - 1;

    int index = 0;
    while (index < totalNumberOfProcess)
    {
     if( (pArray[index].memory > 8000 & fastProcessorFull == false) || (slowProcessorFull == true && fastProcessorFull == false) ){
         if(processor4[p4Index].pid == -1){
            processor4[p4Index] = pArray[index];
            p4Index++;
         }
         else if(processor5[p5Index].pid == -1){
            processor5[p5Index] = pArray[index];
            p5Index++;
         }
         else if(processor6[p6Index].pid == -1){
            processor6[p6Index] = pArray[index];
            p6Index++;
         }
         else {
                
                fastProcessorFull = true;
         
            }

    }
    if( (pArray[index].memory <= 8000  && slowProcessorFull == false) || (fastProcessorFull == true && slowProcessorFull == false) ) {
        if(processor1[p1Index].pid == -1){
            processor1[p1Index] = pArray[index];
            p1Index++;
        }
        else if(processor2[p2Index].pid == -1){
            processor2[p2Index] = pArray[index];
            p2Index++;
        }
        else if(processor3[p3Index].pid == -1){
            processor3[p3Index] = pArray[index];
            p3Index++;
        }
         else {
               
                slowProcessorFull = true;   
         }
       }
       index++;

    }

    file = fopen("Q4.txt", "w");
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
            calculate(processor1, totalNumberOfProcess, file, j, &avgTR, &avgWT);
        }
        else if (j == 2)
        {
            calculate(processor2, totalNumberOfProcess, file, j, &avgTR, &avgWT);
        }
        else if (j == 3)
        {
            calculate(processor3, totalNumberOfProcess, file, j, &avgTR, &avgWT);
        }
        else if (j == 4)
        {
            calculate(processor4, totalNumberOfProcess, file, j, &avgTR, &avgWT);
        }
        else if (j == 5)
        {
            calculate(processor5, totalNumberOfProcess, file, j, &avgTR, &avgWT);
        }
        else
        {
            calculate(processor6, totalNumberOfProcess, file, j, &avgTR, &avgWT);
        }
    }

    printf("Overall WT AVG: %f", avgWT / numOfProcessors);
    printf("\tOverall TR AVG: %f", avgTR / numOfProcessors);
    fprintf(file, "Overall WT AVG: %f", avgWT / numOfProcessors);
    fprintf(file, "\tOverall TR AVG: %f", avgTR / numOfProcessors);
    fclose(file);
}

void calculate(process list[], int totalNumberOfProcess, FILE *file, int processorID, float *avgTR, float *avgWT)
{
    int numOfProcessors = 6;
    int length = totalNumberOfProcess / numOfProcessors;
    int totalWaiting = 0;
    float avg_WaitTime, avg_TurnaroundTime;
    int totalTurnAround = 0;
    int waitingTime[length];
    int turnAroundTime[length];

    int waitTimeAvailable = 0;
    // Calculates waiting time
    waitingTime[0] = 0;
    for (int c = 1; c < length; c++)
    {
        for (int r = 0; r < c; r++)
        {

            // we don't care about arrival times for this project.
            // W = Prev timeStamp
            waitingTime[c] = waitingTime[c - 1] + list[r].burstime;
        }

        totalWaiting += waitingTime[c];
    }

    avg_WaitTime = ((float)totalWaiting) / length;

    // Calculates the TurnAroundTime
    for (int f = 0; f < length; f++)
    {
        // TR = W + Burst Time
        turnAroundTime[f] = list[f].burstime + waitingTime[f];
        totalTurnAround += turnAroundTime[f];
        fprintf(file, "\n p:%d, TurnAround Time: %d, Waiting Time: %d, ", list[f].pid, turnAroundTime[f], waitingTime[f]);
    }

    avg_TurnaroundTime = ((float)totalTurnAround) / length;

    fprintf(file, "\n\nProcessor%d\n", processorID);
    fprintf(file, "-----------------------------------------------");
    fprintf(file, "\nOverall System Waiting Time AVG = %f", avg_WaitTime);
    fprintf(file, "\nOverall System Turnaround Time AVG = %f", avg_TurnaroundTime);
    fprintf(file, "\n-----------------------------------------------");
    fprintf(file, "\n\n");

    printf("\nProcessor %d", processorID);
    printf("\nWaiting Time AVG = %f x10^6", avg_WaitTime);
    printf("\nTurnaround Time AVG = %f MB", avg_TurnaroundTime);
    printf("\n-----------------------------------------------");
    printf("\n\n");

    *avgTR += avg_TurnaroundTime;
    *avgWT += avg_WaitTime;
}
