#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define VECTOR_SIZE 120

// Function to calculate the dot product of two vectors for a given range.
int calculateDotProduct(int start, int end, int *u, int *v)
{
    int result = 0;
    for (int i = start; i < end; i++)
    {
        result += u[i] * v[i];
    }
    return result;
}

int main()
{
    int n;
    printf("Enter the number of processes (n): ");
    scanf("%d", &n);

    // Check if n is a power of 2 and within the allowed range.
    int k = 0;
    int temp = n;
    while (temp > 1)
    {
        if (temp % 2 != 0 || n < 1 || n > 32)
        {
            printf("n must be a power of 2 and within the range [1, 32].\n");
            return 1;
        }
        temp /= 2;
        k++;
    }

    int u[VECTOR_SIZE], v[VECTOR_SIZE];
    srand(time(NULL));

    // Initialize vectors u and v with random values between 0 and 99.
    for (int i = 0; i < VECTOR_SIZE; i++)
    {
        u[i] = rand() % 100;
        v[i] = rand() % 100;
    }

    // Create a file for reading and writing computation results.
    FILE *file = fopen("temp.txt", "w+");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    pid_t child_pid;
    int results[n];

    for (int i = 0; i < n; i++)
    {
        int start = i * (VECTOR_SIZE / n);
        int end = (i + 1) * (VECTOR_SIZE / n);

        child_pid = fork();

        if (child_pid == 0)
        { // Child process
            int result = calculateDotProduct(start, end, u, v);
            fprintf(file, "%d\n", result);
            fclose(file);
            exit(0);
        }
        else if (child_pid < 0)
        { // Fork failed
            perror("Fork failed");
            return 1;
        }
    }

    // Parent process
    for (int i = 0; i < n; i++)
    {
        wait(NULL);
    }

    rewind(file);

    int finalResult = 0;
    int processResult;

    // Read and aggregate results from the file.
    while (fscanf(file, "%d", &processResult) != EOF)
    {
        finalResult += processResult;
    }

    printf("The dot product of the vectors is: %d\n", finalResult);

    fclose(file);

    return 0;
}
