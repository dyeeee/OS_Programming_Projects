/**
 * Solution to exercise project 2
 *
 * Operating System Concepts - Ninth Edition
 * John Wiley & Sons - 2013.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE				7
#define NUMBER_OF_THREADS	3

void *sorter(void *params);	/* thread that performs basic sorting algorithm */
void *merger(void *params);	/* thread that performs merging of results */

int list[SIZE] = {12,1,7,8,6,4,3};
int result[SIZE];
int thread_count = 0;

struct parameters
{
	int from_index;
	int to_index;
};

int main (int argc, const char * argv[]) 
{
	printf("Before process: \n  ");
	for(int i=0;i<SIZE;i++){
		printf("%d ",list[i]);
	}
	printf("\n");
	
	pthread_t thread[3];
	int check[3];
	
	struct parameters p[3];
	p[0].from_index=0;p[0].to_index=SIZE/2-1;
	p[1].from_index=SIZE/2;p[1].to_index=SIZE-1;
	p[2].from_index=0;p[2].to_index=SIZE;

	pthread_create(&thread[0],NULL,sorter,&p[0]);
	pthread_create(&thread[1],NULL,sorter,&p[1]);
	//pthread_create(&thread[2],NULL,merger,&p[2]); //???? put here not ok
	pthread_join(thread[0],&check[0]);
	pthread_join(thread[1],&check[1]);
	
	pthread_create(&thread[2],NULL,merger,&p[2]);
	pthread_join(thread[2],&check[2]);
	
	printf("Before merge: \n  ");
	for(int i=0;i<SIZE;i++){
		printf("%d ",list[i]);
	}
	
	printf("\nAfter merge: \n  ");
	
	for(int i=0;i<SIZE;i++){
		printf("%d ",result[i]);
	}
	printf("\n");
    return 0;
}

/**
 * Sorting thread.
 *
 * This thread can essentially use any algorithm for sorting
 */

void *sorter(void *params)
{
	struct parameters *p=(struct parameters*)params;
	printf("index: %d %d\n",p->from_index,p->to_index);
	int temp = 0;
	for(int i=p->from_index;i<=p->to_index;i++){
		for(int j=i+1;j<=p->to_index;j++){
			if(list[j]<list[i]){
				temp = list[i];
				list[i]=list[j];
				list[j] = temp;
			}
		}
	}
	thread_count++;
	printf("Thread %d result: \n  ",thread_count);
	for(int i=p->from_index;i<=p->to_index;i++){
		printf("%d ",list[i]);
	}
	printf("\n");
	
	//printf("end");
	pthread_exit(0);
}

/**
 * Merge thread
 *
 * Uses simple merge sort for merging two sublists
 */

void *merger(void *params)
{	

	int i = 0;
	int j = SIZE/2;
	int k = 0;
	
	while(i<=SIZE/2-1 && j<=SIZE-1){
		if(list[i] < list[j]){
			result[k] = list[i];
			i++;
			k++;
		}
		else{
			result[k] = list[j];
			j++;
			k++;
		}
		//printf("%d %d\n",i,j);
	}
	
	while(i<=4){
		result[k] = list[i];
		i++;
		k++;
	}
	
	while(j<=9){
		result[k] = list[j];
		j++;
		k++;
	}
	
	// printf("hi\n");
	pthread_exit(0);
}
