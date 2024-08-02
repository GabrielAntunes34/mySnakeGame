all: pair.o snake.o fruit.o level.o main.o
	gcc *.o -lncurses -Wall -std=c99 -o main

run: all removeO
	./main

main.o:
	gcc -c main.c -o main.o

level.o:
	gcc -c level.c -o level.o

fruit.o:
	gcc -c fruit.c -o fruit.o

snake.o:
	gcc -c snake.c -o snake.o

pair.o:
	gcc -c pair.c -o pair.o

removeO:
	rm *.o

clean:
	rm main