include ../Make.defines

OBJS = ../event/*.o

PROGS =	gateway

all:	${PROGS}

gateway:	gateway.o engine.o server.o engine_manager.o acceptor.o
		${CC} ${CFLAGS} -o $@ gateway.o engine.o server.o engine_manager.o acceptor.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
