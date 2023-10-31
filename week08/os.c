// os.c
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

int numPages;
int numFrames;
int freeFrames;
int diskAccesses = 0;

void handleSigusr1(int signum)
{
    int fd = open(PAGETABLE, O_RDWR);
    struct PTE *pageTable = mmap(NULL, sizeof(struct PTE) * numPages,
                                 PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    int victimPage = -1;
    for (int i = 0; i < numPages; i++)
    {
        if (pageTable[i].referenced > 0)
        {
            if (freeFrames > 0)
            {
                pageTable[i].frame = freeFrames - 1;
                pageTable[i].valid = true;
                freeFrames--;
            }
            else
            {
                victimPage = rand() % numPages;
                while (pageTable[victimPage].valid == false)
                {
                    victimPage = rand() % numPages;
                }

                if (pageTable[victimPage].dirty)
                {
                    diskAccesses++;
                    sleep(1); // Write page to disk
                }

                pageTable[victimPage].valid = false;
                pageTable[i].frame = pageTable[victimPage].frame;
                pageTable[i].valid = true;
            }

            pageTable[i].referenced = 0;
            pageTable[i].dirty = false;

            diskAccesses++;
            sleep(1); // Read page from disk

            printf("Page table after page fault:\n");
            for (int i = 0; i < numPages; i++)
            {
                printf("Page %d: %d %d %d %d\n", i, pageTable[i].valid,
                       pageTable[i].frame, pageTable[i].dirty,
                       pageTable[i].referenced);
            }

            kill(getpid(), SIGCONT); // Signal MMU page loaded
        }
    }

    // Check if MMU is done
    bool done = true;
    for (int i = 0; i < numPages; i++)
    {
        if (pageTable[i].referenced > 0)
        {
            done = false;
            break;
        }
    }

    if (done)
    {
        munmap(pageTable, sizeof(struct PTE) * numPages);
        close(fd);
        unlink(PAGETABLE);
        printf("Total disk accesses: %d\n", diskAccesses);
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    numPages = atoi(argv[1]);
    numFrames = atoi(argv[2]);
    freeFrames = numFrames;

    int fd = open(PAGETABLE, O_CREAT | O_RDWR, 0666);
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

    signal(SIGUSR1, handleSigusr1);

    while (1)
    {
        pause();
    }

    return 0;
}
