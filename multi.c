#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
inner sum은 전체 결과 행렬을 계산하여 합을 구하는 방법이다.
outer sum은 결과 행렬을 구하지 않고 부분합을 구해서 배열에 저장하여 전체합을 구하는 방법이다.
*/

unsigned long long *A, *B, *C, *totalSum;

int num_thrd;

unsigned long long sum = 0;

int multi(void * partition);

int main(int argc, char *argv[]) {
	FILE * file1; FILE * file2;
	int i;
	int _partition;
	pthread_t* thread;

	A = (unsigned long long *)malloc(sizeof(unsigned long long) * 4000 * 4000);
	B = (unsigned long long *)malloc(sizeof(unsigned long long) * 4000 * 4000);
	C = (unsigned long long *)malloc(sizeof(unsigned long long) * 4000 * 4000);

	if(argc != 4) {
		exit(-1);
	}
	file1 = fopen(argv[1], "r+");
	file2 = fopen(argv[2], "r+");

	for(i = 0; i < 4000* 4000; i++) {
		fscanf(file1, "%llu", &A[i]);
		fscanf(file2, "%llu", &B[i]);
	}
	num_thrd = atoi(argv[3]);
	thread = (pthread_t*) malloc(num_thrd * sizeof(pthread_t));
	totalSum = (unsigned long long *)malloc(sizeof(unsigned long long) * num_thrd);

	//outer sum
	//printf("This version is using the outer sum\n");

	//inner sum
	printf("This version is using the inner sum\n");


	printf("Let's start make multi thread\n");

	for(i = 0; i < num_thrd; i++) {
		totalSum[i] = 0;
	}

	for (i = 1; i < num_thrd; i++) {
    	if (pthread_create (&thread[i], NULL, multi, (void*)i) != 0 ) {
    		perror("Can't create thread");
    		free(thread);
    		exit(-1);
    	}
  	}

	multi(0);

	for (i = 1; i < num_thrd; i++) {
		pthread_join (thread[i], NULL);
	}
	
//this is inner sum
	printf("The reult sum is ");
	for(i = 0; i < 4000 * 4000; i++) {
		sum += C[i];
	} // compute the result sum out of multi function

//this is outer sum
	// printf("The total sum is ");
	// for(i = 0; i < num_thrd; i++) {
	// 	sum += totalSum[i];
	// }

	free(thread);
	printf("%llu\n", sum);
	return 0;
}

int multi(void * partition) {
	int i, j, k;
	unsigned long long temp = 0;

	int p = (int)partition;
	int from = p * (4000/num_thrd);
	int to = (p+1) * (4000/num_thrd);

	if(num_thrd == 30 && partition == 29) {
		printf("the rest exits\n");
		to = 4000;
	}

// this is outer sum
	// for(i = from; i < to; i++) {
	// 	for(j = 0; j < 4000; j++) {
	// 		C[i * 4000 + j] = 0;
	// 		for(k = 0; k < 4000; k++) {
	// 			temp += A[k + i * 4000] * B[j + k * 4000];
	// 		}
	// 		totalSum[p] += temp;
	// 		temp = 0;
	// 	}
	// }

// this is inener sum
	for(i = from; i < to; i++) {
		for(j = 0; j < 4000; j++) {
			C[i * 4000 + j] = 0;
			for(k = 0; k < 4000; k++) {
				C[i * 4000 + j] += A[k + i * 4000] * B[j + k * 4000];
			}
		}
	}


	return 0;
}
