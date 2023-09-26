#include <stdio.h>

int tribonacci(const int *p, int n) {
    // Initialize variables to store the Tribonacci numbers
    int T0 = *p;         // The first cell pointed by p
    int T1 = *(p + 1);   // The second cell pointed by p
    int T2 = *(p + 2);   // The third cell pointed by p

    if (n == 1) {
        return T0;
    } else if (n == 2) {
        return T1;
    } else if (n == 3) {
        return T2;
    }

    // Calculate the Tribonacci numbers iteratively
    for (int i = 4; i <= n; i++) {
        int temp = T0 + T1 + T2;
        T0 = T1;
        T1 = T2;
        T2 = temp;
    }

    return T2; // T2 holds the Tribonacci number for n
}

int main() {
    const int x = 1;
    int cells[3]; // Create an array of 3 integers
    int const *p = cells; // Pointer to the first cell

    // Fill the first two cells with the value of x
    cells[0] = x;
    cells[1] = x;

    // Fill the third cell with the value of 2x
    cells[2] = 2 * x;

    // Print the memory addresses of the cells
    printf("Address of the first cell: %p\n", (void *)p);
    printf("Address of the second cell: %p\n", (void *)(p + 1));
    printf("Address of the third cell: %p\n", (void *)(p + 2));

    // Check if the cells are contiguous
    if ((char *)(p + 1) - (char *)p == (char *)(p + 2) - (char *)(p + 1)) {
        printf("The cells are contiguous.\n");
    } else {
        printf("The cells are not contiguous.\n");
    }

    // Calculate the Tribonacci number for n = 10
    int n = 6;
    int result = tribonacci(p, n);
    printf("Tribonacci number for n = %d is %d\n", n, result);
    printf("given that the tribonacci starts from 1 as stated in the question\n");
    return 0;
}
