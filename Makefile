CC=gcc
CFLAGS=-Wall -pthread

all: udp_queue

udp_queue: src/main.c src/queue.c src/queue.h
	$(CC) $(CFLAGS) -o udp_queue src/main.c src/queue.c

clean:
	rm -f udp_queue
