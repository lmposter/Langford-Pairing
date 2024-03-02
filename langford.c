#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Function to print usage instructions
void printUsage(const char *programName) {
    // Print error message with program usage instructions
    fprintf(stderr, "Usage: %s [-h] -c n | num...\n", programName);
}

// Function to print the sequence
void printSequence(int size, const int *sequence) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", sequence[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

// Function to check if the sequence is a Langford pairing
bool is_langford_pairing(int size, const int *sequence) {
    // Langford pairings are not possible for odd-sized sequences
    if (size % 2 != 0) {
        return false;
    }

    int n = size / 2;
    for (int i = 1; i <= n; ++i) {
        int first = -1, second = -1;

        // Find positions of two occurrences of each number
        for (int j = 0; j < size; ++j) {
            if (sequence[j] == i) {
                if (first == -1) {
                    first = j;
                } else if (second == -1) {
                    second = j;
                    break;
                }
            }
        }

        // Check if the positions satisfy Langford's condition
        if (second - first != i + 1) {
            return false;
        }
    }
    return true;
}

// Function to reverse an array from 'start' to 'end'
void reverseArray(int *arr, int start, int end) {
    while (start < end) {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

// Function to create a Langford pairing sequence. This implementation follows
// the solution presented in the video at https://www.youtube.com/watch?v=Lju6aYms2EA.
void createLangfordPairing(int *sequence, int n) {
    int x = (n + 3) / 4;
    int a = 2 * x - 1, b = 4 * x - 2, c = 4 * x - 1, d = 4 * x;

    // Sub-sequences used in constructing the Langford pairing
    int p[x - 1], q[x - 1], r[x - 1], s[x - 1], R_p[x - 1], R_q[x - 1], R_r[x - 1], R_s[x - 1];

    int p_index = 0, q_index = 0, r_index = 0, s_index = 0;

    // Populate p, q, r, s with respective values
    for (int i = 1; i < a; i += 2) {
        p[p_index++] = i;
    }
    for (int i = 2; i < a; i += 2) {
        q[q_index++] = i;
    }
    for (int i = a + 2; i < b; i += 2) {
        r[r_index++] = i;
    }
    for (int i = a + 1; i < b; i += 2) {
        s[s_index++] = i;
    }

    // Copy values to reversed sequences
    for (int i = 0; i < x - 1; i++) {
        R_p[i] = p[i];
        R_q[i] = q[i];
        R_r[i] = r[i];
        R_s[i] = s[i];
    }

    // Reverse p, q, r, s
    reverseArray(R_p, 0, p_index - 1);
    reverseArray(R_q, 0, q_index - 1);
    reverseArray(R_r, 0, r_index - 1);
    reverseArray(R_s, 0, s_index - 1);

    // Construct the Langford sequence
    int index = 0;
    for (int i = 0; i < s_index; i++) sequence[index++] = R_s[i];
    for (int i = 0; i < p_index; i++) sequence[index++] = R_p[i];
    sequence[index++] = b;
    for (int i = 0; i < p_index; i++) sequence[index++] = p[i];
    sequence[index++] = c;
    for (int i = 0; i < s_index; i++) sequence[index++] = s[i];
    if (n % 4 == 0) sequence[index++] = d;
    if (n % 4 == 3) sequence[index++] = a;
    for (int i = 0; i < r_index; i++) sequence[index++] = R_r[i];
    for (int i = 0; i < q_index; i++) sequence[index++] = R_q[i];
    sequence[index++] = b;
    sequence[index++] = a;
    for (int i = 0; i < q_index; i++) sequence[index++] = q[i];
    sequence[index++] = c;
    for (int i = 0; i < r_index; i++) sequence[index++] = r[i];
    if (n % 4 == 0) {
        sequence[index++] = a;
        sequence[index++] = d;
    }
}

// Main function to handle command-line arguments
int main(int argc, char *argv[]) {
    // Check if the program is called without arguments or with '-h'
    if (argc == 1 || (strcmp(argv[1], "-h") == 0)) {
        printUsage(argv[0]);
        return 0;
    }

    // Check if '-c' option is used to create a Langford pairing
    if (strcmp(argv[1], "-c") == 0) {
        if (argc != 3) {
            fprintf(stderr, argc < 3 ? "%s: -c option requires an argument.\n" : "%s: -c option received too many arguments.\n", argv[0]);
            return 1;
        }

        char *endptr;
        long n = strtol(argv[2], &endptr, 10);
        if (*endptr != '\0') {
            fprintf(stderr, "Error: %s is not an integer.\n", argv[2]);
            return 1;
        }

        printf("Creating a Langford pairing with n=%ld\n", n);
        if (n >= 0 && (n % 4 == 0 || n % 4 == 3)) {
            int sequence[2 * n];
            createLangfordPairing(sequence, n);
            printSequence(2 * n, sequence);
        } else {
            printf("No results found.\n");
        }
    } else {
        // Verify if the provided sequence is a Langford pairing
        int size = argc - 1;
        int sequence[size];
        for (int i = 1; i < argc; i++) {
            char *endptr;
            sequence[i - 1] = (int)strtol(argv[i], &endptr, 10);
            if (*endptr != '\0') {
                fprintf(stderr, "Error: %s is not an integer.\n", argv[i]);
                return 1;
            }
        }
        printf("Your sequence: ");
        printSequence(size, sequence);

        if (is_langford_pairing(size, sequence)) {
            printf("It is a Langford pairing!\n");
        } else {
            printf("It is NOT a Langford pairing.\n");
        }
    }

    return 0;
}