#include <apims/mpstype.hpp>

#include "common.cpp"
#include <algorithm>

using namespace std;
using namespace apims;
using namespace dpl;

int MpsLocalType::ourNextId = 1;
int MpsGlobalType::ourNextId = 1;

// Global Type BNFs {{{
const string MpsGlobalType::BNF_TVALS=
"Tvals ::= < exps > |";
const string MpsGlobalType::BNF_TARG=
"Targ ::= id : Mtype = exp";
const string MpsGlobalType::BNF_TARGS=
"Targs ::= < Targs2 > |";
const string MpsGlobalType::BNF_TARGS2=
"Targs2 ::= Targs3 |";
const string MpsGlobalType::BNF_TARGS3=
"Targs3 ::= Targ | Targ , Targs3";
const string MpsGlobalType::BNF_ASSERTION=
"Assertion ::= [[ exp ]] |";
const string MpsGlobalType::BNF_NAMEDASSERTION=
"NamedAssertion ::= as id Assertion |";
const string MpsGlobalType::BNF_LABEL=
"Label ::= bid Assertion";
const string MpsGlobalType::BNF_GTYPE=
"Gtype ::= int => int < Mtype > NamedAssertion ; Gtype \
         | int => int { Gbranches } \
         | rec gvar Targs . Gtype \
         | gvar Tvals \
         | Gend \
         | { Gbranches }";
const string MpsGlobalType::BNF_GBRANCHES=
"Gbranches ::= Label : Gtype \
             | Label : Gtype , Gbranches";
// }}}
// Local Type BNFs {{{
const string MpsLocalType::BNF_LTYPE=
"Ltype ::= int << < Mtype > NamedAssertion ; Ltype \
         | int >> < Mtype > NamedAssertion ; Ltype \
         | forall id [[ exp ]] ; Ltype \
         | int << { Lbranches } \
         | int >> { Lbranches } \
         | rec lvar Targs . Ltype \
         | lvar Tvals \
         | Lend \
         | { Lbranches }";
const string MpsLocalType::BNF_LBRANCHES=
"Lbranches ::= Label : Ltype \
                                 | Label : Ltype , Lbranches";
// }}}
// Message Type BNFs {{{
const string MpsMsgType::BNF_STYPE=
"Stype ::= Int | String | Bool | ( Stypes )";
const string MpsMsgType::BNF_STYPES=
"Stypes ::= Stypes2 |";
const string MpsMsgType::BNF_STYPES2=
"Stypes2 ::= Stype , Stypes2 | Stype";
const string MpsMsgType::BNF_MTYPE=
"Mtype ::= Stype \
         | < Gtype > \
         | Ltype @ ( int of int ) \
         | Gtype @ ( int of int )";
// }}}
string MpsLocalType::NewLVar(string basename) // {{{
{
  string result = (string)"~%"+basename;
  result += int2string(ourNextId);
  return result;
} // }}}
string MpsGlobalType::NewGVar(string orig) // {{{
{
  string result = "~$x";
  result += int2string(ourNextId);
  return result;
} // }}}

/* Parser and helper functions
 *
 */
extern void SetAssertion(const parsed_tree *tree,  map<string,MpsExp*> &assertions); // From mpsterm.cpp
extern void FindExps(const parsed_tree *tree, vector<MpsExp*> &dest);                // From mpsterm.cpp
void FindGlobalBranches(const parsed_tree *tree, map<string,MpsGlobalType*> &dest, map<string,MpsExp*> &assertions) // {{{
{
  if (tree->type_name == "Gbranches" && tree->case_name == "case1") // Label : Gtype {{{
  {
    dest[tree->content[0]->content[0]->root.content] = MpsGlobalType::Create(tree->content[2]);
    SetAssertion(tree->content[0],assertions);
  } // }}}
  else if (tree->type_name == "Gbranches" && tree->case_name == "case2") // Label : Gtype , Gbranches {{{
  {
    dest[tree->content[0]->content[0]->root.content] = MpsGlobalType::Create(tree->content[2]);
    SetAssertion(tree->content[0],assertions);
    FindGlobalBranches(tree->content[4],dest,assertions);
  } // }}}
  else // unknown {{{
  {
#if APIMS_DEBUG_LEVEL>1
    cerr << "Unknown globalbranch constructor: " << tree->type_name << "." << tree->case_name << endl;
#endif
  } // }}}
} // }}}
void FindLocalBranches(const parsed_tree *tree, map<string,MpsLocalType*> &dest, map<string,MpsExp*> &assertions) // {{{
{
  if (tree->type_name == "Lbranches" && tree->case_name == "case1") // Label : Ltype {{{
  {
    dest[tree->content[0]->content[0]->root.content] = MpsLocalType::Create(tree->content[2]);
    SetAssertion(tree->content[0],assertions);
  } // }}}
  else if (tree->type_name == "Lbranches" && tree->case_name == "case2") // Label : Ltype , Lbranches {{{
  {
    dest[tree->content[0]->content[0]->root.content] = MpsLocalType::Create(tree->content[2]);
    SetAssertion(tree->content[0],assertions);
    FindLocalBranches(tree->content[4],dest,assertions);
  } // }}}
  else // unknown {{{
  {
#if APIMS_DEBUG_LEVEL>1
    cerr << "Unknown localbranch constructor: " << tree->type_name << "." << tree->case_name << endl;
#endif
  } // }}}
} // }}}
void FindMsgTypes(const parsed_tree *tree, vector<MpsMsgType*> &dest) // {{{
{
  if (tree->type_name == "Stypes" && tree->case_name == "case1") // Stypes2 {{{
  {
    FindMsgTypes(tree->content[0],dest);
  } // }}}
  else if (tree->type_name == "Stypes" && tree->case_name == "case2") // {{{
  {
  } // }}}
  else if (tree->type_name == "Stypes2" && tree->case_name == "case1") // Stype , Stypes2{{{
  {
    dest.push_back(MpsMsgType::Create(tree->content[0]));
    FindMsgTypes(tree->content[2],dest);
  } // }}}
  else if (tree->type_name == "Stypes2" && tree->case_name == "case2") // Stype {{{
  {
    dest.push_back(MpsMsgType::Create(tree->content[0]));
  } // }}}
  else // unknown {{{
  {
#if APIMS_DEBUG_LEVEL>1
    cerr << "Unknown MsgTuple constructor: " << tree->type_name << "." << tree->case_name << endl;
#endif
  } // }}}
} // }}}
MpsMsgType *MpsMsgType::Create(const parsed_tree *tree) // {{{
{
  if (tree->type_name == "Mtype" && tree->case_name == "case1") // Stype {{{
  {
    return MpsMsgType::Create(tree->content[0]);
  } // }}}
  else if (tree->type_name == "Mtype" && tree->case_name == "case2") // < Gtype > {{{
  {
    MpsGlobalType *gtype = MpsGlobalType::Create(tree->content[1]);
    MpsChannelMsgType *result = new MpsChannelMsgType(*gtype);
    // Clean up
    delete gtype;

    return result;
  } // }}}
  else if (tree->type_name == "Mtype" && tree->case_name == "case3") // Ltype @ ( int of int ) {{{
  {
    MpsLocalType *ltype = MpsLocalType::Create(tree->content[0]);
    int pid = string2int(tree->content[3]->root.content);
    int maxpid = string2int(tree->content[5]->root.content);
    MpsDelegateMsgType *result = new MpsDelegateLocalMsgType(*ltype,pid,maxpid);
    // Clean up
    delete ltype;

    return result;
  } // }}}
  else if (tree->type_name == "Mtype" && tree->case_name == "case4") // Gtype @ ( int of int ) {{{
  {
    MpsGlobalType *gtype = MpsGlobalType::Create(tree->content[0]);
    int pid = string2int(tree->content[3]->root.content);
    int maxpid = string2int(tree->content[5]->root.content);
    MpsDelegateGlobalMsgType *result = new MpsDelegateGlobalMsgType(*gtype,pid,maxpid);

    // Clean up
    delete gtype;

    return result;
  } // }}}
  else if (tree->type_name == "Stype" && tree->case_name == "case1") // Int {{{
  {
    MpsIntMsgType *result = new MpsIntMsgType();
    return result;
  } // }}}
  else if (tree->type_name == "Stype" && tree->case_name == "case2") // String {{{
  {
    MpsStringMsgType *result = new MpsStringMsgType();
    return result;
  } // }}}
  else if (tree->type_name == "Stype" && tree->case_name == "case3") // Bool {{{
  {
    MpsBoolMsgType *result = new MpsBoolMsgType();
    return result;
  } // }}}
  else if (tree->type_name == "Stype" && tree->case_name == "case4") // ( Stypes ) {{{
  {
    vector<MpsMsgType*> elements;
    elements.clear();
    FindMsgTypes(tree->content[1],elements);
    if (elements.size()==1) // Single type
      return elements[0];
    else
    {
      MpsTupleMsgType *result = new MpsTupleMsgType(elements);
      DeleteVector(elements);
      return result;
    }
  } // }}}
  
#if APIMS_DEBUG_LEVEL>1
  cerr << "Unknown msg-type parsetree: " << tree->type_name << "." << tree->case_name << endl;
#endif
  return new MpsIntMsgType();
} // }}}
void CreateStateArgs(const parsed_tree *tree, vector<TypeArg> &dest) // {{{
{
  if (tree->type_name == "Targs" && tree->case_name == "case1") // < Targs2 > {{{
    return CreateStateArgs(tree->content[1], dest); // }}}
  if (tree->type_name == "Targs" && tree->case_name == "case2") // epsilon {{{
    return; // }}}
  if (tree->type_name == "Targs2" && tree->case_name == "case1") // Targs3 {{{
    return CreateStateArgs(tree->content[0],dest); // }}}
  if (tree->type_name == "Targs2" && tree->case_name == "case2") // epsilon {{{
    return; // }}}
  if (tree->type_name == "Targs3" && tree->case_name == "case1") // Targ {{{
    return CreateStateArgs(tree->content[0],dest); // }}}
  if (tree->type_name == "Targs3" && tree->case_name == "case2") // Targ , Targs3 {{{
  { CreateStateArgs(tree->content[0],dest);
    return CreateStateArgs(tree->content[2],dest);
  } // }}}
  if (tree->type_name == "Targ" && tree->case_name == "case1") // id : Bool = exp {{{
  { string argname=tree->content[0]->root.content;
    MpsMsgType *argtype=new MpsBoolMsgType();
    MpsExp *argvalue= MpsExp::Create(tree->content[4]);
    dest.push_back(TypeArg(argname,*argtype,*argvalue));
    // clean up
    delete argtype;
    delete argvalue;
    return;
  } // }}}

#if APIMS_DEBUG_LEVEL>1
  cerr << "Unknown State Argument: " << tree->type_name << "." << tree->case_name << endl;
#endif
  return;
} // }}}
void CreateStateValues(const parsed_tree *tree, vector<MpsExp*> &dest) // {{{
{
  if (tree->type_name == "Tvals" && tree->case_name == "case1") // < exps > {{{
    return FindExps(tree->content[1], dest); // }}}
  if (tree->type_name == "Tvals" && tree->case_name == "case2") // epsilon {{{
    return; // }}}

#if APIMS_DEBUG_LEVEL>1
  cerr << "Unknown State Value: " << tree->type_name << "." << tree->case_name << endl;
#endif
  return;
} // }}}
MpsExp *CreateNamedAssertion(const parsed_tree *tree) // {{{
{ 
  if (tree->type_name == "Assertion" && tree->case_name == "case1") // [[ exp ]] {{{
  {
    return MpsExp::Create(tree->content[1]);
  } // }}}
  else if (tree->type_name == "Assertion" && tree->case_name == "case2") // {{{
  {
    return new MpsBoolVal(true);
  } // }}}
  // else
#if APIMS_DEBUG_LEVEL>1
  cerr << "Unknown Assertion parsetree: " << tree->type_name << "." << tree->case_name << endl;
#endif
  return new MpsBoolVal(false);
} // }}}
MpsExp *CreateNamedAssertion(const parsed_tree *tree, bool &used, std::string &name) // {{{
{
  if (tree->type_name == "NamedAssertion" && tree->case_name == "case1") // as id Assertion {{{
  {
    used=true;
    name=tree->content[1]->root.content;
    return CreateNamedAssertion(tree->content[2]);
  } // }}}
  else if (tree->type_name == "NamedAssertion" && tree->case_name == "case2") // {{{
  {
    used=false;
    return new MpsBoolVal(true);
  } // }}}
#if APIMS_DEBUG_LEVEL>1
  cerr << "Unknown NamedAssertion parsetree: " << tree->type_name << "." << tree->case_name << endl;
#endif
  return new MpsBoolVal(false);
} // }}}
MpsGlobalType *MpsGlobalType::Create(const parsed_tree *tree) // {{{
{
  if (tree->type_name == "Gtype" && tree->case_name == "case1") // int => int < Mtype > NamedAssertion ; Gtype {{{
  {
    MpsGlobalType *succ = MpsGlobalType::Create(tree->content[8]);
    MpsMsgType *msgtype = MpsMsgType::Create(tree->content[4]);
    int from = string2int(tree->content[0]->root.content);
    int to = string2int(tree->content[2]->root.content);
    bool a_used=false;
    string a_name;
    MpsExp *assertion = CreateNamedAssertion(tree->content[6],a_used, a_name);
    MpsGlobalType *result = NULL;
    if (a_used)
      result = new MpsGlobalMsgType(from,to,*msgtype,*succ,*assertion,a_name);
    else
      result = new MpsGlobalMsgType(from,to,*msgtype,*succ);
    delete assertion;
    delete msgtype;
    delete succ;
    return result;
  } // }}}
  else if (tree->type_name == "Gtype" && tree->case_name == "case2") // int => int { Gbranches } {{{
  {
    int from = string2int(tree->content[0]->root.content);
    int to = string2int(tree->content[2]->root.content);
    map<string,MpsGlobalType*> branches;
    branches.clear();
    map<string,MpsExp*> assertions;
    assertions.clear();
    FindGlobalBranches(tree->content[4],branches,assertions);
    MpsGlobalType *result = new MpsGlobalBranchType(from,to,branches,assertions);
    // Clean up
    DeleteMap(branches);
    DeleteMap(assertions);
    return result;
  } // }}}
  else if (tree->type_name == "Gtype" && tree->case_name == "case3") // rec gvar Targs . Gtype {{{
  {
    MpsGlobalType *succ = MpsGlobalType::Create(tree->content[4]);
    string name = tree->content[1]->root.content;
    vector<TypeArg> args;
    args.clear();
    CreateStateArgs(tree->content[2],args);
    MpsGlobalRecType *result = new MpsGlobalRecType(name,*succ, args);
    delete succ;
    return result;
  } // }}}
  else if (tree->type_name == "Gtype" && tree->case_name == "case4") // gvar Tvals {{{
  {
    vector<MpsExp*> args;
    args.clear();
    CreateStateValues(tree->content[1],args);
    MpsGlobalVarType *result = new MpsGlobalVarType(tree->content[0]->root.content, args);
    // Clean Up
    while (args.size()>0)
    { delete args.back();
      args.pop_back();
    }
    return result;
  } // }}}
  else if (tree->type_name == "Gtype" && tree->case_name == "case5") // Gend {{{
  {
    MpsGlobalEndType *result = new MpsGlobalEndType();
    return result;
  } // }}}
  else if (tree->type_name == "Gtype" && tree->case_name == "case6") // { Gbranches } {{{
  {
    map<string,MpsGlobalType*> branches;
    branches.clear();
    map<string,MpsExp*> assertions;
    assertions.clear();
    FindGlobalBranches(tree->content[1],branches,assertions);
    MpsGlobalSyncType *result = new MpsGlobalSyncType(branches,assertions);
    // Clean up
    DeleteMap(branches);
    DeleteMap(assertions);
    return result;
  } // }}}
  
#if APIMS_DEBUG_LEVEL>1
  cerr << "Unknown global-type parsetree: " << tree->type_name << "." << tree->case_name << endl;
#endif
  return new MpsGlobalEndType();
} // }}}
MpsLocalType *MpsLocalType::Create(const parsed_tree *tree) // {{{
{
  if (tree->type_name == "Ltype" && tree->case_name == "case1") //  int << < Mtype > NamedAssertion ; Ltype {{{
  {
    MpsLocalType *succ = MpsLocalType::Create(tree->content[7]);
    MpsMsgType *msgtype = MpsMsgType::Create(tree->content[3]);
    int channel = string2int(tree->content[0]->root.content);
    bool a_used=false;
    string a_name;
    MpsExp *assertion = CreateNamedAssertion(tree->content[5],a_used, a_name);
    MpsLocalSendType *result=NULL;
    if (a_used)
      result = new MpsLocalSendType(channel,*msgtype,*succ,*assertion,a_name);
    else
      result = new MpsLocalSendType(channel,*msgtype,*succ);
    delete assertion;
    delete msgtype;
    delete succ;
    return result;
  } // }}}
  else if (tree->type_name == "Ltype" && tree->case_name == "case2") //  int >> < Mtype > NamedAssertion ; Ltype {{{
  {
    MpsLocalType *succ = MpsLocalType::Create(tree->content[7]);
    MpsMsgType *msgtype = MpsMsgType::Create(tree->content[3]);
    int channel = string2int(tree->content[0]->root.content);
    bool a_used=false;
    string a_name;
    MpsExp *assertion = CreateNamedAssertion(tree->content[5],a_used, a_name);
    MpsLocalRcvType *result=NULL;
    if (a_used)
      result = new MpsLocalRcvType(channel,*msgtype,*succ,*assertion,a_name);
    else
      result = new MpsLocalRcvType(channel,*msgtype,*succ);
    delete assertion;
    delete msgtype;
    delete succ;
    return result;
  } // }}}
  else if (tree->type_name == "Ltype" && tree->case_name == "case3") //  forall id [[ exp ]] ; Ltype {{{
  {
    string name=tree->content[1]->root.content;
    MpsExp *assertion=MpsExp::Create(tree->content[3]);
    MpsLocalType *succ = MpsLocalType::Create(tree->content[6]);
    MpsLocalForallType *result=new MpsLocalForallType(name,*assertion,*succ);
    delete succ;
    delete assertion;
    return result;
  } // }}}
  else if (tree->type_name == "Ltype" && tree->case_name == "case4") //  int << { Lbranches } {{{
  {
    int channel = string2int(tree->content[0]->root.content);
    map<string,MpsLocalType*> branches;
    branches.clear();
    map<string,MpsExp*> assertions;
    assertions.clear();
    FindLocalBranches(tree->content[3],branches,assertions);
    MpsLocalSelectType *result = new MpsLocalSelectType(channel,branches,assertions);
    // Clean up
    DeleteMap(branches);
    DeleteMap(assertions);

    return result;
  } // }}}
  else if (tree->type_name == "Ltype" && tree->case_name == "case5") //  int >> { Lbranches } {{{
  {
    int channel = string2int(tree->content[0]->root.content);
    map<string,MpsLocalType*> branches;
    branches.clear();
    map<string,MpsExp*> assertions;
    assertions.clear();
    FindLocalBranches(tree->content[3],branches,assertions);
    MpsLocalBranchType *result = new MpsLocalBranchType(channel,branches,assertions);
    // Clean up
    DeleteMap(branches);
    DeleteMap(assertions);

    return result;
  } // }}}
  else if (tree->type_name == "Ltype" && tree->case_name == "case6") // rec lvar Targs . Ltype {{{
  {
    MpsLocalType *succ = MpsLocalType::Create(tree->content[4]);
    string name = tree->content[1]->root.content;
    vector<TypeArg> args;
    args.clear();
    CreateStateArgs(tree->content[2],args);
    MpsLocalRecType *result = new MpsLocalRecType(name,*succ,args);
    delete succ;
    return result;
  } // }}}
  else if (tree->type_name == "Ltype" && tree->case_name == "case7") // lvar Tvals {{{
  {
    vector<MpsExp*> args;
    args.clear();
    CreateStateValues(tree->content[1],args);
    MpsLocalVarType *result = new MpsLocalVarType(tree->content[0]->root.content, args);
    // Clean Up
    while (args.size()>0)
    { delete args.back();
      args.pop_back();
    }
    return result;
  } // }}}
  else if (tree->type_name == "Ltype" && tree->case_name == "case8") // Lend {{{
  {
    MpsLocalEndType *result = new MpsLocalEndType();
    return result;
  } // }}}
  else if (tree->type_name == "Ltype" && tree->case_name == "case9") // { Lbranches } {{{
  {
    map<string,MpsLocalType*> branches;
    branches.clear();
    map<string,MpsExp*> assertions;
    assertions.clear();
    FindLocalBranches(tree->content[1],branches,assertions);
    MpsLocalSyncType *result = new MpsLocalSyncType(branches,assertions);
    // Clean up
    DeleteMap(branches);
    DeleteMap(assertions);

    return result;
  } // }}}
  
#if APIMS_DEBUG_LEVEL>1
  cerr << "Unknown local-type constructor: " << tree->type_name << "." << tree->case_name << endl;
#endif
  return new MpsLocalEndType();
} // }}}

/* MpsGlobalType implementation
 *
 */

// Constructors
MpsGlobalMsgType::MpsGlobalMsgType(int sender, int receiver, const MpsMsgType &msg, const MpsGlobalType &succ) // {{{
{
  mySender = sender;
  myReceiver = receiver;
  myMsgType = msg.Copy();
  mySucc = succ.Copy();
  myAssertionType=false;
  myAssertion=new MpsBoolVal(true);
} // }}}
MpsGlobalMsgType::MpsGlobalMsgType(int sender, int receiver, const MpsMsgType &msg, const MpsGlobalType &succ, const MpsExp &assertion, const string &id) // {{{
{
  mySender = sender;
  myReceiver = receiver;
  myMsgType = msg.Copy();
  mySucc = succ.Copy();
  myAssertionType=true;
  myAssertion=assertion.Copy();
  myId=id;
} // }}}
MpsGlobalBranchType::MpsGlobalBranchType(int sender, int receiver, const map<string,MpsGlobalType*> &branches, const map<string,MpsExp*> &assertions) // {{{
{
  mySender = sender;
  myReceiver = receiver;
  myBranches.clear();
  for (map<string,MpsGlobalType*>::const_iterator it=branches.begin(); it!=branches.end(); ++it)
    myBranches[it->first] = it->second->Copy();
  myAssertions.clear();
  for (map<string,MpsExp*>::const_iterator it=assertions.begin(); it!=assertions.end(); ++it)
    myAssertions[it->first] = it->second->Copy();
} // }}}
MpsGlobalRecType::MpsGlobalRecType(const std::string &name, const MpsGlobalType &succ, const vector<TypeArg> &args) // {{{
{
  myName = name;
  mySucc = succ.Copy();
  myArgs = args;
} // }}}
MpsGlobalVarType::MpsGlobalVarType(const string &name, const vector<MpsExp*> &args) // {{{
{
  myName = name;
  myValues.clear();
  for (vector<MpsExp*>::const_iterator it=args.begin(); it!=args.end(); ++it)
    myValues.push_back((*it)->Copy());
} // }}}
MpsGlobalEndType::MpsGlobalEndType() // {{{
{
} // }}}
MpsGlobalSyncType::MpsGlobalSyncType(const std::map<std::string,MpsGlobalType*> &branches, const std::map<std::string,MpsExp*> &assertions) // {{{
{
  myBranches.clear();
  for (map<string,MpsGlobalType*>::const_iterator it=branches.begin(); it!=branches.end(); ++it)
    myBranches[it->first] = it->second->Copy();
  myAssertions.clear();
  for (map<string,MpsExp*>::const_iterator it=assertions.begin(); it!=assertions.end(); ++it)
    myAssertions[it->first] = it->second->Copy();
} // }}}

// Destructors
MpsGlobalType::~MpsGlobalType() // {{{
{
} // }}}
MpsGlobalMsgType::~MpsGlobalMsgType() // {{{
{
  delete myMsgType;
  delete mySucc;
  delete myAssertion;
} // }}}
MpsGlobalBranchType::~MpsGlobalBranchType() // {{{
{
  DeleteMap(myBranches);
  DeleteMap(myAssertions);
} // }}}
MpsGlobalRecType::~MpsGlobalRecType() // {{{
{
  delete mySucc;
} // }}}
MpsGlobalVarType::~MpsGlobalVarType() // {{{
{
  DeleteVector(myValues);
} // }}}
MpsGlobalEndType::~MpsGlobalEndType() // {{{
{
} // }}}
MpsGlobalSyncType::~MpsGlobalSyncType() // {{{
{
  DeleteMap(myBranches);
  DeleteMap(myAssertions);
} // }}}

