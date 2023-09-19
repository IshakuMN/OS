#include <stdio.h>
#include <string.h>

int main() {
    char input[256];
    char ch;
    int i = 0;

    printf("Type a string character by character, and end with a dot (.) or Enter:\n");

    while (1) {
        ch = getchar();

        if (ch == '\n' || ch == '.') {
            break;
        }

        if (i < 255) {
            input[i] = ch;
            i++;
        }
    }

    input[i] = '\0';

    printf("\"");
    for (int j = i - 1; j >= 0; j--) {
        printf("%c", input[j]);
    }
    printf("\"\n");

    return 0;
}
