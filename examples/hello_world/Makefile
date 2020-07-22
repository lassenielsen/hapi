#=====================================================================#
#                      HAPI PROGRAM MAKEFILE                          #
# This is the Makefile for a HAPI program.                            #
# Simply update the name variable, to adapt this makefile to another  #
# HAPI project.                                                       #
#                                                                     #
# "make autoclean" will remove temporary files (.pip and .cpp)        #
# "make clean" will remove the executable as well.                    #
# "make" or "make build" will build the application executable        #
#                                                                     #
# Enjoy!                                                              #
# - Lasse Nielsen.                                                    #
#=====================================================================#
name = hello_world

CPP = cpp
HAPI = hapi
Copt = -O3
Cdbg = -g3
CC = g++ -std=c++11

default: build

all: build autoclean

phony: build clean autoclean

build: $(name)

$(name): $(name).cpp
	@echo "=== Building executable from C++ code in $(name).cpp"
	$(CC) $(Copt) $(name).cpp -o $(name) -lpi -lpthread

$(name)_debug: $(name).cpp
	@echo "=== Building executable from C++ code in $(name).cpp"
	$(CC) $(Cdbg) $(name).cpp -o $(name)_debug -lpi_debug -lpthread

$(name).cpp: $(name).pip
	@echo "=== Compiling $(name).pip to $(name).cpp"
	$(HAPI) -compile -optimize -f $(name).pip > $(name).cpp

$(name).pip: $(name).pi
	@echo "=== Preprocessing $(name).pi to $(name).pip"
	$(CPP) $(name).pi -I/opt/hapi/include | grep -v "^#" > $(name).pip

autoclean:
	@rm -f $(name).pip
	@rm -f $(name).cpp

clean: autoclean
	@rm -f $(name)