// Make Deep Copy
MpsGlobalMsgType *MpsGlobalMsgType::Copy() const// {{{
{
  if (myAssertionType)
    return new MpsGlobalMsgType(mySender, myReceiver, *myMsgType, *mySucc, *myAssertion, myId);
  else
    return new MpsGlobalMsgType(mySender, myReceiver, *myMsgType, *mySucc);
} // }}}
MpsGlobalBranchType *MpsGlobalBranchType::Copy() const // {{{
{
  return new MpsGlobalBranchType(mySender, myReceiver, myBranches, myAssertions);
} // }}}
MpsGlobalRecType *MpsGlobalRecType::Copy() const // {{{
{
  return new MpsGlobalRecType(myName, *mySucc, myArgs);
} // }}}
MpsGlobalVarType *MpsGlobalVarType::Copy() const // {{{
{
  return new MpsGlobalVarType(myName, myValues);
} // }}}
MpsGlobalEndType *MpsGlobalEndType::Copy() const // {{{
{
  return new MpsGlobalEndType();
} // }}}
MpsGlobalSyncType *MpsGlobalSyncType::Copy() const // {{{
{
  return new MpsGlobalSyncType(myBranches, myAssertions);
} // }}}

// TODO: Improve equality to co-inductive type equality and subtyping
// Compare
bool ERROR_GLOBALEQ(const MpsExp &Theta, const MpsGlobalType &lhs, const MpsGlobalType &rhs, string msg) // {{{
{
#if APIMS_DEBUG_LEVEL>20
  cerr << "!!!!Types are not equal:" << endl
       << "!!!!Theta: " << Theta.ToString() << endl
       << "!!!!!!LHS: " << lhs.ToString("!!!!!!!!!! ") << endl
       << "!!!!!!RHS: " << rhs.ToString("!!!!!!!!!! ") << endl
       << "!!!!!!MSG: " << msg << endl;
#endif
  return false;
} // }}}
bool CompareAssertions(const MpsExp &Theta, const MpsExp &lhs, const MpsExp &rhs) // {{{
{ MpsExp *notLhs = new MpsUnOpExp("not",lhs);
  MpsExp *notRhs = new MpsUnOpExp("not",rhs);
  MpsExp *check1 = new MpsBinOpExp("or",*notLhs,rhs,MpsBoolMsgType(),MpsBoolMsgType());
  MpsExp *check2 = new MpsBinOpExp("or",*notRhs,lhs,MpsBoolMsgType(),MpsBoolMsgType());
  MpsExp *check = new MpsBinOpExp("and",*check1,*check2,MpsBoolMsgType(),MpsBoolMsgType());
  delete notLhs;
  delete notRhs;
  delete check1;
  delete check2;
  vector<const MpsExp*> hyps;
  hyps.push_back(&Theta);
  bool result = check->ValidExp(hyps);
  hyps.pop_back();
  delete check;
  return result;
} // }}}
bool MpsGlobalMsgType::Equal(const MpsExp &Theta, const MpsGlobalType &rhs) const // {{{
{
  const MpsGlobalMsgType *rhsptr=dynamic_cast<const MpsGlobalMsgType*>(&rhs);
  if (rhsptr==NULL)
    return ERROR_GLOBALEQ(Theta,*this,rhs,"RHS not message-type");
  if (mySender != rhsptr->mySender ||
      myReceiver != rhsptr->myReceiver ||
      not myMsgType->Equal(Theta,*rhsptr->myMsgType))
    return ERROR_GLOBALEQ(Theta,*this,rhs,"Head mismatch");

  if (myAssertionType != rhsptr->myAssertionType)
    return ERROR_GLOBALEQ(Theta,*this,rhs,"Assertion-type mismatch");

  // Rename to common name in Assertions
  string newId=MpsExp::NewVar(myId);
  MpsExp *lhsAssertion=myAssertion->Rename(myId,newId);
  MpsExp *rhsAssertion=rhsptr->myAssertion->Rename(myId,newId);
  // Compare Assertions
  if (not CompareAssertions(Theta,*lhsAssertion,*rhsAssertion))
  { delete lhsAssertion;
    delete rhsAssertion;
    return ERROR_GLOBALEQ(Theta,*this,rhs,"Assertion mismatch");
  }
  delete rhsAssertion;
  MpsExp *newTheta=new MpsBinOpExp("and",Theta,*lhsAssertion,MpsBoolMsgType(),MpsBoolMsgType());
  delete lhsAssertion;
  // Rename to common name in Succ
  MpsGlobalType *lhsSucc=mySucc->ERename(myId,newId);
  MpsGlobalType *rhsSucc=rhsptr->mySucc->ERename(rhsptr->myId,newId);
  bool checkSucc=lhsSucc->Equal(*newTheta,*rhsSucc);
  delete lhsSucc;
  delete rhsSucc;
  delete newTheta;

  return checkSucc;
} // }}}
bool MpsGlobalBranchType::Equal(const MpsExp &Theta, const MpsGlobalType &rhs) const // {{{
{
  // Check top-level type and channel
  const MpsGlobalBranchType *rhsptr=dynamic_cast<const MpsGlobalBranchType*>(&rhs);
  if (rhsptr==NULL)
    return ERROR_GLOBALEQ(Theta,*this,rhs,"RHS not branch-type");
  if (mySender != rhsptr->mySender ||
      myReceiver != rhsptr->myReceiver)
    return ERROR_GLOBALEQ(Theta,*this,rhs,"Head mismatch");

  // Check label in lhs means label in rhs
  // and their assertions and types correspond
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
  {
    map<string,MpsGlobalType*>::const_iterator it2=rhsptr->myBranches.find(it->first);
    if (it2 == rhsptr->myBranches.end())
      return ERROR_GLOBALEQ(Theta,*this,rhs,(string)"No RHS branch for label: " + it->first);
    // Find Assertions
    map<string,MpsExp*>::const_iterator lhsAssertion=myAssertions.find(it->first);
    map<string,MpsExp*>::const_iterator rhsAssertion=rhsptr->myAssertions.find(it->first);
    // Compare Assertions
    bool lhsHasA=lhsAssertion!=myAssertions.end();
    bool rhsHasA=rhsAssertion!=rhsptr->myAssertions.end();
    if (lhsHasA!=rhsHasA)
      return ERROR_GLOBALEQ(Theta,*this,rhs,(string)"Assertion-type for label: " + it->first);
    MpsExp *newTheta=NULL;
    if (lhsHasA)
    { if (not CompareAssertions(Theta,*lhsAssertion->second,*rhsAssertion->second))
        return ERROR_GLOBALEQ(Theta,*this,rhs,(string)"Assertion not equivalent for label: " + it->first);
      newTheta=new MpsBinOpExp("and",Theta,*lhsAssertion->second,MpsBoolMsgType(),MpsBoolMsgType());
    }
    else
      newTheta=Theta.Copy();
    // Compare Branch Processes
    bool checkBranch = it->second->Equal(*newTheta,*it2->second);
    delete newTheta;
    if (not checkBranch)
      return false;
  }
  // Check label in rhs means label in lhs
  for (map<string,MpsGlobalType*>::const_iterator it2=rhsptr->myBranches.begin();it2!=rhsptr->myBranches.end();++it2)
  {
    map<string,MpsGlobalType*>::const_iterator it=myBranches.find(it->first);
    if (it == myBranches.end())
      return ERROR_GLOBALEQ(Theta,*this,rhs,(string)"No RHS branch for label: " + it->first);
  }

  // All checks passed
  return true;
} // }}}
bool MpsGlobalRecType::Equal(const MpsExp &Theta, const MpsGlobalType &rhs) const // {{{
{
  const MpsGlobalRecType *rhsptr=dynamic_cast<const MpsGlobalRecType*>(&rhs);
  if (rhsptr==NULL) // RHS is not recursive type
    return ERROR_GLOBALEQ(Theta,*this,rhs,"RHS not rec-type");
  if (myArgs.size()!=rhsptr->myArgs.size()) // Different number of arguments
    return ERROR_GLOBALEQ(Theta,*this,rhs,"Different number of arguments");

  for (int i=0; i<myArgs.size(); ++i)
  { if (not myArgs[i].myType->Equal(Theta,*rhsptr->myArgs[i].myType))
      return ERROR_GLOBALEQ(Theta,*this,rhs,(string)"Not same type for argument: " + int2string(i));
    if (not CompareAssertions(Theta,*myArgs[i].myValue,*rhsptr->myArgs[i].myValue))
      return ERROR_GLOBALEQ(Theta,*this,rhs,"Not equivalent value for argument: " + int2string(i));
  }
  MpsGlobalType *newlhs = mySucc->Copy();
  MpsGlobalType *newrhs = rhsptr->mySucc->Copy();
   
  for (int i=0; i<myArgs.size(); ++i)
  { if (not (myArgs[i].myName == rhsptr->myArgs[i].myName)) // Rename to common name
    { string newName=MpsExp::NewVar(myArgs[i].myName);
      MpsGlobalType *newType = newlhs->ERename(myArgs[i].myName,newName);
      delete newlhs;
      newlhs=newType;
      newType = newrhs->ERename(rhsptr->myArgs[i].myName,newName);
      delete newrhs;
      newrhs=newType;
    }
  }
  if (myName != rhsptr->myName) // Rename to common name
  {
    string newName = NewGVar(myName);
    MpsGlobalType *newType = newlhs->LRename(myName,newName);
    delete newlhs;
    newlhs=newType;
    newType = newrhs->LRename(rhsptr->myName,newName);
    delete newrhs;
    newrhs=newType;
  }
  // Now compare resulting successors
  bool result = newlhs->Equal(Theta,*newrhs);

  // Clean Up
  delete newlhs;
  delete newrhs;
} // }}}
bool MpsGlobalVarType::Equal(const MpsExp &Theta, const MpsGlobalType &rhs) const // {{{
{
  const MpsGlobalVarType *rhsptr=dynamic_cast<const MpsGlobalVarType*>(&rhs);
  if (rhsptr==NULL)
    return ERROR_GLOBALEQ(Theta,*this,rhs,"RHS not VAR-type");
  // Check name
  if (myName != rhsptr->myName)
    return ERROR_GLOBALEQ(Theta,*this,rhs,"Differend variable name");
  // Check arguments
  if (myValues.size()!=rhsptr->myValues.size())
    return ERROR_GLOBALEQ(Theta,*this,rhs,"Different number of arguments");
  for (int i=0; i<myValues.size(); ++i)
    if (not CompareAssertions(Theta,*myValues[i],*rhsptr->myValues[i]))
      return ERROR_GLOBALEQ(Theta,*this,rhs,(string)"Not equivalent values for argument: " + int2string(i));
  // All checks passed
  return true;
} // }}}
bool MpsGlobalEndType::Equal(const MpsExp &Theta, const MpsGlobalType &rhs) const // {{{
{
  return typeid(rhs)==typeid(MpsGlobalEndType);
} // }}}
bool MpsGlobalSyncType::Equal(const MpsExp &Theta, const MpsGlobalType &rhs) const // {{{
{
  // Check top-level type and channel
  const MpsGlobalSyncType *rhsptr=dynamic_cast<const MpsGlobalSyncType*>(&rhs);
  if (rhsptr==NULL)
    return ERROR_GLOBALEQ(Theta,*this,rhs,"RHS not sync-type");

  // Check label in lhs means label in rhs
  // and their assertions and types correspond
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
  {
    map<string,MpsGlobalType*>::const_iterator it2=rhsptr->myBranches.find(it->first);
    if (it2 == rhsptr->myBranches.end())
      return ERROR_GLOBALEQ(Theta,*this,rhs,(string)"No RHS branch for label: " + it->first);
    // Find Assertions
    map<string,MpsExp*>::const_iterator lhsAssertion=myAssertions.find(it->first);
    map<string,MpsExp*>::const_iterator rhsAssertion=rhsptr->myAssertions.find(it->first);
    // Compare Assertions
    bool lhsHasA=lhsAssertion!=myAssertions.end();
    bool rhsHasA=rhsAssertion!=rhsptr->myAssertions.end();
    if (lhsHasA!=rhsHasA)
      return ERROR_GLOBALEQ(Theta,*this,rhs,(string)"Assertion-type for label: " + it->first);
    MpsExp *newTheta=NULL;
    if (lhsHasA)
    { if (not CompareAssertions(Theta,*lhsAssertion->second,*rhsAssertion->second))
        return ERROR_GLOBALEQ(Theta,*this,rhs,(string)"Assertion not equivalent for label: " + it->first);
      newTheta=new MpsBinOpExp("and",Theta,*lhsAssertion->second,MpsBoolMsgType(),MpsBoolMsgType());
    }
    else
      newTheta=Theta.Copy();
    // Compare Branch Processes
    bool checkBranch = it->second->Equal(*newTheta,*it2->second);
    delete newTheta;
    if (not checkBranch)
      return false;
  }
  // Check label in rhs means label in lhs
  for (map<string,MpsGlobalType*>::const_iterator it2=rhsptr->myBranches.begin();it2!=rhsptr->myBranches.end();++it2)
  {
    map<string,MpsGlobalType*>::const_iterator it=myBranches.find(it->first);
    if (it == myBranches.end())
      return ERROR_GLOBALEQ(Theta,*this,rhs,(string)"No RHS branch for label: " + it->first);
  }

  // All checks passed
  return true;
} // }}}

// Free Global Type Variables
set<string> MpsGlobalMsgType::FGV() const // {{{
{
  set<string> result=mySucc->FGV();
  set<string> msgFGV=myMsgType->FGV();
  result.insert(msgFGV.begin(),msgFGV.end());
  return result;
} // }}}
set<string> MpsGlobalBranchType::FGV() const // {{{
{
  set<string> result;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    set<string> fgv = it->second->FGV();
    result.insert(fgv.begin(),fgv.end());
  }
  return result;
} // }}}
set<string> MpsGlobalRecType::FGV() const // {{{
{
  set<string> result = mySucc->FGV();
  result.erase(myName);
  for (vector<TypeArg>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  { set<string> tmp=it->myType->FGV();
    result.insert(tmp.begin(),tmp.end());
  }
  return result;
} // }}}
set<string> MpsGlobalVarType::FGV() const // {{{
{
  set<string> result;
  result.insert(myName);
  return result;
} // }}}
set<string> MpsGlobalEndType::FGV() const // {{{
{
  return set<string>();
} // }}}
set<string> MpsGlobalSyncType::FGV() const // {{{
{
  set<string> result;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    set<string> fgv = it->second->FGV();
    result.insert(fgv.begin(),fgv.end());
  }
  return result;
} // }}}

// Free Local Type Variables
set<string> MpsGlobalMsgType::FLV() const // {{{
{
  set<string> result=mySucc->FLV();
  set<string> msgFLV=myMsgType->FLV();
  result.insert(msgFLV.begin(),msgFLV.end());
  return result;
} // }}}
set<string> MpsGlobalBranchType::FLV() const // {{{
{
  set<string> result;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    set<string> flv = it->second->FLV();
    result.insert(flv.begin(),flv.end());
  }
  return result;
} // }}}
set<string> MpsGlobalRecType::FLV() const // {{{
{
  set<string> result = mySucc->FLV();
  for (vector<TypeArg>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  { set<string> tmp=it->myType->FLV();
    result.insert(tmp.begin(),tmp.end());
  }
  return result;
} // }}}
set<string> MpsGlobalVarType::FLV() const // {{{
{
  return set<string>();
} // }}}
set<string> MpsGlobalEndType::FLV() const // {{{
{
  return set<string>();
} // }}}
set<string> MpsGlobalSyncType::FLV() const // {{{
{
  set<string> result;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    set<string> flv = it->second->FLV();
    result.insert(flv.begin(),flv.end());
  }
  return result;
} // }}}

// Free Expression Variables
set<string> MpsGlobalMsgType::FEV() const // {{{
{
  set<string> result=mySucc->FEV();
  set<string> tmp=myMsgType->FEV();
  result.insert(tmp.begin(),tmp.end());
  if (myAssertionType)
  { tmp=myAssertion->FV();
    result.insert(tmp.begin(),tmp.end());
  }
  return result;
} // }}}
set<string> MpsGlobalBranchType::FEV() const // {{{
{
  set<string> result;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    set<string> tmp = it->second->FEV();
    result.insert(tmp.begin(),tmp.end());
  }
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin(); it!=myAssertions.end(); ++it)
  {
    set<string> tmp = it->second->FV();
    result.insert(tmp.begin(),tmp.end());
  }
  return result;
} // }}}
set<string> MpsGlobalRecType::FEV() const // {{{
{
  set<string> succFEV = mySucc->FEV();
  set<string> result;
  for (vector<TypeArg>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  { set<string> tmp=it->myType->FEV();
    result.insert(tmp.begin(),tmp.end());
    tmp=it->myValue->FV();
    result.insert(tmp.begin(),tmp.end());
    succFEV.erase(it->myName);
  }
  result.insert(succFEV.begin(),succFEV.end());

  return result;
} // }}}
set<string> MpsGlobalVarType::FEV() const // {{{
{ set<string> result;
  for (vector<MpsExp*>::const_iterator it=myValues.begin(); it!=myValues.end(); ++it)
  { set<string> tmp=(*it)->FV();
    result.insert(tmp.begin(),tmp.end());
  }
  return result;
} // }}}
set<string> MpsGlobalEndType::FEV() const // {{{
{
  return set<string>();
} // }}}
set<string> MpsGlobalSyncType::FEV() const // {{{
{
  set<string> result;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    set<string> flv = it->second->FEV();
    result.insert(flv.begin(),flv.end());
  }
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin(); it!=myAssertions.end(); ++it)
  {
    set<string> tmp = it->second->FV();
    result.insert(tmp.begin(),tmp.end());
  }
  return result;
} // }}}

// Rename Global Type Variable
MpsGlobalMsgType *MpsGlobalMsgType::GRename(const string &from, const string &to) const // {{{
{
  MpsGlobalType *newSucc = mySucc->GRename(from,to);
  MpsMsgType *newMsgType = myMsgType->GRename(from,to);
  MpsGlobalMsgType *result = NULL;
  if (myAssertionType)
    result = new MpsGlobalMsgType(mySender, myReceiver, *newMsgType, *newSucc, *myAssertion, myId);
  else
    result = new MpsGlobalMsgType(mySender, myReceiver, *newMsgType, *newSucc);
  delete newSucc;
  delete newMsgType;
  return result;
} // }}}
MpsGlobalBranchType *MpsGlobalBranchType::GRename(const string &from, const string &to) const // {{{
{
  map<string,MpsGlobalType*> newBranches;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->GRename(from,to);
  MpsGlobalBranchType *result = new MpsGlobalBranchType(mySender, myReceiver, newBranches, myAssertions);
  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsGlobalRecType *MpsGlobalRecType::GRename(const string &from, const string &to) const // {{{
{
  // Substitute in Args
  vector<TypeArg> newArgs;
  for (vector<TypeArg>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  { MpsMsgType *newType = it->myType->GRename(from,to);
    newArgs.push_back(TypeArg(it->myName,*newType,*it->myValue));
    delete newType;
  }

  if (from == myName) // If name is hidden, make no further substitutions
    return new MpsGlobalRecType(myName, *mySucc, newArgs);

  MpsGlobalRecType *result=NULL;
  if (myName == to) // Rename to avoid variable capturing
  {
    string newName=NewGVar();
    MpsGlobalType *tmpSucc=mySucc->GRename(myName,newName);
    MpsGlobalType *newSucc=tmpSucc->GRename(from,to);
    delete tmpSucc;
    result = new MpsGlobalRecType(newName,*newSucc, newArgs);
    delete newSucc;
  }
  else // No rename is necessary
  {
    MpsGlobalType *newSucc=mySucc->GRename(from,to);
    result= new MpsGlobalRecType(myName,*newSucc,newArgs);
    delete newSucc;
  }
  return result;
} // }}}
MpsGlobalVarType *MpsGlobalVarType::GRename(const string &from, const string &to) const // {{{
{
  // assert to != myName
  if (from == myName)
    return new MpsGlobalVarType(to,myValues);
  else
    return Copy();
} // }}}
MpsGlobalEndType *MpsGlobalEndType::GRename(const string &from, const string &to) const // {{{
{
  return Copy();
} // }}}
MpsGlobalSyncType *MpsGlobalSyncType::GRename(const string &from, const string &to) const // {{{
{
  map<string,MpsGlobalType*> newBranches;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->GRename(from,to);
  // Substitute in assertions?
  MpsGlobalSyncType *result = new MpsGlobalSyncType(newBranches,myAssertions);
  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}

// Rename Local Type Variable
MpsGlobalMsgType *MpsGlobalMsgType::LRename(const string &from, const string &to) const // {{{
{
  MpsGlobalType *newSucc = mySucc->LRename(from,to);
  MpsMsgType *newMsgType = myMsgType->LRename(from,to);
  MpsGlobalMsgType *result = NULL;
  if (myAssertionType)
    result = new MpsGlobalMsgType(mySender, myReceiver, *newMsgType, *newSucc, *myAssertion, myId);
  else
    result = new MpsGlobalMsgType(mySender, myReceiver, *newMsgType, *newSucc);
  // Clean Up
  delete newSucc;
  delete newMsgType;

  return result;
} // }}}
MpsGlobalBranchType *MpsGlobalBranchType::LRename(const string &from, const string &to) const // {{{
{
  map<string,MpsGlobalType*> newBranches;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->LRename(from,to);
  MpsGlobalBranchType *result = new MpsGlobalBranchType(mySender, myReceiver, newBranches, myAssertions);
  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsGlobalRecType *MpsGlobalRecType::LRename(const string &from, const string &to) const // {{{
{
  // Substitute in Args
  vector<TypeArg> newArgs;
  for (vector<TypeArg>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  { MpsMsgType *newType = it->myType->LRename(from,to);
    newArgs.push_back(TypeArg(it->myName,*newType,*it->myValue));
    delete newType;
  }

  MpsGlobalType *newSucc=mySucc->LRename(from,to);
  MpsGlobalRecType *  result= new MpsGlobalRecType(myName,*newSucc,newArgs);

  // Clean Up
  delete newSucc;

  return result;
} // }}}
MpsGlobalVarType *MpsGlobalVarType::LRename(const string &from, const string &to) const // {{{
{
  return Copy();
} // }}}
MpsGlobalEndType *MpsGlobalEndType::LRename(const string &from, const string &to) const // {{{
{
  return Copy();
} // }}}
MpsGlobalSyncType *MpsGlobalSyncType::LRename(const string &from, const string &to) const // {{{
{
  map<string,MpsGlobalType*> newBranches;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->LRename(from,to);
  // Substitute in assertions?
  MpsGlobalSyncType *result = new MpsGlobalSyncType(newBranches,myAssertions);
  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}

// Rename Assertion Variable
MpsGlobalMsgType *MpsGlobalMsgType::ERename(const string &from, const string &to) const // {{{
{ MpsMsgType *newMsgType = myMsgType->ERename(from,to);
  if (myAssertionType && myId==from) // Make no further renamings
  { MpsGlobalMsgType *result=NULL;
    if (myAssertionType)
      result=new MpsGlobalMsgType(mySender, myReceiver, *newMsgType, *mySucc, *myAssertion, myId);
    else
      result=new MpsGlobalMsgType(mySender, myReceiver, *newMsgType, *mySucc);
    delete newMsgType;
    return result;
  }

  string newId;
  MpsExp *tmpAssertion=NULL;
  MpsGlobalType *tmpSucc=NULL;
  if (myAssertionType && myId==to)
  { newId=MpsExp::NewVar(myId);
    tmpAssertion=myAssertion->Rename(myId,newId);
    tmpSucc=mySucc->ERename(myId,newId);
  }
  else if (myAssertionType)
  { newId=myId;
    tmpAssertion=myAssertion->Copy();
    tmpSucc=mySucc->Copy();
  }
  else
  { newId=myId;
    tmpAssertion=new MpsBoolVal(true);
    tmpSucc=mySucc->Copy();
  }

  MpsExp *newAssertion = tmpAssertion->Rename(from,to);
  delete tmpAssertion;
  MpsGlobalType *newSucc=tmpSucc->ERename(from,to);
  delete tmpSucc;
  MpsGlobalMsgType *result=NULL;
  if (myAssertionType)
    result=new MpsGlobalMsgType(mySender, myReceiver, *newMsgType, *newSucc, *newAssertion, newId);
  else
    result=new MpsGlobalMsgType(mySender, myReceiver, *newMsgType, *newSucc);

  delete newAssertion;
  delete newSucc;
  delete newMsgType;
  return result;
} // }}}
MpsGlobalBranchType *MpsGlobalBranchType::ERename(const string &from, const string &to) const // {{{
{
  map<string,MpsGlobalType*> newBranches;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->ERename(from,to);
  map<string,MpsExp*> newAssertions;
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin();it!=myAssertions.end();++it)
    newAssertions[it->first] = it->second->Rename(from,to);
  MpsGlobalBranchType *result = new MpsGlobalBranchType(mySender, myReceiver, newBranches, newAssertions);
  // Clean up
  DeleteMap(newBranches);
  DeleteMap(newAssertions);

  return result;
} // }}}
MpsGlobalRecType *MpsGlobalRecType::ERename(const string &from, const string &to) const // {{{
{
  MpsGlobalType *newSucc = mySucc->ERename(from,to);
  vector<TypeArg> newArgs;
  for (vector<TypeArg>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  { MpsMsgType *newType=it->myType->ERename(from,to);
    MpsExp *newValue=it->myValue->Rename(from,to);
    newArgs.push_back(TypeArg(it->myName,*newType,*newValue));
    delete newType;
    delete newValue;
  }
  MpsGlobalRecType *result=new MpsGlobalRecType(myName,*newSucc,newArgs);
  // Clean Up
  delete newSucc;

  return result;
} // }}}
MpsGlobalVarType *MpsGlobalVarType::ERename(const string &from, const string &to) const // {{{
{
  vector<MpsExp*> newValues;
  for (vector<MpsExp*>::const_iterator it=myValues.begin(); it!=myValues.end(); ++it)
    newValues.push_back((*it)->Rename(from,to));
  MpsGlobalVarType *result=new MpsGlobalVarType(myName,newValues);
  // Clean Up
  DeleteVector(newValues);

  return result;
} // }}}
MpsGlobalEndType *MpsGlobalEndType::ERename(const string &from, const string &to) const // {{{
{
  return Copy();
} // }}}
MpsGlobalSyncType *MpsGlobalSyncType::ERename(const string &from, const string &to) const // {{{
{
  map<string,MpsGlobalType*> newBranches;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->ERename(from,to);
  map<string,MpsExp*> newAssertions;
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin();it!=myAssertions.end();++it)
    newAssertions[it->first] = it->second->Rename(from,to);
  MpsGlobalSyncType *result = new MpsGlobalSyncType(newBranches,newAssertions);
  // Clean up
  DeleteMap(newBranches);
  DeleteMap(newAssertions);

  return result;
} // }}}

// Global Type Substitution
MpsGlobalMsgType *MpsGlobalMsgType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsMsgType *newMsgType = myMsgType->GSubst(source,dest,args);

  // Create newSucc and newAssertion
  MpsGlobalType *newSucc=NULL;
  MpsExp *newAssertion=NULL;
  string newId;
  set<string> fv=dest.FEV();
  if (myAssertionType && fv.find(myId)!=fv.end()) // Rename myId before substituting
  { newId = MpsExp::NewVar(myId);
    MpsGlobalType *tmpSucc=mySucc->ERename(myId,newId);
    newSucc=tmpSucc->GSubst(source,dest,args);
    delete tmpSucc;
    newAssertion = myAssertion->Rename(myId, newId);
  }
  else
  { newId = myId;
    newSucc = mySucc->GSubst(source,dest,args);
    newAssertion = myAssertion->Copy();
  }

  // Create result
  MpsGlobalMsgType *result = NULL;
  if (myAssertionType)
    result = new MpsGlobalMsgType(mySender, myReceiver, *newMsgType, *newSucc, *newAssertion, newId);
  else
    result = new MpsGlobalMsgType(mySender, myReceiver, *newMsgType, *newSucc);

  // Clean Up
  delete newSucc;
  delete newMsgType;
  delete newAssertion;
  return result;
} // }}}
MpsGlobalBranchType *MpsGlobalBranchType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  map<string,MpsGlobalType*> newBranches;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->GSubst(source,dest,args);
  MpsGlobalBranchType *result = new MpsGlobalBranchType(mySender, myReceiver, newBranches, myAssertions);
  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsGlobalRecType *MpsGlobalRecType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  vector<TypeArg> newArgs;
  for (vector<TypeArg>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  { MpsMsgType *newType=it->myType->GSubst(source,dest,args);
    newArgs.push_back(TypeArg(it->myName,*newType, *it->myValue));
    delete newType;
  }
  if (source == myName)
    return new MpsGlobalRecType(myName,*mySucc,newArgs);
  MpsGlobalRecType *result=NULL;
  set<string> fv = dest.FGV();
  if (fv.find(myName) != fv.end()) // Rename to avoid variable capturing
  {
    string newName=NewGVar();
    MpsGlobalType *tmpSucc=mySucc->GRename(myName,newName);
    MpsGlobalType *newSucc=tmpSucc->GSubst(source,dest,args);
    delete tmpSucc;
    result = new MpsGlobalRecType(newName,*newSucc,newArgs);
    delete newSucc;
  }
  else // No rename is necessary
  {
    MpsGlobalType *newSucc=mySucc->GSubst(source,dest,args);
    result= new MpsGlobalRecType(myName,*newSucc,newArgs);
    delete newSucc;
  }
  return result;
} // }}}
MpsGlobalType *MpsGlobalVarType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  // assert to != myName
  if (source != myName)
    return Copy();
  if (myValues.size() != args.size())
  { 
#if APIMS_DEBUG_LEVEL>2
    cerr << "GSubst ERROR: argument size mismatch" << endl;
#endif
    return new MpsGlobalVarType("ERROR",vector<MpsExp*>());
  }
  MpsGlobalType *result=dest.Copy();
  for (int i=0; i<myValues.size(); ++i)
  { MpsGlobalType *tmpType = result->ESubst(args[i],*myValues[i]);
    delete result;
    result=tmpType;
  }
  return result;
} // }}}
MpsGlobalEndType *MpsGlobalEndType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  return Copy();
} // }}}
MpsGlobalSyncType *MpsGlobalSyncType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  map<string,MpsGlobalType*> newBranches;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->GSubst(source,dest,args);
  // Substitute in assertions?
  MpsGlobalSyncType *result = new MpsGlobalSyncType(newBranches,myAssertions);
  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}

