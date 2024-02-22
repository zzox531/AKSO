CC 		= gcc
CFLAGS  = -Wall -Wextra -Wno-implicit-fallthrough -std=gnu17 -fPIC -O2
LDFLAGS = -shared -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc -Wl,--wrap=reallocarray -Wl,--wrap=free -Wl,--wrap=strdup -Wl,--wrap=strndup


.PHONY: all clean

all: libseq.so seq_example.o
	gcc -o seq seq_example.o -L. -lseq

libseq.so: memory_tests.o seq.o
	gcc ${LDFLAGS} -o libseq.so memory_tests.o seq.o

seq_example.o: seq_example.c seq.h memory_tests.h

memory_tests.o: memory_tests.c memory_tests.h
	gcc $(CFLAGS) -c memory_tests.c -o memory_tests.o	

seq.o: seq.c memory_tests.h
	gcc $(CFLAGS) -c seq.c -o seq.o

clean:
	rm -f *.o libseq.so seq