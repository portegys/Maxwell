# Build the morphogen objects.

CC = gcc

CCFLAGS = -O -DUNIX

all: BondMorph.o CreateMorph.o Gene.o \
	Genome.o GrappleMorph.o Maxwell.o \
	Morphogen.o OrientMorph.o PropelMorph.o \
	TypeMorph.o

BondMorph.o: BondMorph.hpp BondMorph.cpp ../base/Parameters.h
	$(CC) $(CCFLAGS) -c BondMorph.cpp

CreateMorph.o: CreateMorph.hpp CreateMorph.cpp ../base/Parameters.h
	$(CC) $(CCFLAGS) -c CreateMorph.cpp

Gene.o: Gene.hpp Gene.cpp ../base/Parameters.h
	$(CC) $(CCFLAGS) -c Gene.cpp

Genome.o: Genome.hpp Genome.cpp ../base/Parameters.h
	$(CC) $(CCFLAGS) -c Genome.cpp

GrappleMorph.o: GrappleMorph.hpp GrappleMorph.cpp ../base/Parameters.h
	$(CC) $(CCFLAGS) -c GrappleMorph.cpp

Maxwell.o: Maxwell.hpp Maxwell.cpp ../base/Parameters.h
	$(CC) $(CCFLAGS) -c Maxwell.cpp

Morphogen.o: Morphogen.hpp Morphogen.cpp ../base/Parameters.h
	$(CC) $(CCFLAGS) -c Morphogen.cpp

OrientMorph.o: OrientMorph.hpp OrientMorph.cpp ../base/Parameters.h
	$(CC) $(CCFLAGS) -c OrientMorph.cpp

PropelMorph.o: PropelMorph.hpp PropelMorph.cpp ../base/Parameters.h
	$(CC) $(CCFLAGS) -c PropelMorph.cpp

TypeMorph.o: TypeMorph.hpp TypeMorph.cpp ../base/Parameters.h
	$(CC) $(CCFLAGS) -c TypeMorph.cpp

clean:
	/bin/rm -f *.o

