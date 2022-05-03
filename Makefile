CC:=gcc
CFLAGS:=
LDFLAGS:=

all: test

test: test.o example.o
	$(CC) $(CFLAGS) test.o example.o -o test $(LDFLAGS)

example.o: example.c
	$(CC) $(CFLAGS) -o example.o -c example.c 

test.o: test.c
	$(CC) $(CFLAGS) -o test.o -c test.c

clean:
	rm -f *.gcov
	rm -f *.gcda *.gcno
	rm -f *.o
	rm -f test