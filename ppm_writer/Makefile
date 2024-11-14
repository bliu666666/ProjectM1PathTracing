CC:gcc
CFLAGS=-lm

ppm_writer:
	$(CC) $(CFLAGS) -c ppm_writer.c

test: ppm_writer
	$(CC) $(CFLAGS) -c test.c
	$(CC) $(CFLAGS) -o test ppm_writer.o test.o

clean:
	rm -f *.o *.ppm test
