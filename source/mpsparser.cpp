#include <apims/mpsparser.hpp>
#include <apims/common.hpp>

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
    MpsTerm *succ=PiActs(tree->content[1]);
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
  else if (tree->case_name == "piact_link") // Dlocal id = new id ( int of int ) ; {{{
  { MpsTerm *result = new MpsLink(tree->content[4]->root.content,
                                  tree->content[1]->root.content,
                                  stoi(tree->content[6]->root.content),
                                  stoi(tree->content[8]->root.content),
                                  *succ,
                                  false);
    delete succ;
    return result;
  } // }}}
  else if (tree->case_name == "piact_ch") // Dlocal Gtype id ( Participants ) ; {{{
  { vector<MpsParticipant> participants;
    Participants(tree->content[4],participants);
    if (participants.size()!=type->GetMaxPid())
    { delete succ;
      throw string("Participant count does not match type: ") + tree->type_name + "." + tree->case_name;
    }
    MpsGlobalType *type = Gtype(tree->content[1]);
    MpsTerm *result = new MpsNu(participants,tree->content[2]->root.content, *succ, *type);
    delete succ;
    delete type;
    return result;
  } // }}}
  else if (tree->case_name == "piact_ass") // Dlocal Mtype id = Exp ; {{{
  { MpsMsgType *type = Mtype(tree->content[1]);
    MpsExp *value = Exp(tree->content[4]);
    MpsTerm *result = new MpsAssign(tree->content[2]->root.content, *value, *type, *succ);
    delete type;
    delete value;
    delete succ;
    return result;
  } // }}}
  else if (tree->case_name == "piact_a_ass") // Dlocal id = Exp ; {{{
  { MpsExp *value = Exp(tree->content[4]);
    MpsTerm *result = new MpsAssign(tree->content[2]->root.content, *value, MpsMsgNoType(), *succ);
    delete value;
    delete succ;
    return result;
  } // }}}
  else if (tree->case_name == "piact_def") // Dlocal pvar Dargs ( Args ) ( Pi ) {{{
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
  else if (tree->case_name == "piact_hosts") // host ( Exps ) ; {{{
  { vector<MpsExp*> args;
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

    MpsHostStatement *result = new MpsHostStatement(hostParts,expParts,*succ,vector<MpsMsgType*>());
    // Clean up
    delete succ;
    DeleteVector(expParts);
    DeleteVector(args);
    return result;
  } // }}}
  else if (exp->case_name == "piact_hosth") // hostheader ( Exps ) ; {{{
  { vector<MpsExp*> args;
    Exps(exp->content[2],args);
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

MpsTerm *MpsParser::PiEAct(const parsetree *tree, MpsTerm *succ) // {{{
  if (tree->case_name == "pieact_link") // global id = new id ( int of int ) ; Pi {{{
  { MpsTerm *succ = new Pi(tree->content[11];
    MpsTerm *result = new MpsLink(tree->content[4]->root.content,
                                  tree->content[1]->root.content,
                                  stoi(tree->content[6]->root.content),
                                  stoi(tree->content[8]->root.content),
                                  *succ,
                                  false);
    delete succ;
    return result;
  } // }}}

  if (exp->type_name == "pi" && exp->case_name == "case2") // pi2 {{{
  {
    return MpsTerm::Create(exp->content[0]);
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case1") // ( pi ) {{{
  {
    return MpsTerm::Create(exp->content[1]);
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case2") // end {{{
  {
    return new MpsEnd();
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case3") // pvar dexps ( exps ) {{{
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
  else if (exp->type_name == "pi2" && exp->case_name == "case16") // if Exp then pi2 else pi2 {{{
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
  else if (exp->type_name == "pi2" && exp->case_name == "case17") // ch >> { branches } {{{
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
  else if (exp->type_name == "pi2" && exp->case_name == "case18") // sync ( Exps ) { branches } {{{
  { 
    vector<MpsExp*> args;
    args.clear();
    FindExps(exp->content[2],args);
    // Exps must be int , id
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
  else if (exp->type_name == "pi2" && exp->case_name == "case19") // guisync ( Exps ) { inputbranches } {{{
  { 
    vector<MpsExp*> args;
    args.clear();
    FindExps(exp->content[2],args);
    // Exps must be int , id, int
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

  throw string("Unknown PiEAct parsetree: ") + tree->type_name + "." + tree->case_name;
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
  else if (tree->case_name == "recv_ses") // >> id @ ( int of int ) {{{
  { MpsTerm *result = new MpsRcv(dest,
                                 tree->content[1]->root.content,
                                 string2int(tree->content[4]->root.content),
                                 string2int(tree->content[6]->root.content),
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
  { MpsTerm *tmp=CreateRecvs(tree->content[1],dest,succ);
    return CreateRecv(tree->content[0],dest,tmp);
  } // }}}

  throw string("Unknown Recvs parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsTerm *MpsTerm::Pi(const std::string &str) // {{{
{
  SlrParser parser("pi");
  parser.LoadFile("/opt/apims/bnf/syntax.bnf");
  parsetree *tree = parser.Parse(str);
  MpsTerm *result=MpsTerm::Create(tree);
  delete tree;
  return result;
} // }}}


MpsChannel Channel(const parsetree *tree) // {{{
{
  if (tree->case_name == "ch") // id [ int ]
    return MpsChannel channel(tree->content[0]->root.content,stoi(tree->content[2]->root.content));
  throw string("Unknown Ch parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsExp *NamedAssertion(const parsetree *tree, bool &used, std::string &name) // {{{
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
MpsExp *Assertion(const parsetree *tree) // {{{
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
void MpsParser::Tvals(const parsetree *tree, vector<MpsExp*> &dest) // {{{
{
  if (tree->case_name == "tvals_some") // < Exps > {{{
    return Exps(tree->content[1], dest); // }}}
  if (tree->case_name == "tvals_none") // epsilon {{{
    return; // }}}

  throw string("Unknown Tvals: ") + tree->type_name + "." + tree->case_name;
} // }}}
void Targs(const parsetree *tree, vector<TypeArg> &dest) // {{{
{
  if (tree->case_name == "targs_some") // < Targs2 > {{{
    return Targs2(tree->content[1], dest); // }}}
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
void MpaParser::Exps(const parsetree *exp, vector<MpsExp*> &dest) // {{{
{
  if (exp->case_name == "tuple_some") // Exps2
    return Exps(exp->content[0],dest);
  else if (exp->case_name == "tuple_none") //
    return;
  else if (exp->case_name == "tuple_end") // Exp
  { dest.push_back(Exp(exp->content[0]));
    return;
  }
  else if (exp->case_name == "tuple_cons") // Exp , Exps2
  { dest.push_back(Exp(exp->content[0]));
    Exps(exp->content[2],dest);
    return;
  }

#if APIMS_DEBUG_LEVEL>1
    cerr << "Unknown Exps constructor: " << exp->type_name << " . " << exp->case_name << endl;
#endif
  return;
} // }}}

void GBranches(const parsetree *tree, map<string,MpsGlobalType*> &dest, map<string,MpsExp*> &assertions) // {{{
{
  if (tree->type_name == "Gbranches" && tree->case_name == "gbranches_end") // bid Assertion : Gtype {{{
  {
    dest[tree->content[0]->content[0]->root.content] = MpsGlobalType::Create(tree->content[2]);
    SetAssertion(tree->content[0],assertions);
  } // }}}
  else if (tree->type_name == "Gbranches" && tree->case_name == "gbranches_cont") // bid Assertion COLON Gtype , Gbranches {{{
  {
    dest[tree->content[0]->content[0]->root.content] = Gtype(tree->content[2]);
    SetAssertion(tree->content[0]->content[1],assertions);
    Gbranches(tree->content[4],dest,assertions);
  } // }}}

#if APIMS_DEBUG_LEVEL>1
    cerr << "Unknown Gbranches constructor: " << tree->type_name << "." << tree->case_name << endl;
#endif
} // }}}
MpsGlobalType *MpsParser::Gtype(const parsetree *tree) // {{{
{
  if (tree->case_name == "gtype_term") // Gterm {{{
  { return Gterm(tree->content[0]);
  } // }}}
  else if (tree->case_name == "gtype_act") // Gact ; Gtype {{{
  { MpsGlobalType *succ = Gtype(tree->content[2]);
    return Gact(tree->content[0],succ);
  } // }}}

  throw string("Unknown Gtype parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsGlobalType *MpsParser::Gterm(const parsetree *tree) { // {{{
  if (tree->type_name == "Gterm" && tree->case_name == "gterm_end") // Gend ; {{{
  { return new MpsGlobalEndType();
  } // }}}
  else if (tree->type_name == "Gterm" && tree->case_name == "gterm_call") // gvar Tvals ; {{{
  { vector<MpsExp*> args;
    Tvals(tree->content[1],args);
    MpsGlobalVarType *result = new MpsGlobalVarType(tree->content[0]->root.content, args);
    // Clean Up
    DeleteVector(args);
    return result;
  } // }}}
  else if (tree->type_name == "Gterm" && tree->case_name == "gterm_branch") // int -> int { Gbranches } {{{
  { int from = string2int(tree->content[0]->root.content);
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
  else if (tree->type_name == "Gterm" && tree->case_name == "gterm_synch") // { Gbranches } {{{
  { map<string,MpsGlobalType*> branches;
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

  throw string("Unknown Gterm parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsGlobalType *MpsParser::Gact(const parsetree *tree, MpsGlobalType *succ) { // {{{
  if (tree->type_name == "Gact" && tree->case_name == "gact_msg") // int -> int < Mtype > NamedAssertion {{{
  { MpsMsgType *msgtype = Mtype(tree->content[4]);
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
    // Clean Up
    delete assertion;
    delete msgtype;
    delete succ;
    return result;
  } // }}}
  if (tree->type_name == "Gact" && tree->case_name == "gact_rec") // rec gvar Targs {{{
  { 
    string name = tree->content[1]->root.content;
    vector<TypeArg> args;
    args.clear();
    CreateStateArgs(tree->content[2],args);
    MpsGlobalRecType *result = new MpsGlobalRecType(name,*succ, args);
    delete succ;
    return result;
  } // }}}

  throw string("Unknown Gact parsetree: ") + tree->type_name + "." + tree->case_name;
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

void Lbranches(const parsetree *tree, map<string,MpsLocalType*> &dest, map<string,MpsExp*> &assertions) // {{{
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
MpsLocalType *MpsParser::Ltype(const parsetree *tree) // {{{
{
  if (tree->case_name == "ltype_term") // Lterm {{{
  { return Lterm(tree->content[0]);
  } // }}}
  else if (tree->case_name == "ltype_act") // Lact ; Ltype {{{
  { MpsLocalType *succ = Ltype(tree->content[2]);
    return Lact(tree->content[0],succ);
  } // }}}

  throw string("Unknown Ltype parsetree: ") + tree->type_name + "." + tree->case_name;
} // }}}
MpsLocalType *MpsParser::Lterm(const parsetree *tree) // {{{
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
  else if (tree->case_name == "lterm_call") // lvar Tvals {{{
  {
    vector<MpsExp*> args;
    Tvals(tree->content[1],args);
    MpsLocalVarType *result = new MpsLocalVarType(tree->content[0]->root.content, args);
    // Clean Up
    DeleteVector(args);
    return result;
  } // }}}
  else if (tree->case_name == "lterm_end") // Lend {{{
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
MpsLocalType *MpaParser::Lact(const parsetree *tree, MpsLocalType *succ) // {{{
{
  if (tree->case_name == "lact_snd") //  int << < Mtype > NamedAssertion {{{
  {
    MpsMsgType *msgtype = Mtype(tree->content[3]);
    int channel = string2int(tree->content[0]->root.content);
    bool a_used=false;
    string a_name;
    MpsExp *assertion = NamedAssertion(tree->content[5],a_used, a_name);
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
  else if (tree->case_name == "lact_rcv") //  int >> < Mtype > NamedAssertion {{{
  {
    MpsMsgType *msgtype = PT2Mtype(tree->content[3]);
    int channel = string2int(tree->content[0]->root.content);
    bool a_used=false;
    string a_name;
    MpsExp *assertion = NamedAssertion(tree->content[5],a_used, a_name);
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
  else if (tree->case_name == "lact_forall") //  forall id where Exp {{{
  {
    string name=tree->content[1]->root.content;
    MpsExp *assertion=Exp(tree->content[3]);
    MpsLocalForallType *result=new MpsLocalForallType(name,*assertion,*succ);
    delete succ;
    delete assertion;
    return result;
  } // }}}
  else if (tree->case_name == "lact_rec") // rec lvar Targs {{{
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
MpsLocalType *MpsParser::Ltype(const string &str) // {{{
{
  SlrParser parser("Ltype");
  parser.LoadFile("/opt/apims/bnf/syntax.bnf");
  parsetree *tree = parser.Parse(str);
  MpsLocalType *result=Ltype(tree);
  delete tree;
  return result;
} // }}}

MpsParticipant Participant(const dpl::parsetree &exp) // {{{
{ // participant ::= mode int
  std::string id=exp.content[1]->root.content;
  return MpsParticipant(string2int(id),id,Mode(*exp.content[0],false));
 } // }}}
bool MpsParser::Mode(const dpl::parsetree &exp, bool d) // {{{
{ // mode ::= | pure | impure
  if (exp.case_name == "mode_default")
    return d;
  else if (exp.case_name == "mode_pure") // pure
    return true;
  else if (exp.case_name == "mode_impure") // impure
    return false;

  throw std::string("Unknown mode parsetree: ") + exp.type_name + "." + exp.case_name;
} // }}}
void MpsParser::Participants(const parsetree *exp, vector<MpsParticipant> &dest) // {{{
{
  if (exp->case_name == "participants_end") // participant {{{
  { dest.push_back(Participant(*exp->content[0]));
    return;
  } // }}}
  if (exp->type_name == "participants" && exp->case_name == "case2") // participant , participants {{{
  {
    dest.push_back(Participant(*exp->content[0]));
    return Participants(exp->content[2], dest);
  } // }}}

  throw string("Unknown participants parsetree: ") + exp->type_name + "." + exp->case_name;
} // }}}
void MpsParser::Stypes(const parsetree *tree, vector<MpsMsgType*> &dest) // {{{
{
  if (tree->case_name == "tuple_none") // {{{
  { return;
  } // }}}
  else if (tree->case_name == "tuple_end") // {{{
  { dest.push_back(Stype(tree->content[0]));
    return;
  } // }}}
  else if (tree->case_name == "tuple_some") // {{{
  { return Stypes(tree->content[0],dest);
  } // }}}
  else if (tree->case_name == "tuple_cont") // {{{
  { dest.push_back(Stype(tree->content[0]));
    return Stypes(tree->content[2],dest);
  } // }}}

  throw string("Unknown Stypes parsetree: " + tree->type_name + "." + tree->case_name);
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
    elements.clear();
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
MpsMsgType *MpsParser::Mtype(const parsetree *tree) // {{{
{
  if (tree->case_name == "mtype_stype") // Stype {{{
  {
    return Stype(tree->content[0]);
  } // }}}
  else if (tree->case_name == "mtype_gtype") // { participants } @ Gtype {{{
  {
    MpsGlobalType *gtype = Gtype(tree->content[4]);
    vector<MpsParticipant> participants;
    Participants(tree->content[1],participants);
    if (participants.size()!=gtype->GetMaxPid())
      throw string("Wrong participant count in ") + tree->type_name << "." tree->case_name;
    MpsChannelMsgType *result = new MpsChannelMsgType(*gtype,participants);
    // Clean up
    delete gtype;

    return result;
  } // }}}
  else if (tree->case_name == "mtype_ltype") // ( int of { participants } ) @ Ltype {{{
  {
    MpsLocalType *ltype = Ltype(tree->content[8]);
    int pid = string2int(tree->content[1]->root.content);
    vector<MpsParticipant> participants;
    Participants(tree->content[4],participants);
    MpsDelegateMsgType *result = new MpsDelegateLocalMsgType(*ltype,pid,participants);
    // Clean up
    delete ltype;

    return result;
  } // }}}
  else if (tree->case_name == "mtype_ptype") // ( int of { participants } ) @ Gtype {{{
  {
    MpsGlobalType *gtype = Gtype(tree->content[8]);
    int pid = string2int(tree->content[1]->root.content);
    vector<MpsParticipant> participants;
    Participants(tree->content[4],participants);
    if (participants.size()!=gtype->GetMaxPid())
      throw string("Wrong participant count in ") + tree->type_name << "." tree->case_name;
    MpsDelegateGlobalMsgType *result = new MpsDelegateGlobalMsgType(*gtype,pid,participants);

    // Clean up
    delete gtype;

    return result;
  } // }}}

  throw string("Unknown Mtype parsetree: " + tree->type_name + "." + tree->case_name);
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

MpsExp *MpsParser::Exp(const parsetree *exp) // {{{
{
  if (exp->case_name == "exp_lvl") // Exp<number> {{{
  {
    return Exp(exp->content[0]);
  } // }}}
  if (exp->case_name == "exp_if") // if Exp then Exp else Exp {{{
  {
    MpsExp *cond = MpsExp::Create(exp->content[1]);
    MpsExp *truebranch = MpsExp::Create(exp->content[3]);
    MpsExp *falsebranch = MpsExp::Create(exp->content[5]);
    MpsExp *result = new MpsCondExp(*cond,*truebranch,*falsebranch);
    delete cond;
    delete truebranch;
    delete falsebranch;
    return result;
  } // }}}
  if (exp->case_name == "exp_eq") // Exp3 = Exp2 {{{
  {
    MpsExp *left = Exp(exp->content[0]);
    MpsExp *right = Exp(exp->content[2]);
    MpsExp *result = new MpsBinOpExp("=", *left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  if (exp->case_name == "exp_leq") // Exp4 <= Exp3 {{{
  {
    MpsExp *left = Exp(exp->content[0]);
    MpsExp *right = Exp(exp->content[2]);
    MpsExp *result = new MpsBinOpExp("<=", *left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  if (exp->case_name == "exp_plus") // Exp5 + Exp4 {{{
  {
    MpsExp *left = Exp(exp->content[0]);
    MpsExp *right = Exp(exp->content[2]);
    MpsExp *result = new MpsBinOpExp("+",*left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  if (exp->case_name == "exp_minus") // Exp6 - Exp5 {{{
  {
    MpsExp *left = Exp(exp->content[0]);
    MpsExp *right = Exp(exp->content[2]);
    MpsExp *result = new MpsBinOpExp("-",*left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  if (exp->case_name == "exp_mult") // Exp7 * Exp6 {{{
  {
    MpsExp *left = Exp(exp->content[0]);
    MpsExp *right = Exp(exp->content[2]);
    MpsExp *result = new MpsBinOpExp("*",*left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  if (exp->case_name == "exp_div") // Exp8 / Exp7 {{{
  {
    MpsExp *left = Exp(exp->content[0]);
    MpsExp *right = Exp(exp->content[2]);
    MpsExp *result = new MpsBinOpExp("/",*left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  if (exp->case_name == "exp_and") // Exp9 and Exp8 {{{
  {
    MpsExp *left = Exp(exp->content[0]);
    MpsExp *right = Exp(exp->content[2]);
    MpsExp *result = new MpsBinOpExp("and", *left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  if (exp->case_name == "exp_or") // Exp10 or Exp9 {{{
  {
    MpsExp *left = Exp(exp->content[0]);
    MpsExp *right = Exp(exp->content[2]);
    MpsExp *result = new MpsBinOpExp("or",*left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  if (exp->case_name == "exp_amp") // Exp10 & int {{{
  {
    MpsExp *left=Exp(exp->content[0]);
    mpz_t val;
    mpz_init_set_str(val,exp->content[2]->root.content.c_str(),10);
    MpsExp *right=new MpsIntVal(val);
    mpz_clear(val);
    MpsExp *result = new MpsBinOpExp("&", *left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  if (exp->case_name == "exp_not") // not Exp12 {{{
  {
    MpsExp *sub = Exp(exp->content[1]);
    MpsExp *result = new MpsUnOpExp("not",*sub);
    delete sub;
    return result;
  } // }}}
  if (exp->case_name == "exp_id") // id {{{
  {
    return new MpsVarExp(exp->content[0]->root.content,MpsMsgNoType());
  } // }}}
  if (exp->case_name == "exp_int") // int {{{
  {
    mpz_t val;
    mpz_init_set_str(val,exp->content[0]->root.content.c_str(),10);
    MpsExp *exp=new MpsIntVal(val);
    mpz_clear(val);
    return exp;
  } // }}}
  if (exp->case_name == "exp_str") // string {{{
  {
    return new MpsStringVal(unwrap_string(exp->content[0]->root.content));
  } // }}}
  if (exp->case_name == "exp_true") // true {{{
  {
    return new MpsBoolVal(true);
  } // }}}
  if (exp->case_name == "exp_false") // false {{{
  {
    return new MpsBoolVal(false);
  } // }}}
  if (exp->case_name == "exp_tuple") // ( Exps ) {{{
  {
    vector<MpsExp*> elements;
    elements.clear();
    Exps(exp->content[1],elements);
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
  if (exp->case_name == "exp_sys") // system & string {{{
  {
    return new MpsSystemExp(unwrap_string(exp->content[2]->root.content));
  } // }}}

  throw string("Unknown Exp-parsetree: " + exp->type_name + "." + exp->case_name;
} // }}}
MpsExp *MpsParser::Exp(const std::string &exp) // {{{
{
  SlrParser parser("Exp");
  parser.LoadFile("/opt/apims/bnf/syntax.bnf");
  parsetree *tree = parser.Parse(str);
  MpsMsgType *result=Exp(tree);
  delete tree;
  return result;
} // }}}

// Helper functions










MpsExp *CreateAssertion(const parsetree *tree) // {{{
{
  if (tree->type_name == "Label" && tree->case_name == "case1") // bid Assertion
    return CreateAssertion(tree->content[1]);
  else if (tree->type_name == "Assertion" && tree->case_name == "case1") // [[ Exp ]]
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
void AddBranch(const parsetree *exp, map<string,MpsTerm*> &dest,  map<string,MpsExp*> &assertions) // {{{
{
  if (exp->type_name == "branch" && exp->case_name == "case1") // bid Assertion COLON pi
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
void FindBranches(const parsetree *exp, map<string,MpsTerm*> &dest, map<string,MpsExp*> &assertions) // {{{
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
void FindArgs(const parsetree *exp, vector<string> &args, vector<MpsMsgType*> &types) // {{{
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
void FindGUIArgs(const parsetree *exp, vector<string> &args, vector<MpsMsgType*> &types, vector<MpsExp*> &values) // {{{
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
void AddInputBranch(const parsetree *exp, map<string,inputbranch> &dest) // {{{
{
  if (exp->type_name == "inputbranch" && exp->case_name == "case1") // bid Assertion ( args ) : pi
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
void FindInputBranches(const parsetree *exp, map<string,inputbranch> &dest) // {{{
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
vector<string> FindNames(const parsetree *names) // {{{
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
void HostStmt(vector<string> &hostParts, vector<MpsExp*> &expParts, const parsetree *hstmt) // {{{
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













void FindMsgTypes(const parsetree *tree, vector<MpsMsgType*> &dest) // {{{
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
MpsGlobalType *MpsGlobalType::Create(const string &str) // {{{
{
  SlrParser parser("Gtype");
  parser.LoadFile("/opt/apims/bnf/syntax.bnf");
  parsetree *tree = parser.Parse(str);
  MpsGlobalType *result=Create(tree);
  delete tree;
  return result;
} // }}}
MpsLocalType *MpsLocalType::Create(const string &str) // {{{
{
  SlrParser parser("Ltype");
  parser.LoadFile("/opt/apims/bnf/syntax.bnf");
  parsetree *tree = parser.Parse(str);
  MpsLocalType *result=Create(tree);
  delete tree;
  return result;
} // }}}
MpsMsgType *MpsMsgType::Create(const string &str) // {{{
{
  SlrParser parser("Mtype");
  parser.LoadFile("/opt/apims/bnf/syntax.bnf");
  parsetree *tree = parser.Parse(str);
  MpsMsgType *result=Create(tree);
  delete tree;
  return result;
} // }}}

