#include <stdio.h>
#include <limits.h>
#include <float.h>

int main() {
    // Declare variables
    int integerVariable;
    unsigned short int unsignedShortVariable;
    signed long int longVariable;
    float floatVariable;
    double doubleVariable;

    // Assign maximum values
    integerVariable = INT_MAX;
    unsignedShortVariable = USHRT_MAX;
    longVariable = LONG_MAX;
    floatVariable = FLT_MAX;
    doubleVariable = DBL_MAX;

    // Print sizes and values
    printf("Size of int: %lu bytes\n", sizeof(int));
    printf("Maximum int value: %d\n", integerVariable);
    
    printf("Size of unsigned short int: %lu bytes\n", sizeof(unsigned short int));
    printf("Maximum unsigned short int value: %u\n", unsignedShortVariable);
    
    printf("Size of signed long int: %lu bytes\n", sizeof(signed long int));
    printf("Maximum signed long int value: %ld\n", longVariable);
    
    printf("Size of float: %lu bytes\n", sizeof(float));
    printf("Maximum float value: %f\n", floatVariable);
    
    printf("Size of double: %lu bytes\n", sizeof(double));
    printf("Maximum double value: %lf\n", doubleVariable);
    
    return 0;
}
