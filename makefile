CC = g++

#put compiler flags here
CFLAGS = -lglut -lGLU -lGL -lm

#put source files here
SOURCES = mmu.cpp tlb.cpp memory_simulation.cpp mem_unit.cpp guifuncs.cpp rr.cpp prog4.cpp pagereplacefuncs.cpp


#everything else should be automated; just type make

#ps. i don't really know what going on below here 

#computer sorcery
OBJECTS = $(SOURCES:.cpp=.o)
RM = rm -f

all: prog4


#black magic; small animal must be sacrificed or
#this will return an error
prog4: $(OBJECTS)
	g++ -o prog4 $(OBJECTS) $(CFLAGS)
	$(RM) *.o

depend: .depend

#who could possible come up with this
.depend: $(SOURCES)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^>>./.depend;


#it was all an illusion
clean:
	$(RM) *~ .dependprog4

include .depend

#code mostly taken from the wizard at: 
#	stackoverflow.com/questions/2481269/how-to-make-simple-c-makefile
