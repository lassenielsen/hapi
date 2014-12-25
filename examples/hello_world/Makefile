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
name = hello_world

CPP = cpp
HAPI = hapi
Copt = -O3
CC = g++

default: build

all: build clean

phony: build clean

build: $(name)

$(name): $(name).cpp
	$(CC) $(Copt) $(name).cpp -o $(name) -lpi

$(name).cpp: $(name).pip
	$(HAPI) -compile -f $(name).pip > $(name).cpp

$(name).pip: $(name).pi
	$(CPP) $(name).pi -I/opt/hapi/include | grep -v "^#" > $(name).pip

clean:
	rm -f $(name).pip
	rm -f $(name).cpp
