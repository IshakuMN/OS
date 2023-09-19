#include <stdio.h>
#include <ctype.h>

int count(const char *str, char ch) {
    int count = 0;
    while (*str) {
        if (tolower(*str) == tolower(ch)) {
            count++;
        }
        str++;
    }
    return count;
}

void countAll(const char *str) {
    int count[256] = {0}; // Assuming ASCII characters, initialize count array with zeros

    // Count occurrences of each character
    while (*str) {
        count[tolower(*str)]++;
        str++;
    }

    // Print the results
    for (int i = 0; i < 256; i++) {
        if (count[i] > 0) {
            printf("%c:%d, ", (char)i, count[i]);
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_string>\n", argv[0]);
        return 1;
    }

    char *inputString = argv[1];

    // Call countAll to count and print occurrences of each character
    countAll(inputString);

    return 0;
}
