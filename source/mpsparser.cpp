#include <hapi/mpsparser.hpp>
#include <hapi/common.hpp>
#include <dpl/slrparser.hpp>
#include <hapi/hapi_bnf.hpp>

using namespace std;
using namespace dpl;
using namespace hapi;

/* Parsing of MpsTerms from a string
 */
MpsTerm *MpsParser::Pi(const parsetree *tree) // {{{
{
  if (tree->Case() == "pi_lvl") // Pi<number> {{{
  {
    return Pi(tree->Child(0));
  } // }}}
  else if (tree->Case() == "pi_ppar") // Pi2 ppar Pi {{{
  {
    MpsTerm *left = Pi(tree->Child(0));
    MpsTerm *right = Pi(tree->Child(2));
    MpsTerm *result = new MpsPar(*left, *right,vector<string>(),vector<string>());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->Case() == "pi_par") // PiActs par Pi2 {{{
  {
    MpsTerm *left = PiActs(tree->Child(0));
    MpsTerm *right = Pi(tree->Child(2));
    MpsTerm *result = new MpsPar(*left, *right,vector<string>(),vector<string>());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->Case() == "pi_acts") //  PiActs {{{
  {                                           
    return PiActs(tree->Child(0));         
  } // }}}                                    
  else if (tree->Case() == "pi_eacts") //  PiEActs {{{
  {                                           
    return PiEActs(tree->Child(0));        
  } // }}}                                    

  throw string("Unknown Pi parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
MpsTerm *MpsParser::PiActs(const parsetree *tree) // {{{
{
  if (tree->Case() == "piacts_end") // PiTerm {{{
  {
    return PiTerm(tree->Child(0));
  } // }}}
  else if (tree->Case() == "piacts_act") // PiAct PiActs {{{
  {
    MpsTerm *succ=PiActs(tree->Child(1));
    return PiAct(tree->Child(0),succ);
  } // }}}

  throw string("Unknown PiActs parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
MpsTerm *MpsParser::PiEActs(const parsetree *tree) // {{{
{
  if (tree->Case() == "pieacts_end") // PiEAct {{{
  {
    return PiEAct(tree->Child(0));
  } // }}}
  else if (tree->Case() == "pieacts_act") // PiAct PiEActs {{{
  {
    MpsTerm *succ=PiEActs(tree->Child(1));
    return PiAct(tree->Child(0),succ);
  } // }}}

  throw string("Unknown PiEActs parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
MpsTerm *MpsParser::PiAct(const parsetree *tree, MpsTerm *succ) // {{{
{ if (tree->Case() =="piact_sends") // Ch Sends {{{
  {
    return Sends(tree->Child(1),Channel(tree->Child(0)),succ);
  } // }}}
  else if (tree->Case() =="piact_recvs") // Ch Recvs {{{
  {
    return Recvs(tree->Child(1),Channel(tree->Child(0)),succ);
  } // }}}
  else if (tree->Case() == "piact_tsnd") // id << Mtype ; {{{
  { MpsMsgType *type=Mtype(tree->Child(2));
    MpsTerm *result = new MpsSndType(tree->Child(0)->Token().Content(),
                                     *type,
                                     *succ,
                                     false);
    delete type;
    delete succ;
    return result;
  } // }}}
  else if (tree->Case() == "piact_trcv") // id >> avar ; {{{
  { MpsTerm *result = new MpsRcvType(tree->Child(0)->Token().Content(),
                                     tree->Child(2)->Token().Content(),
                                     *succ,
                                     false);
    delete succ;
    return result;
  } // }}}
  else if (tree->Case() == "piact_link") // id = new id ( int of int ) ; {{{
  { MpsTerm *result = new MpsLink(tree->Child(3)->Token().Content(),
                                  tree->Child(0)->Token().Content(),
                                  stoi(tree->Child(5)->Token().Content()),
                                  stoi(tree->Child(7)->Token().Content()),
                                  *succ,
                                  false);
    delete succ;
    return result;
  } // }}}
  else if (tree->Case() == "piact_new") // Gtype Ids ( Participants ) ; {{{
  { vector<MpsParticipant> participants;
    Participants(tree->Child(3),participants);
    vector<string> names;
    Ids(tree->Child(1), names);
    MpsGlobalType *type=Gtype(tree->Child(0));
    MpsTerm *result = new MpsNew(names, participants, *type, *succ);
    delete type;
    delete succ;
    return result;
  } // }}}
  else if (tree->Case() == "piact_ch") // Gtype id ( Participants ) ; {{{
  { vector<MpsParticipant> participants;
    Participants(tree->Child(3),participants);
    MpsGlobalType *type = Gtype(tree->Child(0));
    if (participants.size()<type->GetMaxPid())
    { delete succ;
      delete type;
      throw string("Participant count does not match type: ") + tree->Type() + "." + tree->Case();
    }
    MpsTerm *result = new MpsNu(participants,tree->Child(1)->Token().Content(), *succ, *type);
    delete succ;
    delete type;
    return result;
  } // }}}
  else if (tree->Case() == "piact_ass") // Mtype id = Exp ; {{{
  { MpsMsgType *type = Mtype(tree->Child(0));
    MpsExp *value = Exp(tree->Child(3));
    MpsTerm *result = new MpsAssign(tree->Child(1)->Token().Content(), *value, *type, *succ);
    delete type;
    delete value;
    delete succ;
    return result;
  } // }}}
  else if (tree->Case() == "piact_a_ass") // id = Exp ; {{{
  { MpsExp *value = Exp(tree->Child(2));
    MpsTerm *result = new MpsAssign(tree->Child(0)->Token().Content(), *value, MpsMsgNoType(), *succ);
    delete value;
    delete succ;
    return result;
  } // }}}
  else if (tree->Case() == "piact_def") // local Mode pvar Dargs ( Args ) ( Pi ) {{{
  { MpsTerm *body = Pi(tree->Child(8));
    // Parse args
    vector<string> args;
    vector<MpsMsgType*> types;
    Args(tree->Child(5),args,types);
    // Parse state
    vector<string> stateargs;
    vector<MpsMsgType*> statetypes;
    DArgs(tree->Child(3),stateargs,statetypes);
    bool pure=Mode(tree->Child(1),false);
    MpsTerm *result = new MpsDef(tree->Child(2)->Token().Content(), args, types, stateargs, statetypes, *body, *succ, pure);
    // Clean up
    delete succ;
    delete body;
    DeleteVector(types);
    DeleteVector(statetypes);
    return result;
  } // }}}
//  else if (tree->Case() == "piact_sdef") // local Mode service pvar Dargs ( Args ) ( Pi ) {{{
//  { MpsTerm *body = Pi(tree->Child(9));
//    // Parse args
//    vector<string> args;
//    vector<MpsMsgType*> types;
//    Args(tree->Child(6),args,types);
//    // Parse state
//    vector<string> stateargs;
//    vector<MpsMsgType*> statetypes;
//    DArgs(tree->Child(4),stateargs,statetypes);
//    bool pure=Mode(tree->Child(1),false);
//    // Build term
//    string initName=tree->Child(3)->Token().Content()+"_Init";
//    ...
//    MpsTerm *call=new MpsCall(initName,
//    ...
//    MpsTerm *result = new MpsDef(tree->Child(2)->Token().Content(), args, types, stateargs, statetypes, *body, *succ, MpsMsgEnv(), pure);
//    // Clean up
//    delete succ;
//    delete body;
//    DeleteVector(types);
//    DeleteVector(statetypes);
//    return result;
//  } // }}}
  else if (tree->Case() == "piact_gval") // guivalue ( Exps ) ; {{{
  { 
    vector<MpsExp*> args;
    Exps(tree->Child(2),args);
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
  else if (tree->Case() == "piact_hosts") // host ( Exps ) Mode ; {{{
  { vector<MpsExp*> args;
    bool pure=Mode(tree->Child(4),false);
    Exps(tree->Child(2),args);
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
  else if (tree->Case() == "piact_hosth") // hostheader ( Exps ) ; {{{
  { vector<MpsExp*> args;
    Exps(tree->Child(2),args);
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

  throw string("Unknown PiAct parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
MpsTerm *MpsParser::PiEAct(const parsetree *tree) // {{{
{
  if (tree->Case() == "pieact_link") // global id = new id ( int of int ) ; Pi {{{
  { MpsTerm *succ = Pi(tree->Child(11));
    MpsTerm *result = new MpsLink(tree->Child(4)->Token().Content(),
                                  tree->Child(1)->Token().Content(),
                                  stoi(tree->Child(6)->Token().Content()),
                                  stoi(tree->Child(8)->Token().Content()),
                                  *succ,
                                  false);
    delete succ;
    return result;
  } // }}}
  else if (tree->Case() == "pieact_new") // global Gtype Ids ( Participants ) ; Pi {{{
  { MpsTerm *succ = Pi(tree->Child(7));
    vector<MpsParticipant> participants;
    Participants(tree->Child(4),participants);
    vector<string> names;
    Ids(tree->Child(2), names);
    MpsGlobalType *type=Gtype(tree->Child(1));
    MpsTerm *result = new MpsNew(names, participants, *type, *succ);
    delete type;
    delete succ;
    return result;
  } // }}}
  else if (tree->Case() == "pieact_ch") // global Gtype id ( Participants ) ; Pi {{{
  { MpsTerm *succ = Pi(tree->Child(7));
    vector<MpsParticipant> participants;
    Participants(tree->Child(4),participants);
    MpsGlobalType *type = Gtype(tree->Child(1));
    if (participants.size()!=type->GetMaxPid())
    { delete succ;
      delete type;
      throw string("Participant count does not match type: ") + tree->Type() + "." + tree->Case();
    }
    MpsTerm *result = new MpsNu(participants,tree->Child(2)->Token().Content(), *succ, *type);
    delete succ;
    delete type;
    return result;
  } // }}}
  else if (tree->Case() == "pieact_ass") // global Mtype id = Exp ; Pi {{{
  { MpsTerm *succ = Pi(tree->Child(6));
    MpsMsgType *type = Mtype(tree->Child(1));
    MpsExp *value = Exp(tree->Child(4));
    MpsTerm *result = new MpsAssign(tree->Child(2)->Token().Content(), *value, *type, *succ);
    delete type;
    delete value;
    delete succ;
    return result;
  } // }}}
  else if (tree->Case() == "pieact_a_ass") // global id = Exp ; Pi {{{
  { MpsTerm *succ = Pi(tree->Child(5));
    MpsExp *value = Exp(tree->Child(3));
    MpsTerm *result = new MpsAssign(tree->Child(1)->Token().Content(), *value, MpsMsgNoType(), *succ);
    delete value;
    delete succ;
    return result;
  } // }}}
  else if (tree->Case() == "pieact_def") // global Mode pvar Dargs ( Args ) ( Pi ) Pi {{{
  { MpsTerm *succ = Pi(tree->Child(10));
    MpsTerm *body = Pi(tree->Child(8));
    // Parse args
    vector<string> args;
    vector<MpsMsgType*> types;
    Args(tree->Child(5),args,types);
    // Parse state
    vector<string> stateargs;
    vector<MpsMsgType*> statetypes;
    DArgs(tree->Child(3),stateargs,statetypes);
    bool pure=Mode(tree->Child(1),false);
    MpsTerm *result = new MpsDef(tree->Child(2)->Token().Content(), args, types, stateargs, statetypes, *body, *succ, pure);
    // Clean up
    delete succ;
    delete body;
    DeleteVector(types);
    DeleteVector(statetypes);
    return result;
  } // }}}
  else if (tree->Case() == "pieact_sdef") // global Mode service pvar Dargs ( Args ) ( Pi ) Pi {{{
  { MpsTerm *succ = Pi(tree->Child(11));
    MpsTerm *body = Pi(tree->Child(9));
    // Parse args
    vector<string> args;
    vector<MpsMsgType*> types;
    Args(tree->Child(6),args,types);
    // Parse state
    vector<string> stateargs;
    vector<MpsMsgType*> statetypes;
    DArgs(tree->Child(4),stateargs,statetypes);
    bool pure=Mode(tree->Child(1),false);
    // Extract name
    string name = tree->Child(2)->Token().Content();




    MpsTerm *result = new MpsDef(tree->Child(2)->Token().Content(), args, types, stateargs, statetypes, *body, *succ, pure);
    // Clean up
    delete succ;
    delete body;
    DeleteVector(types);
    DeleteVector(statetypes);
    return result;
  } // }}}

  throw string("Unknown PiEAct parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
MpsTerm *MpsParser::PiTerm(const parsetree *tree) // {{{
{
  if (tree->Case() == "piterm_pi") // ( Pi ) {{{
  {
    return Pi(tree->Child(1));
  } // }}}
  else if (tree->Case() == "piterm_end") // {{{
  {
    return new MpsEnd();
  } // }}}
  else if (tree->Case() == "piterm_call") // pvar DExps ( Exps ) {{{
  {
    vector<MpsExp*> args;
    Exps(tree->Child(3),args);
    vector<MpsExp*> state;
    DExps(tree->Child(1),state);
    MpsTerm *result = new MpsCall(tree->Child(0)->Token().Content(), args, state, vector<MpsMsgType*>(), vector<MpsMsgType*>());
    DeleteVector(args);
    DeleteVector(state);
    return result;
  } // }}}
  else if (tree->Case() == "piterm_if") // if Exp then Pi else PiActs {{{
  {
    MpsExp *cond = Exp(tree->Child(1));
    MpsTerm *truebranch = Pi(tree->Child(3));
    MpsTerm *falsebranch = PiActs(tree->Child(5));
    MpsTerm *result = new MpsCond(*cond,*truebranch,*falsebranch);
    delete cond;
    delete truebranch;
    delete falsebranch;
    return result;
  } // }}}
  else if (tree->Case() == "piterm_br") // Ch >> { Branches } {{{
  {
    map<string, MpsTerm*> branches;
    map<string, MpsExp*> assertions;
    Branches(tree->Child(3), branches, assertions);
    MpsChannel source=Channel(tree->Child(0));
    // BRANCHE TERMS DOES NOT USE ASSERTIONS
    MpsTerm *result = new MpsBranch(source, branches, vector<string>());

    // Clean up
    DeleteMap(branches);
    DeleteMap(assertions);
    return result;
  } // }}}
  else if (tree->Case() == "piterm_syn") // sync ( Exps ) { Branches } {{{
  { 
    vector<MpsExp*> args;
    Exps(tree->Child(2),args);
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
    Branches(tree->Child(5), branches, assertions);

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
  else if (tree->Case() == "piterm_gsyn") // guisync ( Exps ) { Inputbranches } {{{
  { 
    vector<MpsExp*> args;
    Exps(tree->Child(2),args);
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
    InputBranches(tree->Child(5), branches);

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

  throw string("Unknown PiTerm parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}

MpsGlobalType *MpsParser::Gtype(const parsetree *tree) // {{{
{
  if (tree->Case() == "gtype_term") // Gterm {{{
  { return Gterm(tree->Child(0));
  } // }}}
  else if (tree->Case() == "gtype_act") // Gact Gtype {{{
  { MpsGlobalType *succ = Gtype(tree->Child(1));
    return Gact(tree->Child(0),succ);
  } // }}}

  throw string("Unknown Gtype parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
MpsGlobalType *MpsParser::Gact(const parsetree *tree, MpsGlobalType *succ) { // {{{
  if (tree->Case() == "gact_msg") // int -> int : Mtype NamedAssertion ; {{{
  { MpsMsgType *msgtype = Mtype(tree->Child(4));
    int from = stoi(tree->Child(0)->Token().Content());
    int to = stoi(tree->Child(2)->Token().Content());
    bool a_used=false;
    string a_name;
    MpsExp *assertion = NamedAssertion(tree->Child(5),a_used, a_name);
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
  if (tree->Case() == "gact_rec") // rec gvar Targs ; {{{
  { 
    string name = tree->Child(1)->Token().Content();
    vector<TypeArg> args;
    Targs(tree->Child(2),args);
    MpsGlobalRecType *result = new MpsGlobalRecType(name,*succ, args);
    delete succ;
    return result;
  } // }}}
  if (tree->Case() == "gact_tmsg") // int : avar Modal ; {{{
  { bool linear=Modal(tree->Child(3));
    int from = stoi(tree->Child(0)->Token().Content());
    MpsGlobalType *result = new MpsGlobalTypeMsgType(from,tree->Child(2)->Token().Content(), *succ, linear);
    delete succ;
    return result;
  } // }}}

  throw string("Unknown Gact parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
MpsGlobalType *MpsParser::Gterm(const parsetree *tree) { // {{{
  if (tree->Type() == "Gterm" && tree->Case() == "gterm_end") // $end ; {{{
  { return new MpsGlobalEndType();
  } // }}}
  else if (tree->Case() == "gterm_call") // gvar Tvals ; {{{
  { vector<MpsExp*> args;
    Tvals(tree->Child(1),args);
    MpsGlobalVarType *result = new MpsGlobalVarType(tree->Child(0)->Token().Content(), args);
    // Clean Up
    DeleteVector(args);
    return result;
  } // }}}
  else if (tree->Case() == "gterm_branch") // int -> int { Gbranches } {{{
  { int from = string2int(tree->Child(0)->Token().Content());
    int to = string2int(tree->Child(2)->Token().Content());
    map<string,MpsGlobalType*> branches;
    map<string,MpsExp*> assertions;
    Gbranches(tree->Child(4),branches,assertions);
    MpsGlobalType *result = new MpsGlobalBranchType(from,to,branches,assertions);
    // Clean up
    DeleteMap(branches);
    DeleteMap(assertions);
    return result;
  } // }}}
  else if (tree->Case() == "gterm_synch") // { Gbranches } {{{
  { map<string,MpsGlobalType*> branches;
    map<string,MpsExp*> assertions;
    Gbranches(tree->Child(1),branches,assertions);
    MpsGlobalSyncType *result = new MpsGlobalSyncType(branches,assertions);
    // Clean up
    DeleteMap(branches);
    DeleteMap(assertions);
    return result;
  } // }}}

  throw string("Unknown Gterm parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
void MpsParser::Gbranches(const parsetree *tree, map<string,MpsGlobalType*> &dest, map<string,MpsExp*> &assertions) // {{{
{
  if (tree->Case() == "gbranches_end") // bid Assertion : Gtype {{{
  {
    dest[tree->Child(0)->Token().Content()] = Gtype(tree->Child(3));
    assertions[tree->Child(0)->Token().Content()] = Assertion(tree->Child(1));
    return;
  } // }}}
  else if (tree->Type() == "Gbranches" && tree->Case() == "gbranches_cont") // bid Assertion COLON Gtype Gbranches {{{
  {
    dest[tree->Child(0)->Token().Content()] = Gtype(tree->Child(3));
    assertions[tree->Child(0)->Token().Content()] = Assertion(tree->Child(1));
    Gbranches(tree->Child(4),dest,assertions);
    return;
  } // }}}

  throw string("Unknown Gbranches parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}

MpsLocalType *MpsParser::Ltype(const parsetree *tree) // {{{
{
  if (tree->Case() == "ltype_term") // Lterm {{{
  { return Lterm(tree->Child(0));
  } // }}}
  else if (tree->Case() == "ltype_act") // Lact Ltype {{{
  { MpsLocalType *succ = Ltype(tree->Child(1));
    return Lact(tree->Child(0),succ);
  } // }}}

  throw string("Unknown Ltype parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
MpsLocalType *MpsParser::Lact(const parsetree *tree, MpsLocalType *succ) // {{{
{
  if (tree->Case() == "lact_snd") //  int << Mtype NamedAssertion ; {{{
  {
    MpsMsgType *msgtype = Mtype(tree->Child(2));
    int channel = string2int(tree->Child(0)->Token().Content());
    bool a_used=false;
    string a_name;
    MpsExp *assertion = NamedAssertion(tree->Child(3),a_used, a_name);
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
  else if (tree->Case() == "lact_rcv") //  int >> Mtype NamedAssertion ; {{{
  {
    MpsMsgType *msgtype = Mtype(tree->Child(2));
    int channel = string2int(tree->Child(0)->Token().Content());
    bool a_used=false;
    string a_name;
    MpsExp *assertion = NamedAssertion(tree->Child(3),a_used, a_name);
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
  else if (tree->Case() == "lact_forall") //  forall id where Exp ; {{{
  {
    string name=tree->Child(1)->Token().Content();
    MpsExp *assertion=Exp(tree->Child(3));
    MpsLocalForallType *result=new MpsLocalForallType(name,*assertion,*succ);
    delete succ;
    delete assertion;
    return result;
  } // }}}
  else if (tree->Case() == "lact_tsnd") //  << avar Modal ; {{{
  { bool linear=Modal(tree->Child(2));
    MpsLocalType *result = new MpsLocalTypeSendType(tree->Child(1)->Token().Content(),*succ,linear);
    delete succ;
    return result;
  } // }}}
  else if (tree->Case() == "lact_trcv") //  >> avar Modal ; {{{
  { bool linear=Modal(tree->Child(2));
    MpsLocalType *result = new MpsLocalTypeRcvType(tree->Child(1)->Token().Content(),*succ,linear);
    delete succ;
    return result;
  } // }}}
  else if (tree->Case() == "lact_rec") // rec lvar Targs ; {{{
  {
    string name = tree->Child(1)->Token().Content();
    vector<TypeArg> args;
    Targs(tree->Child(2),args);
    MpsLocalRecType *result = new MpsLocalRecType(name,*succ,args);
    delete succ;
    return result;
  } // }}}
  
  throw string("Unknown Lact parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
MpsLocalType *MpsParser::Lterm(const parsetree *tree) // {{{
{
  if (tree->Case() == "lterm_bsnd") //  int << { Lbranches } {{{
  {
    int channel = string2int(tree->Child(0)->Token().Content());
    map<string,MpsLocalType*> branches;
    map<string,MpsExp*> assertions;
    Lbranches(tree->Child(3),branches,assertions);
    MpsLocalSelectType *result = new MpsLocalSelectType(channel,branches,assertions);
    // Clean up
    DeleteMap(branches);
    DeleteMap(assertions);

    return result;
  } // }}}
  else if (tree->Case() == "lterm_brcv") // int >> { Lbranches } {{{
  {
    int channel = string2int(tree->Child(0)->Token().Content());
    map<string,MpsLocalType*> branches;
    map<string,MpsExp*> assertions;
    Lbranches(tree->Child(3),branches,assertions);
    MpsLocalBranchType *result = new MpsLocalBranchType(channel,branches,assertions);
    // Clean up
    DeleteMap(branches);
    DeleteMap(assertions);

    return result;
  } // }}}
  else if (tree->Case() == "lterm_call") // lvar Tvals ; {{{
  {
    vector<MpsExp*> args;
    Tvals(tree->Child(1),args);
    MpsLocalVarType *result = new MpsLocalVarType(tree->Child(0)->Token().Content(), args);
    // Clean Up
    DeleteVector(args);
    return result;
  } // }}}
  else if (tree->Case() == "lterm_end") // %end {{{
  {
    MpsLocalEndType *result = new MpsLocalEndType();
    return result;
  } // }}}
  else if (tree->Case() == "lterm_sync") // { Lbranches } {{{
  {
    map<string,MpsLocalType*> branches;
    map<string,MpsExp*> assertions;
    Lbranches(tree->Child(1),branches,assertions);
    MpsLocalSyncType *result = new MpsLocalSyncType(branches,assertions);
    // Clean up
    DeleteMap(branches);
    DeleteMap(assertions);

    return result;
  } // }}}

  throw string("Unknown Lterm parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
void MpsParser::Lbranches(const parsetree *tree, map<string,MpsLocalType*> &dest, map<string,MpsExp*> &assertions) // {{{
{
  if (tree->Case() == "lbranches_end") // bid Assertion : Ltype {{{
  {
    dest[tree->Child(0)->Token().Content()] = Ltype(tree->Child(3));
    assertions[tree->Child(0)->Token().Content()]=Assertion(tree->Child(1));
    return;
  } // }}}
  else if (tree->Type() == "Lbranches" && tree->Case() == "case2") // bid Assertion : Ltype , Lbranches {{{
  {
    dest[tree->Child(0)->Token().Content()] = Ltype(tree->Child(3));
    assertions[tree->Child(0)->Token().Content()]=Assertion(tree->Child(1));
    return Lbranches(tree->Child(5),dest,assertions);
  } // }}}

  throw string("Unknown localbranch constructor: ") + tree->Type() + "." + tree->Case();
} // }}}

MpsMsgType *MpsParser::Mtype(const parsetree *tree) // {{{
{
  if (tree->Case() == "mtype_stype") // Stype {{{
  {
    return Stype(tree->Child(0));
  } // }}}
  else if (tree->Case() == "mtype_gtype") // Gtype ( Participants ) {{{
  {
    MpsGlobalType *gtype = Gtype(tree->Child(0));
    vector<MpsParticipant> participants;
    Participants(tree->Child(2),participants);
    if (participants.size()<gtype->GetMaxPid())
      throw string("Wrong participant count in ") + tree->Type() + "." + tree->Case();
    MpsChannelMsgType *result = new MpsChannelMsgType(*gtype,participants);
    // Clean up
    delete gtype;

    return result;
  } // }}}
  else if (tree->Case() == "mtype_avar") // avar Modal {{{
  { bool linear=Modal(tree->Child(1));
    return new MpsVarMsgType(tree->Child(0)->Token().Content(), linear);
  } // }}}
  else if (tree->Case() == "mtype_ltype") // Ltype ( int of Participants ) {{{
  {
    MpsLocalType *ltype = Ltype(tree->Child(0));
    int pid = string2int(tree->Child(2)->Token().Content());
    vector<MpsParticipant> participants;
    Participants(tree->Child(4),participants);
    MpsDelegateMsgType *result = new MpsDelegateLocalMsgType(*ltype,pid,participants);
    // Clean up
    delete ltype;

    return result;
  } // }}}
  else if (tree->Case() == "mtype_ptype") // Gtype ( int of Participants ) {{{
  {
    MpsGlobalType *gtype = Gtype(tree->Child(0));
    int pid = string2int(tree->Child(2)->Token().Content());
    vector<MpsParticipant> participants;
    Participants(tree->Child(4),participants);
    if (participants.size()<gtype->GetMaxPid())
      throw string("Wrong participant count in ") + tree->Type() + "." + tree->Case();
    MpsDelegateGlobalMsgType *result = new MpsDelegateGlobalMsgType(*gtype,pid,participants);

    // Clean up
    delete gtype;

    return result;
  } // }}}

  throw string("Unknown Mtype parsetree: " + tree->Type() + "." + tree->Case());
} // }}}
MpsMsgType *MpsParser::Stype(const parsetree *tree) // {{{
{
  if (tree->Case() == "type_int") // Int {{{
  {
    MpsIntMsgType *result = new MpsIntMsgType();
    return result;
  } // }}}
  if (tree->Case() == "type_unsafeint") // Int {{{
  {
    MpsUnsafeIntMsgType *result = new MpsUnsafeIntMsgType();
    return result;
  } // }}}
  if (tree->Case() == "type_float") // Int {{{
  {
    MpsFloatMsgType *result = new MpsFloatMsgType();
    return result;
  } // }}}
  else if (tree->Case() == "type_string") // String {{{
  {
    MpsStringMsgType *result = new MpsStringMsgType();
    return result;
  } // }}}
  else if (tree->Case() == "type_bool") // Bool {{{
  {
    MpsBoolMsgType *result = new MpsBoolMsgType();
    return result;
  } // }}}
  else if (tree->Case() == "type_tuple") // ( Stypes ) {{{
  {
    vector<MpsMsgType*> elements;
    Stypes(tree->Child(1),elements);
    if (elements.size()==1) // Single type
      return elements[0];
    else
    {
      MpsTupleMsgType *result = new MpsTupleMsgType(elements);
      DeleteVector(elements);
      return result;
    }
  } // }}}

  throw string("Unknown Stype parsetree: " + tree->Type() + "." + tree->Case());
} // }}}
void MpsParser::Stypes(const parsetree *tree, vector<MpsMsgType*> &dest) // {{{
{
  if (tree->Case() == "tuple_none") // {{{
  { return;
  } // }}}
  else if (tree->Case() == "tuple_end") // Stype {{{
  { dest.push_back(Stype(tree->Child(0)));
    return;
  } // }}}
  else if (tree->Case() == "tuple_some") // Stypes2 {{{
  { return Stypes(tree->Child(0),dest);
  } // }}}
  else if (tree->Case() == "tuple_cont") // Stype , Stypes2{{{
  { dest.push_back(Stype(tree->Child(0)));
    return Stypes(tree->Child(2),dest);
  } // }}}

  throw string("Unknown Stypes parsetree: " + tree->Type() + "." + tree->Case());
} // }}}

MpsExp *MpsParser::Exp(const parsetree *tree) // {{{
{
  if (tree->Case() == "exp_lvl") // Exp<number> {{{
  {
    return Exp(tree->Child(0));
  } // }}}
  else if (tree->Case() == "exp_if") // if Exp then Exp else Exp {{{
  {
    MpsExp *cond = Exp(tree->Child(1));
    MpsExp *truebranch = Exp(tree->Child(3));
    MpsExp *falsebranch = Exp(tree->Child(5));
    MpsExp *result = new MpsCondExp(*cond,*truebranch,*falsebranch);
    delete cond;
    delete truebranch;
    delete falsebranch;
    return result;
  } // }}}
  else if (tree->Case() == "exp_eq") // Exp3 = Exp2 {{{
  {
    MpsExp *left = Exp(tree->Child(0));
    MpsExp *right = Exp(tree->Child(2));
    MpsExp *result = new MpsBinOpExp("=", *left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->Case() == "exp_leq") // Exp4 <= Exp3 {{{
  {
    MpsExp *left = Exp(tree->Child(0));
    MpsExp *right = Exp(tree->Child(2));
    MpsExp *result = new MpsBinOpExp("<=", *left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->Case() == "exp_geq") // Exp4 >= Exp3 {{{
  {
    MpsExp *left = Exp(tree->Child(0));
    MpsExp *right = Exp(tree->Child(2));
    MpsExp *result = new MpsBinOpExp(">=", *left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->Case() == "exp_lt") // Exp4 < Exp3 {{{
  {
    MpsExp *left = Exp(tree->Child(0));
    MpsExp *right = Exp(tree->Child(2));
    MpsExp *result = new MpsBinOpExp("<", *left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->Case() == "exp_gt") // Exp4 > Exp3 {{{
  {
    MpsExp *left = Exp(tree->Child(0));
    MpsExp *right = Exp(tree->Child(2));
    MpsExp *result = new MpsBinOpExp(">", *left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->Case() == "exp_plus") // Exp5 + Exp4 {{{
  {
    MpsExp *left = Exp(tree->Child(0));
    MpsExp *right = Exp(tree->Child(2));
    MpsExp *result = new MpsBinOpExp("+",*left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->Case() == "exp_minus") // Exp6 - Exp5 {{{
  {
    MpsExp *left = Exp(tree->Child(0));
    MpsExp *right = Exp(tree->Child(2));
    MpsExp *result = new MpsBinOpExp("-",*left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->Case() == "exp_mult") // Exp7 * Exp6 {{{
  {
    MpsExp *left = Exp(tree->Child(0));
    MpsExp *right = Exp(tree->Child(2));
    MpsExp *result = new MpsBinOpExp("*",*left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->Case() == "exp_div") // Exp8 / Exp7 {{{
  {
    MpsExp *left = Exp(tree->Child(0));
    MpsExp *right = Exp(tree->Child(2));
    MpsExp *result = new MpsBinOpExp("/",*left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->Case() == "exp_and") // Exp9 and Exp8 {{{
  {
    MpsExp *left = Exp(tree->Child(0));
    MpsExp *right = Exp(tree->Child(2));
    MpsExp *result = new MpsBinOpExp("and", *left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->Case() == "exp_or") // Exp10 or Exp9 {{{
  {
    MpsExp *left = Exp(tree->Child(0));
    MpsExp *right = Exp(tree->Child(2));
    MpsExp *result = new MpsBinOpExp("or",*left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->Case() == "exp_amp") // Exp10 & int {{{
  {
    MpsExp *left=Exp(tree->Child(0));
    mpz_t val;
    mpz_init_set_str(val,tree->Child(2)->Token().Content().c_str(),10);
    MpsExp *right=new MpsIntVal(val);
    mpz_clear(val);
    MpsExp *result = new MpsBinOpExp("&", *left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  else if (tree->Case() == "exp_not") // not Exp12 {{{
  {
    MpsExp *sub = Exp(tree->Child(1));
    MpsExp *result = new MpsUnOpExp("not",*sub);
    delete sub;
    return result;
  } // }}}
  else if (tree->Case() == "exp_id") // id {{{
  {
    return new MpsVarExp(tree->Child(0)->Token().Content(),MpsMsgNoType());
  } // }}}
  else if (tree->Case() == "exp_int") // int {{{
  {
    mpz_t val;
    string strval=tree->Child(0)->Token().Content();
    if (strval[0]=='~')
      strval[0]='-';
    mpz_init_set_str(val,strval.c_str(),10);
    MpsExp *exp=new MpsIntVal(val);
    mpz_clear(val);
    return exp;
  } // }}}
  else if (tree->Case() == "exp_uint") // int {{{
  {
    long int val;
    string strval=tree->Child(0)->Token().Content();
    if (strval[0]=='~')
      strval[0]='-';
    val=atol(strval.c_str());
    MpsExp *exp=new MpsUnsafeIntVal(val);
    return exp;
  } // }}}
  else if (tree->Case() == "exp_float") // float {{{
  {
    mpf_t val;
    string strval=tree->Child(0)->Token().Content();
    if (strval[0]=='"')
      strval[0]='-';
    mpf_init_set_str(val,strval.c_str(),10);

    MpsExp *exp=new MpsFloatVal(val);
    mpf_clear(val);
    return exp;
  } // }}}
  else if (tree->Case() == "exp_str") // string {{{
  {
    return new MpsStringVal(unwrap_string(tree->Child(0)->Token().Content()));
  } // }}}
  else if (tree->Case() == "exp_true") // true {{{
  {
    return new MpsBoolVal(true);
  } // }}}
  else if (tree->Case() == "exp_false") // false {{{
  {
    return new MpsBoolVal(false);
  } // }}}
  else if (tree->Case() == "exp_tuple") // ( Exps ) {{{
  {
    vector<MpsExp*> elements;
    elements.clear();
    Exps(tree->Child(1),elements);
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
  else if (tree->Case() == "exp_sys") // system & string {{{
  {
    return new MpsSystemExp(unwrap_string(tree->Child(2)->Token().Content()));
  } // }}}

  throw string("Unknown Exp-parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
void MpsParser::Exps(const parsetree *tree, vector<MpsExp*> &dest) // {{{
{
  if (tree->Case() == "tuple_some") // Exps2
    return Exps(tree->Child(0),dest);
  else if (tree->Case() == "tuple_none") //
    return;
  else if (tree->Case() == "tuple_end") // Exp
  { dest.push_back(Exp(tree->Child(0)));
    return;
  }
  else if (tree->Case() == "tuple_cons") // Exp , Exps2
  { dest.push_back(Exp(tree->Child(0)));
    Exps(tree->Child(2),dest);
    return;
  }

  throw string("Unknown Exps parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}

void MpsParser::Ids(const parsetree *tree, vector<string> &dest) // {{{
{
  if (tree->Case() == "ids_cons") // id , Ids2
  { dest.push_back(tree->Child(0)->Token().Content());
    Ids(tree->Child(2),dest);
    return;
  }
  else if (tree->Case() == "ids_end") // id
  {
    dest.push_back(tree->Child(0)->Token().Content());
    return;
  }

  throw string("Unknown Ids parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
MpsChannel MpsParser::Channel(const parsetree *tree) // {{{
{
  if (tree->Case() == "ch") // id [ int ]
    return MpsChannel(tree->Child(0)->Token().Content(),stoi(tree->Child(2)->Token().Content()));
  throw string("Unknown Ch parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
MpsParticipant MpsParser::Participant(const dpl::parsetree *tree) // {{{
{ // participant ::= int Mode
  std::string id=tree->Child(0)->Token().Content();
  return MpsParticipant(string2int(id),id,Mode(tree->Child(1),false));
 } // }}}
void MpsParser::Participants(const parsetree *tree, vector<MpsParticipant> &dest) // {{{
{
  if (tree->Case() == "participants_end") // Participant {{{
  { dest.push_back(Participant(tree->Child(0)));
    return;
  } // }}}
  if (tree->Case() == "participants_cont") // Participant , Participants {{{
  {
    dest.push_back(Participant(tree->Child(0)));
    return Participants(tree->Child(2), dest);
  } // }}}

  throw string("Unknown participants parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
MpsExp *MpsParser::Assertion(const parsetree *tree) // {{{
{ 
  if (tree->Case() == "ass_some") // req exp {{{
  {
    return Exp(tree->Child(1));
  } // }}}
  else if (tree->Case() == "ass_none") // {{{
  {
    return new MpsBoolVal(true);
  } // }}}

  throw string("Unknown Assertion parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
MpsExp *MpsParser::NamedAssertion(const parsetree *tree, bool &used, std::string &name) // {{{
{
  if (tree->Case() == "name") // as id Assertion {{{
  {
    used=true;
    name=tree->Child(1)->Token().Content();
    return Assertion(tree->Child(2));
  } // }}}
  else if (tree->Case() == "noname") // {{{
  {
    used=false;
    return new MpsBoolVal(true);
  } // }}}

  throw string("Unknown NamedAssertion parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
void MpsParser::Branch(const parsetree *tree, map<string,MpsTerm*> &terms,  map<string,MpsExp*> &assertions) // {{{
{
  if (tree->Case() == "branch") // bid Assertion COLON pi
  {
    terms[tree->Child(0)->Token().Content()] = Pi(tree->Child(3));
    assertions[tree->Child(0)->Token().Content()] = Assertion(tree->Child(1));
    return;
  }

  throw string("Unknown branch constructor: ") + tree->Type() + "." + tree->Case();
} // }}}
void MpsParser::Branches(const parsetree *tree, map<string,MpsTerm*> &terms, map<string,MpsExp*> &assertions) // {{{
{
  if (tree->Case() == "branches_end") // Branch
    return Branch(tree->Child(0), terms, assertions);
  else if (tree->Case() == "branches_cont") // Branch Branches
  { Branch(tree->Child(0), terms, assertions);
    return Branches(tree->Child(1), terms, assertions);
  }

  throw string("Unknown branches constructor: ") + tree->Type() + "." + tree->Case();
} // }}}
void MpsParser::InputBranch(const parsetree *tree, map<string,inputbranch> &dest) // {{{
{
  if (tree->Case() == "inputbranch") // bid Assertion ( Targs2 ) COLON Pi
  {
    inputbranch branch;
    branch.names.clear();
    branch.args.clear();
    branch.types.clear();
    branch.values.clear();
    Targs(tree->Child(3),branch.args,branch.types,branch.values);
    branch.names = branch.args;
    branch.term = Pi(tree->Child(6));
    branch.assertion=Assertion(tree->Child(1));
    dest[tree->Child(0)->Token().Content()] = branch;
    return;
  }

  throw string("Unknown inputbranch constructor: ") + tree->Type() + "." + tree->Case();
} // }}}
void MpsParser::InputBranches(const parsetree *tree, map<string,inputbranch> &dest) // {{{
{
  if (tree->Case() == "inputbranches_end") // inputbranch
    return InputBranch(tree->Child(0), dest);
  else if (tree->Case() == "inputbranches_cont") // inputbranch inputbranches
  {
    InputBranch(tree->Child(0), dest);
    return InputBranches(tree->Child(1),dest);
  }

  throw string("Unknown inputbranches constructor: ") + tree->Type() + "." + tree->Case();
} // }}}
void MpsParser::Args(const parsetree *tree, vector<string> &args, vector<MpsMsgType*> &types) // {{{
{
  if (tree->Case() == "args_some") // Args2
    return Args(tree->Child(0),args,types);
  else if (tree->Case() == "args_none") // 
    return; // Do nothing
  else if (tree->Case() == "args_end") // Mtype id
  {
    args.push_back(tree->Child(1)->Token().Content());
    types.push_back(Mtype(tree->Child(0)));
    return;
  }
  else if (tree->Case() == "args_cons") // Mtype id  , Args2
  {
    args.push_back(tree->Child(1)->Token().Content());
    types.push_back(Mtype(tree->Child(0)));
    return Args(tree->Child(3),args,types);
  }

  throw string("Unknown Args constructor: ") + tree->Type() + "." + tree->Case();
} // }}}
void MpsParser::DArgs(const parsetree *tree, vector<string> &args, vector<MpsMsgType*> &types) // {{{
{
  if (tree->Case() == "dargs_some") // < Args2 >
    return Args(tree->Child(1),args,types);
  else if (tree->Case() == "dargs_none") // 
    return; // Do nothing

  throw string("Unknown Dargs constructor: ") + tree->Type() + "." + tree->Case();
} // }}}
void MpsParser::DExps(const parsetree *tree, vector<MpsExp*> &exps) // {{{
{
  if (tree->Case() == "dexps_some") // < Exps >
    return Exps(tree->Child(1),exps);
  else if (tree->Case() == "dexps_none") // 
    return; // Do nothing

  throw string("Unknown Dexps constructor: ") + tree->Type() + "." + tree->Case();
} // }}}
bool MpsParser::Mode(const dpl::parsetree *tree, bool d) // {{{
{ // mode ::= | pure | impure
  if (tree->Case() == "mode_default")
    return d;
  else if (tree->Case() == "mode_pure") // pure
    return true;
  else if (tree->Case() == "mode_impure") // impure
    return false;

  throw std::string("Unknown mode parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
MpsTerm *MpsParser::Send(const parsetree *tree, MpsChannel dest, MpsTerm *succ) // {{{
{
  if (tree->Case() == "send_val") // << Exp {{{
  { MpsExp *value = Exp(tree->Child(1));
    MpsTerm *result = new MpsSnd(dest, *value, *succ, MpsMsgNoType(), false);
    delete value;
    delete succ;
    return result;
  } // }}}
  else if (tree->Case() == "send_lbl") // << bid {{{
  { MpsTerm *result = new MpsSelect(dest, tree->Child(1)->Token().Content(), *succ, false);
    delete succ;
    return result;
  } // }}}

  throw string("Unknown Send parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
MpsTerm *MpsParser::Sends(const parsetree *tree, MpsChannel dest, MpsTerm *succ) // {{{
{
  if (tree->Case() == "sends_end") // send {{{
  { return Send(tree->Child(0),dest,succ);
  } // }}}
  else if (tree->Case() == "sends_cont") // send sends {{{
  { MpsTerm *tmp=Sends(tree->Child(1),dest,succ);
    return Send(tree->Child(0),dest,tmp);
  } // }}}

  throw string("Unknown Sends parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
MpsTerm *MpsParser::Recv(const parsetree *tree, MpsChannel dest, MpsTerm *succ) // {{{
{
  if (tree->Case() == "recv_val") // >> id {{{
  { MpsTerm *result = new MpsRcv(dest, tree->Child(1)->Token().Content(), -1, -1, *succ, MpsMsgNoType(), false);

    delete succ;
    return result;
  } // }}}
  else if (tree->Case() == "recv_ses") // >> id ( int of int ) {{{
  { MpsTerm *result = new MpsRcv(dest,
                                 tree->Child(1)->Token().Content(),
                                 string2int(tree->Child(3)->Token().Content()),
                                 string2int(tree->Child(5)->Token().Content()),
                                 *succ,
                                 MpsMsgNoType(),
                                 false);
    delete succ;
    return result;
  } // }}}

  throw string("Unknown Recv parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
MpsTerm *MpsParser::Recvs(const parsetree *tree, MpsChannel dest, MpsTerm *succ) // {{{
{
  if (tree->Case() == "recvs_end") // recv {{{
  { return Recv(tree->Child(0),dest,succ);
  } // }}}
  else if (tree->Case() == "recvs_cont") // recv recvs {{{
  { MpsTerm *tmp=Recvs(tree->Child(1),dest,succ);
    return Recv(tree->Child(0),dest,tmp);
  } // }}}

  throw string("Unknown Recvs parsetree: ") + tree->Type() + "." + tree->Case();
} // }}}
void MpsParser::Tvals(const parsetree *tree, vector<MpsExp*> &dest) // {{{
{
  if (tree->Case() == "tvals_some") // < Exps > {{{
    return Exps(tree->Child(1), dest); // }}}
  if (tree->Case() == "tvals_none") // epsilon {{{
    return; // }}}

  throw string("Unknown Tvals: ") + tree->Type() + "." + tree->Case();
} // }}}
TypeArg MpsParser::Targ(const parsetree *tree) // {{{
{
  if (tree->Case() == "targ_def") // Mtype id = Exp {{{
  { string argname=tree->Child(1)->Token().Content();
    MpsMsgType *argtype=Mtype(tree->Child(0));
    MpsExp *argvalue=Exp(tree->Child(3));
    TypeArg result(argname,*argtype,*argvalue);
    // clean up
    delete argtype;
    delete argvalue;
    return result;
  } // }}}

  throw string("Unknown Targ constructor: ") + tree->Type() + "." + tree->Case();
} // }}}
void MpsParser::Targ(const parsetree *tree, vector<string> &names, vector<MpsMsgType*> &types, vector<MpsExp*> &values) // {{{
{
  if (tree->Case() == "targ_def") // Mtype id = Exp {{{
  { names.push_back(tree->Child(1)->Token().Content());
    types.push_back(Mtype(tree->Child(0)));
    values.push_back(Exp(tree->Child(3)));
    return;
  } // }}}

  throw string("Unknown Targ constructor: ") + tree->Type() + "." + tree->Case();
} // }}}
void MpsParser::Targs(const parsetree *tree, vector<TypeArg> &dest) // {{{
{
  if (tree->Case() == "targs_some") // < Targs2 > OR < Targs3 > {{{
    return Targs(tree->Child(1), dest); // }}}
  else if (tree->Case() == "targs_none") // epsilon {{{
    return; // }}}
  else if (tree->Case() == "targs_end") // Targ {{{
  { dest.push_back(Targ(tree->Child(0)));
    return;
  } // }}}
  else if (tree->Case() == "targs_cont") // Targ , Targs2 {{{
  { dest.push_back(Targ(tree->Child(0)));
    return Targs(tree->Child(2),dest);
  } // }}}

  throw string("Unknown Targs constructor: ") + tree->Type() + "." + tree->Case();
} // }}}
void MpsParser::Targs(const parsetree *tree, vector<string> &names, vector<MpsMsgType*> &types, vector<MpsExp*> &values) // {{{
{
  if (tree->Case() == "targs_some") // < Targs2 > OR < Targs3 > {{{
    return Targs(tree->Child(1), names, types, values); // }}}
  else if (tree->Case() == "targs_none") // epsilon {{{
    return; // }}}
  else if (tree->Case() == "targs_end") // Targ {{{
    return Targ(tree->Child(0),names,types,values); // }}}
  else if (tree->Case() == "targs_cont") // Targ , Targs2 {{{
  { Targ(tree->Child(0),names,types,values);
    return Targs(tree->Child(2),names,types,values);
  } // }}}

  throw string("Unknown Targs constructor: ") + tree->Type() + "." + tree->Case();
} // }}}
bool MpsParser::Modal(const parsetree *tree) // {{{
{
  if (tree->Case() == "mod_lin") // linear
    return true;
  else if (tree->Case()== "mod_alin") //  alinear
    return false;

  throw string("Unknown Modal constructor: ") + tree->Type() + "." + tree->Case();
} // }}}

MpsTerm *MpsParser::Pi(const std::string &str) // {{{
{
  SlrParser parser("Pi");
  parser.Load(string((char*)bnf_hapi_bnf));
  parsetree *tree = parser.Parse(str);
  MpsTerm *result=Pi(tree);
  delete tree;
  return result;
} // }}}
MpsGlobalType *MpsParser::Gtype(const string &str) // {{{
{
  SlrParser parser("Gtype");
  parser.Load(string((char*)bnf_hapi_bnf));
  parsetree *tree = parser.Parse(str);
  MpsGlobalType *result=Gtype(tree);
  delete tree;
  return result;
} // }}}
MpsLocalType *MpsParser::Ltype(const string &str) // {{{
{
  SlrParser parser("Ltype");
  parser.Load(string((char*)bnf_hapi_bnf));
  parsetree *tree = parser.Parse(str);
  MpsLocalType *result=Ltype(tree);
  delete tree;
  return result;
} // }}}
MpsMsgType *MpsParser::Mtype(const string &str) // {{{
{
  SlrParser parser("Mtype");
  parser.Load(string((char*)bnf_hapi_bnf));
  parsetree *tree = parser.Parse(str);
  MpsMsgType *result=Mtype(tree);
  delete tree;
  return result;
} // }}}
MpsExp *MpsParser::Exp(const std::string &str) // {{{
{
  SlrParser parser("Exp");
  parser.Load(string((char*)bnf_hapi_bnf));
  parsetree *tree = parser.Parse(str);
  MpsExp *result=Exp(tree);
  delete tree;
  return result;
} // }}}
