CC = gcc
CFLAGS = -I. -std=c89 -pedantic -Wall -Werror
DEPS = bscanf.h
OBJ = bscanf.o test_bscanf.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test_bscanf: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o test_bscanf