// Local Type Substitution
MpsGlobalMsgType *MpsGlobalMsgType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsMsgType *newMsgType = myMsgType->LSubst(source,dest,args);

  // Create newSucc and newAssertion
  MpsGlobalType *newSucc=NULL;
  MpsExp *newAssertion=NULL;
  string newId;
  set<string> fv=dest.FEV();
  if (myAssertionType && fv.find(myId)!=fv.end()) // Rename myId before substituting
  { newId = MpsExp::NewVar(myId);
    MpsGlobalType *tmpSucc=mySucc->ERename(myId,newId);
    newSucc=tmpSucc->LSubst(source,dest,args);
    delete tmpSucc;
    newAssertion = myAssertion->Rename(myId, newId);
  }
  else
  { newId = myId;
    newSucc = mySucc->LSubst(source,dest,args);
    newAssertion = myAssertion->Copy();
  }

  // Create result
  MpsGlobalMsgType *result = NULL;
  if (myAssertionType)
    result = new MpsGlobalMsgType(mySender, myReceiver, *newMsgType, *newSucc, *newAssertion, newId);
  else
    result = new MpsGlobalMsgType(mySender, myReceiver, *newMsgType, *newSucc);

  // Clean Up
  delete newSucc;
  delete newMsgType;
  delete newAssertion;
  return result;
} // }}}
MpsGlobalBranchType *MpsGlobalBranchType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  map<string,MpsGlobalType*> newBranches;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->LSubst(source,dest,args);
  MpsGlobalBranchType *result = new MpsGlobalBranchType(mySender, myReceiver, newBranches, myAssertions);

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsGlobalRecType *MpsGlobalRecType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  vector<TypeArg> newArgs;
  for (vector<TypeArg>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  { MpsMsgType *newType=it->myType->LSubst(source,dest,args);
    newArgs.push_back(TypeArg(it->myName,*newType, *it->myValue));
    delete newType;
  }
  MpsGlobalType *newSucc=mySucc->LSubst(source,dest,args);
  MpsGlobalRecType *result= new MpsGlobalRecType(myName,*newSucc,newArgs);

  // Clean Up
  delete newSucc;

  return result;
} // }}}
MpsGlobalVarType *MpsGlobalVarType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  return Copy();
} // }}}
MpsGlobalEndType *MpsGlobalEndType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  return Copy();
} // }}}
MpsGlobalSyncType *MpsGlobalSyncType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  map<string,MpsGlobalType*> newBranches;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->LSubst(source,dest,args);
  MpsGlobalSyncType *result = new MpsGlobalSyncType(newBranches,myAssertions);

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}

// Expression Substitution
MpsGlobalMsgType *MpsGlobalMsgType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  MpsMsgType *newMsgType = myMsgType->ESubst(source,dest);
  if (myAssertionType && myId == source)
  { MpsGlobalMsgType *result = new MpsGlobalMsgType(mySender, myReceiver, *newMsgType, *mySucc, *myAssertion, myId);
    delete newMsgType;
    return result;
  }

  set<string> fv=dest.FV();
  if (myAssertionType && fv.find(myId)!=fv.end()) // Rename myId before substituting
  { string newId = MpsExp::NewVar(myId);
    MpsGlobalType *tmpSucc=mySucc->ERename(myId,newId);
    MpsGlobalType *newSucc=tmpSucc->ESubst(source,dest);
    delete tmpSucc;
    MpsExp *tmpAssertion = myAssertion->Rename(myId, newId);
    MpsExp *newAssertion = tmpAssertion->Subst(source,dest);
    delete tmpAssertion;
    MpsGlobalMsgType *result = new MpsGlobalMsgType(mySender, myReceiver, *newMsgType, *newSucc, *newAssertion, newId);
    delete newMsgType;
    delete newSucc;
    delete newAssertion;

    return result;
  }

  MpsGlobalType *newSucc = mySucc->ESubst(source,dest);
  MpsExp *newAssertion=myAssertion->Subst(source,dest);
  
  MpsGlobalMsgType *result=NULL;
  if (myAssertionType)
    result = new MpsGlobalMsgType(mySender, myReceiver, *newMsgType, *newSucc, *myAssertion, myId);
  else
    result = new MpsGlobalMsgType(mySender, myReceiver, *newMsgType, *newSucc);

  // Clean Up
  delete newMsgType;
  delete newSucc;
  delete newAssertion;

  return result;
} // }}}
MpsGlobalBranchType *MpsGlobalBranchType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  map<string,MpsGlobalType*> newBranches;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->ESubst(source,dest);
  map<string,MpsExp*> newAssertions;
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin();it!=myAssertions.end();++it)
    newAssertions[it->first] = it->second->Subst(source,dest);
  MpsGlobalBranchType *result = new MpsGlobalBranchType(mySender, myReceiver, newBranches, newAssertions);

  // Clean up
  DeleteMap(newBranches);
  DeleteMap(newAssertions);

  return result;
} // }}}
MpsGlobalRecType *MpsGlobalRecType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  bool hidden=false;
  vector<TypeArg> newArgs;
  for (vector<TypeArg>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  { MpsMsgType *newType=it->myType->ESubst(source,dest);
    MpsExp *newValue=it->myValue->Subst(source,dest);
    newArgs.push_back(TypeArg(it->myName,*newType, *newValue));
    delete newType;
    delete newValue;
    if (it->myName==source)
      hidden=true;
  }

  if (hidden)
    return new MpsGlobalRecType(myName,*mySucc,newArgs);

  MpsGlobalType *newSucc=mySucc->Copy();
  set<string> fv = dest.FV();

  for (vector<TypeArg>::iterator it=newArgs.begin(); it!=newArgs.end(); ++it)
  { if (fv.find(it->myName)!=fv.end())
    { string newName = MpsExp::NewVar(it->myName);
      MpsGlobalType *tmpSucc = newSucc->ERename(it->myName,newName);
      delete newSucc;
      newSucc=tmpSucc;
      it->myName=newName;
    }
  }
  MpsGlobalType *tmpSucc=newSucc->ESubst(source,dest);
  delete newSucc;
  newSucc=tmpSucc;

  MpsGlobalRecType *result= new MpsGlobalRecType(myName,*newSucc,newArgs);

  // Clean Up
  delete newSucc;

  return result;
} // }}}
MpsGlobalVarType *MpsGlobalVarType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  vector<MpsExp*> newValues;
  for (vector<MpsExp*>::const_iterator it=myValues.begin(); it!=myValues.end(); ++it)
    newValues.push_back((*it)->Subst(source,dest));

  MpsGlobalVarType *result = new MpsGlobalVarType(myName,newValues);

  // Clean Up
  DeleteVector(newValues);

  return result;
} // }}}
MpsGlobalEndType *MpsGlobalEndType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  return Copy();
} // }}}
MpsGlobalSyncType *MpsGlobalSyncType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  map<string,MpsGlobalType*> newBranches;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->ESubst(source,dest);
  map<string,MpsExp*> newAssertions;
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin();it!=myAssertions.end();++it)
    newAssertions[it->first] = it->second->Subst(source,dest);
  MpsGlobalSyncType *result = new MpsGlobalSyncType(newBranches,newAssertions);

  // Clean up
  DeleteMap(newBranches);
  DeleteMap(newAssertions);

  return result;
} // }}}

