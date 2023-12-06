#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

pthread_mutex_t *mutexes;
int num_resources;
int num_threads;

typedef struct
{
    int thread_id;
    int resource_id;
} request;

request *requests;
int num_requests;

void *process_request(void *arg)
{
    int thread_id = ((request *)arg)->thread_id;
    int resource_id = ((request *)arg)->resource_id;

    // Print thread created
    printf("Thread %d created\n", thread_id);

    // Print mutexes held
    printf("Mutexes held by thread %d: ", thread_id);
    for (int i = 0; i < num_resources; i++)
    {
        if (pthread_mutex_trylock(&mutexes[i]) == 0)
        {
            printf("%d ", i);
            pthread_mutex_unlock(&mutexes[i]);
        }
    }
    printf("\n");

    // Try locking mutex
    printf("Thread %d tries to lock mutex %d\n", thread_id, resource_id);
    int rc = pthread_mutex_trylock(&mutexes[resource_id]);

    // Check for deadlock
    if (rc == EBUSY)
    {
        // Deadlock detection

        // Print deadlock message and status
        printf("Deadlock detected\n");
        printf("Thread statuses:\n");
        for (int i = 0; i < num_threads; i++)
        {
            printf("Thread %d: ", i);
            for (int j = 0; j < num_resources; j++)
            {
                if (pthread_mutex_trylock(&mutexes[j]) == 0)
                {
                    printf("%d ", j);
                    pthread_mutex_unlock(&mutexes[j]);
                }
            }
            printf("\n");
        }

        exit(1);
    }

    // Lock mutex
    pthread_mutex_lock(&mutexes[resource_id]);
    int r = rand() % 5;
    sleep(r);

    // Unlock mutex
    printf("Thread %d unlocked mutex %d\n", thread_id, resource_id);
    pthread_mutex_unlock(&mutexes[resource_id]);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    num_resources = atoi(argv[1]);
    num_threads = atoi(argv[2]);

    // Allocate mutexes
    mutexes = malloc(num_resources * sizeof(pthread_mutex_t));
    for (int i = 0; i < num_resources; i++)
    {
        pthread_mutex_init(&mutexes[i], NULL);
    }

    // Read input file
    FILE *fp = fopen("input.txt", "r");
    if (!fp)
    {
        printf("Error opening file\n");
        exit(1);
    }

    int t, r;
    num_requests = 0;
    while (fscanf(fp, "%d %d", &t, &r) == 2)
    {
        num_requests++;
    }

    requests = malloc(num_requests * sizeof(request));

    rewind(fp);
    int i = 0;
    while (fscanf(fp, "%d %d", &t, &r) == 2)
    {
        requests[i].thread_id = t;
        requests[i].resource_id = r;
        i++;
    }

    fclose(fp);

    // Process requests in parallel
    pthread_t threads[num_requests];
    for (i = 0; i < num_requests; i++)
    {
        pthread_create(&threads[i], NULL, process_request, (void *)&requests[i]);
    }

    // Join threads
    for (i = 0; i < num_requests; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Write output
    FILE *out = fopen("ex1.txt", "w");
    fprintf(out, "No deadlocks\n");
    fclose(out);

    // Clean up
    for (i = 0; i < num_resources; i++)
    {
        pthread_mutex_destroy(&mutexes[i]);
    }
    free(mutexes);
    free(requests);

    return 0;
}