all: server casaeditrice lettore

server: server.o list.o
	gcc -Wall -o $@ $^

casa_editrice: casaeditrice.o list.o
	gcc -Wall -o $@ $^

%.o: %.c
	gcc -c $<

clean:
	rm -rf *.o server casaeditrice lettore *~

