#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void calculate(int list[], int totalNumberOfProcess, FILE *file, int processorID, int pid[]);

void main()
{

    int k;
    float avg_WaitTime, avg_TurnaroundTime, avg_cycles, totalWaiting = 0, totalTurnAround = 0, totalCycle = 0;
    printf("This program is utilizing FIFO algorithm:\n\n");

    int totalNumberOfProcess = 250;
    // int totalNumberOfProcess = 36;

    // System has 250 processes
    int processorID[totalNumberOfProcess];
    int burstTimeList[totalNumberOfProcess];

    int readyList[totalNumberOfProcess];

    int processor1[totalNumberOfProcess / 6];
    int processor2[totalNumberOfProcess / 6];
    int processor3[totalNumberOfProcess / 6];
    int processor4[totalNumberOfProcess / 6];
    int processor5[totalNumberOfProcess / 6];
    int processor6[totalNumberOfProcess / 6];

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

    // Generates 250 processes
    for (int i = 0; i < totalNumberOfProcess; i++)
    {
        // the following generating random numbers in a range was found on:
        // "https://www.geeksforgeeks.org/generating-random-number-range-c/"
        int memory = (rand() % (memoryMax - memoryMin + 1)) + memoryMin;
        int burstTime = (rand() % (burstTimeMax - burstTimeMin + 1)) + burstTimeMin;

        // index = pid, value = burstTime.
        readyList[i] = burstTime;
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
            pid1[i1] = readyIndex;
            i1++;
        }
        else if (processor2[index] == -1)
        {

            processor2[index] = readyList[readyIndex];
            pid2[i2] = readyIndex;
            i2++;
        }
        else if (processor3[index] == -1)
        {
            processor3[index] = readyList[readyIndex];
            pid3[i3] = readyIndex;
            i3++;
        }
        else if (processor4[index] == -1)
        {
            processor4[index] = readyList[readyIndex];
            pid4[i4] = readyIndex;
            i4++;
        }
        else if (processor5[index] == -1)
        {
            processor5[index] = readyList[readyIndex];
            pid5[i5] = readyIndex;
            i5++;
        }
        else
        {
            processor6[index] = readyList[readyIndex];
            pid6[i6] = readyIndex;
            i6++;
        }
        index++;
        readyIndex++;
    }
    file = fopen("Q1A.txt", "w");
    if (file == NULL)
    {
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }

    // At this point, every processor should have a list of processes
    // waiting to be run

    int overallAVG[6];

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

    // Calculates waiting time
    int waitTimeAvailable = 0;
    waitingTime[0] = 0;
    for (int c = 1; c < length; c++)
    {

        // we don't care about arrival times for this project.
        // W = Prev timeStamp
        for (int r = 0; r < c; r++)
        {

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
    printf("\nProcessor%d\n", processorID);
    fprintf(file, "\n\nProcessor%d\n", processorID);
    fprintf(file, "-----------------------------------------------");
    fprintf(file, "\nOverall System Waiting Time AVG = %f", avg_WaitTime);
    fprintf(file, "\nOverall System Turnaround Time AVG = %f", avg_TurnaroundTime);
    fprintf(file, "\n-----------------------------------------------");
    fprintf(file, "\n\n");
    printf("\nWaiting Time AVG = %f x10^6", avg_WaitTime);
    printf("\nTurnaround Time AVG = %f MB", avg_TurnaroundTime);
    printf("\n-----------------------------------------------");
    printf("\n\n");
}
