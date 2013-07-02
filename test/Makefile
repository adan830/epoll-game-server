include ../Make.defines

PROGS =	client

all:	${PROGS}

client:	client.o
		${CC} ${CFLAGS} -o $@ client.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
