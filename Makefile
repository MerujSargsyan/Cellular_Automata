CC=gcc
CFLAGS=-Wall -Wextra -std=gnu99

all:
	mkdir -p build
	cd ./build && cmake ..
	cd ./build && $(MAKE) 

run: 
	./build/bin/CellularAutomata

clean:
	rm -rf build

