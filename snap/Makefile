#
# Small makefile for testing with non-Microsoft compilers
#

## Linux (uncomment if using linux)
CXXFLAGS += -Wall
LDFLAGS += -lrt

all: testSnap

testSnap: testSnap.cpp Snap.o 
	g++ -o testSnap testSnap.cpp Snap.o -I../GLib $(LDFLAGS)

Snap.o:
	g++ -c $(CXXFLAGS) Snap.cpp -I../GLib

clean:
	rm -f *.o testSnap testSnap.exe *.plt *.tab *.png
