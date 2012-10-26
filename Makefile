CC=gcc
CFLAGS=-Wall -Werror -g -std=c99
LDFLAGS=

all: eth_switch1 eth_switch2 eth_switch3
eth_switch1: main.o eth_switch1.o
eth_switch2: main.o eth_switch2.o
eth_switch3: main.o eth_switch3.o

eth_switch1.o: eth_switch1.c switch.h capacity.h
eth_switch2.o: eth_switch2.c switch.h capacity.h
eth_switch3.o: eth_switch3.c switch.h capacity.h
main.o: main.c switch.h capacity.h

clean:
	-rm -rf eth_switch1.o eth_switch2.o eth_switch3.o main.o \
		switch1 switch2 switch3
