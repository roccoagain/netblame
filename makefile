all: netblame

netblame.o: netblame.c
	gcc -Wall -Wextra -c netblame.c -o netblame.o

netblame: netblame.o
	gcc -Wall -Wextra netblame.o -o netblame -lncurses -lpcap

clean:
	rm -f netblame netblame.o