// Rename all variables to unique names to avoid all name clashes
MpsGlobalMsgType *MpsGlobalMsgType::RenameAll() const // {{{
{
  // Create new id for assertion
  string newId=MpsExp::NewVar(myId);
  // Renaming in message type
  MpsMsgType *newMsgType = myMsgType->RenameAll();
  // Renaming in assertion
  MpsExp *newAssertion=myAssertion->Rename(myId,newId);
  // Renaminf in succ
  MpsGlobalType *tmpSucc=mySucc->LRename(myId,newId);
  MpsGlobalType *newSucc=tmpSucc->RenameAll();
  delete tmpSucc;
  
  // Create result
  MpsGlobalMsgType *result=NULL;
  if (myAssertionType)
    result = new MpsGlobalMsgType(mySender, myReceiver, *newMsgType, *newSucc, *newAssertion, newId);
  else
    result = new MpsGlobalMsgType(mySender, myReceiver, *newMsgType, *newSucc);

  // Clean Up
  delete newMsgType;
  delete newSucc;
  delete newAssertion;

  return result;
} // }}}
MpsGlobalBranchType *MpsGlobalBranchType::RenameAll() const // {{{
{
  // Create new branches with renaming
  map<string,MpsGlobalType*> newBranches;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->RenameAll();
  MpsGlobalBranchType *result = new MpsGlobalBranchType(mySender, myReceiver, newBranches, myAssertions);

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsGlobalRecType *MpsGlobalRecType::RenameAll() const // {{{
{
  // Create new name
  string newName=MpsGlobalType::NewGVar(myName);
  // Create list of renames arguments (with renamed types)
  vector<TypeArg> newArgs;
  for (vector<TypeArg>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  { MpsMsgType *newType=it->myType->RenameAll();
    newArgs.push_back(TypeArg(MpsLocalType::NewLVar(it->myName),*newType, *it->myValue));
    delete newType;
  }

  MpsGlobalType *newSucc=mySucc->GRename(myName,newName);

  for (vector<TypeArg>::iterator it=newArgs.begin(); it!=newArgs.end(); ++it)
  { string newName = MpsExp::NewVar(it->myName);
    MpsGlobalType *tmpSucc = newSucc->ERename(it->myName,newName);
    delete newSucc;
    newSucc=tmpSucc;
    it->myName=newName;
  }
  MpsGlobalType *tmpSucc=newSucc->RenameAll();
  delete newSucc;
  newSucc=tmpSucc;

  MpsGlobalRecType *result= new MpsGlobalRecType(myName,*newSucc,newArgs);

  // Clean Up
  delete newSucc;

  return result;
} // }}}
MpsGlobalVarType *MpsGlobalVarType::RenameAll() const // {{{
{
  return Copy();
} // }}}
MpsGlobalEndType *MpsGlobalEndType::RenameAll() const // {{{
{
  return Copy();
} // }}}
MpsGlobalSyncType *MpsGlobalSyncType::RenameAll() const // {{{
{
  map<string,MpsGlobalType*> newBranches;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->RenameAll();
  MpsGlobalSyncType *result = new MpsGlobalSyncType(newBranches,myAssertions);

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}

// Make parsable string representation
string MpsGlobalMsgType::ToString(const string &indent) const// {{{
{
  string result = int2string(mySender) + "=>" + int2string(myReceiver) + "<";
  string newIndent = indent;
  for (int i=0; i<result.size(); ++i)
    newIndent += " ";
  result += myMsgType->ToString(newIndent) + ">";
  if (myAssertionType)
    result += (string)" as " + myId + "[[" + myAssertion->ToString() + "]]";
  result += (string)";\n"
         + indent + mySucc->ToString(indent);
  return result;
} // }}}
string MpsGlobalBranchType::ToString(const string &indent) const // {{{
{
  string newIndent = indent + "    ";
  string result = int2string(mySender) + "=>" + int2string(myReceiver) + "\n"
                + indent + "{ ";
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
  {
    if (it != myBranches.begin())
      result += ",\n" + indent + "  ";

    // Print Label
    result += it->first;
    // Print assertion if any
    map<string,MpsExp*>::const_iterator assertion = myAssertions.find(it->first);
    if (assertion != myAssertions.end())
      result += + "[[" + assertion->second->ToString() + "]]";
    // Print branch
    result += + ":\n"
            + newIndent + it->second->ToString(newIndent);
  }
  result += "\n" + indent + "}";

  return result;
} // }}}
string MpsGlobalRecType::ToString(const string &indent) const// {{{
{
  string result = "rec " + myName;
  if (myArgs.size()>0)
  { // Declare all state variables
    result += "<";
    for (int i=0; i<myArgs.size(); ++i)
    { // Insert seperator
      if (i>0)
        result+=", ";
      // Declare NAME:TYPE=VALUE
      result += myArgs[i].myName + ":"
              + myArgs[i].myType->ToString() + "="
              + myArgs[i].myValue->ToString();
    }
    result += ">";
  }
  result +=".\n" + indent + mySucc->ToString(indent);
  return result;
} // }}}
string MpsGlobalVarType::ToString(const string &indent) const// {{{
{
  string result = myName;
  if (myValues.size()>0)
  { // Declare all state arguments
    result += "<";
    for (int i=0; i<myValues.size(); ++i)
    { // Insert seperator
      if (i>0)
        result+=", ";
      // Add value
      result += myValues[i]->ToString();
    }
    result += ">";
  }
  return result;
} // }}}
string MpsGlobalEndType::ToString(const string &indent) const// {{{
{
  return "Gend";
} // }}}
string MpsGlobalSyncType::ToString(const string &indent) const// {{{
{
  string newIndent = indent + "    ";
  string result = "{ ";
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
  {
    if (it != myBranches.begin())
      result += ",\n" + indent + "  ";

    // Print Label
    result += it->first;
    // Print assertion if any
    map<string,MpsExp*>::const_iterator assertion = myAssertions.find(it->first);
    if (assertion != myAssertions.end())
      result += + "[[" + assertion->second->ToString() + "]]";
    // Print Branch
    result += ":\n"
            + newIndent + it->second->ToString(newIndent);
  }
  result += "\n" + indent + "}";

  return result;
} // }}}

// Make string representation with Latex markup
string MpsGlobalMsgType::ToTex(int indent, int sw) const// {{{
{
  string result = ToTex_PP(mySender) + "$\\to$" + ToTex_PP(myReceiver)
                + "$\\langle$" + myMsgType->ToTex(indent+2,sw) + "$\\rangle$";
  if (myAssertionType)
    result += (string)" as " + myId + "[[" + myAssertion->ToString() + "]]";
  result += (string)";\\newline\n"
          + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
  return result;
} // }}}
string MpsGlobalBranchType::ToTex(int indent, int sw) const // {{{
{
  string result = ToTex_PP(mySender) + "$\\to$" + ToTex_PP(myReceiver) + "\\newline"
                + ToTex_Hspace(indent,sw) + "\\{ ";
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
  {
    if (it != myBranches.begin())
      result += (string)", \\newline\n"
              + ToTex_Hspace(indent+2,sw);

    // Print Label
    result += ToTex_Label(it->first);
    // Print assertion if any
    map<string,MpsExp*>::const_iterator assertion = myAssertions.find(it->first);
    if (assertion != myAssertions.end())
      result += + "$\\llbracket$" + assertion->second->ToString() + "$\\rrbracket$";
    // Print branch
    result += (string)":\\newline\n"
            + ToTex_Hspace(indent+4,sw) + it->second->ToTex(indent+4,sw);
  }
  result += (string)"\\newline\n" + ToTex_Hspace(indent,sw) + "\\}\n";

  return result;
} // }}}
string MpsGlobalRecType::ToTex(int indent, int sw) const// {{{
{
  string result = ToTex_KW("rec") + " " + ToTex_Var(myName);
  if (myArgs.size()>0)
  { // Declare all state variables
    result += "$\\langle$";
    for (int i=0; i<myArgs.size(); ++i)
    { // Insert seperator
      if (i>0)
        result+=", ";
      // Declare NAME:TYPE=VALUE
      result += myArgs[i].myName + ":"
              + myArgs[i].myType->ToTex(indent+2,sw) + "="
              + myArgs[i].myValue->ToString();
    }
    result += "$\\rangle$";
  }
  result += (string)".\\newline\n"
          + ToTex_Hspace(indent+2,sw) + mySucc->ToTex(indent+2,sw);
  return result;
} // }}}
string MpsGlobalVarType::ToTex(int indent, int sw) const// {{{
{
  string result = ToTex_Var(myName);
  if (myValues.size()>0)
  { // Declare all state arguments
    result += "$\\langle$";
    for (int i=0; i<myValues.size(); ++i)
    { // Insert seperator
      if (i>0)
        result+=", ";
      // Add value
      result += myValues[i]->ToString();
    }
    result += "$\\rangle$\n";
  }
  return result;
} // }}}
string MpsGlobalEndType::ToTex(int indent, int sw) const// {{{
{
  return ToTex_KW("end");
} // }}}
string MpsGlobalSyncType::ToTex(int indent, int sw) const// {{{
{
  string result = "\\{ ";
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
  {
    if (it != myBranches.begin())
      result += (string)",\\newline\n"
              + ToTex_Hspace(indent+2,sw);

    // Print Label
    result += ToTex_Label(it->first);
    // Print assertion if any
    map<string,MpsExp*>::const_iterator assertion = myAssertions.find(it->first);
    if (assertion != myAssertions.end() && assertion->second->ToString()!="true")
      result += + "$\\llbracket$" + assertion->second->ToString() + "$\\rrbracket$";
    // Print Branch
    result += ":\\newline\n"
            + ToTex_Hspace(indent+4,sw) + it->second->ToTex(indent+4,sw);
  }
  result += (string)"\\newline\n"
          + ToTex_Hspace(indent,sw) + "\\}";

  return result;
} // }}}

// Projection from global to local type
MpsLocalType *MpsGlobalMsgType::Project(int pid) const // {{{
{
  MpsLocalType *result = mySucc->Project(pid);
  if (pid==myReceiver) // Sender {{{
  {
    MpsLocalType *tmp=NULL;
    if (myAssertionType)
      tmp = new MpsLocalRcvType(mySender,*myMsgType,*result,*myAssertion,myId);
    else
      tmp = new MpsLocalRcvType(mySender,*myMsgType,*result);
    delete result;
    result=tmp;
  } // }}}
  if (pid==mySender) // Sender {{{
  {
    MpsLocalType *tmp=NULL;
    if (myAssertionType)
      tmp=new MpsLocalSendType(myReceiver,*myMsgType,*result,*myAssertion,myId);
    else
      tmp=new MpsLocalSendType(myReceiver,*myMsgType,*result);
    delete result;
    result=tmp;
  } // }}}
  if (myAssertionType && pid!=mySender && pid!=myReceiver) // Sender {{{
  {
    MpsLocalType *tmp=new MpsLocalForallType(myId,*myAssertion,*result);
    delete result;
    result=tmp;
  } // }}}
  return result;
} // }}}
MpsLocalType *MpsGlobalBranchType::Project(int pid) const // {{{
{
  MpsLocalType *result=NULL;

  // Make the projection of each branch;
  map<string,MpsLocalType*> branches;
  branches.clear();
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    branches[it->first] = it->second->Project(pid);

  if (pid==mySender) // Select {{{
  {
    result = new MpsLocalSelectType(myReceiver,branches, myAssertions);
  } // }}}
  else if (pid==myReceiver) // Branch {{{
  {
    result = new MpsLocalBranchType(mySender,branches, myAssertions);
  } // }}}
  else // Merge {{{
  {
    map<string,MpsLocalType*>::const_iterator it=branches.begin();
    result = it->second->Copy();
    while (it!=branches.end())
    {
      MpsLocalType *tmpresult = result->Merge(*it->second);
      delete result;
      result = tmpresult;
      it++;
    }
  } // }}}

  // Clean up
  DeleteMap(branches);

  return result;
} // }}}
MpsLocalType *MpsGlobalRecType::Project(int pid) const // {{{
{
  MpsLocalType *succ = mySucc->Project(pid);
  string name = myName;
  replace(name.begin(),name.end(),'$','%');
  MpsLocalType *result = new MpsLocalRecType(name,*succ,myArgs);
  delete succ;
  return result;
} // }}}
MpsLocalType *MpsGlobalVarType::Project(int pid) const // {{{
{
  string name = myName;
  replace(name.begin(),name.end(),'$','%');
  MpsLocalType *result = new MpsLocalVarType(name, myValues);
  return result;
} // }}}
MpsLocalType *MpsGlobalEndType::Project(int pid) const // {{{
{
  MpsLocalType *result = new MpsLocalEndType();
  return result;
} // }}}
MpsLocalType *MpsGlobalSyncType::Project(int pid) const // {{{
{
  // Make the projection of each branch;
  map<string,MpsLocalType*> branches;
  branches.clear();
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    branches[it->first] = it->second->Project(pid);

  MpsLocalType *result=new MpsLocalSyncType(branches, myAssertions);

  // Clean up
  DeleteMap(branches);

  return result;
} // }}}

// Find Max(Pids(G))
int MpsGlobalMsgType::GetMaxPid() const // {{{
{
  int maxpid = mySucc->GetMaxPid();
  if (mySender > maxpid)
    maxpid = mySender;
  if (myReceiver > maxpid)
    maxpid = myReceiver;
  return maxpid;
} // }}}
int MpsGlobalBranchType::GetMaxPid() const // {{{
{
  int maxpid = mySender;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    int succmax = it->second->GetMaxPid();
    if (succmax > maxpid)
      maxpid = succmax;
  }
  if (myReceiver > maxpid)
    maxpid = myReceiver;
  return maxpid;
} // }}}
int MpsGlobalRecType::GetMaxPid() const // {{{
{
  int maxpid = mySucc->GetMaxPid();
  return maxpid;
} // }}}
int MpsGlobalVarType::GetMaxPid() const // {{{
{
  return 1;
} // }}}
int MpsGlobalEndType::GetMaxPid() const // {{{
{
  return 1;
} // }}}
int MpsGlobalSyncType::GetMaxPid() const // {{{
{
  int maxpid = 1;
  for (map<string,MpsGlobalType*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    int succmax = it->second->GetMaxPid();
    if (succmax > maxpid)
      maxpid = succmax;
  }
  return maxpid;
} // }}}

/* MpsLocalType implementation
 *
 */

// Constructors
MpsLocalSendType::MpsLocalSendType(int receiver, const MpsMsgType &msgtype, const MpsLocalType &succ) // {{{
{
  myReceiver = receiver;
  myMsgType = msgtype.Copy();
  mySucc = succ.Copy();
  myAssertionType=false;
  myAssertion=new MpsBoolVal(true);
} // }}}
MpsLocalSendType::MpsLocalSendType(int receiver, const MpsMsgType &msgtype, const MpsLocalType &succ, const MpsExp &assertion, const std::string &id) // {{{
{
  myReceiver = receiver;
  myMsgType = msgtype.Copy();
  mySucc = succ.Copy();
  myAssertionType=true;
  myAssertion=assertion.Copy();
  myId=id;
} // }}}
MpsLocalRcvType::MpsLocalRcvType(int sender, const MpsMsgType &msgtype, const MpsLocalType &succ) // {{{
{
  mySender = sender;
  myMsgType = msgtype.Copy();
  mySucc = succ.Copy();
  myAssertionType=false;
  myAssertion=new MpsBoolVal(true);
} // }}}
MpsLocalRcvType::MpsLocalRcvType(int sender, const MpsMsgType &msgtype, const MpsLocalType &succ, const MpsExp &assertion, const std::string &id) // {{{
{
  mySender = sender;
  myMsgType = msgtype.Copy();
  mySucc = succ.Copy();

  myAssertionType=true;
  myId=id;
  myAssertion=assertion.Copy();
} // }}}
MpsLocalForallType::MpsLocalForallType(const std::string &name, const MpsExp &assertion, const MpsLocalType &succ) // {{{
{
  myName=name;
  myAssertion=assertion.Copy();
  mySucc = succ.Copy();
} // }}}
MpsLocalSelectType::MpsLocalSelectType(int receiver, const std::map<std::string,MpsLocalType*> &branches, const map<string,MpsExp*> &assertions) // {{{
{
  myReceiver = receiver;
  myBranches.clear();
  for (map<string,MpsLocalType*>::const_iterator it=branches.begin();it!=branches.end();++it)
    myBranches[it->first]=it->second->Copy();
  myAssertions.clear();
  for (map<string,MpsExp*>::const_iterator it=assertions.begin();it!=assertions.end();++it)
    myAssertions[it->first]=it->second->Copy();
} // }}}
MpsLocalBranchType::MpsLocalBranchType(int sender, const map<string,MpsLocalType*> &branches, const map<string,MpsExp*> &assertions) // {{{
{
  mySender = sender;
  myBranches.clear();
  for (map<string,MpsLocalType*>::const_iterator it=branches.begin();it!=branches.end();++it)
    myBranches[it->first]=it->second->Copy();
  for (map<string,MpsExp*>::const_iterator it=assertions.begin();it!=assertions.end();++it)
    myAssertions[it->first]=it->second->Copy();
} // }}}
MpsLocalRecType::MpsLocalRecType(std::string name, const MpsLocalType &succ, const vector<TypeArg> &args) // {{{
{
  myName = name;
  mySucc = succ.Copy();
  myArgs=args;
} // }}}
MpsLocalVarType::MpsLocalVarType(string name, const vector<MpsExp*> &values) // {{{
{
  myName = name;
  myValues.clear();
  for (vector<MpsExp*>::const_iterator it=values.begin(); it!=values.end(); ++it)
    myValues.push_back((*it)->Copy());
} // }}}
MpsLocalEndType::MpsLocalEndType() // {{{
{
} // }}}
MpsLocalSyncType::MpsLocalSyncType(const map<string,MpsLocalType*> &branches, const map<string,MpsExp*> &assertions) // {{{
{
  myBranches.clear();
  for (map<string,MpsLocalType*>::const_iterator it=branches.begin();it!=branches.end();++it)
    myBranches[it->first]=it->second->Copy();
  for (map<string,MpsExp*>::const_iterator it=assertions.begin();it!=assertions.end();++it)
    myAssertions[it->first]=it->second->Copy();
} // }}}

// Destructors
MpsLocalType::~MpsLocalType() // {{{
{
} // }}}
MpsLocalSendType::~MpsLocalSendType() // {{{
{
  delete myMsgType;
  delete mySucc;
  delete myAssertion;
} // }}}
MpsLocalRcvType::~MpsLocalRcvType() // {{{
{
  delete myMsgType;
  delete mySucc;
  delete myAssertion;
} // }}}
MpsLocalForallType::~MpsLocalForallType() // {{{
{
  delete myAssertion;
  delete mySucc;
} // }}}
MpsLocalSelectType::~MpsLocalSelectType() // {{{
{
  DeleteMap(myBranches);
  DeleteMap(myAssertions);
} // }}}
MpsLocalBranchType::~MpsLocalBranchType() // {{{
{
  DeleteMap(myBranches);
  DeleteMap(myAssertions);
} // }}}
MpsLocalRecType::~MpsLocalRecType() // {{{
{
  delete mySucc;
} // }}}
MpsLocalVarType::~MpsLocalVarType() // {{{
{
  DeleteVector(myValues);
} // }}}
MpsLocalEndType::~MpsLocalEndType() // {{{
{
} // }}}
MpsLocalSyncType::~MpsLocalSyncType() // {{{
{
  DeleteMap(myBranches);
  DeleteMap(myAssertions);
} // }}}

// Make Deep copy
MpsLocalSendType *MpsLocalSendType::Copy() const // {{{
{
  if (myAssertionType)
    return new MpsLocalSendType(myReceiver,*myMsgType,*mySucc,*myAssertion,myId);
  else
    return new MpsLocalSendType(myReceiver,*myMsgType,*mySucc);
} // }}}
MpsLocalRcvType *MpsLocalRcvType::Copy() const // {{{
{
  if (myAssertionType)
    return new MpsLocalRcvType(mySender,*myMsgType,*mySucc,*myAssertion,myId);
  else
    return new MpsLocalRcvType(mySender,*myMsgType,*mySucc);
} // }}}
MpsLocalForallType *MpsLocalForallType::Copy() const // {{{
{
  return new MpsLocalForallType(myName, *myAssertion,*mySucc);
} // }}}
MpsLocalSelectType *MpsLocalSelectType::Copy() const // {{{
{
  return new MpsLocalSelectType(myReceiver,myBranches,myAssertions);
} // }}}
MpsLocalBranchType *MpsLocalBranchType::Copy() const // {{{
{
  return new MpsLocalBranchType(mySender,myBranches, myAssertions);
} // }}}
MpsLocalRecType *MpsLocalRecType::Copy() const // {{{
{
  return new MpsLocalRecType(myName, *mySucc, myArgs);
} // }}}
MpsLocalVarType *MpsLocalVarType::Copy() const // {{{
{
  return new MpsLocalVarType(myName,myValues);
} // }}}
MpsLocalEndType *MpsLocalEndType::Copy() const // {{{
{
  return new MpsLocalEndType();
} // }}}
MpsLocalSyncType *MpsLocalSyncType::Copy() const // {{{
{
  return new MpsLocalSyncType(myBranches,myAssertions);
} // }}}

// TODO: Improve equality to co-inductive type equality and subtyping
// Compare
// Helper function, to eliminate exceeding foralls
bool ERROR_LOCALEQ(const MpsExp &Theta, const MpsLocalType &lhs, const MpsLocalType &rhs, string msg) // {{{
{
#if APIMS_DEBUG_LEVEL>20
  cerr << "!!!!Types are not equal:" << msg << endl
       << "!!!!Theta: " << Theta.ToString() << endl
       << "!!!!!!LHS: " << lhs.ToString("!!!!!!!!!! ") << endl
       << "!!!!!!RHS: " << rhs.ToString("!!!!!!!!!! ") << endl;
#endif
  return false;
} // }}}
bool CompareForall(const MpsExp &Theta, const MpsLocalType &lhs, const MpsLocalType &rhs) // {{{
{ 
  const MpsLocalForallType *lhsptr=dynamic_cast<const MpsLocalForallType*>(&lhs);
  const MpsLocalForallType *rhsptr=dynamic_cast<const MpsLocalForallType*>(&rhs);
  if (lhsptr!=NULL && rhsptr!=NULL)
  {
    string newName = MpsExp::NewVar(lhsptr->GetName());
    MpsExp *lhsAssertion = lhsptr->GetAssertion().Rename(lhsptr->GetName(),newName);
    MpsExp *rhsAssertion = rhsptr->GetAssertion().Rename(rhsptr->GetName(),newName);
    bool checkAssertions = CompareAssertions(Theta,*lhsAssertion,*rhsAssertion);
    delete rhsAssertion;
    if (not checkAssertions)
    { delete lhsAssertion;
      return ERROR_LOCALEQ(Theta,lhs,rhs,"Assertions not quivalent");
    }
    MpsLocalType *lhsSucc=lhsptr->GetSucc()->ERename(lhsptr->GetName(),newName);
    MpsLocalType *rhsSucc=rhsptr->GetSucc()->ERename(rhsptr->GetName(),newName);
    MpsExp *newTheta = new MpsBinOpExp("and",Theta,*lhsAssertion,MpsBoolMsgType(),MpsBoolMsgType());
    delete lhsAssertion;
    bool result = CompareForall(*newTheta,*lhsSucc,*rhsSucc);
    delete lhsSucc;
    delete rhsSucc;
    delete newTheta;
    return result;
  }
  else if (lhsptr!=NULL)
  {
    string newName = MpsExp::NewVar(lhsptr->GetName());
    MpsLocalType *lhsSucc=lhsptr->GetSucc()->ERename(lhsptr->GetName(),newName);
    MpsExp *newAssertion = lhsptr->GetAssertion().Rename(lhsptr->GetName(),newName);
    MpsExp *newTheta = new MpsBinOpExp("and",Theta,*newAssertion,MpsBoolMsgType(),MpsBoolMsgType());
    delete newAssertion;
    bool result = CompareForall(*newTheta,*lhsSucc,rhs);
    delete lhsSucc;
    delete newTheta;
    return result;
  }
  else if (rhsptr!=NULL)
  {
    string newName = MpsExp::NewVar(rhsptr->GetName());
    MpsLocalType *rhsSucc=rhsptr->GetSucc()->ERename(rhsptr->GetName(),newName);
    MpsExp *newAssertion = rhsptr->GetAssertion().Rename(rhsptr->GetName(),newName);
    MpsExp *newTheta = new MpsBinOpExp("and",Theta,*newAssertion,MpsBoolMsgType(),MpsBoolMsgType());
    delete newAssertion;
    bool result = CompareForall(*newTheta,lhs,*rhsSucc);
    delete rhsSucc;
    delete newTheta;
    return result;
  }
  return lhs.Equal(Theta,rhs);
} // }}}
bool MpsLocalSendType::Equal(const MpsExp &Theta,const MpsLocalType &rhs) const // {{{
{
  // Remove forall instances
  if (dynamic_cast<const MpsLocalForallType*>(&rhs)!=NULL)
    return CompareForall(Theta,*this,rhs);
  // Compare
  const MpsLocalSendType *rhsptr=dynamic_cast<const MpsLocalSendType*>(&rhs);
  if (rhsptr==NULL)
    return ERROR_LOCALEQ(Theta,*this,rhs,"Different head");
  if (myAssertionType != rhsptr->myAssertionType)
    return ERROR_LOCALEQ(Theta,*this,rhs,"Different assertion type");
  if (myReceiver != rhsptr->myReceiver ||
      not myMsgType->Equal(Theta,*rhsptr->myMsgType))
    return ERROR_LOCALEQ(Theta,*this,rhs,"Different message type");
  if (myAssertionType && myId != rhsptr->myId)
  { // Rename
    string newId = MpsExp::NewVar(myId);
    // Compare Assertions
    MpsExp *lhsAssertion=myAssertion->Rename(myId,newId);
    MpsExp *rhsAssertion=rhsptr->myAssertion->Rename(rhsptr->myId,newId);
    bool checkAssercion = CompareAssertions(Theta,*lhsAssertion,*rhsAssertion);
    MpsExp *newTheta=new MpsBinOpExp("and",Theta,*lhsAssertion,MpsBoolMsgType(),MpsBoolMsgType());
    delete lhsAssertion;
    delete rhsAssertion;
    if (not checkAssercion)
    { delete newTheta;
      return ERROR_LOCALEQ(Theta,*this,rhs,"Different assertions");
    }
    // Compare Successors
    MpsLocalType *lhsSucc=mySucc->ERename(myId,newId);
    MpsLocalType *rhsSucc=rhsptr->mySucc->ERename(rhsptr->myId,newId);
    bool checkSucc = lhsSucc->Equal(*newTheta,*rhsSucc);
    delete newTheta;
    delete lhsSucc;
    delete rhsSucc;
    if (not checkSucc)
      return false;
  }
  else
  {
    if (myAssertionType)
    { if ( not CompareAssertions(Theta,*myAssertion,*rhsptr->myAssertion))
        return ERROR_LOCALEQ(Theta,*this,rhs,"Different assertions");
      MpsExp *newTheta=new MpsBinOpExp("and",Theta,*myAssertion,MpsBoolMsgType(),MpsBoolMsgType());
      bool checkSucc=mySucc->Equal(*newTheta,*rhsptr->mySucc);
      delete newTheta;
      if (not checkSucc)
        return false;
    }
    else
      if (not (mySucc->Equal(Theta,*rhsptr->mySucc))) // Compare Successors
        return false;
  }
  return true;
} // }}}
bool MpsLocalRcvType::Equal(const MpsExp &Theta, const MpsLocalType &rhs) const // {{{
{
  // Remove forall instances
  if (dynamic_cast<const MpsLocalForallType*>(&rhs)!=NULL)
    return CompareForall(Theta,*this,rhs);
  // Compare
  const MpsLocalRcvType *rhsptr=dynamic_cast<const MpsLocalRcvType*>(&rhs);
  if (rhsptr==NULL)
    return ERROR_LOCALEQ(Theta,*this,rhs,"Different head");
  if (myAssertionType != rhsptr->myAssertionType)
    return ERROR_LOCALEQ(Theta,*this,rhs,"Different assertion type");
  if (mySender != rhsptr->mySender ||
      not myMsgType->Equal(Theta,*rhsptr->myMsgType))
    return ERROR_LOCALEQ(Theta,*this,rhs,"");
  if (myAssertionType && myId != rhsptr->myId)
  { // Rename
    string newId = MpsExp::NewVar(myId);
    // Compare Assertions
    MpsExp *lhsAssertion=myAssertion->Rename(myId,newId);
    MpsExp *rhsAssertion=rhsptr->myAssertion->Rename(rhsptr->myId,newId);
    bool checkAssercion = CompareAssertions(Theta,*lhsAssertion,*rhsAssertion);
    MpsExp *newTheta=new MpsBinOpExp("and",Theta,*lhsAssertion,MpsBoolMsgType(),MpsBoolMsgType());
    delete lhsAssertion;
    delete rhsAssertion;
    if (not checkAssercion)
    { delete newTheta;
      return ERROR_LOCALEQ(Theta,*this,rhs,"");
    }
    MpsLocalType *lhsSucc=mySucc->ERename(myId,newId);
    MpsLocalType *rhsSucc=rhsptr->mySucc->ERename(rhsptr->myId,newId);
    bool checkSucc = lhsSucc->Equal(*newTheta,*rhsSucc);
    delete newTheta;
    delete lhsSucc;
    delete rhsSucc;
    if (not checkSucc)
      return false;
  }
  else
  {
    if (myAssertionType)
    { if ( not CompareAssertions(Theta,*myAssertion,*rhsptr->myAssertion))
        return ERROR_LOCALEQ(Theta,*this,rhs,"");
      MpsExp *newTheta=new MpsBinOpExp("and",Theta,*myAssertion,MpsBoolMsgType(),MpsBoolMsgType());
      bool checkSucc=mySucc->Equal(*newTheta,*rhsptr->mySucc);
      delete newTheta;
      if (not checkSucc)
        return false;
    }
    else
      if (not (mySucc->Equal(Theta,*rhsptr->mySucc))) // Compare Successors
        return false;
  }
  return true;
} // }}}
bool MpsLocalForallType::Equal(const MpsExp &Theta, const MpsLocalType &rhs) const // {{{
{
  return CompareForall(Theta,*this,rhs);
} // }}}
bool MpsLocalSelectType::Equal(const MpsExp &Theta, const MpsLocalType &rhs) const // {{{
{
  // Remove forall instances
  if (dynamic_cast<const MpsLocalForallType*>(&rhs)!=NULL)
    return CompareForall(Theta,*this,rhs);
  // Compare
  // Check top-level type and channel
  const MpsLocalSelectType *rhsptr=dynamic_cast<const MpsLocalSelectType*>(&rhs);
  if (rhsptr==NULL)
    return ERROR_LOCALEQ(Theta,*this,rhs,"");
  if (myReceiver != rhsptr->myReceiver)
    return ERROR_LOCALEQ(Theta,*this,rhs,"");

  // Check Branch Assertions
  // Check label in lhs means label in rhs and their types correspond
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin();it!=myAssertions.end();++it)
  {
    map<string,MpsExp*>::const_iterator it2=rhsptr->myAssertions.find(it->first);
    if (it2 == rhsptr->myAssertions.end())
      return ERROR_LOCALEQ(Theta,*this,rhs,"");
    if (not CompareAssertions(Theta,*it->second,*it2->second))
      return ERROR_LOCALEQ(Theta,*this,rhs,"");
  }
  // Check label in rhs means label in lhs
  for (map<string,MpsExp*>::const_iterator it2=rhsptr->myAssertions.begin();it2!=rhsptr->myAssertions.end();++it2)
    if (myAssertions.find(it2->first) == myAssertions.end())
      return ERROR_LOCALEQ(Theta,*this,rhs,"");

  // Check Branch Processes
  // Check label in lhs means label in rhs and their types correspond
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
  {
    map<string,MpsLocalType*>::const_iterator it2=rhsptr->myBranches.find(it->first);
    if (it2 == rhsptr->myBranches.end())
      return ERROR_LOCALEQ(Theta,*this,rhs,"");

    map<string,MpsExp*>::const_iterator assertion=myAssertions.find(it->first);
    MpsExp *newTheta=NULL;
    if (assertion==myAssertions.end())
      newTheta=Theta.Copy();
    else
      newTheta=new MpsBinOpExp("and",Theta,*assertion->second,MpsBoolMsgType(),MpsBoolMsgType());
    bool checkSucc=it->second->Equal(*newTheta,*it2->second);
    delete newTheta;
    if (not checkSucc)
      return false;
  }
  // Check label in rhs means label in lhs
  for (map<string,MpsLocalType*>::const_iterator it2=rhsptr->myBranches.begin();it2!=rhsptr->myBranches.end();++it2)
    if (myBranches.find(it2->first) == myBranches.end())
      return ERROR_LOCALEQ(Theta,*this,rhs,"");

  // All checks passed
  return true;
} // }}}
bool MpsLocalBranchType::Equal(const MpsExp &Theta, const MpsLocalType &rhs) const // {{{
{
  // Remove forall instances
  if (dynamic_cast<const MpsLocalForallType*>(&rhs)!=NULL)
    return CompareForall(Theta,*this,rhs);
  // Compare
  // Check top-level type and channel
  const MpsLocalBranchType *rhsptr=dynamic_cast<const MpsLocalBranchType*>(&rhs);
  if (typeid(rhs)!=typeid(MpsLocalBranchType))
    return ERROR_LOCALEQ(Theta,*this,rhs,"");
  if (mySender != rhsptr->mySender)
    return ERROR_LOCALEQ(Theta,*this,rhs,"");

  // Check Branch Assertions
  // Check label in lhs means label in rhs and their types correspond
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin();it!=myAssertions.end();++it)
  {
    map<string,MpsExp*>::const_iterator it2=rhsptr->myAssertions.find(it->first);
    if (it2 == rhsptr->myAssertions.end())
      return ERROR_LOCALEQ(Theta,*this,rhs,"");
    if (not CompareAssertions(Theta,*it->second,*it2->second))
      return ERROR_LOCALEQ(Theta,*this,rhs,"");
  }
  // Check label in rhs means label in lhs
  for (map<string,MpsExp*>::const_iterator it2=rhsptr->myAssertions.begin();it2!=rhsptr->myAssertions.end();++it2)
    if (myAssertions.find(it2->first) == myAssertions.end())
      return ERROR_LOCALEQ(Theta,*this,rhs,"");

  // Check Branch Processes
  // Check label in lhs means label in rhs and their types correspond
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
  {
    map<string,MpsLocalType*>::const_iterator it2=rhsptr->myBranches.find(it->first);
    if (it2 == rhsptr->myBranches.end())
      return ERROR_LOCALEQ(Theta,*this,rhs,"");

    map<string,MpsExp*>::const_iterator assertion=myAssertions.find(it->first);
    MpsExp *newTheta=NULL;
    if (assertion==myAssertions.end())
      newTheta=Theta.Copy();
    else
      newTheta=new MpsBinOpExp("and",Theta,*assertion->second,MpsBoolMsgType(),MpsBoolMsgType());
    bool checkSucc=it->second->Equal(*newTheta,*it2->second);
    delete newTheta;
    if (not checkSucc)
      return false;
  }
  // Check label in rhs means label in lhs
  for (map<string,MpsLocalType*>::const_iterator it2=rhsptr->myBranches.begin();it2!=rhsptr->myBranches.end();++it2)
    if (myBranches.find(it2->first) == myBranches.end())
      return ERROR_LOCALEQ(Theta,*this,rhs,"");

  // All checks passed
  return true;
} // }}}
bool MpsLocalRecType::Equal(const MpsExp &Theta, const MpsLocalType &rhs) const // {{{
{
  // Remove forall instances
  if (dynamic_cast<const MpsLocalForallType*>(&rhs)!=NULL)
    return CompareForall(Theta,*this,rhs);
  // Compare
  const MpsLocalRecType *rhsptr=dynamic_cast<const MpsLocalRecType*>(&rhs);
  if (rhsptr==NULL) // RHS is not recursive type
    return ERROR_LOCALEQ(Theta,*this,rhs,"");
  if (myArgs.size()!=rhsptr->GetArgs().size()) // Different number of arguments
    return ERROR_LOCALEQ(Theta,*this,rhs,"");

  for (int i=0; i<myArgs.size(); ++i)
  { if (not myArgs[i].myType->Equal(Theta,*rhsptr->GetArgs()[i].myType))
      return ERROR_LOCALEQ(Theta,*this,rhs,"");
    if (not CompareAssertions(Theta,*myArgs[i].myValue,*rhsptr->GetArgs()[i].myValue))
      return ERROR_LOCALEQ(Theta,*this,rhs,"");
  }
  MpsLocalType *newlhs = GetSucc()->Copy();
  MpsLocalType *newrhs = rhsptr->GetSucc()->Copy();
   
  for (int i=0; i<myArgs.size(); ++i)
  { if (not (myArgs[i].myName == rhsptr->GetArgs()[i].myName)) // Rename to common name
    { string newName=MpsExp::NewVar(myArgs[i].myName);
      MpsLocalType *newType = newlhs->ERename(myArgs[i].myName,newName);
      delete newlhs;
      newlhs=newType;
      newType = newrhs->ERename(rhsptr->GetArgs()[i].myName,newName);
      delete newrhs;
      newrhs=newType;
    }
  }
  if (myName != rhsptr->myName) // Rename to common name
  {
    string newName = NewLVar(myName);
    MpsLocalType *newType = newlhs->LRename(myName,newName);
    delete newlhs;
    newlhs=newType;
    newType = newrhs->LRename(rhsptr->myName,newName);
    delete newrhs;
    newrhs=newType;
  }
  // Now compare resulting successors
  bool result = newlhs->Equal(Theta,*newrhs);

  // Clean Up
  delete newlhs;
  delete newrhs;

  return result;
} // }}}
bool MpsLocalVarType::Equal(const MpsExp &Theta, const MpsLocalType &rhs) const // {{{
{
  // Remove forall instances
  if (dynamic_cast<const MpsLocalForallType*>(&rhs)!=NULL)
    return CompareForall(Theta,*this,rhs);
  // Compare
  if (typeid(rhs)!=typeid(MpsLocalVarType))
    return ERROR_LOCALEQ(Theta,*this,rhs,"");
  MpsLocalVarType *rhsptr=(MpsLocalVarType*)&rhs;
  if (myValues.size() != rhsptr->myValues.size())
    return ERROR_LOCALEQ(Theta,*this,rhs,"");
  for (int i=0; i<myValues.size(); ++i)
    if (not CompareAssertions(Theta,*myValues[i],*rhsptr->myValues[i]))
      return ERROR_LOCALEQ(Theta,*this,rhs,"");
  return myName == rhsptr->myName;
} // }}}
bool MpsLocalEndType::Equal(const MpsExp &Theta, const MpsLocalType &rhs) const // {{{
{
  // Remove forall instances
  if (dynamic_cast<const MpsLocalForallType*>(&rhs)!=NULL)
    return CompareForall(Theta,*this,rhs);
  // Compare
  if (dynamic_cast<const MpsLocalEndType*>(&rhs)==NULL)
    return ERROR_LOCALEQ(Theta,*this,rhs,"");
  return true;
} // }}}
bool MpsLocalSyncType::Equal(const MpsExp &Theta, const MpsLocalType &rhs) const // {{{
{
  // Remove forall instances
  if (dynamic_cast<const MpsLocalForallType*>(&rhs)!=NULL)
    return CompareForall(Theta,*this,rhs);
  // Compare
  // Check top-level type and channel
  const MpsLocalSyncType *rhsptr=dynamic_cast<const MpsLocalSyncType*>(&rhs);
  if (rhsptr==NULL)
    return ERROR_LOCALEQ(Theta,*this,rhs,"");

  // Check Branch Assertions
  // Check label in lhs means label in rhs and their types correspond
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin();it!=myAssertions.end();++it)
  {
    map<string,MpsExp*>::const_iterator it2=rhsptr->myAssertions.find(it->first);
    if (it2 == rhsptr->myAssertions.end())
      return ERROR_LOCALEQ(Theta,*this,rhs,"");
    if (not CompareAssertions(Theta,*it->second,*it2->second))
      return ERROR_LOCALEQ(Theta,*this,rhs,"");
  }
  // Check label in rhs means label in lhs
  for (map<string,MpsExp*>::const_iterator it2=rhsptr->myAssertions.begin();it2!=rhsptr->myAssertions.end();++it2)
    if (myAssertions.find(it2->first) == myAssertions.end())
      return ERROR_LOCALEQ(Theta,*this,rhs,"");

  // Check Branch Processes
  // Check label in lhs means label in rhs and their types correspond
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
  {
    map<string,MpsLocalType*>::const_iterator it2=rhsptr->myBranches.find(it->first);
    if (it2 == rhsptr->myBranches.end())
      return ERROR_LOCALEQ(Theta,*this,rhs,"");

    map<string,MpsExp*>::const_iterator assertion=myAssertions.find(it->first);
    MpsExp *newTheta=NULL;
    if (assertion==myAssertions.end())
      newTheta=Theta.Copy();
    else
      newTheta=new MpsBinOpExp("and",Theta,*assertion->second,MpsBoolMsgType(),MpsBoolMsgType());
    bool checkSucc=it->second->Equal(*newTheta,*it2->second);
    delete newTheta;
    if (not checkSucc)
      return false;
  }
  // Check label in rhs means label in lhs
  for (map<string,MpsLocalType*>::const_iterator it2=rhsptr->myBranches.begin();it2!=rhsptr->myBranches.end();++it2)
    if (myBranches.find(it2->first) == myBranches.end())
      return ERROR_LOCALEQ(Theta,*this,rhs,"");

  // All checks passed
  return true;
} // }}}

