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
    cerr << "Syntax: apims '<term>'" << endl;
    cerr << "    or: apims -f <file.mps>" << endl;
    return -1;
  } // }}}
  // Parse program
  MpsTerm *next = MpsTerm::Create(term);
  cout << "Program: " << endl << next->ToString() << endl;
  MpsTerm *current = NULL;
  // Typecheck program
  cout << "************ Type Checking Program ************" << endl;
  if (not next->TypeCheck())
    return 1;
  cout << "************ Type Check Succeeded! ************" << endl;
  // Apply semantics repeatedly (evaluate)
  MpsEnv env;
  vector<MpsFunction> defs;
  srand((unsigned)time(0));
  while (next != NULL) // While it can step, choose random step
  {
    if (current != NULL) // Clean up old state
      delete current;
    // Move to next state
    current = next->Simplify(); // Goto next state (and remove dead processes)
    delete next;
    next = NULL;
    // Print state
    cout << "******************* Step: *******************\n"
         << Env2string(env) << "\n"
         << DefEnv2string(defs) << " in\n"
         << current->ToString()
         << endl;
    // Find next state
    int choices = -1;
    int choice = -1;
    next = current->Step(env,defs,choice,choices);
    if (choices>0)
      cout << "*********** Selected Step: " << choice+1 << " of " << choices << " ***********" << endl;
  }
  cout << "******************* Done! *******************" << endl;
  delete current;
}
