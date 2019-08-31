# Build the utility objects.

CC = gcc

CCFLAGS = -O -DUNIX

all: Compound.o Log.o Random.o Scope.o \
	ScopeFactory.o TestGenome.o ../../bin/TestBody

Compound.o: Compound.hpp Compound.cpp ../base/Parameters.h
	$(CC) $(CCFLAGS) -c Compound.cpp

Log.o: Log.hpp Log.cpp ../base/Parameters.h
	$(CC) $(CCFLAGS) -c Log.cpp

Random.o: Random.hpp Random.cpp ../base/Parameters.h
	$(CC) $(CCFLAGS) -c Random.cpp

Scope.o: Scope.hpp Scope.cpp ../base/Parameters.h
	$(CC) $(CCFLAGS) -c Scope.cpp

ScopeFactory.o: ScopeFactory.hpp ScopeFactory.cpp ../base/Parameters.h
	$(CC) $(CCFLAGS) -c ScopeFactory.cpp

TestGenome.o: TestGenome.hpp TestGenome.cpp ../base/Parameters.h
	$(CC) $(CCFLAGS) -c TestGenome.cpp

../../bin/TestBody: TestBody.o ../base/*.o ../morphogens/*.o \
	Random.o ScopeFactory.o Scope.o Log.o
	$(CC) $(CCFLAGS) -o ../../bin/TestBody TestBody.o \
		../base/*.o ../morphogens/*.o Random.o \
		ScopeFactory.o Scope.o Log.o -lm -lstdc++

TestBody.o: TestBody.cpp ../base/Parameters.h
	$(CC) $(CCFLAGS) -c TestBody.cpp

clean:
	/bin/rm -f *.o

