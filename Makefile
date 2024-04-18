SRC := src

ifndef out 
bin :=bin
else
bin := ${out}/bin
endif

all:
	g++ ${SRC}/board.cpp ${SRC}/loop.cpp ${SRC}/init.cpp ${SRC}/main.cpp `sdl2-config --cflags --libs` -o ${bin}/snek
