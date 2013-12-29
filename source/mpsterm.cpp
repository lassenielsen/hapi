#include <apims/mpstype.hpp>
#include <dpl/parser.hpp>
#include <apims/mpsterms.hpp>
#include <apims/mpsgui.hpp>

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
  return new MpsCall(name, arg, arg, vector<MpsMsgType*>(), vector<MpsMsgType*>());
} // }}}
void MpsTerm::FreeLink(const string &name) // {{{
{ myFreeLinks.push_back(name);
} // }}}
const vector<string> &MpsTerm::GetFreeLinks() const // {{{
{ return myFreeLinks;
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
  MpsParser.DefToken("host", "HOST",2); // Indicate host statement
  MpsParser.DefToken("hostheader", "HOSTHEADER",2); // Indicate host header
  /*** Define grammars ***/
  // Expression Grammar
  MpsParser.DefType(MpsExp::BNF_EXP);
  MpsParser.DefType(MpsExp::BNF_EXPS);                             // Expression Tuple
  MpsParser.DefType(MpsExp::BNF_EXPS2);                            // Nonempty Expression Tuple
  MpsParser.DefType(MpsGlobalType::BNF_ASSERTION);                 // Assertion
  MpsParser.DefType(MpsGlobalType::BNF_NAMEDASSERTION);            // Name and Assertion
  MpsParser.DefType(MpsGlobalType::BNF_LABEL);                     // Branch Label
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
  MpsParser.DefType(MpsGlobalType::BNF_TARGS3);                    // Nonempty Targ list
  MpsParser.DefType(MpsGlobalType::BNF_TARG);                      // Typed argument with default value
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
                                                                   // Processes
  MpsParser.DefType("pi ::= ( nu id : Gtype ) pi \
                          | pi2 par pi \
                          | pi2");
  MpsParser.DefType("pi2 ::= ( pi ) \
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
                           | ch >> id @ ( int of int ) ; pi2 \
                           | host ( exps ) ; pi2 \
                           | hostheader ( exps ) ; pi2");         // More processes

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
void HostStmt(vector<string> &hostParts, vector<MpsExp*> &expParts, const parsed_tree *hstmt) // {{{
{ if (hstmt->type_name == "hstmt" && hstmt->case_name == "case1") // string
  { hostParts.push_back(hstmt->content[0]->root.content.substr(1,hstmt->content[0]->root.content.size()-1));
    return;
  }
  else if (hstmt->type_name == "hstmt" && hstmt->case_name == "case2") // string , exp , hstmt
  { hostParts.push_back(hstmt->content[0]->root.content.substr(1,hstmt->content[0]->root.content.size()-1));
    expParts.push_back(MpsExp::Create(hstmt->content[2]));
    HostStmt(hostParts, expParts, hstmt->content[4]);
    return;
  }

#if APIMS_DEBUG_LEVEL>1
  cerr << "Unknown name list parsetree: " << hstmt->type_name << "." << hstmt->case_name << endl;
#endif
  return;
} // }}}
MpsTerm *MpsTerm::Create(const parsed_tree *exp) // {{{
{
  if (exp->type_name == "pi" && exp->case_name == "case1") // ( nu id : Gtype ) pi {{{
  {
    MpsTerm *succ = MpsTerm::Create(exp->content[6]);
    MpsGlobalType *type = MpsGlobalType::Create(exp->content[4]);
    MpsTerm *result = new MpsNu(exp->content[2]->root.content, *succ, *type);
    delete succ;
    delete type;
    return result;
  } // }}}
  else if (exp->type_name == "pi" && exp->case_name == "case2") // pi par pi {{{
  {
    MpsTerm *left = MpsTerm::Create(exp->content[0]);
    MpsTerm *right = MpsTerm::Create(exp->content[2]);
    MpsTerm *result = new MpsPar(*left, *right,vector<string>(),vector<string>());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (exp->type_name == "pi" && exp->case_name == "case3") // pi2 {{{
  {
    return MpsTerm::Create(exp->content[0]);
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case1") // ( pi ) {{{
  {
    return MpsTerm::Create(exp->content[1]);
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case2") // ch << exp ; pi2 {{{
  {
    MpsTerm *succ = MpsTerm::Create(exp->content[4]);
    MpsExp *value = MpsExp::Create(exp->content[2]);
    MpsChannel dest = ParseChannel(exp->content[0]);
    MpsTerm *result = new MpsSnd(dest, *value, *succ, MpsMsgNoType(), false);
    delete succ;
    delete value;
    return result;
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case3") // ch >> id ; pi2 {{{
  {
    MpsTerm *succ = MpsTerm::Create(exp->content[4]);
    MpsChannel source=ParseChannel(exp->content[0]);
    MpsTerm *result = new MpsRcv(source, exp->content[2]->root.content, -1, -1, *succ, MpsMsgNoType(), false);

    delete succ;
    return result;
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case4") // ch << bid ; pi2 {{{
  {
    MpsTerm *succ = MpsTerm::Create(exp->content[4]);
    MpsChannel dest = ParseChannel(exp->content[0]);
    MpsTerm *result = new MpsSelect(dest, exp->content[2]->root.content, *succ, false);
    delete succ;
    return result;
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case5") // ch >> { branches } {{{
  {
    map<string, MpsTerm*> branches;
    branches.clear();
    map<string, MpsExp*> assertions;
    assertions.clear();
    FindBranches(exp->content[3], branches, assertions);
    MpsChannel source=ParseChannel(exp->content[0]);
    // BRANCHE TERMS DOES NOT USE ASSERTIONS
    MpsTerm *result = new MpsBranch(source, branches, vector<string>());

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
  else if (exp->type_name == "pi2" && exp->case_name == "case6") // def pvar dargs ( args ) = pi in pi2 {{{
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
    MpsTerm *result = new MpsDef(exp->content[1]->root.content, args, types, stateargs, statetypes, *body, *succ, MpsMsgEnv());
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
  else if (exp->type_name == "pi2" && exp->case_name == "case7") // pvar dexps ( exps ) {{{
  {
    vector<MpsExp*> args;
    args.clear();
    FindExps(exp->content[3],args);
    vector<MpsExp*> state;
    state.clear();
    FindExps(exp->content[1],state);
    MpsTerm *result = new MpsCall(exp->content[0]->root.content, args, state, vector<MpsMsgType*>(), vector<MpsMsgType*>());
    DeleteVector(args);
    DeleteVector(state);
    return result;
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case8") // link ( exps ) ; pi2 {{{
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
      DeleteVector(args);
      return result;
    }
    else
    {
      DeleteVector(args);
#if APIMS_DEBUG_LEVEL>1
      cerr << "Parsing error: Arguments for link must be: int, id, id, int" << endl;
#endif
      return new MpsEnd();
    }
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case9") // sync ( exps ) { branches } {{{
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
  else if (exp->type_name == "pi2" && exp->case_name == "case10") // end {{{
  {
    return new MpsEnd();
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case11") // if exp then pi2 else pi2 {{{
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
  else if (exp->type_name == "pi2" && exp->case_name == "case12") // guisync ( exps ) { inputbranches } {{{
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
  else if (exp->type_name == "pi2" && exp->case_name == "case13") // guivalue ( exps ) ; pi2 {{{
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
  else if (exp->type_name == "pi2" && exp->case_name == "case14") // id : Mtype = exp ; pi2 {{{
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
  else if (exp->type_name == "pi2" && exp->case_name == "case15") // ch >> id @ ( int of int ) ; pi2 {{{
  {
    MpsTerm *succ = MpsTerm::Create(exp->content[10]);
    MpsChannel source=ParseChannel(exp->content[0]);
    MpsTerm *result = new MpsRcv(source,
                                 exp->content[2]->root.content,
                                 string2int(exp->content[5]->root.content),
                                 string2int(exp->content[7]->root.content),
                                 *succ,
                                 MpsMsgNoType(),
                                 false);

    delete succ;
    return result;
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case16") // host ( exps ) ; pi2 {{{
  {
    MpsTerm *succ = MpsTerm::Create(exp->content[5]);
    vector<MpsExp*> args;
    args.clear();
    FindExps(exp->content[2],args);
    vector<string> hostParts;
    vector<MpsExp*> expParts;
    // exps must be alternately string and var
    for (int i=0; i<args.size(); ++i)
    { if (i%2==0) // expect string
      { MpsStringVal *val=dynamic_cast<MpsStringVal*>(args[i]);
        if (val==NULL) // Not string value
        {
#if APIMS_DEBUG_LEVEL>1
          cerr << "Parsing error: Arguments for HOST must be string and variable alternately" << endl;
#endif
          break;
        }
        else
        { hostParts.push_back(val->GetValue());
        }
      }
      else
        expParts.push_back(args[i]->Copy());
    }

    MpsHostStatement *result = new MpsHostStatement(hostParts,expParts,*succ,vector<MpsMsgType*>());
    // Clean up
    delete succ;
    DeleteVector(expParts);
    DeleteVector(args);
    return result;
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case17") // hostheader ( exps ) ; pi2 {{{
  {
    MpsTerm *succ = MpsTerm::Create(exp->content[5]);
    vector<MpsExp*> args;
    args.clear();
    FindExps(exp->content[2],args);
    if (args.size()!=1)
    {
#if APIMS_DEBUG_LEVEL>1
      cerr << "Parsing error: HOSTHEADER takes exactly 1 argument" << endl;
#endif
      delete succ;
      DeleteVector(args);
      return new MpsEnd();
    }
    MpsStringVal *header=dynamic_cast<MpsStringVal*>(args[0]);
    if (header==NULL)
    {
#if APIMS_DEBUG_LEVEL>1
      cerr << "Parsing error: Arguments for HOSTHEADER must be a string" << endl;
#endif
      delete succ;
      DeleteVector(args);
      return new MpsEnd();
    }

    MpsHostHeader *result = new MpsHostHeader(header->ToString(),*succ);
    // Clean up
    delete succ;
    DeleteVector(args);
    return result;
  } // }}}

#if APIMS_DEBUG_LEVEL>1
  cerr << "Unknown term parsetree: " << exp->type_name << "." << exp->case_name << endl;
#endif
  return new MpsEnd();
} // }}}

/* Static type-checking of deadlock and communication safety
 */
bool MpsTerm::TypeCheck() // {{{
{
  // Create environments
  MpsBoolVal Theta(true);
  MpsMsgEnv Gamma;
  MpsProcEnv Omega;
  return TypeCheck(Theta,Gamma,Omega);
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
MpsTerm *MpsTerm::ApplySnd(const std::string &path, MpsExp **val, MpsChannel &ch) const // {{{
{ return Error((string)"Applying Snd on "+ToString());
} // }}}
MpsTerm *MpsTerm::ApplyBRcv(const std::string &path, const std::string &label) const // {{{
{ return Error((string)"Applying BRcv on "+ToString());
} // }}}
MpsTerm *MpsTerm::ApplyBSnd(const std::string &path, std::string &label, MpsChannel &ch) const // {{{
{ return Error((string)"Applying BSnd on "+ToString());
} // }}}
MpsTerm *MpsTerm::ApplyLink(const std::vector<std::string> &paths, const std::string &session) const // {{{
{ if (paths.size()>0)
    return Error((string)"Applying Link on "+ToString());
  else
    return Copy();
} // }}}
MpsTerm *MpsTerm::ApplySync(const std::vector<std::string> &paths, const std::string &label) const // {{{
{ if (paths.size()>0)
    return Error((string)"Applying Sync on "+ToString());
  else
    return Copy();
} // }}}
MpsTerm *MpsTerm::ApplyDef(const std::string &path, std::vector<MpsFunction> &dest) const // {{{
{ return Error((string)"Applying Def on "+ToString());
} // }}}
MpsTerm *MpsTerm::ApplyCall(const std::string &path, const std::vector<MpsFunction> &funs) const // {{{
{ return Error((string)"Applying Call on "+ToString());
} // }}}
MpsTerm *MpsTerm::ApplyOther(const std::string &path) const // {{{
{ return Error((string)"Applying Other on "+ToString());
} // }}}

int _compile_id=0;
/* Make executable C++ code for processes
 */
string MpsTerm::MakeC() const // {{{
{ _compile_id=1;
  MpsTerm *step1=RenameAll();
  MpsTerm *step2=step1->CloseDefinitions();
  delete step1;
  MpsFunctionEnv defs;
  // Move definitions to global env
  MpsTerm *main=step2->ExtractDefinitions(defs);
  delete step2;
  string result = (string)
         "#include <iostream>\n"
       + "#include <vector>\n"
       + "#include <libpi/session_fifo.hpp>\n"
       + "#include <libpi/value.hpp>\n"
       + main->ToCHeader()
       + DefEnvToCHeader(defs)
       + "using namespace std;\n"
       + "using namespace libpi;\n\n"
       + DefEnvToC(defs)
       + "\n\n/* Main process */\ninline Cnt *__MAIN__()\n{\n"
       + main->ToC()
       + "}"
       + "\n\n/*Start process, and its continuations */\n"
       + "int main()\n"
       + "{ try {\n"
       + "    Cnt *cnt = __MAIN__();\n"
       + "    while (!cnt->IsEmpty())\n"
       + "    { Cnt *cnt2=cnt->Run();\n"
       + "      delete cnt;\n"
       + "      cnt=cnt2;\n"
       + "    }\n"
       + "    delete cnt;\n"
       + "  } catch (const string &error) {\n"
       + "    cerr << \"Error: \" << error << endl;\n"
       + "    return 1;\n"
       + "  }\n"
       + "  return 0;\n"
       + "}";
  delete main;
  return result;
} // }}}
