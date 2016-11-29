#include<lib.h>


void lock(int *used) {
    while(1) {
        lock_ints();
        if (!(*used)) {
            *used = 1;
            break;
        }
    }
    unlock_ints();
}


#define STACK_SIZE 0x1000

struct thread_t {
    uint64_t stack[STACK_SIZE];
    void (*func)();
    void* prev;
    int active;
    int is_daemon;
};

#define Thread struct thread_t

#define N 10
static Thread threads[N];
Thread *new_thread;

extern void switch_threads(void **prev, void *next);
extern void switch_threads_sti(void **prev, void *next);
extern void* init_stack(uint64_t *stack, void(*f)());

#define M (N+2)
static void* queue[M];
static int queue_start = 0;
static int queue_end = 0;
static int queue_used = 0;

static void push_queue(void * el) {
    queue[queue_end] = el;
    queue_end = (queue_end + 1) % M;
}

static void* pop_queue() {
    void * res = queue[queue_start];
    queue_start = (queue_start + 1) % M;
    return res;
}

static void** back_queue() {
    return queue + (queue_end + M - 1) % M;
}

void remove_from_queue(void * thread) {
    lock(&queue_used);
    if (queue_start == queue_end) {
        return;
    }
    for (int i = 0; i < M; i++) {
        void *cur = pop_queue();
        if (cur != thread) {
            push_queue(cur);
        }
    }
    queue_used = 0;
}

void changeThread() {
    lock_ints();
    if (queue_start == queue_end) {
        return;
    }
    void *cur = pop_queue();
    push_queue(0);
    switch_threads_sti(back_queue(), cur);
}

static void threadf() {
    lock_ints();
    Thread* thread = new_thread;
    
    thread->active = 1;
    if (thread->is_daemon) {
        push_queue(thread->prev);
    }
    
    unlock_ints();
    
    (*(thread->func))();
    
    if (thread->is_daemon) {
        printf("while(1)\n");
        while(1);
    }
    
    lock_ints();
    
    thread->active = 0;
    void **prev = 0;
    
    switch_threads_sti(prev, thread->prev);
}

void start_thread(void (*f)(), int is_daemon) {
    lock_ints();
    
    int i = 0;
    while (i < N && threads[i].active) {
        i++;
    }
    
    if (i >= N) {
        printf("Can't create thread\n");
        return;
    }
    
    new_thread = threads + i;
    new_thread -> func = f;
    new_thread -> is_daemon = is_daemon;
    
    for (int j = 0; j < STACK_SIZE; j++) {
        new_thread->stack[j] = 0;
    }
    
    void* stack_p = init_stack(new_thread->stack + STACK_SIZE - 1, threadf);
    switch_threads(&(threads[i].prev), stack_p);
    
    unlock_ints();
}