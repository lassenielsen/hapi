#include <apims/mpsparser.hpp>
#include <apims/common.hpp>
#include <dpl/slrparser.hpp>

using namespace std;
using namespace dpl;
using namespace apims;

/* Parsing of MpsTerms from a string
 */
MpsTerm *MpsParser::Pi(const parsetree *tree) // {{{
{
 if (tree->case_name == "pi_par") // PiActs par Pi {{{
  {
    MpsTerm *left = PiActs(tree->content[0]);
    MpsTerm *right = Pi(tree->content[2]);
    MpsTerm *result = new MpsPar(*left, *right,vector<string>(),vector<string>());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->case_name == "pi_acts") //  PiActs {{{
  {                                           
    return PiActs(tree->content[0]);         
  } // }}}                                    
  else if (tree->case_name == "pi_eacts") //  PiEActs {{{
  {                                           
    return PiEActs(tree->content[0]);        
  } // }}}                                    

  throw string("Unknown Pi parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsTerm *MpsParser::PiActs(const parsetree *tree) // {{{
{
  if (tree->case_name == "piacts_end") // PiTerm {{{
  {
    return PiTerm(tree->content[0]);
  } // }}}
  else if (tree->case_name == "piacts_act") // PiAct PiActs {{{
  {
    MpsTerm *succ=PiActs(tree->content[1]);
    return PiAct(tree->content[0],succ);
  } // }}}

  throw string("Unknown PiActs parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsTerm *MpsParser::PiEActs(const parsetree *tree) // {{{
{
  if (tree->case_name == "pieacts_end") // PiEAct {{{
  {
    return PiEAct(tree->content[0]);
  } // }}}
  else if (tree->case_name == "pieacts_act") // PiAct PiEActs {{{
  {
    MpsTerm *succ=PiEActs(tree->content[1]);
    return PiAct(tree->content[0],succ);
  } // }}}

  throw string("Unknown PiEActs parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsTerm *MpsParser::PiAct(const parsetree *tree, MpsTerm *succ) // {{{
{
  if (tree->case_name =="piact_sends") // Ch Sends {{{
  {
    return Sends(tree->content[1],Channel(tree->content[0]),succ);
  } // }}}
  else if (tree->case_name =="piact_recvs") // Ch Recvs {{{
  {
    return Recvs(tree->content[1],Channel(tree->content[0]),succ);
  } // }}}
  else if (tree->case_name == "piact_link") // id = new id ( int of int ) ; {{{
  { MpsTerm *result = new MpsLink(tree->content[3]->root.content,
                                  tree->content[0]->root.content,
                                  stoi(tree->content[5]->root.content),
                                  stoi(tree->content[7]->root.content),
                                  *succ,
                                  false);
    delete succ;
    return result;
  } // }}}
  else if (tree->case_name == "piact_ch") // Gtype id ( Participants ) ; {{{
  { vector<MpsParticipant> participants;
    Participants(tree->content[3],participants);
    MpsGlobalType *type = Gtype(tree->content[0]);
    if (participants.size()!=type->GetMaxPid())
    { delete succ;
      delete type;
      throw string("Participant count does not match type: ") + tree->type_name + "." + tree->case_name;
    }
    MpsTerm *result = new MpsNu(participants,tree->content[1]->root.content, *succ, *type);
    delete succ;
    delete type;
    return result;
  } // }}}
  else if (tree->case_name == "piact_ass") // Mtype id = Exp ; {{{
  { MpsMsgType *type = Mtype(tree->content[0]);
    MpsExp *value = Exp(tree->content[3]);
    MpsTerm *result = new MpsAssign(tree->content[1]->root.content, *value, *type, *succ);
    delete type;
    delete value;
    delete succ;
    return result;
  } // }}}
  else if (tree->case_name == "piact_a_ass") // id = Exp ; {{{
  { MpsExp *value = Exp(tree->content[2]);
    MpsTerm *result = new MpsAssign(tree->content[0]->root.content, *value, MpsMsgNoType(), *succ);
    delete value;
    delete succ;
    return result;
  } // }}}
  else if (tree->case_name == "piact_def") // local pvar Dargs ( Args ) ( Pi ) {{{
  { MpsTerm *body = Pi(tree->content[7]);
    // Parse args
    vector<string> args;
    vector<MpsMsgType*> types;
    Args(tree->content[4],args,types);
    // Parse state
    vector<string> stateargs;
    vector<MpsMsgType*> statetypes;
    DArgs(tree->content[2],stateargs,statetypes);
    MpsTerm *result = new MpsDef(tree->content[1]->root.content, args, types, stateargs, statetypes, *body, *succ, MpsMsgEnv());
    // Clean up
    delete succ;
    delete body;
    DeleteVector(types);
    DeleteVector(statetypes);
    return result;
  } // }}}
  else if (tree->case_name == "piact_gval") // guivalue ( Exps ) ; {{{
  { 
    vector<MpsExp*> args;
    Exps(tree->content[2],args);
    // exps must be int, id, int, exp, exp
    if (args.size()!=5 ||
        typeid(*args[0]) != typeid(MpsIntVal) ||
        typeid(*args[1]) != typeid(MpsVarExp) ||
        typeid(*args[2]) != typeid(MpsIntVal))
    { delete succ;
      DeleteVector(args);
      throw string("Parser error: guilvalue must have exactly 5 arguments of type: int, id, int, string, exp");
    }

    MpsTerm *result = new MpsGuiValue(mpz_get_si(((MpsIntVal*)args[0])->GetValue()),
                                      ((MpsVarExp*)args[1])->ToString(),
                                      mpz_get_si(((MpsIntVal*)args[2])->GetValue()),
                                      *(args[3]),
                                      *(args[4]),
                                      *succ);
    // Clean up
    DeleteVector(args);
    delete succ;
    return result;
  } // }}}
  else if (tree->case_name == "piact_hosts") // host ( Exps ) Mode ; {{{
  { vector<MpsExp*> args;
    bool pure=Mode(tree->content[4],false);
    Exps(tree->content[2],args);
    vector<string> hostParts;
    vector<MpsExp*> expParts;
    // exps must be alternately string and var
    for (int i=0; i<args.size(); ++i)
    { if (i%2==0) // expect string
      { MpsStringVal *val=dynamic_cast<MpsStringVal*>(args[i]);
        if (val==NULL) // Not string value
        { delete succ;
          DeleteVector(expParts);
          DeleteVector(args);
          throw string("Parser error: Arguments for HOST must be string and variable alternatively");
        }
        hostParts.push_back(val->GetValue());
      }
      else
        expParts.push_back(args[i]->Copy());
    }

    MpsHostStatement *result = new MpsHostStatement(hostParts,expParts,*succ,vector<MpsMsgType*>(),pure);
    // Clean up
    delete succ;
    DeleteVector(expParts);
    DeleteVector(args);
    return result;
  } // }}}
  else if (tree->case_name == "piact_hosth") // hostheader ( Exps ) ; {{{
  { vector<MpsExp*> args;
    Exps(tree->content[2],args);
    if (args.size()!=1)
    { delete succ;
      DeleteVector(args);
      throw string("Parsing error: HOSTHEADER takes exactly 1 argument");
    }
    MpsStringVal *header=dynamic_cast<MpsStringVal*>(args[0]);
    if (header==NULL)
    { delete succ;
      DeleteVector(args);
      throw string("Parsing error: Arguments for HOSTHEADER must be a string");
    }

    MpsHostHeader *result = new MpsHostHeader(header->ToString(),*succ);
    // Clean up
    delete succ;
    DeleteVector(args);
    return result;
  } // }}}

  throw string("Unknown PiAct parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsTerm *MpsParser::PiEAct(const parsetree *tree) // {{{
{
  if (tree->case_name == "pieact_link") // global id = new id ( int of int ) ; Pi {{{
  { MpsTerm *succ = Pi(tree->content[11]);
    MpsTerm *result = new MpsLink(tree->content[4]->root.content,
                                  tree->content[1]->root.content,
                                  stoi(tree->content[6]->root.content),
                                  stoi(tree->content[8]->root.content),
                                  *succ,
                                  false);
    delete succ;
    return result;
  } // }}}
  else if (tree->case_name == "pieact_ch") // global Gtype id ( Participants ) ; Pi {{{
  { MpsTerm *succ = Pi(tree->content[7]);
    vector<MpsParticipant> participants;
    Participants(tree->content[4],participants);
    MpsGlobalType *type = Gtype(tree->content[1]);
    if (participants.size()!=type->GetMaxPid())
    { delete succ;
      delete type;
      throw string("Participant count does not match type: ") + tree->type_name + "." + tree->case_name;
    }
    MpsTerm *result = new MpsNu(participants,tree->content[2]->root.content, *succ, *type);
    delete succ;
    delete type;
    return result;
  } // }}}
  else if (tree->case_name == "pieact_ass") // global Mtype id = Exp ; Pi {{{
  { MpsTerm *succ = Pi(tree->content[6]);
    MpsMsgType *type = Mtype(tree->content[1]);
    MpsExp *value = Exp(tree->content[4]);
    MpsTerm *result = new MpsAssign(tree->content[2]->root.content, *value, *type, *succ);
    delete type;
    delete value;
    delete succ;
    return result;
  } // }}}
  else if (tree->case_name == "pieact_a_ass") // global id = Exp ; Pi {{{
  { MpsTerm *succ = Pi(tree->content[5]);
    MpsExp *value = Exp(tree->content[3]);
    MpsTerm *result = new MpsAssign(tree->content[1]->root.content, *value, MpsMsgNoType(), *succ);
    delete value;
    delete succ;
    return result;
  } // }}}
  else if (tree->case_name == "pieact_def") // global pvar Dargs ( Args ) ( Pi ) Pi {{{
  { MpsTerm *succ = Pi(tree->content[9]);
    MpsTerm *body = Pi(tree->content[7]);
    // Parse args
    vector<string> args;
    vector<MpsMsgType*> types;
    Args(tree->content[4],args,types);
    // Parse state
    vector<string> stateargs;
    vector<MpsMsgType*> statetypes;
    DArgs(tree->content[2],stateargs,statetypes);
    MpsTerm *result = new MpsDef(tree->content[1]->root.content, args, types, stateargs, statetypes, *body, *succ, MpsMsgEnv());
    // Clean up
    delete succ;
    delete body;
    DeleteVector(types);
    DeleteVector(statetypes);
    return result;
  } // }}}

  throw string("Unknown PiEAct parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsTerm *MpsParser::PiTerm(const parsetree *tree) // {{{
{
  if (tree->case_name == "piterm_pi") // ( Pi ) {{{
  {
    return Pi(tree->content[1]);
  } // }}}
  else if (tree->case_name == "piterm_end") // {{{
  {
    return new MpsEnd();
  } // }}}
  else if (tree->case_name == "piterm_call") // pvar DExps ( Exps ) {{{
  {
    vector<MpsExp*> args;
    Exps(tree->content[3],args);
    vector<MpsExp*> state;
    DExps(tree->content[1],state);
    MpsTerm *result = new MpsCall(tree->content[0]->root.content, args, state, vector<MpsMsgType*>(), vector<MpsMsgType*>());
    DeleteVector(args);
    DeleteVector(state);
    return result;
  } // }}}
  else if (tree->case_name == "piterm_if") // if Exp then Pi else PiActs {{{
  {
    MpsExp *cond = Exp(tree->content[1]);
    MpsTerm *truebranch = Pi(tree->content[3]);
    MpsTerm *falsebranch = PiActs(tree->content[5]);
    MpsTerm *result = new MpsCond(*cond,*truebranch,*falsebranch);
    delete cond;
    delete truebranch;
    delete falsebranch;
    return result;
  } // }}}
  else if (tree->case_name == "piterm_br") // Ch >> { Branches } {{{
  {
    map<string, MpsTerm*> branches;
    map<string, MpsExp*> assertions;
    Branches(tree->content[3], branches, assertions);
    MpsChannel source=Channel(tree->content[0]);
    // BRANCHE TERMS DOES NOT USE ASSERTIONS
    MpsTerm *result = new MpsBranch(source, branches, vector<string>());

    // Clean up
    DeleteMap(branches);
    DeleteMap(assertions);
    return result;
  } // }}}
  else if (tree->case_name == "piterm_syn") // sync ( Exps ) { Branches } {{{
  { 
    vector<MpsExp*> args;
    Exps(tree->content[2],args);
    // Exps must be int , id
    if (args.size()!=2 ||
        typeid(*args[0]) != typeid(MpsIntVal) ||
        typeid(*args[1]) != typeid(MpsVarExp))
    { DeleteVector(args);
      throw string("Parsing error: Arguments for sync must be: int, id");
    }
    // Init branches
    map<string, MpsTerm*> branches;
    map<string, MpsExp*> assertions;
    Branches(tree->content[5], branches, assertions);

    MpsTerm *result = new MpsSync(mpz_get_si(((MpsIntVal*)args[0])->GetValue()),
                                  ((MpsVarExp*)args[1])->ToString(),
                                  branches,
                                  assertions);
    // Clean up
    DeleteVector(args);
    DeleteMap(branches);
    DeleteMap(assertions);
    return result;
  } // }}}
  else if (tree->case_name == "case19") // guisync ( Exps ) { Inputbranches } {{{
  { 
    vector<MpsExp*> args;
    Exps(tree->content[2],args);
    // Exps must be int , id, int
    if (args.size()!=3 ||
        typeid(*args[0]) != typeid(MpsIntVal) ||
        typeid(*args[1]) != typeid(MpsVarExp) ||
        typeid(*args[2]) != typeid(MpsIntVal))
    { DeleteVector(args);
      throw string("Parsing error: Arguments for sync must be: int, id");
    }

    // Init branches
    map<string, inputbranch> branches;
    InputBranches(tree->content[5], branches);

    MpsTerm *result = new MpsGuiSync(mpz_get_si(((MpsIntVal*)args[0])->GetValue()),
                                     ((MpsVarExp*)args[1])->ToString(),
                                     mpz_get_si(((MpsIntVal*)args[2])->GetValue()),
                                     branches);
    // Clean up
    DeleteVector(args);
    while (branches.size() > 0)
    {
      delete branches.begin()->second.term;
      delete branches.begin()->second.assertion;
      DeleteVector(branches.begin()->second.types);
      DeleteVector(branches.begin()->second.values);
      branches.erase(branches.begin());
    }
    return result;
  } // }}}

  throw string("Unknown PiTrtm parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}

MpsGlobalType *MpsParser::Gtype(const parsetree *tree) // {{{
{
  if (tree->case_name == "gtype_term") // Gterm {{{
  { return Gterm(tree->content[0]);
  } // }}}
  else if (tree->case_name == "gtype_act") // Gact Gtype {{{
  { MpsGlobalType *succ = Gtype(tree->content[1]);
    return Gact(tree->content[0],succ);
  } // }}}

  throw string("Unknown Gtype parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsGlobalType *MpsParser::Gact(const parsetree *tree, MpsGlobalType *succ) { // {{{
  if (tree->case_name == "gact_msg") // int -> int : Mtype NamedAssertion ; {{{
  { MpsMsgType *msgtype = Mtype(tree->content[4]);
    int from = stoi(tree->content[0]->root.content);
    int to = stoi(tree->content[2]->root.content);
    bool a_used=false;
    string a_name;
    MpsExp *assertion = NamedAssertion(tree->content[5],a_used, a_name);
    MpsGlobalType *result = NULL;
    if (a_used)
      result = new MpsGlobalMsgType(from,to,*msgtype,*succ,*assertion,a_name);
    else
      result = new MpsGlobalMsgType(from,to,*msgtype,*succ);
    // Clean Up
    delete assertion;
    delete msgtype;
    delete succ;
    return result;
  } // }}}
  if (tree->case_name == "gact_rec") // rec gvar Targs ; {{{
  { 
    string name = tree->content[1]->root.content;
    vector<TypeArg> args;
    Targs(tree->content[2],args);
    MpsGlobalRecType *result = new MpsGlobalRecType(name,*succ, args);
    delete succ;
    return result;
  } // }}}

  throw string("Unknown Gact parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsGlobalType *MpsParser::Gterm(const parsetree *tree) { // {{{
  if (tree->type_name == "Gterm" && tree->case_name == "gterm_end") // $end ; {{{
  { return new MpsGlobalEndType();
  } // }}}
  else if (tree->case_name == "gterm_call") // gvar Tvals ; {{{
  { vector<MpsExp*> args;
    Tvals(tree->content[1],args);
    MpsGlobalVarType *result = new MpsGlobalVarType(tree->content[0]->root.content, args);
    // Clean Up
    DeleteVector(args);
    return result;
  } // }}}
  else if (tree->case_name == "gterm_branch") // int -> int { Gbranches } {{{
  { int from = string2int(tree->content[0]->root.content);
    int to = string2int(tree->content[2]->root.content);
    map<string,MpsGlobalType*> branches;
    map<string,MpsExp*> assertions;
    Gbranches(tree->content[4],branches,assertions);
    MpsGlobalType *result = new MpsGlobalBranchType(from,to,branches,assertions);
    // Clean up
    DeleteMap(branches);
    DeleteMap(assertions);
    return result;
  } // }}}
  else if (tree->case_name == "gterm_synch") // { Gbranches } {{{
  { map<string,MpsGlobalType*> branches;
    map<string,MpsExp*> assertions;
    Gbranches(tree->content[1],branches,assertions);
    MpsGlobalSyncType *result = new MpsGlobalSyncType(branches,assertions);
    // Clean up
    DeleteMap(branches);
    DeleteMap(assertions);
    return result;
  } // }}}

  throw string("Unknown Gterm parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
void MpsParser::Gbranches(const parsetree *tree, map<string,MpsGlobalType*> &dest, map<string,MpsExp*> &assertions) // {{{
{
  if (tree->case_name == "gbranches_end") // bid Assertion : Gtype {{{
  {
    dest[tree->content[0]->root.content] = Gtype(tree->content[3]);
    assertions[tree->content[0]->root.content] = Assertion(tree->content[1]);
    return;
  } // }}}
  else if (tree->type_name == "Gbranches" && tree->case_name == "gbranches_cont") // bid Assertion COLON Gtype Gbranches {{{
  {
    dest[tree->content[0]->root.content] = Gtype(tree->content[3]);
    assertions[tree->content[0]->root.content] = Assertion(tree->content[1]);
    Gbranches(tree->content[4],dest,assertions);
    return;
  } // }}}

  throw string("Unknown Gbranches parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}

MpsLocalType *MpsParser::Ltype(const parsetree *tree) // {{{
{
  if (tree->case_name == "ltype_term") // Lterm {{{
  { return Lterm(tree->content[0]);
  } // }}}
  else if (tree->case_name == "ltype_act") // Lact Ltype {{{
  { MpsLocalType *succ = Ltype(tree->content[1]);
    return Lact(tree->content[0],succ);
  } // }}}

  throw string("Unknown Ltype parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsLocalType *MpsParser::Lact(const parsetree *tree, MpsLocalType *succ) // {{{
{
  if (tree->case_name == "lact_snd") //  int << Mtype NamedAssertion ; {{{
  {
    MpsMsgType *msgtype = Mtype(tree->content[2]);
    int channel = string2int(tree->content[0]->root.content);
    bool a_used=false;
    string a_name;
    MpsExp *assertion = NamedAssertion(tree->content[3],a_used, a_name);
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
  else if (tree->case_name == "lact_rcv") //  int >> Mtype NamedAssertion ; {{{
  {
    MpsMsgType *msgtype = Mtype(tree->content[2]);
    int channel = string2int(tree->content[0]->root.content);
    bool a_used=false;
    string a_name;
    MpsExp *assertion = NamedAssertion(tree->content[3],a_used, a_name);
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
  else if (tree->case_name == "lact_forall") //  forall id where Exp ; {{{
  {
    string name=tree->content[1]->root.content;
    MpsExp *assertion=Exp(tree->content[3]);
    MpsLocalForallType *result=new MpsLocalForallType(name,*assertion,*succ);
    delete succ;
    delete assertion;
    return result;
  } // }}}
  else if (tree->case_name == "lact_rec") // rec lvar Targs ; {{{
  {
    string name = tree->content[1]->root.content;
    vector<TypeArg> args;
    Targs(tree->content[2],args);
    MpsLocalRecType *result = new MpsLocalRecType(name,*succ,args);
    delete succ;
    return result;
  } // }}}
  
  throw string("Unknown Lact parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsLocalType *MpsParser::Lterm(const parsetree *tree) // {{{
{
  if (tree->case_name == "lterm_bsnd") //  int << { Lbranches } {{{
  {
    int channel = string2int(tree->content[0]->root.content);
    map<string,MpsLocalType*> branches;
    map<string,MpsExp*> assertions;
    Lbranches(tree->content[3],branches,assertions);
    MpsLocalSelectType *result = new MpsLocalSelectType(channel,branches,assertions);
    // Clean up
    DeleteMap(branches);
    DeleteMap(assertions);

    return result;
  } // }}}
  else if (tree->case_name == "lterm_brcv") // int >> { Lbranches } {{{
  {
    int channel = string2int(tree->content[0]->root.content);
    map<string,MpsLocalType*> branches;
    map<string,MpsExp*> assertions;
    Lbranches(tree->content[3],branches,assertions);
    MpsLocalBranchType *result = new MpsLocalBranchType(channel,branches,assertions);
    // Clean up
    DeleteMap(branches);
    DeleteMap(assertions);

    return result;
  } // }}}
  else if (tree->case_name == "lterm_call") // lvar Tvals ; {{{
  {
    vector<MpsExp*> args;
    Tvals(tree->content[1],args);
    MpsLocalVarType *result = new MpsLocalVarType(tree->content[0]->root.content, args);
    // Clean Up
    DeleteVector(args);
    return result;
  } // }}}
  else if (tree->case_name == "lterm_end") // %end {{{
  {
    MpsLocalEndType *result = new MpsLocalEndType();
    return result;
  } // }}}
  else if (tree->case_name == "lterm_sync") // { Lbranches } {{{
  {
    map<string,MpsLocalType*> branches;
    map<string,MpsExp*> assertions;
    Lbranches(tree->content[1],branches,assertions);
    MpsLocalSyncType *result = new MpsLocalSyncType(branches,assertions);
    // Clean up
    DeleteMap(branches);
    DeleteMap(assertions);

    return result;
  } // }}}

  throw string("Unknown Lterm parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
void MpsParser::Lbranches(const parsetree *tree, map<string,MpsLocalType*> &dest, map<string,MpsExp*> &assertions) // {{{
{
  if (tree->case_name == "lbranches_end") // bid Assertion : Ltype {{{
  {
    dest[tree->content[0]->root.content] = Ltype(tree->content[3]);
    assertions[tree->content[0]->root.content]=Assertion(tree->content[1]);
    return;
  } // }}}
  else if (tree->type_name == "Lbranches" && tree->case_name == "case2") // bid Assertion : Ltype , Lbranches {{{
  {
    dest[tree->content[0]->root.content] = Ltype(tree->content[3]);
    assertions[tree->content[0]->root.content]=Assertion(tree->content[1]);
    return Lbranches(tree->content[5],dest,assertions);
  } // }}}

  throw string("Unknown localbranch constructor: ") + tree->type_name + "." + tree->case_name;
} // }}}

MpsMsgType *MpsParser::Mtype(const parsetree *tree) // {{{
{
  if (tree->case_name == "mtype_stype") // Stype {{{
  {
    return Stype(tree->content[0]);
  } // }}}
  else if (tree->case_name == "mtype_gtype") // Gtype ( Participants ) {{{
  {
    MpsGlobalType *gtype = Gtype(tree->content[0]);
    vector<MpsParticipant> participants;
    Participants(tree->content[2],participants);
    if (participants.size()!=gtype->GetMaxPid())
      throw string("Wrong participant count in ") + tree->type_name + "." + tree->case_name;
    MpsChannelMsgType *result = new MpsChannelMsgType(*gtype,participants);
    // Clean up
    delete gtype;

    return result;
  } // }}}
  else if (tree->case_name == "mtype_ltype") // Ltype ( int of Participants ) {{{
  {
    MpsLocalType *ltype = Ltype(tree->content[0]);
    int pid = string2int(tree->content[2]->root.content);
    vector<MpsParticipant> participants;
    Participants(tree->content[4],participants);
    MpsDelegateMsgType *result = new MpsDelegateLocalMsgType(*ltype,pid,participants);
    // Clean up
    delete ltype;

    return result;
  } // }}}
  else if (tree->case_name == "mtype_ptype") // Gtype ( int of Participants ) {{{
  {
    MpsGlobalType *gtype = Gtype(tree->content[0]);
    int pid = string2int(tree->content[2]->root.content);
    vector<MpsParticipant> participants;
    Participants(tree->content[4],participants);
    if (participants.size()!=gtype->GetMaxPid())
      throw string("Wrong participant count in ") + tree->type_name + "." + tree->case_name;
    MpsDelegateGlobalMsgType *result = new MpsDelegateGlobalMsgType(*gtype,pid,participants);

    // Clean up
    delete gtype;

    return result;
  } // }}}

  throw string("Unknown Mtype parsetree: " + tree->type_name + "." + tree->case_name);
} // }}}
MpsMsgType *MpsParser::Stype(const parsetree *tree) // {{{
{
  if (tree->case_name == "type_int") // Int {{{
  {
    MpsIntMsgType *result = new MpsIntMsgType();
    return result;
  } // }}}
  else if (tree->case_name == "type_string") // String {{{
  {
    MpsStringMsgType *result = new MpsStringMsgType();
    return result;
  } // }}}
  else if (tree->case_name == "type_bool") // Bool {{{
  {
    MpsBoolMsgType *result = new MpsBoolMsgType();
    return result;
  } // }}}
  else if (tree->case_name == "type_tuple") // ( Stypes ) {{{
  {
    vector<MpsMsgType*> elements;
    Stypes(tree->content[1],elements);
    if (elements.size()==1) // Single type
      return elements[0];
    else
    {
      MpsTupleMsgType *result = new MpsTupleMsgType(elements);
      DeleteVector(elements);
      return result;
    }
  } // }}}

  throw string("Unknown Stype parsetree: " + tree->type_name + "." + tree->case_name);
} // }}}
void MpsParser::Stypes(const parsetree *tree, vector<MpsMsgType*> &dest) // {{{
{
  if (tree->case_name == "tuple_none") // {{{
  { return;
  } // }}}
  else if (tree->case_name == "tuple_end") // Stype {{{
  { dest.push_back(Stype(tree->content[0]));
    return;
  } // }}}
  else if (tree->case_name == "tuple_some") // Stypes2 {{{
  { return Stypes(tree->content[0],dest);
  } // }}}
  else if (tree->case_name == "tuple_cont") // Stype , Stypes2{{{
  { dest.push_back(Stype(tree->content[0]));
    return Stypes(tree->content[2],dest);
  } // }}}

  throw string("Unknown Stypes parsetree: " + tree->type_name + "." + tree->case_name);
} // }}}

MpsExp *MpsParser::Exp(const parsetree *tree) // {{{
{
  if (tree->case_name == "exp_lvl") // Exp<number> {{{
  {
    return Exp(tree->content[0]);
  } // }}}
  else if (tree->case_name == "exp_if") // if Exp then Exp else Exp {{{
  {
    MpsExp *cond = Exp(tree->content[1]);
    MpsExp *truebranch = Exp(tree->content[3]);
    MpsExp *falsebranch = Exp(tree->content[5]);
    MpsExp *result = new MpsCondExp(*cond,*truebranch,*falsebranch);
    delete cond;
    delete truebranch;
    delete falsebranch;
    return result;
  } // }}}
  else if (tree->case_name == "exp_eq") // Exp3 = Exp2 {{{
  {
    MpsExp *left = Exp(tree->content[0]);
    MpsExp *right = Exp(tree->content[2]);
    MpsExp *result = new MpsBinOpExp("=", *left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->case_name == "exp_leq") // Exp4 <= Exp3 {{{
  {
    MpsExp *left = Exp(tree->content[0]);
    MpsExp *right = Exp(tree->content[2]);
    MpsExp *result = new MpsBinOpExp("<=", *left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->case_name == "exp_plus") // Exp5 + Exp4 {{{
  {
    MpsExp *left = Exp(tree->content[0]);
    MpsExp *right = Exp(tree->content[2]);
    MpsExp *result = new MpsBinOpExp("+",*left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->case_name == "exp_minus") // Exp6 - Exp5 {{{
  {
    MpsExp *left = Exp(tree->content[0]);
    MpsExp *right = Exp(tree->content[2]);
    MpsExp *result = new MpsBinOpExp("-",*left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->case_name == "exp_mult") // Exp7 * Exp6 {{{
  {
    MpsExp *left = Exp(tree->content[0]);
    MpsExp *right = Exp(tree->content[2]);
    MpsExp *result = new MpsBinOpExp("*",*left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->case_name == "exp_div") // Exp8 / Exp7 {{{
  {
    MpsExp *left = Exp(tree->content[0]);
    MpsExp *right = Exp(tree->content[2]);
    MpsExp *result = new MpsBinOpExp("/",*left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->case_name == "exp_and") // Exp9 and Exp8 {{{
  {
    MpsExp *left = Exp(tree->content[0]);
    MpsExp *right = Exp(tree->content[2]);
    MpsExp *result = new MpsBinOpExp("and", *left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->case_name == "exp_or") // Exp10 or Exp9 {{{
  {
    MpsExp *left = Exp(tree->content[0]);
    MpsExp *right = Exp(tree->content[2]);
    MpsExp *result = new MpsBinOpExp("or",*left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->case_name == "exp_amp") // Exp10 & int {{{
  {
    MpsExp *left=Exp(tree->content[0]);
    mpz_t val;
    mpz_init_set_str(val,tree->content[2]->root.content.c_str(),10);
    MpsExp *right=new MpsIntVal(val);
    mpz_clear(val);
    MpsExp *result = new MpsBinOpExp("&", *left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->case_name == "exp_not") // not Exp12 {{{
  {
    MpsExp *sub = Exp(tree->content[1]);
    MpsExp *result = new MpsUnOpExp("not",*sub);
    delete sub;
    return result;
  } // }}}
  else if (tree->case_name == "exp_id") // id {{{
  {
    return new MpsVarExp(tree->content[0]->root.content,MpsMsgNoType());
  } // }}}
  else if (tree->case_name == "exp_int") // int {{{
  {
    mpz_t val;
    mpz_init_set_str(val,tree->content[0]->root.content.c_str(),10);
    MpsExp *exp=new MpsIntVal(val);
    mpz_clear(val);
    return exp;
  } // }}}
  else if (tree->case_name == "exp_str") // string {{{
  {
    return new MpsStringVal(unwrap_string(tree->content[0]->root.content));
  } // }}}
  else if (tree->case_name == "exp_true") // true {{{
  {
    return new MpsBoolVal(true);
  } // }}}
  else if (tree->case_name == "exp_false") // false {{{
  {
    return new MpsBoolVal(false);
  } // }}}
  else if (tree->case_name == "exp_tuple") // ( Exps ) {{{
  {
    vector<MpsExp*> elements;
    elements.clear();
    Exps(tree->content[1],elements);
    if (elements.size() == 1) // Single expression
      return elements[0]; // No Clean-up
    else
    {
      MpsTupleExp *result = new MpsTupleExp(elements);
      // Clean up
      DeleteVector(elements);
      return result;
    }
  } // }}}
  else if (tree->case_name == "exp_sys") // system & string {{{
  {
    return new MpsSystemExp(unwrap_string(tree->content[2]->root.content));
  } // }}}

  throw string("Unknown Exp-parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
void MpsParser::Exps(const parsetree *tree, vector<MpsExp*> &dest) // {{{
{
  if (tree->case_name == "tuple_some") // Exps2
    return Exps(tree->content[0],dest);
  else if (tree->case_name == "tuple_none") //
    return;
  else if (tree->case_name == "tuple_end") // Exp
  { dest.push_back(Exp(tree->content[0]));
    return;
  }
  else if (tree->case_name == "tuple_cons") // Exp , Exps2
  { dest.push_back(Exp(tree->content[0]));
    Exps(tree->content[2],dest);
    return;
  }

  throw string("Unknown Exps parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}

MpsChannel MpsParser::Channel(const parsetree *tree) // {{{
{
  if (tree->case_name == "ch") // id [ int ]
    return MpsChannel(tree->content[0]->root.content,stoi(tree->content[2]->root.content));
  throw string("Unknown Ch parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsParticipant MpsParser::Participant(const dpl::parsetree *tree) // {{{
{ // participant ::= int Mode
  std::string id=tree->content[0]->root.content;
  return MpsParticipant(string2int(id),id,Mode(tree->content[1],false));
 } // }}}
void MpsParser::Participants(const parsetree *tree, vector<MpsParticipant> &dest) // {{{
{
  if (tree->case_name == "participants_end") // Participant {{{
  { dest.push_back(Participant(tree->content[0]));
    return;
  } // }}}
  if (tree->case_name == "participants_cont") // Participant , Participants {{{
  {
    dest.push_back(Participant(tree->content[0]));
    return Participants(tree->content[2], dest);
  } // }}}

  throw string("Unknown participants parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsExp *MpsParser::Assertion(const parsetree *tree) // {{{
{ 
  if (tree->case_name == "ass_some") // req exp {{{
  {
    return Exp(tree->content[1]);
  } // }}}
  else if (tree->case_name == "ass_none") // {{{
  {
    return new MpsBoolVal(true);
  } // }}}

  throw string("Unknown Assertion parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsExp *MpsParser::NamedAssertion(const parsetree *tree, bool &used, std::string &name) // {{{
{
  if (tree->case_name == "name") // as id Assertion {{{
  {
    used=true;
    name=tree->content[1]->root.content;
    return Assertion(tree->content[2]);
  } // }}}
  else if (tree->case_name == "noname") // {{{
  {
    used=false;
    return new MpsBoolVal(true);
  } // }}}

  throw string("Unknown NamedAssertion parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
void MpsParser::Branch(const parsetree *tree, map<string,MpsTerm*> &terms,  map<string,MpsExp*> &assertions) // {{{
{
  if (tree->case_name == "branch") // bid Assertion COLON pi
  {
    terms[tree->content[0]->root.content] = Pi(tree->content[3]);
    assertions[tree->content[0]->root.content] = Assertion(tree->content[1]);
    return;
  }

  throw string("Unknown branch constructor: ") + tree->type_name + "." + tree->case_name;
} // }}}
void MpsParser::Branches(const parsetree *tree, map<string,MpsTerm*> &terms, map<string,MpsExp*> &assertions) // {{{
{
  if (tree->case_name == "branches_end") // Branch
    return Branch(tree->content[0], terms, assertions);
  else if (tree->case_name == "branches_cont") // Branch Branches
  { Branch(tree->content[0], terms, assertions);
    return Branches(tree->content[1], terms, assertions);
  }

  throw string("Unknown branches constructor: ") + tree->type_name + "." + tree->case_name;
} // }}}
void MpsParser::InputBranch(const parsetree *tree, map<string,inputbranch> &dest) // {{{
{
  if (tree->case_name == "inputbranch") // bid Assertion ( Targs2 ) COLON Pi
  {
    inputbranch branch;
    branch.names.clear();
    branch.args.clear();
    branch.types.clear();
    branch.values.clear();
    Targs(tree->content[3],branch.args,branch.types,branch.values);
    branch.names = branch.args;
    branch.term = Pi(tree->content[6]);
    branch.assertion=Assertion(tree->content[1]);
    dest[tree->content[0]->root.content] = branch;
    return;
  }

  throw string("Unknown inputbranch constructor: ") + tree->type_name + "." + tree->case_name;
} // }}}
void MpsParser::InputBranches(const parsetree *tree, map<string,inputbranch> &dest) // {{{
{
  if (tree->case_name == "inputbranches_end") // inputbranch
    return InputBranch(tree->content[0], dest);
  else if (tree->case_name == "inputbranches_cont") // inputbranch inputbranches
  {
    InputBranch(tree->content[0], dest);
    return InputBranches(tree->content[1],dest);
  }

  throw string("Unknown inputbranches constructor: ") + tree->type_name + "." + tree->case_name;
} // }}}
void MpsParser::Args(const parsetree *tree, vector<string> &args, vector<MpsMsgType*> &types) // {{{
{
  if (tree->case_name == "args_some") // Args2
    return Args(tree->content[0],args,types);
  else if (tree->case_name == "args_none") // 
    return; // Do nothing
  else if (tree->case_name == "args_end") // Mtype id
  {
    args.push_back(tree->content[1]->root.content);
    types.push_back(Mtype(tree->content[0]));
    return;
  }
  else if (tree->case_name == "args_cons") // Mtype id  , Args2
  {
    args.push_back(tree->content[1]->root.content);
    types.push_back(Mtype(tree->content[0]));
    return Args(tree->content[3],args,types);
  }

  throw string("Unknown Args constructor: ") + tree->type_name + "." + tree->case_name;
} // }}}
void MpsParser::DArgs(const parsetree *tree, vector<string> &args, vector<MpsMsgType*> &types) // {{{
{
  if (tree->case_name == "dargs_some") // < Args2 >
    return Args(tree->content[1],args,types);
  else if (tree->case_name == "dargs_none") // 
    return; // Do nothing

  throw string("Unknown Dargs constructor: ") + tree->type_name + "." + tree->case_name;
} // }}}
void MpsParser::DExps(const parsetree *tree, vector<MpsExp*> &exps) // {{{
{
  if (tree->case_name == "dexps_some") // < Exps >
    return Exps(tree->content[1],exps);
  else if (tree->case_name == "dexps_none") // 
    return; // Do nothing

  throw string("Unknown Dexps constructor: ") + tree->type_name + "." + tree->case_name;
} // }}}
bool MpsParser::Mode(const dpl::parsetree *tree, bool d) // {{{
{ // mode ::= | pure | impure
  if (tree->case_name == "mode_default")
    return d;
  else if (tree->case_name == "mode_pure") // pure
    return true;
  else if (tree->case_name == "mode_impure") // impure
    return false;

  throw std::string("Unknown mode parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsTerm *MpsParser::Send(const parsetree *tree, MpsChannel dest, MpsTerm *succ) // {{{
{
  if (tree->case_name == "send_val") // << Exp {{{
  { MpsExp *value = Exp(tree->content[1]);
    MpsTerm *result = new MpsSnd(dest, *value, *succ, MpsMsgNoType(), false);
    delete value;
    delete succ;
    return result;
  } // }}}
  else if (tree->case_name == "send_lbl") // << bid {{{
  { MpsTerm *result = new MpsSelect(dest, tree->content[1]->root.content, *succ, false);
    delete succ;
    return result;
  } // }}}

  throw string("Unknown Send parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsTerm *MpsParser::Sends(const parsetree *tree, MpsChannel dest, MpsTerm *succ) // {{{
{
  if (tree->case_name == "sends_end") // send {{{
  { return Send(tree->content[0],dest,succ);
  } // }}}
  else if (tree->case_name == "sends_cont") // send sends {{{
  { MpsTerm *tmp=Sends(tree->content[1],dest,succ);
    return Send(tree->content[0],dest,tmp);
  } // }}}

  throw string("Unknown Sends parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsTerm *MpsParser::Recv(const parsetree *tree, MpsChannel dest, MpsTerm *succ) // {{{
{
  if (tree->case_name == "recv_val") // >> id {{{
  { MpsTerm *result = new MpsRcv(dest, tree->content[1]->root.content, -1, -1, *succ, MpsMsgNoType(), false);

    delete succ;
    return result;
  } // }}}
  else if (tree->case_name == "recv_ses") // >> id ( int of int ) {{{
  { MpsTerm *result = new MpsRcv(dest,
                                 tree->content[1]->root.content,
                                 string2int(tree->content[3]->root.content),
                                 string2int(tree->content[5]->root.content),
                                 *succ,
                                 MpsMsgNoType(),
                                 false);
    delete succ;
    return result;
  } // }}}

  throw string("Unknown Recv parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsTerm *MpsParser::Recvs(const parsetree *tree, MpsChannel dest, MpsTerm *succ) // {{{
{
  if (tree->case_name == "recvs_end") // recv {{{
  { return Recv(tree->content[0],dest,succ);
  } // }}}
  else if (tree->case_name == "recvs_cont") // recv recvs {{{
  { MpsTerm *tmp=Recvs(tree->content[1],dest,succ);
    return Recv(tree->content[0],dest,tmp);
  } // }}}

  throw string("Unknown Recvs parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
void MpsParser::Tvals(const parsetree *tree, vector<MpsExp*> &dest) // {{{
{
  if (tree->case_name == "tvals_some") // < Exps > {{{
    return Exps(tree->content[1], dest); // }}}
  if (tree->case_name == "tvals_none") // epsilon {{{
    return; // }}}

  throw string("Unknown Tvals: ") + tree->type_name + "." + tree->case_name;
} // }}}
TypeArg MpsParser::Targ(const parsetree *tree) // {{{
{
  if (tree->case_name == "targ_def") // Mtype id = Exp {{{
  { string argname=tree->content[1]->root.content;
    MpsMsgType *argtype=Mtype(tree->content[0]);
    MpsExp *argvalue=Exp(tree->content[3]);
    TypeArg result(argname,*argtype,*argvalue);
    // clean up
    delete argtype;
    delete argvalue;
    return result;
  } // }}}

  throw string("Unknown Targ constructor: ") + tree->type_name + "." + tree->case_name;
} // }}}
void MpsParser::Targ(const parsetree *tree, vector<string> &names, vector<MpsMsgType*> &types, vector<MpsExp*> &values) // {{{
{
  if (tree->case_name == "targ_def") // Mtype id = Exp {{{
  { names.push_back(tree->content[1]->root.content);
    types.push_back(Mtype(tree->content[0]));
    values.push_back(Exp(tree->content[3]));
    return;
  } // }}}

  throw string("Unknown Targ constructor: ") + tree->type_name + "." + tree->case_name;
} // }}}
void MpsParser::Targs(const parsetree *tree, vector<TypeArg> &dest) // {{{
{
  if (tree->case_name == "targs_some") // < Targs2 > OR < Targs3 > {{{
    return Targs(tree->content[1], dest); // }}}
  else if (tree->case_name == "targs_none") // epsilon {{{
    return; // }}}
  else if (tree->case_name == "targs_end") // Targ {{{
  { dest.push_back(Targ(tree->content[0]));
    return;
  } // }}}
  else if (tree->case_name == "targs_cont") // Targ , Targs2 {{{
  { dest.push_back(Targ(tree->content[0]));
    return Targs(tree->content[2],dest);
  } // }}}

  throw string("Unknown Targs constructor: ") + tree->type_name + "." + tree->case_name;
} // }}}
void MpsParser::Targs(const parsetree *tree, vector<string> &names, vector<MpsMsgType*> &types, vector<MpsExp*> &values) // {{{
{
  if (tree->case_name == "targs_some") // < Targs2 > OR < Targs3 > {{{
    return Targs(tree->content[1], names, types, values); // }}}
  else if (tree->case_name == "targs_none") // epsilon {{{
    return; // }}}
  else if (tree->case_name == "targs_end") // Targ {{{
    return Targ(tree->content[0],names,types,values); // }}}
  else if (tree->case_name == "targs_cont") // Targ , Targs2 {{{
  { Targ(tree->content[0],names,types,values);
    return Targs(tree->content[2],names,types,values);
  } // }}}

  throw string("Unknown Targs constructor: ") + tree->type_name + "." + tree->case_name;
} // }}}

MpsTerm *MpsParser::Pi(const std::string &str) // {{{
{
  SlrParser parser("Pi");
  parser.LoadFile("/opt/apims/bnf/syntax.bnf");
  parsetree *tree = parser.Parse(str);
  MpsTerm *result=Pi(tree);
  delete tree;
  return result;
} // }}}
MpsGlobalType *MpsParser::Gtype(const string &str) // {{{
{
  SlrParser parser("Gtype");
  parser.LoadFile("/opt/apims/bnf/syntax.bnf");
  parsetree *tree = parser.Parse(str);
  MpsGlobalType *result=Gtype(tree);
  delete tree;
  return result;
} // }}}
MpsLocalType *MpsParser::Ltype(const string &str) // {{{
{
  SlrParser parser("Ltype");
  parser.LoadFile("/opt/apims/bnf/syntax.bnf");
  parsetree *tree = parser.Parse(str);
  MpsLocalType *result=Ltype(tree);
  delete tree;
  return result;
} // }}}
MpsMsgType *MpsParser::Mtype(const string &str) // {{{
{
  SlrParser parser("Mtype");
  parser.LoadFile("/opt/apims/bnf/syntax.bnf");
  parsetree *tree = parser.Parse(str);
  MpsMsgType *result=Mtype(tree);
  delete tree;
  return result;
} // }}}
MpsExp *MpsParser::Exp(const std::string &str) // {{{
{
  SlrParser parser("Exp");
  parser.LoadFile("/opt/apims/bnf/syntax.bnf");
  parsetree *tree = parser.Parse(str);
  MpsExp *result=Exp(tree);
  delete tree;
  return result;
} // }}}
