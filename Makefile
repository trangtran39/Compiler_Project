# build

# Linux/Mac
CC=gcc
CCFLAGS=-c
CXX=g++
CXXFLAGS=-std=c++14 -c
RM=rm
OBJ=.o
EXE=
OFLAGS=-o 

# Windows
#CC=cl
#CCFLAGS=/c
#CXX=cl
#CXXFLAGS=/std:c++14 /c
#RM=del
#OBJ=.obj
#EXE=.exe
#OFLAGS=/out:

TARGET=cmmc$(EXE)

$(TARGET): shell$(OBJ) messages$(OBJ) buffer$(OBJ) source$(OBJ) token$(OBJ) scanner$(OBJ) parser$(OBJ) error$(OBJ)
	$(CXX) $(OFLAGS)$(TARGET) shell$(OBJ) messages$(OBJ) buffer$(OBJ) source$(OBJ) error$(OBJ) token$(OBJ) scanner$(OBJ) parser$(OBJ)

clean:
	$(RM) *$(OBJ) cmmc

shell$(OBJ): shell.h shell.cpp parser$(OBJ) messages$(OBJ) source$(OBJ)
	$(CXX) $(CXXFLAGS) -w shell.cpp

messages$(OBJ): messages.h messages.c
		$(CC) $(CCFLAGS) messages.c

buffer$(OBJ): buffer.h buffer.cpp messages$(OBJ)
		$(CXX) $(CXXFLAGS) buffer.cpp

source$(OBJ): source.h source.cpp messages$(OBJ) buffer$(OBJ)
	$(CXX) $(CXXFLAGS) source.cpp

token$(OBJ): token.h token.cpp error.h messages$(OBJ)
	$(CXX) $(CXXFLAGS) token.cpp

error$(OBJ): error.h error.cpp
	$(CXX) $(CXXFLAGS) error.cpp

scanner$(OBJ): scanner.h scanner.cpp error.h token$(OBJ) messages$(OBJ) source$(OBJ)
	$(CXX) $(CXXFLAGS) scanner.cpp

parser$(OBJ): parser.h parser.cpp error.h scanner$(OBJ) messages$(OBJ) source$(OBJ)
	$(CXX) $(CXXFLAGS) parser.cpp
