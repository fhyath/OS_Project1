#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void calculate(int list[], int totalNumberOfProcess, FILE *file, int processorID, int pid[]);

void main()
{

    int k;
    float avg_WaitTime, avg_TurnaroundTime, avg_cycles, avg_MemoryFootprint, totalWaiting = 0, totalTurnAround = 0, totalCycle = 0, totalMemoryFootprint = 0;
    printf("This program is utilizing SJF algorithm:\n\n");

    int totalNumberOfProcess = 250;

    // System has 250 processes
    int processorID[totalNumberOfProcess];
    int burstTimeList[totalNumberOfProcess];

    int waitingTime[totalNumberOfProcess];
    int turnAroundTime[totalNumberOfProcess];

    int processor1[totalNumberOfProcess / 6];
    int processor2[totalNumberOfProcess / 6];
    int processor3[totalNumberOfProcess / 6];
    int processor4[totalNumberOfProcess / 6];
    int processor5[totalNumberOfProcess / 6];
    int processor6[totalNumberOfProcess / 6];

    int readyList[totalNumberOfProcess];

    // Just initializing the processor array.
    for (int i = 0; i < totalNumberOfProcess / 6; i++)
    {
        processor1[i] = -1;
        processor2[i] = -1;
        processor3[i] = -1;
        processor4[i] = -1;
        processor5[i] = -1;
        processor6[i] = -1;
    }

    FILE *file;

    int burstTimeMax = (int)(pow(10, 6)); // x 10 ^ 6 cycles
    int burstTimeMin = 10;                // x 10 ^ 6 cycles
    int memoryMax = 16000;                // in MB
    int memoryMin = 1;                    // in MB

    // burstTimeLIst == readyLIst

    // Generates 250 processes
    for (int i = 0; i < totalNumberOfProcess; i++)
    {
        // the following generating random numbers in a range was found on:
        // "https://www.geeksforgeeks.org/generating-random-number-range-c/"
        int memory = (rand() % (memoryMax - memoryMin + 1)) + memoryMin;
        int burstTime = (rand() % (burstTimeMax - burstTimeMin + 1)) + burstTimeMin;
        processorID[i] = i;
        readyList[i] = burstTime;
    }

    // Orders the list, this greedy/brute force algorithm
    // is ordering it in Descending order from largest burst time to smallest.
    for (int j = 0; j < totalNumberOfProcess - 1; j++)
    {
        for (int l = j + 1; l < totalNumberOfProcess; l++)
        {
            if (readyList[j] < readyList[l])
            {
                // Swaps the burstTime value in the correct order based on how fast
                // the process terminates.
                int temp = readyList[j];
                readyList[j] = readyList[l];
                readyList[l] = temp;

                // Swaps and updates what ordering processor needs to be in.
                int tempProcessor = processorID[j];
                processorID[j] = processorID[l];
                processorID[l] = tempProcessor;
            }
        }
    }

    int pointer1 = 0;
    int pointer2 = totalNumberOfProcess - 1;
    // Re-Sorts the list into ascending order.
    while (pointer1 <= pointer2)
    {
        int temp = readyList[pointer1];
        readyList[pointer1] = readyList[pointer2];
        readyList[pointer2] = temp;

        // Swaps and updates what ordering processor needs to be in.
        int tempProcessor = processorID[pointer1];
        processorID[pointer1] = processorID[pointer2];
        processorID[pointer2] = tempProcessor;
        pointer1++;
        pointer2--;
    }

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
            pid1[i1] = processorID[readyIndex];
            i1++;
        }
        else if (processor2[index] == -1)
        {

            processor2[index] = readyList[readyIndex];
            pid2[i2] = processorID[readyIndex];
            i2++;
        }
        else if (processor3[index] == -1)
        {
            processor3[index] = readyList[readyIndex];
            pid3[i3] = processorID[readyIndex];
            i3++;
        }
        else if (processor4[index] == -1)
        {
            processor4[index] = readyList[readyIndex];
            pid4[i4] = processorID[readyIndex];
            i4++;
        }
        else if (processor5[index] == -1)
        {
            processor5[index] = readyList[readyIndex];
            pid5[i5] = processorID[readyIndex];
            i5++;
        }
        else
        {
            processor6[index] = readyList[readyIndex];
            pid6[i6] = processorID[readyIndex];
            i6++;
        }
        index++;
        readyIndex++;
    }

    file = fopen("Q1B.txt", "w");
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
            calculate(processor1, totalNumberOfProcess, file, j, pid1);
        }
        else if (j == 2)
        {
            calculate(processor2, totalNumberOfProcess, file, j, pid2);
        }
        else if (j == 3)
        {
            calculate(processor3, totalNumberOfProcess, file, j, pid3);
        }
        else if (j == 4)
        {
            calculate(processor4, totalNumberOfProcess, file, j, pid4);
        }
        else if (j == 5)
        {
            calculate(processor5, totalNumberOfProcess, file, j, pid5);
        }
        else
        {
            calculate(processor6, totalNumberOfProcess, file, j, pid6);
        }
    }

    fclose(file);
}

void calculate(int list[], int totalNumberOfProcess, FILE *file, int processorID, int pid[])
{
    int length = totalNumberOfProcess / 6;
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
            waitingTime[c] = waitingTime[c - 1] + list[r];
        }

        totalWaiting += waitingTime[c];
    }

    avg_WaitTime = ((float)totalWaiting) / length;

    // Calculates the TurnAroundTime
    for (int f = 0; f < length; f++)
    {

        // TR = W + Burst Time

        turnAroundTime[f] = list[f] + waitingTime[f];

        totalTurnAround += turnAroundTime[f];

        fprintf(file, "\n p:%d, TurnAround Time: %d, Waiting Time: %d, ", pid[f], turnAroundTime[f], waitingTime[f]);
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
}
