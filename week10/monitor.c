#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>

// Global variable to keep the program running
volatile sig_atomic_t keep_running = 1;

// Signal handler for SIGTERM
void handle_sigterm(int signum)
{
    keep_running = 0;
}

// Function to print stat info
void print_stat_info(const char *path)
{
    struct stat file_stat;
    if (stat(path, &file_stat) < 0)
    {
        perror("stat");
        return;
    }

    // Print stat information
    printf("Stat info for %s:\n", path);
    printf("Inode: %ld\n", (long)file_stat.st_ino);
    // Add other stat info as needed
}

// Main function
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Register signal handler
    signal(SIGTERM, handle_sigterm);

    // Initialize inotify
    int inotify_fd = inotify_init();
    if (inotify_fd < 0)
    {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    // Add watch to the specified path
    int watch_descriptor = inotify_add_watch(inotify_fd, argv[1], IN_ACCESS | IN_CREATE | IN_DELETE | IN_MODIFY | IN_OPEN | IN_ATTRIB);
    if (watch_descriptor < 0)
    {
        perror("inotify_add_watch");
        close(inotify_fd);
        exit(EXIT_FAILURE);
    }

    // Print initial stat info of all entries
    print_stat_info(argv[1]);

    // Event loop
    char buffer[1024];
    while (keep_running)
    {
        int length = read(inotify_fd, buffer, sizeof(buffer));
        if (length < 0)
        {
            perror("read");
            break;
        }

        int i = 0;
        while (i < length)
        {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];
            if (event->len)
            {
                if (event->mask & IN_MODIFY)
                {
                    printf("%s was modified.\n", event->name);
                }
                // Add other event types and corresponding actions
            }
            i += sizeof(struct inotify_event) + event->len;
        }
    }

    // Print stat info before termination
    print_stat_info(argv[1]);

    // Cleanup
    inotify_rm_watch(inotify_fd, watch_descriptor);
    close(inotify_fd);

    return 0;
}
