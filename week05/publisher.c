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
    int i, n;
    char pipe_path[100];

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <num_subscribers>\n", argv[0]);
        exit(1);
    }

    n = atoi(argv[1]);

    for (i = 1; i <= n; i++)
    {
        sprintf(pipe_path, "%s%d", PIPE_NAME, i);

        if (mkfifo(pipe_path, 0666) == -1)
        {
            perror("mkfifo");
            exit(1);
        }
    }

    while (1)
    {
        char message[MAX_MESSAGE_SIZE];

        fgets(message, MAX_MESSAGE_SIZE, stdin);

        for (i = 1; i <= n; i++)
        {
            sprintf(pipe_path, "%s%d", PIPE_NAME, i);

            int fd = open(pipe_path, O_WRONLY);
            if (fd == -1)
            {
                perror("open");
                exit(1);
            }

            write(fd, message, strlen(message));
            close(fd);
        }
    }

    return 0;
}