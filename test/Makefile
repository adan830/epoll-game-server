include ../Make.defines

PROGS =	../structures/link_list.o
PROGS1 = ../gateway/packet.o
PROGS2 =	../structures/double_link_list.o

all:	${PROGS}

client:	client.o
		${CC} ${CFLAGS} -o $@ client.o ${LIBS}

test_list:	test_list.o
		${CC} ${CFLAGS} -o $@ test_list.o ${PROGS} ${LIBS}

test_packet:	test_packet.o
		${CC} ${CFLAGS} -o $@ test_packet.o ${PROGS1} ${LIBS}

test_double_list:	test_double_list.o
		${CC} ${CFLAGS} -o $@ test_double_list.o ${PROGS2} ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
