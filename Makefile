CXX=g++
CXXFLAGS=-g -Wall -std=c++11 -I /usr/include/gtest/
DEFS=-DUSE_SOL_CLASSNAME
GTEST_LD_FLAGS=-lgtest -lgtest_main -lpthread
TARGETS=stream test-content-display
OBJ = content.o user.o strmsrv.o jsonparser.o customparser.o

all: $(TARGETS)


test-content-display: test-content-display.cpp content.o user.o
	$(CXX) $(CXXFLAGS) $(DEFS) $^ -o $@ 
	
stream: stream.cpp $(OBJ)
	$(CXX) $(CXXFLAGS) $(DEFS) $^ -o $@ 

content.o: content.cpp content.h 
	$(CXX) $(CXXFLAGS) $(DEFS) -c $< -o $@

user.o: user.cpp user.h 
	$(CXX) $(CXXFLAGS) $(DEFS) -c $< -o $@

jsonparser.o: jsonparser.cpp jsonparser.h parser.h content.h user.h 
	$(CXX) $(CXXFLAGS) $(DEFS) -c $< -o $@

customparser.o: customparser.cpp customparser.h parser.h content.h user.h 
	$(CXX) $(CXXFLAGS) $(DEFS) -c $< -o $@

strmsrv.o: strmsrv.cpp strmsrv.h parser.h content.h user.h 
	$(CXX) $(CXXFLAGS) $(DEFS) -c $< -o $@



clean:
	rm -rf *.o $(TARGETS)
