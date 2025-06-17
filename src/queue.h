#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

#define QUEUE_SIZE 100

typedef struct {
    char data[QUEUE_SIZE];
    int head;
    int tail;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} queue_t;

void queue_init(queue_t *q);
void queue_destroy(queue_t *q);
void enqueue(queue_t *q, char c);
void dequeue(queue_t *q, char *c);

#endif // QUEUE_H
