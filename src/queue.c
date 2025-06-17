#include "queue.h"
#include <stdlib.h>

void queue_init(queue_t *q) {
    q->head = q->tail = q->count = 0;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);
}

void queue_destroy(queue_t *q) {
    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->not_empty);
    pthread_cond_destroy(&q->not_full);
}

void enqueue(queue_t *q, char c) {
    pthread_mutex_lock(&q->mutex);
    while (q->count == QUEUE_SIZE) {
        pthread_cond_wait(&q->not_full, &q->mutex);
    }
    q->data[q->tail] = c;
    q->tail = (q->tail + 1) % QUEUE_SIZE;
    q->count++;
    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->mutex);
}

void dequeue(queue_t *q, char *c) {
    pthread_mutex_lock(&q->mutex);
    while (q->count == 0) {
        pthread_cond_wait(&q->not_empty, &q->mutex);
    }
    *c = q->data[q->head];
    q->head = (q->head + 1) % QUEUE_SIZE;
    q->count--;
    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->mutex);
}
