CC=g++
CFLAGS=-std=c++11 -c -g -Wall -I../
LDFLAGS= -lpthread -lrestclient-cpp
SOURCES=$(wildcard *.cc)
OBJECTS=$(SOURCES:.cc=.o)

all: $(SOURCES) $(OBJECTS)

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) *.o
