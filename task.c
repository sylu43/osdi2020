#include "uart.h"
#include "task.h"
#include <string.h>

extern struct task* get_current();

void schedule(){
	struct task_t *temp;
	if(runqueue!=NULL){
		prev=current;
		if(current->id!=0){
			for(temp=runqueue;temp->next!=NULL;temp=temp->next);
			temp->next=prev;
			current=prev->next;
			runqueue=current;
		}
		else{
			current=runqueue;
		}
		prev->next=NULL;
		switch_to(prev,current);
	}
	//for(int i=1;i<task_count;i++){
	//	current=&task_pool[0];
	//	switch_to(current,&task_pool[i]);
	//}
}

void idle(){
	current=&task_pool[0];
	while(1){
		schedule();
	}
}

void task1(){
	int i=0;
	print("1\r\n");
	//while(1);
	while(i++<10000000);
	schedule();
	//switch_to(&task_pool[1],&task_pool[0]);
}

void task2(){
	int i=0;
	print("2\r\n");
	while(i++<10000000);
	schedule();
	//switch_to(&task_pool[2],&task_pool[0]);
}

void privilege_task_create(void (*func)()){
	unsigned char *ptr=(unsigned char *)&task_pool[task_count].context;
	struct task_t *temp;
	for(int i=0;i<sizeof(cpu_context);i++){
		*ptr++='\0';
	}
	task_pool[task_count].context.pc=(unsigned long)func;
	task_pool[task_count].context.sp=&kstack_pool[task_count];
	task_pool[task_count].id=task_count;
	task_pool[task_count].stack=kstack_pool[task_count];
	print_dec(task_pool[task_count].id);
	if(task_count==1){
		runqueue=&task_pool[task_count];
	}
	else if(task_count!=0){
		for(temp=runqueue;temp->next!=NULL;temp=temp->next);
		temp->next=&task_pool[task_count];
	}
	task_count++;
}

	
void task_init(){
	task_count=0;
	runqueue=NULL;
	privilege_task_create(idle);
	privilege_task_create(task1);
	privilege_task_create(task2);
	while(uart_read()!='c')
		print("done");
	
}

