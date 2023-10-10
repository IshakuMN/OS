#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

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

// Primes counter in [a, b)
int primes_count(int a, int b)
{
    int ret = 0;
    for (int i = a; i < b; i++)
        if (is_prime(i))
            ret++;
    return ret;
}

// Argument to the start_routine of the thread
typedef struct prime_request
{
    int a, b;
} prime_request;

// Start_routine of the thread
void *prime_counter(void *arg)
{
    // Get the request from arg
    prime_request *req = (prime_request *)arg;

    // Perform the request
    int count = primes_count(req->a, req->b);

    // Allocate memory to store the result and return it
    int *result = (int *)malloc(sizeof(int));
    *result = count;
    return (void *)result;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <n> <m>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    pthread_t threads[m];
    prime_request requests[m];
    int results[m];

    int interval = n / m;
    int remainder = n % m;

    for (int i = 0; i < m; i++)
    {
        requests[i].a = i * interval;
        requests[i].b = (i + 1) * interval + (i == m - 1 ? remainder : 0);
        pthread_create(&threads[i], NULL, prime_counter, &requests[i]);
    }

    int total_primes = 0;

    for (int i = 0; i < m; i++)
    {
        void *result;
        pthread_join(threads[i], &result);
        int *count = (int *)result;
        results[i] = *count;
        total_primes += *count;
        free(count);
    }

    printf("Total primes in [0, %d) = %d\n", n, total_primes);

    return 0;
}
