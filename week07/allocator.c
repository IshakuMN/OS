#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_MEM 100
int mem[MAX_MEM];
#define CLOCKS_PER_SEC 1000
void firstFitAllocate(unsigned int adrs, int size)
{
    int start_index = -1, start_count = 0;
    int i;
    for (i = 0; i < MAX_MEM; i++)
    {
        if (mem[i] == 0)
        {
            if (start_count == 0)
                start_index = i;
            start_count++;
        }
        else
        {
            if (start_count >= size)
            {
                for (int j = start_index; j < start_index + size; j++)
                {
                    mem[j] = adrs;
                }
                return;
            }
            start_count = 0;
        }
    }
    if (start_count >= size)
    {
        for (int j = start_index; j < start_index + size; j++)
        {
            mem[j] = adrs;
        }
        // return;
    }
}
void bestFitAllocate(unsigned int adrs, int size)
{
    int best_size = MAX_MEM + 1;
    int best_index = -1;
    int current_size = 0;
    int current_start = -1;
    for (int i = 0; i < MAX_MEM; i++)
    {
        if (mem[i] == 0)
        {
            current_size++;
            if (current_start == -1)
            {
                current_start = i;
            }
        }
        else
        {
            if (current_size >= size)
            {
                if (current_size < best_size)
                {
                    best_size = current_size;
                    best_index = current_start;
                }
            }
            current_size = 0;
            current_start = -1;
        }
    }

    if (current_size >= size)
    {
        if (current_size < best_size)
        {
            best_size = current_size;
            best_index = current_start;
        }
    }

    if (best_index != -1)
    {
        for (int i = best_index; i < best_index + size; i++)
        {
            mem[i] = adrs;
        }
    }
}
void worstFitAllocate(unsigned int adrs, int size)
{
    int worst_size = 0;
    int worst_index = -1;
    int current_size = 0;
    int current_start = -1;

    for (int i = 0; i < MAX_MEM; i++)
    {
        if (mem[i] == 0)
        {
            current_size++;
            if (current_start == -1)
            {
                current_start = i;
            }
        }
        else
        {
            if (current_size >= size)
            {
                if (current_size > worst_size)
                {
                    worst_size = current_size;
                    worst_index = current_start;
                }
            }
            current_size = 0;
            current_start = -1;
        }
    }

    if (current_size >= size)
    {
        if (current_size > worst_size)
        {
            worst_size = current_size;
            worst_index = current_start;
        }
    }

    if (worst_index != -1)
    {
        for (int i = worst_index; i < worst_index + size; i++)
        {
            mem[i] = adrs;
        }
    }
}
void clear(unsigned int adrs)
{
    int i;
    for (i = 0; i < MAX_MEM; i++)
    {
        if (mem[i] == adrs)
        {
            mem[i] = 0;
        }
    }
}
int main()
{
    FILE *fp = fopen("./queries.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file");
        return 1;
    }

    char query[100];
    unsigned int adrs, size;

    // First Fit
    memset(mem, 0, sizeof(mem));
    int queriesFF = 0;
    double timeFF = 0;

    while (fscanf(fp, "%s", query) > 0)
    {
        if (strcmp(query, "end") == 0)
        {
            break;
        }
        if (strcmp(query, "allocate") == 0)
        {
            fscanf(fp, "%u %d", &adrs, &size);
            double start = (double)clock() / CLOCKS_PER_SEC;
            firstFitAllocate(adrs, size);
            double end = (double)clock() / CLOCKS_PER_SEC;
            timeFF += (end - start);
            queriesFF++;
        }
        else if (strcmp(query, "clear") == 0)
        {
            fscanf(fp, "%u", &adrs);
            clear(adrs);
        }
    }

    double throughputFF = queriesFF / timeFF;

    // Best Fit
    memset(mem, 0, sizeof(mem));
    int queriesBF = 0;
    double timeBF = 0;

    fseek(fp, 0L, SEEK_SET);
    while (fscanf(fp, "%s", query) > 0)
    {
        if (strcmp(query, "end") == 0)
        {
            break;
        }
        if (strcmp(query, "allocate") == 0)
        {
            fscanf(fp, "%u %d", &adrs, &size);
            double start = (double)clock() / CLOCKS_PER_SEC;
            bestFitAllocate(adrs, size);
            double end = (double)clock() / CLOCKS_PER_SEC;
            timeBF += (end - start);
            queriesBF++;
        }
        else if (strcmp(query, "clear") == 0)
        {
            fscanf(fp, "%u", &adrs);
            clear(adrs);
        }
    }

    double throughputBF = queriesBF / timeBF;

    for (int i = 0; i < MAX_MEM; i++)
    {
        printf("%d ", mem[i]);
    }

    // Worst Fit
    memset(mem, 0, sizeof(mem));
    int queriesWF = 0;
    double timeWF = 0;

    fseek(fp, 0L, SEEK_SET);
    while (fscanf(fp, "%s", query) > 0)
    {
        if (strcmp(query, "end") == 0)
        {
            break;
        }
        if (strcmp(query, "allocate") == 0)
        {
            fscanf(fp, "%u %d", &adrs, &size);
            double start = (double)clock() / CLOCKS_PER_SEC;
            worstFitAllocate(adrs, size);
            double end = (double)clock() / CLOCKS_PER_SEC;
            timeWF += (end - start);
            queriesWF++;
        }
        else if (strcmp(query, "clear") == 0)
        {
            fscanf(fp, "%u", &adrs);
            clear(adrs);
        }
    }

    double throughputWF = queriesWF / timeWF;

    printf("\nThroughput for First Fit: %lf\n", throughputFF);
    printf("Throughput for Best Fit: %lf\n", throughputBF);
    printf("Throughput for Worst Fit: %lf\n", throughputWF);

    fclose(fp);

    return 0;
}