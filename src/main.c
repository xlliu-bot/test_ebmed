#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "queue.h"

#define SEND_THRESHOLD 10

void* sender_thread(void* arg) {
    queue_t *q = (queue_t*)arg;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return NULL;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7889);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    char buf[SEND_THRESHOLD];
    int idx = 0;

    while (1) {
        char c;
        dequeue(q, &c);
        buf[idx++] = c;
        if (idx >= SEND_THRESHOLD) {
            sendto(sockfd, buf, idx, 0, (struct sockaddr*)&addr, sizeof(addr));
            idx = 0;
        }
    }
    close(sockfd);
    return NULL;
}

int main() {
    queue_t q;
    queue_init(&q);

    pthread_t tid;
    if (pthread_create(&tid, NULL, sender_thread, &q) != 0) {
        perror("pthread_create");
        return 1;
    }

    const char *message = "Hello from queue! This data will be sent over UDP.";
    size_t len = strlen(message);
    for (size_t i = 0; i < len; ++i) {
        enqueue(&q, message[i]);
        usleep(50000); // simulate producer delay
    }

    pthread_join(tid, NULL); // In practice you might not join

    queue_destroy(&q);
    return 0;
}
