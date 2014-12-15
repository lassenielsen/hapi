#====================================================================#
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
SHELL = /bin/bash
CPP = cpp
APIMS = apims
Copt = -O3
CC = g++

OK_PI_FILES = $(shell ls *.pi)
OK_PIP_FILES = $(OK_PI_FILES:.pi=.pip)
OK_CPP_FILES = $(OK_PI_FILES:.pi=.cpp)
OK_EXE_FILES = $(OK_PI_FILES:.pi=)

build: $(OK_EXE_FILES)

default: build

all: build autoclean

$(OK_PIP_FILES): %.pip : %.pi
	($(CPP) $(@:.pip=.pi) -I/opt/apims/include | grep -v "^#" > $@) ; if [ "$$?" = "0" ]; then echo "OK"; exit 0; else echo "ERROR"; exit 0; fi

$(OK_CPP_FILES): %.cpp : %.pip
	($(APIMS) -compile -f $(@:.cpp=.pip) > $@) ; if [ "$$?" = "0" ]; then echo "OK"; exit 0; else echo "ERROR"; exit 0; fi

$(OK_EXE_FILES): % : %.cpp
	($(CC) $(Copt) $@.cpp -o $@ -lpi) ; if [ "$$?" = "0" ]; then echo "OK"; exit 0; else echo "ERROR"; exit 0; fi

autoclean:
	touch $(OK_PIP_FILES)
	rm -f $(OK_PIP_FILES)
	touch $(OK_CPP_FILES)
	rm -f $(OK_CPP_FILES)

clean: autoclean
	touch $(OK_EXE_FILES)
	rm -f $(OK_EXE_FILES)

phony: clean autoclean build