// Free Global Type Variables
set<string> MpsLocalSendType::FGV() const // {{{
{ set<string> result = mySucc->FGV();
  set<string> tmp = myMsgType->FGV();
  result.insert(tmp.begin(),tmp.end());
  return result;
} // }}}
set<string> MpsLocalRcvType::FGV() const // {{{
{ set<string> result = mySucc->FGV();
  set<string> tmp = myMsgType->FGV();
  result.insert(tmp.begin(),tmp.end());
  return result;
} // }}}
set<string> MpsLocalForallType::FGV() const // {{{
{ return mySucc->FGV();
} // }}}
set<string> MpsLocalSelectType::FGV() const // {{{
{
  set<string> result;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    set<string> tmp=it->second->FGV();
    result.insert(tmp.begin(),tmp.end());
  }
  return result;
} // }}}
set<string> MpsLocalBranchType::FGV() const // {{{
{
  set<string> result;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    set<string> tmp=it->second->FGV();
    result.insert(tmp.begin(),tmp.end());
  }
  return result;
} // }}}
set<string> MpsLocalRecType::FGV() const // {{{
{
  set<string> result = mySucc->FGV();
  for (vector<TypeArg>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  { set<string> tmp=it->myType->FGV();
    result.insert(tmp.begin(),tmp.end());
  }
  return result;
} // }}}
set<string> MpsLocalVarType::FGV() const // {{{
{
  return set<string>();
} // }}}
set<string> MpsLocalEndType::FGV() const // {{{
{
  return set<string>();
} // }}}
set<string> MpsLocalSyncType::FGV() const // {{{
{
  set<string> result;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    set<string> tmp=it->second->FGV();
    result.insert(tmp.begin(),tmp.end());
  }
  return result;
} // }}}

// Free Local Type Variables
set<string> MpsLocalSendType::FLV() const // {{{
{ set<string> result = mySucc->FLV();
  set<string> tmp = myMsgType->FLV();
  result.insert(tmp.begin(),tmp.end());
  return result;
} // }}}
set<string> MpsLocalRcvType::FLV() const // {{{
{ set<string> result = mySucc->FLV();
  set<string> tmp = myMsgType->FLV();
  result.insert(tmp.begin(),tmp.end());
  return result;
} // }}}
set<string> MpsLocalForallType::FLV() const // {{{
{ return mySucc->FLV();
} // }}}
set<string> MpsLocalSelectType::FLV() const // {{{
{
  set<string> result;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    set<string> tmp=it->second->FLV();
    result.insert(tmp.begin(),tmp.end());
  }
  return result;
} // }}}
set<string> MpsLocalBranchType::FLV() const // {{{
{
  set<string> result;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    set<string> tmp=it->second->FLV();
    result.insert(tmp.begin(),tmp.end());
  }
  return result;
} // }}}
set<string> MpsLocalRecType::FLV() const // {{{
{
  set<string> result = mySucc->FLV();
  result.erase(myName);
  for (vector<TypeArg>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  { set<string> tmp=it->myType->FLV();
    result.insert(tmp.begin(),tmp.end());
  }
  return result;
} // }}}
set<string> MpsLocalVarType::FLV() const // {{{
{
  set<string> result;
  result.insert(myName);
  return result;
} // }}}
set<string> MpsLocalEndType::FLV() const // {{{
{
  return set<string>();
} // }}}
set<string> MpsLocalSyncType::FLV() const // {{{
{
  set<string> result;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    set<string> tmp=it->second->FLV();
    result.insert(tmp.begin(),tmp.end());
  }
  return result;
} // }}}

// Free Expression Variables
set<string> MpsLocalSendType::FEV() const // {{{
{ set<string> result = mySucc->FEV();
  set<string> tmp = myMsgType->FEV();
  result.insert(tmp.begin(),tmp.end());
  tmp = myAssertion->FV();
  result.insert(tmp.begin(),tmp.end());
  result.erase(myId);
  return result;
} // }}}
set<string> MpsLocalRcvType::FEV() const // {{{
{ set<string> result = mySucc->FEV();
  set<string> tmp = myMsgType->FEV();
  result.insert(tmp.begin(),tmp.end());
  tmp = myAssertion->FV();
  result.insert(tmp.begin(),tmp.end());
  result.erase(myId);
  return result;
} // }}}
set<string> MpsLocalForallType::FEV() const // {{{
{ set<string> result = mySucc->FEV();
  set<string> tmp = myAssertion->FV();
  result.insert(tmp.begin(),tmp.end());
  result.erase(myName);
  return result;
} // }}}
set<string> MpsLocalSelectType::FEV() const // {{{
{
  set<string> result;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    set<string> tmp=it->second->FEV();
    result.insert(tmp.begin(),tmp.end());
  }
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin(); it!=myAssertions.end(); ++it)
  {
    set<string> tmp=it->second->FV();
    result.insert(tmp.begin(),tmp.end());
  }
  return result;
} // }}}
set<string> MpsLocalBranchType::FEV() const // {{{
{
  set<string> result;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    set<string> tmp=it->second->FEV();
    result.insert(tmp.begin(),tmp.end());
  }
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin(); it!=myAssertions.end(); ++it)
  {
    set<string> tmp=it->second->FV();
    result.insert(tmp.begin(),tmp.end());
  }
  return result;
} // }}}
set<string> MpsLocalRecType::FEV() const // {{{
{
  set<string> succFEV = mySucc->FEV();
  set<string> result;
  for (vector<TypeArg>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  { set<string> tmp=it->myType->FEV();
    result.insert(tmp.begin(),tmp.end());
    tmp=it->myValue->FV();
    result.insert(tmp.begin(),tmp.end());
    succFEV.erase(it->myName);
  }
  result.insert(succFEV.begin(),succFEV.end());

  return result;
} // }}}
set<string> MpsLocalVarType::FEV() const // {{{
{
  set<string> result;
  for (vector<MpsExp*>::const_iterator it=myValues.begin(); it!=myValues.end(); ++it)
  { set<string> tmp = (*it)->FV();
    result.insert(tmp.begin(),tmp.end());
  }
  return result;
} // }}}
set<string> MpsLocalEndType::FEV() const // {{{
{
  return set<string>();
} // }}}
set<string> MpsLocalSyncType::FEV() const // {{{
{
  set<string> result;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    set<string> tmp=it->second->FEV();
    result.insert(tmp.begin(),tmp.end());
  }
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin(); it!=myAssertions.end(); ++it)
  {
    set<string> tmp=it->second->FV();
    result.insert(tmp.begin(),tmp.end());
  }
  return result;
} // }}}

// Rename Global Type Variable
MpsLocalSendType *MpsLocalSendType::GRename(const string &from, const string &to) const // {{{
{
  MpsLocalType *newSucc=mySucc->GRename(from,to);
  MpsMsgType *newMsg = myMsgType->GRename(from,to);
  MpsLocalSendType *result=NULL;
  if (myAssertionType)
    result=new MpsLocalSendType(myReceiver, *newMsg, *newSucc, *myAssertion, myId);
  else
    result=new MpsLocalSendType(myReceiver, *newMsg, *newSucc);

  // Clean Up
  delete newSucc;
  delete newMsg;

  return result;
} // }}}
MpsLocalRcvType *MpsLocalRcvType::GRename(const string &from, const string &to) const // {{{
{
  MpsLocalType *newSucc=mySucc->GRename(from,to);
  MpsMsgType *newMsg = myMsgType->GRename(from,to);
  MpsLocalRcvType *result=NULL;
  if (myAssertionType)
    result=new MpsLocalRcvType(mySender, *newMsg, *newSucc, *myAssertion, myId);
  else
    result=new MpsLocalRcvType(mySender, *newMsg, *newSucc);

  // Clean Up
  delete newSucc;
  delete newMsg;
  return result;
} // }}}
MpsLocalForallType *MpsLocalForallType::GRename(const string &from, const string &to) const // {{{
{
  MpsLocalType *newSucc=mySucc->GRename(from,to);
  MpsLocalForallType *result=new MpsLocalForallType(myName, *myAssertion, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsLocalSelectType *MpsLocalSelectType::GRename(const string &from, const string &to) const // {{{
{
  map<string,MpsLocalType*> newBranches;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->GRename(from,to);
  MpsLocalSelectType *result = new MpsLocalSelectType(myReceiver,newBranches,myAssertions);

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsLocalBranchType *MpsLocalBranchType::GRename(const string &from, const string &to) const // {{{
{
  map<string,MpsLocalType*> newBranches;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->GRename(from,to);
  MpsLocalBranchType *result = new MpsLocalBranchType(mySender,newBranches,myAssertions);
  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsLocalRecType *MpsLocalRecType::GRename(const string &from, const string &to) const // {{{
{
  vector<TypeArg> newArgs;
  for (vector<TypeArg>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  {
    MpsMsgType *newType = it->myType->GRename(from, to);
    TypeArg newArg(it->myName, *newType, *it->myValue);
    newArgs.push_back(newArg);
    delete newType;
  }
  MpsLocalType *newSucc = mySucc->GRename(from,to);

  // Create new type
  MpsLocalRecType *result = new MpsLocalRecType(myName,*newSucc,newArgs);

  // Clean Up
  delete newSucc;

  return result;
} // }}}
MpsLocalVarType *MpsLocalVarType::GRename(const string &from, const string &to) const // {{{
{
  return Copy();
} // }}}
MpsLocalEndType *MpsLocalEndType::GRename(const string &from, const string &to) const // {{{
{
  return Copy();
} // }}}
MpsLocalSyncType *MpsLocalSyncType::GRename(const string &from, const string &to) const // {{{
{
  map<string,MpsLocalType*> newBranches;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
    newBranches[it->first] = it->second->GRename(from,to);

  MpsLocalSyncType *result = new MpsLocalSyncType(newBranches,myAssertions);

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}

// Rename Local Type Variable
MpsLocalSendType *MpsLocalSendType::LRename(const string &from, const string &to) const // {{{
{
  MpsLocalType *newSucc=mySucc->LRename(from,to);
  MpsMsgType *newMsg = myMsgType->LRename(from,to);
  MpsLocalSendType *result=NULL;
  if (myAssertionType)
    result=new MpsLocalSendType(myReceiver, *newMsg, *newSucc, *myAssertion, myId);
  else
    result=new MpsLocalSendType(myReceiver, *newMsg, *newSucc);

  // Clean Up
  delete newSucc;
  delete newMsg;

  return result;
} // }}}
MpsLocalRcvType *MpsLocalRcvType::LRename(const string &from, const string &to) const // {{{
{
  MpsLocalType *newSucc=mySucc->LRename(from,to);
  MpsMsgType *newMsg = myMsgType->LRename(from,to);
  MpsLocalRcvType *result=NULL;
  if (myAssertionType)
    result=new MpsLocalRcvType(mySender, *newMsg, *newSucc, *myAssertion, myId);
  else
    result=new MpsLocalRcvType(mySender, *newMsg, *newSucc);

  // Clean Up
  delete newSucc;
  delete newMsg;
  return result;
} // }}}
MpsLocalForallType *MpsLocalForallType::LRename(const string &from, const string &to) const // {{{
{
  MpsLocalType *newSucc=mySucc->LRename(from,to);
  MpsLocalForallType *result=new MpsLocalForallType(myName, *myAssertion, *newSucc);
  // Clean Up
  delete newSucc;
  return result;
} // }}}
MpsLocalSelectType *MpsLocalSelectType::LRename(const string &from, const string &to) const // {{{
{
  map<string,MpsLocalType*> newBranches;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->LRename(from,to);
  MpsLocalSelectType *result = new MpsLocalSelectType(myReceiver,newBranches,myAssertions);
  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsLocalBranchType *MpsLocalBranchType::LRename(const string &from, const string &to) const // {{{
{
  map<string,MpsLocalType*> newBranches;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->LRename(from,to);
  MpsLocalBranchType *result = new MpsLocalBranchType(mySender,newBranches,myAssertions);
  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsLocalRecType *MpsLocalRecType::LRename(const string &from, const string &to) const // {{{
{
  if (from == myName)
    return Copy();
  MpsLocalRecType *result=NULL;
  if (to==myName) // Rename to avoid variable capturing
  {
    string newName=NewLVar();
    MpsLocalType *tmpSucc=mySucc->LRename(myName,newName);
    MpsLocalType *newSucc=tmpSucc->LRename(from,to);
    result = new MpsLocalRecType(newName,*newSucc,myArgs);
    delete tmpSucc;
    delete newSucc;
  }
  else // No rename is necessary
  {
    MpsLocalType *newSucc=mySucc->LRename(from,to);
    result= new MpsLocalRecType(myName,*newSucc,myArgs);
    delete newSucc;
  }
  return result;
} // }}}
MpsLocalVarType *MpsLocalVarType::LRename(const string &from, const string &to) const // {{{
{
  // assert to != myName
  MpsLocalVarType *result = Copy();
  if (from == myName)
    result->myName=to;

  return result;
} // }}}
MpsLocalEndType *MpsLocalEndType::LRename(const string &from, const string &to) const // {{{
{
  return Copy();
} // }}}
MpsLocalSyncType *MpsLocalSyncType::LRename(const string &from, const string &to) const // {{{
{
  map<string,MpsLocalType*> newBranches;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->LRename(from,to);
  MpsLocalSyncType *result = new MpsLocalSyncType(newBranches,myAssertions);
  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}

// Rename Expression Variable
MpsLocalSendType *MpsLocalSendType::ERename(const string &from, const string &to) const // {{{
{
  MpsMsgType *newMsgType = myMsgType->ERename(from,to);

  if (myAssertionType && myId==from)
  { MpsLocalSendType *result = new MpsLocalSendType(myReceiver, *newMsgType, *mySucc, *myAssertion, myId);
    delete newMsgType;
    return result;
  }

  string newId;
  MpsExp *tmpAssertion=NULL;
  MpsLocalType *tmpSucc=NULL;
  if (myAssertionType && myId==to)
  { newId = MpsExp::NewVar(myId);
    tmpAssertion=myAssertion->Rename(myId,newId);
    tmpSucc=mySucc->ERename(myId,newId);
  }
  else
  { newId=myId;
    tmpSucc=mySucc->Copy();
    if (myAssertionType)
      tmpAssertion=myAssertion->Copy();
    else
      tmpAssertion=new MpsBoolVal(true);
  }

  MpsExp *newAssertion = tmpAssertion->Rename(from,to);
  delete tmpAssertion;
  MpsLocalType *newSucc=tmpSucc->ERename(from,to);
  delete tmpSucc;
  MpsLocalSendType *result=NULL;
  if (myAssertionType)
    result=new MpsLocalSendType(myReceiver, *newMsgType, *newSucc, *newAssertion, newId);
  else
    result=new MpsLocalSendType(myReceiver, *newMsgType, *newSucc);

  // Clean Up
  delete newAssertion;
  delete newSucc;
  delete newMsgType;
  return result;
} // }}}
MpsLocalRcvType *MpsLocalRcvType::ERename(const string &from, const string &to) const // {{{
{
  MpsMsgType *newMsgType = myMsgType->ERename(from,to);

  if (myAssertionType && myId==from)
  { MpsLocalRcvType *result = new MpsLocalRcvType(mySender, *newMsgType, *mySucc, *myAssertion, myId);
    delete newMsgType;
    return result;
  }

  string newId;
  MpsExp *tmpAssertion=NULL;
  MpsLocalType *tmpSucc=NULL;
  if (myAssertionType && myId==to)
  { newId = MpsExp::NewVar(myId);
    tmpAssertion=myAssertion->Rename(myId,newId);
    tmpSucc=mySucc->ERename(myId,newId);
  }
  else if (myAssertionType)
  { newId = myId;
    tmpAssertion=myAssertion->Copy();
    tmpSucc=mySucc->Copy();
  }
  else
  { newId=myId;
    tmpAssertion=new MpsBoolVal(true);
    tmpSucc=mySucc->Copy();
  }

  MpsExp *newAssertion = tmpAssertion->Rename(from,to);
  delete tmpAssertion;
  MpsLocalType *newSucc=tmpSucc->ERename(from,to);
  delete tmpSucc;
  MpsLocalRcvType *result=NULL;
  if (myAssertionType)
    result=new MpsLocalRcvType(mySender, *newMsgType, *newSucc, *newAssertion, newId);
  else
    result=new MpsLocalRcvType(mySender, *newMsgType, *newSucc);

  // Clean Up
  delete newAssertion;
  delete newSucc;
  delete newMsgType;
  return result;
} // }}}
MpsLocalForallType *MpsLocalForallType::ERename(const string &from, const string &to) const // {{{
{
  if (myName==from)
    return Copy();

  string newName;
  MpsExp *tmpAssertion=NULL;
  MpsLocalType *tmpSucc=NULL;
  if (myName==to)
  { newName = MpsExp::NewVar(myName);
    tmpAssertion=myAssertion->Rename(myName,newName);
    tmpSucc=mySucc->ERename(myName,newName);
  }
  else
  { newName = myName;
    tmpAssertion=myAssertion->Copy();
    tmpSucc=mySucc->Copy();
  }

  MpsExp *newAssertion = tmpAssertion->Rename(from,to);
  delete tmpAssertion;
  MpsLocalType *newSucc=tmpSucc->ERename(from,to);
  delete tmpSucc;
  MpsLocalForallType *result=new MpsLocalForallType(newName, *newAssertion, *newSucc);
  // Clean Up
  delete newAssertion;
  delete newSucc;
  return result;
} // }}}
MpsLocalSelectType *MpsLocalSelectType::ERename(const string &from, const string &to) const // {{{
{
  map<string,MpsLocalType*> newBranches;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->ERename(from,to);
  map<string,MpsExp*> newAssertions;
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin();it!=myAssertions.end();++it)
    newAssertions[it->first] = it->second->Rename(from,to);
  MpsLocalSelectType *result = new MpsLocalSelectType(myReceiver,newBranches,newAssertions);

  // Clean up
  DeleteMap(newBranches);
  DeleteMap(newAssertions);

  return result;
} // }}}
MpsLocalBranchType *MpsLocalBranchType::ERename(const string &from, const string &to) const // {{{
{
  map<string,MpsLocalType*> newBranches;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->ERename(from,to);
  map<string,MpsExp*> newAssertions;
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin();it!=myAssertions.end();++it)
    newAssertions[it->first] = it->second->Rename(from,to);
  MpsLocalBranchType *result = new MpsLocalBranchType(mySender,newBranches,newAssertions);
  // Clean up
  DeleteMap(newBranches);
  DeleteMap(newAssertions);

  return result;
} // }}}
MpsLocalRecType *MpsLocalRecType::ERename(const string &from, const string &to) const // {{{
{
  bool hidden=false;
  // Rename in arg values
  vector<TypeArg> newArgs;
  for (vector<TypeArg>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  {
    MpsMsgType *newType = it->myType->ERename(from, to);
    MpsExp *newValue = it->myValue->Rename(from, to);
    TypeArg newArg(it->myName, *newType, *newValue);
    newArgs.push_back(newArg);
    delete newType;
    delete newValue;
    if (from == it->myName) // source is hidden in succ
      hidden=true;
  }

  if (hidden) // source is hidden
    return new MpsLocalRecType(myName, *mySucc, newArgs);

  MpsLocalType *newSucc = mySucc->Copy();
  MpsLocalType *tmp = NULL;
  // Avoid variable capture by renaming
  for (vector<TypeArg>::iterator it=newArgs.begin(); it!=newArgs.end(); ++it)
  {
    if (it->myName==to)
    { string newName=MpsExp::NewVar();
      tmp=newSucc->ERename(it->myName,newName);
      delete newSucc;
      newSucc=tmp;
      it->myName=newName;
    }
  }
  // Substitute in body
  tmp = newSucc->ERename(from,to);
  delete newSucc;
  newSucc=tmp;

  // Create new type
  MpsLocalRecType *result = new MpsLocalRecType(myName,*newSucc,newArgs);
  delete newSucc;
  return result;
} // }}}
MpsLocalVarType *MpsLocalVarType::ERename(const string &from, const string &to) const // {{{
{
  vector<MpsExp*> newValues;
  for (vector<MpsExp*>::const_iterator it=myValues.begin(); it!=myValues.end(); ++it)
    newValues.push_back((*it)->Rename(from,to));

  MpsLocalVarType *result = new MpsLocalVarType(myName,newValues);

  // Clean Up
  DeleteVector(newValues);

  return result;
} // }}}
MpsLocalEndType *MpsLocalEndType::ERename(const string &from, const string &to) const // {{{
{
  return Copy();
} // }}}
MpsLocalSyncType *MpsLocalSyncType::ERename(const string &from, const string &to) const // {{{
{
  map<string,MpsLocalType*> newBranches;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
    newBranches[it->first] = it->second->ERename(from,to);

  map<string,MpsExp*> newAssertions;
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin(); it!=myAssertions.end(); ++it)
    newAssertions[it->first] = it->second->Rename(from,to);

  MpsLocalSyncType *result = new MpsLocalSyncType(newBranches,newAssertions);

  // Clean up
  DeleteMap(newBranches);
  DeleteMap(newAssertions);

  return result;
} // }}}

// Global Type Substitution
MpsLocalSendType *MpsLocalSendType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsMsgType *newMsgType = myMsgType->GSubst(source,dest,args);
  MpsLocalType *newSucc = mySucc->GSubst(source,dest,args);
  MpsLocalSendType *result = new MpsLocalSendType(myReceiver,*newMsgType,*newSucc);

  // Clean Up
  delete newSucc;
  delete newMsgType;

  return result;
} // }}}
MpsLocalRcvType *MpsLocalRcvType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsLocalType *newSucc = mySucc->GSubst(source,dest,args);
  MpsMsgType *newMsgType = myMsgType->GSubst(source,dest,args);
  MpsLocalRcvType *result = new MpsLocalRcvType(mySender,*newMsgType,*newSucc);

  // Clean up
  delete newSucc;
  delete newMsgType;

  return result;
} // }}}
MpsLocalForallType *MpsLocalForallType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsLocalType *newSucc = mySucc->GSubst(source,dest,args);
  MpsLocalForallType *result = new MpsLocalForallType(myName, *myAssertion,*newSucc);
  // Clean up
  delete newSucc;
  return result;
} // }}}
MpsLocalSelectType *MpsLocalSelectType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  map<string,MpsLocalType*> newBranches;
  newBranches.clear();
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->GSubst(source,dest,args);
  MpsLocalSelectType *result = new MpsLocalSelectType(myReceiver,newBranches,myAssertions);

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsLocalBranchType *MpsLocalBranchType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  map<string,MpsLocalType*> newBranches;
  newBranches.clear();
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->GSubst(source,dest,args);
  MpsLocalBranchType *result = new MpsLocalBranchType(mySender,newBranches,myAssertions);

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsLocalRecType *MpsLocalRecType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  vector<TypeArg> newArgs;
  for (vector<TypeArg>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  {
    MpsMsgType *newType = it->myType->GSubst(source, dest,args);
    TypeArg newArg(it->myName, *newType, *it->myValue);
    newArgs.push_back(newArg);
    delete newType;
  }
  MpsLocalType *newSucc=mySucc->GSubst(source,dest,args);
  MpsLocalRecType *result= new MpsLocalRecType(myName,*newSucc,newArgs);

  // Clean Up
  delete newSucc;

  return result;
} // }}}
MpsLocalVarType *MpsLocalVarType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  return Copy();
} // }}}
MpsLocalEndType *MpsLocalEndType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  return Copy();
} // }}}
MpsLocalSyncType *MpsLocalSyncType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  map<string,MpsLocalType*> newBranches;
  newBranches.clear();
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->GSubst(source,dest,args);
  MpsLocalSyncType *result = new MpsLocalSyncType(newBranches,myAssertions);

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}

