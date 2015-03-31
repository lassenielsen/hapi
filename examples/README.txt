HAPI EXAMPLE PROGRAMS

This is a short introduction to the example hapi programs that are available in this folder.
There are a number of sub-folders, and the content of each will be briefly described below.

The hello_world folder holds an example "Hello World" project, including source
code and makefile. Assuming hapi has been correctly installed, enter this
folder and type "make build" to compile the program to an executable.
This example project is intended as a starting platform for making your own
projects - simply copy the folder, rename the .pi file to the desired project
name, update the Makefile 'name' variable to the desired, and start
programming.

The tutorial folder holds programs that shows the capabilities and syntax of
hapi. This is intended as a way to learn the language for people who are used
to programming in other languages.
Each file <filename>.pi files in this folder can be compiled using the command
"make <filename>".

The compile folder holds programs that has been used to test the hapi
compiler, but can also be used to learn the syntax of hapi.
Each file <filename>.pi files in this folder can be compiled using the command
"make <filename>".

The check folder holds test progrqams used to unit-test the type-checking.
There are two types of tests in the subdirectories in this folder.
The files <filename>.pi are programs meant to sucessfully typecheck.
The tests can be performed using the command "make <filename>", and the output
should print OK two times.
The files <filename>.pi_error are programs meant to fail during typecheck.
The tests can be performed using the command "make <filename>_error", and the output
should print OK two times.

Finally, the interpret folder holds a lot of programs, using the
synchronization features, which are currently not supported by the compilation,
but only by the less efficient interpretation. Further many of these programs
use obsolete syntax and thus need to be adjusted in order to be parsed by the
latest version of hapi.

Enjoy
 - Lasse Nielsen
