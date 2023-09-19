#include <stdio.h>
#include <stdlib.h>


void convert(long long number, int s, int t) {
    // Check if s and t are within the range [2, 10]
    if (s < 2 || s > 10 || t < 2 || t > 10) {
        printf("Error: cannot convert!\n");
        return;
    }

    // Convert the number to decimal first
    long long decimal_number = 0;
    long long power = 1;

    while (number > 0) {
        long long digit = number % 10;
        if (digit >= s) {
            printf("Error: cannot convert!\n");
            return;
        }
        decimal_number += digit * power;
        power *= s;
        number /= 10;
    }

    // Convert the decimal number to the target numeral system
    if (decimal_number == 0) {
        printf("0\n");
        return;
    }

    char result[32]; // Assuming a maximum of 32-bit result
    int result_size = 0;

    while (decimal_number > 0) {
        int remainder = decimal_number % t;
        result[result_size] = remainder + '0';
        result_size++;
        decimal_number /= t;
    }

    // Print the converted result
    for (int i = result_size - 1; i >= 0; i--) {
        printf("%c", result[i]);
    }
    printf("\n");
}


int main() {
    long long input;
    int s, t;

    // Read the long long number and source/target numeral systems from the user
    printf("Enter a non-negative long long number: ");
    if (scanf("%lld", &input) != 1) {
        printf("Invalid input for the number.\n");
        return 1;
    }

    printf("Enter the source numeral system (2-10): ");
    if (scanf("%d", &s) != 1) {
        printf("Invalid input for the source numeral system.\n");
        return 1;
    }

    printf("Enter the target numeral system (2-10): ");
    if (scanf("%d", &t) != 1) {
        printf("Invalid input for the target numeral system.\n");
        return 1;
    }

    // Check if the input number is non-negative
    if (input < 0) {
        printf("Error: The input number must be non-negative.\n");
        return 1;
    }

    // Call the convert function to perform the conversion
    convert(input, s, t);

    return 0;
}
