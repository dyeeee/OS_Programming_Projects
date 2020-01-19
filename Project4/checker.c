

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_THREADS 	11
#define PUZZLE_SIZE 		9

pthread_mutex_t mut;

void *row_worker(void *param); 		/* thread that checks rows */
void *column_worker(void *param); 	/* thread that checks columns */
void *subfield_worker(void *param); 	/* thread that checks subfields */

void disp(int p[PUZZLE_SIZE][PUZZLE_SIZE]);

/* example puzzle */
int puzzle[PUZZLE_SIZE][PUZZLE_SIZE] = {
			{5,3,4,6,7,8,9,1,2},
			{6,8,2,1,9,5,3,4,8},
			{1,9,8,3,4,2,5,6,7},
			{8,5,9,7,6,1,4,2,3},
			{4,2,6,8,5,3,7,9,1},
			{7,1,3,9,2,4,8,5,6},
			{9,6,1,5,3,7,2,8,4},
			{2,8,7,4,1,9,6,3,5},
			{3,4,5,2,8,6,1,7,9}
		};

/* data structure for passing data to threads */
struct param
{
	int row;
	int col;
};

int check_array[]={1,2,3,4,5,6,7,8,9};

int main(int argc, char *argv[])
{
    pthread_mutex_init(&mut,NULL);//用默认属性初始化互斥锁
	pthread_t thread[11];
	int check[11];

	struct param p[11];
	p[0].row=0;p[0].col=0; //row worker
	p[1].row=0;p[1].col=0; //row worker
	p[2].row=0;p[2].col=0; //subfield_worker 1
	p[3].row=0;p[3].col=3; //subfield_worker 2
	p[4].row=0;p[4].col=6; //subfield_worker 3
	p[5].row=3;p[5].col=0; //subfield_worker 4
	p[6].row=3;p[6].col=3; //subfield_worker 5
	p[7].row=3;p[7].col=6; //subfield_worker 6
	p[8].row=6;p[8].col=0; //subfield_worker 7
	p[9].row=6;p[9].col=3; //subfield_worker 8
	p[10].row=6;p[10].col=6; //subfield_worker 9


	//create threads
	pthread_create(&thread[0],NULL,row_worker,&p[0]);
	pthread_create(&thread[1],NULL,column_worker,&p[1]);
	pthread_create(&thread[2],NULL,subfield_worker,&p[2]);
	pthread_create(&thread[3],NULL,subfield_worker,&p[3]);
	pthread_create(&thread[4],NULL,subfield_worker,&p[4]);
	pthread_create(&thread[5],NULL,subfield_worker,&p[5]);
	pthread_create(&thread[6],NULL,subfield_worker,&p[6]);
	pthread_create(&thread[7],NULL,subfield_worker,&p[7]);
	pthread_create(&thread[8],NULL,subfield_worker,&p[8]);
	pthread_create(&thread[9],NULL,subfield_worker,&p[9]);
	pthread_create(&thread[10],NULL,subfield_worker,&p[10]);
	

	//wait for all threads to finish executing before proceeding
	pthread_join(thread[0],&check[0]);
	pthread_join(thread[1],&check[1]);
	pthread_join(thread[2],&check[2]);
	pthread_join(thread[2],&check[2]);
	pthread_join(thread[3],&check[3]);
	pthread_join(thread[4],&check[4]);
	pthread_join(thread[5],&check[5]);
	pthread_join(thread[6],&check[6]);
	pthread_join(thread[7],&check[7]);
	pthread_join(thread[8],&check[8]);
	pthread_join(thread[9],&check[9]);
	pthread_join(thread[10],&check[10]);
	

	int result=1;

	for(int i=0;i<11;i++){
		if(check[i]==0)
			result=0;
	}
	if(result==1)
		printf("The solution is valid\n");
	else
		printf("The solution is invalid\n");


	return 0;
}

void *row_worker(void *param)
{
	struct param *p=(struct param*)param;
	int test_array[9];
	int valid=1;
	int tmp;
	for(int row=p->row;row<9;row++){
		
		for(int col=0;col<9;col++){
			test_array[col]=puzzle[row][col];
		}
		
		//sort
		for(int i=0;i<9;i++){
			for(int j=i+1;j<9;j++){
				if(test_array[i]>test_array[j]){
					tmp = test_array[i];
					test_array[i]=test_array[j];
					test_array[j]=tmp;
				}
			}
		}
		
		/*
		//display
		printf("row sorted array %d: ",row);
	    for(int i=0;i<9;i++){
	        printf("%d, ",test_array[i]);
	    }
	    printf("\n");
	    */
		
		//compare
		for(int i=0;i<9;i++){
			if(test_array[i] != check_array[i]){
				valid=0;
				printf("Row %d is invalid.\n",row+1);
				break;
			}
		}
		
	}
	if(valid == 0){
		pthread_exit((int*)0);
	}
	pthread_exit((int*)1);
}

void *column_worker(void *param)
{
	struct param *p=(struct param*)param;
	int test_array[9];
	int valid=1;
	int tmp;
	for(int col=p->col;col<9;col++){
		
		for(int row=0;row<9;row++){
			// printf("%d,%d,%d\n",row,col,puzzle[row][col]);
			test_array[row]=puzzle[row][col];
		}
		
		//sort
		for(int i=0;i<9;i++){
			for(int j=i+1;j<9;j++){
				if(test_array[i]>test_array[j]){
					tmp = test_array[i];
					test_array[i]=test_array[j];
					test_array[j]=tmp;
				}
			}
		}
		
		/*
		//display
		printf("column sorted array %d: ",col);
	    for(int i=0;i<9;i++){
	        printf("%d, ",test_array[i]);
	    }
	    printf("\n");
	    */
		
		//compare
		for(int i=0;i<9;i++){
			if(test_array[i] != check_array[i]){
				valid=0;
				printf("Column %d is invalid.\n",col+1);
				break;
			}
		}
		
	}
	if(valid == 0){
		pthread_exit((int*)0);
	}
	pthread_exit((int*)1);
}

void *subfield_worker(void *param)
{
	struct param *p=(struct param*)param;
	int test_array[9];
	int index=0;
	int valid=1;
	int tmp;
	for(int row=p->row;row<p->row+3;row++){
		for(int col=p->col;col<p->col+3;col++){
			//printf("%d,%d,%d\n",row,col,puzzle[row][col]);
			test_array[index]=puzzle[row][col];
			index += 1;
		}
	}
	//sort
	for(int i=0;i<9;i++){
		for(int j=i+1;j<9;j++){
			if(test_array[i]>test_array[j]){
				tmp = test_array[i];
				test_array[i]=test_array[j];
				test_array[j]=tmp;
			}
		}
	}
	
	/*
	//display
	printf("subfiled sorted array: ");
	for(int i=0;i<9;i++){
	      printf("%d, ",test_array[i]);
	}
	printf("\n");
	*/
		
	//compare
	for(int i=0;i<9;i++){
		if(test_array[i] != check_array[i]){
			valid=0;
			printf("Subfiled(row:%d-%d col:%d-%d) is invalid.\n",p->row+1,p->row+3,p->col+1,p->col+3);
			break;
		}
	}
		
	
	if(valid == 0){
		pthread_exit((int*)0);
	}
	pthread_exit((int*)1);
	
}
