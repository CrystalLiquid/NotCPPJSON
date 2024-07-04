# Project: CPPJSON
# Makefile created by Red Panda C++ 2.26

CPP      = g++
CC       = gcc
OBJ      = tmp_obj/main.o
LINKOBJ  = tmp_obj/main.o
CLEANOBJ  =  tmp_obj/main.o release/CPPJSON
LIBS     =  -s -static
INCS     =  
CXXINCS  =  
BIN      = release/CPPJSON
CXXFLAGS = $(CXXINCS)  -Wfatal-errors -pedantic -O3 -std=c++2b -pipe
CFLAGS   = $(INCS)  -std=c17 -Wfatal-errors -pedantic -O3 -pipe
RM       = rm -rf

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom
	-$(RM) $(CLEANOBJ) > /dev/null 2>&1

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) $(LIBS)

tmp_obj/main.o: main.cpp JSON_IO.hpp JSON_Parse.hpp JSON_ACC_Pool_Str.hpp JSON_Serialize.hpp JSON_Base.h
	$(CPP) -c main.cpp -o tmp_obj/main.o $(CXXFLAGS) 
