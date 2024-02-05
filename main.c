#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define M 4
#define K 3
#define N 3
#define THREADS 10

int A[M][K] = {{1,4}, {2,5}, {3,6}};
int B[K][N] = {{8,7,6}, {5,4,3}};
int C[M][N];

struct v {
    int i;
    int j;
};

void *runner(void *param);

int main(int argc, char *argv[]) {
    int i, j, count = 0;

    printf("Matrix A:\n");
    for(i = 0; i < M; i++) {
        for(j = 0; j < K; j++) {
            printf("%d", A[i][j]);
        }

        printf("\n");
    }

    for(i = 0; i < M; i++) {
        for(j = 0; j < N; j++) {
            struct v *data = (struct v*)malloc(sizeof(struct v));
            data->i = i;
            data->j = j;

            pthread_t tid;
            pthread_attr_t attr;

            pthread_attr_init(&attr);

            pthread_create(&tid, &attr, runner, data);

            pthread_join(tid, NULL);
            count++;
        }
    }

    for(i = 0; i < M; i++) {
        for(j = 0; j < N; j++) {
            printf("%d ", C[i][j]);
        }

        printf("\n");
    }
}

void *runner(void *param) {
    struct v *data = param;
    int n, sum = 0;

    for(n = 0; n < K; n++) {
        sum += A[data->i][n] * B[n][data->j];
    }

    C[data->i][data->j] = sum;

    pthread_exit(0);
}