SRC := src

ifndef TEMPOUT 
bin :=bin
else
bin := ${TEMPOUT}/bin
endif

all: ${bin} build/board.o build/loop.o build/init.o build/main.o
	g++ build/board.o build/loop.o build/init.o build/main.o `sdl2-config --cflags --libs` -o ${bin}/snek

build/board.o: build
	g++ -c ${SRC}/board.cpp -o build/board.o

build/loop.o: build
	g++ -c ${SRC}/loop.cpp -o build/loop.o

build/init.o: build
	g++ -c ${SRC}/init.cpp -o build/init.o

build/main.o: build
	g++ -c ${SRC}/main.cpp -o build/main.o

build:
	@mkdir build

${bin}:
	@mkdir ${bin}

undefine TEMPOUT 