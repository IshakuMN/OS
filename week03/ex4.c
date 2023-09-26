#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <float.h>  // For DBL_MAX
#include <limits.h> // For INT_MIN

// Define a structure to hold the aggregation results for each data type
typedef struct {
    double double_result;
    int int_result;
} AggregationResult;

// Function to perform addition on two values
void* add(const void* a, const void* b) {
    AggregationResult* result = malloc(sizeof(AggregationResult));
    result->double_result = *(double*)a + *(double*)b;
    result->int_result = *(int*)a + *(int*)b;
    return result;
}

// Function to perform multiplication on two values
void* multiply(const void* a, const void* b) {
    AggregationResult* result = malloc(sizeof(AggregationResult));
    result->double_result = *(double*)a * *(double*)b;
    result->int_result = *(int*)a * *(int*)b;
    return result;
}

// Function to find the maximum of two values
void* max(const void* a, const void* b) {
    AggregationResult* result = malloc(sizeof(AggregationResult));
    result->double_result = (*(double*)a > *(double*)b) ? *(double*)a : *(double*)b;
    result->int_result = (*(int*)a > *(int*)b) ? *(int*)a : *(int*)b;
    return result;
}

// Function to perform aggregation on an array
void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*)) {
    AggregationResult* result = malloc(sizeof(AggregationResult));
    result->double_result = *(double*)initial_value;
    result->int_result = *(int*)initial_value;

    char* current = (char*)base;
    for (int i = 0; i < n; i++) {
        result = opr(result, current);
        current += size;
    }

    return result;
}

int main() {
    // Initialize arrays
    double double_array[5] = {1.5, 2.5, 3.5, 4.5, 5.5};
    int int_array[5] = {1, 2, 3, 4, 5};

    // Test aggregation with addition
    AggregationResult* add_result_double = aggregate(double_array, sizeof(double), 5, &(double){0}, add);
    AggregationResult* add_result_int = aggregate(int_array, sizeof(int), 5, &(int){0}, add);

    printf("Addition Result (Double): %lf\n", add_result_double->double_result);
    printf("Addition Result (Int): %d\n", add_result_int->int_result);

    free(add_result_double);
    free(add_result_int);

    // Test aggregation with multiplication
    AggregationResult* multiply_result_double = aggregate(double_array, sizeof(double), 5, &(double){1}, multiply);
    AggregationResult* multiply_result_int = aggregate(int_array, sizeof(int), 5, &(int){1}, multiply);

    printf("Multiplication Result (Double): %lf\n", multiply_result_double->double_result);
    printf("Multiplication Result (Int): %d\n", multiply_result_int->int_result);

    free(multiply_result_double);
    free(multiply_result_int);

   // Test aggregation with max
    AggregationResult* max_result_double = aggregate(double_array, sizeof(double), 5, &(double){-DBL_MAX}, max);
    AggregationResult* max_result_int = aggregate(int_array, sizeof(int), 5, &(int){INT_MIN}, max);

    printf("Max Result (Double): %lf\n", max_result_double->double_result);
    printf("Max Result (Int): %d\n", max_result_int->int_result);

    free(max_result_double);
    free(max_result_int);

    return 0;
}
