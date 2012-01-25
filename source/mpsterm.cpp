#define MPSUSEGUI
#include <dpl/parser.hpp>
#include <apims/mpsterm.hpp>
#include <apims/mpsgui.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <assert.h>

#include "common.cpp"

using namespace std;
using namespace apims;
using namespace dpl;

int MpsTerm::ourNextId = 1;

/* MpsTerm and subclasses implementation
 */
string MpsTerm::NewName(string base) // {{{
{
  string result = (string)"~"+base;
  result += int2string(ourNextId++);
  return result;
} // }}}
MpsTerm *MpsTerm::Error(const string &msg) // {{{
{
  string name=(string)"_ERROR: " + msg;
  vector<MpsExp*> arg;
  return new MpsCall(name, arg, arg);
} // }}}

/* Parsing of MpsTerms from a string
 */
// OPTIONAL: Add xml-expressions
// FIXME: Implement rand
// FIXME: Add expression operations (<,>,<=,>=,s1++s2,head,tail) [+,-,*,/,and,or,not,=]
MpsTerm *MpsTerm::Create(const std::string &exp) // {{{
{
  // Create parser
  Parser MpsParser;
  
  /*** Define Tokens ***/
  // Parenthesis
  MpsParser.DefKeywordToken("(",2);
  MpsParser.DefKeywordToken(")",2);
  // Base expression keywords
  MpsParser.DefKeywordToken("true",0); // Base Value: boolean true
  MpsParser.DefKeywordToken("false",0); // Base Value: boolean false
  MpsParser.DefKeywordToken("and",0); // Base Operation: boolean and
  MpsParser.DefKeywordToken("or",0); // Base Operation: boolean or
  MpsParser.DefKeywordToken("not",0); // Base Operation: boolean not
  MpsParser.DefKeywordToken("if",0);   // Used for if then else expressions and terms
  MpsParser.DefKeywordToken("then",0); // Used for if then else expressions and terms
  MpsParser.DefKeywordToken("else",0); // Used for if then else expressions and terms
  MpsParser.DefKeywordToken("+",10); // Integer Addition
  MpsParser.DefKeywordToken("-",10); // Integer Addition
  MpsParser.DefKeywordToken("*",10); // Integer Addition
  MpsParser.DefKeywordToken("/",10); // Integer Addition
  MpsParser.DefKeywordToken("<=",0); // LEQ
  MpsParser.DefKeywordToken("&",0); // Touple indexing
  MpsParser.DefToken("int", "(~)?[0-9][0-9]*",10); // Base Value: integer
  MpsParser.DefToken("string", "\"[^\"]*\"",10); // Base Value: integer
  // Type keywords
  MpsParser.DefKeywordToken("Int",1);         // Simple type keyword
  MpsParser.DefKeywordToken("String",1);      // Simple type keyword
  MpsParser.DefKeywordToken("Bool",1);        // Simple type keyword
  MpsParser.DefKeywordToken("=>",0);          // Communication type
  MpsParser.DefKeywordToken("@",1);           // Used for T @ ( p of n )
  MpsParser.DefKeywordToken("of",1);          // Usex in T @ ( p of n )
  MpsParser.DefKeywordToken("as",1);          // Usex in 1=>2:1<Bool> as x
  MpsParser.DefKeywordToken("forall",1);      // Usex in forall x [[x or y]]
  MpsParser.DefKeywordToken(".",1);           // Used for rec X . G
  MpsParser.DefKeywordToken("rec",1);         // Recursive type keyword
  MpsParser.DefKeywordToken("Lend",1);        // Local type end
  MpsParser.DefKeywordToken("Gend",1);        // Global type end
  MpsParser.DefKeywordToken("<",2);    // Used for message types
  MpsParser.DefKeywordToken(">",2);    // Used for message types
//  MpsParser.DefToken("dvar", "§[a-zA-Z0-9_]+",10);     // Dependant type var
  MpsParser.DefToken("lvar", "%[a-zA-Z0-9_][a-zA-Z0-9_]*",10);     // Local type var
  MpsParser.DefToken("gvar", "$[a-zA-Z0-9_][a-zA-Z0-9_]*",10);   // Global type var
  MpsParser.DefKeywordToken("[[",1);        // Assertion Start
  MpsParser.DefKeywordToken("]]",1);        // Assertion End
  //MpsParser.DefKeywordToken("((",1);        // Double LPar
  //MpsParser.DefKeywordToken("))",1);        // Double RPar
  // Term keywords
  MpsParser.DefKeywordToken(";",1);    // Command separator
  MpsParser.DefKeywordToken("define",0); // Nonrecursive definition of type
  MpsParser.DefKeywordToken("def",1);  // Recursive def
  MpsParser.DefKeywordToken("in",1);   // Recursive def
  MpsParser.DefKeywordToken(":=",0);   // Value assignment
  MpsParser.DefKeywordToken("=",1);    // Used in def
  MpsParser.DefKeywordToken("link",0); // Init Link
  MpsParser.DefKeywordToken("sync",0); // Synchronise
  MpsParser.DefKeywordToken("nu",0);   // Hide
  MpsParser.DefKeywordToken("end",0);  // Terminated Process
  MpsParser.DefKeywordToken(">>",1);   // Receive
  MpsParser.DefKeywordToken("<<",1);   // Send
  MpsParser.DefToken("par","\\|",1);   // Conc
  MpsParser.DefKeywordToken(",,",1);    // Used in branching
  MpsParser.DefKeywordToken(",",2);    // Used in branching
  MpsParser.DefKeywordToken("{",2);    // Used in branching
  MpsParser.DefKeywordToken("}",2);    // Used in branching
  MpsParser.DefKeywordToken(":",1);    // Used in branching
  MpsParser.DefKeywordToken("[",2);    // Used for session channel
  MpsParser.DefKeywordToken("]",2);    // Used for session channel
  MpsParser.DefToken("","[ \n\r\t][ \n\r\t]*",9);            // Whitespace is ignored
  MpsParser.DefToken("","//[^\n]*",9);                // Comments are ignored
  MpsParser.DefToken("id", "[a-z][a-zA-Z0-9_]*",10);   // Variable or channel name
  MpsParser.DefToken("bid", "[#^][a-zA-Z0-9_]*",10);   // Branch id, called label
  MpsParser.DefToken("pvar", "[A-Z][a-zA-Z0-9_]*",10); // Process var
  MpsParser.DefKeywordToken("guisync",2);    // Used for GUI generation
  MpsParser.DefKeywordToken("guivalue",2);   // Used for GUI generation
  /*** Define grammars ***/
  // Expression Grammar
  MpsParser.DefType(MpsExp::BNF_EXP);
  MpsParser.DefType(MpsExp::BNF_EXPS);         // Expression Tuple
  MpsParser.DefType(MpsExp::BNF_EXPS2);        // Nonempty Expression Tuple
  MpsParser.DefType(MpsGlobalType::BNF_ASSERTION); // Assertion
  MpsParser.DefType(MpsGlobalType::BNF_NAMEDASSERTION); // Name and Assertion
  MpsParser.DefType(MpsGlobalType::BNF_LABEL); // Branch Label
  // Type grammars
  MpsParser.DefType(MpsMsgType::BNF_STYPE);
  MpsParser.DefType(MpsMsgType::BNF_STYPES);
  MpsParser.DefType(MpsMsgType::BNF_STYPES2);
  MpsParser.DefType(MpsMsgType::BNF_MTYPE);
  MpsParser.DefType(MpsLocalType::BNF_LTYPE);
  MpsParser.DefType(MpsLocalType::BNF_LBRANCHES);
  // Typed Args (with initial value)
  MpsParser.DefType(MpsGlobalType::BNF_TARGS);
  MpsParser.DefType(MpsGlobalType::BNF_TARGS2);                    // Targ list
  MpsParser.DefType(MpsGlobalType::BNF_TARGS3);            // Nonempty Targ list
  MpsParser.DefType(MpsGlobalType::BNF_TARG);                  // Typed argument with default value
  MpsParser.DefType(MpsGlobalType::BNF_TVALS);
  MpsParser.DefType(MpsGlobalType::BNF_GTYPE);                     // Global Types
  MpsParser.DefType(MpsGlobalType::BNF_GBRANCHES);
  // Term grammar
  MpsParser.DefType("ch ::= id | id [ int ]");                     // Channels
  MpsParser.DefType("branches ::= branch | branch , branches");    // Nonempty Branches list
  MpsParser.DefType("branch ::= Label : pi");                      // Branch
  MpsParser.DefType("inputbranches ::= inputbranch \
                                     | inputbranch , inputbranches"); // Nonempty GUIBranch list
  MpsParser.DefType("inputbranch ::= Label ( Targs2 ) : pi");      // GUIBranch
  MpsParser.DefType("args ::= args2 |");                           // Argument list
  MpsParser.DefType("args2 ::= id : Mtype | id : Mtype , args2");  // Argument list
  MpsParser.DefType("dargs ::= < args > |");                       // Optional dependant args
  MpsParser.DefType("dexps ::= < exps > |");                       // Optional dependant args
  MpsParser.DefType("ids ::= | id  ids");                          // Name list
  MpsParser.DefType("pi ::= pi par pi2 | pi2");                    // Processes
  MpsParser.DefType("pi2 ::= ( pi ) \
                           | ( nu id : Gtype ) pi2 \
                           | ch << exp ; pi2 \
                           | ch >> id ; pi2 \
                           | ch << bid ; pi2 \
                           | ch >> { branches } \
                           | def pvar dargs ( args ) = pi in pi2 \
                           | pvar dexps ( exps ) \
                           | link ( exps ) ; pi2\
                           | sync ( exps ) { branches } \
                           | end \
                           | if exp then pi2 else pi2 \
                           | guisync ( exps ) { inputbranches } \
                           | guivalue ( exps ) ; pi2 \
                           | id : Mtype = exp ; pi2 \
                           | define gvar ids = Gtype in pi2 \
                           | define lvar ids = Ltype in pi2");         // More processes

  parsed_tree *tree = MpsParser.Parse(exp);
  MpsTerm *result=MpsTerm::Create(tree);
  delete tree;
  return result;
} // }}}
void SetAssertion(const std::string &id, const parsed_tree *tree,  map<string,MpsExp*> &assertions) // {{{
{
  if (tree->type_name == "Assertion" && tree->case_name == "case1") // [[ exp ]]
    assertions[id] = MpsExp::Create(tree->content[1]);
  else if (tree->type_name == "Assertion" && tree->case_name == "case2") // 
    assertions[id] = new MpsBoolVal(true);
  else 
  {
#if APIMS_DEBUG_LEVEL>1
    cerr << "ERROR: Set Assertion called on node: " << tree->type_name << "." << tree->case_name << endl;
#endif
  }
} // }}}
void SetAssertion(const parsed_tree *tree,  map<string,MpsExp*> &assertions) // {{{
{
  if (tree->type_name == "Label" && tree->case_name == "case1") // bid Assertion
    SetAssertion(tree->content[0]->root.content,tree->content[1],assertions);
  else 
  {
#if APIMS_DEBUG_LEVEL>1
    cerr << "ERROR: Set Assertion called on node: " << tree->type_name << "." << tree->case_name << endl;
#endif
  }
} // }}}
MpsExp *CreateAssertion(const parsed_tree *tree) // {{{
{
  if (tree->type_name == "Label" && tree->case_name == "case1") // bid Assertion
    return CreateAssertion(tree->content[1]);
  else if (tree->type_name == "Assertion" && tree->case_name == "case1") // [[ exp ]]
    return MpsExp::Create(tree->content[1]);
  else if (tree->type_name == "Assertion" && tree->case_name == "case2") //
    return new MpsBoolVal(true);
  else
  {
#if APIMS_DEBUG_LEVEL>1
    cerr << "Unknown assertion constructor: " << tree->type_name << "." << tree->case_name << endl;
#endif
  }
  return new MpsBoolVal(false);
} // }}}
void AddBranch(const parsed_tree *exp, map<string,MpsTerm*> &dest,  map<string,MpsExp*> &assertions) // {{{
{
  if (exp->type_name == "branch" && exp->case_name == "case1") // Label : pi
  {
    dest[exp->content[0]->content[0]->root.content] = MpsTerm::Create(exp->content[2]);
    SetAssertion(exp->content[0],assertions);
  }
  else
  {
#if APIMS_DEBUG_LEVEL>1
    cerr << "Unknown branch constructor: " << exp->type_name << "." << exp->case_name << endl;
#endif
  }
  return;
} // }}}
void FindBranches(const parsed_tree *exp, map<string,MpsTerm*> &dest, map<string,MpsExp*> &assertions) // {{{
{
  if (exp->type_name == "branches" && exp->case_name == "case1") // branch
    AddBranch(exp->content[0], dest, assertions);
  else if (exp->type_name == "branches" && exp->case_name == "case2") // branch , branches
  {
    AddBranch(exp->content[0], dest, assertions);
    FindBranches(exp->content[2], dest, assertions);
  }
  else
  {
#if APIMS_DEBUG_LEVEL>1
    cerr << "Unknown branches constructor: " << exp->type_name << "." << exp->case_name << endl;
#endif
  }
  return;
} // }}}
void FindArgs(const parsed_tree *exp, vector<string> &args, vector<MpsMsgType*> &types) // {{{
{
  if (exp->type_name == "args" && exp->case_name == "case1") // args2
    FindArgs(exp->content[0],args,types);
  else if (exp->type_name == "args" && exp->case_name == "case2") // 
    ; // Do nothing
  else if (exp->type_name == "args2" && exp->case_name == "case1") // id : Mtype
  {
    args.push_back(exp->content[0]->root.content);
    types.push_back(MpsMsgType::Create(exp->content[2]));
  }
  else if (exp->type_name == "args2" && exp->case_name == "case2") // id : Mtype , args2
  {
    args.push_back(exp->content[0]->root.content);
    types.push_back(MpsMsgType::Create(exp->content[2]));
    FindArgs(exp->content[4],args,types);
  }
  else if (exp->type_name == "dargs" && exp->case_name == "case1") // < args >
    FindArgs(exp->content[1],args,types);
  else if (exp->type_name == "dargs" && exp->case_name == "case2") //
    ; // Do nothing
  else
  {
#if APIMS_DEBUG_LEVEL>1
    cerr << "Unknown args constructor: " << exp->type_name << "." << exp->case_name << endl;
#endif
  }
  return;
} // }}}
void FindGUIArgs(const parsed_tree *exp, vector<string> &args, vector<MpsMsgType*> &types, vector<MpsExp*> &values) // {{{
{
  if (exp->type_name == "Targs2" && exp->case_name == "case1") // Targs3
    FindGUIArgs(exp->content[0],args,types,values);
  else if (exp->type_name == "Targs2" && exp->case_name == "case2") // 
    ; // Do nothing
  else if (exp->type_name == "Targs3" && exp->case_name == "case1") // Targ
    FindGUIArgs(exp->content[0],args,types,values);
  else if (exp->type_name == "Targs3" && exp->case_name == "case2") // Targ , Targs3
  {
    FindGUIArgs(exp->content[0],args,types,values);
    FindGUIArgs(exp->content[2],args,types,values);
  }
  else if (exp->type_name == "Targ" && exp->case_name == "case1") // id : Mtype = exp
  {
    args.push_back(exp->content[0]->root.content);
    types.push_back(MpsMsgType::Create(exp->content[2]));
    values.push_back(MpsExp::Create(exp->content[4]));
  }
  else
  {
#if APIMS_DEBUG_LEVEL>1
    cerr << "Unknown args constructor: " << exp->type_name << "." << exp->case_name << endl;
#endif
  }
  return;
} // }}}
void AddInputBranch(const parsed_tree *exp, map<string,inputbranch> &dest) // {{{
{
  if (exp->type_name == "inputbranch" && exp->case_name == "case1") // Label ( args ) : pi
  {
    inputbranch branch;
    branch.names.clear();
    branch.args.clear();
    branch.types.clear();
    branch.values.clear();
    FindGUIArgs(exp->content[2],branch.args,branch.types,branch.values);
    branch.names = branch.args;
    branch.term = MpsTerm::Create(exp->content[5]);
    branch.assertion=CreateAssertion(exp->content[0]);
    dest[exp->content[0]->content[0]->root.content] = branch;
  }
  else
  {
#if APIMS_DEBUG_LEVEL>1
    cerr << "Unknown inputbranch constructor: " << exp->type_name << "." << exp->case_name << endl;
#endif
  }
  return;
} // }}}
void FindInputBranches(const parsed_tree *exp, map<string,inputbranch> &dest) // {{{
{
  if (exp->type_name == "inputbranches" && exp->case_name == "case1") // inputbranch
    AddInputBranch(exp->content[0], dest);
  else if (exp->type_name == "inputbranches" && exp->case_name == "case2") // inputbranch , inputbranches
  {
    AddInputBranch(exp->content[0], dest);
    FindInputBranches(exp->content[2],dest);
  }
  else
  {
#if APIMS_DEBUG_LEVEL>1
    cerr << "Unknown branches constructor: " << exp->type_name << "." << exp->case_name << endl;
#endif
  }
  return;
} // }}}
void FindExps(const parsed_tree *exp, vector<MpsExp*> &dest) // {{{
{
  if (exp->type_name == "exps" && exp->case_name == "case1") // exps2
    FindExps(exp->content[0],dest);
  else if (exp->type_name == "exps" && exp->case_name == "case2") //
    ;
  else if (exp->type_name == "exps2" && exp->case_name == "case1") // exp
    dest.push_back(MpsExp::Create(exp->content[0]));
  else if (exp->type_name == "exps2" && exp->case_name == "case2") // exp , exps
  {
    dest.push_back(MpsExp::Create(exp->content[0]));
    FindExps(exp->content[2],dest);
  }
  else if (exp->type_name == "dexps" && exp->case_name == "case1") // < exps >
    FindExps(exp->content[1],dest);
  else if (exp->type_name == "dexps" && exp->case_name == "case2") //
    ;
  else
  {
#if APIMS_DEBUG_LEVEL>1
    cerr << "Unknown exps constructor: " << exp->type_name << " . " << exp->case_name << endl;
#endif
  }
  return;
} // }}}
MpsChannel ParseChannel(const parsed_tree *ch) // {{{
{
  if (ch->type_name == "ch" && ch->case_name == "case1") // ch = id
  {
    MpsChannel channel(ch->content[0]->root.content);
    return channel;
  }
  else if (ch->type_name == "ch" && ch->case_name == "case2") // ch = id [ int ]
  {
    MpsChannel channel(ch->content[0]->root.content,string2int(ch->content[2]->root.content));
    return channel;
  }
  else
  {
#if APIMS_DEBUG_LEVEL>1
    cerr << "Parsing error: ParseChannel on non-channel: " << ch->type_name << "." << ch->case_name << endl;
#endif
    return MpsChannel("_ERROR");
  }
} // }}}
vector<string> FindNames(const parsed_tree *names) // {{{
{ if (names->type_name == "ids" && names->case_name == "case1") // Empty
    return vector<string>();
  else if (names->type_name == "ids" && names->case_name == "case2") // id ids
  { vector<string> result = FindNames(names->content[1]);
    result.insert(result.begin(),names->content[0]->root.content);
    return result;
  }

#if APIMS_DEBUG_LEVEL>1
  cerr << "Unknown name list parsetree: " << names->type_name << "." << names->case_name << endl;
#endif
  return vector<string>();
} // }}}
MpsTerm *MpsTerm::Create(const parsed_tree *exp) // {{{
{
  if (exp->type_name == "pi" && exp->case_name == "case1") // pi par pi {{{
  {
    MpsTerm *left = MpsTerm::Create(exp->content[0]);
    MpsTerm *right = MpsTerm::Create(exp->content[2]);
    MpsTerm *result = new MpsPar(*left, *right);
    delete left;
    delete right;
    return result;
  } // }}}
  else if (exp->type_name == "pi" && exp->case_name == "case2") // pi2 {{{
  {
    return MpsTerm::Create(exp->content[0]);
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case1") // ( pi ) {{{
  {
    return MpsTerm::Create(exp->content[1]);
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case2") // ( nu id : Gtype ) pi2 {{{
  {
    MpsTerm *succ = MpsTerm::Create(exp->content[6]);
    MpsGlobalType *type = MpsGlobalType::Create(exp->content[4]);
    MpsTerm *result = new MpsNu(exp->content[2]->root.content, *succ, *type);
    delete succ;
    delete type;
    return result;
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case3") // ch << exp ; pi2 {{{
  {
    MpsTerm *succ = MpsTerm::Create(exp->content[4]);
    MpsExp *value = MpsExp::Create(exp->content[2]);
    MpsChannel dest = ParseChannel(exp->content[0]);
    MpsTerm *result = new MpsSnd(dest, *value, *succ);
    delete succ;
    delete value;
    return result;
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case4") // ch >> id ; pi2 {{{
  {
    MpsTerm *succ = MpsTerm::Create(exp->content[4]);
    MpsChannel source=ParseChannel(exp->content[0]);
    MpsTerm *result = new MpsRcv(source, exp->content[2]->root.content, *succ);

    delete succ;
    return result;
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case5") // ch << bid ; pi2 {{{
  {
    MpsTerm *succ = MpsTerm::Create(exp->content[4]);
    MpsChannel dest = ParseChannel(exp->content[0]);
    MpsTerm *result = new MpsSelect(dest, exp->content[2]->root.content, *succ);
    delete succ;
    return result;
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case6") // ch >> { branches } {{{
  {
    map<string, MpsTerm*> branches;
    branches.clear();
    map<string, MpsExp*> assertions;
    assertions.clear();
    FindBranches(exp->content[3], branches, assertions);
    MpsChannel source=ParseChannel(exp->content[0]);
    MpsTerm *result = new MpsBranch(source, branches, assertions);

    // Clean up
    while (branches.size() > 0)
    {
      delete branches.begin()->second;
      branches.erase(branches.begin());
    }
    while (assertions.size() > 0)
    {
      delete assertions.begin()->second;
      assertions.erase(assertions.begin());
    }
    return result;
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case7") // def pvar dargs ( args ) = pi in pi2 {{{
  {
    MpsTerm *body = MpsTerm::Create(exp->content[7]);
    MpsTerm *succ = MpsTerm::Create(exp->content[9]);
    // Parse args
    vector<string> args;
    args.clear();
    vector<MpsMsgType*> types;
    types.clear();
    FindArgs(exp->content[4],args,types);
    // Parse state
    vector<string> stateargs;
    stateargs.clear();
    vector<MpsMsgType*> statetypes;
    statetypes.clear();
    FindArgs(exp->content[2],stateargs,statetypes);
    MpsTerm *result = new MpsDef(exp->content[1]->root.content, args, types, stateargs, statetypes, *body, *succ);
    // Clean up
    delete succ;
    delete body;
    while (types.size() > 0)
    {
      delete *types.begin();
      types.erase(types.begin());
    }
    while (statetypes.size() > 0)
    {
      delete *statetypes.begin();
      statetypes.erase(statetypes.begin());
    }
    return result;
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case8") // pvar dexps ( exps ) {{{
  {
    vector<MpsExp*> args;
    args.clear();
    FindExps(exp->content[3],args);
    vector<MpsExp*> state;
    state.clear();
    FindExps(exp->content[1],state);
    MpsTerm *result = new MpsCall(exp->content[0]->root.content, args, state);
    DeleteVector(args);
    DeleteVector(state);
    return result;
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case9") // link ( exps ) ; pi2 {{{
  { 
    vector<MpsExp*> args;
    args.clear();
    FindExps(exp->content[2],args);
    // exps must be int , id , id , int
    if (args.size()==4 &&
        typeid(*args[0]) == typeid(MpsIntVal) &&
        typeid(*args[1]) == typeid(MpsVarExp) &&
        typeid(*args[2]) == typeid(MpsVarExp) &&
        typeid(*args[3]) == typeid(MpsIntVal))
    {
      MpsTerm *succ = MpsTerm::Create(exp->content[5]);
      MpsTerm *result = new MpsLink(((MpsVarExp*)args[1])->ToString(),
                                    ((MpsVarExp*)args[2])->ToString(),
                                    mpz_get_si(((MpsIntVal*)args[3])->GetValue()),
                                    mpz_get_si(((MpsIntVal*)args[0])->GetValue()),
                                    *succ);
      delete succ;
      while (args.size()>0)
      {
        delete *args.begin();
        args.erase(args.begin());
      }
      return result;
    }
    else
    {
      while (args.size()>0)
      {
        delete *args.begin();
        args.erase(args.begin());
      }
#if APIMS_DEBUG_LEVEL>1
      cerr << "Parsing error: Arguments for link must be: int, id, id, int" << endl;
#endif
      return new MpsEnd();
    }
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case10") // sync ( exps ) { branches } {{{
  { 
    vector<MpsExp*> args;
    args.clear();
    FindExps(exp->content[2],args);
    // exps must be int , id
    if (args.size()==2 &&
        typeid(*args[0]) == typeid(MpsIntVal) &&
        typeid(*args[1]) == typeid(MpsVarExp))
    {
      // Init branches
      map<string, MpsTerm*> branches;
      branches.clear();
      map<string, MpsExp*> assertions;
      assertions.clear();
      FindBranches(exp->content[5], branches, assertions);

      MpsTerm *result = new MpsSync(mpz_get_si(((MpsIntVal*)args[0])->GetValue()),
                                    ((MpsVarExp*)args[1])->ToString(),
                                    branches,
                                    assertions);
      // Clean up
      while (args.size()>0)
      {
        delete *args.begin();
        args.erase(args.begin());
      }
      while (branches.size() > 0)
      {
        delete branches.begin()->second;
        branches.erase(branches.begin());
      }
      while (assertions.size() > 0)
      {
        delete assertions.begin()->second;
        assertions.erase(assertions.begin());
      }
      return result;
    }
    else
    {
      while (args.size()>0)
      {
        delete *args.begin();
        args.erase(args.begin());
      }
#if APIMS_DEBUG_LEVEL>1
      cerr << "Parsing error: Arguments for sync must be: int, id" << endl;
#endif
      return new MpsEnd();
    }
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case11") // end {{{
  {
    return new MpsEnd();
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case12") // if exp then pi2 else pi2 {{{
  {
    MpsExp *cond = MpsExp::Create(exp->content[1]);
    MpsTerm *truebranch = MpsTerm::Create(exp->content[3]);
    MpsTerm *falsebranch = MpsTerm::Create(exp->content[5]);
    MpsTerm *result = new MpsCond(*cond,*truebranch,*falsebranch);
    delete cond;
    delete truebranch;
    delete falsebranch;
    return result;
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case13") // guisync ( exps ) { inputbranches } {{{
  { 
    vector<MpsExp*> args;
    args.clear();
    FindExps(exp->content[2],args);
    // exps must be int , id, int
    if (args.size()==3 &&
        typeid(*args[0]) == typeid(MpsIntVal) &&
        typeid(*args[1]) == typeid(MpsVarExp) &&
        typeid(*args[2]) == typeid(MpsIntVal))
    {
      // Init branches
      map<string, inputbranch> branches;
      branches.clear();
      FindInputBranches(exp->content[5], branches);

      MpsTerm *result = new MpsGuiSync(mpz_get_si(((MpsIntVal*)args[0])->GetValue()),
                                       ((MpsVarExp*)args[1])->ToString(),
                                       mpz_get_si(((MpsIntVal*)args[2])->GetValue()),
                                       branches);
      // Clean up
      while (args.size()>0)
      {
        delete *args.begin();
        args.erase(args.begin());
      }
      while (branches.size() > 0)
      {
        delete branches.begin()->second.term;
        delete branches.begin()->second.assertion;
        while (branches.begin()->second.types.size()>0)
        {
          delete *branches.begin()->second.types.begin();
          branches.begin()->second.types.erase(branches.begin()->second.types.begin());
        }
        while (branches.begin()->second.values.size()>0)
        {
          delete *branches.begin()->second.values.begin();
          branches.begin()->second.values.erase(branches.begin()->second.values.begin());
        }
        branches.erase(branches.begin());
      }
      return result;
    }
    else
    {
      while (args.size()>0)
      {
        delete *args.begin();
        args.erase(args.begin());
      }
#if APIMS_DEBUG_LEVEL>1
      cerr << "Parsing error: Arguments for sync must be: int, id" << endl;
#endif
      return new MpsEnd();
    }
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case14") // guivalue ( exps ) ; pi2 {{{
  { 
    vector<MpsExp*> args;
    args.clear();
    FindExps(exp->content[2],args);
    // exps must be int, id, int, string, exp
    if (args.size()==5 &&
        typeid(*args[0]) == typeid(MpsIntVal) &&
        typeid(*args[1]) == typeid(MpsVarExp) &&
        typeid(*args[2]) == typeid(MpsIntVal))
    {
      // Init successor
      MpsTerm *succ = MpsTerm::Create(exp->content[5]);
      MpsTerm *result = new MpsGuiValue(mpz_get_si(((MpsIntVal*)args[0])->GetValue()),
                                        ((MpsVarExp*)args[1])->ToString(),
                                        mpz_get_si(((MpsIntVal*)args[2])->GetValue()),
                                        *((MpsExp*)args[3]),
                                        *((MpsExp*)args[4]),
                                        *succ);
      // Clean up
      while (args.size()>0)
      {
        delete *args.begin();
        args.erase(args.begin());
      }
      delete succ;
      return result;
    }
    else
    {
      while (args.size()>0)
      {
        delete *args.begin();
        args.erase(args.begin());
      }
#if APIMS_DEBUG_LEVEL>1
      cerr << "Parsing error: Arguments for guivalue must be: int, session, int, exp, exp" << endl;
#endif
      return new MpsEnd();
    }
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case15") // id : Mtype = exp ; pi2 {{{
  { 
    MpsMsgType *type = MpsMsgType::Create(exp->content[2]);
    MpsExp *value = MpsExp::Create(exp->content[4]);
    MpsTerm *succ = MpsTerm::Create(exp->content[6]);
    MpsTerm *result = new MpsAssign(exp->content[0]->root.content, *value, *type, *succ);
    delete type;
    delete value;
    delete succ;
    return result;
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case16") // define gvar ids = Gtype in pi2 {{{
  { 
    MpsTerm *term = MpsTerm::Create(exp->content[6]);
    MpsGlobalType *gtype = MpsGlobalType::Create(exp->content[4]);
    vector<string> names = FindNames(exp->content[2]);
    string name = exp->content[1]->root.content;
    MpsTerm *result = term->GSubst(name,*gtype, names);

    // Clean up
    delete term;
    delete gtype;

    return result;
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case17") // define lvar ids = Ltype in pi2 {{{
  { 
    MpsTerm *term = MpsTerm::Create(exp->content[6]);
    MpsLocalType *ltype = MpsLocalType::Create(exp->content[4]);
    vector<string> names=FindNames(exp->content[2]);
    string name = exp->content[1]->root.content;
    MpsTerm *result = term->LSubst(name,*ltype, names);

    // Clean up
    delete term;
    delete ltype;

    return result;
  } // }}}

#if APIMS_DEBUG_LEVEL>1
  cerr << "Unknown term parsetree: " << exp->type_name << "." << exp->case_name << endl;
#endif
  return new MpsEnd();
} // }}}

/* MpsTerm Constructors
 */
MpsEnd::MpsEnd() // {{{
{
} // }}}
MpsSnd::MpsSnd(const MpsChannel &channel, const MpsExp &exp, const MpsTerm &succ) // {{{
: myChannel(channel)
{
  mySucc = succ.Copy();
  myExp = exp.Copy();
} // }}}
MpsRcv::MpsRcv(const MpsChannel &channel, const string &dest, const MpsTerm &succ) // {{{
: myChannel(channel), myDest(dest)
{
  mySucc = succ.Copy();
} // }}}
MpsSelect::MpsSelect(const MpsChannel &channel, const string &label, const MpsTerm &succ) // {{{
: myChannel(channel), myLabel(label)
{
  // Assert succ!=NULL
  mySucc = succ.Copy();
} // }}}
MpsBranch::MpsBranch(const MpsChannel &channel, const map<string,MpsTerm*> &branches, const map<string,MpsExp*> &assertions) // {{{
: myChannel(channel)
{
  myBranches.clear();
  myAssertions.clear();
  for (map<string,MpsTerm*>::const_iterator it = branches.begin(); it != branches.end(); ++it)
  {
    // assert it->second != NULL
    MpsTerm *branch = it->second->Copy();
    myBranches[it->first] = branch;
    // Copy assertion
    map<string,MpsExp*>::const_iterator it2=assertions.find(it->first);
    MpsExp *assertion=NULL;
    if (it2==assertions.end())
      assertion=new MpsBoolVal(true);
    else
      assertion=it2->second->Copy();
    myAssertions[it->first] = assertion;
  }
} // }}}
MpsPar::MpsPar(const MpsTerm &left, const MpsTerm &right) // {{{
{
  // Assert left != NULL
  // Assert right != NULL
  myLeft = left.Copy();
  myRight = right.Copy();
} // }}}
MpsDef::MpsDef(const string &name, const std::vector<std::string> &args, const vector<MpsMsgType*> &types, const std::vector<std::string> &stateargs, const vector<MpsMsgType*> &statetypes, const MpsTerm &body, const MpsTerm &succ) // {{{
: myName(name),
  myArgs(args),
  myStateArgs(stateargs)
{
  myTypes.clear();
  for (vector<MpsMsgType*>::const_iterator type=types.begin(); type!=types.end(); ++type)
    myTypes.push_back((*type)->Copy());
  myStateTypes.clear();
  for (vector<MpsMsgType*>::const_iterator type=statetypes.begin(); type!=statetypes.end(); ++type)
    myStateTypes.push_back((*type)->Copy());
  // Assert body!=NULL
  // Assert succ!=NULL
  myBody = body.Copy();
  mySucc = succ.Copy();
} // }}}
MpsCall::MpsCall(const string &name, const vector<MpsExp*> &args, const vector<MpsExp*> &state) // {{{
: myName(name)
{
  myArgs.clear();
  for (vector<MpsExp*>::const_iterator it=args.begin(); it!=args.end(); ++it)
    myArgs.push_back((*it)->Copy());
  myState.clear();
  for (vector<MpsExp*>::const_iterator it=state.begin(); it!=state.end(); ++it)
    myState.push_back((*it)->Copy());
} // }}}
MpsNu::MpsNu(const string &channel, const MpsTerm &succ, const MpsGlobalType &type) // {{{
: myChannel(channel)
{
  mySucc = succ.Copy();
  myType = type.Copy();
} // }}}
MpsLink::MpsLink(const string &channel, const std::string &session, int pid, int maxpid, const MpsTerm &succ) // {{{
: myChannel(channel),
  mySession(session),
  myMaxpid(maxpid),
  myPid(pid)
{
  // Assert succ!=NULL
  mySucc = succ.Copy();
} // }}}
MpsSync::MpsSync(int maxpid, const std::string &session, const std::map<std::string, MpsTerm*> &branches, const std::map<std::string, MpsExp*> &assertions) // {{{
: mySession(session),
  myMaxpid(maxpid)
{
  myBranches.clear();
  for (map<string,MpsTerm*>::const_iterator it = branches.begin(); it != branches.end(); ++it)
    myBranches[it->first] = it->second->Copy();
  myAssertions.clear();
  for (map<string,MpsExp*>::const_iterator it=assertions.begin(); it!=assertions.end(); ++it)
    myAssertions[it->first] = it->second->Copy();
} // }}}
MpsCond::MpsCond(const MpsExp &cond, const MpsTerm &truebranch, const MpsTerm &falsebranch) // {{{
{
  myCond = cond.Copy();
  myTrueBranch = truebranch.Copy();
  myFalseBranch = falsebranch.Copy();
} // }}}
MpsGuiSync::MpsGuiSync(int maxpid, const std::string &session, int pid, const std::map<std::string, inputbranch> &branches) // {{{
: mySession(session),
  myMaxpid(maxpid),
  myPid(pid)
{
  myBranches.clear();
  for (map<string,inputbranch>::const_iterator it=branches.begin(); it!=branches.end(); ++it)
  {
    inputbranch branch;
    branch.term = it->second.term->Copy();
    branch.assertion = it->second.assertion->Copy();
    branch.names = it->second.names;
    branch.args = it->second.args;
    branch.types.clear();
    for (vector<MpsMsgType*>::const_iterator type=it->second.types.begin(); type!=it->second.types.end(); ++type)
      branch.types.push_back((*type)->Copy());
    branch.values.clear();
    for (vector<MpsExp*>::const_iterator value=it->second.values.begin(); value!=it->second.values.end(); ++value)
      branch.values.push_back((*value)->Copy());
    myBranches[it->first] = branch;
  }
} // }}}
MpsGuiValue::MpsGuiValue(int maxpid, const std::string &session, int pid, const MpsExp &name, const MpsExp &value, const MpsTerm &succ) // {{{
: mySession(session),
  myMaxpid(maxpid),
  myPid(pid)
{
  mySucc = succ.Copy();
  myValue = value.Copy();
  myName = name.Copy();
} // }}}
MpsAssign::MpsAssign(const string &id, const MpsExp &exp, const MpsMsgType &type, const MpsTerm &succ) // {{{
{
  myId = id;
  myExp = exp.Copy();
  myType = type.Copy();
  mySucc = succ.Copy();
} // }}}

/* MpsTerm Destructors
 */
MpsEnd::~MpsEnd() // {{{
{
} // }}}
MpsSnd::~MpsSnd() // {{{
{
  delete mySucc;
  delete myExp;
} // }}}
MpsRcv::~MpsRcv() // {{{
{
  delete mySucc;
} // }}}
MpsSelect::~MpsSelect() // {{{
{
  delete mySucc;
} // }}}
MpsBranch::~MpsBranch() // {{{
{
  while (myBranches.size()>0)
  {
    // assert myBranches.begin()->second != NULL
    delete myBranches.begin()->second;
    myBranches.erase(myBranches.begin());
  }
  DeleteMap(myAssertions);
} // }}}
MpsPar::~MpsPar() // {{{
{
  // assert myLeft and myRight != NULL
  delete myLeft;
  delete myRight;
} // }}}
MpsDef::~MpsDef() // {{{
{
  // assert mySucc != NULL
  // assert myBody != NULL
  DeleteVector(myTypes);
  DeleteVector(myStateTypes);
  delete mySucc;
  delete myBody;
} // }}}
MpsCall::~MpsCall() // {{{
{
  DeleteVector(myArgs);
  DeleteVector(myState);
} // }}}
MpsNu::~MpsNu() // {{{
{
  // assert mySucc != NULL
  delete mySucc;
  delete myType;
} // }}}
MpsLink::~MpsLink() // {{{
{
  // assert mySucc != NULL
  delete mySucc;
} // }}}
MpsSync::~MpsSync() // {{{
{
  DeleteMap(myBranches);
  DeleteMap(myAssertions);
} // }}}
MpsCond::~MpsCond() // {{{
{
  delete myCond;
  delete myTrueBranch;
  delete myFalseBranch;
} // }}}
MpsGuiSync::~MpsGuiSync() // {{{
{
  while (myBranches.size()>0)
  {
    delete myBranches.begin()->second.term;
    delete myBranches.begin()->second.assertion;
    while (myBranches.begin()->second.types.size()>0)
    {
      delete *myBranches.begin()->second.types.begin();
      myBranches.begin()->second.types.erase(myBranches.begin()->second.types.begin());
    }
    while (myBranches.begin()->second.values.size()>0)
    {
      delete *myBranches.begin()->second.values.begin();
      myBranches.begin()->second.values.erase(myBranches.begin()->second.values.begin());
    }
    myBranches.erase(myBranches.begin());
  }
} // }}}
MpsGuiValue::~MpsGuiValue() // {{{
{
  delete myName;
  delete myValue;
  delete mySucc;
} // }}}
MpsAssign::~MpsAssign() // {{{
{
  delete myExp;
  delete myType;
  delete mySucc;
} // }}}

/* Static type-checking of deadlock and communication safety
 */
bool MpsTerm::TypeCheck() // {{{
{
  // Create environments
  MpsBoolVal Theta(true);
  MpsGlobalEnv Gamma;
  MpsLocalEnv Delta;
  MpsMsgEnv Sigma;
  MpsProcEnv Omega;
  return TypeCheck(Theta,Gamma,Delta,Sigma,Omega);
} // }}}

// Typechecking output functions
string PrintGamma(const MpsGlobalEnv &Gamma, const string &indent="") // {{{
{
  string result="";
  for (MpsGlobalEnv::const_iterator it=Gamma.begin();it!=Gamma.end();++it)
  {
    if (it!=Gamma.begin())
      result += ",\n" + indent;
    string newIndent = indent + "  ";
    for (int i=0; i<it->first.size(); ++i)
      newIndent += " ";
    result += it->first + ": " + it->second->ToString(newIndent);
  }
  return result;
} // }}}
string PrintDelta(const MpsLocalEnv &Delta, const string &indent) // {{{
{
  string result="";
  for (MpsLocalEnv::const_iterator it=Delta.begin();it!=Delta.end();++it)
  {
    if (it!=Delta.begin())
      result += ",\n" + indent;
    string newIndent = indent + "  ";
    for (int i=0; i<it->first.size(); ++i)
      newIndent += " ";
    result += it->first + ": " + it->second.type->ToString(newIndent) + "@(" + int2string(it->second.pid) + " of " + int2string(it->second.maxpid) + ")";
  }
  return result;
} // }}}
string PrintSigma(const MpsMsgEnv &Sigma, const string &indent) // {{{
{
  string result="";
  for (MpsMsgEnv::const_iterator it=Sigma.begin();it!=Sigma.end();++it)
  {
    if (it!=Sigma.begin())
      result += ",\n" + indent;
    string newIndent = indent + "  ";
    for (int i=0; i<it->first.size(); ++i)
      newIndent += " ";
    result += it->first + ": " + it->second->ToString(newIndent);
  }
  return result;
} // }}}
string PrintOmega(const MpsProcEnv &Omega, const string &indent) // {{{
{
  string result = "";
  for (MpsProcEnv::const_iterator it=Omega.begin();it!=Omega.end();++it)
  {
    if (it!=Omega.begin())
      result += ",\n" + indent;
    result += it->first + "< ";
    string newIndent = indent + "  ";
    for (int i=0;i < it->first.size();++i)
      newIndent += " ";
    for (int i=0; i<it->second.stypes.size(); ++i)
    { if (i>0)
        result += ",\n" + newIndent;
      result += it->second.snames[i] + ": " + it->second.stypes[i]->ToString(newIndent);
    }
    result += " >\n"+newIndent+"( ";
    for (vector<MpsMsgType*>::const_iterator arg=it->second.types.begin(); arg!=it->second.types.end(); ++arg)
    {
      if (arg!=it->second.types.begin())
        result += ",\n" + newIndent;
      result += (*arg)->ToString(newIndent);
    }
    result += " )";
  }
  return result;
} // }}}
bool PrintTypeError(const string &message, const MpsTerm &term, const MpsExp &Theta, const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma, const MpsProcEnv &Omega) // {{{
{
#if APIMS_DEBUG_LEVEL>1
  cerr << "!!!!!!!!!!!!!!! Type Error: !!!!!!!!!!!!!!!" << endl
       << "!!!!!Term: " << term.ToString("!!!!!      ") << endl
       << "!!!!Theta: " << Theta.ToString() << endl
       << "!!!!Gamma: " << PrintGamma(Gamma,"!!!!       ") << endl
       << "!!!!Delta: " << PrintDelta(Delta,"!!!!       ") << endl
       << "!!!!Sigma: " << PrintSigma(Sigma,"!!!!       ") << endl
       << "!!!!Omega: " << PrintOmega(Omega,"!!!!       ") << endl
       << "!!Message: " << message << endl;
#endif
  return false;
} // }}}

// Helpfunction for when unfolding is necessary
inline bool TypeCheckRec(const MpsExp &Theta, const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma, const MpsProcEnv &Omega, MpsTerm &term, const string &session) // Using new rule unfold (or eq) {{{
{
  MpsLocalEnv::const_iterator it=Delta.find(session);
  if (it==Delta.end())
    return PrintTypeError((string)"Unfolding closed session: " + session,term,Theta,Gamma,Delta,Sigma,Omega);
  const MpsLocalRecType *type = dynamic_cast<const MpsLocalRecType*>(it->second.type);
  if (type==NULL)
    return PrintTypeError((string)"Unfolding non-rec type: " + it->second.type->ToString(),term,Theta,Gamma,Delta,Sigma,Omega);
  MpsLocalEnv newDelta = Delta;
  // Create type for substitution
  vector<TypeArg> args;
  vector<string> argnames;
  for (vector<TypeArg>::const_iterator it=type->GetArgs().begin(); it!=type->GetArgs().end(); ++it)
  { MpsExp *newValue = new MpsVarExp(it->myName);
    TypeArg arg(it->myName, *it->myType,  *newValue);
    delete newValue;
    args.push_back(arg);
    argnames.push_back(it->myName);
  }
  MpsLocalType *substType=new MpsLocalRecType(type->GetName(), *type->GetSucc(), args);
  MpsLocalType *newType = type->GetSucc()->LSubst(type->GetName(),*substType,argnames);
  delete substType;
  for (vector<TypeArg>::const_iterator arg=type->GetArgs().begin(); arg!=type->GetArgs().end(); ++arg)
  { MpsLocalType *tmpType = newType->ESubst(arg->myName,*arg->myValue);
    delete newType;
    newType=tmpType;
  }
  newDelta[session].type = newType;
  bool result = false;
  if (dynamic_cast<MpsLocalRecType*>(newType)==NULL)
    result = term.TypeCheck(Theta,Gamma,newDelta,Sigma,Omega);
  else
    result = PrintTypeError((string)"Using non-contractive type: " + it->second.type->ToString(),term,Theta,Gamma,Delta,Sigma,Omega);
  delete newType;
  return result;
} // }}}
inline bool TypeCheckForall(const MpsExp &Theta, const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma, const MpsProcEnv &Omega, MpsTerm &term, const string &session) // Using new rule forall {{{
{
  MpsLocalEnv::const_iterator it=Delta.find(session);
  if (it==Delta.end())
    return PrintTypeError((string)"Forall on closed session: " + session,term,Theta,Gamma,Delta,Sigma,Omega);
  const MpsLocalForallType *type = dynamic_cast<const MpsLocalForallType*>(it->second.type);
  if (type==NULL)
    return PrintTypeError((string)"Forall on non-forall type: " + it->second.type->ToString(),term,Theta,Gamma,Delta,Sigma,Omega);
  // Find new name for bound variable
  string newName = MpsExp::NewVar(type->GetName());
  // Create type for substitution
  MpsLocalType *newType = type->GetSucc()->ERename(type->GetName(),newName);
  // Create new Theta
  MpsExp *newAssertion = type->GetAssertion().Rename(type->GetName(),newName);
  MpsExp *newTheta=new MpsBinOpExp("and",Theta,*newAssertion);
  delete newAssertion;
  // Create new Delta
  MpsLocalEnv newDelta = Delta;
  newDelta[session].type = newType;
  bool result = term.TypeCheck(*newTheta,Gamma,newDelta,Sigma,Omega);
  // Clean Up
  delete newType;
  delete newTheta;
  return result;
} // }}}

// TODO: Copy-Cleanup and collect all environments in one MpsMsgEnv &env
bool MpsEnd::TypeCheck(const MpsExp &Theta, const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma, const MpsProcEnv &Omega) // Use rule Inact {{{
{
  MpsLocalEndType endType;
  // Check that Delta is completed
  for (MpsLocalEnv::const_iterator session=Delta.begin();session!=Delta.end();++session)
    if (!session->second.type->Equal(Theta,endType))
      return PrintTypeError((string)"Unfinished Session: " + session->first,*this,Theta,Gamma,Delta,Sigma,Omega);
  return true;
} // }}}
bool MpsSnd::TypeCheck(const MpsExp &Theta, const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma, const MpsProcEnv &Omega) // Use rules Send and Deleg (and new rule for delegaing the session itself) {{{
{
  MpsLocalEnv::const_iterator session=Delta.find(myChannel.GetName());
  // Check session is open
  if (session==Delta.end())
    return PrintTypeError((string)"Sending on closed session: " + myChannel.GetName(),*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check if unfolding is necessary
  if (typeid(*session->second.type)==typeid(MpsLocalRecType))
    return TypeCheckRec(Theta, Gamma, Delta, Sigma, Omega, *this, session->first);
  if (typeid(*session->second.type)==typeid(MpsLocalForallType))
    return TypeCheckForall(Theta, Gamma, Delta, Sigma, Omega, *this, session->first);
  // Check session has send type
  MpsLocalSendType *typeptr = dynamic_cast<MpsLocalSendType*>(session->second.type);
  if (typeptr==NULL)
    return PrintTypeError((string)"Sending on session with non-send type: " + myChannel.GetName(),*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check channel index is correct
  if (myChannel.GetIndex() != typeptr->GetChannel())
    return PrintTypeError((string)"Sending on wrong index in session: " + myChannel.ToString(),*this,Theta,Gamma,Delta,Sigma,Omega);
  // Make new environment
  MpsLocalType *newType=NULL;
  if (typeptr->GetAssertionType())
    newType = typeptr->GetSucc()->ESubst(typeptr->GetAssertionName(),*myExp);
  else
    newType = typeptr->GetSucc()->Copy();
  MpsLocalEnv newDelta = Delta;
  newDelta[myChannel.GetName()].type=newType;
  // Check message type
  MpsMsgType *exptype = myExp->TypeCheck(Gamma,newDelta,Sigma);
  bool msgtypematch = exptype->Equal(Theta,*typeptr->GetMsgType());
  delete exptype;
  if (not msgtypematch)
    return PrintTypeError((string)"Message does not have type:\n!!" + typeptr->GetMsgType()->ToString("!!"),*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check Assertion is fulfilled
  if (typeptr->GetAssertionType())
  { if (typeptr->GetMsgType()->ToString()!="Bool")
      return PrintTypeError((string)"Assertions not supported for message-type: " + typeptr->GetMsgType()->ToString(),*this,Theta,Gamma,Delta,Sigma,Omega);
    MpsExp *msgAssertion=typeptr->GetAssertion().Subst(typeptr->GetAssertionName(),*myExp);
    vector<const MpsExp*> hyps;
    hyps.push_back(&Theta);
    bool checkAssertion=msgAssertion->ValidExp(hyps);
    hyps.pop_back();
    delete msgAssertion;
    if (not checkAssertion)
      return PrintTypeError((string)"Assertion not respected",*this,Theta,Gamma,Delta,Sigma,Omega);
  }
  // If delegating remove delegated session
  if (dynamic_cast<MpsDelegateMsgType*>(typeptr->GetMsgType())!=NULL)
    newDelta.erase(newDelta.find(myExp->ToString()));
  // Check rest of program
  bool result = mySucc->TypeCheck(Theta,Gamma,newDelta,Sigma,Omega);
  // Clean Up
  delete newType;
  return result;
} // }}}
bool MpsRcv::TypeCheck(const MpsExp &Theta, const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma, const MpsProcEnv &Omega) // Use rules Rcv and Srec {{{
{
  MpsLocalEnv::const_iterator session=Delta.find(myChannel.GetName());
  // Check session is open
  if (session==Delta.end())
    return PrintTypeError((string)"Receiving on closed session: " + myChannel.GetName(),*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check if unfolding is necessary
  if (typeid(*session->second.type)==typeid(MpsLocalRecType))
    return TypeCheckRec(Theta,Gamma, Delta, Sigma, Omega, *this, session->first);
  if (typeid(*session->second.type)==typeid(MpsLocalForallType))
    return TypeCheckForall(Theta,Gamma, Delta, Sigma, Omega, *this, session->first);
  // Check session has receive type
  MpsLocalRcvType *typeptr = dynamic_cast<MpsLocalRcvType*>(session->second.type);
  if (typeptr==NULL)
    return PrintTypeError((string)"Receiving on session: " + myChannel.GetName(),*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check channel index is correct
  if (myChannel.GetIndex() != typeptr->GetChannel())
    return PrintTypeError((string)"Receiving on session(wrong index): " + myChannel.ToString(),*this,Theta,Gamma,Delta,Sigma,Omega);
  // Is renaming of myDest necessary?
  bool rename = false;
  if (typeptr->GetAssertionType())
  { set<string> fv=Theta.FV();
    if (fv.find(myDest)!=fv.end())
      rename=true;
    for (MpsLocalEnv::const_iterator it=Delta.begin(); (not rename) && it!=Delta.end(); ++it)
    { fv = it->second.type->FEV();
      if (fv.find(myDest)!=fv.end())
        rename=true;
    }
  }
  string newDest=rename?MpsExp::NewVar(myDest):myDest;
  // Make new Delta
  MpsLocalEnv newDelta;
  for (MpsLocalEnv::const_iterator it=Delta.begin(); it!=Delta.end(); ++it)
  { newDelta[it->first].pid=it->second.pid;
    newDelta[it->first].maxpid=it->second.maxpid;
    if (it->first==myChannel.GetName())
    { if (typeptr->GetAssertionType())
      { MpsLocalType *tmpType=NULL;
        if (rename && typeptr->GetAssertionName()!=myDest) // Rename in succ
          tmpType=typeptr->GetSucc()->ERename(myDest,newDest);
        else
          tmpType=typeptr->GetSucc()->Copy();
        newDelta[it->first].type=tmpType->ERename(typeptr->GetAssertionName(),myDest);
      }
      else
        newDelta[it->first].type=typeptr->GetSucc()->Copy();
    }
    else
    { if (rename)
        newDelta[it->first].type=it->second.type->ERename(myDest,newDest);
      else
        newDelta[it->first].type=it->second.type;
    }
  }
  // Save created type for deletion
  MpsLocalType *newType=newDelta[myChannel.GetName()].type;
  // Create new Gamma (and Sigma)
  MpsGlobalEnv newGamma = Gamma;
  MpsMsgEnv newSigma = Sigma;
  // Check if assertion domain is respected
  if (typeptr->GetAssertionType() && typeid(*typeptr->GetMsgType()) != typeid(MpsBoolMsgType))
    return PrintTypeError((string)"Assertion of non-boolean type: " + typeptr->ToString("!!!!!      "),*this,Theta,Gamma,Delta,Sigma,Omega);

  // If simple type or tuple add to Sigma
  if (typeid(*typeptr->GetMsgType()) == typeid(MpsIntMsgType) ||
      typeid(*typeptr->GetMsgType()) == typeid(MpsStringMsgType) ||
      typeid(*typeptr->GetMsgType()) == typeid(MpsBoolMsgType) ||
      typeid(*typeptr->GetMsgType()) == typeid(MpsTupleMsgType))
  {
    newSigma[myDest]=typeptr->GetMsgType();
  }
  // If channel, add to Gamma
  else if (typeid(*typeptr->GetMsgType()) == typeid(MpsChannelMsgType))
  {
    MpsChannelMsgType *mtptr = (MpsChannelMsgType*)typeptr->GetMsgType();
    newGamma[myDest] = mtptr->GetGlobalType();
  }
  // If delegation then add delegated session
  else if (dynamic_cast<MpsDelegateMsgType*>(typeptr->GetMsgType())!=NULL)
  {
    // Check that no unfinished session is replaced
    MpsLocalEnv::const_iterator oldsession = newDelta.find(myDest);
    if (oldsession != newDelta.end() && typeid(*oldsession->second.type)!=typeid(MpsLocalEndType))
      return PrintTypeError((string)"Overwriting open session: " + myDest,*this,Theta,Gamma,Delta,Sigma,Omega);
    MpsDelegateMsgType *mtptr = dynamic_cast<MpsDelegateMsgType*>(typeptr->GetMsgType());
    newDelta[myDest].type = mtptr->GetLocalType();
    newDelta[myDest].pid = mtptr->GetPid();
    newDelta[myDest].maxpid = mtptr->GetMaxpid();
  }
  else // Unknown MsgType
    return PrintTypeError((string)"Unknown Message-Type: " + typeptr->GetMsgType()->ToString(),*this,Theta,Gamma,Delta,Sigma,Omega);
  // Create new Assumptions
  MpsExp *newTheta=NULL;
  if (typeptr->GetAssertionType())
  { MpsExp *newAssertion=NULL;
    if (typeptr->GetAssertionName()!=myDest)
    { MpsExp *tmpAssertion=NULL;
      if (rename)
        tmpAssertion=typeptr->GetAssertion().Rename(myDest,newDest);
      else
        tmpAssertion=typeptr->GetAssertion().Copy();
      newAssertion=tmpAssertion->Rename(typeptr->GetAssertionName(),myDest);
      delete tmpAssertion;
    }
    else
      newAssertion=typeptr->GetAssertion().Copy();
    newTheta=new MpsBinOpExp("and",Theta,*newAssertion);
    delete newAssertion;
  }
  else
    newTheta=Theta.Copy();
  // Check rest of program
  bool result = mySucc->TypeCheck(*newTheta,newGamma,newDelta,newSigma,Omega);
  // Clean Up
  delete newTheta;
  if (rename)
  {
    while (newDelta.size()>0)
    { delete newDelta.begin()->second.type;
      newDelta.erase(newDelta.begin());
    }
  }
  else
    delete newType;

  return result;
} // }}}
bool MpsSelect::TypeCheck(const MpsExp &Theta, const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma, const MpsProcEnv &Omega) // Use rule Sel {{{
{
  MpsLocalEnv::const_iterator session=Delta.find(myChannel.GetName());
  // Check session is open
  if (session==Delta.end())
  {
    return PrintTypeError((string)"Typechecking error - Selecting on closed session: " + myChannel.GetName(),*this,Theta,Gamma,Delta,Sigma,Omega);
  }
  // Check if unfolding is necessary
  if (typeid(*session->second.type)==typeid(MpsLocalRecType))
    return TypeCheckRec(Theta,Gamma, Delta, Sigma, Omega, *this, session->first);
  if (typeid(*session->second.type)==typeid(MpsLocalForallType))
    return TypeCheckForall(Theta,Gamma, Delta, Sigma, Omega, *this, session->first);
  // Check session has select type
  MpsLocalSelectType *typeptr = dynamic_cast<MpsLocalSelectType*>(session->second.type);
  if (typeptr==NULL)
    return PrintTypeError((string)"Typechecking error - Selecting on session: " + myChannel.GetName() + "with type: " + session->second.type->ToString("           "),*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check channel index is correct
  if (myChannel.GetIndex() != typeptr->GetChannel())
    return PrintTypeError((string)"Typechecking error - Sending on session: " + myChannel.ToString() + "with type: " + typeptr->ToString("           "),*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check label ok
  map<string,MpsLocalType*>::iterator branch=typeptr->GetBranches().find(myLabel);
  if (branch==typeptr->GetBranches().end())
    return PrintTypeError((string)"Typechecking error - Sending label: " + myLabel + "on session: " + myChannel.ToString() + "with type: " + typeptr->ToString("           "),*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check label is active (assertion valid)
  vector<const MpsExp*> hyps;
  hyps.push_back(&Theta);
  map<string,MpsExp*>::const_iterator assertion=typeptr->GetAssertions().find(myLabel);
  if (assertion==typeptr->GetAssertions().end())
    return PrintTypeError((string)"Label has no assertion",*this,Theta,Gamma,Delta,Sigma,Omega);
  if (not assertion->second->ValidExp(hyps))
    return PrintTypeError((string)"Assertion not respected",*this,Theta,Gamma,Delta,Sigma,Omega);
  
  // Make new environment
  MpsLocalEnv newDelta = Delta;
  newDelta[myChannel.GetName()].type = branch->second;

  // Check rest of program
  return mySucc->TypeCheck(Theta,Gamma,newDelta,Sigma,Omega);
} // }}}
bool MpsBranch::TypeCheck(const MpsExp &Theta, const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma, const MpsProcEnv &Omega) // Use rule Branch {{{
{
  MpsLocalEnv::const_iterator session=Delta.find(myChannel.GetName());
  // Check session is open
  if (session==Delta.end())
    return PrintTypeError((string)"Branching on closed session " + myChannel.GetName(),*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check if unfolding is necessary
  if (typeid(*session->second.type)==typeid(MpsLocalRecType))
    return TypeCheckRec(Theta,Gamma, Delta, Sigma, Omega, *this, session->first);
  if (typeid(*session->second.type)==typeid(MpsLocalForallType))
    return TypeCheckForall(Theta,Gamma, Delta, Sigma, Omega, *this, session->first);
  // Check session has branch type
  MpsLocalBranchType *typeptr = dynamic_cast<MpsLocalBranchType*>(session->second.type);
  if (typeptr==NULL)
    return PrintTypeError((string)"Branching on non-branch session: " + myChannel.GetName(),*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check channel index is correct
  if (myChannel.GetIndex() != typeptr->GetChannel())
    return PrintTypeError((string)"Branching on wrong index: " + myChannel.ToString(),*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check label ok
  map<string,MpsLocalType*> &branches=typeptr->GetBranches();
  for (map<string,MpsLocalType*>::const_iterator branch=branches.begin();branch!=branches.end();++branch)
  {
    MpsLocalEnv newDelta = Delta;
    newDelta[myChannel.GetName()].type = branch->second;
    map<string,MpsTerm*>::iterator succ = myBranches.find(branch->first);
    if (succ==myBranches.end())
      return PrintTypeError((string)"Branching cannot receive label: " + branch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
    // Make new Theta
    map<string,MpsExp*>::const_iterator assertion=typeptr->GetAssertions().find(branch->first);
    if (assertion==typeptr->GetAssertions().end())
      return PrintTypeError((string)"Branch has no assertion: " + branch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
    MpsExp *newTheta = new MpsBinOpExp("and",Theta,*assertion->second);
    // Typecheck Branch
    bool brcheck=succ->second->TypeCheck(*newTheta,Gamma,newDelta,Sigma,Omega);
    delete newTheta;
    if (not brcheck)
      return false;
  }
  // All checks passed
  return true;
} // }}}
bool MpsPar::TypeCheck(const MpsExp &Theta, const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma, const MpsProcEnv &Omega) // Use rule Par {{{
{
  // Split Delta
  MpsLocalEnv newDelta1;
  newDelta1.clear();
  MpsLocalEnv newDelta2;
  newDelta2.clear();
  set<string> leftSessions = myLeft->FEV();
  for (MpsLocalEnv::const_iterator session=Delta.begin(); session!=Delta.end(); ++session)
  {
    if (leftSessions.find(session->first)!=leftSessions.end())
      newDelta1[session->first]=session->second;
    else
      newDelta2[session->first]=session->second;
  }

  // Check each sub-process with the split Deltas
  return myLeft->TypeCheck(Theta,Gamma,newDelta1,Sigma,Omega) && myRight->TypeCheck(Theta,Gamma,newDelta2,Sigma,Omega);
} // }}}
bool MpsDef::TypeCheck(const MpsExp &Theta, const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma, const MpsProcEnv &Omega) // * Use rule Def {{{
{
  // Check if def is sound
  if (myArgs.size() != myTypes.size())
    return PrintTypeError((string)"Bad def: difference in number of arguments and number of types",*this,Theta,Gamma,Delta,Sigma,Omega);
  if (myStateArgs.size() != myStateTypes.size())
    return PrintTypeError((string)"Bad def: difference in number of state arguments and number of state types",*this,Theta,Gamma,Delta,Sigma,Omega);
  // Make new process-environment
  MpsProcEnv newOmega = Omega;
  newOmega[myName].types = myTypes;
  newOmega[myName].snames = myStateArgs;
  for (vector<MpsMsgType*>::const_iterator it=myStateTypes.begin(); it!=myStateTypes.end(); ++it)
    if (dynamic_cast<const MpsBoolMsgType*>(*it)==NULL)
      return PrintTypeError((string)"State argument of unsupported type: " + (*it)->ToString(),*this,Theta,Gamma,Delta,Sigma,Omega);
  newOmega[myName].stypes = myStateTypes;
  // Make new environments for body
  MpsMsgEnv newSigma = Sigma;
  MpsGlobalEnv newGamma=Gamma;
  MpsLocalEnv newDelta;
  newDelta.clear();
  // Update environments from state-arguments
  set<string> usedArgs;
  for (int i=0; i<myStateArgs.size() /* && i<myStateTypes.size()*/; ++i)
  { // Only type bool already verified
    usedArgs.insert(myStateArgs[i]);
    newSigma[myStateArgs[i]]=myStateTypes[i];
  }
  // Update environments from arguments
  for (int i=0; i<myArgs.size() /* && i<myTypes.size()*/; ++i)
  {
    if (typeid(*myTypes[i]) == typeid(MpsChannelMsgType))
    {
      if (usedArgs.find(myArgs[i]) != usedArgs.end())
        return PrintTypeError((string)"Bad def: cannot type multiple input of channel: " + myArgs[i],*this,Theta,Gamma,Delta,Sigma,Omega);
      MpsChannelMsgType *type = (MpsChannelMsgType*)myTypes[i];
      newGamma[myArgs[i]] = type->GetGlobalType();
      usedArgs.insert(myArgs[i]);
    }
    else if (dynamic_cast<MpsDelegateMsgType*>(myTypes[i])!=NULL)
    {
      MpsDelegateMsgType *type = dynamic_cast<MpsDelegateMsgType*>(myTypes[i]);
      if (usedArgs.find(myArgs[i]) != usedArgs.end())
        return PrintTypeError((string)"Bad def: cannot type multiple input of session: " + myArgs[i],*this,Theta,Gamma,Delta,Sigma,Omega);
      newDelta[myArgs[i]].type = type->GetLocalType();
      newDelta[myArgs[i]].pid = type->GetPid();
      newDelta[myArgs[i]].maxpid = type->GetMaxpid();
      usedArgs.insert(myArgs[i]);
    }
    else
    {
      if (usedArgs.find(myArgs[i]) != usedArgs.end())
        return PrintTypeError((string)"Bad def: cannot type multiple input of channel: " + myArgs[i],*this,Theta,Gamma,Delta,Sigma,Omega);
      newSigma[myArgs[i]]=myTypes[i];
      usedArgs.insert(myArgs[i]);
    }
  }
  // Make subcalls
  return mySucc->TypeCheck(Theta,Gamma,Delta,Sigma,newOmega) && myBody->TypeCheck(Theta,newGamma,newDelta,newSigma,newOmega);
} // }}}
bool MpsCall::TypeCheck(const MpsExp &Theta, const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma, const MpsProcEnv &Omega) // * Use rule Var {{{
{
  // Check variable is defined
  MpsProcEnv::const_iterator omega = Omega.find(myName);
  if (omega == Omega.end())
    return PrintTypeError((string)"Process Variable not defined: " + myName,*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check correct number of arguments
  if (omega->second.stypes.size() != myState.size() ||
      omega->second.stypes.size() != myState.size() ||
      omega->second.types.size() != myArgs.size())
    return PrintTypeError((string)"Process Variable wrong argument-count: " + myName,*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check argument-types and remove used sessions from endDelta
  MpsLocalEnv endDelta=Delta;
  for (int i=0;i<myState.size();++i)
  {
    MpsMsgType *statetype = myState[i]->TypeCheck(Gamma, Delta, Sigma);
#if APIMS_DEBUG_LEVEL>99
    cerr << ">>>>Comparing: " << endl
         << ">>>>>>>>Theta: " << Theta.ToString() << endl
         << ">>>>>>>>>>LHS: " << statetype->ToString(">>>>>>>>>>>>>>") << endl
         << ">>>>>>>>>>RHS: " << omega->second.stypes[i]->ToString(">>>>>>>>>>>>>>") << endl;
#endif
    bool statetypematch = statetype->Equal(Theta,*omega->second.stypes[i]);
    delete statetype;    
    if (!statetypematch)
      return PrintTypeError((string)"State argument does not have type: " + omega->second.stypes[i]->ToString(),*this,Theta,Gamma,Delta,Sigma,Omega);
  }
  for (int i=0;i<myArgs.size();++i)
  {
    MpsMsgType *argType = myArgs[i]->TypeCheck(Gamma,endDelta,Sigma);
    MpsMsgType *callType = omega->second.types[i]->Copy();
    for (int j=0; j<myState.size(); ++j)
    {
      MpsMsgType *tmpType=callType->ESubst(omega->second.snames[j],*myState[j]);
      delete callType;
      callType=tmpType;
    }
#if APIMS_DEBUG_LEVEL>99
    cerr << ">>>>Comparing: " << endl
         << ">>>>>>>>Theta: " << Theta.ToString() << endl
         << ">>>>>>>>>>LHS: " << argType->ToString(">>>>>>>>>>>>>>") << endl
         << ">>>>>>>>>>RHS: " << callType->ToString(">>>>>>>>>>>>>>") << endl;
#endif
    bool argtypematch = argType->Equal(Theta,*callType);
    string callTypeString = callType->ToString();
    delete argType;
    delete callType;
    if (not argtypematch)
      return PrintTypeError((string)"Argument does not have type: " + callTypeString,*this,Theta,Gamma,Delta,Sigma,Omega);
    if (dynamic_cast<MpsDelegateMsgType*>(omega->second.types[i]) != NULL)
    {
      if (typeid(*myArgs[i]) != typeid(MpsVarExp))
        return PrintTypeError((string)"Argument must be session: " + myArgs[i]->ToString(),*this,Theta,Gamma,Delta,Sigma,Omega);
      else
      {
        MpsVarExp *var=(MpsVarExp*)myArgs[i];
        MpsLocalEnv::iterator session=endDelta.find(var->ToString());
        if (session == endDelta.end())
          return PrintTypeError((string)"Argument session used more than once: " + var->ToString(),*this,Theta,Gamma,Delta,Sigma,Omega);
        // Remove the used session (linearity)
        endDelta.erase(session);
      }
    }
  }
  // Check that endDelta is completed
  for (MpsLocalEnv::const_iterator session=endDelta.begin();session!=endDelta.end();++session)
    if (typeid(*session->second.type)!=typeid(MpsLocalEndType))
      return PrintTypeError((string)"Unfinished Session: " + session->first,*this,Theta,Gamma,Delta,Sigma,Omega);
  return true;
} // }}}
bool MpsNu::TypeCheck(const MpsExp &Theta, const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma, const MpsProcEnv &Omega) // Use rule Nres {{{
{
  // Check that only completed sessions are hidden
  MpsLocalEnv newDelta = Delta;
  MpsLocalEnv::iterator session=newDelta.find(myChannel);
  if (session!=newDelta.end())
  {
    if (typeid(*session->second.type) != typeid(MpsLocalEndType))
      return PrintTypeError((string)"Hiding uncompleted session:" + myChannel,*this,Theta,Gamma,Delta,Sigma,Omega);
    // Remove hidden session
    newDelta.erase(session);
  }
  // Add channel to environment
  MpsGlobalEnv newGamma = Gamma;
  // FIXME: Check that myType is linear and coherent
  newGamma[myChannel] = myType;
  return mySucc->TypeCheck(Theta,newGamma,newDelta,Sigma,Omega);
} // }}}
bool MpsLink::TypeCheck(const MpsExp &Theta, const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma, const MpsProcEnv &Omega) // * Use rules Mcast and Macc {{{
{
  // Check that only completed sessions are hidden
  MpsLocalEnv::const_iterator session=Delta.find(mySession);
  if (session!=Delta.end() && dynamic_cast<const MpsLocalEndType*>(session->second.type)==NULL)
    return PrintTypeError((string)"Linking on open session:" + mySession,*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check linking on available channel
  MpsGlobalEnv::const_iterator channel=Gamma.find(myChannel);
  if (channel==Gamma.end())
    return PrintTypeError((string)"Linking on closed channel:" + myChannel,*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check correct maxpid
  if (myMaxpid < channel->second->GetMaxPid())
    return PrintTypeError((string)"MaxPID cannot be smaller than:" + int2string(channel->second->GetMaxPid()),*this,Theta,Gamma,Delta,Sigma,Omega);
  // Create local type
  MpsLocalType *newType=channel->second->Project(myPid);
  set<string> fv = newType->FEV();
  // Rename all free variables
  for (set<string>::const_iterator it=fv.begin(); it!=fv.end(); ++it)
  { MpsLocalType *tmpType=newType->ERename(*it,MpsExp::NewVar(*it));
    delete newType;
    newType=tmpType;
  }
  // Create Delta with new session
  MpsLocalEnv newDelta = Delta;
  newDelta[mySession].type = newType;
  newDelta[mySession].pid = myPid;
  newDelta[mySession].maxpid = myMaxpid;
  bool result=mySucc->TypeCheck(Theta,Gamma,newDelta,Sigma,Omega);

  // Clean up
  delete newType;

  return result;
} // }}}
bool MpsSync::TypeCheck(const MpsExp &Theta, const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma, const MpsProcEnv &Omega) // Use rule Sync {{{
{
  MpsLocalEnv::const_iterator session=Delta.find(mySession);
  // Check that session is open
  if (session==Delta.end())
    return PrintTypeError((string)"Synchonising on closed session " + mySession,*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check if unfolding is necessary
  if (typeid(*session->second.type)==typeid(MpsLocalRecType))
    return TypeCheckRec(Theta,Gamma, Delta, Sigma, Omega, *this, session->first);
  if (typeid(*session->second.type)==typeid(MpsLocalForallType))
    return TypeCheckForall(Theta,Gamma, Delta, Sigma, Omega, *this, session->first);
  // Check session has sync type
  MpsLocalSyncType *typeptr = dynamic_cast<MpsLocalSyncType*>(session->second.type);
  if (typeptr==NULL)
    return PrintTypeError((string)"Synchronising on non-sync session: " + mySession,*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check maxpid
  if (myMaxpid != session->second.maxpid)
    return PrintTypeError((string)"Synchronising with wrong participant count",*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check if mandatory labels are accepted
  map<string,MpsLocalType*> &branches=typeptr->GetBranches();
  map<string,MpsExp*> &assertions=typeptr->GetAssertions();
  vector<const MpsExp*> hyps;
  hyps.push_back(&Theta);
  MpsExp *mandatoryOr=new MpsBoolVal(false); // FIXME: This is necessary because Global Type Validity Check is Missing
  for (map<string,MpsLocalType*>::const_iterator branch=branches.begin();branch!=branches.end();++branch)
  {
    if (branch->first[0]=='^') // Mandatory branch
    {
      map<string,MpsTerm*>::const_iterator myBranch=myBranches.find(branch->first);
      map<string,MpsExp*>::const_iterator assertion=assertions.find(branch->first);
      if (assertion==assertions.end())
        return PrintTypeError((string)"Synchronisation type has no assertion for branch: " + branch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
      // FIXME: mandatoryOr
      MpsExp *tmpOr = new MpsBinOpExp("or",*mandatoryOr,*assertion->second);
      delete mandatoryOr;
      mandatoryOr=tmpOr;
      // Check Label Inclusion
      if (myBranch == myBranches.end())
      { MpsExp *notAssertion = new MpsUnOpExp("not",*assertion->second); // Do not require inactive branches
        bool inactive = notAssertion->ValidExp(hyps);
        delete notAssertion;
        if (not inactive)
          return PrintTypeError((string)"Synchronisation missing mandatory branch: " + branch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
      }
      else
      {
        // Check Assertion Implication
        map<string,MpsExp*>::const_iterator myAssertion=myAssertions.find(branch->first);
        if (myAssertion==myAssertions.end())
          return PrintTypeError((string)"Synchronisation process has no assertion for branch: " + branch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
        MpsExp *notAssertion = new MpsUnOpExp("not",*assertion->second);
        MpsExp *implication = new MpsBinOpExp("or",*notAssertion,*myAssertion->second);
        delete notAssertion;
        bool checkImplication=implication->ValidExp(hyps);
        delete implication;
        if (not checkImplication)
          return PrintTypeError((string)"Synchronisation may not accept mandatory branch: " + branch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
      }
    }
  }
  // FIXME: mandatoryOr
  bool checkMandatory=mandatoryOr->ValidExp(hyps);
  delete mandatoryOr;
  if (not checkMandatory)
    return PrintTypeError((string)"Synchronisation may have no mandatory branches: " + mySession,*this,Theta,Gamma,Delta,Sigma,Omega);

  // Check typing of all branches in the process
  for (map<string,MpsTerm*>::const_iterator branch=myBranches.begin();branch!=myBranches.end();++branch)
  {
    // Check Label Inclusion
    map<string,MpsLocalType*>::const_iterator type=branches.find(branch->first);
    if (type==branches.end())
      return PrintTypeError((string)"Synchronisation accepts untyped label: " + branch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
    // TypeCheck Assertion
    map<string,MpsExp*>::const_iterator myAssertion=myAssertions.find(branch->first);
    if (myAssertion==myAssertions.end())
      return PrintTypeError((string)"Synchronisation process has no assertion for branch: " + branch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
    MpsMsgType *assertionType=myAssertion->second->TypeCheck(Gamma,Delta,Sigma);
    bool checkAssertionType = dynamic_cast<MpsBoolMsgType*>(assertionType)!=NULL;
    delete assertionType;
    if (not checkAssertionType)
      return PrintTypeError((string)"Synchronisation has untyped assertion for branch: " + branch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
    // Check Assertion Implication
    map<string,MpsExp*>::const_iterator assertion=assertions.find(branch->first);
    if (assertion==assertions.end())
        return PrintTypeError((string)"Synchronisation type has no assertion for branch: " + branch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
    MpsExp *notAssertion = new MpsUnOpExp("not",*myAssertion->second);
    MpsExp *implication = new MpsBinOpExp("or",*notAssertion,*assertion->second);
    delete notAssertion;
    bool checkImplication = implication->ValidExp(hyps);
    delete implication;
    if (not checkImplication)
        return PrintTypeError((string)"Synchronisation may accept inactive branch: " + branch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
    // Make new Delta
    MpsLocalEnv newDelta = Delta;
    newDelta[mySession].type = type->second;
    // Make new Theta
    MpsExp *newTheta=new MpsBinOpExp("and",Theta,*myAssertion->second);
    // Check Branch
    bool checkBranch=branch->second->TypeCheck(*newTheta,Gamma,newDelta,Sigma,Omega);
    delete newTheta;
    if (not checkBranch)
      return false;
  }
  // All checks passed
  return true;
} // }}}
bool MpsCond::TypeCheck(const MpsExp &Theta, const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma, const MpsProcEnv &Omega) // Use rule Cond {{{
{
  MpsBoolMsgType booltype;
  MpsMsgType *condtype = myCond->TypeCheck(Gamma,Delta,Sigma);
  bool condtypematch = booltype.Equal(Theta,*condtype);
  delete condtype;
  if (!condtypematch)
    return PrintTypeError("Condition not of type Bool",*this,Theta,Gamma,Delta,Sigma,Omega);
  // Make new Thetas
  MpsExp *trueTheta = new MpsBinOpExp("and",Theta,*myCond);;
  MpsExp *notCond = new MpsUnOpExp("not",*myCond);
  MpsExp *falseTheta = new MpsBinOpExp("and",Theta,*notCond);
  delete notCond;
  
  bool result = myTrueBranch->TypeCheck(*trueTheta,Gamma,Delta,Sigma,Omega)
             && myFalseBranch->TypeCheck(*falseTheta,Gamma,Delta,Sigma,Omega);
  // Clean Up
  delete trueTheta;
  delete falseTheta;

  return result;
} // }}}
bool MpsGuiSync::TypeCheck(const MpsExp &Theta, const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma, const MpsProcEnv &Omega) // * Use rule Sync (extended) {{{
{
  MpsLocalEnv::const_iterator session=Delta.find(mySession);
  // Check that session is open
  if (session==Delta.end())
    return PrintTypeError((string)"Synchonising on closed session " + mySession,*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check if unfolding is necessary
  if (typeid(*session->second.type)==typeid(MpsLocalRecType))
    return TypeCheckRec(Theta,Gamma, Delta, Sigma, Omega, *this, session->first);
  if (typeid(*session->second.type)==typeid(MpsLocalForallType))
    return TypeCheckForall(Theta,Gamma, Delta, Sigma, Omega, *this, session->first);
  // Check session has sync type
  MpsLocalSyncType *typeptr = dynamic_cast<MpsLocalSyncType*>(session->second.type);
  if (typeptr==NULL)
    return PrintTypeError((string)"Synchronising on non-sync session: " + mySession,*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check pid and maxpid
  if (session->second.pid!=myPid)
    return PrintTypeError((string)"guisync gives wrong pid for session: " + mySession,*this,Theta,Gamma,Delta,Sigma,Omega);
  if (session->second.maxpid!=myMaxpid)
    return PrintTypeError((string)"guisync gives wrong maxpid for sessoin: " + mySession,*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check if mandatory labels are accepted
  map<string,MpsLocalType*> &branches=typeptr->GetBranches();
  map<string,MpsExp*> &assertions=typeptr->GetAssertions();
  vector<const MpsExp*> hyps;
  hyps.push_back(&Theta);
  MpsExp *mandatoryOr=new MpsBoolVal(false); // FIXME: This is necessary because Global Type Validity Check is Missing
  for (map<string,MpsLocalType*>::const_iterator branch=branches.begin();branch!=branches.end();++branch)
  {
    if (branch->first[0]=='^') // Mandatory branch
    {
      map<string,inputbranch>::const_iterator myBranch=myBranches.find(branch->first);
      map<string,MpsExp*>::const_iterator assertion=assertions.find(branch->first);
      if (assertion==assertions.end())
        return PrintTypeError((string)"Synchronisation type has no assertion for branch: " + branch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
      if (assertion==assertions.end())
        return PrintTypeError((string)"Synchronisation type has no assertion for branch: " + branch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
      // FIXME: mandatoryOr
      MpsExp *tmpOr = new MpsBinOpExp("or",*mandatoryOr,*assertion->second);
      delete mandatoryOr;
      mandatoryOr=tmpOr;
      // Check Label Inclusion
      if (myBranch == myBranches.end())
      { MpsExp *notAssertion = new MpsUnOpExp("not",*assertion->second); // Do not require inactive branches
        bool inactive = notAssertion->ValidExp(hyps);
        delete notAssertion;
        if (not inactive)
          return PrintTypeError((string)"Synchronisation missing mandatory branch: " + branch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
      }
      else
      {
        // Check Assertion Implication
        MpsExp *notAssertion = new MpsUnOpExp("not",*assertion->second);
        MpsExp *implication = new MpsBinOpExp("or",*notAssertion,*myBranch->second.assertion);
        delete notAssertion;
        bool checkImplication=implication->ValidExp(hyps);
        delete implication;
        if (not checkImplication)
          return PrintTypeError((string)"Synchronisation may not accept mandatory branch: " + branch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
      }
    }
  }
  // FIXME: mandatoryOr
  bool checkMandatory=mandatoryOr->ValidExp(hyps);
  delete mandatoryOr;
  if (not checkMandatory)
    return PrintTypeError((string)"Synchronisation may have no mandatory branches: " + mySession,*this,Theta,Gamma,Delta,Sigma,Omega);

  // Check typing of all branches
  for (map<string,inputbranch>::const_iterator myBranch=myBranches.begin();myBranch!=myBranches.end();++myBranch)
  {
    // Typecheck arguments
    for (int brancharg=0; brancharg<myBranch->second.args.size(); ++brancharg)
    {
      MpsMsgType *branchargtype = myBranch->second.values[brancharg]->TypeCheck(Gamma, Delta, Sigma);
      bool branchargtypematch = branchargtype->Equal(Theta,*myBranch->second.types[brancharg]);
      delete branchargtype;
      if (!branchargtypematch)
        return PrintTypeError((string)"Ill typed argument: " + myBranch->second.args[brancharg] + " in branch: " + myBranch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
    }
    // Check Label Inclusion
    map<string,MpsLocalType*>::const_iterator type=branches.find(myBranch->first);
    if (type==branches.end())
      return PrintTypeError((string)"Synchronisation accepts untyped label: " + myBranch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
    // TypeCheck Assertion
    MpsMsgType *assertionType=myBranch->second.assertion->TypeCheck(Gamma,Delta,Sigma);
    bool checkAssertionType = dynamic_cast<MpsBoolMsgType*>(assertionType)!=NULL;
    delete assertionType;
    if (not checkAssertionType)
      return PrintTypeError((string)"Synchronisation has untyped assertion for branch: " + myBranch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
    // Check Assertion Implication
    map<string,MpsExp*>::const_iterator assertion=assertions.find(myBranch->first);
    if (assertion==assertions.end())
        return PrintTypeError((string)"Synchronisation type has no assertion for branch: " + myBranch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
    MpsExp *notAssertion = new MpsUnOpExp("not",*myBranch->second.assertion);
    MpsExp *implication = new MpsBinOpExp("or",*notAssertion,*assertion->second);
    delete notAssertion;
    bool checkImplication = implication->ValidExp(hyps);
    delete implication;
    if (not checkImplication)
        return PrintTypeError((string)"Synchronisation may accept inactive branch: " + myBranch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
    // Make new Delta
    // Prepare renaming
    map<string,string> renaming;
    for (int brancharg=0; brancharg<myBranch->second.args.size(); ++brancharg)
    { string name=myBranch->second.args[brancharg];
      renaming[name]=MpsExp::NewVar(name);
    }
    MpsLocalEnv newDelta;
    // Rename args in Delta
    for (map<string,delta>::const_iterator it=Delta.begin(); it!=Delta.end(); ++it)
    { MpsLocalType *tmp1=NULL;
      if (it->first==mySession) // Use branch type for mySession
        tmp1 = type->second->Copy();
      else
        tmp1 = it->second.type->Copy();
      for (map<string,string>::const_iterator tr=renaming.begin(); tr!=renaming.end(); ++tr)
      { MpsLocalType *tmp2=tmp1->ERename(tr->first,tr->second);
        delete tmp1;
        tmp1=tmp2;
      }
      newDelta[it->first].type=tmp1;
      newDelta[it->first].pid=it->second.pid;
      newDelta[it->first].maxpid=it->second.maxpid;
    }
    // Make new Sigma
    MpsMsgEnv newSigma = Sigma;
    if (myBranch->second.args.size() != myBranch->second.types.size() ||
        myBranch->second.args.size() != myBranch->second.names.size())
      return PrintTypeError((string)"Number of arguments, types and names inconsistent in branch: " + myBranch->first,*this,Theta,Gamma,Delta,Sigma,Omega);
    for (int i=0; i<myBranch->second.args.size(); ++i)
      newSigma[myBranch->second.args[i] ] = myBranch->second.types[i]; // Only simple types
    // Make new Theta
    MpsExp *newTheta=new MpsBinOpExp("and",Theta,*myBranch->second.assertion);
    // Rename args in Theta
    for (map<string,string>::const_iterator tr=renaming.begin(); tr!=renaming.end(); ++tr)
    { MpsExp *tmpTheta=newTheta->Rename(tr->first,tr->second);
      delete newTheta;
      newTheta=tmpTheta;
    }
    // Check Branch
    bool checkBranch=myBranch->second.term->TypeCheck(*newTheta,Gamma,newDelta,newSigma,Omega);
    delete newTheta;
    while (newDelta.size()>0)
    { delete newDelta.begin()->second.type;
      newDelta.erase(newDelta.begin());
    }
    if (not checkBranch)
      return false;
  }
  // All checks passed
  return true;
} // }}}
bool MpsGuiValue::TypeCheck(const MpsExp &Theta, const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma, const MpsProcEnv &Omega) // Type check name, value and session {{{
{
  MpsStringMsgType stringtype;
  // Check label is a string
  MpsMsgType *nametype = myName->TypeCheck(Gamma,Delta,Sigma);
  bool nametypematch = stringtype.Equal(Theta,*nametype);
  delete nametype;
  if (!nametypematch)
    return PrintTypeError("Name for guivalue must be of type String",*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check ownership of session
  MpsLocalEnv::const_iterator session=Delta.find(mySession);
  // Check session is open
  if (session==Delta.end())
    return PrintTypeError((string)"guivalue on closed session: " + mySession,*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check if correct PID is given
  if (session->second.pid!=myPid)
    return PrintTypeError((string)"guivalue gives wrong pid for session: " + mySession,*this,Theta,Gamma,Delta,Sigma,Omega);
  if (session->second.maxpid!=myMaxpid)
    return PrintTypeError((string)"guivalue gives wrong maxpid for sessoin: " + mySession,*this,Theta,Gamma,Delta,Sigma,Omega);
  // Check that value is welltyped
  MpsMsgType *valType = myValue->TypeCheck(Gamma,Delta,Sigma);
  bool untyped = dynamic_cast<MpsMsgNoType*>(valType)!=NULL;
  delete valType;
  if (untyped)
    return PrintTypeError((string)"guivalue uses untyped expression: " + myValue->ToString(),*this,Theta,Gamma,Delta,Sigma,Omega);

  return mySucc->TypeCheck(Theta,Gamma,Delta,Sigma,Omega);
} // }}}
bool MpsAssign::TypeCheck(const MpsExp &Theta, const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma, const MpsProcEnv &Omega) // * Check exp has correct type, and check succ in updated sigma {{{
{
  if (dynamic_cast<MpsDelegateMsgType*>(myType)!=NULL)
    return PrintTypeError("Assignment type cannot be a session, because it breaks linearity",*this,Theta,Gamma,Delta,Sigma,Omega);
  MpsMsgType *exptype=myExp->TypeCheck(Gamma,Delta,Sigma);
  bool exptypematch = exptype->Equal(Theta,*myType);
  delete exptype;
  if (not exptypematch)
    return PrintTypeError((string)"Expression does not have type: " + myType->ToString(),*this,Theta,Gamma,Delta,Sigma,Omega);
  // Make new Successor
  // FIXME: Better to change environments, but requires a lot of renaming and assertions of the type myId=myExp
  MpsTerm *newSucc = mySucc->ESubst(myId,*myExp);
  // Check new Successor
  bool result = newSucc->TypeCheck(Theta,Gamma,Delta,Sigma,Omega);
  // Clean up
  delete newSucc;

  return result;
} // }}}

/* Compile to C code
 */
int _compile_id;
string Compile_NewVar(string base) // {{{
{
  string result = (string)"_"+base;
  result += int2string(_compile_id++);
  return result;
} // }}}
string MpsTerm::Compile () // {{{
{ _compile_id=1;
  string decls;
  string defs;
  string main;
  Compile(decls,defs,main);
  return (string) "/* Procedure declerations */"
                + decls
                + "\n\n/* Procedure definitions */"
                + defs
                + "\n\n/* Main process */\nint main()\n{"
                + main
                + "\n}";
} // }}}
void MpsEnd::Compile(std::string &decls, string &defs, string &main) // Use rule Inact {{{
{
  main += "\n  return 0;";
  return;
} // }}}
void MpsSnd::Compile(std::string &decls, string &defs, string &main) // Use rules Send and Deleg (and new rule for delegaing the session itself) {{{
{
  main += "\n  MsgSend(" + myChannel.ToString() + ", " + myExp->ToString() + ");";
  mySucc->Compile(decls,defs,main);
  return;
} // }}}
void MpsRcv::Compile(std::string &decls, string &defs, string &main) // Use rules Rcv and Srec {{{
{
  string var = Compile_NewVar(myDest);
  main += "\n string " + var + ";\n  MsgRcv(" + myChannel.ToString() + ", &" + var + ");";
  MpsTerm *tmpSucc = mySucc->ERename(myDest,var);
  tmpSucc->Compile(decls,defs,main);
  delete tmpSucc;
  return;
} // }}}
void MpsSelect::Compile(std::string &decls, string &defs, string &main) // Use rule Sel {{{
{
  main += "\n  MsgSend(" + myChannel.ToString() + ", \"" + myLabel + "\");";
  mySucc->Compile(decls,defs,main);
  return;
} // }}}
void MpsBranch::Compile(std::string &decls, string &defs, string &main) // Use rule Branch {{{
{
  string label=Compile_NewVar("label");
  main += "\n  string " + label + ";\n  MsgRcv(" + myChannel.ToString() + ", &" + label + ");";
  for (map<string,MpsTerm*>::const_iterator branch=myBranches.begin(); branch!=myBranches.end(); ++branch)
  { if (branch!=myBranches.begin())
      main += "\n  else";
    main += "\n  if (" + label + "==\"" + branch->first + "\")\n  {";
    branch->second->Compile(decls,defs,main);
    main += "\n  }";
  }
  main += "\n  else cerr << \"ERROR: UNEXPECTED LABEL \" << " + label + " << endl;\n  return -1;";
  return;
} // }}}
void MpsPar::Compile(std::string &decls, string &defs, string &main) // Use rule Par {{{
{
  main += "\n  if (fork()==0)\n  {";
  myLeft->Compile(decls,defs,main);
  main += "\n  }\n  else\n  {";
  myRight->Compile(decls,defs,main);
  main += "\n  }";
  return;
} // }}}
void MpsDef::Compile(std::string &decls, string &defs, string &main) // * Use rule Def {{{
{ // Fixme: include extra arguments (free variables in body)
  string name=Compile_NewVar("Proc");
  decls += "\nvoid " + name + "(";
  bool separator = false;
  for (int i=0; i<myStateArgs.size(); ++i)
  { if (separator)
      decls += ", ";
    else
      separator=true;
    decls += myStateTypes[i]->ToString();
    decls += " " + myStateArgs[i];
  }
  for (int i=0; i<myArgs.size(); ++i)
  { if (separator)
      decls += ", ";
    else
      separator=true;
    decls += myTypes[i]->ToString();
    decls += " " + myArgs[i];
  }
  decls += ");";
  string body;
  MpsTerm *tmpBody=myBody->PRename(myName,name);
  tmpBody->Compile(decls,defs,body);
  delete tmpBody;
  defs += "\nvoid " + name + "(";
  separator = false;
  for (int i=0; i<myStateArgs.size(); ++i)
  { if (separator)
      defs += ", ";
    else
      separator=true;
    defs += myStateTypes[i]->ToString();
    defs += " " + myStateArgs[i];
  }
  for (int i=0; i<myArgs.size(); ++i)
  { if (separator)
      defs += ", ";
    else
      separator=true;
    defs += myTypes[i]->ToString();
    defs += " " + myArgs[i];
  }
  defs += ")\n\{" + body + "\n}";
  MpsTerm *tmpSucc=mySucc->PRename(myName,name);
  tmpSucc->Compile(decls,defs,main);
  delete tmpSucc;
  return;
} // }}}
void MpsCall::Compile(std::string &decls, string &defs, string &main) // * Use rule Var {{{
{
  // FIXME: Make call
  main += "\n  return " + myName + "(";
  bool separator=false;
  for (int i=0; i<myState.size(); ++i)
  { if (separator)
      main +=", ";
    else
      separator=true;
    main += myState[i]->ToString();
  }
  for (int i=0; i<myArgs.size(); ++i)
  { if (separator)
      main +=", ";
    else
      separator=true;
    main += myArgs[i]->ToString();
  }
  main += ");";
  return;
} // }}}
void MpsNu::Compile(std::string &decls, string &defs, string &main) // Use rule Nres {{{
{
  // FIXME: Create queues
  mySucc->Compile(decls,defs,main);
  return;
} // }}}
void MpsLink::Compile(std::string &decls, string &defs, string &main) // * Use rules Mcast and Macc {{{
{
  // FIXME: Create queues
  mySucc->Compile(decls,defs,main);
  return;
} // }}}
void MpsSync::Compile(std::string &decls, string &defs, string &main) // Use rule Sync {{{
{
  // FIXME: Perform Synchronization
  return;
} // }}}
void MpsCond::Compile(std::string &decls, string &defs, string &main) // Use rule Cond {{{
{
  // FIXME: Create code
  return;
} // }}}
void MpsGuiSync::Compile(std::string &decls, string &defs, string &main) // * Use rule Sync (extended) {{{
{
  // FIXME: Create code
  return;
} // }}}
void MpsGuiValue::Compile(std::string &decls, string &defs, string &main) // Type check name, value and session {{{
{
  // FIXME: Create code
  mySucc->Compile(decls,defs,main);
  return;
} // }}}
void MpsAssign::Compile(std::string &decls, string &defs, string &main) // * Check exp has correct type, and check succ in updated sigma {{{
{
  // FIXME: Create code
  return;
} // }}}

/* Create list of possible steps
 */
// Remove unused function definitions
void CleanDefinitions(const MpsTerm *body, vector<MpsFunction> &env) // Remove unused functions from env {{{
{
  // Generate list of used functions
  set<string> usedDefs;
  set<string> bufDefs=body->FPV();
  while (bufDefs.size()>0)
  { string funName = *bufDefs.begin();
    bufDefs.erase(funName);
    usedDefs.insert(funName);
    vector<MpsFunction>::const_iterator fun=env.begin(); // Find definition
    while (fun!=env.end() && fun->GetName()!=funName) // FIXME: Linear search!
      ++fun;
    if (fun!=env.end())
    { // Add used definitions recursively (if new)
      set<string> tmp=fun->GetBody().FPV();
      for (set<string>::const_iterator it=tmp.begin(); it!=tmp.end(); ++it)
        if (usedDefs.count(*it)==0)
          bufDefs.insert(*it);
    }
  }
  // Remove unused functions
  vector<MpsFunction>::iterator fun=env.begin();
  while (fun!=env.end())
  { if (usedDefs.count(fun->GetName())==0)
      fun=env.erase(fun);
    else
      ++fun;
  }
} // }}}
// Find possible steps, and make step based on given choice
MpsTerm *MpsTerm::Step(MpsEnv &env, vector<MpsFunction> &defs, int &choice, int &choices) // {{{
{
  vector<MpsStep> steps;
  Steps(env,defs,steps);
  choices = steps.size();
  MpsTerm *result=NULL;
  if (choices>0)
  {
    if (choice<0 || choice>=choices) // Make random choice
      choice = rand()%choices;
    result = steps[choice].Apply(*this,env,defs);
    CleanDefinitions(result,defs);
  }
  else
    choice=-1;
  return result;
} // }}}
// Create all n-combinations of the elements
vector<vector<int> > combine(vector<int>::const_iterator first, vector<int>::const_iterator end, int n) // {{{
{ vector<vector<int> > result;
  if (n==0)
    result.push_back(vector<int>());
  if (first==end || n<=0 || end-first<n)
    return result;

  vector<vector<int> > with=combine(first+1,end,n-1);
  for (vector<vector<int> >::iterator it=with.begin(); it!=with.end(); ++it)
    it->push_back(*first);
  vector<vector<int> > without=combine(first+1,end,n);
  result.insert(result.end(),with.begin(),with.end());
  result.insert(result.end(),without.begin(),without.end());
  return result;
} // }}}
// Get list of partial steps from Substeps, and create list of complete steps
void MpsTerm::Steps(MpsEnv &env, const vector<MpsFunction> &defs, vector<MpsStep> &dest) // {{{
{
  bool first=true;
  bool wait=true;
  // Retry until steps found, or no gui dependance
  while (first || (wait && dest.size()==0 && mpsgui::GuiEnabeled()))
  { first=false;
    vector<MpsStep> substeps;
    substeps.clear();
    wait=SubSteps(substeps);
  
    vector<MpsStep> linksteps;
    vector<MpsStep> syncsteps;
    // Step 0: Traverse list and move complete steps to dest
    while (substeps.size()>0)
    {
      if (substeps.front().GetEvent().myType==link)
        linksteps.push_back(substeps.front());
      else if (substeps.front().GetEvent().myType==sync)
        syncsteps.push_back(substeps.front());
      else if (substeps.front().GetEvent().myType==rcv ||
               substeps.front().GetEvent().myType==brcv)
      { MpsEnv::const_iterator queue = env.find(substeps.front().GetEvent().myChannel);
        if (queue!=env.end() && queue->second.size()>0)
          dest.push_back(substeps.front());
      }
      else // tau or send step
        dest.push_back(substeps.front());
      substeps.erase(substeps.begin());
    }
    // Step 1a: Group link steps by (channel,maxpid)
    map<pair<string,int>,map<int,vector<int> > > links; // (channel,maxpid) -> pid -> index set
    links.clear();
    for (int i=0; i<linksteps.size(); ++i)
      links[pair<string,int>(linksteps[i].GetEvent().myChannel.ToString(),linksteps[i].GetEvent().myMaxpid)]
           [linksteps[i].GetEvent().myPid].push_back(i);
    // Step 1b: Collect complete link steps and add to dest
    for (map<pair<string,int>,map<int,vector<int> > >::iterator group=links.begin(); group!=links.end(); ++group) // foreach (channel,maxpid)
    { if (group->first.second>0) // Check maxpid>0
      { vector<vector<int> > combis; // Initialize groups as the possible 1st participants
        combis.clear();
        for (vector<int>::const_iterator p=group->second[1].begin();p!=group->second[1].end(); ++p) // for each participant with pid=1
        { vector<int> combi;
          combi.clear();
          combi.push_back(*p);
          combis.push_back(combi);
        }
        for (int pid=2; pid<=group->first.second; ++pid) // For each participant id 2..maxpid
        { vector<vector<int> > last_combis=combis;
          combis.clear();
          for (vector<vector<int> >::const_iterator c=last_combis.begin();c!=last_combis.end(); ++c) // For each existing group
          for (vector<int>::const_iterator group_pid=group->second[pid].begin(); group_pid!=group->second[pid].end(); ++group_pid) // for each pid'th participant
          { vector<int> new_combi=*c;          // Create group extended with
            new_combi.push_back(*group_pid);   // new participant
            combis.push_back(new_combi);
          }
        }
        // Add complete links to dest
        for (vector<vector<int> >::const_iterator fullgroup=combis.begin();fullgroup!=combis.end(); ++fullgroup) // For each existing group
        { MpsEvent event;
          event.myType=tau;
          event.mySubType=link;
          event.myChannel = MpsChannel(group->first.first);
          event.mySession = "session";
          event.myMaxpid=group->first.second;
          vector<string> paths;
          for (vector<int>::const_iterator p=fullgroup->begin(); p!=fullgroup->end(); ++p) // for each participant in group
            paths.push_back(linksteps[*p].GetPaths()[0]);
          dest.push_back(MpsStep(event,paths));
        }
      }
    }
    // clean up
    linksteps.clear();
    links.clear();
    // Step 2a: Group sync steps by (session,maxpid)
    map<pair<string,int>,map<string,vector<int> > > syncs; // (session,maxpid) -> label -> index set
    for (int i=0; i<syncsteps.size(); ++i)
      syncs[pair<string,int>(syncsteps[i].GetEvent().mySession,syncsteps[i].GetEvent().myMaxpid)]
           [syncsteps[i].GetEvent().myLabel].push_back(i);
    // Step 2b: Collect complete sync steps and add to dest
    for (map<pair<string,int>,map<string,vector<int> > >::iterator group=syncs.begin(); group!=syncs.end(); ++group) // foreach (channel,maxpid)
      for (map<string,vector<int> >::const_iterator choice=group->second.begin(); choice!=group->second.end(); ++choice) // foreach choice in group
      { vector<vector<int> > combis=combine(choice->second.begin(),choice->second.end(),group->first.second);
        for (vector<vector<int> >::const_iterator combi=combis.begin(); combi!=combis.end(); ++combi)
        { MpsEvent event;
          event.myType=tau;
          event.mySubType=sync;
          event.myChannel = MpsChannel(group->first.first);
          event.mySession=group->first.first;
          event.myMaxpid=group->first.second;
          event.myLabel=choice->first;
          vector<string> paths;
          for (vector<int>::const_iterator p=combi->begin(); p!=combi->end(); ++p) // for each participant in sync
            paths.push_back(syncsteps[*p].GetPaths()[0]);
          dest.push_back(MpsStep(event,paths));
        }
      }
    // clean up
    syncsteps.clear();
    syncs.clear();
  
    if (wait && dest.size()==0) // Wait on GUI decission
    {
      if (mpsgui::GuiEnabeled())
      {
#if APIMS_DEBUG_LEVEL>4
        cout << "*********** Waiting for GUI ***********" << endl;
#endif
        mpsgui::GuiWait();
#if APIMS_DEBUG_LEVEL>4
        cout << "*********** GUI Event *****************" << endl;
#endif
      }
      else
      {
#if APIMS_DEBUG_LEVEL>4
        cout << "******* Stuck: GUI not enabeled *******" << endl;
#endif
      }
    }
  }
} // }}}

/* Applying Steps
 */
MpsTerm *MpsTerm::ApplyRcv(const std::string &path, const MpsExp *val) const // {{{
{ return Error((string)"Applying Rcv on "+ToString());
} // }}}
MpsTerm *MpsPar::ApplyRcv(const std::string &path, const MpsExp *val) const // {{{
{ if (path.size()==0)
    return Error((string)"Applying Rcv on "+ToString());
  MpsTerm *left=NULL;
  MpsTerm *right=NULL;
  if (path[0]=='l') // left
  { left=myLeft->ApplyRcv(path.substr(1),val);
    right=myRight->Copy();
  }
  else // assume right
  { left=myLeft->Copy();
    right=myRight->ApplyRcv(path.substr(1),val);
  }
  MpsTerm *result = new MpsPar(*left,*right);
  delete left;
  delete right;
  return result;
} // }}}
MpsTerm *MpsRcv::ApplyRcv(const std::string &path, const MpsExp *val) const // {{{
{ if (path.size()>0)
    return Error((string)"Applying Rcv on "+ToString()+" with nonempty path "+path);
 return mySucc->ESubst(myDest,*val);
} // }}}
MpsTerm *MpsTerm::ApplySnd(const std::string &path, MpsExp **val, MpsChannel &ch) const // {{{
{ return Error((string)"Applying Snd on "+ToString());
} // }}}
MpsTerm *MpsPar::ApplySnd(const std::string &path, MpsExp **val, MpsChannel &ch) const // {{{
{ if (path.size()==0)
    return Error((string)"Applying Snd on "+ToString());
  MpsTerm *left=NULL;
  MpsTerm *right=NULL;
  if (path[0]=='l') // left
  { left=myLeft->ApplySnd(path.substr(1),val,ch);
    right=myRight->Copy();
  }
  else // assume right
  { left=myLeft->Copy();
    right=myRight->ApplySnd(path.substr(1),val,ch);
  }
  MpsTerm *result = new MpsPar(*left,*right);
  delete left;
  delete right;
  return result;
} // }}}
MpsTerm *MpsSnd::ApplySnd(const std::string &path, MpsExp **val, MpsChannel &ch) const // {{{
{ if (path.size()>0)
    return Error((string)"Applying Snd on "+ToString()+" with nonempty path "+path);
  (*val)=myExp->Eval();
  ch=myChannel;
  return mySucc->Copy();
} // }}}
MpsTerm *MpsTerm::ApplyBRcv(const std::string &path, const std::string &label) const // {{{
{ return Error((string)"Applying BRcv on "+ToString());
} // }}}
MpsTerm *MpsPar::ApplyBRcv(const std::string &path, const std::string &label) const // {{{
{ if (path.size()==0)
    return Error((string)"Applying BRcv on "+ToString());
  MpsTerm *left=NULL;
  MpsTerm *right=NULL;
  if (path[0]=='l') // left
  { left=myLeft->ApplyBRcv(path.substr(1),label);
    right=myRight->Copy();
  }
  else // assume right
  { left=myLeft->Copy();
    right=myRight->ApplyBRcv(path.substr(1),label);
  }
  MpsTerm *result = new MpsPar(*left,*right);
  delete left;
  delete right;
  return result;
} // }}}
MpsTerm *MpsBranch::ApplyBRcv(const std::string &path, const std::string &label) const // {{{
{ if (path.size()>0)
    return Error((string)"Applying BRcv on "+ToString()+" with nonempty path "+path);
  map<string,MpsTerm*>::const_iterator branch = myBranches.find(label);
  if (branch==myBranches.end())
    return Error((string)"Applying BRcv on "+ToString()+" with unknown label "+label);
  return branch->second->Copy();
} // }}}
MpsTerm *MpsTerm::ApplyBSnd(const std::string &path, std::string &label, MpsChannel &ch) const // {{{
{ return Error((string)"Applying BSnd on "+ToString());
} // }}}
MpsTerm *MpsPar::ApplyBSnd(const std::string &path, std::string &label, MpsChannel &ch) const // {{{
{ if (path.size()==0)
    return Error((string)"Applying BSnd on "+ToString());
  MpsTerm *left=NULL;
  MpsTerm *right=NULL;
  if (path[0]=='l') // left
  { left=myLeft->ApplyBSnd(path.substr(1),label,ch);
    right=myRight->Copy();
  }
  else // assume right
  { left=myLeft->Copy();
    right=myRight->ApplyBSnd(path.substr(1),label,ch);
  }
  MpsTerm *result=new MpsPar(*left,*right);
  // Clean up
  delete left;
  delete right;
  // Return
  return result;
} // }}}
MpsTerm *MpsSelect::ApplyBSnd(const std::string &path, std::string &label, MpsChannel &ch) const // {{{
{ if (path.size()>0)
    return Error((string)"Applying BSnd on "+ToString()+" with nonempty path "+path);
  label=myLabel;
  ch=myChannel;
  return mySucc->Copy();
} // }}}
MpsTerm *MpsTerm::ApplyLink(const std::vector<std::string> &paths, const std::string &session) const // {{{
{ if (paths.size()>0)
    return Error((string)"Applying Link on "+ToString());
  else
    return Copy();
} // }}}
MpsTerm *MpsPar::ApplyLink(const std::vector<std::string> &paths, const std::string &session) const // {{{
{ vector<string> pleft;
  vector<string> pright;
  // split paths
  for (vector<string>::const_iterator path=paths.begin();path!=paths.end(); ++path)
  { if (path->size()==0)
      return Error((string)"Applying Link on "+ToString());
    if ((*path)[0]=='l') // left
      pleft.push_back(path->substr(1));
    else // assume right
      pright.push_back(path->substr(1));
  }
  // Create subterms
  MpsTerm *left=myLeft->ApplyLink(pleft,session);
  MpsTerm *right=myRight->ApplyLink(pright,session);
  // Create result
  MpsTerm *result=new MpsPar(*left,*right);
  // Clean up
  delete left;
  delete right;
  return result;
} // }}}
MpsTerm *MpsLink::ApplyLink(const std::vector<std::string> &paths, const std::string &session) const // {{{
{ if (paths.size()==0)
    return Copy();
  else if (paths.size()>1)
    return Error((string)"Applying Link on "+ToString());

  if (myPid==1) // Only first participant registers session
    mpsgui::CreateSession(session,myMaxpid); // Inform GUI
  return mySucc->ERename(mySession,session);
} // }}}
MpsTerm *MpsTerm::ApplySync(const std::vector<std::string> &paths, const std::string &label) const // {{{
{ if (paths.size()>0)
    return Error((string)"Applying Sync on "+ToString());
  else
    return Copy();
} // }}}
MpsTerm *MpsPar::ApplySync(const std::vector<std::string> &paths, const std::string &label) const // {{{
{ vector<string> pleft;
  vector<string> pright;
  // split paths
  for (vector<string>::const_iterator path=paths.begin();path!=paths.end(); ++path)
  { if (path->size()==0)
      return Error((string)"Applying Sync on "+ToString());
    if ((*path)[0]=='l') // left
      pleft.push_back(path->substr(1));
    else // assume right
      pright.push_back(path->substr(1));
  }
  // Create subterms
  MpsTerm *left=myLeft->ApplySync(pleft,label);
  MpsTerm *right=myRight->ApplySync(pright,label);
  // Create result
  MpsTerm *result=new MpsPar(*left,*right);
  // Clean up
  delete left;
  delete right;
  return result;
} // }}}
MpsTerm *MpsSync::ApplySync(const std::vector<std::string> &paths, const std::string &label) const // {{{
{ if (paths.size()==0)
    return Copy();
  if (paths.size()>1)
    return Error((string)"Applying Sync with multiple paths on "+ToString());

  map<string,MpsTerm*>::const_iterator branch=myBranches.find(label);
  if (branch==myBranches.end())
    return Error((string)"Applying Sync with unknown label: "+label+" on "+ToString());
  else
    return branch->second->Copy();
} // }}}
MpsTerm *MpsGuiSync::ApplySync(const std::vector<std::string> &paths, const std::string &label) const // {{{
{ if (paths.size()==0)
    return Copy();
  if (paths.size()>1)
    return Error((string)"Applying Sync with multiple paths on "+ToString());

  map<string,inputbranch>::const_iterator branch=myBranches.find(label);
  if (branch==myBranches.end())
    return Error((string)"Applying Sync with unknown label: "+label+" on "+ToString());

  vector<mpsgui::Choice> choices=mpsgui::GetChoices(mySession,myPid);
  for (vector<mpsgui::Choice>::const_iterator choice=choices.begin(); choice!=choices.end(); ++choice)
  { if (choice->name==label)
    { if (choice->accept)
      { MpsTerm *result = branch->second.term->Copy();
        for (vector<mpsgui::ChoiceArg>::const_iterator arg=choice->args.begin(); arg!=choice->args.end(); ++arg)
        { MpsTerm *tmp=result->ESubst(arg->name,*(arg->value));
          delete result;
          result=tmp;
        }
        return result;
      }
      else
        return Error((string)"Applying Sync with rejected label: "+label+" on "+ToString());
    }
  }
  return Error((string)"Applying Sync with inactive label: "+label+" on "+ToString());
} // }}}
MpsTerm *MpsTerm::ApplyDef(const std::string &path, std::vector<MpsFunction> &dest) const // {{{
{ return Error((string)"Applying Def on "+ToString());
} // }}}
MpsTerm *MpsPar::ApplyDef(const std::string &path, std::vector<MpsFunction> &dest) const // {{{
{ if (path.size()==0)
    return Error((string)"Applying Def on "+ToString());
  MpsTerm *left=NULL;
  MpsTerm *right=NULL;
  if (path[0]=='l') // left
  { left=myLeft->ApplyDef(path.substr(1),dest);
    right=myRight->Copy();
  }
  else // assume right
  { left=myLeft->Copy();
    right=myRight->ApplyDef(path.substr(1),dest);
  }
  MpsTerm *result=new MpsPar(*left,*right);
  // Clean up
  delete left;
  delete right;
  // Return
  return result;
} // }}}
MpsTerm *MpsDef::ApplyDef(const std::string &path, std::vector<MpsFunction> &dest) const // {{{
{ if (path.size()>0)
    return Error((string)"Applying Def on "+ToString()+" with nonempty pathh"+path);
  string name = NewName(myName);
  MpsTerm *newBody=myBody->PRename(myName,name);
  MpsFunction newdef(name,myStateArgs,myArgs,*newBody);
  dest.push_back(newdef);
  delete newBody;
  return mySucc->PRename(myName,name);
} // }}}
MpsTerm *MpsTerm::ApplyCall(const std::string &path, const std::vector<MpsFunction> &funs) const // {{{
{ return Error((string)"Applying Call on "+ToString());
} // }}}
MpsTerm *MpsPar::ApplyCall(const std::string &path, const std::vector<MpsFunction> &funs) const // {{{
{ if (path.size()==0)
    return Error((string)"Applying Call on "+ToString());
  MpsTerm *left=NULL;
  MpsTerm *right=NULL;
  if (path[0]=='l') // left
  { left=myLeft->ApplyCall(path.substr(1),funs);
    right=myRight->Copy();
  }
  else // assume right
  { left=myLeft->Copy();
    right=myRight->ApplyCall(path.substr(1),funs);
  }
  MpsTerm *result=new MpsPar(*left,*right);
  // Clean up
  delete left;
  delete right;
  // Return
  return result;
} // }}}
MpsTerm *MpsCall::ApplyCall(const std::string &path, const std::vector<MpsFunction> &funs) const // {{{
{ if (path.size()!=0)
    return Error((string)"Applying Call on "+ToString());
  for (vector<MpsFunction>::const_iterator fun=funs.begin(); fun!=funs.end(); ++fun)
  { if (fun->GetName()==myName)
      return this->PSubst(myName,fun->GetBody(),fun->GetArgs(), fun->GetStateArgs());
  }
  return Error((string)"Applying Call, but no definition "+myName+" in env");
} // }}}
MpsTerm *MpsTerm::ApplyOther(const std::string &path) const // {{{
{ return Error((string)"Applying Other on "+ToString());
} // }}}
MpsTerm *MpsPar::ApplyOther(const std::string &path) const // {{{
{ if (path.size()==0)
    return Error((string)"Applying Other on "+ToString());
  MpsTerm *left=NULL;
  MpsTerm *right=NULL;
  if (path[0]=='l') // left
  { left=myLeft->ApplyOther(path.substr(1));
    right=myRight->Copy();
  }
  else // assume right
  { left=myLeft->Copy();
    right=myRight->ApplyOther(path.substr(1));
  }
  MpsTerm *result=new MpsPar(*left,*right);
  // Clean up
  delete left;
  delete right;
  // Return
  return result;
} // }}}
MpsTerm *MpsNu::ApplyOther(const std::string &path) const // {{{
{ if (path.size()!=0)
    return Error((string)"Applying Other with nonempty path on "+ToString());
  string name = NewName(myChannel);
  return mySucc->ERename(myChannel,name);
} // }}}
MpsTerm *MpsCond::ApplyOther(const std::string &path) const // {{{
{ if (path.size()!=0)
    return Error((string)"Applying Other with nonempty path on "+ToString());
  MpsExp *exp = myCond->Eval();
  if (typeid(*exp) == typeid(MpsBoolVal))
  {
    MpsBoolVal *val = (MpsBoolVal*)exp;
    bool cond = val->GetValue();
    delete exp;
    if (cond)
      return myTrueBranch->Copy();
    else
      return myFalseBranch->Copy();
  }
  else
    return Error((string)"Applying Other on noncomplete condition: "+ToString());
} // }}}
MpsTerm *MpsGuiValue::ApplyOther(const std::string &path) const // {{{
{ if (path.size()!=0)
    return Error((string)"Applying Other with nonempty path on "+ToString());
  MpsExp *name=myName->Eval();
  MpsExp *value=myValue->Eval();
  MpsStringVal *stringValue = dynamic_cast<MpsStringVal*>(value);
  if (stringValue!=NULL)
    mpsgui::SetValue(mySession,myPid,name->ToString(),stringValue->ToString()); // Inform GUI using string content
  else
    mpsgui::SetValue(mySession,myPid,name->ToString(),value->ToString()); // Inform GUI
  delete name;
  delete value;
  return mySucc->Copy();
} // }}}
MpsTerm *MpsAssign::ApplyOther(const std::string &path) const // {{{
{ if (path.size()!=0)
    return Error((string)"Applying Other with nonempty path on "+ToString());
  MpsExp *value=myExp->Eval();
  MpsTerm *result = mySucc->ESubst(myId,*value);
  delete value;
  return result;
} // }}}

/* Create list of possible internal steps
 */
bool MpsEnd::SubSteps(vector<MpsStep> &dest) // {{{
{
  return false; // No steps
} // }}}
bool MpsSnd::SubSteps(vector<MpsStep> &dest) // {{{
{
  MpsEvent event;
  event.myType = snd;
  event.myChannel = myChannel;
  event.myValue = myExp->Eval();
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event, paths));
  return false;
} // }}}
bool MpsRcv::SubSteps(vector<MpsStep> &dest) // {{{
{
  MpsEvent event;
  event.myType = rcv;
  event.myChannel = myChannel;
  event.myValue = new MpsVarExp(MpsExp::NewVar());
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event, paths));
  return false;
} // }}}
bool MpsSelect::SubSteps(vector<MpsStep> &dest) // {{{
{
  MpsEvent event;
  event.myType = bsnd;
  event.myChannel = myChannel;
  event.myLabel = myLabel;
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event, paths));
  return false;
} // }}}
bool MpsBranch::SubSteps(vector<MpsStep> &dest) // {{{
{
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    MpsEvent event;
    event.myType = brcv;
    event.myChannel = myChannel;
    event.myLabel = it->first;
    vector<string> paths;
    paths.push_back("");
    dest.push_back(MpsStep(event,paths));
  }
  return false;
} // }}}
bool MpsPar::SubSteps(vector<MpsStep> &dest) // {{{
{
  vector<MpsStep> leftsteps;
  leftsteps.clear();
  bool wait_left=myLeft->SubSteps(leftsteps);
  vector<MpsStep> rightsteps;
  rightsteps.clear();
  bool wait_right=myRight->SubSteps(rightsteps);
  // Add all steps from left process
  for (vector<MpsStep>::const_iterator it = leftsteps.begin(); it != leftsteps.end(); ++it)
  { vector<string> paths;
    for (vector<string>::const_iterator path=it->GetPaths().begin(); path!=it->GetPaths().end(); ++path)
      paths.push_back((string)"l"+(*path));
    dest.push_back(MpsStep(it->GetEvent(), paths));
  }
  // Add all steps from right process
  for (vector<MpsStep>::const_iterator it = rightsteps.begin(); it != rightsteps.end(); ++it)
  { vector<string> paths;
    for (vector<string>::const_iterator path=it->GetPaths().begin(); path!=it->GetPaths().end(); ++path)
      paths.push_back((string)"r"+(*path));
    dest.push_back(MpsStep(it->GetEvent(), paths));
  }
  // Clean up
  leftsteps.clear();
  rightsteps.clear();
  return wait_left || wait_right;
} // }}}
bool MpsDef::SubSteps(vector<MpsStep> &dest) // {{{
{
  // Make empty event
  MpsEvent event;
  event.myType = tau;
  event.mySubType = fundef;

  // Add step
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event, paths));

  return false;
} // }}}
bool MpsCall::SubSteps(vector<MpsStep> &dest) // {{{
{
  // Create tau event
  MpsEvent event;
  event.myType=tau;
  event.mySubType = funcall;
  event.myStateArgs = myState.size();
  event.myArgs = myArgs.size();
  // Add step to dest
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event, paths));
  // Does not depend on GUI
  return false;
} // }}}
bool MpsNu::SubSteps(vector<MpsStep> &dest) // {{{
{
  MpsEvent event;
  event.myType=tau;
  event.mySubType=nu;
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event,paths));
  return false;
} // }}}
bool MpsLink::SubSteps(vector<MpsStep> &dest) // {{{
{
  MpsEvent event;
  event.myType    = link;
  event.myChannel = myChannel;
  event.myMaxpid  = myMaxpid;
  event.myPid     = myPid;
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event,paths));
  return false;
} // }}}
bool MpsSync::SubSteps(vector<MpsStep> &dest) // {{{
{
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    MpsExp *include=myAssertions[it->first]->Eval(); //Evaluate assertion
    if (typeid(*include) != typeid(MpsBoolVal))
    { delete include;
      continue;
    }
    MpsBoolVal *boolval = (MpsBoolVal*)include;
    if (boolval->GetValue()) // If assertion is true, then include branch
    {
      MpsEvent event;
      event.myType = sync;
      event.mySession = mySession;
      event.myLabel = it->first;
      event.myMaxpid = myMaxpid;
      vector<string> paths;
      paths.push_back("");
      dest.push_back(MpsStep(event,paths));
    }
    delete include;
  }
  return false;
} // }}}
bool MpsCond::SubSteps(vector<MpsStep> &dest) // {{{
{
  MpsExp *exp = myCond->Eval();
  if (typeid(*exp) == typeid(MpsBoolVal))
  {
    // Add Step event
    MpsEvent event;
    event.myType = tau;
    event.mySubType = cond;
    // Insert step
    vector<string> paths;
    paths.push_back("");
    dest.push_back(MpsStep(event,paths));
  }
  else
    cerr << "ERROR: unable to evaluate condition: " << myCond->ToString() << endl;
  delete exp;
  return false;
} // }}}
bool MpsGuiSync::SubSteps(vector<MpsStep> &dest) // {{{
{
  // Get Choices from GUI
  vector<mpsgui::Choice> choices = mpsgui::GetChoices(mySession, myPid);
  if (choices.size()==0) // If choices not registered
  { // Register choices
    // Consider each choice
    for (map<string,inputbranch>::const_iterator choice=myBranches.begin(); choice!=myBranches.end(); ++choice)
    { // Check if choice is active
      MpsExp *accept=choice->second.assertion->Eval();
      if (typeid(*accept)==typeid(MpsBoolVal) && ((MpsBoolVal*)accept)->GetValue())
      { // Add choice
        mpsgui::Choice newChoice;
        newChoice.name = choice->first;
        for (int i=0; i<choice->second.names.size(); ++i)
        {
          mpsgui::ChoiceArg newArg(choice->second.names[i],
              *choice->second.types[i],
              *choice->second.values[i]);
          newChoice.args.push_back(newArg);
        }
        newChoice.accept=(choice->second.names.size()==0); // Auto accept, if no input required
        choices.push_back(newChoice);
      }
      delete accept;
    }
    // Update GUI with new choices
    mpsgui::SetChoices(mySession, myPid, choices);
  }
  // Create a possible step for each choice
  for (vector<mpsgui::Choice>::const_iterator choice=choices.begin(); choice!=choices.end(); ++choice)
  {
    map<string,inputbranch>::const_iterator branch=myBranches.find(choice->name);
    if (branch!=myBranches.end()) // choice is legal
    {
      if (choice->accept)
      {
        MpsEvent event;
        event.myType = sync;
        event.mySession = mySession;
        event.myLabel = branch->first;
        event.myMaxpid = myMaxpid;
        vector<string> paths;
        paths.push_back("");
        dest.push_back(MpsStep(event, paths));
      }
    }
    else
    {
#if APIMS_DEBUG_LEVEL>10
      cerr << "Illegal choice: " << choice->name << endl;
#endif
    }
  }
  return true;
} // }}}
bool MpsGuiValue::SubSteps(vector<MpsStep> &dest) // {{{
{
  MpsEvent event;
  event.myType = tau;
  event.mySubType = guivalue;
  event.mySession=mySession;
  //event.myName=myName->ToString();
  event.myPid = myPid;
  event.myValue = myValue->Copy();
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event, paths));
  return false;
} // }}}
bool MpsAssign::SubSteps(vector<MpsStep> &dest) // {{{
{
  MpsEvent event;
  event.myType = tau;
  event.mySubType = assign;
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event, paths));
  return false;
} // }}}

/* Renaming of Process Variable
 */
MpsTerm *MpsEnd::PRename(const string &src, const string &dst) const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsSnd::PRename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PRename(src,dst);
  MpsTerm *result = new MpsSnd(myChannel, *myExp, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsRcv::PRename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PRename(src,dst);
  MpsTerm *result = new MpsRcv(myChannel, myDest, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSelect::PRename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PRename(src, dst);
  MpsTerm *result = new MpsSelect(myChannel, myLabel, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsBranch::PRename(const string &src, const string &dst) const // {{{
{
  map<string, MpsTerm*> newBranches;
  newBranches.clear();
  // PRename each branch
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    // assert it->second != NULL
    MpsTerm *newBranch = it->second->PRename(src,dst);
    newBranches[it->first] = newBranch;
  }
  MpsTerm *result = new MpsBranch(myChannel, newBranches, myAssertions);
  // Clean up
  DeleteMap(newBranches);
  return result;
} // }}}
MpsTerm *MpsPar::PRename(const string &src, const string &dst) const // {{{
{
  // assert myLeft != NULL
  // assert myRight != NULL
  MpsTerm *newLeft = myLeft->PRename(src,dst);
  MpsTerm *newRight = myRight->PRename(src,dst);
  MpsTerm *result = new MpsPar(*newLeft, *newRight);
  delete newLeft;
  delete newRight;
  return result;
} // }}}
MpsTerm *MpsDef::PRename(const string &src, const string &dst) const // {{{
{
  if (src == myName)
    return Copy();
  
  MpsTerm *newBody = myBody->PRename(src,dst);
  MpsTerm *newSucc = mySucc->PRename(src,dst);
  MpsTerm *result = new MpsDef(myName,myArgs,myTypes,myStateArgs,myStateTypes,*newBody,*newSucc);
  delete newBody;
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsCall::PRename(const string &src, const string &dst) const // {{{
{
  if (myName != src)
    return Copy();
  else
    return new MpsCall(dst,myArgs,myState);
} // }}}
MpsTerm *MpsNu::PRename(const string &src, const string &dst) const // {{{
{
  MpsTerm *newSucc = mySucc->PRename(src,dst);
  MpsTerm *result = new MpsNu(myChannel,*newSucc,*myType);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsLink::PRename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PRename(src,dst);
  MpsTerm *result = new MpsLink(myChannel, mySession, myPid, myMaxpid, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSync::PRename(const string &src, const string &dst) const // {{{
{
  map<string, MpsTerm*> newBranches;
  newBranches.clear();
  // PReplace each branch
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    MpsTerm *newBranch = it->second->PRename(src,dst);
    newBranches[it->first] = newBranch;
  }
  MpsTerm *result = new MpsSync(myMaxpid, mySession, newBranches, myAssertions);
  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsTerm *MpsCond::PRename(const string &src, const string &dst) const // {{{
{
  MpsTerm *newTrueBranch = myTrueBranch->PRename(src,dst);
  MpsTerm *newFalseBranch = myFalseBranch->PRename(src,dst);
  MpsCond *result = new MpsCond(*myCond, *newTrueBranch, *newFalseBranch);
  delete newTrueBranch;
  delete newFalseBranch;
  return result;
} // }}}
MpsTerm *MpsGuiSync::PRename(const string &src, const string &dst) const // {{{
{
  map<string, inputbranch> newBranches;
  newBranches.clear();
  // PSubst each branch
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    inputbranch newBranch;
    newBranch.term = it->second.term->PRename(src,dst);
    newBranch.assertion = it->second.assertion->Copy();
    newBranch.args = it->second.args;
    newBranch.names = it->second.names;
    newBranch.types.clear();
    for (vector<MpsMsgType*>::const_iterator type=it->second.types.begin(); type!=it->second.types.end(); ++type)
      newBranch.types.push_back((*type)->Copy());
    newBranch.values.clear();
    for (vector<MpsExp*>::const_iterator value=it->second.values.begin(); value!=it->second.values.end(); ++value)
      newBranch.values.push_back((*value)->Copy());
    newBranches[it->first] = newBranch;
  }
  MpsGuiSync *result = new MpsGuiSync(myMaxpid, mySession, myPid, newBranches);
  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second.term;
    delete newBranches.begin()->second.assertion;
    DeleteVector(newBranches.begin()->second.types);
    DeleteVector(newBranches.begin()->second.values);
    newBranches.erase(newBranches.begin());
  }

  return result;
} // }}}
MpsTerm *MpsGuiValue::PRename(const string &src, const string &dst) const // {{{
{
  MpsTerm *newSucc = mySucc->PRename(src,dst);
  MpsGuiValue *result = new MpsGuiValue(myMaxpid, mySession, myPid, *myName, *myValue, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsAssign *MpsAssign::PRename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PRename(src,dst);
  MpsAssign *result = new MpsAssign(myId, *myExp, *myType, *newSucc);
  delete newSucc;
  return result;
} // }}}

/* Renaming of Expression Variable
 */
MpsTerm *MpsEnd::ERename(const string &src, const string &dst) const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsSnd::ERename(const string &src, const string &dst) const // {{{
{
  MpsTerm *newSucc = mySucc->ERename(src,dst);
  MpsExp *newExp=myExp->Rename(src,dst);
  MpsChannel newChannel=myChannel.Rename(src,dst);
  MpsTerm *result = new MpsSnd(newChannel, *newExp, *newSucc);
  delete newSucc;
  delete newExp;
  return result;
} // }}}
MpsTerm *MpsRcv::ERename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  MpsChannel newChannel=myChannel.Rename(src,dst);
  if (src == myDest) // No substitution is needed in successor
    return new MpsRcv(newChannel, myDest, *mySucc);

  MpsTerm *newSucc=NULL;
  string newDest=myDest;
  if (dst==myDest) // Must rename to avoid capturing
  {
    newDest = MpsExp::NewVar(myDest);
    MpsTerm *tmpSucc = mySucc->ERename(myDest,newDest);
    newSucc = tmpSucc->ERename(src,dst);
    delete tmpSucc;
  }
  else
    newSucc = mySucc->ERename(src,dst);
  MpsTerm *result = new MpsRcv(newChannel, newDest, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSelect::ERename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  // assert myChannel != dest
  MpsChannel newChannel=myChannel.Rename(src,dst);
  MpsTerm *newSucc = mySucc->ERename(src,dst);
  MpsTerm *result = new MpsSelect(newChannel, myLabel, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsBranch::ERename(const string &src, const string &dst) const // {{{
{
  MpsChannel newChannel=myChannel.Rename(src,dst);
  map<string, MpsTerm*> newBranches;
  newBranches.clear();
  // ERename each branch
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    MpsTerm *newBranch = it->second->ERename(src,dst);
    newBranches[it->first] = newBranch;
  }
  map<string, MpsExp*> newAssertions;
  newAssertions.clear();
  // Rename each assersion
  for (map<string,MpsExp*>::const_iterator it = myAssertions.begin(); it != myAssertions.end(); ++it)
  {
    MpsExp *newAssertion = it->second->Rename(src,dst);
    newAssertions[it->first] = newAssertion;
  }
  MpsTerm *result = new MpsBranch(newChannel, newBranches, newAssertions);
  // Clean up
  DeleteMap(newBranches);
  DeleteMap(newAssertions);
  return result;
} // }}}
MpsTerm *MpsPar::ERename(const string &src, const string &dst) const // {{{
{
  MpsTerm *newLeft = myLeft->ERename(src,dst); // Rename in left term
  MpsTerm *newRight = myRight->ERename(src,dst); // Rename in right term
  MpsTerm *result = new MpsPar(*newLeft, *newRight); // Combine renamed terms
  delete newLeft;
  delete newRight;
  return result;
} // }}}
MpsTerm *MpsDef::ERename(const string &src, const string &dst) const // {{{
{
  MpsTerm *newSucc = mySucc->ERename(src,dst); // Succ is substituted immediately
  MpsTerm *newBody = myBody->Copy(); // Body is substituted afer possible argument renamings

  bool body_hidden=false;

  // Process StateArgs
  vector<string> newStateArgs;
  for (vector<string>::const_iterator arg=myStateArgs.begin(); arg!=myStateArgs.end(); ++arg)
  { if (src==*arg)
    { body_hidden=true;
      newStateArgs.push_back(*arg);
    }
    else if (dst==*arg) // rename arg in body
    { string newArg = MpsExp::NewVar(*arg);
      MpsTerm *tmpBody = newBody->ERename(*arg,newArg);
      delete newBody;
      newBody=tmpBody;
      newStateArgs.push_back(newArg);
    }
    else
      newStateArgs.push_back(*arg);
  }

  // Process Args
  vector<string> newArgs;
  for (vector<string>::const_iterator arg=myArgs.begin(); arg!=myArgs.end(); ++arg)
  { if (src==*arg)
    { body_hidden=true;
      newArgs.push_back(*arg);
    }
    else if (dst==*arg) // rename arg in body
    { string newArg = MpsExp::NewVar(*arg);
      MpsTerm *tmpBody = newBody->ERename(*arg,newArg);
      delete newBody;
      newBody=tmpBody;
      newArgs.push_back(newArg);
    }
    else
      newArgs.push_back(*arg);
  }
      
  if (not body_hidden)
  { MpsTerm *tmpBody = newBody->ERename(src,dst);
    delete newBody;
    newBody=tmpBody;
  }

  MpsTerm *result = new MpsDef(myName,newArgs,myTypes,newStateArgs,myStateTypes,*newBody,*newSucc);
  delete newBody;
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsCall::ERename(const string &src, const string &dst) const // {{{
{
  vector<MpsExp*> newArgs;
  for (vector<MpsExp*>::const_iterator it=myArgs.begin();it!=myArgs.end();++it)
    newArgs.push_back((*it)->Rename(src,dst));
  vector<MpsExp*> newState;
  for (vector<MpsExp*>::const_iterator it=myState.begin();it!=myState.end();++it)
    newState.push_back((*it)->Rename(src,dst));
  MpsTerm *result = new MpsCall(myName,newArgs,newState);
  DeleteVector(newArgs);
  DeleteVector(newState);
  return result;
} // }}}
MpsTerm *MpsNu::ERename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  if (myChannel==src) // No substitution necessary
    return Copy();

  MpsTerm *newSucc = NULL;
  string newChannel=myChannel;
  if (myChannel==dst) // Must rename myChannel
  {
    newChannel = MpsExp::NewVar();
    MpsTerm *tmpSucc = mySucc->ERename(myChannel,newChannel);
    newSucc = tmpSucc->ERename(src,dst);
    delete tmpSucc;
  }
  else
    newSucc = mySucc->ERename(src,dst);
  MpsTerm *result = new MpsNu(newChannel, *newSucc, *myType);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsLink::ERename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  string newChannel = myChannel==src?dst:myChannel;
  if (mySession == src) // No further substitution needed
    return new MpsLink(newChannel, mySession, myPid, myMaxpid, *mySucc);

  string newSession = mySession;
  MpsTerm *newSucc = NULL;
  if (mySession==dst)
  {
    newSession = MpsExp::NewVar(mySession);
    MpsTerm *tmpSucc = mySucc->ERename(mySession, newSession);
    newSucc = tmpSucc->ERename(src,dst);
    delete tmpSucc;
  }
  else
    newSucc = mySucc->ERename(src,dst);

  MpsTerm *result = new MpsLink(newChannel, newSession, myPid, myMaxpid, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSync::ERename(const string &src, const string &dst) const // {{{
{
  string newSession = mySession==src?dst:mySession;
  map<string, MpsTerm*> newBranches;
  // ERename each branch
  for (map<string,MpsTerm*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    MpsTerm *newBranch = it->second->ERename(src,dst);
    newBranches[it->first] = newBranch;
  }

  map<string, MpsExp*> newAssertions;
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin(); it!=myAssertions.end(); ++it)
  {
    MpsExp *newAssertion = it->second->Rename(src,dst);
    newAssertions[it->first] = newAssertion;
  }

  MpsTerm *result = new MpsSync(myMaxpid, newSession, newBranches, newAssertions);

  // Clean up
  DeleteMap(newBranches);
  DeleteMap(newAssertions);

  return result;
} // }}}
MpsTerm *MpsCond::ERename(const string &src, const string &dst) const // {{{
{
  MpsExp *newCond = myCond->Rename(src,dst);
  MpsTerm *newTrueBranch = myTrueBranch->ERename(src,dst);
  MpsTerm *newFalseBranch = myFalseBranch->ERename(src,dst);
  MpsCond *result = new MpsCond(*newCond, *newTrueBranch, *newFalseBranch);
  delete newCond;
  delete newTrueBranch;
  delete newFalseBranch;
  return result;
} // }}}
MpsTerm *MpsGuiSync::ERename(const string &src, const string &dst) const // {{{
{
  map<string, inputbranch> newBranches;
  newBranches.clear();
  string newSession=mySession==src?dst:mySession;
  // ERename each branch
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    inputbranch newBranch;
    newBranch.term = it->second.term->Copy(); // Copy branch-term
    newBranch.assertion = it->second.assertion->Rename(src,dst);
    newBranch.names = it->second.names;
    newBranch.types.clear();
    for (vector<MpsMsgType*>::const_iterator type=it->second.types.begin(); type!=it->second.types.end(); ++type)
      newBranch.types.push_back((*type)->Copy());
    newBranch.values.clear();
    for (vector<MpsExp*>::const_iterator value=it->second.values.begin(); value!=it->second.values.end(); ++value)
      newBranch.values.push_back((*value)->Rename(src,dst));
    // vector<string>::find
    bool found=false;
    for (vector<string>::const_iterator arg=it->second.args.begin(); arg!=it->second.args.end(); ++arg)
      if (*arg == src)
        found=true;
    if (found) // Do not substitute in body and no argument renaming
    {
      newBranch.args = it->second.args;
    }
    else
    {
      // Find new arguments, and rename if necessary
      newBranch.args.clear();
      for (vector<string>::const_iterator arg=it->second.args.begin(); arg!=it->second.args.end(); ++arg)
      {
        if (dst==*arg) //Argument must be renamed
        {
          string newArg = MpsExp::NewVar(); // Make unused name
          MpsTerm *tmpTerm = newBranch.term->ERename(*arg,newArg); // Replace old variable with new one
          delete newBranch.term;
          newBranch.term=tmpTerm;
          newBranch.args.push_back(newArg); // Add new argument name
        }
        else
          newBranch.args.push_back(*arg); // Add original argument name
      }
      MpsTerm *tmpTerm = newBranch.term->ERename(src,dst); // Make substitution in body
      delete newBranch.term;
      newBranch.term=tmpTerm;
    }

    newBranches[it->first] = newBranch;
  }
  MpsGuiSync *result = new MpsGuiSync(myMaxpid, newSession, myPid, newBranches);

  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second.term;
    delete newBranches.begin()->second.assertion;
    while (newBranches.begin()->second.types.size()>0)
    {
      delete *newBranches.begin()->second.types.begin();
      newBranches.begin()->second.types.erase(newBranches.begin()->second.types.begin());
    }
    while (newBranches.begin()->second.values.size()>0)
    {
      delete *newBranches.begin()->second.values.begin();
      newBranches.begin()->second.values.erase(newBranches.begin()->second.values.begin());
    }
    newBranches.erase(newBranches.begin());
  }

  return result;
} // }}}
MpsTerm *MpsGuiValue::ERename(const string &src, const string &dst) const // {{{
{
  string newSession=mySession==src?dst:mySession;
  MpsTerm *newSucc = mySucc->ERename(src,dst);
  MpsExp *newName = myName->Rename(src,dst);
  MpsExp *newValue = myValue->Rename(src,dst);
  MpsGuiValue *result = new MpsGuiValue(myMaxpid, newSession, myPid, *newName, *newValue, *newSucc);

  // Clean up
  delete newName;
  delete newValue;
  delete newSucc;

  return result;
} // }}}
MpsAssign *MpsAssign::ERename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->ERename(src,dst);
  MpsExp *newExp = myExp->Rename(src,dst);
  MpsAssign *result = new MpsAssign(myId, *newExp, *myType, *newSucc);
  delete newSucc;
  delete newExp;
  return result;
} // }}}

/* Substitution of Process Variable
 */
MpsTerm *MpsEnd::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<string> &stateargs) const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsSnd::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<string> &stateargs) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PSubst(var, exp, args, stateargs);
  MpsTerm *result = new MpsSnd(myChannel, *myExp, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsRcv::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<string> &stateargs) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PSubst(var, exp, args, stateargs);
  MpsTerm *result = new MpsRcv(myChannel, myDest, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSelect::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<string> &stateargs) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PSubst(var, exp, args, stateargs);
  MpsTerm *result = new MpsSelect(myChannel, myLabel, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsBranch::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<string> &stateargs) const // {{{
{
  map<string, MpsTerm*> newBranches;
  newBranches.clear();
  // PSubst each branch
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    // assert it->second != NULL
    MpsTerm *newBranch = it->second->PSubst(var,exp,args,stateargs);
    newBranches[it->first] = newBranch;
  }
  MpsTerm *result = new MpsBranch(myChannel, newBranches, myAssertions);
  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second;
    newBranches.erase(newBranches.begin());
  }
  return result;
} // }}}
MpsTerm *MpsPar::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<string> &stateargs) const // {{{
{
  // assert myLeft != NULL
  // assert myRight != NULL
  MpsTerm *newLeft = myLeft->PSubst(var,exp,args,stateargs);
  MpsTerm *newRight = myRight->PSubst(var,exp,args,stateargs);
  MpsTerm *result = new MpsPar(*newLeft, *newRight);
  delete newLeft;
  delete newRight;
  return result;
} // }}}
MpsTerm *MpsDef::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<string> &stateargs) const // {{{
{
  // assert(mySucc != NULL);
  // assert myName not in exp.FPV()
  if (var == myName)
    return Copy();

  string newName = myName;
  MpsTerm *newSucc = NULL;
  MpsTerm *newBody = NULL;

  // Check if renaming of myName is necessary
  set<string> fpv=exp.FPV();
  if (fpv.find(newName) != fpv.end()) // Must rename myName
  {
    newName = NewName();
    vector<MpsExp*> tmpArgs;
    tmpArgs.clear();
    for (vector<string>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
      tmpArgs.push_back(new MpsVarExp(*it));
    vector<MpsExp*> tmpState;
    tmpState.clear();
    for (vector<string>::const_iterator it=myStateArgs.begin(); it!=myStateArgs.end(); ++it)
      tmpState.push_back(new MpsVarExp(*it));
    MpsTerm *newVar = new MpsCall(newName,tmpArgs,tmpState);
    DeleteVector(tmpArgs);
    DeleteVector(tmpState);
    newSucc = mySucc->PSubst(myName,*newVar,myArgs,myStateArgs); // Initialise newSucc
    newBody = myBody->PSubst(myName,*newVar,myArgs,myStateArgs); // Initialise newBody
    delete newVar;
  }
  else // Renaming of myName not necessary
  {
    newSucc = mySucc->Copy(); // Initialise newSucc
    newBody = myBody->Copy(); // Initialise newBody
  }

  set<string> fev=exp.FEV();
  vector<string> newArgs;
  newArgs.clear();
  for (vector<string>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  {
    if (fev.find(*it) != fev.end()) // Must rename argument
    {
      string newArg = MpsExp::NewVar(); // Make new name
      MpsExp *newVar = new MpsVarExp(newArg);
      MpsTerm *tmpBody = newBody->ESubst(*it,*newVar); // Rename argument
      delete newVar;
      delete newBody;
      newBody = tmpBody;
      newArgs.push_back(newArg); // Add the argument
    }
    else
      newArgs.push_back(*it); // Add the argument
  }
  { // Make substitution
    MpsTerm *tmpSucc = newSucc->PSubst(var,exp,args,stateargs);
    delete newSucc;
    newSucc = tmpSucc;

    MpsTerm *tmpBody = newBody->PSubst(var,exp,args,stateargs);
    delete newBody;
    newBody = tmpBody;
  }
  // Make result
  MpsTerm *result = new MpsDef(newName, newArgs, myTypes, myStateArgs, myStateTypes, *newBody, *newSucc);
  delete newBody;
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsCall::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<string> &stateargs) const // {{{
{
  if (myName == var)
  {
    if (myState.size() != stateargs.size())
    {
#if APIMS_DEBUG_LEVEL>2
      cerr << "MpsCall::PSubst wrong number of state arguments in match" << endl;
#endif
      return new MpsEnd();
    }
    if (myArgs.size() != args.size())
    {
#if APIMS_DEBUG_LEVEL>2
      cerr << "MpsCall::PSubst wrong number of arguments in match" << endl;
#endif
      return new MpsEnd();
    }
    // Create new Variable Names
    vector<string> stateNames;
    stateNames.clear();
    vector<string> argNames;
    argNames.clear();
    MpsTerm *newTerm=exp.Copy();
    for (vector<string>::const_iterator it=stateargs.begin();it!=stateargs.end();++it)
    {
      // Create new variable
      MpsExp *newVar=new MpsVarExp(MpsExp::NewVar());
      MpsTerm *tmpTerm=newTerm->ESubst(*it,*newVar); // Rename statearg to new name
      stateNames.push_back(newVar->ToString());
      delete newVar;
      delete newTerm;
      newTerm=tmpTerm;
    }
    for (vector<string>::const_iterator it=args.begin();it!=args.end();++it)
    {
      // Create new variable
      MpsExp *newVar=new MpsVarExp(MpsExp::NewVar());
      MpsTerm *tmpTerm=newTerm->ESubst(*it,*newVar); // Rename arg to new name
      argNames.push_back(newVar->ToString());
      delete newVar;
      delete newTerm;
      newTerm=tmpTerm;
    }
    // Now the args and stateargs cannot capture any variables in myArgs and myState
    
    vector<MpsExp*>::const_iterator stateExp=myState.begin();
    for (vector<string>::const_iterator stateName=stateNames.begin();
         stateName!=stateNames.end() && stateExp!=myState.end();
         ++stateName,++stateExp)
    { // Substitute the argNames for the arguments one by one
      MpsExp *value=(*stateExp)->Eval();
      MpsTerm *tmpTerm=newTerm->ESubst(*stateName,*value);
      delete value;
      delete newTerm;
      newTerm = tmpTerm;
    }
    vector<MpsExp*>::const_iterator argExp=myArgs.begin();
    for (vector<string>::const_iterator argName=argNames.begin();
         argName!=argNames.end() && argExp!=myArgs.end();
         ++argName,++argExp)
    { // Substitute the argNames for the arguments one by one
      MpsExp *value=(*argExp)->Eval();
      MpsTerm *tmpTerm=newTerm->ESubst(*argName,*value);
      delete value;
      delete newTerm;
      newTerm = tmpTerm;
    }
    return newTerm;
  }
  else
    return new MpsCall(myName,myArgs,myState);
} // }}}
MpsTerm *MpsNu::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<string> &stateargs) const // {{{
{
  // assert mySucc != NULL
  string newChannel = myChannel;
  
  set<string> fev=exp.FEV();
  MpsTerm *newSucc = NULL;
  if (fev.find(myChannel)!= fev.end()) // Rename channel before subst
  {
    newChannel = MpsExp::NewVar();
    MpsExp *newVar = new MpsVarExp(newChannel);
    MpsTerm *tmpSucc = mySucc->ESubst(myChannel,*newVar);
    newSucc = tmpSucc->PSubst(var,exp,args,stateargs);
    delete tmpSucc;
    delete newVar;
  }
  else
    newSucc = mySucc->PSubst(var,exp,args,stateargs);
  MpsTerm *result = new MpsNu(newChannel, *newSucc, *myType);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsLink::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<string> &stateargs) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PSubst(var,exp,args,stateargs);
  MpsTerm *result = new MpsLink(myChannel, mySession, myPid, myMaxpid, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSync::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<string> &stateargs) const // {{{
{
  map<string, MpsTerm*> newBranches;
  newBranches.clear();
  // PSubst each branch
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    // assert it->second != NULL
    MpsTerm *newBranch = it->second->PSubst(var,exp,args,stateargs);
    newBranches[it->first] = newBranch;
  }
  MpsTerm *result = new MpsSync(myMaxpid, mySession, newBranches, myAssertions);
  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second;
    newBranches.erase(newBranches.begin());
  }

  return result;
} // }}}
MpsTerm *MpsCond::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<string> &stateargs) const // {{{
{
  MpsTerm *newTrueBranch = myTrueBranch->PSubst(var,exp,args,stateargs);
  MpsTerm *newFalseBranch = myFalseBranch->PSubst(var,exp,args,stateargs);
  MpsCond *result = new MpsCond(*myCond, *newTrueBranch, *newFalseBranch);
  delete newTrueBranch;
  delete newFalseBranch;
  return result;
} // }}}
MpsGuiSync *MpsGuiSync::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<string> &stateargs) const // {{{
{
  map<string, inputbranch> newBranches;
  newBranches.clear();
  // PSubst each branch
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    inputbranch newBranch;
    newBranch.term = it->second.term->PSubst(var,exp,args,stateargs);
    newBranch.assertion = it->second.assertion->Copy();
    newBranch.args = it->second.args;
    newBranch.names = it->second.names;
    newBranch.types.clear();
    for (vector<MpsMsgType*>::const_iterator type=it->second.types.begin(); type!=it->second.types.end(); ++type)
      newBranch.types.push_back((*type)->Copy());
    newBranch.values.clear();
    for (vector<MpsExp*>::const_iterator value=it->second.values.begin(); value!=it->second.values.end(); ++value)
      newBranch.values.push_back((*value)->Copy());
    newBranches[it->first] = newBranch;
  }
  MpsGuiSync *result = new MpsGuiSync(myMaxpid, mySession, myPid, newBranches);
  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second.term;
    delete newBranches.begin()->second.assertion;
    while (newBranches.begin()->second.types.size()>0)
    {
      delete *newBranches.begin()->second.types.begin();
      newBranches.begin()->second.types.erase(newBranches.begin()->second.types.begin());
    }
    while (newBranches.begin()->second.values.size()>0)
    {
      delete *newBranches.begin()->second.values.begin();
      newBranches.begin()->second.values.erase(newBranches.begin()->second.values.begin());
    }
    newBranches.erase(newBranches.begin());
  }

  return result;
} // }}}
MpsGuiValue *MpsGuiValue::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<string> &stateargs) const // {{{
{
  MpsTerm *newSucc = mySucc->PSubst(var, exp, args, stateargs);
  MpsGuiValue *result = new MpsGuiValue(myMaxpid, mySession, myPid, *myName, *myValue, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsAssign *MpsAssign::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<string> &stateargs) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PSubst(var, exp, args, stateargs);
  MpsAssign *result = new MpsAssign(myId, *myExp, *myType, *newSucc);
  delete newSucc;
  return result;
} // }}}

/* Substitution of Expression Variable
 */
MpsTerm *MpsEnd::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsSnd::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  // assert mySucc != NULL
  // assert myExp != NULL
  // assert myChannel != dest
  // dest not in FPV
  MpsTerm *newSucc = mySucc->ESubst(source,dest);
  MpsExp *newExp=myExp->Subst(source,dest);
  MpsChannel newChannel=myChannel.Subst(source,dest);
  MpsTerm *result = new MpsSnd(newChannel, *newExp, *newSucc);
  delete newSucc;
  delete newExp;
  return result;
} // }}}
MpsTerm *MpsRcv::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  // assert mySucc != NULL
  MpsChannel newChannel=myChannel.Subst(source,dest);
  if (source == myDest) // No substitution is needed in successor
    return new MpsRcv(newChannel, myDest, *mySucc);

  MpsTerm *newSucc = NULL;
  string newDest = myDest;
  set<string> fv = dest.FV();
  if (fv.find(myDest) != fv.end()) // Must rename to avoid capturing
  {
    newDest = MpsExp::NewVar();
    MpsExp *newVar = new MpsVarExp(newDest);
    MpsTerm *tmpSucc = mySucc->ESubst(myDest,*newVar);
    newSucc = tmpSucc->ESubst(source,dest);
    delete tmpSucc;
    delete newVar;
  }
  else
    newSucc = mySucc->ESubst(source,dest);
  MpsTerm *result = new MpsRcv(newChannel, newDest, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSelect::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  // assert mySucc != NULL
  // assert myChannel != dest
  MpsChannel newChannel=myChannel.Subst(source,dest);
  string newLabel = myLabel;
  MpsTerm *newSucc = mySucc->ESubst(source,dest);
  MpsTerm *result = new MpsSelect(newChannel, newLabel, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsBranch::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  MpsChannel newChannel=myChannel.Subst(source,dest);
  map<string, MpsTerm*> newBranches;
  newBranches.clear();
  // ESubst each branch
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    // assert it->second != NULL
    MpsTerm *newBranch = it->second->ESubst(source,dest);
    newBranches[it->first] = newBranch;
  }
  map<string, MpsExp*> newAssertions;
  newAssertions.clear();
  // ESubst each branch
  for (map<string,MpsExp*>::const_iterator it = myAssertions.begin(); it != myAssertions.end(); ++it)
  {
    // assert it->second != NULL
    MpsExp *newAssertion = it->second->Subst(source,dest);
    newAssertions[it->first] = newAssertion;
  }
  MpsTerm *result = new MpsBranch(newChannel, newBranches, newAssertions);
  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second;
    newBranches.erase(newBranches.begin());
  }
  DeleteMap(newAssertions);
  return result;
} // }}}
MpsTerm *MpsPar::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  // assert myLeft != NULL
  // assert myRight != NULL
  MpsTerm *newLeft = myLeft->ESubst(source,dest); // Substitute left term
  MpsTerm *newRight = myRight->ESubst(source,dest); // Substitute right term
  MpsTerm *result = new MpsPar(*newLeft, *newRight); // Combine substituted terms
  delete newLeft;
  delete newRight;
  return result;
} // }}}
MpsTerm *MpsDef::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  // assert(mySucc != NULL);
  // Renaming myName not necessary, as dest cannot contain process variables
  string newName = myName; // Copy name
  MpsTerm *newSucc = mySucc->ESubst(source,dest); // Succ is substituted immediately
  MpsTerm *newBody = myBody->Copy(); // Body is substituted afer possible argument renamings
  vector<MpsMsgType*> newTypes;
  for (vector<MpsMsgType*>::const_iterator it=myTypes.begin(); it!=myTypes.end(); ++it)
    newTypes.push_back((*it)->ESubst(source,dest));
  vector<MpsMsgType*> newStateTypes;
  for (vector<MpsMsgType*>::const_iterator it=myStateTypes.begin(); it!=myStateTypes.end(); ++it)
    newStateTypes.push_back((*it)->ESubst(source,dest));

  MpsTerm *result = NULL; // Find result
  // vector<string>::find
  bool found=false;
  for (vector<string>::const_iterator it=myStateArgs.begin(); it!=myStateArgs.end(); ++it)
    if (*it == source)
      found=true;
  for (vector<string>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
    if (*it == source)
      found=true;
  if (found) // Do not substitute in body
    result = new MpsDef(newName, myArgs, newTypes, myStateArgs, newStateTypes, *newBody, *newSucc);
  else
  {
    vector<string> newStateArgs; // Find new state arguments, and rename if necessary
    vector<string> newArgs; // Find new arguments, and rename if necessary
    set<string> fv=dest.FV();
    for (vector<string>::const_iterator it=myStateArgs.begin(); it!=myStateArgs.end(); ++it)
    {
      if (fv.find(*it)!=fv.end()) //Argument must be renamed
      {
        string newArg = MpsExp::NewVar(*it); // Make unused name
        MpsTerm *tmpBody = newBody->ERename(*it,newArg); // Replace old variable with new one
        delete newBody;
        newBody=tmpBody;
        newStateArgs.push_back(newArg); // Add argument name
      }
      else
        newStateArgs.push_back(*it); // Add argument name
    }
    for (vector<string>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
    {
      if (fv.find(*it)!=fv.end()) //Argument must be renamed
      {
        string newArg = MpsExp::NewVar(*it); // Make unused name
        MpsTerm *tmpBody = newBody->ERename(*it,newArg); // Replace old variable with new one
        delete newBody;
        newBody=tmpBody;
        newArgs.push_back(newArg); // Add argument name
      }
      else
        newArgs.push_back(*it); // Add argument name
    }
    MpsTerm *tmpBody = newBody->ESubst(source,dest); // Make substitution in body
    delete newBody;
    newBody=tmpBody;
    result = new MpsDef(newName, newArgs, newTypes, newStateArgs, newStateTypes, *newBody, *newSucc);
  }
  delete newBody;
  delete newSucc;
  DeleteVector(newTypes);
  DeleteVector(newStateTypes);
  return result;
} // }}}
MpsTerm *MpsCall::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  vector<MpsExp*> newArgs;
  for (vector<MpsExp*>::const_iterator it=myArgs.begin();it!=myArgs.end();++it)
    newArgs.push_back((*it)->Subst(source,dest));
  vector<MpsExp*> newState;
  for (vector<MpsExp*>::const_iterator it=myState.begin();it!=myState.end();++it)
    newState.push_back((*it)->Subst(source,dest));
  MpsTerm *result = new MpsCall(myName,newArgs,newState);
  DeleteVector(newArgs);
  DeleteVector(newState);
  return result;
} // }}}
MpsTerm *MpsNu::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  MpsGlobalType *newType=myType->ESubst(source,dest);
  // assert mySucc != NULL
  if (myChannel==source) // No substitution necessary
  { MpsTerm *result=new MpsNu(myChannel, *mySucc, *newType);
    delete newType;
    return result;
  }

  MpsTerm *newSucc = NULL;
  set<string> fv = dest.FV();
  string newChannel = myChannel;
  if (fv.find(myChannel) != fv.end()) // Must rename myChannel
  {
    newChannel = MpsExp::NewVar();
    MpsExp *newVar = new MpsVarExp(newChannel);
    MpsTerm *tmpSucc = mySucc->ESubst(myChannel,*newVar);
    newSucc = tmpSucc->ESubst(source,dest);
    delete tmpSucc;
    delete newVar;
  }
  else
    newSucc = mySucc->ESubst(source,dest);
  MpsTerm *result = new MpsNu(newChannel, *newSucc, *newType);
  delete newType;
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsLink::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  // assert mySucc != NULL
  string newChannel = stringESubst(myChannel,source,dest);
  if (mySession == source) // No further substitution needed
    return new MpsLink(newChannel, mySession, myPid, myMaxpid, *mySucc);

  set<string> fv=dest.FV();
  string newSession = mySession;
  MpsTerm *newSucc = NULL;
  if (fv.find(mySession) != fv.end())
  {
    newSession = MpsExp::NewVar();
    MpsExp *newVar = new MpsVarExp(newSession);
    MpsTerm *tmpSucc = mySucc->ESubst(mySession, *newVar);
    newSucc = tmpSucc->ESubst(source,dest);
    delete newVar;
    delete tmpSucc;
  }
  else
    newSucc = mySucc->ESubst(source,dest);

  MpsTerm *result = new MpsLink(newChannel, newSession, myPid, myMaxpid, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSync::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  string newSession = stringESubst(mySession,source,dest);

  // ESubst each branch
  map<string, MpsTerm*> newBranches;
  for (map<string,MpsTerm*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    MpsTerm *newBranch = it->second->ESubst(source,dest);
    newBranches[it->first] = newBranch;
  }

  // Subst each assertion
  map<string, MpsExp*> newAssertions;
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin(); it!=myAssertions.end(); ++it)
  {
    MpsExp *newAssertion = it->second->Subst(source,dest);
    newAssertions[it->first] = newAssertion;
  }
  MpsTerm *result = new MpsSync(myMaxpid, newSession, newBranches, newAssertions);

  // Clean up
  DeleteMap(newBranches);
  DeleteMap(newAssertions);

  return result;
} // }}}
MpsTerm *MpsCond::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  MpsExp *newCond = myCond->Subst(source,dest);
  MpsTerm *newTrueBranch = myTrueBranch->ESubst(source,dest);
  MpsTerm *newFalseBranch = myFalseBranch->ESubst(source,dest);
  MpsCond *result = new MpsCond(*newCond, *newTrueBranch, *newFalseBranch);
  delete newCond;
  delete newTrueBranch;
  delete newFalseBranch;
  return result;
} // }}}
MpsGuiSync *MpsGuiSync::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  map<string, inputbranch> newBranches;
  newBranches.clear();
  string newSession=stringESubst(mySession,source,dest);
  // ESubst each branch
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    inputbranch newBranch;
    newBranch.term = it->second.term->Copy(); // Copy branch-term
    newBranch.assertion = it->second.assertion->Subst(source,dest);
    newBranch.names = it->second.names;
    newBranch.types.clear();
    for (vector<MpsMsgType*>::const_iterator type=it->second.types.begin(); type!=it->second.types.end(); ++type)
      newBranch.types.push_back((*type)->Copy());
    newBranch.values.clear();
    for (vector<MpsExp*>::const_iterator value=it->second.values.begin(); value!=it->second.values.end(); ++value)
      newBranch.values.push_back((*value)->Subst(source,dest));
    // vector<string>::find
    bool found=false;
    for (vector<string>::const_iterator arg=it->second.args.begin(); arg!=it->second.args.end(); ++arg)
      if (*arg == source)
        found=true;
    if (found/*myArgs.find(source) != myArgs.end()*/) // Do not substitute in body and no argument renaming
    {
      newBranch.args = it->second.args;
    }
    else
    {
      // Find new arguments, and rename if necessary
      newBranch.args.clear();
      set<string> fv=dest.FV();
      for (vector<string>::const_iterator arg=it->second.args.begin(); arg!=it->second.args.end(); ++arg)
      {
        if (fv.find(*arg)!=fv.end()) //Argument must be renamed
        {
          string newArg = MpsExp::NewVar(); // Make unused name
          MpsExp *newVar = new MpsVarExp(newArg); // Make variable with the new name
          MpsTerm *tmpTerm = newBranch.term->ESubst(*arg,*newVar); // Replace old variable with new one
          delete newVar;
          delete newBranch.term;
          newBranch.term=tmpTerm;
          newBranch.args.push_back(newArg); // Add new argument name
        }
        else
          newBranch.args.push_back(*arg); // Add original argument name
      }
      MpsTerm *tmpTerm = newBranch.term->ESubst(source,dest); // Make substitution in body
      delete newBranch.term;
      newBranch.term=tmpTerm;
    }

    newBranches[it->first] = newBranch;
  }
  MpsGuiSync *result = new MpsGuiSync(myMaxpid, newSession, myPid, newBranches);

  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second.term;
    delete newBranches.begin()->second.assertion;
    while (newBranches.begin()->second.types.size()>0)
    {
      delete *newBranches.begin()->second.types.begin();
      newBranches.begin()->second.types.erase(newBranches.begin()->second.types.begin());
    }
    while (newBranches.begin()->second.values.size()>0)
    {
      delete *newBranches.begin()->second.values.begin();
      newBranches.begin()->second.values.erase(newBranches.begin()->second.values.begin());
    }
    newBranches.erase(newBranches.begin());
  }

  return result;
} // }}}
MpsGuiValue *MpsGuiValue::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  string newSession=stringESubst(mySession,source,dest);
  MpsTerm *newSucc = mySucc->ESubst(source,dest);
  MpsExp *newName = myName->Subst(source,dest);
  MpsExp *newValue = myValue->Subst(source,dest);
  MpsGuiValue *result = new MpsGuiValue(myMaxpid, newSession, myPid, *newName, *newValue, *newSucc);

  // Clean up
  delete newName;
  delete newValue;
  delete newSucc;

  return result;
} // }}}
MpsAssign *MpsAssign::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  MpsExp *newExp=myExp->Subst(source,dest);
  MpsTerm *newSucc;
  if (myId==source)
    newSucc=mySucc->Copy();
  else
    newSucc = mySucc->ESubst(source,dest);
  MpsAssign *result = new MpsAssign(myId, *newExp, *myType, *newSucc);
  delete newExp;
  delete newSucc;
  return result;
} // }}}

/* Substitution of Global Type Variable
 */
MpsTerm *MpsEnd::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsSnd::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  MpsTerm *result = new MpsSnd(myChannel, *myExp, *newSucc);

  // Clean Up
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsRcv::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  MpsTerm *result = new MpsRcv(myChannel, myDest, *newSucc);

  // Clean Up
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsSelect::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  MpsTerm *result = new MpsSelect(myChannel, myLabel, *newSucc);

  // Clean Up
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsBranch::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  map<string, MpsTerm*> newBranches;
  newBranches.clear();
  // GSubst each branch
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
    newBranches[it->first] = it->second->GSubst(source,dest,args);
  MpsTerm *result = new MpsBranch(myChannel, newBranches, myAssertions);

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsTerm *MpsPar::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newLeft = myLeft->GSubst(source,dest,args); // Substitute left term
  MpsTerm *newRight = myRight->GSubst(source,dest,args); // Substitute right term
  MpsTerm *result = new MpsPar(*newLeft, *newRight); // Combine substituted terms

  // Clean Up
  delete newLeft;
  delete newRight;

  return result;
} // }}}
MpsTerm *MpsDef::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  MpsTerm *newBody = myBody->GSubst(source,dest,args);

  vector<MpsMsgType*> newStateTypes;
  newStateTypes.clear();
  for (vector<MpsMsgType*>::const_iterator type=myStateTypes.begin(); type!=myStateTypes.end(); ++type)
    newStateTypes.push_back((*type)->GSubst(source,dest,args));
  vector<MpsMsgType*> newTypes;
  newTypes.clear();
  for (vector<MpsMsgType*>::const_iterator type=myTypes.begin(); type!=myTypes.end(); ++type)
    newTypes.push_back((*type)->GSubst(source,dest,args));
  
  MpsTerm *result = new MpsDef(myName,myArgs,newTypes,myStateArgs,newStateTypes,*newBody,*newSucc);

  // Clean Up
  DeleteVector(newStateTypes);
  DeleteVector(newTypes);
  delete newBody;
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsCall::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsNu::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsGlobalType *newType = myType->GSubst(source,dest,args);
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  MpsTerm *result =  new MpsNu(myChannel, *newSucc, *newType);

  // Clean Up
  delete newType;
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsLink::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  MpsTerm *result = new MpsLink(myChannel,mySession,myPid,myMaxpid,*newSucc);

  // Clean Up
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsSync::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  // GSubst each branch
  map<string, MpsTerm*> newBranches;
  for (map<string,MpsTerm*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    MpsTerm *newBranch = it->second->GSubst(source,dest,args);
    newBranches[it->first] = newBranch;
  }

  MpsTerm *result = new MpsSync(myMaxpid, mySession, newBranches, myAssertions);

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsTerm *MpsCond::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newTrueBranch = myTrueBranch->GSubst(source,dest,args);
  MpsTerm *newFalseBranch = myFalseBranch->GSubst(source,dest,args);
  MpsCond *result = new MpsCond(*myCond, *newTrueBranch, *newFalseBranch);

  // Clean Up
  delete newTrueBranch;
  delete newFalseBranch;

  return result;
} // }}}
MpsGuiSync *MpsGuiSync::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  map<string, inputbranch> newBranches;
  newBranches.clear();
  // GSubst each branch
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    inputbranch newBranch;
    newBranch.term = it->second.term->GSubst(source,dest,args);
    newBranch.assertion = it->second.assertion->Copy();
    newBranch.names = it->second.names;
    newBranch.args = it->second.args;
    newBranch.types.clear();
    for (vector<MpsMsgType*>::const_iterator type=it->second.types.begin(); type!=it->second.types.end(); ++type)
      newBranch.types.push_back((*type)->GSubst(source,dest,args));
    newBranch.values.clear();
    for (vector<MpsExp*>::const_iterator value=it->second.values.begin(); value!=it->second.values.end(); ++value)
      newBranch.values.push_back((*value)->Copy());

    newBranches[it->first] = newBranch;
  }
  MpsGuiSync *result = new MpsGuiSync(myMaxpid, mySession, myPid, newBranches);

  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second.term;
    delete newBranches.begin()->second.assertion;
    while (newBranches.begin()->second.types.size()>0)
    {
      delete *newBranches.begin()->second.types.begin();
      newBranches.begin()->second.types.erase(newBranches.begin()->second.types.begin());
    }
    while (newBranches.begin()->second.values.size()>0)
    {
      delete *newBranches.begin()->second.values.begin();
      newBranches.begin()->second.values.erase(newBranches.begin()->second.values.begin());
    }
    newBranches.erase(newBranches.begin());
  }

  return result;
} // }}}
MpsGuiValue *MpsGuiValue::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  MpsGuiValue *result = new MpsGuiValue(myMaxpid, mySession, myPid, *myName, *myValue, *newSucc);

  // Clean up
  delete newSucc;

  return result;
} // }}}
MpsAssign *MpsAssign::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsMsgType *newType=myType->GSubst(source,dest,args);
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  MpsAssign *result = new MpsAssign(myId, *myExp, *newType, *newSucc);

  // Clean Up
  delete newType;
  delete newSucc;

  return result;
} // }}}

/* Substitution of Local Type Variable
 */
MpsTerm *MpsEnd::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsSnd::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->LSubst(source,dest,args);
  MpsTerm *result = new MpsSnd(myChannel, *myExp, *newSucc);

  // Clean Up
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsRcv::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->LSubst(source,dest,args);
  MpsTerm *result = new MpsRcv(myChannel, myDest, *newSucc);

  // Clean Up
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsSelect::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->LSubst(source,dest,args);
  MpsTerm *result = new MpsSelect(myChannel, myLabel, *newSucc);

  // Clean Up
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsBranch::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  map<string, MpsTerm*> newBranches;
  newBranches.clear();
  // LSubst each branch
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
    newBranches[it->first] = it->second->LSubst(source,dest,args);
  MpsTerm *result = new MpsBranch(myChannel, newBranches, myAssertions);

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsTerm *MpsPar::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newLeft = myLeft->LSubst(source,dest,args); // Substitute left term
  MpsTerm *newRight = myRight->LSubst(source,dest,args); // Substitute right term
  MpsTerm *result = new MpsPar(*newLeft, *newRight); // Combine substituted terms

  // Clean Up
  delete newLeft;
  delete newRight;

  return result;
} // }}}
MpsTerm *MpsDef::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->LSubst(source,dest,args);
  MpsTerm *newBody = myBody->LSubst(source,dest,args);

  vector<MpsMsgType*> newStateTypes;
  newStateTypes.clear();
  for (vector<MpsMsgType*>::const_iterator type=myStateTypes.begin(); type!=myStateTypes.end(); ++type)
    newStateTypes.push_back((*type)->LSubst(source,dest,args));
  vector<MpsMsgType*> newTypes;
  newTypes.clear();
  for (vector<MpsMsgType*>::const_iterator type=myTypes.begin(); type!=myTypes.end(); ++type)
    newTypes.push_back((*type)->LSubst(source,dest,args));
  
  MpsTerm *result = new MpsDef(myName,myArgs,newTypes,myStateArgs,newStateTypes,*myBody,*mySucc);

  // Clean Up
  DeleteVector(newStateTypes);
  DeleteVector(newTypes);
  delete newBody;
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsCall::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsNu::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsGlobalType *newType = myType->LSubst(source,dest,args);
  MpsTerm *newSucc = mySucc->LSubst(source,dest,args);
  MpsTerm *result =  new MpsNu(myChannel, *newSucc, *newType);

  // Clean Up
  delete newType;
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsLink::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->LSubst(source,dest,args);
  MpsTerm *result = new MpsLink(myChannel,mySession,myPid,myMaxpid,*newSucc);

  // Clean Up
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsSync::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  // LSubst each branch
  map<string, MpsTerm*> newBranches;
  for (map<string,MpsTerm*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    MpsTerm *newBranch = it->second->LSubst(source,dest,args);
    newBranches[it->first] = newBranch;
  }

  MpsTerm *result = new MpsSync(myMaxpid, mySession, newBranches, myAssertions);

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsTerm *MpsCond::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newTrueBranch = myTrueBranch->LSubst(source,dest,args);
  MpsTerm *newFalseBranch = myFalseBranch->LSubst(source,dest,args);
  MpsCond *result = new MpsCond(*myCond, *newTrueBranch, *newFalseBranch);

  // Clean Up
  delete newTrueBranch;
  delete newFalseBranch;

  return result;
} // }}}
MpsGuiSync *MpsGuiSync::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  map<string, inputbranch> newBranches;
  newBranches.clear();
  // LSubst each branch
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    inputbranch newBranch;
    newBranch.term = it->second.term->LSubst(source,dest,args);
    newBranch.assertion = it->second.assertion->Copy();
    newBranch.names = it->second.names;
    newBranch.args = it->second.args;
    newBranch.types.clear();
    for (vector<MpsMsgType*>::const_iterator type=it->second.types.begin(); type!=it->second.types.end(); ++type)
      newBranch.types.push_back((*type)->LSubst(source,dest,args));
    newBranch.values.clear();
    for (vector<MpsExp*>::const_iterator value=it->second.values.begin(); value!=it->second.values.end(); ++value)
      newBranch.values.push_back((*value)->Copy());

    newBranches[it->first] = newBranch;
  }
  MpsGuiSync *result = new MpsGuiSync(myMaxpid, mySession, myPid, newBranches);

  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second.term;
    delete newBranches.begin()->second.assertion;
    while (newBranches.begin()->second.types.size()>0)
    {
      delete *newBranches.begin()->second.types.begin();
      newBranches.begin()->second.types.erase(newBranches.begin()->second.types.begin());
    }
    while (newBranches.begin()->second.values.size()>0)
    {
      delete *newBranches.begin()->second.values.begin();
      newBranches.begin()->second.values.erase(newBranches.begin()->second.values.begin());
    }
    newBranches.erase(newBranches.begin());
  }

  return result;
} // }}}
MpsGuiValue *MpsGuiValue::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->LSubst(source,dest,args);
  MpsGuiValue *result = new MpsGuiValue(myMaxpid, mySession, myPid, *myName, *myValue, *newSucc);

  // Clean up
  delete newSucc;

  return result;
} // }}}
MpsAssign *MpsAssign::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsMsgType *newType=myType->LSubst(source,dest,args);
  MpsTerm *newSucc = mySucc->LSubst(source,dest,args);
  MpsAssign *result = new MpsAssign(myId, *myExp, *newType, *newSucc);

  // Clean Up
  delete newType;
  delete newSucc;

  return result;
} // }}}

/* Free Process Variables
 */
set<string> MpsEnd::FPV() const // {{{
{
  set<string> result;
  result.clear();
  return result;
} // }}}
set<string> MpsSnd::FPV() const // {{{
{
  set<string> result = mySucc->FPV();
  return result;
} // }}}
set<string> MpsRcv::FPV() const // {{{
{
  set<string> result = mySucc->FPV();
  return result;
} // }}}
set<string> MpsSelect::FPV() const // {{{
{
  set<string> result = mySucc->FPV();
  return result;
} // }}}
set<string> MpsBranch::FPV() const // {{{
{
  set<string> result;
  result.clear();
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    set<string> sub = it->second->FPV();
    result.insert(sub.begin(), sub.end());
  }
  return result;
} // }}}
set<string> MpsPar::FPV() const // {{{
{
  set<string> result = myLeft->FPV();
  set<string> sub = myRight->FPV();
  result.insert(sub.begin(),sub.end());
  return result;
} // }}}
set<string> MpsDef::FPV() const // {{{
{
  set<string> result = mySucc->FPV();
  set<string> result2 = myBody->FPV();
  result.insert(result2.begin(),result2.end());
  result.erase(myName);
  return result;
} // }}}
set<string> MpsCall::FPV() const // {{{
{
  set<string> result;
  result.clear();
  result.insert(myName);
  return result;
} // }}}
set<string> MpsNu::FPV() const // {{{
{
  set<string> result = mySucc->FPV();
  return result;
} // }}}
set<string> MpsLink::FPV() const // {{{
{
  set<string> result = mySucc->FPV();
  return result;
} // }}}
set<string> MpsSync::FPV() const // {{{
{
  set<string> result;
  result.clear();
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    set<string> sub = it->second->FPV();
    result.insert(sub.begin(), sub.end());
  }
  return result;
} // }}}
set<string> MpsCond::FPV() const // {{{
{
  set<string> result;
  result.clear();
  set<string> tfpv=myTrueBranch->FPV();
  set<string> ffpv=myFalseBranch->FPV();
  result.insert(tfpv.begin(), tfpv.end());
  result.insert(ffpv.begin(), ffpv.end());
  return result;
} // }}}
set<string> MpsGuiSync::FPV() const // {{{
{
  set<string> result;
  result.clear();
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    set<string> sub = it->second.term->FPV();
    result.insert(sub.begin(), sub.end());
  }
  return result;
} // }}}
set<string> MpsGuiValue::FPV() const // {{{
{
  set<string> result = mySucc->FPV();
  return result;
} // }}}
set<string> MpsAssign::FPV() const // {{{
{
  return mySucc->FPV();
} // }}}

/* Free Expression Variables
 */
set<string> MpsEnd::FEV() const // {{{
{
  set<string> result;
  result.clear();
  return result;
} // }}}
set<string> MpsSnd::FEV() const // {{{
{
  set<string> result = mySucc->FEV();
  result.insert(myChannel.GetName());
  set<string> fv=myExp->FV();
  result.insert(fv.begin(),fv.end());
  return result;
} // }}}
set<string> MpsRcv::FEV() const // {{{
{
  set<string> result = mySucc->FEV();
  result.erase(myDest);
  result.insert(myChannel.GetName());
  return result;
} // }}}
set<string> MpsSelect::FEV() const // {{{
{
  set<string> result = mySucc->FEV();
  result.insert(myChannel.GetName());
  return result;
} // }}}
set<string> MpsBranch::FEV() const // {{{
{
  set<string> result;
  result.clear();
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    set<string> sub = it->second->FEV();
    result.insert(sub.begin(), sub.end());
  }
  result.insert(myChannel.GetName());
  return result;
} // }}}
set<string> MpsPar::FEV() const // {{{
{
  set<string> result = myLeft->FEV();
  set<string> sub = myRight->FEV();
  result.insert(sub.begin(),sub.end());
  return result;
} // }}}
set<string> MpsDef::FEV() const // {{{
{
  set<string> result = mySucc->FEV();
  set<string> result2 = myBody->FEV();
  for (vector<string>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
    result2.erase(*it);
  result.insert(result2.begin(),result2.end());
  return result;
} // }}}
set<string> MpsCall::FEV() const // {{{
{
  set<string> result;
  result.clear();
  for (vector<MpsExp*>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  {
    set<string> fv=(*it)->FV();
    result.insert(fv.begin(),fv.end());
  }
  return result;
} // }}}
set<string> MpsNu::FEV() const // {{{
{
  set<string> result = mySucc->FEV();
  result.erase(myChannel);
  return result;
} // }}}
set<string> MpsLink::FEV() const // {{{
{
  set<string> result = mySucc->FEV();
  result.erase(mySession);
  result.insert(myChannel);
  return result;
} // }}}
set<string> MpsSync::FEV() const // {{{
{
  set<string> result;
  result.clear();
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    set<string> sub = it->second->FEV();
    result.insert(sub.begin(), sub.end());
  }
  result.insert(mySession);
  return result;
} // }}}
set<string> MpsCond::FEV() const // {{{
{
  set<string> result;
  result.clear();
  set<string> cfv=myCond->FV();
  set<string> tfev=myTrueBranch->FEV();
  set<string> ffev=myFalseBranch->FEV();
  result.insert(cfv.begin(), cfv.end());
  result.insert(tfev.begin(), tfev.end());
  result.insert(ffev.begin(), ffev.end());
  return result;
} // }}}
set<string> MpsGuiSync::FEV() const // {{{
{
  set<string> result;
  result.clear();
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    set<string> sub = it->second.term->FEV();
    for (vector<string>::const_iterator arg=it->second.args.begin(); arg!=it->second.args.end(); ++arg)
      sub.erase(*arg);
    result.insert(sub.begin(), sub.end());
  }
  result.insert(mySession);
  return result;
} // }}}
set<string> MpsGuiValue::FEV() const // {{{
{
  set<string> result = mySucc->FEV();
  result.insert(mySession);
  set<string> fv=myName->FV();
  result.insert(fv.begin(),fv.end());
  fv=myValue->FV();
  result.insert(fv.begin(),fv.end());
  return result;
} // }}}
set<string> MpsAssign::FEV() const // {{{
{
  set<string> result = mySucc->FEV();
  result.erase(myId);
  set<string> fv=myExp->FV();
  result.insert(fv.begin(),fv.end());
  return result;
} // }}}

/* Make deep copy
 */
MpsTerm *MpsEnd::Copy() const // {{{
{
  return new MpsEnd();
} // }}}
MpsTerm *MpsSnd::Copy() const // {{{
{
  // assert mySucc != NULL
  return new MpsSnd(myChannel, *myExp, *mySucc);
} // }}}
MpsTerm *MpsRcv::Copy() const // {{{
{
  // assert mySucc != NULL
  return new MpsRcv(myChannel, myDest, *mySucc);
} // }}}
MpsTerm *MpsSelect::Copy() const // {{{
{
  // assert mySucc != NULL
  return new MpsSelect(myChannel, myLabel, *mySucc);
} // }}}
MpsTerm *MpsBranch::Copy() const // {{{
{
  return new MpsBranch(myChannel,myBranches,myAssertions);
} // }}}
MpsTerm *MpsPar::Copy() const // {{{
{
  return new MpsPar(*myLeft, *myRight);
} // }}}
MpsTerm *MpsDef::Copy() const // {{{
{
  // assert mySucc != NULL
  return new MpsDef(myName, myArgs, myTypes, myStateArgs, myStateTypes, *myBody, *mySucc);
} // }}}
MpsTerm *MpsCall::Copy() const // {{{
{
  // assert mySucc != NULL
  return new MpsCall(myName, myArgs, myState);
} // }}}
MpsTerm *MpsNu::Copy() const // {{{
{
  // assert mySucc != NULL
  return new MpsNu(myChannel, *mySucc, *myType);
} // }}}
MpsTerm *MpsLink::Copy() const // {{{
{
  // assert mySucc != NULL
return new MpsLink(myChannel, mySession, myPid, myMaxpid, *mySucc);
} // }}}
MpsTerm *MpsSync::Copy() const // {{{
{
  return new MpsSync(myMaxpid, mySession,myBranches, myAssertions);
} // }}}
MpsTerm *MpsCond::Copy() const // {{{
{
  return new MpsCond(*myCond, *myTrueBranch, *myFalseBranch);
} // }}}
MpsGuiSync *MpsGuiSync::Copy() const // {{{
{
  return new MpsGuiSync(myMaxpid, mySession, myPid, myBranches);
} // }}}
MpsGuiValue *MpsGuiValue::Copy() const // {{{
{
  // assert mySucc != NULL
  // assert myName != NULL
  // assert myValue != NULL
  return new MpsGuiValue(myMaxpid, mySession, myPid, *myName, *myValue, *mySucc);
} // }}}
MpsAssign *MpsAssign::Copy() const // {{{
{
  return new MpsAssign(myId, *myExp, *myType, *mySucc);
} // }}}

/* Is the process terminated/completed
 */
bool MpsEnd::Terminated() const // {{{
{
  return true;
} // }}}
bool MpsSnd::Terminated() const // {{{
{
  return false;
} // }}}
bool MpsRcv::Terminated() const // {{{
{
  return false;
} // }}}
bool MpsSelect::Terminated() const // {{{
{
  return false;
} // }}}
bool MpsBranch::Terminated() const // {{{
{
  return false;
} // }}}
bool MpsPar::Terminated() const // {{{
{
  return myLeft->Terminated() && myRight->Terminated();
} // }}}
bool MpsDef::Terminated() const // {{{
{
  return false;
} // }}}
bool MpsCall::Terminated() const // {{{
{
  return false;
} // }}}
bool MpsNu::Terminated() const // {{{
{
  return mySucc->Terminated();
} // }}}
bool MpsLink::Terminated() const // {{{
{
  return false;
} // }}}
bool MpsSync::Terminated() const // {{{
{
  return false;
} // }}}
bool MpsCond::Terminated() const // {{{
{
  return false;
} // }}}
bool MpsGuiSync::Terminated() const // {{{
{
  return false;
} // }}}
bool MpsGuiValue::Terminated() const // {{{
{
  return false;
} // }}}
bool MpsAssign::Terminated() const // {{{
{
  return false;
} // }}}

/* Simplify term by removing terminated processes
 */
MpsTerm *MpsEnd::Simplify() const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsSnd::Simplify() const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->Simplify();
  MpsTerm *result = new MpsSnd(myChannel, *myExp, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsRcv::Simplify() const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->Simplify();
  MpsTerm *result = new MpsRcv(myChannel, myDest, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSelect::Simplify() const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->Simplify();
  MpsTerm *result = new MpsSelect(myChannel, myLabel, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsBranch::Simplify() const // {{{
{
  map<string,MpsTerm*> newBranches;
  newBranches.clear();
  for (map<string,MpsTerm*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
    newBranches[it->first] = it->second->Simplify();
  MpsTerm *result = new MpsBranch(myChannel,newBranches,myAssertions);
  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second;
    newBranches.erase(newBranches.begin());
  }
  return result;
} // }}}
MpsTerm *MpsPar::Simplify() const // {{{
{
  MpsTerm *newLeft = myLeft->Simplify();
  MpsTerm *newRight = myRight->Simplify();
  MpsTerm *result = NULL;
  if (newLeft->ToString() == "end")
    result = newRight->Copy();
  else if (newRight->ToString() == "end")
    result = newLeft->Copy();
  else
    result = new MpsPar(*newLeft, *newRight);
  delete newLeft;
  delete newRight;
  return result;
} // }}}
MpsTerm *MpsDef::Simplify() const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->Simplify();
  MpsTerm *newBody = myBody->Simplify();
  MpsTerm *result=NULL;
  if (newSucc->ToString() == "end")
    result = new MpsEnd();
  else
    result = new MpsDef(myName, myArgs, myTypes, myStateArgs, myStateTypes, *newBody, *newSucc);
  delete newSucc;
  delete newBody;
  return result;
} // }}}
MpsTerm *MpsCall::Simplify() const // {{{
{
  return new MpsCall(myName, myArgs, myState);
} // }}}
MpsTerm *MpsNu::Simplify() const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->Simplify();
  MpsTerm *result = new MpsNu(myChannel, *newSucc, *myType);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsLink::Simplify() const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->Simplify();
  MpsTerm *result = new MpsLink(myChannel, mySession, myPid, myMaxpid, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSync::Simplify() const // {{{
{
  map<string,MpsTerm*> newBranches;
  newBranches.clear();
  for (map<string,MpsTerm*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
    newBranches[it->first] = it->second->Simplify();
  MpsTerm *result = new MpsSync(myMaxpid,mySession,newBranches,myAssertions);
  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second;
    newBranches.erase(newBranches.begin());
  }
  return result;
} // }}}
MpsTerm *MpsCond::Simplify() const // {{{
{
  MpsTerm *newTrueBranch = myTrueBranch->Simplify();
  MpsTerm *newFalseBranch = myFalseBranch->Simplify();
  MpsTerm *result = new MpsCond(*myCond, *newTrueBranch, *newFalseBranch);
  delete newTrueBranch;
  delete newFalseBranch;
  return result;
} // }}}
MpsGuiSync *MpsGuiSync::Simplify() const // {{{
{
  map<string,inputbranch> newBranches;
  newBranches.clear();
  for (map<string,inputbranch>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    inputbranch newBranch;
    newBranch.term = it->second.term->Simplify();
    newBranch.assertion = it->second.assertion->Copy();
    newBranch.names = it->second.names;
    newBranch.args = it->second.args;
    newBranch.types.clear();
    for (vector<MpsMsgType*>::const_iterator type=it->second.types.begin(); type!=it->second.types.end(); ++type)
      newBranch.types.push_back((*type)->Copy());
    newBranch.values.clear();
    for (vector<MpsExp*>::const_iterator value=it->second.values.begin(); value!=it->second.values.end(); ++value)
      newBranch.values.push_back((*value)->Copy());
    newBranches[it->first] = newBranch;
  }
  MpsGuiSync *result = new MpsGuiSync(myMaxpid,mySession, myPid, newBranches);
  // Clean up
  while (newBranches.size()>0)
  {
    delete newBranches.begin()->second.term;
    delete newBranches.begin()->second.assertion;
    while (newBranches.begin()->second.types.size()>0)
    {
      delete *newBranches.begin()->second.types.begin();
      newBranches.begin()->second.types.erase(newBranches.begin()->second.types.begin());
    }
    while (newBranches.begin()->second.values.size()>0)
    {
      delete *newBranches.begin()->second.values.begin();
      newBranches.begin()->second.values.erase(newBranches.begin()->second.values.begin());
    }
    newBranches.erase(newBranches.begin());
  }
  return result;
} // }}}
MpsGuiValue *MpsGuiValue::Simplify() const // {{{
{
  // assert mySucc != NULL
  // assert myValue != NULL
  MpsTerm *newSucc = mySucc->Simplify();
  MpsGuiValue *result = new MpsGuiValue(myMaxpid, mySession, myPid, *myName, *myValue, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsAssign *MpsAssign::Simplify() const // {{{
{
  MpsTerm *newSucc = mySucc->Simplify();
  MpsAssign *result = new MpsAssign(myId, *myExp, *myType, *newSucc);
  delete newSucc;
  return result;
} // }}}

/* Make parsable string representation of term
 */
string MpsEnd::ToString(string indent) const // {{{
{
  return "end";
} // }}}
string MpsSnd::ToString(string indent) const // {{{
{
  return myChannel.ToString() + " << " + myExp->ToString() + ";\n" + indent + mySucc->ToString(indent);
} // }}}
string MpsRcv::ToString(string indent) const // {{{
{
  return myChannel.ToString() + " >> " + myDest + ";\n" + indent + mySucc->ToString(indent);
} // }}}
string MpsSelect::ToString(string indent) const // {{{
{
  return myChannel.ToString() + " << " + myLabel + ";\n" + indent + mySucc->ToString(indent);
} // }}}
string MpsBranch::ToString(string indent) const // {{{
{
  string newIndent = indent + "  ";
  string result = myChannel.ToString() + " >>\n" + indent + "{ ";
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    if (it != myBranches.begin())
      result += ",\n" + newIndent;
    result += it->first;
    map<string,MpsExp*>::const_iterator ass=myAssertions.find(it->first);
    if (ass != myAssertions.end())
      result += + "[[" + ass->second->ToString() + "]]";
    result += (string)":\n" + newIndent + it->second->ToString(newIndent);
  }
  result += "\n" + indent + "}";
  return result;
} // }}}
string MpsPar::ToString(string indent) const // {{{
{
  string newIndent = indent + "  ";
  return (string)"( " + myLeft->ToString(newIndent) + "\n" + indent + "| " + myRight->ToString(newIndent) + "\n" + indent + ")";
} // }}}
string MpsDef::ToString(string indent) const // {{{
{
  string newIndent = indent + "   ";
  string typeIndent = indent + "     ";
  for (int i=0; i<myName.size();++i)
    typeIndent+=" ";
  string result = (string)"def " + myName;
  if (myStateArgs.size()>0)
  {
    result += "<";
    for (int i=0; i<myStateArgs.size() && i<myStateTypes.size(); ++i)
    {
      if (i>0)
        result += ",\n" + typeIndent;
      string newTypeIndent = typeIndent + "  ";
      for (int j=0; j<myStateArgs[i].size();++j)
        newTypeIndent+=" ";
      result += myStateArgs[i] + ": " + myStateTypes[i]->ToString(newTypeIndent);
    }
    result += ">\n" + typeIndent.substr(1);
  }
  result += "(";
  for (int i=0; i<myArgs.size() && i<myTypes.size(); ++i)
  {
    if (i>0)
      result += ",\n" + typeIndent;
    string newTypeIndent = typeIndent + "  ";
    for (int j=0; j<myArgs[i].size();++j)
      newTypeIndent+=" ";
    result += myArgs[i] + ": " + myTypes[i]->ToString(newTypeIndent);
  }
  result += ")";
  result += (string)"=\n" + newIndent + myBody->ToString(newIndent);
  result += (string)"\n" + indent + "in " + mySucc->ToString(newIndent);
  return result;
} // }}}
string MpsCall::ToString(string indent) const // {{{
{
  string result = myName;
  if (myState.size()>0)
  {
    result += "<";
    for (vector<MpsExp*>::const_iterator it=myState.begin(); it!=myState.end(); ++it)
    {
      if (it != myState.begin())
        result += ", ";
      result += (*it)->ToString();
    }
    result += ">";
  }
  result += "(";
  for (vector<MpsExp*>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  {
    if (it != myArgs.begin())
      result += ", ";
    result += (*it)->ToString();
  }
  result += ")";
  return result;
} // }}}
string MpsNu::ToString(string indent) const // {{{
{
  string typeIndent = indent + "     ";
  for (int i=0; i<myChannel.size();++i)
    typeIndent+=" ";
  string newIndent = indent + "  ";
  return (string)"(nu " + myChannel + ":" + myType->ToString(typeIndent) + ")\n" + newIndent + mySucc->ToString(newIndent);
} // }}}
string MpsLink::ToString(string indent) const // {{{
{
  return (string) "link(" + int2string(myMaxpid) + "," + myChannel + "," + mySession + "," + int2string(myPid) + ");\n" + indent + mySucc->ToString(indent);
} // }}}
string MpsSync::ToString(string indent) const // {{{
{
  string newIndent = indent + "    ";
  string result = (string)"sync(" + int2string(myMaxpid) + "," + mySession + ")\n" + indent + "{ ";
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    if (it != myBranches.begin())
      result += ",\n" + indent + "  ";
    result += it->first;
    map<string,MpsExp*>::const_iterator ass=myAssertions.find(it->first);
    if (ass != myAssertions.end())
      result += "[[" + ass->second->ToString() + "]]";
    result += (string)":\n" + newIndent + it->second->ToString(newIndent);
  }
  result += "\n" + indent + "}";
  return result;
} // }}}
string MpsCond::ToString(string indent) const // {{{
{
  string result = (string)"if " + myCond->ToString() + "\n"
                + indent + "then " + myTrueBranch->ToString(indent + "     ") + "\n"
                + indent + "else " + myFalseBranch->ToString(indent + "     ");
  return result;
} // }}}
string MpsGuiSync::ToString(string indent) const // {{{
{
  string newIndent = indent + "    ";
  string result = (string)"guisync(" + int2string(myMaxpid) + "," + mySession + "," + int2string(myPid) + ")\n" + indent + "{ ";
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    if (it != myBranches.begin())
      result += ",\n" + indent + "  ";
    result += it->first + "[[" + it->second.assertion->ToString() + "]](";
    for (int i=0; i<it->second.args.size() && i<it->second.names.size() && i<it->second.types.size(); ++i)
    {
      if (i>0)
        result += ",";
      result += it->second.args[i] + "=" + it->second.values[i]->ToString() + ": " +it->second.types[i]->ToString();
    }
    result += "):\n" + newIndent + it->second.term->ToString(newIndent);
  }
  result += "\n" + indent + "}";
  return result;
} // }}}
string MpsGuiValue::ToString(string indent) const // {{{
{
  return (string)"guivalue(" + int2string(myMaxpid) + "," + mySession + "," + int2string(myPid) + "," + myName->ToString() + "," + myValue->ToString() + ");\n" + indent + mySucc->ToString(indent);
} // }}}
string MpsAssign::ToString(string indent) const // {{{
{
  return myId + ":" + myType->ToString() + "=" + myExp->ToString() + ";\n" + indent + mySucc->ToString();
} // }}}

/* Make string representation of term with latex markup
 */
string MpsEnd::ToTex(int indent, int sw) const // {{{
{
  return ToTex_KW("end");
} // }}}
string MpsSnd::ToTex(int indent, int sw) const // {{{
{
  return ToTex_Channel(myChannel) + "$\\ll$" + myExp->ToString() + ";\\newline\n"
       + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
} // }}}
string MpsRcv::ToTex(int indent, int sw) const // {{{
{
  return ToTex_Channel(myChannel) + "$\\gg$" + myDest + ";\\newline\n"
       + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
} // }}}
string MpsSelect::ToTex(int indent, int sw) const // {{{
{
  return ToTex_Channel(myChannel) + "$\\ll$ "
       + ToTex_Label(myLabel) + ";\\newline\n"
       + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
} // }}}
string MpsBranch::ToTex(int indent, int sw) const // {{{
{
  string result = ToTex_Channel(myChannel) + "$\\gg$\\newline\n"
                + ToTex_Hspace(indent,sw) + "\\{ ";
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    if (it != myBranches.begin())
      result += ",\\newline\n"
              + ToTex_Hspace(indent+2,sw);
    result += ToTex_Label(it->first);
    map<string,MpsExp*>::const_iterator ass=myAssertions.find(it->first);
    if (ass != myAssertions.end() && ass->second->ToString()!="true")
      result += + "$\\llbracket$" + ass->second->ToString() + "$\\rrbracket$";
    result += (string)":\\newline\n"
            + ToTex_Hspace(indent+2,sw) + it->second->ToTex(indent + 2,sw);
  }
  result += ToTex_Hspace(indent,sw) + "\\}";
  return result;
} // }}}
string MpsPar::ToTex(int indent, int sw) const // {{{
{
  return (string)"( " + myLeft->ToTex(indent+2,sw) + "\\newline\n"
       + ToTex_Hspace(indent,sw) + "| " + myRight->ToTex(indent+2,sw) + "\\newline\n"
       + ToTex_Hspace(indent,sw) + ")";
} // }}}
string MpsDef::ToTex(int indent, int sw) const // {{{
{
  int typeIndent = indent + 5 + myName.size();
  string result = ToTex_KW("def") + " " + ToTex_Var(myName);
  if (myStateArgs.size()>0)
  {
    result += "$\\langle$";
    for (int i=0; i<myStateArgs.size() && i<myStateTypes.size(); ++i)
    {
      if (i>0)
        result += ",\\newline\n" + ToTex_Hspace(typeIndent,sw);
      int newTypeIndent = typeIndent + 2 + myStateArgs[i].size();
      result += myStateArgs[i] + ": " + myStateTypes[i]->ToTex(newTypeIndent,sw);
    }
    result += "$\\rangle$\\newline\n" + ToTex_Hspace(typeIndent-1,sw);
  }
  if (myArgs.size()>0)
  {
    result += "(";
    for (int i=0; i<myArgs.size() && i<myTypes.size(); ++i)
    {
      if (i>0)
        result += ",\\newline\n" + ToTex_Hspace(typeIndent,sw);
      int newTypeIndent = typeIndent + 2 + myArgs[i].size();
      result += myArgs[i] + ": " + myTypes[i]->ToTex(newTypeIndent,sw);
    }
    result += ")";
  }
  result += (string)"=\\newline\n"
          + ToTex_Hspace(indent+2,sw) + myBody->ToTex(indent+2,sw) + "\\newline\n"
          + ToTex_Hspace(indent,sw) + ToTex_KW("in") + " " + mySucc->ToTex(indent+3,sw);
  return result;
} // }}}
string MpsCall::ToTex(int indent, int sw) const // {{{
{
  string result = ToTex_Var(myName);
  if (myState.size()>0)
  {
    result += "$\\langle$";
    for (vector<MpsExp*>::const_iterator it=myState.begin(); it!=myState.end(); ++it)
    {
      if (it != myState.begin())
        result += ", ";
      result += (*it)->ToString();
    }
    result += "$\\rangle$";
  }
  result += "(";
  for (vector<MpsExp*>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  {
    if (it != myArgs.begin())
      result += ", ";
    result += (*it)->ToString();
  }
  result += ")";
  return result;
} // }}}
string MpsNu::ToTex(int indent, int sw) const // {{{
{
  int typeIndent = indent + 5 + myChannel.size();
  return "(" + ToTex_KW("nu") + " " + ToTex_Session(myChannel) + ":" + myType->ToTex(typeIndent,sw) + ")\\newline\n"
       + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
} // }}}
string MpsLink::ToTex(int indent, int sw) const // {{{
{
  return ToTex_KW("link") + "(" + ToTex_PP(myMaxpid) + "," + ToTex_ChName(myChannel) + "," + ToTex_Session(mySession) + "," + ToTex_PP(myPid) + ");\\newline\n"
       + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
} // }}}
string MpsSync::ToTex(int indent, int sw) const // {{{
{
  int newIndent = indent + 4;
  string result = ToTex_KW("sync") + "(" + ToTex_PP(myMaxpid) + "," + ToTex_Session(mySession) + ")\\newline\n"
                + ToTex_Hspace(indent,sw) + "\\{ ";
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    if (it != myBranches.begin())
      result += ",\\newline\n"
              + ToTex_Hspace(indent+2,sw);
    result += ToTex_Label(it->first);
    map<string,MpsExp*>::const_iterator ass=myAssertions.find(it->first);
    if (ass != myAssertions.end() && ass->second->ToString()!="true")
      result += "$\\llbracket$" + ass->second->ToString() + "$\\rrbracket$";
    result += (string)":\\newline\n"
            + ToTex_Hspace(newIndent,sw) + it->second->ToTex(newIndent,sw);
  }
  result += "\\newline\n"
          + ToTex_Hspace(indent,sw) + "\\}";
  return result;
} // }}}
string MpsCond::ToTex(int indent, int sw) const // {{{
{
  string result = ToTex_KW("if") +" " + myCond->ToString() + "\\newline\n"
                + ToTex_Hspace(indent,sw) + ToTex_KW("then") + " " + myTrueBranch->ToTex(indent + 5,sw) + "\\newline\n"
                + ToTex_Hspace(indent,sw) + ToTex_KW("else") + " " + myFalseBranch->ToTex(indent + 5,sw);
  return result;
} // }}}
string MpsGuiSync::ToTex(int indent, int sw) const // {{{
{
  int newIndent = indent + 4;
  string result = ToTex_KW("guisync") + "(" + ToTex_PP(myMaxpid) + "," + ToTex_Session(mySession) + "," + ToTex_PP(myPid) + ")\\newline\n"
                + ToTex_Hspace(indent,sw) + "\\{ ";
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    if (it != myBranches.begin())
      result += ",\\newline\n"
              + ToTex_Hspace(indent+2,sw);
    result += ToTex_Label(it->first);
    if (it->second.assertion->ToString()!="true")
      result += "$\\llbracket$" + it->second.assertion->ToString() + "$\\rrbracket$";
    result += "(";
    for (int i=0; i<it->second.args.size() && i<it->second.names.size() && i<it->second.types.size(); ++i)
    {
      if (i>0)
        result += ",";
      result += it->second.args[i] + "=" + it->second.values[i]->ToString() + ": " +it->second.types[i]->ToString();
    }
    result += "):\\newline\n"
            + ToTex_Hspace(newIndent,sw) + it->second.term->ToTex(newIndent,sw);
  }
  result += "\\newline\n"
          + ToTex_Hspace(indent,sw) + "\\}";
  return result;
} // }}}
string MpsGuiValue::ToTex(int indent, int sw) const // {{{
{
  return ToTex_KW("guivalue") + "(" + ToTex_PP(myMaxpid) + ","
       + ToTex_Session(mySession) + ","
       + ToTex_PP(myPid) + ","
       + myName->ToString() + ","
       + myValue->ToString() + ");\\newline\n"
       + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
} // }}}
string MpsAssign::ToTex(int indent, int sw) const // {{{
{
  return ToTex_Var(myId) + ":" + myType->ToTex(indent + myId.size() + 1) + "=" + myExp->ToString() + ";\\newline\n"
       + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
} // }}}

