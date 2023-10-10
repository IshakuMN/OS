#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define PIPE_NAME "/tmp/ex1/s"
#define MAX_MESSAGE_SIZE 1024

int main(int argc, char *argv[])
{
    char pipe_path[100];

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <id>\n", argv[0]);
        exit(1);
    }

    sprintf(pipe_path, "%s%s", PIPE_NAME, argv[1]);

    printf("Opening pipe: %s\n", pipe_path);

    int fd = open(pipe_path, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }

    while (1)
    {
        char message[MAX_MESSAGE_SIZE];
        int n = read(fd, message, MAX_MESSAGE_SIZE);

        if (n == -1)
        {
            perror("read");
            exit(1);
        }

        if (n == 0)
        {
            printf("Pipe closed\n");
            break;
        }

        printf("Received message: %s", message);
    }

    close(fd);

    return 0;
}