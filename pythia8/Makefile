PROGRAM  =  DIS_18275
ROOTLIBS = $(shell root-config --libs)
CXX      =  g++
CXXFLAGS =  -m64 -fno-inline -O  -W -Wall -pthread -std=c++11  -Wno-deprecated-declarations 
CPPFLAGS = -I${EICDIRECTORY}/include -I$(ROOTSYS)/include
LDFLAGS  = -L${EICDIRECTORY}/lib -Wl,-rpath,${EICDIRECTORY}/lib -lpythia8 -ldl $(ROOTLIBS) -L${EICDIRECTORY}/lib64 -lHepMC3

$(PROGRAM):	$(PROGRAM).cc Makefile
		$(CXX) $(CXXFLAGS)  $(PROGRAM).cc $(CPPFLAGS) $(LDFLAGS) -o $(PROGRAM) 

