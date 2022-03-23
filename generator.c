#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
    int pid;
    int burstime;
    int memory;
} process;

void calculate(process list[], int totalNumberOfProcess, FILE *file, int processorID);

void main()
{

    FILE *fp;
    int burstTimeMax = (int)(pow(10, 6)); // x 10 ^ 6 cycles
    int burstTimeMin = 10;                // x 10 ^ 6 cycles
    int memoryMax = 16000;                // in MB
    int memoryMin = 1;
    int totalNumberOfProcess = 250;
    // Variables for process array
    process pArray[totalNumberOfProcess];
    int count = 0;

    fp = fopen("processes.csv", "w");
    if (fp == NULL)
    {
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }

    // while (fscanf(fp, " %d,%d,%d", &pArray[count].pid, &pArray[count].burstime, &pArray[count].memory) == 3)

    for (int i = 0; i < totalNumberOfProcess; i++)
    {        
        int memory = (rand() % (memoryMax - memoryMin + 1)) + memoryMin;
        int burstTime = (rand() % (burstTimeMax - burstTimeMin + 1)) + burstTimeMin;
        pArray[i].pid = i;
        pArray[i].burstime = burstTime;
        pArray[i].memory = memory;

        fprintf(fp, "%d,%d,%d\n", pArray[i].pid, pArray[i].burstime, pArray[i].memory);
    }

    fclose(fp);
}