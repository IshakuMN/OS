
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <unistd.h>

int main()
{
    srand(time(NULL));

    const int PASSWORD_LEN = 8;
    char *password = mmap(NULL, PASSWORD_LEN, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);

    strcpy(password, "pass:");
    for (int i = 5; i < PASSWORD_LEN; i++)
    {
        password[i] = 33 + rand() % 94; // Printable ASCII chars
    }

    while (1)
    {
        sleep(1);
    }

    munmap(password, PASSWORD_LEN);

    return 0;
}
