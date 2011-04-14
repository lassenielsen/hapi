#include <apims/mpsterm.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>

using namespace std;
using namespace apims;
int main(int argc, char **argv)
{
  bool argFault=true;
  string term="";
  if (argc==3) // {{{
  {
    string option=argv[1];
    if (option == "-f")
    { // Read program from file
      ifstream fin(argv[2]);
      if (fin)
      {
        argFault=false;
        fin.seekg (0, ios::end);
        int size = fin.tellg();
        char *memblock = new char [size+1];
        fin.seekg (0, ios::beg);
        fin.read (memblock, size);
        fin.close();
        memblock[size]=0;
        term=memblock;
        delete[] memblock;
      }
    }
  } // }}}
  else if (argc==2) // {{{
  {
    term=argv[1];
    argFault=false;
  } // }}}
  if (argFault) // {{{
  {
    cerr << "Syntax: apims_test '<term>'" << endl;
    cerr << "    or: apims_test -f <file.mps>" << endl;
    return -1;
  } // }}}
  // Parse program
  MpsTerm *program = MpsTerm::Create(term);
  cout << "Program: " << endl
       << program->ToString() << endl;
  // Typecheck program
  cout << "************ Type Checking Program ************" << endl;
  if (not program->TypeCheck())
    return 1;
  cout << "************ Type Check Succeeded! ************" << endl;
  delete program;
  return 0;
}
