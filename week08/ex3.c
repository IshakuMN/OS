#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

#define MEMORY_ALLOCATION_SIZE (10 * 1024 * 1024) // 10 MB

int main()
{
    struct rusage usage;
    int seconds = 10;

    for (int i = 0; i < seconds; i++)
    {
        // Allocate 10 MB of memory
        char *ptr = (char *)malloc(MEMORY_ALLOCATION_SIZE);
        if (ptr == NULL)
        {
            perror("Memory allocation failed");
            return 1;
        }

        // Fill the allocated memory with zeros
        memset(ptr, 0, MEMORY_ALLOCATION_SIZE);

        // Print memory usage with getrusage
        if (getrusage(RUSAGE_SELF, &usage) == 0)
        {
            printf("Memory usage (in KB) - Resident set size (RSS): %ld, Shared memory size: %ld\n",
                   usage.ru_maxrss, usage.ru_ixrss);
        }
        else
        {
            perror("getrusage failed");
        }

        // Sleep for 1 second
        sleep(1);

        // Free the allocated memory to prevent memory leaks
        free(ptr);
    }

    return 0;
}
