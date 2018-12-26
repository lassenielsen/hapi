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
Copt = -O3 -std=c++11
CC = g++

OK_PI_FILES = $(shell ls *.pi)
OK_PIP_FILES = $(OK_PI_FILES:.pi=.pip)
OK_CPP_FILES = $(OK_PI_FILES:.pi=.cpp)
OK_EXE_FILES = $(OK_PI_FILES:.pi=)
OK_DBG_FILES = $(OK_PI_FILES:.pi=.dbg)
OK_MON_FILES = $(OK_PI_FILES:.pi=.mon)
OK_PFL_FILES = $(OK_PI_FILES:.pi=.pfl)
OK_DOT_FILES = $(OK_PI_FILES:.pi=.dot)
OK_EPS_FILES = $(OK_PI_FILES:.pi=.eps)

build: $(OK_EXE_FILES)

default: build

all: build autoclean

$(OK_PIP_FILES): %.pip : %.pi
	@echo "=== Preprocessing $(@:.pip=.pi) to $@"
	@($(CPP) $(@:.pip=.pi) -I/opt/hapi/include | grep -v "^#" > $@) ; echo "OK"; exit 0;

$(OK_CPP_FILES): %.cpp : %.pip
	@echo "=== Compiling $(@:.cpp=.pip) to $@"
	@($(HAPI) -compile -optimize -f $(@:.cpp=.pip) > $@) ; if [ "$$?" = "0" ]; then echo "OK"; exit 0; else echo "ERROR"; exit 0; fi

$(OK_EXE_FILES): % : %.cpp
	@echo "=== Building executable from C++ code in $@.cpp"
	@($(CC) $(Copt) $@.cpp -o $@ -lpi -lpthread -lgmp) ; if [ "$$?" = "0" ]; then echo "OK"; exit 0; else echo "ERROR"; exit 0; fi

$(OK_DBG_FILES): %.dbg : %.cpp
	@echo "=== Building debug executable from C++ code in $(@:.dbg=.cpp)"
	@($(CC) $(Copt) $(@:.dbg=.cpp) -o $@ -pg -lpi_debug -lpthread -lgmp) ; if [ "$$?" = "0" ]; then echo "OK"; exit 0; else echo "ERROR"; exit 0; fi

$(OK_MON_FILES): %.mon : %.dbg
	@echo "=== Starting debug executable $(@:.mon=.dbg)"
	@./$(@:.mon=.dbg) ; if [ "$$?" = "0" ]; then echo "OK"; exit 0; else echo "ERROR"; exit 0; fi
	mv gmon.out $@

$(OK_PFL_FILES): %.pfl : %.mon %.dbg
	@echo "=== Creating profile output from monitor file $(@:.pfl=.mon)"
	@gprof ./$(@:.pfl=.dbg) $(@:.pfl=.mon) > $@ ; if [ "$$?" = "0" ]; then echo "OK"; exit 0; else echo "ERROR"; exit 0; fi

$(OK_DOT_FILES): %.dot : %.pfl
	@echo "=== Creating dot diagram from frofile results in file $(@:.dot=.pfl)"
	@gprof2dot ./$(@:.dot=.pfl) > $@ ; if [ "$$?" = "0" ]; then echo "OK"; exit 0; else echo "ERROR"; exit 0; fi

$(OK_EPS_FILES): %.eps : %.dot
	@echo "=== Creating eps figure from dot diagram in file $(@:.eps=.dot)"
	@dot -Teps ./$(@:.eps=.dot) > $@ ; if [ "$$?" = "0" ]; then echo "OK"; exit 0; else echo "ERROR"; exit 0; fi

autoclean:
	touch $(OK_PIP_FILES)
	rm -f $(OK_PIP_FILES)
	touch $(OK_CPP_FILES)
	rm -f $(OK_CPP_FILES)
	touch $(OK_MON_FILES)
	rm -f $(OK_MON_FILES)
	touch $(OK_PFL_FILES)
	rm -f $(OK_PFL_FILES)
	touch $(OK_DOT_FILES)
	rm -f $(OK_DOT_FILES)

clean: autoclean
	touch $(OK_EXE_FILES)
	rm -f $(OK_EXE_FILES)
	touch $(OK_DBG_FILES)
	rm -f $(OK_DBG_FILES)
	touch $(OK_EPS_FILES)
	rm -f $(OK_EPS_FILES)

phony: clean autoclean build
