CC=gcc

ppm_writer:
	$(CC) -c ppm_writer.c

test: ppm_writer
	$(CC) -c test.c
	$(CC) -o test ppm_writer.o test.o -lm

clean:
	rm -f *.o *.ppm test
