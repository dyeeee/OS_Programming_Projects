#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> 

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3

pthread_mutex_t mutex; // global mutex

int ALL_RESOURCE[NUMBER_OF_RESOURCES];

/*available amount of each resource */
int available[NUMBER_OF_RESOURCES]; //初始资源数量

/*maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/*currently allocated resource */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/*remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

pthread_t *Customers;
void *customerBehavior(void *threadID);

int request_resources(int customerID, int request[]);
int release_resources(int customerID, int release[]);
int safety_check();
void display();

int main(int argc, char const *argv[]){

	if(argc != NUMBER_OF_RESOURCES + 1)
	{
		printf("Lack of parameters. (./banker NumofRA NumofRB NumofRC)\n");
		return -1;
	}
	for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
	{
		ALL_RESOURCE[i] = atoi(argv[i+1]);   //argv[0] is name of program, atoi translated char to int
		available[i] = ALL_RESOURCE[i];
	}

	pthread_mutex_init(&mutex, NULL); // Init mutex
	
	Customers = (pthread_t*)malloc(sizeof(pthread_t)*NUMBER_OF_CUSTOMERS);
	for(int i=0;i<NUMBER_OF_CUSTOMERS; i++){
        pthread_create(&Customers[i], NULL, customerBehavior, (void*)i);
    }
    
    for(int i=0;i<NUMBER_OF_CUSTOMERS; i++){
        pthread_join(Customers[i], NULL); 
	}
	
	free(Customers);
}

void *customerBehavior(void *threadID){
	int id = (int*)threadID; 
	// printf("%d\n",id);
	 
	for(int i=0;i<NUMBER_OF_RESOURCES;i++){
		maximum[id][i] = rand()%ALL_RESOURCE[i] +1 ;  //随机生成进程的最大需求, 限制其小于最大资源量
	}
	
	//一开始NEED = MAX
	for(int i=0;i<NUMBER_OF_RESOURCES;i++){
		need[id][i] = maximum[id][i];
	}
	
	pthread_mutex_lock(&mutex);
	//输出最大需求
	printf("MAX for P%d: ",id);
	for(int i=0;i<NUMBER_OF_RESOURCES;i++){
		printf("%d ",maximum[id][i]);
	}
	printf("\n");
	pthread_mutex_unlock(&mutex); 
	sleep(2);
	
	int release_check = NUMBER_OF_RESOURCES;  // 检查每个资源是否全部得到分配
	int request_check;
	int request[NUMBER_OF_RESOURCES];
	
	//第一次请求
	for(int i=0;i<NUMBER_OF_RESOURCES;i++){
			request[i] = rand() % maximum[id][i] + 1 ;  //需求一定小于自己的最大需求
	}
	display();
	request_check = request_resources(id, request);
	//检查需求情况
	for(int i=0;i<NUMBER_OF_RESOURCES;i++){
		if(maximum[id][i]==allocation[id][i]) release_check-=1;
		// printf("P%d's release check:%d. max[%d][%d]=%d,allocation[%d][%d]=%d \n",id,release_check,id,i,maximum[id][i],id,i,allocation[id][i]);
	}
	
	//第一次请求后，进程没有释放的时候进入循环
	while(release_check != 0){
			//重新生成请求,限制需求一定小于自己的Need
			for(int i=0;i<NUMBER_OF_RESOURCES;i++){
				if(need[id][i]==0){
					request[i] = 0;
				}
				else{
					request[i] = rand() % need[id][i] + 1   ;  //需求一定小于自己的需求
				}
			}
				
		release_check = NUMBER_OF_RESOURCES;
		sleep(1);
		display();
		request_check = request_resources(id, request);
		
		if(request_check == 0){
			//检查需求情况
			for(int i=0;i<NUMBER_OF_RESOURCES;i++){
				if(maximum[id][i]==allocation[id][i]) release_check-=1;
			}
		}
		else{
			//release_check = 0;  //testtesttest
		}
	}
	
	release_resources(id, request);		
}

int request_resources(int customerID, int request[]){
	printf("REQUEST from P%d: %d %d %d\n", customerID, request[0], request[1],request[2]);
	
	/// 资源请求算法
	// 第一步判断Request有没有小于Need, 前面限制了随机请求一定小于Need，错误1就不会发生
	int check1 = 0;
	for(int i=0;i<NUMBER_OF_RESOURCES;i++){
		if(request[i]>need[customerID][i]) {
			check1+=1;
			printf("ERROR: Request[%d]:%d>Need[%d]:%d\n",i,request[i],i,need[customerID][i]);
		}
	}
	if(check1 != 0) return -1;
	
	// 第二步判断Request有没有小于Available
	int check2 = 0 ;
	for(int i=0;i<NUMBER_OF_RESOURCES;i++){
		if(request[i]>available[i]) {
			check2+=1;
			printf("ERROR: Request[%d]:%d>Available[%d]:%d\n",i,request[i],i,available[i]);
		}
	}
	if(check2 != 0) {
		return -2;
	}
	
	/// 第三步，分配资源
	//先保存此时的资源状态
	int current_available[NUMBER_OF_RESOURCES];
	for(int i=0;i<NUMBER_OF_RESOURCES;i++){
		current_available[i] = available[i];
	}
	
	int current_allocation[NUMBER_OF_RESOURCES];
	for(int i=0;i<NUMBER_OF_RESOURCES;i++){
		current_allocation[i] = allocation[customerID][i];
	}
	
	int current_need[NUMBER_OF_RESOURCES];
	for(int i=0;i<NUMBER_OF_RESOURCES;i++){
		current_need[i] = need[customerID][i];
	}
	//printf("Before allocation: \n");
	//display();
	//再分配资源
	for(int i=0;i<NUMBER_OF_RESOURCES;i++){
		available[i] = available[i]-request[i];
	}
	
	for(int i=0;i<NUMBER_OF_RESOURCES;i++){
		allocation[customerID][i] = allocation[customerID][i] + request[i];
	}
	
	for(int i=0;i<NUMBER_OF_RESOURCES;i++){
		need[customerID][i] = need[customerID][i] - request[i];
	}
	
	/// 第四步，检查安全状态
	int check3 = safety_check();
	if(check3 == 0){
		return 0;
	}
	else{
		for(int i=0;i<NUMBER_OF_RESOURCES;i++){
			available[i] = current_available[i];
		}
		for(int i=0;i<NUMBER_OF_RESOURCES;i++){
			allocation[customerID][i] = current_allocation[i];
		}
		for(int i=0;i<NUMBER_OF_RESOURCES;i++){
			need[customerID][i] = current_need[i];
		}
		//printf("Not Safe: \n");
		//display();

		return -2;
	}
}


int release_resources(int customerID, int release[]){
	for(int i=0;i<NUMBER_OF_RESOURCES;i++){
		available[i] += allocation[customerID][i];
		allocation[customerID][i] = 0;
	}
	printf("P%d is released.\n",customerID);
	return 0;
}

int safety_check(){
	int Work[NUMBER_OF_RESOURCES];
	int Finish[NUMBER_OF_CUSTOMERS];
	// 初始化Work
	for(int i=0;i<NUMBER_OF_RESOURCES;i++){
		Work[i] = available[i];
	}
	// 初始化Finishi
	for(int i=0;i<NUMBER_OF_CUSTOMERS;i++){
		Finish[i] = 1;
	}
	
	int safety_line[NUMBER_OF_CUSTOMERS];
	int line_count =0;
	
	//第二步 查找满足条件的i
for(int x=0;x<NUMBER_OF_CUSTOMERS;x++){
	for(int i=0;i<NUMBER_OF_CUSTOMERS;i++){
		int i_exist = 0;
		if(Finish[i] == 1){
			for(int j=0;j<NUMBER_OF_RESOURCES;j++){
				if(need[i][j] > Work[j]) i_exist+=1; 
			}
		}
		else{
			i_exist = 1;
		}
		
		//第三步
		if(i_exist == 0){
			for(int k=0;k<NUMBER_OF_RESOURCES;k++){
				Work[k] = Work[k]+allocation[i][k]; 
			}
			Finish[i] = 0;
			safety_line[line_count] = i;
			line_count += 1;
			break;
		}
	}
}


	int result = NUMBER_OF_CUSTOMERS;
	//第四步
	for(int i=0;i<NUMBER_OF_CUSTOMERS;i++){
		if(Finish[i] == 0) result-=1;
	}
	
	
	if(result == 0){
	pthread_mutex_lock(&mutex); 
		printf("Safe. Allocation Permitted.\n");
		printf("Safety Sequence: ");
	for(int i=0;i<NUMBER_OF_CUSTOMERS;i++){
		printf("%d ", safety_line[i]);
	}
	printf("\n");
	pthread_mutex_unlock(&mutex); 
		return 0; //Safe
	}
	else
		printf("Not Safe. Statement Rolled Back.\n");
		return -1; //not safe
		
}

void display(){
pthread_mutex_lock(&mutex); 
	printf("\nCurrent Snapshot:\n");
	printf("    Max\n");
	for(int i=0;i<NUMBER_OF_CUSTOMERS;i++){
		printf("P%d   ",i);
		for(int j=0;j<NUMBER_OF_RESOURCES;j++){
			printf("%d ",maximum[i][j]);
		}
		printf("\n");
	}

	printf("  Allocation\n");
	for(int i=0;i<NUMBER_OF_CUSTOMERS;i++){
		printf("P%d   ",i);
		for(int j=0;j<NUMBER_OF_RESOURCES;j++){
			printf("%d ",allocation[i][j]);
		}
		printf("\n");
	}
	
	printf("    Need\n");
	for(int i=0;i<NUMBER_OF_CUSTOMERS;i++){
		printf("P%d   ",i);
		for(int j=0;j<NUMBER_OF_RESOURCES;j++){
			printf("%d ",need[i][j]);
		}
		printf("\n");
	}
	
	printf("  Available\n");
	printf("    ");
	for(int i=0;i<NUMBER_OF_RESOURCES;i++){
		printf("%d ",available[i]);
	}
	printf("\n");
pthread_mutex_unlock(&mutex); 
}


