#include <apims/mpsterm.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace std;
using namespace apims;
int main(int argc, char **argv)
{ try
  { // Config Parameters
    bool cfgPrint=true;
    bool cfgPrintTex=false;
    bool cfgSteps=false;
    bool cfgBuffers=false;
    bool cfgChoices=false;
    bool cfgTypecheck=true;
    bool cfgEval=true;
    bool cfgCompile=false;
    bool cfgOptimizeParallelism=false;
    string term="";
    string dest="";
    int spacewidth=185;

    bool argFault=true;
    for (int i=1; i<argc; ++i) // {{{
    { if ((string)argv[i]=="-texonly")
      { cfgPrintTex=true;
        cfgPrint=false;
        cfgSteps=false;
        cfgBuffers=false;
        cfgChoices=false;
        cfgTypecheck=false;
        cfgEval=false;
        cfgCompile=false;
      }
      else if ((string)argv[i]=="-spacewidth" && i+1<argc)
      { ++i;
        spacewidth=string2int(argv[i]);
      }
      else if ((string)argv[i]=="-steps")
        cfgSteps=true;
      else if ((string)argv[i]=="-buffers")
        cfgBuffers=true;
      else if ((string)argv[i]=="-choices")
        cfgChoices=true;
      else if ((string)argv[i]=="-nocheck")
        cfgTypecheck=false;
      else if ((string)argv[i]=="-noeval")
        cfgEval=false;
      else if ((string)argv[i]=="-eval")
        cfgEval=true;
      else if ((string)argv[i]=="-check")
      { cfgEval=false; cfgTypecheck=true; }
      else if ((string)argv[i]=="-compile" || (string)argv[i]=="-c")
      { cfgCompile=true; cfgEval=false; }
      else if ((string)argv[i]=="-optimize" || (string)argv[i]=="-O")
      { cfgOptimizeParallelism=true; }
      else if ((string)argv[i]=="-f" && i+1<argc)
      { ++i;
        // Read program from file
        ifstream fin(argv[i]);
        if (fin)
        { argFault=false;
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
      else if ((string)argv[i]=="-o" && i+1<argc)
      { ++i;
        dest=argv[i];
      }
      else if (argv[i][0]!='-')
      { argFault=false;
        term=argv[i];
      }
      else
      { argFault=true;
        i=argc;
      }
    } // }}}
    if (argFault) // {{{
    {
      cerr << "Syntax: apims '<term>'" << endl;
      cerr << "    or: apims [-steps] [-buffers] [-choices] [-check] [-nocheck] [-compile] [-texonly] [-spacewidth <width>] [-o <file>] (-f <file.mps>|<program>)" << endl
           << "Options:" << endl
           << " -steps: Print each step of the evaluation for debugging" << endl
           << " -buffers: Print the content of the communication buffers" << endl
           << " -choices: Print the step choices" << endl
           << " -check: Perform typecheck but not evaluation" << endl
           << " -nocheck: Perform evaluation without typechecking" << endl
           << " -compile: Create equivalent C-code" << endl
           << " -o <file>: Save output to <file>" << endl
           << " -f <file>: Read program source from <file>" << endl
           << " <program>: Give the program source as a command line argument" << endl;
      return -1;
    } // }}}

    ostream *out=NULL;
    if (dest=="")
      out= &cout;
    else
      out = new ofstream(dest.c_str());

    if (cfgCompile)
      (*out) << "/* Pre compilation output:" << endl;
    // Parse program
    MpsTerm *current = MpsTerm::Create(term);
    if (cfgPrint)
      (*out) << "******************* Program *******************" << endl
             << current->ToString() << endl;
    if (cfgPrintTex)
      (*out) << current->ToTex(0,spacewidth)<< endl;
    if (cfgTypecheck)
    { // Typecheck program
      (*out) << "************ Type Checking Program ************" << endl;
      try
      { if (not current->TypeCheck())
          return 1;
      }
      catch (string s)
      { (*out) << "ERROR: " << s << endl;
        return 1;
      }
      (*out) << "************ Type Check Succeeded! ************" << endl;
    }
    else
      (*out) << "************** NO TYPE CHECKEING **************" << endl;
    if (cfgOptimizeParallelism)
    { // Optimizing parallelism
      MpsTerm *cOpt = current->Parallelize();
      delete current;
      current=cOpt;
      if (cfgPrint)
        (*out) << "****** Parallelization Optimized Program ******" << endl
               << current->ToString() << endl;
    }
    if (cfgCompile)
      (*out) << "*/" << endl
             << current->MakeC() << endl;
    if (cfgEval)
    { // Apply semantics repeatedly (evaluate)
      MpsTerm *next = current;
      current=NULL;
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
        if (cfgSteps)
          (*out) << "******************* Step: *******************\n"
                 << EnvToString(env) << "\n"
                 << DefEnvToString(defs) << " in\n"
                 << current->ToString() << endl;
        else if (cfgBuffers)
          (*out) << EnvToString(env) << endl;
        // Find next state
        int choices = -1;
        int choice = -1;
        next = current->Step(env,defs,choice,choices);
        if (cfgChoices && choices>0)
          (*out) << "*********** Selected Step: " << choice+1 << " of " << choices << " ***********" << endl;
      }
      (*out) << "******************* Done! *******************" << endl;
    }
    if (dest!="")
      delete out;
    delete current;
  }
  catch (const string &s)
  { cerr << "Error: " << s << endl;
    return -1;
  }
  return 0;
}
