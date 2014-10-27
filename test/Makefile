#
# Makefile for non-Microsoft compilers
#

## Linux  (uncomment the 2 lines below for compilation on Linux)
CXXFLAGS += -Wall -std=c++11
LDFLAGS += -lrt -pthread -std=c++11

## Main application file
MAIN = test

all: $(MAIN)

#opt: CXXFLAGS += -O4
#opt: LDFLAGS += -O4
#opt: $(MAIN)

# COMPILE
$(MAIN): $(MAIN).cpp Snap.o
	g++ -o $(MAIN) $(MAIN).cpp Snap.o $(LDFLAGS)

Snap.o: 
	g++ -c $(CXXFLAGS) ../snap/Snap.cpp -I../glib -I../snap

clean:
	rm -f *.o $(MAIN) $(MAIN).exe $(MAIN).Err *.gz
	rm -rf Debug Release
