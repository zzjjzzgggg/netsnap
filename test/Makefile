#
# Makefile for non-Microsoft compilers
#

## Linux  (uncomment the 2 lines below for compilation on Linux)
CXXFLAGS += -Wall -std=c++11
LDFLAGS += -lrt -pthread -std=c++11
OBJ = Snap.o  argsparser.o

## Main application file
MAIN = test

all: $(MAIN)

#opt: CXXFLAGS += -O4
#opt: LDFLAGS += -O4
#opt: $(MAIN)

# COMPILE
thread: test_thread_pool.cpp
	g++ -o test_thread $(LDFLAGS) $<

$(MAIN): $(MAIN).cpp $(OBJ)
	g++ -o $(MAIN) $(MAIN).cpp $(OBJ) $(LDFLAGS)

argsparser.o: argsparser.cpp argsparser.h
	g++ -c $(CXXFLAGS) $<

Snap.o:
	g++ -c $(CXXFLAGS) ../snap/Snap.cpp -I../glib -I../snap

clean:
	rm -f *.o $(MAIN) $(MAIN).exe $(MAIN).Err *.gz
	rm -rf Debug Release
