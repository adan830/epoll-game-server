include ../Make.defines

OBJS = gateway.o engine.o socket.o engine_manager.o acceptor.o \
		fdevent.o fdevent_linux_sysepoll.o epoll.o packet.o
# OBJS = ../event/*.o
STRUCTURES = ../structures/*.o

PROGS =	gateway

all:	${PROGS}

gateway:	${OBJS}
		${CC} ${CFLAGS} -o $@ ${OBJS} ${STRUCTURES} ${LIBS}

clean:
		rm -f ${OBJS}
