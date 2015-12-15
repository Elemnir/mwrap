CC=gcc
CFLAGS=-fPIC -g
CLDFLAGS=-fPIC -shared
CLIBS=-ldl
CSOURCES=mwmalloc.c bssalloc.c apphooks.c
COBJECTS=$(CSOURCES:.c=.o)
PRELOADLIB=libmwrap.so

CXX=g++
CXXFLAGS= -std=c++11
CXXLDFLAGS=
CXXSOURCES=harness.cpp harnesshooks.cpp
CXXOBJECTS=$(CXXSOURCES:.cpp=.o)
HARNESS=mwrap


all: $(CSOURCES) $(PRELOADLIB) $(HARNESS)

$(PRELOADLIB): $(COBJECTS)
	$(CC) $(CLDFLAGS) -o $(PRELOADLIB) $(COBJECTS) $(CLIBS)

.c.o:
	$(CC) $(CFLAGS) -c $<

$(HARNESS): $(CXXOBJECTS)
	$(CXX) $(CXXLDFLAGS) -o $(HARNESS) $(CXXOBJECTS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o $(HARNESS) $(PRELOADLIB)
