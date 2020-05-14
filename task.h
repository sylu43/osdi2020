#include <stdint.h>
#include "uart.h"

typedef struct {
    unsigned long x19;
    unsigned long x20;
    unsigned long x21;
    unsigned long x22;
    unsigned long x23;
    unsigned long x24;
    unsigned long x25;
    unsigned long x26;
    unsigned long x27;
    unsigned long x28;
    unsigned long fp;
    unsigned long pc;
    unsigned long sp;
}cpu_context;

struct task_t {
	cpu_context context;
	uint8_t id;
	char *stack;
	struct task_t *next;
};

uint8_t task_count;
struct task_t task_pool[64];
char kstack_pool[64][4096];
struct task_t *prev,*current;
struct task_t *runqueue;

void idle();
void task1();
void task2();
