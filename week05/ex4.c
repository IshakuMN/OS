#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define MAX_THREADS 100

// Primality test
bool is_prime(int n)
{
    if (n <= 1)
        return false;
    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}

// Global variables
int n, m, k = 0, c = 0;
pthread_mutex_t mutex_k, mutex_c;

// Thread start routine
void *prime_checker(void *arg)
{
    while (1)
    {
        int num_to_check;

        // Acquire mutex for k
        pthread_mutex_lock(&mutex_k);
        num_to_check = k;
        k++; // Increment k
        pthread_mutex_unlock(&mutex_k);

        // Check if the thread is done
        if (num_to_check >= n)
            break;

        // Check for primality
        if (is_prime(num_to_check))
        {
            // Acquire mutex for c
            pthread_mutex_lock(&mutex_c);
            c++; // Increment c
            pthread_mutex_unlock(&mutex_c);
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <n> <m>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    m = atoi(argv[2]);

    if (m > MAX_THREADS)
    {
        fprintf(stderr, "Error: Maximum number of threads exceeded.\n");
        return 1;
    }

    pthread_t threads[MAX_THREADS];
    pthread_mutex_init(&mutex_k, NULL);
    pthread_mutex_init(&mutex_c, NULL);

    // Create threads
    for (int i = 0; i < m; i++)
    {
        pthread_create(&threads[i], NULL, prime_checker, NULL);
    }

    // Join threads
    for (int i = 0; i < m; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex_k);
    pthread_mutex_destroy(&mutex_c);

    printf("Total primes in [0, %d) = %d\n", n, c);

    return 0;
}
