CXXFLAGS += -Wall -std=c++11
LDFLAGS += -lrt -std=c++11

## Main application file
MAIN = test

all: $(MAIN)


# COMPILE
$(MAIN): $(MAIN).cpp
	g++ -o $(MAIN) $(MAIN).cpp $(LDFLAGS)


clean:
	rm -f *.o $(MAIN) *.Err *.gz
	rm -rf Debug Release
