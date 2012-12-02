// This is a concept test of how to enable integration with the host
// language (the language that APIMS will be compiled to in the source code.
// This is intended to enable usage of the features and libraries provided
// by the host language.
// This example is simple but illustrates the concept fairly well.
// Notice that usage of host language integration is untyped! and even the
// program structure may be compromised by the host language code snippets
// without detection by the tokenizer, parer, typechecker and (first level)
// compilation.

// Add C header
\begin{CHEADER}
#include <iostream> 
using namespace std;
\end{CHEADER}

define $cout =
rec $x.
  1=>2:1
  {^string: 1=>2:1<String>;$x,
   ^int:    1=>2:1<Int>;$x,
   ^bool:   1=>2:1<Bool>;$x,
   ^endl:   $x,
   ^close:  end
  }
in
// public means that its scope is unlimited.
// the default is private, meaning that the channel can only bee used within
// its scope unless it is transmitted to the processes outside the scope.
//
// dynamic (default) means that the channel port is unspecified, and a suitable port can be found at runtime.
// alternatively a fixed channel port can be specified by (channel cout@12345: $cout).
(public dynamic channel cout: $cout)
def Service_cout() =
  def Cout(client:$cout@(2 of 2)) =
  client[1]>>
  {^endl: $x
  {^string:
    client[1]>>s;
    \begin{C}
      cout << \begin{EXP} s \end{EXP};
    \end{C};
    Cout(client),
   ^int:
    client[1]>>i;
    \begin{C}
      char *Str=NULL;
      mpi_get_str(Str,10,\begin{EXP} i \end{EXP}); // Convertes MPI value to char*
      cout << Str;
      delete [] Str; // Clean up mem
    \end{C};
    Cout(client),
   ^bool:
    client[1]>>b;
    \begin{C}
      cout << \begin{EXP}if (b) then "True" else "False"\end{EXP};
    \end{C};
    Cout(client),
   ^close:
    Service_cout()
  }
  in
    link(2,cout,session,2);
    Cout(session)
in Service_cout() |
( // Proccesses that will print a sequnce of strings,
  // and the outputs of the different processes will not be mixed!
  link(2,cout,out,1);
  out[1]<<^string;
  out[1]<<"Hello World 1.1";
  out[1]<<^newline;
  out[1]<<^string;
  out[1]<<"Hello World 1.2";
  out[1]<<^newline;
  out[1]<<^string;
  out[1]<<"Hello World 1.3";
  out[1]<<^newline;
  out[1]<<^close;
| link(2,cout,out,1);
  out[1]<<^string;
  out[1]<<"Hello World 2.1";
  out[1]<<^newline;
  out[1]<<^string;
  out[1]<<"Hello World 2.2";
  out[1]<<^newline;
  out[1]<<^string;
  out[1]<<"Hello World 2.3";
  out[1]<<^newline;
  out[1]<<^close;
| // Using another language extension (syntactic sugar):
  link(2,cout,out,1);
  out[1]<< ^string << "Hello World 3.1" << ^newline;
  out[1]<< ^string << "Hello World 3.2" << ^newline;
  out[1]<< ^string << "Hello World 3.3" << ^newline;
  out[1]<< ^close;
)
