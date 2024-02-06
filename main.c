#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#define MAX 4 // *** change to 10000 max size for matrix row and column ***

// each thread computes single element in the resultant matrix
void *mult(void* arg) {
	int *data = (int *)arg;
	int k = 0, i = 0;
	
	int x = data[0];
	for (i = 1; i <= x; i++) {
		k += data[i]*data[i+x];
    }
	
	int *p = (int*)malloc(sizeof(int));
	*p = k;
	
    // terminates a thread and return value is passed as pointer
	pthread_exit(p);
}

int main(int argc, char *argv[]) {
    // get input file name from user
    char filename[50];
    if(argc!=2) {
        printf("Enter the input file name: ");
        scanf("%s", filename);
    } else {
        strcpy(filename, argv[1]);
    }

    // read the file
    FILE *fp = fopen(filename, "r");

    // error if file cannot open
    if(fp==NULL) {
        printf("Error opening file %s\n", argv[1]);
        return 1;
    }
    
	int matA[MAX][MAX]; 
	int matB[MAX][MAX]; 
	
	int r1=MAX,c1=MAX,r2=MAX,c2=MAX,i,j,k;

	// read matrix A dimensions and elements
    fscanf(fp, "%d %d", &r1, &c1);
    for(i=0; i<r1; i++) {
        for(j=0; j<c1; j++) {
            fscanf(fp, "%d", &matA[i][j]);
        }
    }
	
	// displays matrix A	
    printf("Matrix A:\n");	 
	for(i = 0; i < r1; i++) {
		for(j = 0; j < c1; j++) {
			printf("%d ", matA[i][j]);
        }
		printf("\n");
	}

    // read matrix B dimensions and elements
    fscanf(fp, "%d %d", &r2, &c2);
    for(i=0; i<r2; i++) {
        for(j=0; j<c2; j++) {
            fscanf(fp, "%d", &matB[i][j]);
        }
    }
			
	// displays matrix B
    printf("Matrix B:\n");	 
	for(i = 0; i < r2; i++) {
		for(j = 0; j < c2; j++) {
			printf("%d ", matB[i][j]);
        }
		printf("\n"); 
	}

    fclose(fp);

    if(c1!=r2) {
        printf("Error: Matrix dimensions mismatch for multiplication.\n");
        return 1;
    }
	
	int max = r1*c2;
	
	// declares array of threads of size r1*c2	 
	pthread_t *threads;
	threads = (pthread_t*)malloc(max*sizeof(pthread_t));
	
	int count = 0;
	int* data = NULL;
	for (i = 0; i < r1; i++) {
		for (j = 0; j < c2; j++) {
			// stores row and column elements in data 
			data = (int *)malloc((20)*sizeof(int));
			data[0] = c1;

			for (k = 0; k < c1; k++) {
				data[k+1] = matA[i][k];
            }
	
			for (k = 0; k < r2; k++) {
				data[k+c1+1] = matB[k][j];
            }
			
			// creates threads
			pthread_create(&threads[count++], NULL, mult, (void*)(data));
		}
    }
	
	printf("Matrix C (A x B):\n");
	for (i = 0; i < max; i++) {
	    void *k;
	    // joins all threads and collecting return value 
	    pthread_join(threads[i], &k);
	    int *p = (int *)k;
	    printf("%d ",*p);

	    if ((i + 1) % c2 == 0) {
		    printf("\n");
        }
    }

	return 0;
}