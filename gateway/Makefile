include ../Make.defines

OBJS = gateway.o engine.o server.o engine_manager.o acceptor.o \
		fdevent.o fdevent_linux_sysepoll.o epoll.o
# OBJS = ../event/*.o

PROGS =	gateway

all:	${PROGS}

gateway:	${OBJS}
		${CC} ${CFLAGS} -o $@ ${OBJS} ${LIBS}

clean:
		rm -f ${OBJS}
