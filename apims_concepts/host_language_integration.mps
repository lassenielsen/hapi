// This is a concept test of how to enable integration with the host
// language (the language that APIMS will be compiled to in the source code.
// This is intended to enable usage of the features and libraries provided
// by the host language.
// This example is simple but illustrates the concept fairly well.
// Notice that usage of host language integration is untyped! and even the
// program structure may be compromised by the host language code snippets
// without detection by the tokenizer, parer, typechecker and (first level)
// compilation.
\CHEADER{{
#include <iostream> 
using namespace std;
}} // Add C header
define $cout =
rec $x.
  1=>2:1
  {^print_string: 1=>2:1<String>;$x,
   ^print_int:    1=>2:1<String>;$x,
   ^print_bool:   1=>2:1<String>;$x,
   ^close: end
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
  {^print_newline: $x
  {^print_string:
    client[1]>>s;
    \C{{ cout << \EXP{{s}} }};
    Cout(client),
   ^print_int:
    client[1]>>i;
    \C{{ cout << \EXP{{i}} }}; // fixme: needs printing of multi precission value
    Cout(client),
   ^print_bool:
    client[1]>>b;
    \C{{ cout << \EXP{{if (b) then "True" else "False"}} }};
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
  out[1]<<^print_string;
  out[1]<<"Hello World 1.1";
  out[1]<<^print_newline;
  out[1]<<^print_string;
  out[1]<<"Hello World 1.2";
  out[1]<<^print_newline;
  out[1]<<^print_string;
  out[1]<<"Hello World 1.3";
  out[1]<<^print_newline;
  out[1]<<^close;
| link(2,cout,out,1);
  out[1]<<^print_string;
  out[1]<<"Hello World 2.1";
  out[1]<<^print_newline;
  out[1]<<^print_string;
  out[1]<<"Hello World 2.2";
  out[1]<<^print_newline;
  out[1]<<^print_string;
  out[1]<<"Hello World 2.3";
  out[1]<<^print_newline;
  out[1]<<^close;
| link(2,cout,out,1);
  out[1]<<^print_string;
  out[1]<<"Hello World 3.1";
  out[1]<<^print_newline;
  out[1]<<^print_string;
  out[1]<<"Hello World 3.2";
  out[1]<<^print_newline;
  out[1]<<^print_string;
  out[1]<<"Hello World 3.3";
  out[1]<<^print_newline;
  out[1]<<^close;
)
