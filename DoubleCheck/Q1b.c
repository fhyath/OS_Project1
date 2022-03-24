#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

    int processorID[totalNumberOfProcess];
    int burstTimeList[totalNumberOfProcess];
    int waitingTime[totalNumberOfProcess];
    int turnAroundTime[totalNumberOfProcess];

    process pArray[totalNumberOfProcess];
    int count = 0;

    process processor1[totalNumberOfProcess / 6];
    process processor2[totalNumberOfProcess / 6];
    process processor3[totalNumberOfProcess / 6];
    process processor4[totalNumberOfProcess / 6];
    process processor5[totalNumberOfProcess / 6];
    process processor6[totalNumberOfProcess / 6];

    int readyList[totalNumberOfProcess];

    // Just initializing the processor array.
    for (int i = 0; i < totalNumberOfProcess / 6; i++)
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

    for (int i = 0; i < count; i++)
    {
        printf("%d,%d,%d\n", pArray[i].pid, pArray[i].burstime, pArray[i].memory);
    }

    fclose(fp);
    printf("%d", pArray[3].burstime);
    // Orders the list, this greedy/brute force algorithm
    // is ordering it in Descending order from largest burst time to smallest.
    for (int j = 0; j < totalNumberOfProcess - 1; j++)
    {
        for (int l = j + 1; l < totalNumberOfProcess; l++)
        {
            if (pArray[j].burstime < pArray[l].burstime)
            {
                // Swaps the burstTime value in the correct order based on how fast
                // the process terminates.
                int temp = pArray[j].burstime;
                pArray[j].burstime = pArray[l].burstime;
                pArray[l].burstime = temp;

                // Swaps and updates what ordering processor needs to be in.
                int tempProcessor = pArray[j].pid;
                pArray[j].pid = pArray[l].pid;
                pArray[l].pid = tempProcessor;
            }
        }
    }

    int pointer1 = 0;
    int pointer2 = totalNumberOfProcess - 1;
    // Re-Sorts the list into ascending order.
    while (pointer1 <= pointer2)
    {
        int temp = pArray[pointer1].burstime;
        pArray[pointer1].burstime = pArray[pointer2].burstime;
        pArray[pointer2].burstime = temp;

        // Swaps and updates what ordering processor needs to be in.
        int tempProcessor = pArray[pointer1].pid;
        pArray[pointer1].pid = pArray[pointer2].pid;
        pArray[pointer2].pid = tempProcessor;
        pointer1++;
        pointer2--;
    }

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
        }
        else if (processor2[index].pid == -1)
        {
            processor2[index] = pArray[readyIndex];
        }
        else if (processor3[index].pid == -1)
        {
            processor3[index] = pArray[readyIndex];
        }
        else if (processor4[index].pid == -1)
        {
            processor4[index] = pArray[readyIndex];
        }
        else if (processor5[index].pid == -1)
        {
            processor5[index] = pArray[readyIndex];
        }
        else if (processor6[index].pid == -1)
        {
            processor6[index] = pArray[readyIndex];
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

    printf("Overall WT AVG: %f", avgWT / (totalNumberOfProcess / 6));
    printf("\tOverall TR AVG: %f", avgTR / (totalNumberOfProcess / 6));
    fprintf(file, "Overall WT AVG: %f", avgWT / (totalNumberOfProcess / 6));
    fprintf(file, "\tOverall TR AVG: %f", avgTR / (totalNumberOfProcess / 6));
    fclose(file);
}

void calculate(process list[], int totalNumberOfProcess, FILE *file, int processorID, float *avgTR, float *avgWT)
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