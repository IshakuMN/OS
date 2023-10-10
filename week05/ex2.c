#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

// Define the Thread struct
struct Thread
{
    pthread_t id;
    int i;
    char message[256];
};

// Function to be executed by each thread
void *thread_function(void *arg)
{
    struct Thread *thread_data = (struct Thread *)arg;

    // Print the message
    printf("Thread %d prints message: %s\n", thread_data->i, thread_data->message);

    // Exit the thread
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int n;
    printf("Enter the number of threads: ");
    scanf("%d", &n);

    struct Thread threads[n];

    for (int i = 0; i < n; i++)
    {
        // Fill in thread data
        threads[i].i = i + 1;
        snprintf(threads[i].message, sizeof(threads[i].message), "Hello from thread %d", threads[i].i);

        // Create the thread
        printf("Thread %d is created\n", threads[i].i);
        pthread_create(&threads[i].id, NULL, thread_function, (void *)&threads[i]);

        // Wait for the thread to finish
        pthread_join(threads[i].id, NULL);
    }

    return 0;
}
