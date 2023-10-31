// mmu.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define PAGETABLE "/tmp/pagetable"

struct PTE
{
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

int main(int argc, char *argv[])
{
    int numPages = atoi(argv[1]);
    char *refString = argv[2];
    pid_t osPid = atoi(argv[3]);

    int fd = open(PAGETABLE, O_RDWR);
    ftruncate(fd, sizeof(struct PTE) * numPages);

    struct PTE *pageTable = mmap(NULL, sizeof(struct PTE) * numPages,
                                 PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // Initialize page table
    for (int i = 0; i < numPages; i++)
    {
        pageTable[i].valid = false;
        pageTable[i].frame = -1;
        pageTable[i].dirty = false;
        pageTable[i].referenced = 0;
    }

    char *ptr = refString;
    while (*ptr != '\0')
    {
        char type = *ptr;
        int pageNum = *(ptr + 1) - '0';

        if (!pageTable[pageNum].valid)
        {
            pageTable[pageNum].referenced = 1;
            kill(osPid, SIGUSR1);
            pause();
        }

        if (type == 'W')
        {
            pageTable[pageNum].dirty = true;
        }

        printf("Page table after %c%d:\n", type, pageNum);
        for (int i = 0; i < numPages; i++)
        {
            printf("Page %d: %d %d %d %d\n", i, pageTable[i].valid, pageTable[i].frame,
                   pageTable[i].dirty, pageTable[i].referenced);
        }

        ptr += 2;
    }

    kill(osPid, SIGUSR1); // Signal OS we're done

    munmap(pageTable, sizeof(struct PTE) * numPages);
    close(fd);

    return 0;
}
