#include <stdio.h>
#include <stdlib.h>
#include "mmu.c"

struct PTE *pageTable;

int main(int argc, char *argv[])
{

    if (argc < 4)
    {
        printf("Usage: pager numPages numFrames algorithm\n");
        exit(1);
    }

    int numPages = atoi(argv[1]);
    int numFrames = atoi(argv[2]);
    char *algorithm = argv[3];

    printf("Using %s page replacement algorithm\n", algorithm);

    int evictedPage;

    if (strcmp(algorithm, "random") == 0)
    {
        evictedPage = random_page(pageTable, 3);
    }
    else if (strcmp(algorithm, "nfu") == 0)
    {
        evictedPage = nfu(pageTable, 3);
    }
    else if (strcmp(algorithm, "aging") == 0)
    {
        evictedPage = aging(pageTable, 3);
    }
    else
    {
        printf("Invalid algorithm\n");
        exit(1);
    }

    // Rest of pager code

    return 0;
}