#=====================================================================#
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
SHELL=/bin/bash
CPP = cpp
HAPI = hapi
Copt = -O3
CC = g++

OK_PI_FILES = $(shell ls *.pi)
OK_PIP_FILES = $(OK_PI_FILES:.pi=.pip)
#OK_CPP_FILES = $(OK_PI_FILES:.pi=.cpp)
OK_TESTS = $(OK_PI_FILES:.pi=)

ERR_PI_FILES = $(shell ls *.pi_error)
ERR_PIP_FILES = $(ERR_PI_FILES:.pi_error=.pip_error)
ERR_TESTS = $(ERR_PI_FILES:.pi_error=_error)

test: $(OK_TESTS) $(ERR_TESTS)

default: test

all: build autoclean

$(OK_PIP_FILES): %.pip : %.pi
	@echo "Generating $@" ; ($(CPP) $(@:.pip=.pi) -I/opt/hapi/include | grep -v "^#" > $@) ; if [ "$$?" = "0" ]; then echo "OK"; exit 0; else echo "ERROR"; exit 0; fi

$(OK_TESTS): % : %.pip
	@echo "TESTING $@" ; ($(HAPI) -check -f $@.pip >> /dev/null) ; if [ "$$?" = "0" ]; then echo "OK"; exit 0; else echo "ERROR"; exit 0; fi

$(ERR_PIP_FILES): %.pip_error : %.pi_error
	@echo "Generating $@" ; ($(CPP) $(@:.pip_error=.pi_error) -I/opt/hapi/include | grep -v "^#" > $@) ; if [ "$$?" = "0" ]; then echo "OK"; exit 0; else echo "ERROR"; exit 0; fi

$(ERR_TESTS): %_error : %.pip_error
	@echo "TESTING $@" ; ($(HAPI) -check -f $(@:_error=.pip_error) 2>&1 | grep '!!!!!!!!!!!!!!! Type Error: !!!!!!!!!!!!!!!' >> /dev/null) ; if [ "$$?" = "0" ]; then echo "OK"; exit 0; else echo "ERROR"; exit 0; fi

autoclean:
	touch $(OK_PIP_FILES)
	rm -f $(OK_PIP_FILES)
	touch $(ERR_PIP_FILES)
	rm -f $(ERR_PIP_FILES)

clean: autoclean

phony: clean autoclean build $(OK_TESTS) $(ERR_TESTS)
