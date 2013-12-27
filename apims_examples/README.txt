APIMS EXAMPLE PROGRAMS

This is a short introduction to the example apims programs that are available in this folder.
There are a number of sub-folders, and the content of each will be briefly described below.

The hello_world folder holds an example "Hello World" project, including source
code and makefile. Assuming APIMS has been correctly installed, enter this
folder and type "make build" to compile the program to an executable.
This example project is intended as a starting platform for making yout own
projects - simply copy the folder, rename the .pi file to the desired project
name, update the Makefile 'name' variable to the desired, and start
programming.

The compile folder holds programs that has been used to test the apims
compiler, but can also be used to learn the syntax of APIMS.
The .pi files in this folder can be compiled using the command
"apims-compile <file>.pi".

The check folder holds test progrqams used to test the type-checking.

Finally, the interpret folder holds a lot of programs, using the
synchronization features, which are currently not supported by the compilation,
but only by the less efficient interpretation. Further many of these programs
use obsolete syntax and thus need to be adjusted in order to be parsed by the
latest version of APIMS.

Enjoy
 - Lasse Nielsen