// Local Type Substitution
MpsLocalSendType *MpsLocalSendType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsLocalType *newSucc = mySucc->LSubst(source,dest,args);
  MpsMsgType *newMsgType = myMsgType->LSubst(source,dest,args);
  MpsLocalSendType *result = NULL;
  if (myAssertionType)
    result=new MpsLocalSendType(myReceiver,*newMsgType,*newSucc,*myAssertion,myId);
  else
    result=new MpsLocalSendType(myReceiver,*newMsgType,*newSucc);

  // Clean Up
  delete newSucc;
  delete newMsgType;

  return result;
} // }}}
MpsLocalRcvType *MpsLocalRcvType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsLocalType *newSucc = mySucc->LSubst(source,dest,args);
  MpsMsgType *newMsgType = myMsgType->LSubst(source,dest,args);
  MpsLocalRcvType *result = NULL;
  if (myAssertionType)
    result=new MpsLocalRcvType(mySender,*newMsgType,*newSucc,*myAssertion,myId);
  else
    result=new MpsLocalRcvType(mySender,*newMsgType,*newSucc);

  // Clean up
  delete newSucc;
  delete newMsgType;

  return result;
} // }}}
MpsLocalForallType *MpsLocalForallType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsLocalType *newSucc = mySucc->LSubst(source,dest,args);
  MpsLocalForallType *result=new MpsLocalForallType(myName, *myAssertion,*newSucc);
  // Clean up
  delete newSucc;
  return result;
} // }}}
MpsLocalSelectType *MpsLocalSelectType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  map<string,MpsLocalType*> newBranches;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->LSubst(source,dest,args);
  MpsLocalSelectType *result = new MpsLocalSelectType(myReceiver,newBranches,myAssertions);
  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsLocalBranchType *MpsLocalBranchType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  map<string,MpsLocalType*> newBranches;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->LSubst(source,dest,args);
  MpsLocalBranchType *result = new MpsLocalBranchType(mySender,newBranches,myAssertions);
  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsLocalRecType *MpsLocalRecType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  if (source == myName)
    return Copy();
  MpsLocalRecType *result=NULL;
  set<string> fv = dest.FLV();
  if (fv.find(myName) != fv.end()) // Rename to avoid variable capturing
  {
    string newName=NewLVar();
    MpsLocalType *tmpSucc=mySucc->LRename(myName,newName);
    MpsLocalType *newSucc=tmpSucc->LSubst(source,dest,args);
    result = new MpsLocalRecType(newName,*newSucc,myArgs);
    delete tmpSucc;
    delete newSucc;
  }
  else // No rename is necessary
  {
    MpsLocalType *newSucc=mySucc->LSubst(source,dest,args);
    result= new MpsLocalRecType(myName,*newSucc,myArgs);
    delete newSucc;
  }
  return result;
} // }}}
MpsLocalType *MpsLocalVarType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  // assert to != myName

  if (source != myName)
    return Copy();
  if (args.size()!=myValues.size())
  {
#if APIMS_DEBUG_LEVEL>2
    cerr << "LSubst error: argument and value list have different sizes" << endl;
#endif
    return new MpsLocalVarType("ERROR",vector<MpsExp*>());
  }
  MpsLocalType *result = dest.Copy();
  for (int i=0; i<myValues.size(); ++i)
  { MpsLocalType *tmpType = result->ESubst(args[i],*myValues[i]);
    delete result;
    result=tmpType;
  }
  return result;
} // }}}
MpsLocalEndType *MpsLocalEndType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  return Copy();
} // }}}
MpsLocalSyncType *MpsLocalSyncType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  map<string,MpsLocalType*> newBranches;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->LSubst(source,dest,args);
  MpsLocalSyncType *result = new MpsLocalSyncType(newBranches,myAssertions);
  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}

// Expression Substitution
MpsLocalSendType *MpsLocalSendType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  MpsMsgType *newMsgType = myMsgType->ESubst(source,dest);

  if (myAssertionType && source==myId) // Do not substitute in assertion and succ
  { MpsLocalSendType *result = new MpsLocalSendType(myReceiver, *newMsgType, *mySucc, *myAssertion, myId);
    delete newMsgType;
    return result;
  }

  // Create new assertion and succ
  string newId;
  MpsLocalType *newSucc=NULL;
  MpsExp *newAssertion=NULL;
  set<string> fv=dest.FV();
  if (myAssertionType && fv.find(myId)!=fv.end()) // Rename myId
  { newId=MpsExp::NewVar(myId);
    MpsLocalType *tmpSucc=mySucc->ERename(myId,newId);
    newSucc=tmpSucc->ESubst(source,dest);
    delete tmpSucc;
    MpsExp *tmpAssertion=myAssertion->Rename(myId,newId);
    newAssertion=tmpAssertion->Subst(source,dest);
    delete tmpAssertion;
  }
  else
  { newId=myId;
    newSucc=mySucc->ESubst(source,dest);
    newAssertion=myAssertion->Subst(source,dest);
  }

  // Create result
  MpsLocalSendType *result=NULL;
  if (myAssertionType)
    result=new MpsLocalSendType(myReceiver,*newMsgType,*newSucc,*newAssertion,newId);
  else
    result=new MpsLocalSendType(myReceiver,*newMsgType,*newSucc);
  // Clean Up
  delete newSucc;
  delete newAssertion;
  delete newMsgType;

  return result;
} // }}}
MpsLocalRcvType *MpsLocalRcvType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  MpsMsgType *newMsgType = myMsgType->ESubst(source,dest);

  if (myAssertionType && source==myId) // Do not substitute in assertion and succ
  { MpsLocalRcvType *result = new MpsLocalRcvType(mySender, *newMsgType, *mySucc, *myAssertion, myId);
    delete newMsgType;
    return result;
  }

  // Create new assertion and succ
  string newId;
  MpsLocalType *newSucc=NULL;
  MpsExp *newAssertion=NULL;
  set<string> fv=dest.FV();
  if (myAssertionType && fv.find(myId)!=fv.end()) // Rename myId
  { newId=MpsExp::NewVar(myId);
    MpsLocalType *tmpSucc=mySucc->ERename(myId,newId);
    newSucc=tmpSucc->ESubst(source,dest);
    delete tmpSucc;
    MpsExp *tmpAssertion=myAssertion->Rename(myId,newId);
    newAssertion=tmpAssertion->Subst(source,dest);
    delete tmpAssertion;
  }
  else
  { newId=myId;
    newSucc=mySucc->ESubst(source,dest);
    newAssertion=myAssertion->Subst(source,dest);
  }

  // Create result
  MpsLocalRcvType *result=NULL;
  if (myAssertionType)
    result=new MpsLocalRcvType(mySender,*newMsgType,*newSucc,*newAssertion,newId);
  else
    result=new MpsLocalRcvType(mySender,*newMsgType,*newSucc);
  // Clean Up
  delete newSucc;
  delete newAssertion;
  delete newMsgType;

  return result;
} // }}}
MpsLocalForallType *MpsLocalForallType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  if (source==myName) // Do not substitute in assertion and succ
    return Copy();

  // Create new assertion and succ
  string newName;
  MpsLocalType *newSucc=NULL;
  MpsExp *newAssertion=NULL;
  set<string> fv=dest.FV();
  if (fv.find(myName)!=fv.end()) // Rename myName
  { newName=MpsExp::NewVar(myName);
    MpsLocalType *tmpSucc=mySucc->ERename(myName,newName);
    newSucc=tmpSucc->ESubst(source,dest);
    delete tmpSucc;
    MpsExp *tmpAssertion=myAssertion->Rename(myName,newName);
    newAssertion=tmpAssertion->Subst(source,dest);
    delete tmpAssertion;
  }
  else
  { newName=myName;
    newSucc=mySucc->ESubst(source,dest);
    newAssertion=myAssertion->Subst(source,dest);
  }

  // Create result
  MpsLocalForallType *result=new MpsLocalForallType(newName,*newAssertion,*newSucc);
  // Clean Up
  delete newSucc;
  delete newAssertion;

  return result;
} // }}}
MpsLocalSelectType *MpsLocalSelectType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  map<string,MpsLocalType*> newBranches;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->ESubst(source,dest);
  map<string,MpsExp*> newAssertions;
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin();it!=myAssertions.end();++it)
    newAssertions[it->first] = it->second->Subst(source,dest);
  MpsLocalSelectType *result = new MpsLocalSelectType(myReceiver,newBranches,newAssertions);
  // Clean up
  DeleteMap(newBranches);
  DeleteMap(newAssertions);

  return result;
} // }}}
MpsLocalBranchType *MpsLocalBranchType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  map<string,MpsLocalType*> newBranches;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->ESubst(source,dest);
  map<string,MpsExp*> newAssertions;
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin();it!=myAssertions.end();++it)
    newAssertions[it->first] = it->second->Subst(source,dest);
  MpsLocalBranchType *result = new MpsLocalBranchType(mySender,newBranches,newAssertions);
  // Clean up
  DeleteMap(newBranches);
  DeleteMap(newAssertions);

  return result;
} // }}}
MpsLocalRecType *MpsLocalRecType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  bool hidden=false;
  // Substitute in arg values
  vector<TypeArg> newArgs;
  for (vector<TypeArg>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  {
    MpsMsgType *newType = it->myType->ESubst(source, dest);
    MpsExp *newValue = it->myValue->Subst(source, dest);
    TypeArg newArg(it->myName, *newType, *newValue);
    newArgs.push_back(newArg);
    delete newType;
    delete newValue;
    if (source == it->myName) // source is hidden in succ
      hidden=true;
  }

  if (hidden) // source is hidden
    return new MpsLocalRecType(myName, *mySucc, newArgs);

  MpsLocalType *newSucc = mySucc->Copy();
  MpsLocalType *tmpSucc = NULL;
  // Avoid variable capture by renaming
  set<string> fv = dest.FV();
  for (vector<TypeArg>::iterator it=newArgs.begin(); it!=newArgs.end(); ++it)
  {
    if (fv.find(it->myName)!=fv.end())
    { string newName=MpsExp::NewVar(it->myName);
      tmpSucc=newSucc->ERename(it->myName,newName);
      delete newSucc;
      newSucc=tmpSucc;
      it->myName=newName;
    }
  }
  // Substitute in body
  tmpSucc = newSucc->ESubst(source,dest);
  delete newSucc;
  newSucc=tmpSucc;

  // Create new type
  MpsLocalRecType *result = new MpsLocalRecType(myName,*newSucc,newArgs);

  delete newSucc;

  return result;
} // }}}
MpsLocalVarType *MpsLocalVarType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  vector<MpsExp*> newValues;
  for (vector<MpsExp*>::const_iterator it=myValues.begin(); it!=myValues.end(); ++it)
    newValues.push_back((*it)->Subst(source,dest));
  MpsLocalVarType *result=new MpsLocalVarType(myName,newValues);
  // Clean up
  DeleteVector(newValues);

  return result;
} // }}}
MpsLocalEndType *MpsLocalEndType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  return Copy();
} // }}}
MpsLocalSyncType *MpsLocalSyncType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  map<string,MpsLocalType*> newBranches;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->ESubst(source,dest);
  map<string,MpsExp*> newAssertions;
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin();it!=myAssertions.end();++it)
    newAssertions[it->first] = it->second->Subst(source,dest);
  MpsLocalSyncType *result = new MpsLocalSyncType(newBranches,newAssertions);
  // Clean up
  DeleteMap(newBranches);
  DeleteMap(newAssertions);

  return result;
} // }}}

