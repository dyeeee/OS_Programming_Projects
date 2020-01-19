#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>     //For sleep calls
#include <time.h>       //For random time wait
#include <semaphore.h>  //Using semaphores as signals for availability of business 

#define MAX_NUMBER_OF_SEATS 3  
#define SLEEP_MAX 5 

pthread_t TA;  // Teacher thread
pthread_t *Students;  // Pointer to an array of students thread

pthread_mutex_t mutex;  // global mutex

sem_t Teacher;
sem_t Student;
sem_t teach;
sem_t finish;

int seat[3]; 
int next_seat = 0; 
int next_teach = 0; 

void *studentBehavior(void *threadID);
void *taBehavior();
void rand_sleep(void); 

int studentsNumber = 5;
int waiting_students = 0;


int main(){
	/// Initialization ///
	sem_init(&Teacher, 0, 0); 
	sem_init(&Student, 0, 0);
	// first 0: Semaphore shared among threads not processes 
	// second 0: Initialize sem value (TA is initially asleep, No students right now)
	sem_init(&teach, 0, 0);
	sem_init(&finish, 0, 0);
	
	
	pthread_mutex_init(&mutex, NULL); // Init mutex
	
	/// Create students threads
	Students = (pthread_t*)malloc(sizeof(pthread_t)*studentsNumber); // Allocate memory for 5 students
	for(int i=0;i<studentsNumber; i++)
        pthread_create(&Students[i], NULL, studentBehavior, (void*)i+1); // Create threads. Passing number i as SID
        
    /// Create TA thread
	pthread_create(&TA, NULL, taBehavior,NULL);
	
	/// Waiting TA&Students thread finished.
	pthread_join(TA, NULL);
	for(int i=0;i<studentsNumber; i++)
        pthread_join(Students[i], NULL);

	free(Students);
	return 0;
}

void *taBehavior(){
	while(1){
		sem_wait(&Student);
		printf("	[ta] TA is readying.\n"); 
		sem_post(&Teacher); 
		
		sem_wait(&teach);
		printf("	[ta] TA is teaching student.\n	[ta] Teaching finish.\n"); 
		rand_sleep(); 
		sem_post(&finish);

	}
}


void *studentBehavior(void *threadID){
	int id = (int*)threadID; 
	//printf("[stu] student %d is leaving for TA office\n",id);
	while(1) 
 	{ 
 		rand_sleep();
 		pthread_mutex_lock(&mutex); 
 		if(waiting_students < MAX_NUMBER_OF_SEATS)	 
 		{ 
 			waiting_students ++ ;
 			pthread_mutex_unlock(&mutex); 
 			 
 			
 			pthread_mutex_lock(&mutex);
 			sem_post(&Student);
 			seat[next_seat] = id; 
 			int temp = next_seat;  //存下当前坐在哪一张座椅上
 			printf("[stu] student %d is sitting outside. Waiting students : %d\n",id,waiting_students); 
 			printf("		[Seats]  : [1] %d [2] %d [3] %d\n",seat[0],seat[1],seat[2]); 
 			next_seat = (next_seat + 1) % MAX_NUMBER_OF_SEATS; 
 			pthread_mutex_unlock(&mutex);
 			
 			sem_wait(&Teacher);  
 			printf("[stu] student %d stand up. \n",id); 
 			seat[temp] = 0;   //站起来，将自己的座椅设置为空
 			printf("		[Seats]  : [1] %d [2] %d [3] %d\n",seat[0],seat[1],seat[2]);
 			
 			pthread_mutex_lock(&mutex); 
 			waiting_students -- ;
 			printf("[stu] student %d is going inside.\n",id); 
 			 
 			sem_post(&teach); 
 			sem_wait(&finish);  
 			pthread_mutex_unlock(&mutex); 
 			
 			//break;			
 		}
 		else
 		{
 			pthread_mutex_unlock(&mutex); 
 			printf("[stu] no more seats. Student %d will be back later\n",id);
 			rand_sleep();
 		}
 	}
}


 void rand_sleep(void){ 
 	int time = rand() % SLEEP_MAX + 1; 
 	sleep(time); 
 }

















