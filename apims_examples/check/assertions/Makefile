#=====================================================================#
#                      APIMS PROGRAM MAKEFILE                         #
# This is the Makefile for an APIMS program.                          #
# Simply update the name variable, to adapt this makefile to another  #
# APIMS project.                                                      #
#                                                                     #
# "make clean" will remove temporary files (.pip and .cpp)            #
# "make" or "make build" will build the application executable        #
#                                                                     #
# Enjoy!                                                              #
# - Lasse Nielsen.                                                    #
#=====================================================================#
CPP = cpp
APIMS = apims
Copt = -O3
CC = g++

PI_FILES = $(shell ls *.pi)
PIP_FILES = $(PI_FILES:.pi=.pip)
CPP_FILES = $(PI_FILES:.pi=.cpp)
EXE_FILES = $(PI_FILES:.pi=)

build: $(EXE_FILES)

default: build

all: build autoclean

phony: build clean all autoclean

%.pip: %.pi
	$(CPP) $(@:.pip=.pi) -I/opt/apims/include | grep -v "^#" > $@

%.cpp: %.pip
	$(APIMS) -compile -f $(@:.cpp=.pip) > $@

%: %.cpp
	$(CC) $(Copt) $@.cpp -o $@ -lpi

auto clean:
	touch $(PIP_FILES)
	rm -f $(PIP_FILES)
	touch $(CPP_FILES)
	rm -f $(CPP_FILES)

clean:
	touch $(EXE_FILES)
	rm -f $(EXE_FILES)
