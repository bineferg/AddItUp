CC=gccx

all: additup

additup: additup.c
	$(CC) -o additup additup.c

clean:
	rm -f additup *.o
