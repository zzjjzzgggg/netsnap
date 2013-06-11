#
# Makefile for non-Microsoft compilers
#

## Linux  (uncomment the 2 lines below for compilation on Linux)
CXXFLAGS += -std=c++98 -Wall
LDFLAGS += -lrt

## CygWin (uncomment the 2 lines below for compilation on CygWin)
#CXXFLAGS += -Wall
#LDFLAGS += 

SNAPDIR = ..
LIB = -I $(SNAPDIR)/glib -I $(SNAPDIR)/snap

## Main application file
MAIN = graphstat

all: $(MAIN)

opt: CXXFLAGS += -O4
opt: LDFLAGS += -O4
opt: $(MAIN)

# COMPILE
$(MAIN): $(MAIN).cpp  $(MAIN).h Snap.o 
	g++ -o $(MAIN) $(MAIN).cpp Snap.o $(LDFLAGS)
	
Snap.o: 
	g++ -c $(CXXFLAGS) $(LIB) $(SNAPDIR)/snap/Snap.cpp

clean:
	rm -f *.o  $(MAIN)  $(MAIN).exe
	rm -rf Debug Release

