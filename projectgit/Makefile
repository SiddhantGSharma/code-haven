EXENAME = finalproj
OBJS = main.o Dijkstra.o Parser.o  Graph.o

CXX = clang++
CXXFLAGS = $(CS225) -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic
LD = clang++
LDFLAGS = -std=c++1y -stdlib=libc++ -lc++abi -lm

# Custom Clang version enforcement Makefile rule:
ccred=$(shell echo -e "\033[0;31m")
ccyellow=$(shell echo -e "\033[0;33m")
ccend=$(shell echo -e "\033[0m")

IS_EWS=$(shell hostname | grep "ews.illinois.edu")
IS_CORRECT_CLANG=$(shell clang -v 2>&1 | grep "version 6")
ifneq ($(strip $(IS_EWS)),)
ifeq ($(strip $(IS_CORRECT_CLANG)),)
CLANG_VERSION_MSG = $(error $(ccred) On EWS, please run 'module load llvm/6.0.1' first when running CS225 assignments. $(ccend))
endif
else
CLANG_VERSION_MSG = $(warning $(ccyellow) Looks like you are not on EWS. Be sure to test on EWS before the deadline. $(ccend))
endif

.PHONY: all test clean output_msg

all : $(EXENAME)

output_msg: ; $(CLANG_VERSION_MSG)

$(EXENAME): output_msg $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXENAME)

Dijkstra.o: algorithms/Dijkstra.h algorithms/Dijkstra.cpp
	$(CXX) $(CXXFLAGS) algorithms/Dijkstra.cpp

Parser.o: Parser.h Parser.cpp 
	$(CXX) $(CXXFLAGS) Parser.cpp

Graph.o: Graph.h Graph.cpp 
	$(CXX) $(CXXFLAGS) Graph.cpp

main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp

test: output_msg catchmain.o test.o Parser.o Graph.o Dijkstra.o
	$(LD) catchmain.o test.o Parser.o Graph.o Dijkstra.o $(LDFLAGS) -o test

catchmain.o : tests/catchmain.cpp tests/catch.hpp
	$(CXX) $(CXXFLAGS) tests/catchmain.cpp

test.o : tests/test.cpp tests/catch.hpp
	$(CXX) $(CXXFLAGS) tests/test.cpp

clean:
	-rm -f *.o $(EXENAME) test