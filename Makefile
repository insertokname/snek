SRC := src

ifndef TEMPOUT 
bin :=bin
else
bin := ${TEMPOUT}/bin
endif

all: ${bin}
	g++ ${SRC}/board.cpp ${SRC}/loop.cpp ${SRC}/init.cpp ${SRC}/main.cpp `sdl2-config --cflags --libs` -o ${bin}/snek

${bin}:
	@mkdir ${bin}

undefine TEMPOUT 