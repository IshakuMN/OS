
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <ctype.h>

#define FILE_SIZE 500 * 1024 * 1024 // 500 MiB
#define CHUNK_SIZE 1024 * sysconf(_SC_PAGESIZE)

int main()
{

    int fd, fd_rand;
    char c;
    int count = 0;

    // Create empty file
    fd = open("text.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ftruncate(fd, FILE_SIZE);

    // Open /dev/random
    fd_rand = open("/dev/random", O_RDONLY);

    // Generate random characters
    while (count < FILE_SIZE)
    {
        read(fd_rand, &c, 1);
        if (isprint(c))
        {
            write(fd, &c, 1);
            count++;
            if (count % 1024 == 0)
            {
                write(fd, "\n", 1);
            }
        }
    }

    close(fd_rand);

    // Memory map the file
    long sz = sysconf(_SC_PAGESIZE);
    int capital_count = 0;
    off_t offset = 0;

    while (offset < FILE_SIZE)
    {
        char *map = mmap(NULL, CHUNK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
        if (map == MAP_FAILED)
        {
            perror("mmap");
            exit(1);
        }

        // Count capital letters
        for (int i = 0; i < CHUNK_SIZE; i++)
        {
            if (isupper(map[i]))
            {
                capital_count++;
            }
        }

        // Change capital to lowercase
        for (int i = 0; i < CHUNK_SIZE; i++)
        {
            if (isupper(map[i]))
            {
                map[i] = tolower(map[i]);
            }
        }

        munmap(map, CHUNK_SIZE);

        offset += CHUNK_SIZE;
    }

    printf("Total capital letters: %d\n", capital_count);

    close(fd);

    return 0;
}