// Rename all bindings
MpsLocalSendType *MpsLocalSendType::RenameAll() const // {{{
{
  // Create new id for assertion
  string newId=MpsLocalType::NewLVar(myId);
  // Renaming in message type
  MpsMsgType *newMsgType = myMsgType->RenameAll();
  // Renaming in assertion
  MpsExp *newAssertion=myAssertion->Rename(myId,newId);
  // Renaminf in succ
  MpsLocalType *tmpSucc=mySucc->LRename(myId,newId);
  MpsLocalType *newSucc=tmpSucc->RenameAll();
  delete tmpSucc;
  
  // Create result
  MpsLocalSendType *result=NULL;
  if (myAssertionType)
    result = new MpsLocalSendType(myReceiver, *newMsgType, *newSucc, *newAssertion, newId);
  else
    result = new MpsLocalSendType(myReceiver, *newMsgType, *newSucc);

  // Clean Up
  delete newMsgType;
  delete newSucc;
  delete newAssertion;

  return result;
} // }}}
MpsLocalRcvType *MpsLocalRcvType::RenameAll() const // {{{
{
  // Create new id for assertion
  string newId=MpsLocalType::NewLVar(myId);
  // Renaming in message type
  MpsMsgType *newMsgType = myMsgType->RenameAll();
  // Renaming in assertion
  MpsExp *newAssertion=myAssertion->Rename(myId,newId);
  // Renaminf in succ
  MpsLocalType *tmpSucc=mySucc->LRename(myId,newId);
  MpsLocalType *newSucc=tmpSucc->RenameAll();
  delete tmpSucc;
  
  // Create result
  MpsLocalRcvType *result=NULL;
  if (myAssertionType)
    result = new MpsLocalRcvType(mySender, *newMsgType, *newSucc, *newAssertion, newId);
  else
    result = new MpsLocalRcvType(mySender, *newMsgType, *newSucc);

  // Clean Up
  delete newMsgType;
  delete newSucc;
  delete newAssertion;

  return result;
} // }}}
MpsLocalForallType *MpsLocalForallType::RenameAll() const // {{{
{
  // Create new name
  string newName=MpsLocalType::NewLVar(myName);
  // Rename in assertions
  MpsExp *newAssertion = myAssertion->Rename(myName,newName);
  // Renaming in succ
  MpsLocalType *tmpSucc=mySucc->LRename(myName,newName);
  MpsLocalType *newSucc=tmpSucc->RenameAll();
  delete tmpSucc;

  // Create result
  MpsLocalForallType *result=new MpsLocalForallType(newName,*newAssertion,*newSucc);

  // Clean up
  delete newAssertion;
  delete newSucc;

  return result;
} // }}}
MpsLocalSelectType *MpsLocalSelectType::RenameAll() const // {{{
{
  // Create new branches with renaming
  map<string,MpsLocalType*> newBranches;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->RenameAll();
  MpsLocalSelectType *result = new MpsLocalSelectType(myReceiver, newBranches, myAssertions);

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsLocalBranchType *MpsLocalBranchType::RenameAll() const // {{{
{
  // Create new branches with renaming
  map<string,MpsLocalType*> newBranches;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
    newBranches[it->first] = it->second->RenameAll();
  MpsLocalBranchType *result = new MpsLocalBranchType(mySender, newBranches, myAssertions);

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsLocalRecType *MpsLocalRecType::RenameAll() const // {{{
{
  // Create new name
  string newName=MpsLocalType::NewLVar(myName);
  // Create list of renames arguments (with renamed types)
  vector<TypeArg> newArgs;
  for (vector<TypeArg>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  { MpsMsgType *newType=it->myType->RenameAll();
    newArgs.push_back(TypeArg(MpsLocalType::NewLVar(it->myName),*newType, *it->myValue));
    delete newType;
  }

  MpsLocalType *newSucc=mySucc->LRename(myName,newName);

  for (vector<TypeArg>::iterator it=newArgs.begin(); it!=newArgs.end(); ++it)
  { string newName = MpsExp::NewVar(it->myName);
    MpsLocalType *tmpSucc = newSucc->ERename(it->myName,newName);
    delete newSucc;
    newSucc=tmpSucc;
    it->myName=newName;
  }
  MpsLocalType *tmpSucc=newSucc->RenameAll();
  delete newSucc;
  newSucc=tmpSucc;

  MpsLocalRecType *result= new MpsLocalRecType(myName,*newSucc,newArgs);

  // Clean Up
  delete newSucc;

  return result;
} // }}}
MpsLocalVarType *MpsLocalVarType::RenameAll() const // {{{
{
  return Copy();
} // }}}
MpsLocalEndType *MpsLocalEndType::RenameAll() const // {{{
{
  return Copy();
} // }}}
MpsLocalSyncType *MpsLocalSyncType::RenameAll() const // {{{
{
  return Copy();
} // }}}

// Make parsable string representation
string MpsLocalSendType::ToString(const string &indent) const // {{{
{
  string newIndent = indent + "  ";
  string result = int2string(myReceiver) + " << <" + myMsgType->ToString(newIndent) + ">";
  if (myAssertionType)
    result += (string)" as " + myId + "[[" + myAssertion->ToString() + "]]";
  result += (string)";\n"
          + indent + mySucc->ToString(indent);
  return result;
} // }}}
string MpsLocalRcvType::ToString(const string &indent) const // {{{
{
  string newIndent = indent + "  ";
  string result = int2string(mySender) + " >> <" + myMsgType->ToString(newIndent) + ">";
  if (myAssertionType)
    result += (string)" as " + myId + "[[" + myAssertion->ToString() + "]]";
  result += (string)"\n"
          + indent + mySucc->ToString(indent);
  return result;
} // }}}
string MpsLocalForallType::ToString(const string &indent) const // {{{
{
  string result = (string)"forall " + myName + " [[" +myAssertion->ToString() + "]];\n"
            + indent + mySucc->ToString(indent);
  return result;
} // }}}
string MpsLocalSelectType::ToString(const string &indent) const // {{{
{
  string newIndent = indent + "  ";
  string result = int2string(myReceiver) + "<<\n"
                + indent + "{ ";
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
  {
    if (it != myBranches.begin())
      result += ",\n" + newIndent;

    result += it->first;
    // Print Assertion
    map<string,MpsExp*>::const_iterator ass = myAssertions.find(it->first);
    if (ass != myAssertions.end())
      result += (string)"[[" + ass->second->ToString() + "]]";
    else
    {
#if APIMS_DEBUG_LEVEL>99
      cerr << "ERROR: Missing assertion for breanch " << it->first << endl;
#endif
    }
    result += (string)":\n" + newIndent + it->second->ToString(newIndent);
  }
  result += "\n" + indent + "}";

  return result;
} // }}}
string MpsLocalBranchType::ToString(const string &indent) const // {{{
{
  string newIndent = indent + "  ";
  string result = int2string(mySender) + ">>\n"
                + indent + "{ ";
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();
       it!=myBranches.end();
       ++it)
  {
    if (it != myBranches.begin())
      result += ",\n" + newIndent;

    // Print Label
    result += it->first;
    // Print Assertion
    map<string,MpsExp*>::const_iterator ass = myAssertions.find(it->first);
    if (ass != myAssertions.end())
      result += (string)"[[" + ass->second->ToString() + "]]";
    else
    {
#if APIMS_DEBUG_LEVEL>99
      cerr << "ERROR: Missing assertion for breanch " << it->first << endl;
#endif
    }
    result += (string)":\n"
            + newIndent + it->second->ToString(newIndent);
  }
  result += "\n" + indent + "}";

  return result;
} // }}}
string MpsLocalRecType::ToString(const string &indent) const // {{{
{
  string result="rec " + myName;
  if (myArgs.size()>0)
  { // Declare all state variables
    result += "<";
    for (int i=0; i<myArgs.size(); ++i)
    { // Insert seperator
      if (i>0)
        result+=", ";
      // Declare NAME:TYPE=VALUE
      result += myArgs[i].myName + ":"
              + myArgs[i].myType->ToString() + "="
              + myArgs[i].myValue->ToString();
    }
    result += ">";
  }
  result +=  ".\n" + indent + mySucc->ToString(indent);
  return result;
} // }}}
string MpsLocalVarType::ToString(const string &indent) const // {{{
{
  string result=myName;
  if (myValues.size()>0)
  { // Declare all state variables
    result += "<";
    for (int i=0; i<myValues.size(); ++i)
    { // Insert seperator
      if (i>0)
        result+=", ";
      // Add value
      result += myValues[i]->ToString();
    }
    result += ">";
  }

  return result;
} // }}}
string MpsLocalEndType::ToString(const string &indent) const // {{{
{
  string result="Lend";
  return result;
} // }}}
string MpsLocalSyncType::ToString(const string &indent) const // {{{
{
  string newIndent = indent + "    ";
  string result = "{ ";
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
  {
    if (it != myBranches.begin())
      result += ",\n" + indent + "  ";

    // Print label
    result += it->first;
    // Print assertion if any
    map<string,MpsExp*>::const_iterator assertion = myAssertions.find(it->first);
    if (assertion != myAssertions.end())
      result += + "[[" + assertion->second->ToString() + "]]";
    // Print branch
    result += ":\n"
            + newIndent + it->second->ToString(newIndent);
  }
  result += "\n" + indent + "}";

  return result;
} // }}}

// Make string representation with Latex markup
string MpsLocalSendType::ToTex(int indent, int sw) const // {{{
{
  int newIndent = indent + 2;
  string result = ToTex_SID(myReceiver) + "$\\ll\\langle$" + myMsgType->ToTex(newIndent,sw) + "$\\rangle$";
  if (myAssertionType)
    result += (string)" as " + myId + "$\\llbracket$" + myAssertion->ToString() + "$\\rrbracket$";
  result += (string)";\\newline\n"
          + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
  return result;
} // }}}
string MpsLocalRcvType::ToTex(int indent, int sw) const // {{{
{
  int newIndent = indent + 2;
  string result = ToTex_SID(mySender) + "$\\gg\\langle$" + myMsgType->ToTex(newIndent,sw) + "$\\rangle$";
  if (myAssertionType)
    result += (string)" as " + myId + "$\\llbracket$" + myAssertion->ToString() + "$\\rrbracket$";
  result += (string)";\\newline\n"
          + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
  return result;
} // }}}
string MpsLocalForallType::ToTex(int indent, int sw) const // {{{
{
  string result = (string)"$\\forall$ " + myName + " $\\llbracket$" +myAssertion->ToString() + "$\\rrbracket$;\n"
                + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
  return result;
} // }}}
string MpsLocalSelectType::ToTex(int indent, int sw) const // {{{
{
  int lblIndent = indent + 2;
  int newIndent = indent + 4;
  string result = ToTex_SID(myReceiver) + "$\\ll$\\newline\n"
                + ToTex_Hspace(indent,sw) + "\\{ ";
  for (map<string,MpsLocalType*>::const_iterator branch=myBranches.begin(); branch!=myBranches.end(); ++branch)
  { if (branch!=myBranches.begin())
      result += ",\\newline\n"
              + ToTex_Hspace(lblIndent,sw);
    result += ToTex_Label(branch->first);
    map<string,MpsExp*>::const_iterator assertion=myAssertions.find(branch->first);
    if (assertion!=myAssertions.end() && assertion->second->ToString()!="true")
      result += "$\\llbracket$" + assertion->second->ToString() + "$\\rrbracket$";
    result += ":\\newline\n"
            + ToTex_Hspace(newIndent,sw) + branch->second->ToTex(newIndent,sw);
  }
  result += "\\newline\n"
          + ToTex_Hspace(indent,sw) + "\\}";
  return result;
} // }}}
string MpsLocalBranchType::ToTex(int indent, int sw) const // {{{
{
  int lblIndent = indent + 2;
  int newIndent = indent + 4;
  string result = ToTex_SID(mySender) + "$\\gg$\\newline\n"
                + ToTex_Hspace(indent,sw) + "\\{ ";
  for (map<string,MpsLocalType*>::const_iterator branch=myBranches.begin(); branch!=myBranches.end(); ++branch)
  { if (branch!=myBranches.begin())
      result += ",\\newline\n"
              + ToTex_Hspace(lblIndent,sw);
    result += ToTex_Label(branch->first);
    map<string,MpsExp*>::const_iterator assertion=myAssertions.find(branch->first);
    if (assertion!=myAssertions.end() && assertion->second->ToString()!="true")
      result += "$\\llbracket$" + assertion->second->ToString() + "$\\rrbracket$";
    result += ":\\newline\n"
            + ToTex_Hspace(newIndent,sw) + branch->second->ToTex(newIndent,sw);
  }
  result += "\\newline\n"
          + ToTex_Hspace(indent,sw) + "\\}";
  return result;
} // }}}
string MpsLocalRecType::ToTex(int indent, int sw) const // {{{
{
  string result= ToTex_KW("rec") + " " + ToTex_Var(myName);
  if (myArgs.size()>0)
  { // Declare all state variables
    result += "$\\langle$";
    for (int i=0; i<myArgs.size(); ++i)
    { // Insert seperator
      if (i>0)
        result+=", ";
      // Declare NAME:TYPE=VALUE
      result += ToTex_Var(myArgs[i].myName) + ":"
              + myArgs[i].myType->ToTex(indent+4,sw) + "="
              + myArgs[i].myValue->ToString();
    }
    result += "$\\rangle$";
  }
  result +=  ".\\newline\n"
          + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
  return result;
} // }}}
string MpsLocalVarType::ToTex(int indent, int sw) const // {{{
{
  string result=ToTex_Var(myName);
  if (myValues.size()>0)
  { // Declare all state variables
    result += "$\\langle$";
    for (int i=0; i<myValues.size(); ++i)
    { // Insert seperator
      if (i>0)
        result+=", ";
      // Add value
      result += myValues[i]->ToString();
    }
    result += "$\\rangle$";
  }

  return result;
} // }}}
string MpsLocalEndType::ToTex(int indent, int sw) const // {{{
{
  string result=ToTex_KW("end");
  return result;
} // }}}
string MpsLocalSyncType::ToTex(int indent, int sw) const // {{{
{
  int lblIndent = indent + 2;
  int newIndent = indent + 4;
  string result = "\\{ ";
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
  {
    if (it != myBranches.begin())
      result += ",\n" + ToTex_Hspace(lblIndent,sw);

    // Print label
    result += ToTex_Label(it->first);
    // Print assertion if any
    map<string,MpsExp*>::const_iterator assertion = myAssertions.find(it->first);
    if (assertion != myAssertions.end())
      result += + "$\\llbracket$" + assertion->second->ToString() + "$\\rrbracket$";
    // Print branch
    result += ":\\newline\n"
            + ToTex_Hspace(newIndent,sw) + it->second->ToTex(newIndent,sw);
  }
  result += "\\newline\n"
          + ToTex_Hspace(indent,sw) + "\\}";
  return result;
} // }}}

// Merge two types to find greatest common subtype
MpsLocalType *MERGE_ERROR(const MpsLocalType &lhs, const MpsLocalType &rhs, string msg) // {{{
{
#if APIMS_DEBUG_LEVEL>2
  cerr << "!!!!MERGE ERROR: " << msg << endl
       << "!!!!!!!!!!!!LHS: " << lhs.ToString("!!!!!!!!!!!!LHS: ") << endl
       << "!!!!!!!!!!!!RHS: " << rhs.ToString("!!!!!!!!!!!!RHS: ") << endl;
#endif
  vector<MpsExp*> values;
  values.clear();
  values.push_back(new MpsStringVal(msg));
  return new MpsLocalVarType(MpsLocalType::NewLVar("ERROR"),values);
} // }}}
MpsExp *MERGE_ERROR_EXP(const MpsLocalType &lhs, const MpsLocalType &rhs, string msg) // {{{
{
#if APIMS_DEBUG_LEVEL>2
  cerr << "!!!!MERGE ERROR: " << msg << endl
       << "!!!!!!!!!!!!LHS: " << lhs.ToString("!!!!!!!!!!!!LHS: ") << endl
       << "!!!!!!!!!!!!RHS: " << rhs.ToString("!!!!!!!!!!!!RHS: ") << endl;
#endif
  return new MpsVarExp(MpsExp::NewVar("ERROR"));
} // }}}
MpsLocalType *MpsLocalSendType::Merge(MpsLocalType &rhs) const // {{{
{
  // Check same Construct, channel, message-type and assertiontype
  MpsLocalSendType *rhsptr = dynamic_cast<MpsLocalSendType*>(&rhs);
  if (rhsptr==NULL ||
      myReceiver != rhsptr->myReceiver ||
      not myMsgType->Equal(MpsBoolVal(true),*rhsptr->myMsgType) ||
      myAssertionType != rhsptr->myAssertionType)
    return MERGE_ERROR(*this,rhs,"MergeError");

  string newId;
  MpsExp *lhsAssertion;
  MpsExp *rhsAssertion;
  MpsLocalType *lhsSucc;
  MpsLocalType *rhsSucc;
  if (myAssertionType && myId != rhsptr->myId) // Rename
  { newId=MpsExp::NewVar(myId);
    lhsAssertion=myAssertion->Rename(myId,newId);
    rhsAssertion=rhsptr->myAssertion->Rename(rhsptr->myId,newId);
    lhsSucc=mySucc->ERename(myId,newId);
    rhsSucc=rhsptr->mySucc->ERename(rhsptr->myId,newId);
  }
  else if (myAssertionType)
  { newId=myId;
    lhsAssertion=myAssertion->Copy();
    rhsAssertion=rhsptr->myAssertion->Copy();
    lhsSucc=mySucc->Copy();
    rhsSucc=rhsptr->mySucc->Copy();
  }
  else
  { newId="";
    lhsAssertion=new MpsBoolVal(true);
    rhsAssertion=new MpsBoolVal(true);
    lhsSucc=mySucc->Copy();
    rhsSucc=rhsptr->mySucc->Copy();
  }

  MpsLocalType *result = NULL;
  if (not (lhsAssertion == rhsAssertion))
    return MERGE_ERROR(*this,rhs,"MergeError");
  else
  { MpsLocalType *succ = mySucc->Merge(*rhsptr->mySucc);
    if (myAssertionType)
      result = new MpsLocalSendType(myReceiver,*myMsgType,*succ, *lhsAssertion, newId);
    else
      result = new MpsLocalSendType(myReceiver,*myMsgType,*succ);
    delete succ;
  }
  // Clean Up
  delete lhsAssertion;
  delete rhsAssertion;
  delete lhsSucc;
  delete rhsSucc;
  return result;
} // }}}
MpsLocalType *MpsLocalRcvType::Merge(MpsLocalType &rhs) const // {{{
{
  // Check same Construct, channel, message-type and assertiontype
  MpsLocalRcvType *rhsptr = dynamic_cast<MpsLocalRcvType*>(&rhs);
  if (rhsptr==NULL ||
      mySender != rhsptr->mySender ||
      not myMsgType->Equal(MpsBoolVal(true),*rhsptr->myMsgType) ||
      myAssertionType != rhsptr->myAssertionType)
    return MERGE_ERROR(*this,rhs,"MergeError");

  string newId;
  MpsExp *lhsAssertion;
  MpsExp *rhsAssertion;
  MpsLocalType *lhsSucc;
  MpsLocalType *rhsSucc;
  if (myAssertionType && myId != rhsptr->myId) // Rename
  { newId=MpsExp::NewVar(myId);
    lhsAssertion=myAssertion->Rename(myId,newId);
    rhsAssertion=rhsptr->myAssertion->Rename(rhsptr->myId,newId);
    lhsSucc=mySucc->ERename(myId,newId);
    rhsSucc=rhsptr->mySucc->ERename(rhsptr->myId,newId);
  }
  else if (myAssertionType)
  { newId=myId;
    lhsAssertion=myAssertion->Copy();
    rhsAssertion=rhsptr->myAssertion->Copy();
    lhsSucc=mySucc->Copy();
    rhsSucc=rhsptr->mySucc->Copy();
  }
  else
  { newId="";
    lhsAssertion=new MpsBoolVal(true);
    rhsAssertion=new MpsBoolVal(true);
    lhsSucc=mySucc->Copy();
    rhsSucc=rhsptr->mySucc->Copy();
  }

  MpsLocalType *result = NULL;
  if (not (lhsAssertion == rhsAssertion))
    result = MERGE_ERROR(*this,rhs,"MergeError");
  else
  { MpsLocalType *succ = mySucc->Merge(*rhsptr->mySucc);
    if (myAssertionType)
      result = new MpsLocalRcvType(mySender,*myMsgType,*succ,*lhsAssertion,newId);
    else
      result = new MpsLocalRcvType(mySender,*myMsgType,*succ);
    delete succ;
  }
  // Clean Up
  delete lhsAssertion;
  delete rhsAssertion;
  delete lhsSucc;
  delete rhsSucc;
  return result;
} // }}}
MpsLocalType *MpsLocalForallType::Merge(MpsLocalType &rhs) const // {{{
{
  MpsLocalForallType *rhsptr = dynamic_cast<MpsLocalForallType*>(&rhs);
  if (rhsptr==NULL)
    return MERGE_ERROR(*this,rhs,"MergeError");
  string newName;
  MpsExp *lhsAssertion;
  MpsExp *rhsAssertion;
  MpsLocalType *lhsSucc;
  MpsLocalType *rhsSucc;
  if (myName != rhsptr->myName) // Rename
  { newName=MpsExp::NewVar(myName);
    lhsAssertion=myAssertion->Rename(myName,newName);
    rhsAssertion=rhsptr->myAssertion->Rename(rhsptr->myName,newName);
    lhsSucc=mySucc->ERename(myName,newName);
    rhsSucc=rhsptr->mySucc->ERename(rhsptr->myName,newName);
  }
  else
  { newName=myName;
    lhsAssertion=myAssertion->Copy();
    rhsAssertion=rhsptr->myAssertion->Copy();
    lhsSucc=mySucc->Copy();
    rhsSucc=rhsptr->mySucc->Copy();
  }

  MpsLocalType *result = NULL;
  if (not (lhsAssertion == rhsAssertion))
    result = MERGE_ERROR(*this,rhs,"MergeError");
  else
  {
    MpsLocalType *succ = lhsSucc->Merge(*rhsSucc);
    result = new MpsLocalForallType(newName,*lhsAssertion,*succ);
    delete succ;
  }
  // Clean Up
  delete lhsAssertion;
  delete rhsAssertion;
  delete lhsSucc;
  delete rhsSucc;
  return result;
} // }}}
MpsLocalType *MpsLocalSelectType::Merge(MpsLocalType &rhs) const // {{{
{
  if (typeid(rhs) != typeid(MpsLocalSelectType))
    return MERGE_ERROR(*this,rhs,"MergeError");
  MpsLocalSelectType *rhsptr = (MpsLocalSelectType*)&rhs;
  if (myReceiver != rhsptr->myReceiver)
    return MERGE_ERROR(*this,rhs,"MergeError");
  map<string,MpsLocalType*> branches;
  branches.clear();
  map<string,MpsExp*> assertions;
  assertions.clear();
  // Insert common branches
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
  {
    map<string,MpsLocalType*>::const_iterator it2=rhsptr->myBranches.find(it->first);
    if (it2 != rhsptr->myBranches.end())
      branches[it->first] = it->second->Merge(*it2->second);
    map<string,MpsExp*>::const_iterator as1=myAssertions.find(it->first);
    map<string,MpsExp*>::const_iterator as2=rhsptr->myAssertions.find(it->first);
    if (as1==myAssertions.end() && as2==rhsptr->myAssertions.end())
      assertions[it->first] = new MpsBoolVal(true);
    else if (as1!=myAssertions.end() && as2!=rhsptr->myAssertions.end() && (*as1->second)==(*as2->second))
      assertions[as1->first] = as1->second->Copy();
    else
      assertions[it->first] = MERGE_ERROR_EXP(*this,rhs,"MergeError");
  }
  MpsLocalSelectType *result = new MpsLocalSelectType(myReceiver,branches,assertions);

  // Clean up
  DeleteMap(branches);
  DeleteMap(assertions);

  return result;
} // }}}
MpsLocalType *MpsLocalBranchType::Merge(MpsLocalType &rhs) const // {{{
{
  if (typeid(rhs) != typeid(MpsLocalBranchType))
    return MERGE_ERROR(*this,rhs,"MergeError");
  MpsLocalBranchType *rhsptr = (MpsLocalBranchType*)&rhs;
  if (mySender != rhsptr->mySender)
    return MERGE_ERROR(*this,rhs,"MergeError");

  map<string,MpsLocalType*> branches;
  branches.clear();
  map<string,MpsExp*> assertions;
  assertions.clear();
  // Insert all branches from this
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
  {
    map<string,MpsLocalType*>::const_iterator it2=rhsptr->myBranches.find(it->first); // find branch in rhs
    if (it2 != rhsptr->myBranches.end()) // common branch
    {
      branches[it->first] = it->second->Merge(*it2->second);
      map<string,MpsExp*>::const_iterator ass1=myAssertions.find(it->first);
      map<string,MpsExp*>::const_iterator ass2=rhsptr->myAssertions.find(it->first);
      if (ass1 == myAssertions.end()) // LHS: No assertion given for branch
        if (ass2 != rhsptr->myAssertions.end()) // RHS: Assertion given for branch
          assertions[it->first] = MERGE_ERROR_EXP(*this,rhs,"MergeError");
      else // LHS: Assertion given for branch
        if (ass2 == rhsptr->myAssertions.end()) // RHS: No assertion given for branch
          assertions[it->first] = MERGE_ERROR_EXP(*this,rhs,"MergeError");
        else if (ass1->second == ass2->second) // Assertion on LHS and RHS is the same
          assertions[it->first] = ass1->second->Copy();
        else
          assertions[it->first] = MERGE_ERROR_EXP(*this,rhs,"MergeError");
    }
    else
    {
      branches[it->first] = it->second->Copy();
      map<string,MpsExp*>::const_iterator ass1=myAssertions.find(it->first);
      if (ass1!=myAssertions.end())
        assertions[ass1->first] = ass1->second->Copy();
    }
  }
  // Insert branches from rhs not in this
  for (map<string,MpsLocalType*>::const_iterator it2=rhsptr->myBranches.begin();it2!=rhsptr->myBranches.end();++it2)
  {
    map<string,MpsLocalType*>::const_iterator it=myBranches.find(it2->first); // find branch in this
    if (it == myBranches.end()) // not common branch
    {
      branches[it2->first] = it2->second->Copy();
      map<string,MpsExp*>::const_iterator ass2=rhsptr->myAssertions.find(it2->first);
      if (ass2!=rhsptr->myAssertions.end())
        assertions[ass2->first] = ass2->second->Copy();
    }
  }
  MpsLocalBranchType *result = new MpsLocalBranchType(mySender,branches,assertions);

  // Clean up
  DeleteMap(branches);
  DeleteMap(assertions);

  return result;
} // }}}
MpsLocalType *MpsLocalRecType::Merge(MpsLocalType &rhs) const // {{{
{
  if (typeid(rhs) != typeid(MpsLocalRecType))
    return MERGE_ERROR(*this,rhs,"MergeError");
  // FIXME: Check same args and vals
  MpsLocalRecType *rhsptr = (MpsLocalRecType*)&rhs;
  string newName = NewLVar();
  MpsLocalType *lhssucc = mySucc->LRename(myName,newName);
  MpsLocalType *rhssucc = rhsptr->mySucc->LRename(rhsptr->myName,newName);

  MpsLocalType *succ = lhssucc->Merge(*rhssucc);

  MpsLocalRecType *result = new MpsLocalRecType(newName,*succ,myArgs);
  // Clean up
  delete succ;
  delete lhssucc;
  delete rhssucc;
  
  return result;
} // }}}
MpsLocalType *MpsLocalVarType::Merge(MpsLocalType &rhs) const // {{{
{
  if (typeid(rhs) != typeid(MpsLocalVarType))
    return MERGE_ERROR(*this,rhs,"MergeError");
  MpsLocalVarType *rhsptr = (MpsLocalVarType*)&rhs;
  if (myName != rhsptr->myName)
    return MERGE_ERROR(*this,rhs,"MergeError");
  // FIXME: Check same or equivalent args
  return Copy();
} // }}}
MpsLocalType *MpsLocalEndType::Merge(MpsLocalType &rhs) const // {{{
{
  const MpsLocalEndType *rhsptr=dynamic_cast<const MpsLocalEndType*>(&rhs);
  if (rhsptr==NULL)
    return MERGE_ERROR(*this,rhs,"MergeError");
  return Copy();
} // }}}
MpsLocalType *MpsLocalSyncType::Merge(MpsLocalType &rhs) const // {{{
{
  if (typeid(rhs) != typeid(MpsLocalSyncType))
    return MERGE_ERROR(*this,rhs,"MergeError");
  MpsLocalSyncType *rhsptr = (MpsLocalSyncType*)&rhs;
  // Check mandatory labels are the same
  bool same_mandatory = true;
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();same_mandatory && it!=myBranches.end();++it)
  {
    if (it->first[0] == '^') // Is the branch mandatory
    {
      map<string,MpsLocalType*>::const_iterator it2=rhsptr->myBranches.find(it->first); // find branch in rhs
      if (it2 == rhsptr->myBranches.end()) // not common branch
        same_mandatory = false;
    }
  }
  for (map<string,MpsLocalType*>::const_iterator it2=rhsptr->myBranches.begin();same_mandatory && it2!=rhsptr->myBranches.end();++it2)
  {
    if (it2->first[0] == '^') // Is the branch mandatory
    {
      map<string,MpsLocalType*>::const_iterator it=myBranches.find(it2->first); // find branch in lhs
      if (it == rhsptr->myBranches.end()) // not common branch
        same_mandatory = false;
    }
  }
  if (not same_mandatory)
    return MERGE_ERROR(*this,rhs,"MergeError");
  // Create type
  map<string,MpsLocalType*> branches;
  branches.clear();
  map<string,MpsExp*> assertions;
  assertions.clear();
  // Insert common branches
  for (map<string,MpsLocalType*>::const_iterator it=myBranches.begin();it!=myBranches.end();++it)
  {
    map<string,MpsLocalType*>::const_iterator it2=rhsptr->myBranches.find(it->first); // find branch in rhs
    if (it2 != rhsptr->myBranches.end()) // common branch
    {	    
      branches[it->first] = it->second->Merge(*it2->second);
      map<string,MpsExp*>::const_iterator ass1=myAssertions.find(it->first);
      map<string,MpsExp*>::const_iterator ass2=rhsptr->myAssertions.find(it->first);
      if (ass1 != myAssertions.end())
      {
	if (ass2 == rhsptr->myAssertions.end())
	  assertions[it->first] = MERGE_ERROR_EXP(*this,rhs,"Merge Error");
      }
      else
      {
	if (ass2 == rhsptr->myAssertions.end())
	  assertions[it->first] = MERGE_ERROR_EXP(*this,rhs,"Merge Error");
	else
	  if ((*ass1->second) == (*ass2->second))
	    assertions[it->first] = ass1->second->Copy();
	  else
	    assertions[it->first] = MERGE_ERROR_EXP(*this,rhs,"Merge Error");
      }
    }
  }
  MpsLocalSyncType *result = new MpsLocalSyncType(branches,assertions);

  // Clean up
  DeleteMap(branches);
  DeleteMap(assertions);

  return result;
} // }}}

// Individual accesors
const string &MpsLocalForallType::GetName() const // {{{
{
  return myName;
} // }}}
const string &MpsLocalRecType::GetName() const // {{{
{
  return myName;
} // }}}
int MpsLocalSendType::GetReceiver() // {{{
{
  return myReceiver;
} // }}}
int MpsLocalRcvType::GetSender() // {{{
{
  return mySender;
} // }}}
int MpsLocalSelectType::GetReceiver() // {{{
{
  return myReceiver;
} // }}}
int MpsLocalBranchType::GetSender() // {{{
{
  return mySender;
} // }}}
const MpsLocalType *MpsLocalSendType::GetSucc() const // {{{
{
  return mySucc;
} // }}}
const MpsLocalType *MpsLocalRcvType::GetSucc() const // {{{
{
  return mySucc;
} // }}}
const MpsLocalType *MpsLocalForallType::GetSucc() const // {{{
{
  return mySucc;
} // }}}
const MpsLocalType *MpsLocalRecType::GetSucc() const // {{{
{
  return mySucc;
} // }}}
map<string,MpsLocalType*> &MpsLocalSelectType::GetBranches() // {{{
{
  return myBranches;
} // }}}
map<string,MpsLocalType*> &MpsLocalBranchType::GetBranches() // {{{
{
  return myBranches;
} // }}}
map<string,MpsLocalType*> &MpsLocalSyncType::GetBranches() // {{{
{
  return myBranches;
} // }}}
bool MpsLocalSendType::GetAssertionType() const // {{{
{ return myAssertionType;
} //}}}
bool MpsLocalRcvType::GetAssertionType() const // {{{
{ return myAssertionType;
} //}}}
const string &MpsLocalSendType::GetAssertionName() const // {{{
{ return myId;
} //}}}
const string &MpsLocalRcvType::GetAssertionName() const // {{{
{ return myId;
} //}}}
const MpsExp &MpsLocalSendType::GetAssertion() const // {{{
{ return *myAssertion;
} //}}}
const MpsExp &MpsLocalRcvType::GetAssertion() const // {{{
{ return *myAssertion;
} //}}}
const MpsExp &MpsLocalForallType::GetAssertion() const // {{{
{ return *myAssertion;
} //}}}
map<string,MpsExp*> &MpsLocalSelectType::GetAssertions() // {{{
{
  return myAssertions;
} // }}}
map<string,MpsExp*> &MpsLocalBranchType::GetAssertions() // {{{
{
  return myAssertions;
} // }}}
map<string,MpsExp*> &MpsLocalSyncType::GetAssertions() // {{{
{
  return myAssertions;
} // }}}
MpsMsgType *MpsLocalSendType::GetMsgType() // {{{
{
  return myMsgType;
} // }}}
MpsMsgType *MpsLocalRcvType::GetMsgType() // {{{
{
  return myMsgType;
} // }}}
const vector<TypeArg> &MpsLocalRecType::GetArgs() const // {{{
{
  return myArgs;
} // }}}

/* MpsMsgType implementation
 *
 */

// Constructors
MpsMsgNoType::MpsMsgNoType() // {{{
{
} // }}}
MpsIntMsgType::MpsIntMsgType() // {{{
{
} // }}}
MpsStringMsgType::MpsStringMsgType() // {{{
{
} // }}}
MpsBoolMsgType::MpsBoolMsgType() // {{{
{
} // }}}
MpsTupleMsgType::MpsTupleMsgType(const vector<MpsMsgType*> &elements) // {{{
{
  myElements.clear();
  for (vector<MpsMsgType*>::const_iterator it=elements.begin(); it!=elements.end(); ++it)
    myElements.push_back((*it)->Copy());
} // }}}
MpsChannelMsgType::MpsChannelMsgType(const MpsGlobalType &type) // {{{
{
  myType = type.Copy();
} // }}}
MpsDelegateMsgType::MpsDelegateMsgType(int pid, int maxpid) // {{{
{
  myPid = pid;
  myMaxpid = maxpid;
} // }}}
MpsDelegateLocalMsgType::MpsDelegateLocalMsgType(const MpsLocalType &type, int pid, int maxpid) // {{{
: MpsDelegateMsgType(pid,maxpid)
{
  myType=type.Copy();
} // }}}
MpsDelegateGlobalMsgType::MpsDelegateGlobalMsgType(const MpsGlobalType &type, int pid, int maxpid) // {{{
: MpsDelegateMsgType(pid,maxpid)
{
  myGlobalType=type.Copy();
  myLocalType=myGlobalType->Project(GetPid());
} // }}}

// Destructors
MpsMsgType::~MpsMsgType() // {{{
{
} // }}}
MpsMsgNoType::~MpsMsgNoType() // {{{
{
} // }}}
MpsIntMsgType::~MpsIntMsgType() // {{{
{
} // }}}
MpsStringMsgType::~MpsStringMsgType() // {{{
{
} // }}}
MpsBoolMsgType::~MpsBoolMsgType() // {{{
{
} // }}}
MpsTupleMsgType::~MpsTupleMsgType() // {{{
{
  DeleteVector(myElements);
} // }}}
MpsChannelMsgType::~MpsChannelMsgType() // {{{
{
  delete myType;
} // }}}
MpsDelegateMsgType::~MpsDelegateMsgType() // {{{
{
} // }}}
MpsDelegateLocalMsgType::~MpsDelegateLocalMsgType() // {{{
{
  delete myType;
} // }}}
MpsDelegateGlobalMsgType::~MpsDelegateGlobalMsgType() // {{{
{
  delete myGlobalType;
  delete myLocalType;
} // }}}

// Make Deep Copy
MpsMsgNoType *MpsMsgNoType::Copy() const // {{{
{
  return new MpsMsgNoType();
} // }}}
MpsIntMsgType *MpsIntMsgType::Copy() const // {{{
{
  return new MpsIntMsgType();
} // }}}
MpsStringMsgType *MpsStringMsgType::Copy() const // {{{
{
  return new MpsStringMsgType();
} // }}}
MpsBoolMsgType *MpsBoolMsgType::Copy() const // {{{
{
  return new MpsBoolMsgType();
} // }}}
MpsTupleMsgType *MpsTupleMsgType::Copy() const // {{{
{
  return new MpsTupleMsgType(myElements);
} // }}}
MpsChannelMsgType *MpsChannelMsgType::Copy() const // {{{
{
  return new MpsChannelMsgType(*myType);
} // }}}
MpsDelegateLocalMsgType *MpsDelegateLocalMsgType::Copy() const // {{{
{
  return new MpsDelegateLocalMsgType(*myType,GetPid(),GetMaxpid());
} // }}}
MpsDelegateGlobalMsgType *MpsDelegateGlobalMsgType::Copy() const // {{{
{
  return new MpsDelegateGlobalMsgType(*myGlobalType,GetPid(),GetMaxpid());
} // }}}

// Compare
bool MpsMsgNoType::Equal(const MpsExp &Theta, const MpsMsgType &rhs) const // {{{
{
  return dynamic_cast<const MpsMsgNoType*>(&rhs) != NULL;
} // }}}
bool MpsIntMsgType::Equal(const MpsExp &Theta, const MpsMsgType &rhs) const // {{{
{
  return dynamic_cast<const MpsIntMsgType*>(&rhs) != NULL;
} // }}}
bool MpsStringMsgType::Equal(const MpsExp &Theta, const MpsMsgType &rhs) const // {{{
{
  return dynamic_cast<const MpsStringMsgType*>(&rhs) != NULL;
} // }}}
bool MpsBoolMsgType::Equal(const MpsExp &Theta, const MpsMsgType &rhs) const // {{{
{
  return dynamic_cast<const MpsBoolMsgType*>(&rhs) != NULL;
} // }}}
bool MpsTupleMsgType::Equal(const MpsExp &Theta, const MpsMsgType &rhs) const // {{{
{
  const MpsTupleMsgType *rhsptr = dynamic_cast<const MpsTupleMsgType*>(&rhs);
  if (rhsptr==NULL) // Not toupe
    return false;

  if (GetSize() != rhsptr->GetSize()) // Size mismatch
    return false;

  for (int i=0; i<GetSize(); ++i) // Check each index
    if (not GetElement(i)->Equal(Theta,*rhsptr->GetElement(i)))
      return false;
  return true;
} // }}}
bool MpsChannelMsgType::Equal(const MpsExp &Theta, const MpsMsgType &rhs) const // {{{
{
  const MpsChannelMsgType *rhsptr = dynamic_cast<const MpsChannelMsgType*>(&rhs);
  if (rhsptr==NULL)
    return false;
  return myType->Equal(Theta,*rhsptr->myType);
} // }}}
bool MpsDelegateMsgType::Equal(const MpsExp &Theta, const MpsMsgType &rhs) const // {{{
{

  const MpsDelegateMsgType *rhsptr=dynamic_cast<const MpsDelegateMsgType*>(&rhs);
  if (rhsptr==NULL)
    return false;
  return GetPid() == rhsptr->GetPid() &&
         GetMaxpid() == rhsptr->GetMaxpid() &&
         GetLocalType()->Equal(Theta,*rhsptr->GetLocalType());
} // }}}

// Free Global Type Variables
set<string> MpsMsgNoType::FGV() const // {{{
{
  return set<string>();
} // }}}
set<string> MpsIntMsgType::FGV() const // {{{
{
  return set<string>();
} // }}}
set<string> MpsStringMsgType::FGV() const // {{{
{
  return set<string>();
} // }}}
set<string> MpsBoolMsgType::FGV() const // {{{
{
  return set<string>();
} // }}}
set<string> MpsTupleMsgType::FGV() const // {{{
{
  set<string> result;
  for (vector<MpsMsgType*>::const_iterator elt=myElements.begin(); elt!=myElements.end(); ++elt)
  { set<string> fev=(*elt)->FGV();
    result.insert(fev.begin(),fev.end());
  }

  return result;
} // }}}
set<string> MpsChannelMsgType::FGV() const // {{{
{
  return myType->FGV();
} // }}}
set<string> MpsDelegateLocalMsgType::FGV() const // {{{
{
  return myType->FGV();
} // }}}
set<string> MpsDelegateGlobalMsgType::FGV() const // {{{
{
  return myGlobalType->FGV();
} // }}}

// Free Local Type Variables
set<string> MpsMsgNoType::FLV() const // {{{
{
  return set<string>();
} // }}}
set<string> MpsIntMsgType::FLV() const // {{{
{
  return set<string>();
} // }}}
set<string> MpsStringMsgType::FLV() const // {{{
{
  return set<string>();
} // }}}
set<string> MpsBoolMsgType::FLV() const // {{{
{
  return set<string>();
} // }}}
set<string> MpsTupleMsgType::FLV() const // {{{
{
  set<string> result;
  for (vector<MpsMsgType*>::const_iterator elt=myElements.begin(); elt!=myElements.end(); ++elt)
  { set<string> fev=(*elt)->FLV();
    result.insert(fev.begin(),fev.end());
  }

  return result;
} // }}}
set<string> MpsChannelMsgType::FLV() const // {{{
{
  return myType->FLV();
} // }}}
set<string> MpsDelegateLocalMsgType::FLV() const // {{{
{
  return myType->FLV();
} // }}}
set<string> MpsDelegateGlobalMsgType::FLV() const // {{{
{
  return myGlobalType->FLV();
} // }}}

// Free Expression Variables
set<string> MpsMsgNoType::FEV() const // {{{
{
  return set<string>();
} // }}}
set<string> MpsIntMsgType::FEV() const // {{{
{
  return set<string>();
} // }}}
set<string> MpsStringMsgType::FEV() const // {{{
{
  return set<string>();
} // }}}
set<string> MpsBoolMsgType::FEV() const // {{{
{
  return set<string>();
} // }}}
set<string> MpsTupleMsgType::FEV() const // {{{
{
  set<string> result;
  for (vector<MpsMsgType*>::const_iterator elt=myElements.begin(); elt!=myElements.end(); ++elt)
  { set<string> fev=(*elt)->FEV();
    result.insert(fev.begin(),fev.end());
  }

  return result;
} // }}}
set<string> MpsChannelMsgType::FEV() const // {{{
{
  return myType->FEV();
} // }}}
set<string> MpsDelegateLocalMsgType::FEV() const // {{{
{
  return myType->FEV();
} // }}}
set<string> MpsDelegateGlobalMsgType::FEV() const // {{{
{
  return myGlobalType->FEV();
} // }}}

// Rename Global Type Variable
MpsMsgNoType *MpsMsgNoType::GRename(const string &source, const string &dest) const // {{{
{
  return Copy();
} // }}}
MpsIntMsgType *MpsIntMsgType::GRename(const string &source, const string &dest) const // {{{
{
  return Copy();
} // }}}
MpsStringMsgType *MpsStringMsgType::GRename(const string &source, const string &dest) const // {{{
{
  return Copy();
} // }}}
MpsBoolMsgType *MpsBoolMsgType::GRename(const string &source, const string &dest) const // {{{
{
  return Copy();
} // }}}
MpsTupleMsgType *MpsTupleMsgType::GRename(const string &source, const string &dest) const // {{{
{
  vector<MpsMsgType*> newElements;
  for (vector<MpsMsgType*>::const_iterator it=myElements.begin(); it!=myElements.end(); ++it)
    newElements.push_back((*it)->GRename(source,dest));
  MpsTupleMsgType *result = new MpsTupleMsgType(newElements);
  DeleteVector(newElements);
  return result;
} // }}}
MpsChannelMsgType *MpsChannelMsgType::GRename(const string &source, const string &dest) const // {{{
{ MpsGlobalType *newType = myType->GRename(source,dest);
  MpsChannelMsgType *result = new MpsChannelMsgType(*newType);
  delete newType;
  return result;
} // }}}
MpsDelegateLocalMsgType *MpsDelegateLocalMsgType::GRename(const string &source, const string &dest) const // {{{
{
  MpsLocalType *newType = myType->GRename(source,dest);
  MpsDelegateLocalMsgType *result = new MpsDelegateLocalMsgType(*newType,GetPid(),GetMaxpid());
  delete newType;
  return result;
} // }}}
MpsDelegateGlobalMsgType *MpsDelegateGlobalMsgType::GRename(const string &source, const string &dest) const // {{{
{
  MpsGlobalType *newType = myGlobalType->GRename(source,dest);
  MpsDelegateGlobalMsgType *result = new MpsDelegateGlobalMsgType(*newType,GetPid(),GetMaxpid());
  delete newType;
  return result;
} // }}}

// Rename Local Type Variable
MpsMsgNoType *MpsMsgNoType::LRename(const string &source, const string &dest) const // {{{
{
  return Copy();
} // }}}
MpsIntMsgType *MpsIntMsgType::LRename(const string &source, const string &dest) const // {{{
{
  return Copy();
} // }}}
MpsStringMsgType *MpsStringMsgType::LRename(const string &source, const string &dest) const // {{{
{
  return Copy();
} // }}}
MpsBoolMsgType *MpsBoolMsgType::LRename(const string &source, const string &dest) const // {{{
{
  return Copy();
} // }}}
MpsTupleMsgType *MpsTupleMsgType::LRename(const string &source, const string &dest) const // {{{
{
  vector<MpsMsgType*> newElements;
  for (vector<MpsMsgType*>::const_iterator it=myElements.begin(); it!=myElements.end(); ++it)
    newElements.push_back((*it)->LRename(source,dest));
  MpsTupleMsgType *result = new MpsTupleMsgType(newElements);
  DeleteVector(newElements);
  return result;
} // }}}
MpsChannelMsgType *MpsChannelMsgType::LRename(const string &source, const string &dest) const // {{{
{ MpsGlobalType *newType = myType->LRename(source,dest);
  MpsChannelMsgType *result = new MpsChannelMsgType(*newType);
  delete newType;
  return result;
} // }}}
MpsDelegateLocalMsgType *MpsDelegateLocalMsgType::LRename(const string &source, const string &dest) const // {{{
{
  MpsLocalType *newType = myType->LRename(source,dest);
  MpsDelegateLocalMsgType *result = new MpsDelegateLocalMsgType(*newType,GetPid(),GetMaxpid());
  delete newType;
  return result;
} // }}}
MpsDelegateGlobalMsgType *MpsDelegateGlobalMsgType::LRename(const string &source, const string &dest) const // {{{
{
  MpsGlobalType *newType = myGlobalType->LRename(source,dest);
  MpsDelegateGlobalMsgType *result = new MpsDelegateGlobalMsgType(*newType,GetPid(),GetMaxpid());
  delete newType;
  return result;
} // }}}

// Rename Expression Variable
MpsMsgNoType *MpsMsgNoType::ERename(const string &source, const string &dest) const // {{{
{
  return Copy();
} // }}}
MpsIntMsgType *MpsIntMsgType::ERename(const string &source, const string &dest) const // {{{
{
  return Copy();
} // }}}
MpsStringMsgType *MpsStringMsgType::ERename(const string &source, const string &dest) const // {{{
{
  return Copy();
} // }}}
MpsBoolMsgType *MpsBoolMsgType::ERename(const string &source, const string &dest) const // {{{
{
  return Copy();
} // }}}
MpsTupleMsgType *MpsTupleMsgType::ERename(const string &source, const string &dest) const // {{{
{
  vector<MpsMsgType*> newElements;
  for (vector<MpsMsgType*>::const_iterator it=myElements.begin(); it!=myElements.end(); ++it)
    newElements.push_back((*it)->ERename(source,dest));
  MpsTupleMsgType *result = new MpsTupleMsgType(newElements);
  DeleteVector(newElements);
  return result;
} // }}}
MpsChannelMsgType *MpsChannelMsgType::ERename(const string &source, const string &dest) const // {{{
{ MpsGlobalType *newType = myType->ERename(source,dest);
  MpsChannelMsgType *result = new MpsChannelMsgType(*newType);
  delete newType;
  return result;
} // }}}
MpsDelegateLocalMsgType *MpsDelegateLocalMsgType::ERename(const string &source, const string &dest) const // {{{
{
  MpsLocalType *newType = myType->ERename(source,dest);
  MpsDelegateLocalMsgType *result = new MpsDelegateLocalMsgType(*newType,GetPid(),GetMaxpid());
  delete newType;
  return result;
} // }}}
MpsDelegateGlobalMsgType *MpsDelegateGlobalMsgType::ERename(const string &source, const string &dest) const // {{{
{
  MpsGlobalType *newType = myGlobalType->ERename(source,dest);
  MpsDelegateGlobalMsgType *result = new MpsDelegateGlobalMsgType(*newType,GetPid(),GetMaxpid());
  delete newType;
  return result;
} // }}}

// Global Type Substitution
MpsMsgNoType *MpsMsgNoType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  return new MpsMsgNoType();
} // }}}
MpsIntMsgType *MpsIntMsgType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  return new MpsIntMsgType();
} // }}}
MpsStringMsgType *MpsStringMsgType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  return new MpsStringMsgType();
} // }}}
MpsBoolMsgType *MpsBoolMsgType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  return new MpsBoolMsgType();
} // }}}
MpsTupleMsgType *MpsTupleMsgType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  vector<MpsMsgType*> newElements;
  for (vector<MpsMsgType*>::const_iterator elt=myElements.begin(); elt!=myElements.end(); ++elt)
    newElements.push_back((*elt)->GSubst(source,dest,args));

  MpsTupleMsgType *result=new MpsTupleMsgType(newElements);

  //Clean Up
  DeleteVector(newElements);

  return result;
} // }}}
MpsChannelMsgType *MpsChannelMsgType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsGlobalType *newType=myType->GSubst(source,dest,args);
  MpsChannelMsgType *result=new MpsChannelMsgType(*newType);

  // Clean Up
  delete newType;

  return result;
} // }}}
MpsDelegateLocalMsgType *MpsDelegateLocalMsgType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsLocalType *newType = myType->GSubst(source,dest,args);
  MpsDelegateLocalMsgType *result=new MpsDelegateLocalMsgType(*newType,GetPid(),GetMaxpid());

  // Clean Up
  delete newType;

  return result;
} // }}}
MpsDelegateGlobalMsgType *MpsDelegateGlobalMsgType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsGlobalType *newType = myGlobalType->GSubst(source,dest,args);
  MpsDelegateGlobalMsgType *result=new MpsDelegateGlobalMsgType(*newType,GetPid(),GetMaxpid());

  // Clean Up
  delete newType;

  return result;
} // }}}

