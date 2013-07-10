include ../Make.defines

PROGS =	../structures/link_list.o

all:	${PROGS}

client:	client.o
		${CC} ${CFLAGS} -o $@ client.o ${LIBS}

test_list:	test_list.o
		${CC} ${CFLAGS} -o $@ test_list.o ${PROGS} ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
