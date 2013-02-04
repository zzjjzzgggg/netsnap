#
# Small makefile for testing with non-Microsoft compilers
#

CXX = g++
CXXFLAGS += -std=c++98
LDFLAGS += -lrt

MAINOBJS = base.o testBase.o

all: testBase

testBase: $(MAINOBJS)
	$(CXX) -o testBase $(MAINOBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $<
	
clean:
	rm -f *.o *.gch testBase testBase.exe
