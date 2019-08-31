# Make Maxwell

all:
	@echo "Making base..."
	@(cd src/base; make)
	@echo "Making morphogens..."
	@(cd src/morphogens; make)
	@echo "Making util..."
	@(cd src/util; make)
	@echo "Making evolve..."
	@(cd src/evolve; make)
	@echo "done"

zip:
	@echo "Creating maxwell.zip file..."
	@/bin/ls -d src/*/*.h src/*/*.hpp src/*/*.cpp makefile \
		src/*/makefile bin work work/*.txt | zip maxwell -@
	@echo "done"

clean:
	(cd src/base; make clean)
	(cd src/morphogens; make clean)
	(cd src/util; make clean)
	(cd src/evolve; make clean)
