COMPILER = gcc
CCFLAGS = -lm
all: c-sim

c-sim: c-sim.o
	$(COMPILER) $(CCFLAGS) -o c-sim c-sim.o
c-sim.o: c-sim.c c-sim.h
	$(COMPILER) $(CCFLAGS) -c c-sim.c
clean:
	rm -f c-sim c-sim.o