#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_SIZE 10000
#define MAX_THREADS 64

// initialize matrices
int matA[MAX_SIZE][MAX_SIZE];
int matB[MAX_SIZE][MAX_SIZE];
int matC[MAX_SIZE][MAX_SIZE];

int r1, c1, r2, c2;

// structure to pass arguments to the thread
typedef struct {
    int row;
} thread_args;

// function to multiply a row of matrices A and B
void* multiply_row(void* arg) {
    thread_args* args = (thread_args*)arg;
    int row = args->row;
    
    for (int j = 0; j < c2; j++) {
        matC[row][j] = 0;
        for (int k = 0; k < c1; k++) {
            matC[row][j] += matA[row][k] * matB[k][j];
        }
    }

    pthread_exit(NULL);
}

// function to read matrix from file
bool read_matrix(FILE* fp, int mat[MAX_SIZE][MAX_SIZE], int* rows, int* cols) {
    if (fscanf(fp, "%d %d", rows, cols) != 2)
        return false;

    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            if (fscanf(fp, "%d", &mat[i][j]) != 1)
                return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_file> <num_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[2]);
    if (num_threads <= 0 || num_threads > MAX_THREADS) {
        printf("Invalid number of threads\n");
        return 1;
    }

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        printf("Error opening file %s\n", argv[1]);
        return 1;
    }

    // read matrix A
    if (!read_matrix(fp, matA, &r1, &c1)) {
        printf("Error reading matrix A\n");
        fclose(fp);
        return 1;
    }

    // read matrix B
    if (!read_matrix(fp, matB, &r2, &c2)) {
        printf("Error reading matrix B\n");
        fclose(fp);
        return 1;
    }

    fclose(fp);

    // check if dimensions are compatible for multiplication
    if (c1 != r2) {
        printf("Error: Matrix dimensions mismatch for multiplication\n");
        return 1;
    }

    // print Matrix A
    printf("Matrix A:\n");
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c1; j++) {
            printf("%d ", matA[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // print Matrix B
    printf("Matrix B:\n");
    for (int i = 0; i < r2; i++) {
        for (int j = 0; j < c2; j++) {
            printf("%d ", matB[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // initialize threads
    pthread_t threads[MAX_THREADS];
    thread_args args[MAX_THREADS];

    clock_t start_time = clock();

    // create threads to compute each row of result matrix
    for (int i = 0; i < r1; i++) {
        args[i].row = i;
        if (pthread_create(&threads[i], NULL, multiply_row, &args[i]) != 0) {
            printf("Error creating thread\n");
            return 1;
        }
        printf("Created worker thread %lu for row %d\n", threads[i], i);
    }

    // wait for all threads to finish
    for (int i = 0; i < r1; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t end_time = clock();
    double total_time = ((double)(end_time - start_time)) * 1000 / CLOCKS_PER_SEC;

    // print result matrix C
    printf("\nMatrix C = A x B:\n");
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            printf("%d ", matC[i][j]);
        }
        printf("\n");
    }

    printf("\nTotal execution time using %d threads is %.3f ms\n", num_threads, total_time);

    return 0;
}
