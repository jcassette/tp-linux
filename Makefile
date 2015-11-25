CC = gcc
CXX = g++
CFLAGS = -Wall -std=gnu99
CXXFLAGS = -Wall
LDFLAGS = 


all: client.elf server.elf
	
client.elf: client.o
	$(CC) $(LDFLAGS) $^ -o $@
	
server.elf: server.o
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f *.o
	rm -f *.elf

