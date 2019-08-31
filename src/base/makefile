# Build the base objects.

CC = gcc

CCFLAGS = -O -DUNIX

all: Automaton.o Body.o Bond.o Cell.o \
	Emission.o Mechanics.o Orientation.o \
	Particle.o Signal.o

Automaton.o: Automaton.hpp Automaton.cpp Parameters.h
	$(CC) $(CCFLAGS) -c Automaton.cpp

Body.o: Body.hpp Body.cpp Parameters.h
	$(CC) $(CCFLAGS) -c Body.cpp

Bond.o: Bond.hpp Bond.cpp Parameters.h
	$(CC) $(CCFLAGS) -c Bond.cpp

Cell.o: Cell.hpp Cell.cpp Parameters.h
	$(CC) $(CCFLAGS) -c Cell.cpp

Emission.o: Emission.hpp Emission.cpp Parameters.h
	$(CC) $(CCFLAGS) -c Emission.cpp

Mechanics.o: Mechanics.hpp Mechanics.cpp Parameters.h
	$(CC) $(CCFLAGS) -c Mechanics.cpp

Orientation.o: Orientation.hpp Orientation.cpp Parameters.h
	$(CC) $(CCFLAGS) -c Orientation.cpp

Particle.o: Particle.hpp Particle.cpp Parameters.h
	$(CC) $(CCFLAGS) -c Particle.cpp

Signal.o: Signal.hpp Signal.cpp Parameters.h
	$(CC) $(CCFLAGS) -c Signal.cpp

clean:
	/bin/rm -f *.o
