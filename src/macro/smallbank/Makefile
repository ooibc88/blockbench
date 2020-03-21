CC=g++
CFLAGS=-std=c++11 -g -Wall -pthread -I./ 
LDFLAGS= -lpthread  -lcurl -lrestclient-cpp 
SUBDIRS=api_adapters utils
OBJECTS=api_adapters/SmallBank.o api_adapters/EVMDB.o api_adapters/Fabric.o utils/timer.o
EXEC=driver

all: $(SUBDIRS) $(EXEC)

$(SUBDIRS):
	$(MAKE) -C $@

$(EXEC): $(wildcard *.cc)
	$(CC) $(CFLAGS) $< $(OBJECTS) $(LDFLAGS) -o $@

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done
	$(RM) $(EXEC)

.PHONY: $(SUBDIRS) $(EXEC)
