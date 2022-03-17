#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void main()
{

     int k;
    float avg_WaitTime, avg_TurnaroundTime, avg_cycles, avg_MemoryFootprint, totalWaiting = 0, totalTurnAround = 0, totalCycle = 0, totalMemoryFootprint = 0;
    printf("This program is utilizing FIFO algorithm:\n\n");
    // Enter in 6 processors

    printf("Please enter number of available processes: ");
    scanf("%d", &k);

    int totalNumberOfProcess = 250 + k;

    // System has 250 processes
    int processorID[totalNumberOfProcess];
    int burstTimeList[totalNumberOfProcess];
    int waitingTime[totalNumberOfProcess];
    int turnAroundTime[totalNumberOfProcess];
    int memFoot[totalNumberOfProcess];
    int readyList[totalNumberOfProcess];

    FILE *file;

    int burstTimeMax = (int)(pow(10, 6));
    int burstTimeMin = 10;
    int memoryMax = 16000; // in MB
    int memoryMin = 1;     // in MB

    // Generates 250 processes
    for (int i = 0; i < 250; i++)
    {
        // the following generating random numbers in a range was found on:
        // "https://www.geeksforgeeks.org/generating-random-number-range-c/"
        int memory = (rand() % (memoryMax - memoryMin + 1)) + memoryMin;
        int burstTime = (rand() % (burstTimeMax - burstTimeMin + 1)) + burstTimeMin;
        processorID[i] = i;
        burstTimeList[i] = burstTime;
        memFoot[i] = memory;
    }

    // Adds the new 6 available processors, for now memory and processing time are all the same.
    int oneTimeMemValue = (rand() % (memoryMax - memoryMin + 1)) + memoryMin;
    int oneTimeBurstValue = (rand() % (burstTimeMax - burstTimeMin + 1)) + burstTimeMin;
    for (int i = 250; i < totalNumberOfProcess; i++)
    {
        int memory = oneTimeMemValue;
        int burstTime = oneTimeBurstValue;
        //printf("\njust in case memory = %d, burstime = %d", memory, burstTime);
        processorID[i] = i;

        // burst time (processing time) for each process in the system.
        burstTimeList[i] = burstTime;
        memFoot[i] = memory;
    }

    // At this point, we should have a list of all the processors
    // from fastest to slowest burst times.

    // Calculates waiting time
    int waitTimeAvailable = 0;
    waitingTime[0] = 0;
    for (int c = 1; c < totalNumberOfProcess; c++)
    {
        for (int r = 0; r < c; r++)
        {

            // we don't care about arrival times for this project.
            // W = Prev timeStamp
            waitingTime[c] = waitingTime[c - 1] + burstTimeList[r];
            memFoot[c] += burstTimeList[r];
        }      
        if (250 <= processorID[c] && processorID[c] < totalNumberOfProcess)
        {
            waitTimeAvailable += waitingTime[c];
        }  
        totalWaiting += waitingTime[c];
        totalMemoryFootprint += memFoot[c];
    }
    float avg_waitTimeAvailable = ((float)waitTimeAvailable) / k;
    avg_WaitTime = ((float)totalWaiting) / totalNumberOfProcess;
    avg_MemoryFootprint = ((float)(totalMemoryFootprint)) / totalNumberOfProcess;

    file = fopen("Q1_part_A.txt", "w");
    if (file == NULL)
    {
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "FIFO scheduling algorithm for Q1 part A");

    // Calculates the TurnAroundTime
    int avgTurnAround = 0;
    for (int f = 0; f < totalNumberOfProcess; f++)
    {
        // TR = W + Burst Time
        turnAroundTime[f] = burstTimeList[f] + waitingTime[f];
        totalTurnAround += turnAroundTime[f];  
        //fprintf(file, "\n p:%d, TurnAround Time: %d, Waiting Time: %d, ", processorID[f], turnAroundTime[f], waitingTime[f]);

        if (250 <= processorID[f] && processorID[f] < totalNumberOfProcess)
        {

            // fprintf(file, "\n p:%d, TurnAround Time: %d, Waiting Time: %d, ", processorID[f], turnAroundTime[f], waitingTime[f]);
            //printf("\n p:%d, TurnAround Time: %d, Waiting Time: %d,", processorID[f], turnAroundTime[f], waitingTime[f]);
            avgTurnAround += turnAroundTime[f];
            printf("\n p:%d, TurnAround Time: %d, Waiting Time: %d,", processorID[f], turnAroundTime[f], waitingTime[f]);   
        }
          
    }
   
    float avg_avgTurnAroundAvailable = ((float)avgTurnAround) / k;
    avg_TurnaroundTime = ((float)totalTurnAround) / totalNumberOfProcess;
    fprintf(file, "\n\nOverall System Waiting Time AVG = %f", avg_WaitTime);
    fprintf(file, "\nOverall System Turnaround Time AVG = %f\n", avg_TurnaroundTime);

    printf("\n\nOverall System Waiting Time AVG = %f x10^6", avg_WaitTime);
    printf("\nOverall System Turnaround Time AVG = %f MB", avg_TurnaroundTime);

    printf("\n\navailable wait AVG= %f x10^6", avg_waitTimeAvailable);
    printf("\navailable Turnaround Time AVG = %f MB", avg_avgTurnAroundAvailable);

    fclose(file);
}
