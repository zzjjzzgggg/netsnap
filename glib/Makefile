#
# Small makefile for testing with non-Microsoft compilers
#

CXXFLAGS += -Wall
LDFLAGS += -lrt

MAINOBJS = base.o testBase.o

all: testBase

testBase: $(MAINOBJS)
	g++ -o testBase $(MAINOBJS) $(LDFLAGS)

%.o: %.cpp
	g++ -c $(CXXFLAGS) $<
	
clean:
	rm -f *.o *.gch testBase testBase.exe
