# Build the Maxwell Evolve program.

CC = gcc

CCFLAGS = -O -DUNIX

all: ../../bin/Evolve
	@(cd ../base; make)
	@(cd ../morphogens; make)
	@(cd ../util; make)

../../bin/Evolve: Evolve.o ../base/*.o ../morphogens/*.o ../util/*.o
	$(CC) $(CCFLAGS) -o ../../bin/Evolve Evolve.o \
		../base/*.o ../morphogens/*.o ../util/Compound.o \
		../util/Log.o ../util/Random.o ../util/Scope.o \
		../util/ScopeFactory.o ../util/TestGenome.o \
		 -lm -lglut

Evolve.o: Evolve.cpp ../base/Parameters.h ../morphogens/*.hpp
	$(CC) $(CCFLAGS) -c Evolve.cpp

clean:
	@/bin/rm -f *.o

