#
# Makefile for non-Microsoft compilers
#

## Linux  (uncomment the 2 lines below for compilation on Linux)
CXXFLAGS += -Wall -std=c++11
LDFLAGS += -lrt -pthread -std=c++11

## CygWin (uncomment the 2 lines below for compilation on CygWin)
#CXXFLAGS += -Wall
#LDFLAGS +=

SNAPDIR = /home/jzzhao/git_project/netsnap
LIB = -I$(SNAPDIR)

## Main application file
MAIN = graphstat

all: $(MAIN)

opt: CXXFLAGS += -O4
opt: LDFLAGS += -O4
opt: $(MAIN)

# COMPILE
$(MAIN): $(MAIN).cpp $(MAIN).h Snap.o
	clang++ -o $(MAIN) $(MAIN).cpp Snap.o $(LIB) $(LDFLAGS)

Snap.o:
	clang++ -c $(CXXFLAGS) $(LIB) $(SNAPDIR)/snap/Snap.cpp

clean:
	rm -f *.o $(MAIN) $(MAIN).exe $(MAIN).Err
	rm -rf Debug Release
