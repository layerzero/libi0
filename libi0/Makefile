.PHONY: all clean

all: libi0rt.o malloc.o

libi0rt.o: stdlib.c sio.h stddef.h stdint.h stdio.h string.h time.h unistd.h
	$(CC0_CC) -o libi0rt.o stdlib.c

malloc.o: malloc.c libi0rt.o
	$(CC0_CC) -o malloc.o malloc.c

clean:
	rm -rf libi0rt.o malloc.o
