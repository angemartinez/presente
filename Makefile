game:
	mkdir -p bin
	gcc -o bin/presente -g -Wall -std=c99 \
		src/*.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
play: game
	cd bin; ./presente
debug: game
	cd bin; valgrind ./presente
