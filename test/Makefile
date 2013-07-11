include ../Make.defines

PROGS =	../structures/link_list.o
PROGS1 = ../gateway/packet.o

all:	${PROGS}

client:	client.o
		${CC} ${CFLAGS} -o $@ client.o ${LIBS}

test_list:	test_list.o
		${CC} ${CFLAGS} -o $@ test_list.o ${PROGS} ${LIBS}

test_packet:	test_packet.o
		${CC} ${CFLAGS} -o $@ test_packet.o ${PROGS1} ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
