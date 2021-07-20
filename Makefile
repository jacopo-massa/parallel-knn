CXX	= g++ -std=c++17
INCLUDES	=
CXXFLAGS	= -g

LDFLAGS	= -pthread
OPTFLAGS	= -O3 -finline-functions

OBJECTS	= utils/point.o utils/util.o utils/heap.o

TARGETS	= seq stl ff

.PHONY: all clean cleanall
.SUFFIXES: .cpp

all	: $(TARGETS)

seq: $(OBJECTS) seq.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $(OBJECTS) seq.o

stl: $(OBJECTS) stl.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $(OBJECTS) stl.o $(LDFLAGS)

ff: $(OBJECTS) ff.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $(OBJECTS) ff.o $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ -c $< $(LDFLAGS)

clean:
	rm -f $(TARGETS)

cleanall:	clean
	rm -f utils/*.o
	rm -f *.txt *~
	rm -f *.o *~