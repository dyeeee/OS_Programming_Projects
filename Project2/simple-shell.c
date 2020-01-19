/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 * Rewrite by DuYe 2019 222017321102034
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE        80 /* 80 chars per line, per command */
#define HISTORY_NUM  10 //the size of history function

//define a queue for history function
typedef struct {
    char arr[HISTORY_NUM][MAX_LINE];
    int head;
    int tail;
} Queue;

void queue_init(Queue *p_queue);
int queue_full(const Queue *p_queue);
int queue_empty(const Queue *p_queue);
int queue_size(const Queue *p_queue);
int queue_push(Queue *p_queue, char *val); // When queue full, pop the least recetn element
int queue_pop(Queue *p_queue, char *get);
int queue_printf(const Queue *p_queue);
int queue_find(const Queue *p_queue, int index,char *tmp);

int main(void)
{
    char *args[MAX_LINE/2 + 1];    /* command line (of 80) has max of 40 arguments */
    int i;
    int background = 0;
    
    Queue his_queue;
    queue_init(&his_queue);
    
    while (1){
        printf("osh> ");
        fflush(stdout);
        i = 0;
        
        //User input
        char tmp[1024] = {0};
        char input_og[1024] = {0};
        fgets(tmp, sizeof(tmp)-1, stdin);
        tmp[strlen(tmp)-1]=0;
        strcpy(input_og, tmp);
	
		// cut the command
        int i=0;
    	args[0] = strtok(tmp," ");
    	while(args[i] != NULL){
        	//printf("[%s]",args[i]);
        	i++;
        	args[i] = strtok(NULL," ");    
    	}

		//background running tag for "!!/!1/!2.."
        if(strcmp(args[i-1],"&") == 0){
            background = 1;
            args[i-1] = NULL;
            //printf("Background Running..\n");
        }       


		//check !!/!2/!3.. exec or not
		int check = 1;
        
        if(strcmp(args[0],"exit") == 0){ break; } //exit func
        else if(strcmp(args[0],"history") == 0){//history func{
			queue_printf(&his_queue);
			continue; 
		} 
        else if(strcmp(args[0], "!!") == 0){
            check = queue_find(&his_queue, queue_size(&his_queue), input_og);
        }
		else if(strcmp(args[0], "!1") == 0){
            check = queue_find(&his_queue, 1, input_og);
        }
        else if(strcmp(args[0], "!2") == 0){
            check = queue_find(&his_queue, 2, input_og);
        }
        else if(strcmp(args[0], "!3") == 0){
            check = queue_find(&his_queue, 3, input_og);
        }
        else if(strcmp(args[0], "!4") == 0){
            check = queue_find(&his_queue, 4, input_og);
        }
        else if(strcmp(args[0], "!5") == 0){
            check = queue_find(&his_queue, 5, input_og);
        }
        else if(strcmp(args[0], "!6") == 0){
            check = queue_find(&his_queue, 6, input_og);
        }
        else if(strcmp(args[0], "!7") == 0){
            check = queue_find(&his_queue, 7, input_og);
        }
        else if(strcmp(args[0], "!8") == 0){
            check = queue_find(&his_queue, 8, input_og);
        }
        else if(strcmp(args[0], "!9") == 0){
            check = queue_find(&his_queue, 9, input_og);
        }
        else if(strcmp(args[0], "!10") == 0){
            check = queue_find(&his_queue, 10, input_og);
        }
        else {
            queue_push(&his_queue, input_og);
        }
        
		//!!/!2/!3.. failed
		if(!check){continue;}

		// cut the command
        i=0;
    	args[0] = strtok(input_og," ");
    	while(args[i] != NULL){
        	//printf("[%s]",args[i]);
        	i++;
        	args[i] = strtok(NULL," ");    
    	}
        
		//background running tag
        if(strcmp(args[i-1],"&") == 0){
            background = 1;
            args[i-1] = NULL;
            //printf("Background Running..\n");
        }
	     

        //fork the process
        pid_t id = fork(); //pid_t is the type of pid
        if(id == 0){//child process
            execvp(args[0],args);
            perror("execvp failed.");
            exit(1);
        }
        else{
            if(background == 0){
                waitpid(id,NULL,0); // wait for child process
            }
            else{//background running
				printf("Background Running..\n");
				background = 0;
                usleep(10000);
            }
        }
    }
    return 0;
}

void queue_init(Queue *p_queue) {
    p_queue->head = 0;
    p_queue->tail = 0;
}

int queue_empty(const Queue *p_queue) {
    return !(p_queue->tail - p_queue->head);
}

int queue_full(const Queue *p_queue) {
    if (p_queue->tail == HISTORY_NUM) {
        printf("%d",p_queue->tail);
        return 1;
    }
    return 0;
}

int queue_push(Queue *p_queue, char* val) {
    if (queue_full(p_queue)) {
        //printf("FULL\n");
        int i;
        for (i =0; i<p_queue->tail-1; i++) {
            strcpy(p_queue->arr[i], p_queue->arr[i+1]);
        }
        //p_queue->tail--;
        strcpy(p_queue->arr[p_queue->tail-1], val);
        return 0;
    }
    else {
        strcpy(p_queue->arr[p_queue->tail], val);
        p_queue->tail++;
        return 1;
    }
}

int queue_size(const Queue *p_queue) {
    return (p_queue->tail - p_queue->head);
}

int queue_printf(const Queue *p_queue){
    int i;
    if (queue_empty(p_queue)) {
	printf("No commands in history.\n");
        return 0;
    }
    else {
        for (i = p_queue->tail-1; i >=0; i--){
            printf("    %2d  [%s] \n", i+1,p_queue->arr[i]);
        }
    }
    return 0;
}

int queue_find(const Queue *p_queue, int index, char *tmp){
    if(queue_empty(p_queue)){
	printf("No commands in history.\n");
	return 0;
    }
    else if (queue_size(p_queue)<index) {
        printf("No such command in history.\n");
        return 0;
    }
    else {
        printf("Exec [%s]\n", p_queue->arr[index-1]);
        strcpy(tmp, p_queue->arr[index-1]);
    }
    return 1;
}
