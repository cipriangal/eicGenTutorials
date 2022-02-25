.DELETE_ON_ERROR:

ROOTCONFIG  := root-config
ROOTCFLAGS  := $(shell $(ROOTCONFIG) --cflags)
ROOTLDFLAGS := $(shell $(ROOTCONFIG) --ldflags)
ROOTGLIBS   := $(shell $(ROOTCONFIG) --glibs)
ROOTINCDIR  := $(shell $(ROOTCONFIG) --incdir)

CXX         := g++

INCLUDES    := -I${EICDIRECTORY}/include -I$(ROOTINCDIR)
CXXFLAGS    := $(ROOTCFLAGS) $(INCLUDES)

LD          := g++
LDFLAGS     := -O2 $(ROOTLDFLAGS)

LIBS        :=  -L${EICDIRECTORY}/lib -Wl,-rpath,${EICDIRECTORY}/lib -ldl \
		$(ROOTGLIBS)                                              \
	       -L${EICDIRECTORY}/lib64 -lHepMC3
SRC_FILES   := $(wildcard *.cxx)
EXE_FILES   := $(SRC_FILES:%.cxx=%) 

.PHONY: all clean

all: $(EXE_FILES)

%: %.o
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(EXE_FILES:%=%.o) $(EXE_FILES)

