#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to generate filename based on n and format flag
void generate_filename(int n, int format_flag, char *filename) {
    char n_str[7];
    snprintf(n_str, sizeof(n_str), "%06d", n);
    if (format_flag == 0) {
        snprintf(filename, 256, "array_%s_asc.out", n_str);
    } else {
        snprintf(filename, 256, "array_%s_bin.out", n_str);
    }
}

// Function to print the array to file in the specified format
void print_to_file(int n, double **A, int format_flag) {
    char filename[256];
    generate_filename(n, format_flag, filename);

    if (format_flag == 0) { // ASCII format
        FILE *file = fopen(filename, "w");
        if (!file) {
            perror("Failed to open file");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fprintf(file, "%.15f ", A[i][j]);
            }
            fprintf(file, "\n");
        }
        fclose(file);
    } else if (format_flag == 1) { // Binary format
        FILE *file = fopen(filename, "wb");
        if (!file) {
            perror("Failed to open file");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < n; i++) {
            fwrite(A[i], sizeof(double), n, file);
        }
        fclose(file);
    }
}

int main() {
    // Step 1: Read n from input file
    FILE *input_file = fopen("input.in", "r");
    if (!input_file) {
        perror("Failed to open input file");
        return EXIT_FAILURE;
    }

    int n;
    fscanf(input_file, "%d", &n);
    fclose(input_file);

    // Step 2: Dynamically allocate 2D array
    double **A = (double **)malloc(n * sizeof(double *));
    if (!A) {
        perror("Memory allocation failed");
        return EXIT_FAILURE;
    }
    for (int i = 0; i < n; i++) {
        A[i] = (double *)malloc(n * sizeof(double));
        if (!A[i]) {
            perror("Memory allocation failed");
            return EXIT_FAILURE;
        }
    }

    // Step 3: Fill the array A[i][j] = i + j
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = (double)(i + j);
        }
    }

    // Step 4: Print the array to file in both formats
    print_to_file(n, A, 0); // ASCII format
    print_to_file(n, A, 1); // Binary format

    // Step 5: Free allocated memory
    for (int i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);

    return 0;
}
