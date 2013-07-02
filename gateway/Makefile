include ../Make.defines

OBJS = ../event/*.o

PROGS =	server

all:	${PROGS}

server:	mgeeg.o global_gateway_server.o
		${CC} ${CFLAGS} -o $@ mgeeg.o global_gateway_server.o ${OBJS} ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