// Local Type Substitution
MpsMsgNoType *MpsMsgNoType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  return new MpsMsgNoType();
} // }}}
MpsIntMsgType *MpsIntMsgType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  return new MpsIntMsgType();
} // }}}
MpsStringMsgType *MpsStringMsgType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  return new MpsStringMsgType();
} // }}}
MpsBoolMsgType *MpsBoolMsgType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  return new MpsBoolMsgType();
} // }}}
MpsTupleMsgType *MpsTupleMsgType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  vector<MpsMsgType*> newElements;
  for (vector<MpsMsgType*>::const_iterator elt=myElements.begin(); elt!=myElements.end(); ++elt)
    newElements.push_back((*elt)->LSubst(source,dest,args));

  MpsTupleMsgType *result=new MpsTupleMsgType(newElements);

  //Clean Up
  DeleteVector(newElements);

  return result;
} // }}}
MpsChannelMsgType *MpsChannelMsgType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsGlobalType *newType=myType->LSubst(source,dest,args);
  MpsChannelMsgType *result=new MpsChannelMsgType(*newType);

  // Clean Up
  delete newType;

  return result;
} // }}}
MpsDelegateLocalMsgType *MpsDelegateLocalMsgType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsLocalType *newType = myType->LSubst(source,dest,args);
  MpsDelegateLocalMsgType *result=new MpsDelegateLocalMsgType(*newType,GetPid(),GetMaxpid());

  // Clean Up
  delete newType;

  return result;
} // }}}
MpsDelegateGlobalMsgType *MpsDelegateGlobalMsgType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsGlobalType *newType = myGlobalType->LSubst(source,dest,args);
  MpsDelegateGlobalMsgType *result=new MpsDelegateGlobalMsgType(*newType,GetPid(),GetMaxpid());

  // Clean Up
  delete newType;

  return result;
} // }}}

// Expression Substitution
MpsMsgNoType *MpsMsgNoType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  return new MpsMsgNoType();
} // }}}
MpsIntMsgType *MpsIntMsgType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  return new MpsIntMsgType();
} // }}}
MpsStringMsgType *MpsStringMsgType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  return new MpsStringMsgType();
} // }}}
MpsBoolMsgType *MpsBoolMsgType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  return new MpsBoolMsgType();
} // }}}
MpsTupleMsgType *MpsTupleMsgType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  vector<MpsMsgType*> newElements;
  for (vector<MpsMsgType*>::const_iterator elt=myElements.begin(); elt!=myElements.end(); ++elt)
    newElements.push_back((*elt)->ESubst(source,dest));

  MpsTupleMsgType *result=new MpsTupleMsgType(newElements);

  //Clean Up
  DeleteVector(newElements);

  return result;
} // }}}
MpsChannelMsgType *MpsChannelMsgType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  MpsGlobalType *newType=myType->ESubst(source,dest);
  MpsChannelMsgType *result=new MpsChannelMsgType(*newType);

  // Clean Up
  delete newType;

  return result;
} // }}}
MpsDelegateLocalMsgType *MpsDelegateLocalMsgType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  MpsLocalType *newType = myType->ESubst(source,dest);
  MpsDelegateLocalMsgType *result=new MpsDelegateLocalMsgType(*newType,GetPid(),GetMaxpid());

  // Clean Up
  delete newType;

  return result;
} // }}}
MpsDelegateGlobalMsgType *MpsDelegateGlobalMsgType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  MpsGlobalType *newType = myGlobalType->ESubst(source,dest);
  MpsDelegateGlobalMsgType *result=new MpsDelegateGlobalMsgType(*newType,GetPid(),GetMaxpid());

  // Clean Up
  delete newType;

  return result;
} // }}}

// Make parsable string representation
string MpsMsgNoType::ToString(const string &indent) const // {{{
{
  string result="Untyped";
  return result;
} // }}}
string MpsIntMsgType::ToString(const string &indent) const // {{{
{
  string result="Int";
  return result;
} // }}}
string MpsStringMsgType::ToString(const string &indent) const // {{{
{
  string result="String";
  return result;
} // }}}
string MpsBoolMsgType::ToString(const string &indent) const // {{{
{
  string result="Bool";
  return result;
} // }}}
string MpsTupleMsgType::ToString(const string &indent) const // {{{
{
  string newIndent = indent + "  ";
  string result="( ";
  for (vector<MpsMsgType*>::const_iterator it=myElements.begin(); it!=myElements.end(); ++it)
  {
    if (it!=myElements.begin())
      result += ",\n" + newIndent;
    result += (*it)->ToString(newIndent);
  }
  result += " )";
  return result;
} // }}}
string MpsChannelMsgType::ToString(const string &indent) const // {{{
{
  string result=(string)"<" + myType->ToString(indent) + ">";
  return result;
} // }}}
string MpsDelegateLocalMsgType::ToString(const string &indent) const // {{{
{
  string result=myType->ToString(indent) + "@(" + int2string(GetPid()) + " of " + int2string(GetMaxpid()) + ")";
  return result;
} // }}}
string MpsDelegateGlobalMsgType::ToString(const string &indent) const // {{{
{
  string result=myLocalType->ToString(indent) + "@(" + int2string(GetPid()) + " of " + int2string(GetMaxpid()) + ")";
  return result;
} // }}}

// Make string representation with latex markup
string MpsMsgNoType::ToTex(int indent, int sw) const // {{{
{
  string result=ToTex_KW("Untyped");
  return result;
} // }}}
string MpsIntMsgType::ToTex(int indent, int sw) const // {{{
{
  string result=ToTex_KW("Int");
  return result;
} // }}}
string MpsStringMsgType::ToTex(int indent, int sw) const // {{{
{
  string result=ToTex_KW("String");
  return result;
} // }}}
string MpsBoolMsgType::ToTex(int indent, int sw) const // {{{
{
  string result=ToTex_KW("Bool");
  return result;
} // }}}
string MpsTupleMsgType::ToTex(int indent, int sw) const // {{{
{
  int newIndent = indent + 2;
  string result="( ";
  for (vector<MpsMsgType*>::const_iterator it=myElements.begin(); it!=myElements.end(); ++it)
  {
    if (it!=myElements.begin())
      result += ",\\newline\n"
              + ToTex_Hspace(newIndent,sw);
    result += (*it)->ToTex(newIndent,sw);
  }
  result += " )";
  return result;
} // }}}
string MpsChannelMsgType::ToTex(int indent, int sw) const // {{{
{
  string result=(string)"$\\langle$" + myType->ToTex(indent,sw) + "$\\rangle$";
  return result;
} // }}}
string MpsDelegateLocalMsgType::ToTex(int indent, int sw) const // {{{
{
  string result=myType->ToTex(indent,sw) + "@(" + ToTex_PP(GetPid()) + " of " + ToTex_PP(GetMaxpid()) + ")";
  return result;
} // }}}
string MpsDelegateGlobalMsgType::ToTex(int indent, int sw) const // {{{
{
  string result=myLocalType->ToTex(indent,sw) + "@(" + ToTex_PP(GetPid()) + " of " + ToTex_PP(GetMaxpid()) + ")";
  return result;
} // }}}

// Compile to C++
string MpsMsgNoType::ToC() const // {{{
{
  return "void *";
} // }}}
string MpsIntMsgType::ToC() const // {{{
{
  return "mpz_t";
} // }}}
string MpsStringMsgType::ToC() const // {{{
{
  return "std::string";
} // }}}
string MpsBoolMsgType::ToC() const // {{{
{
  return "bool";
} // }}}
string MpsTupleMsgType::ToC() const // {{{
{
  string result="struct {";
  int i=0;
  for (vector<MpsMsgType*>::const_iterator it=myElements.begin(); it!=myElements.end(); ++it)
  {
    result += (*it)->ToString() + " x"+int2string(i++) + "; ";
  }
  result += "}";
  return result;
} // }}}
string MpsChannelMsgType::ToC() const // {{{
{
  return "libpi::Channel";
} // }}}
string MpsDelegateMsgType::ToC() const // {{{
{
  return "libpi::Session";
} // }}}

// Individual accesors
int MpsTupleMsgType::GetSize() const // {{{
{
  return myElements.size();
} // }}}
const MpsMsgType *MpsTupleMsgType::GetElement(int index) const // {{{
{
  if (index<0 || index >=GetSize())
  {
#if APIMS_DEBUG_LEVEL>1
    cerr << "ERROR: Tuple Exp Type index out of bounds!" << endl;
#endif
    return new MpsMsgNoType(); 
  }
  return myElements[index];
} // }}}
const MpsLocalType *MpsDelegateLocalMsgType::GetLocalType() const // {{{
{ return myType;
} // }}}
const MpsLocalType *MpsDelegateGlobalMsgType::GetLocalType() const // {{{
{ return myLocalType;
} // }}}
MpsLocalType *MpsDelegateLocalMsgType::GetLocalType() // {{{
{ return myType;
} // }}}
MpsLocalType *MpsDelegateGlobalMsgType::GetLocalType() // {{{
{ return myLocalType;
} // }}}

/* TypeArg implementation
 *
 */

TypeArg::TypeArg(const string &name, const MpsMsgType &type, const MpsExp &value) // {{{
{
  myName = name;
  myType = type.Copy();
  myValue = value.Copy();
} // }}}
TypeArg::TypeArg(const TypeArg &rhs) // {{{
{
  myName = rhs.myName;
  myType = rhs.myType->Copy();
  myValue = rhs.myValue->Copy();
} // }}}
TypeArg &TypeArg::operator=(const TypeArg &rhs) // {{{
{
  delete myType;
  delete myValue;
  myName = rhs.myName;
  myType = rhs.myType->Copy();
  myValue = rhs.myValue->Copy();
} // }}}
TypeArg::~TypeArg() // {{{
{
  delete myType;
  delete myValue;
} // }}}
string TypeArg::ToString(const string &indent) const // {{{
{
  string result = myName + ": " + myType->ToString(indent + "    ") + " = " + myValue->ToString();
  return result;
} // }}}

