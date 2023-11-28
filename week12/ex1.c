#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>

int main()
{

    int fd;
    fd = open("/dev/input/by-path/platform-i8042-serio-0-event-kbd", O_RDONLY);

    FILE *fptr;
    fptr = fopen("ex1.txt", "w");

    if (fd == -1)
    {
        perror("Unable to open keyboard device");
        exit(EXIT_FAILURE);
    }

    fprintf(fptr, "Shortcuts available: \n");
    fprintf(fptr, "P+E -> \"I passed the Exam!\" \n");
    fprintf(fptr, "C+A+P -> \"Get some cappuccino!\" \n");
    fprintf(fptr, "C+B -> \"Code hard!\" \n\n");

    printf("Shortcuts available: \n");
    printf("P+E -> \"I passed the Exam!\" \n");
    printf("C+A+P -> \"Get some cappuccino!\" \n");
    printf("C+B -> \"Code hard!\" \n\n");

    struct input_event ie;
    ssize_t n;

    bool shortcut = false;
    char keys[6] = {0};
    int idx = 0;

    while (1)
    {

        n = read(fd, &ie, sizeof(ie));

        if (n == sizeof(ie))
        {

            if (ie.type == EV_KEY && ie.value >= 0 && ie.value <= 2)
            {

                if (ie.value == 1)
                {

                    if (ie.code == KEY_E && !strcmp(keys, "P"))
                    {
                        printf("I passed the Exam!\n");
                        shortcut = true;
                    }
                    else if (ie.code == KEY_P && !strcmp(keys, "CAB"))
                    {
                        printf("Get some cappuccino!\n");
                        shortcut = true;
                    }
                    else if (ie.code == KEY_B && !strcmp(keys, "CB"))
                    {
                        printf("Code hard!\n");
                        shortcut = true;
                    }
                    else
                    {
                        keys[idx++] = ie.code;
                    }
                }
                else
                {
                    idx = 0;
                    memset(keys, 0, sizeof(keys));
                    shortcut = false;
                }

                if (!shortcut)
                {
                    printf("%s 0x%04x (%d)\n", (ie.value == 1) ? "PRESSED" : (ie.value == 2) ? "REPEATED"
                                                                                             : "RELEASED",
                           ie.code, ie.code);
                }
            }
        }
        else
        {
            perror("error reading");
            exit(EXIT_FAILURE);
        }
    }

    if (!shortcut)
    {
        printf("%s 0x%04x (%d)\n", (ie.value == 1) ? "PRESSED" : (ie.value == 2) ? "REPEATED"
                                                                                 : "RELEASED",
               ie.code, ie.code);

        fprintf(fptr, "%s 0x%04x (%d)\n", (ie.value == 1) ? "PRESSED" : (ie.value == 2) ? "REPEATED"
                                                                                        : "RELEASED",
                ie.code, ie.code);
    }

    fclose(fptr);
    close(fd);

    return 0;
}