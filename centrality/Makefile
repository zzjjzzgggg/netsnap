#
# Makefile for non-Microsoft compilers
#

## Linux  (uncomment the 2 lines below for compilation on Linux)
CXXFLAGS += -std=c++98 -Wall
LDFLAGS += -lrt

## GLIB and SNAP libary
SNAPDIR = ..
LIB = -I $(SNAPDIR)/glib -I $(SNAPDIR)/snap

## Main application file
#MAIN = centrality
MAIN = groupcentrality
OBJS = Snap.o advcentr.o

all: $(MAIN)

opt: CXXFLAGS += -O4
opt: LDFLAGS += -O4
opt: $(MAIN)

# COMPILE
$(MAIN): $(MAIN).cpp $(OBJS)
	g++ -o $(MAIN) $(OBJS) $(LDFLAGS) $<

advcentr.o: advcentr.cpp advcentr.h
	g++ -c $(CXXFLAGS) $<

Snap.o: 
	g++ -c $(CXXFLAGS) $(LIB) $(SNAPDIR)/snap/Snap.cpp

clean:
	rm -f *.o $(MAIN) $(MAIN).exe $(MAIN).Err
	rm -rf Debug Release
