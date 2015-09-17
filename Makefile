CC=gcc
CFLAGS=-O0 -Wall
DEPS = 

#OBJ = phods.o phodsOPT.o 

#%.o: %.c $(DEPS)
#	$(CC) -c -o $@ $< $(CFLAGS)

all: phodsOPT ask1_1
#all: phods phodsOPT

phods: phods.o
	$(CC) $(CFLAGS) -o $@ $^ 

ask1_1: ask1_1.o
	$(CC) $(CFLAGS) -o $@ $^ 

phodsOPT: phodsOPT.o
	$(CC) $(CFLAGS) -o $@ $^ 

.PHONY: clean

clean:
	rm -f *.o *~
