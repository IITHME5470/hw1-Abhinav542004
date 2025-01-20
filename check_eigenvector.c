#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_FILENAME_LENGTH 50
#define EPSILON 1e-6

void readMatrix(const char *filename, int n, double matrix[n][n]);
void readVector(const char *filename, int n, double vector[n]);
int isEigenvector(int n, double matrix[n][n], double vector[n], double *eigenvalue);
void appendEigenvalue(const char *filename, double eigenvalue);

int main() {
    // Read n from input file
    FILE *inputFile = fopen("input.in", "r");
    if (!inputFile) {
        perror("Error opening input.in");
        return 1;
    }
    int n;
    fscanf(inputFile, "%d", &n);
    fclose(inputFile);

    if (n <= 0 || n > 1000) { // Sanity check for n
        printf("Error: Invalid value of n (%d). Must be between 1 and 1000.\n", n);
        return 1;
    }

    // Dynamically allocate memory for the matrix and vectors
    double matrix[n][n];

    // Read the matrix from the file
    char matrixFilename[MAX_FILENAME_LENGTH];
    snprintf(matrixFilename, sizeof(matrixFilename), "mat%06d.in", n);
    readMatrix(matrixFilename, n, matrix);

    // Process vector files
    for (int vecnum = 1;; ++vecnum) {
        char vectorFilename[MAX_FILENAME_LENGTH];
        snprintf(vectorFilename, sizeof(vectorFilename), "vec%06d_%d.in", n, vecnum);

        FILE *vectorFile = fopen(vectorFilename, "r");
        if (!vectorFile) {
            break; // No more vector files
        }
        fclose(vectorFile);

        // Dynamically allocate memory for the vector
        double vector[n];
        readVector(vectorFilename, n, vector);

        // Check if the vector is an eigenvector and handle the output
        double eigenvalue;
        if (isEigenvector(n, matrix, vector, &eigenvalue)) {
            printf("%s : Yes : %lf\n", vectorFilename, eigenvalue);
            appendEigenvalue(vectorFilename, eigenvalue);
        } else {
            printf("%s : Not an eigenvector\n", vectorFilename);
        }
    }

    return 0;
}

void readMatrix(const char *filename, int n, double matrix[n][n]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening matrix file");
        exit(1);
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            fscanf(file, "%lf", &matrix[i][j]);
        }
    }
    fclose(file);
}

void readVector(const char *filename, int n, double vector[n]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening vector file");
        exit(1);
    }
    for (int i = 0; i < n; ++i) {
        fscanf(file, "%lf", &vector[i]);
    }
    fclose(file);
}

int isEigenvector(int n, double matrix[n][n], double vector[n], double *eigenvalue) {
    double result[n];
    double sum_ratios = 0;
    int count_valid = 0;

    // Compute matrix-vector multiplication
    for (int i = 0; i < n; ++i) {
        result[i] = 0;
        for (int j = 0; j < n; ++j) {
            result[i] += matrix[i][j] * vector[j];
        }
    }

    // Calculate average ratio for non-zero components
    for (int i = 0; i < n; ++i) {
        if (fabs(vector[i]) > EPSILON) {
            sum_ratios += result[i] / vector[i];
            count_valid++;
        }
    }

    if (count_valid == 0) {
        return 0; // Zero vector is not an eigenvector
    }

    *eigenvalue = sum_ratios / count_valid;

    // Verify if Av = ?v within tolerance
    for (int i = 0; i < n; ++i) {
        if (fabs(vector[i]) > EPSILON) {
            if (fabs(result[i] - (*eigenvalue) * vector[i]) > EPSILON) {
                return 0; // Not an eigenvector
            }
        }
    }

    return 1;
}

void appendEigenvalue(const char *filename, double eigenvalue) {
    FILE *file = fopen(filename, "a");
    if (!file) {
        perror("Error opening vector file for appending");
        exit(1);
    }
    fprintf(file, "\n%lf", eigenvalue);
    fclose(file);
}
