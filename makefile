CC = gcc
CFLAGS = -Wall -g

build: main

main:
	gcc main.c F1.c -o main

run: build
	./main

clean:
	rm -f *.o main