CC=gcc
CFLAGS=-fPIC
CLDFLAGS=-fPIC -shared
CSOURCES=mwmalloc.c bssalloc.c hooks.c
COBJECTS=$(CSOURCES:.c=.o)
PRELOADLIB=libmwrap.so

CXX=g++
CXXFLAGS=
CXXLDFLAGS=
CXXSOURCES=harness.cpp
CXXOBJECTS=$(CXXSOURCES:.cpp=.o)
HARNESS=mwrap


all: $(CSOURCES) $(PRELOADLIB) $(HARNESS)

$(PRELOADLIB): $(COBJECTS)
	$(CC) $(CLDFLAGS) -o $(PRELOADLIB) $(COBJECTS)

.c.o:
	$(CC) $(CFLAGS) -c $<

$(HARNESS): $(CXXOBJECTS)
	$(CXX) $(CXXLDFLAGS) -o $(HARNESS) $(CXXOBJECTS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o $(HARNESS) $(PRELOADLIB)
