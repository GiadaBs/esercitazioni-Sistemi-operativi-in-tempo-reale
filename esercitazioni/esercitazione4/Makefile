CC = gcc
CFLAGS = -g -Wall

all: server client_c client_f

server: server.o list.o
	$(CC) $(CFLAGS) -o server server.o list.o

client_c: client_c.o list.o
	$(CC) $(CFLAGS) -o client_c client_c.o list.o

client_f: client_f.o list.o
	$(CC) $(CFLAGS) -o client_f client_f.o list.o

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f server client_c client_f *.o *~
