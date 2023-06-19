CC=g++
TARGET = snake
OBJECT=src/Control.o src/Snake.o src/Main.o

all:$(TARGET)

snake: $(OBJECT)
	$(CC) -g -o snake src/Main.cpp src/Control.cpp src/Snake.cpp -std=c++11 -lncurses

Contorl.o : src/Control.cpp src/Control.h
	$(CC) -g -c src/Control.cpp

Snake.o : src/Snake.cpp src/Snake.h
	$(CC) -g -c src/Snake.cpp

clean:
	rm -rf src/*.o src/*.exe
