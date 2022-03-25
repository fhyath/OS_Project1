#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct {
    int pid;
    int burstime;
    int memory;
}process;

void calculate(process list[], int totalNumberOfProcess, FILE *file, int processorID, float *avgTR, float *avgWT);

void main()
{    
    float avgTR = 0.0;
    float avgWT = 0.0;
    float avg_WaitTime, avg_TurnaroundTime, avg_cycles, totalWaiting = 0, totalTurnAround = 0, totalCycle = 0;
    int totalNumberOfProcess = 250;
    int numOfProcessors = 6;

    // File Variables
    FILE *file;
    FILE *fp;

    //Variables for process array
    process pArray[totalNumberOfProcess];
    int count = 0;
    printf("This program is utilizing FIFO algorithm:\n\n");

    // System has 250 processes
    int processorID[totalNumberOfProcess];

    int readyList[totalNumberOfProcess];

    process processor1[totalNumberOfProcess / numOfProcessors];
    process processor2[totalNumberOfProcess / numOfProcessors];
    process processor3[totalNumberOfProcess / numOfProcessors];
    process processor4[totalNumberOfProcess / numOfProcessors];
    process processor5[totalNumberOfProcess / numOfProcessors];
    process processor6[totalNumberOfProcess / numOfProcessors];


    // Just initializing the processor array with -1 PID
    for (int i = 0; i < totalNumberOfProcess / numOfProcessors; i++)
    {
        process temp = { -1, -1, -1};
        processor1[i] = temp;
        processor2[i] = temp;
        processor3[i] = temp;
        processor4[i] = temp;
        processor5[i] = temp;
        processor6[i] = temp;
    }

    // Open CSV with processes and store it into an array of structs

    fp = fopen("processes.csv", "r");
    if (fp == NULL) {
        fprintf(stderr, "Error reading file\n");
    }

    while (fscanf(fp, " %d,%d,%d", &pArray[count].pid, &pArray[count].burstime, &pArray[count].memory)  == 3) {
        count++;
    }

    for (int i = 0; i < count; i++) {
        printf("%d,%d,%d\n", pArray[i].pid, pArray[i].burstime, pArray[i].memory);
    }
   
    fclose(fp);
    //printf("%d",pArray[3].burstime);

   
    int readyIndex = 0;
    int index = 0;
    while (readyIndex < totalNumberOfProcess)
    {
        if (index >= totalNumberOfProcess / numOfProcessors)
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

    file = fopen("Q1A.txt", "w");
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

    printf("Overall WT AVG: %f", avgWT / (numOfProcessors));
    printf("\tOverall TR AVG: %f", avgTR / (numOfProcessors));
    fprintf(file, "Overall WT AVG: %f", avgWT / (numOfProcessors));
    fprintf(file, "\tOverall TR AVG: %f", avgTR / (numOfProcessors));
    fclose(file);
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

    // Calculates waiting time
    int waitTimeAvailable = 0;
    waitingTime[0] = 0;
    for (int c = 1; c < length; c++)
    {

        // we don't care about arrival times for this project.
        // W = Prev timeStamp
        for (int r = 0; r < c; r++)
        {

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

    *avgTR += avg_TurnaroundTime;
    *avgWT += avg_WaitTime;
}