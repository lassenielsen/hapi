#====================================================================#
#                      HAPI PROGRAM MAKEFILE                         #
# This is the Makefile for an HAPI program.                          #
# Simply update the name variable, to adapt this makefile to another  #
# HAPI project.                                                      #
#                                                                     #
# "make clean" will remove temporary files (.pip and .cpp)            #
# "make" or "make build" will build the application executable        #
#                                                                     #
# Enjoy!                                                              #
# - Lasse Nielsen.                                                    #
#=====================================================================#
SHELL = /bin/bash
CPP = cpp
HAPI = hapi
Copt = -O3
CC = g++

PI_FILES = $(shell ls *.pi)
PIP_FILES = $(PI_FILES:.pi=.pip)
RUN_FILES = $(PI_FILES:.pi=_run)
DBG_FILES = $(PI_FILES:.pi=_debug)

build: $(OK_EXE_FILES)

default: build

all: build autoclean

$(PIP_FILES): %.pip : %.pi
	($(CPP) $(@:.pip=.pi) -I/opt/hapi/include | grep -v "^#" > $@) ; if [ "$$?" = "0" ]; then echo "OK"; exit 0; else echo "ERROR"; exit 0; fi

$(RUN_FILES): %_run : %.pip
	$(HAPI) -f $(@:_run=.pip)

$(DBG_FILES): %_debug : %.pip
	$(HAPI) -steps -buffers -f $(@:_debug=.pip)

autoclean:
	touch $(PIP_FILES)
	rm -f $(PIP_FILES)

clean: autoclean

phony: clean autoclean build $(RUN_FILES)
