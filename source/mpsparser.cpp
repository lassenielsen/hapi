#include <apims/mpsparser.hpp>
#include <apims/common.hpp>

using namespace apims;

void MpsParser::DefGtype(Parser &parser) // {{{
{ 
  if (parser.IsType("Gtype"))
    return;

  DefExp(parser);
  DefMtype(parser);

  MpsParser.DefKeywordToken("Gend",1);
  MpsParser.DefKeywordToken(";",1);
  MpsParser.DefToken("int", "(~)?[0-9][0-9]*",10);
  MpsParser.DefKeywordToken("->",0);
  MpsParser.DefKeywordToken("<",2);
  MpsParser.DefKeywordToken(">",2);
  MpsParser.DefKeywordToken("rec",1);
  MpsParser.DefToken("gvar", "$[a-zA-Z0-9_][a-zA-Z0-9_]*",10);
  MpsParser.DefToken("COLON",":",1);
  MpsParser.DefKeywordToken("as",1);
  MpsParser.DefToken("id", "[a-z][a-zA-Z0-9_]*",10);
  MpsParser.DefKeywordToken("req",1);
  MpsParser.DefKeywordToken("=",1);
  MpsParser.DefKeywordToken(",",2);
  MpsParser.DefToken("bid", "[#^][a-zA-Z0-9_]*",10);
  MpsParser.DefKeywordToken("{",2);
  MpsParser.DefKeywordToken("}",2);

  parser.DefType("Tvals ::= ::tvals_some < Exps > \
                          | ::tvals_none\
                 ");
  parser.DefType("Targ ::= id COLON Mtype = Exp");
  parser.DefType("Targs ::= ::targs_some < Targs2 > \
                          | ::targs_none \
                 ");
  parser.DefType("Targs2 ::= ::targs2_some Targs3 \
                           | ::targs2_none \
                 ");
  parser.DefType("Targs3 ::= ::targs3_end Targ \
                           | ::targs3_cont Targ , Targs3 \
                 ");
  parser.DefType("Assertion ::= ::ass_some req Exp \
                              | ::ass_none \
                 ");
  parser.DefType("NamedAssertion ::= ::name as id Assertion \
                                   | ::noname \
                 ");
  parser.DefType("Label ::= bid Assertion");
  parser.DefType("Gact ::= ::gact_msg int -> int < Mtype > NamedAssertion \
                         | ::gact_rec rec gvar Targs \
                 ");
  parser.DefType("Gterm ::= ::gterm_end    Gend ; \
                          | ::gterm_call   gvar Tvals ; \
                          | ::gterm_branch int -> int { Gbranches } \
                          | ::gterm_synch  { Gbranches } \
                 ");
  parser.DefType("Gtype ::= ::gtype_term Gterm \
                          | ::gtype_act  Gact ; Gtype
                 ");
  parser.DefType("Gbranches ::= ::gbranches_end  Label COLON Gtype \
                              | ::gbranches_cont Label COLON Gtype Gbranches \
                 ");
} // }}}
MpsGlobalType *MpsParser::Gtype(const parsetree *tree) // {{{
{
  if (tree->type_name == "Gtype" && tree->case_name == "gtype_term") // Gterm {{{
  { return Gterm(tree->content[0]);
  } // }}}
  else if (tree->type_name == "Gtype" && tree->case_name == "gtype_act") // Gact ; Gtype {{{
  { MpsGlobalType *succ = Gtype(tree->content[2]);
    return Gact(tree->content[0],succ);
  } // }}}

#if APIMS_DEBUG_LEVEL>1
  cerr << "Unknown Gtype parsetree: " << tree->type_name << "." << tree->case_name << endl;
#endif
  return new MpsGlobalEndType();
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

#if APIMS_DEBUG_LEVEL>1
  cerr << "Unknown Gterm parsetree: " << tree->type_name << "." << tree->case_name << endl;
#endif
  return new MpsGlobalEndType();
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

#if APIMS_DEBUG_LEVEL>1
  cerr << "Unknown Gact parsetree: " << tree->type_name << "." << tree->case_name << endl;
#endif
  return new MpsGlobalEndType();
} // }}}
MpsGlobalType *MpsParser::Gtype(const string &str) // {{{
{
  SlrParser parser("Gtype");
  DefGtype(parser);
  parsetree *tree = parser.Parse(str);
  MpsGlobalType *result=Gtype(tree);
  delete tree;
  return result;
} // }}}

void MpsParser::DefLtype(Parser &parser) // {{{
{ 
  if (parser.IsType("Ltype"))
    return;

  DefExp(parser);
  DefMtype(parser);

  MpsParser.DefKeywordToken("req",1);
  MpsParser.DefKeywordToken("as",1);
  MpsParser.DefToken("id", "[a-z][a-zA-Z0-9_]*",10);
  MpsParser.DefToken("COLON",":",1);
  MpsParser.DefKeywordToken("=",1);
  MpsParser.DefKeywordToken("<",2);
  MpsParser.DefKeywordToken(">",2);
  MpsParser.DefKeywordToken(",",2);
  MpsParser.DefKeywordToken(";",1);
  parser.DefKeywordToken(">>",1);
  parser.DefKeywordToken("<<",1);
  parser.DefKeywordToken("forall",1);
  MpsParser.DefKeywordToken("{",2);
  MpsParser.DefKeywordToken("}",2);
  parser.DefKeywordToken("Lend",1);

  parser.DefType("Assertion ::= ::ass_some req Exp \
                              | ::ass_none \
                 ");
  parser.DefType("NamedAssertion ::= ::name as id Assertion \
                                   | ::noname \
                 ");
  parser.DefType("Targ ::= id COLON Mtype = Exp");
  parser.DefType("Targs ::= ::targs_some < Targs2 > \
                          | ::targs_none \
                 ");
  parser.DefType("Targs2 ::= ::targs2_some Targs3 \
                           | ::targs2_none \
                 ");
  parser.DefType("Targs3 ::= ::targs3_end Targ \
                           | ::targs3_cont Targ , Targs3 \
                 ");
  parser.DefType("Tvals ::= ::tvals_some < Exps > \
                          | ::tvals_none\
                 ");
  parser.DefType("Lact ::= int << < Mtype > NamedAssertion \
                         | int >> < Mtype > NamedAssertion \
                         | forall id where Exp \
                         | rec lvar Targs \
                 ");
  parser.DefType("Lterm ::= int << { Lbranches } \
                          | int >> { Lbranches } \
                          | lvar Tvals ; \
                          | Lend ; \
                          | { Lbranches } \
                 ");
  parser.DefType("Ltype ::= Lterm \
                          | Lact ; Ltype
                 ");
  parser.DefType("Lbranches ::= Label COLON Ltype \
                              | Label COLON Ltype , Lbranches \
                 ");
} // }}}
MpsLocalType *MpsParser::Ltype(const parsetree *tree) // {{{
{
  if (tree->type_name == "Ltype" && tree->case_name == "case1") //  int << < Mtype > NamedAssertion ; Ltype {{{
  {
    MpsLocalType *succ = PT2Ltype(tree->content[7]);
    MpsMsgType *msgtype = PT2Mtype(tree->content[3]);
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
    MpsLocalType *succ = PT2Ltype(tree->content[7]);
    MpsMsgType *msgtype = PT2Mtype(tree->content[3]);
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
  else if (tree->type_name == "Ltype" && tree->case_name == "case3") //  forall id [[ Exp ]] ; Ltype {{{
  {
    string name=tree->content[1]->root.content;
    MpsExp *assertion=PT2Exp(tree->content[3]);
    MpsLocalType *succ = PT2Ltype(tree->content[6]);
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
    MpsLocalType *succ = PT2Ltype(tree->content[4]);
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
MpsLocalType *MpsParser::Ltype(const string &str) // {{{
{
  SlrParser parser("Ltype");
  DefLtype(parser);
  parsetree *tree = parser.Parse(str);
  MpsLocalType *result=Ltype(tree);
  delete tree;
  return result;
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

    static MpsParticipant Create(const dpl::parsetree &exp) // {{{
    { // participant ::= mode int
      std::string id=exp.content[1]->root.content;
      return MpsParticipant(string2int(id),id,MpsParticipant::CreateMode(*exp.content[0],false));
    } // }}}
    static bool CreateMode(const dpl::parsetree &exp, bool d) // {{{
    { // mode ::= | pure | impure
      if (exp.type_name == "mode" && exp.case_name == "case1")
        return d;
      else if (exp.type_name == "mode" && exp.case_name == "case2") // pure
        return true;
      else if (exp.type_name == "mode" && exp.case_name == "case3") // impure
        return false;
      else throw std::string("MpsParticipant::CreateMode: Unknown pconstructor: ") + exp.type_name + " case: " + exp.case_name;
    
      return d;
    } // }}}

void MpsParser::Participants(const parsetree *exp, vector<MpsParticipant> &dest) // {{{
{
  if (exp->case_name == "participants_end") // participant {{{
  { dest.push_back(MpsParticipant::Create(*exp->content[0]));
    return;
  } // }}}
  if (exp->type_name == "participants" && exp->case_name == "case2") // participant , participants {{{
  {
    dest.push_back(MpsParticipant::Create(*exp->content[0]));
    return FindParticipants(exp->content[2], dest);
  } // }}}
  throw string("FindParticipants: Unknown participants parsetree: ") + exp->type_name + "." + exp->case_name;
} // }}}
void MpsParser::DefMtype(Parser &parser) // {{{
{ 
  if (parser.IsType("Mtype"))
    return;

  DefGtype(parser);
  DefLtype(parser);

  parser.DefKeywordToken("Int",1);
  parser.DefKeywordToken("String",1);
  parser.DefKeywordToken("Bool",1);
  parser.DefKeywordToken("(",2);
  parser.DefKeywordToken(")",2);
  parser.DefKeywordToken("{",2);
  parser.DefKeywordToken("}",2);
  parser.DefKeywordToken(",",2);
  parser.DefToken("int", "(~)?[0-9][0-9]*",10);
  parser.DefKeywordToken("pure",2);
  parser.DefKeywordToken("impure",2);
  parser.DefKeywordToken("of",1);
  parser.DefKeywordToken("@",1);

  parser.DefType("Stype ::= ::type_int    Int \
                          | ::type_string String \
                          | ::type_bool   Bool \
                          | ::type_tuple  ( Stypes ) \
                 ");
  parser.DefType("Stypes ::= ::tuple_some Stypes2 \
                           | ::tuple_none \
                 ");
  parser.DefType("Stypes2 ::= ::tuple_cont Stype , Stypes2 \
                            | ::tuple_end Stype \
                 ");
  parser.DefType("mode ::= ::mode_default \
                         | ::mode_pure pure \
                         | ::mode_impure impure");
  parser.DefType("participant ::= mode int"); // | mode id
  parser.DefType("participants ::= ::participants_end  participant \
                                 | ::participants_cont participant , participants \
                 ");
  parser.DefType("Mtype ::= ::mtype_stype    Stype \
                          | ::mtype_gtype    { participants } @ Gtype \
                          | ::mtype_ltype    ( int of { participants } ) @ Ltype \
                          | ::mtype_projtype ( int of { participants } ) @ Gtype \
                 ");
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
  else if (tree->case_name == "mtype_projtype") // ( int of { participants } ) @ Gtype {{{
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
  DefMtype(parser);
  parsetree *tree = parser.Parse(str);
  MpsMsgType *result=Mtype(tree);
  delete tree;
  return result;
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
void MpsParser::DefExp(Parser &parser) // {{{
{ 
  if (parser.IsType("Exp"))
    return;

  /*** Define Tokens ***/
  MpsParser.DefToken("","[ \n\r\t][ \n\r\t]*",9);
  MpsParser.DefKeywordToken("(",2);
  MpsParser.DefKeywordToken(")",2);
  MpsParser.DefKeywordToken("true",0);
  MpsParser.DefKeywordToken("false",0);
  MpsParser.DefKeywordToken("and",0);
  MpsParser.DefKeywordToken("or",0);
  MpsParser.DefKeywordToken("not",0);
  MpsParser.DefKeywordToken("if",0);
  MpsParser.DefKeywordToken("then",0);
  MpsParser.DefKeywordToken("else",0);
  MpsParser.DefKeywordToken("+",10);
  MpsParser.DefKeywordToken("-",10);
  MpsParser.DefKeywordToken("*",10);
  MpsParser.DefKeywordToken("/",10);
  MpsParser.DefToken("int", "(~)?[0-9][0-9]*",10);
  MpsParser.DefToken("string", "\"[^\"]*\"",10);
  MpsParser.DefToken("id", "[a-z][a-zA-Z0-9_]*",10);
  // Expression Grammar
  parser.DefType("Exp  ::= ::exp_if    if Exp then Exp else Exp | ::exp_lvl Exp2");
  parser.DefType("Exp2 ::= ::exp_eq    Exp3 = Exp2 | ::exp_lvl Exp3");
  parser.DefType("Exp3 ::= ::exp_leq   Exp4 <= Exp3 | ::exp_lvl Exp4");
  parser.DefType("Exp4 ::= ::exp_plus  Exp5 + Exp4 | ::exp_lvl Exp5");
  parser.DefType("Exp5 ::= ::exp_minus Exp6 - Exp5 | ::exp_lvl Exp6");
  parser.DefType("Exp6 ::= ::exp_mult  Exp7 * Exp6 | ::exp_lvl Exp7");
  parser.DefType("Exp7 ::= ::exp_div   Exp8 / Exp7 | ::exp_lvl Exp8");
  parser.DefType("Exp8 ::= ::exp_and   Exp9 and Exp8 | ::exp_lvl Exp9");
  parser.DefType("Exp9 ::= ::exp_or    Exp10 or Exp9 | ::exp_lvl Exp10");
  parser.DefType("Exp10 ::= ::exp_amp   Exp10 & int | ::exp_lvl Exp11");
  parser.DefType("Exp12 ::= ::exp_not   not Exp12 | ::exp_lvl Exp13");
  parser.DefType("Exp13 ::= ::exp_id    id \
                          | ::exp_int   int \
                          | ::exp_str   string \
                          | ::exp_true  true \
                          | ::exp_false false \
                          | ::exp_tuple ( Exps ) \
                          | ::exp_sys   system & string | ::exp_lvl Exp12");
                 ");
  parser.DefType("Exps ::= ::tuple_some Exps2 | ::tuple_none ");
  parser.DefType("Exps2 ::= ::tuple_end Exp | ::tuple_cons Exp , Exps2");
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
  DefExp(parser);
  parsetree *tree = parser.Parse(str);
  MpsMsgType *result=Exp(tree);
  delete tree;
  return result;
} // }}}

/* Parsing of MpsTerms from a string
 */
// OPTIONAL: Add xml-expressions
// FIXME: Implement rand
// FIXME: Add expression operations (<,>,<=,>=,s1++s2,head,tail) [+,-,*,/,and,or,not,=]
void MpsParser::DefPi(Parser &parser) // {{{
{
  if (parser.IsType("Pi"))
    return;
  // Use expressions
  DefExp(parser);
  // Use global types
  DefGtype(parser);
  // Use local types
  DefLtype(parser);
  //parser.DefType(MpsLocalType::BNF_LTYPE);
  //parser.DefType(MpsLocalType::BNF_LBRANCHES);
  // Use message types
  MpsMsgType::AddParserDef(parser);
  //parser.DefType(MpsMsgType::BNF_STYPE);
  //parser.DefType(MpsMsgType::BNF_STYPES);
  //parser.DefType(MpsMsgType::BNF_STYPES2);
  //parser.DefType(MpsMsgType::BNF_MTYPE);
  // Type grammars

  /*** Define Tokens ***/
  // Parenthesis
  parser.DefKeywordToken("(",2);
  parser.DefKeywordToken(")",2);
  // Base expression keywords
  parser.DefKeywordToken("true",0); // Base Value: boolean true
  parser.DefKeywordToken("false",0); // Base Value: boolean false
  parser.DefKeywordToken("and",0); // Base Operation: boolean and
  parser.DefKeywordToken("or",0); // Base Operation: boolean or
  parser.DefKeywordToken("not",0); // Base Operation: boolean not
  parser.DefKeywordToken("if",0);   // Used for if then else expressions and terms
  parser.DefKeywordToken("then",0); // Used for if then else expressions and terms
  parser.DefKeywordToken("else",0); // Used for if then else expressions and terms
  parser.DefKeywordToken("+",10); // Integer Addition
  parser.DefKeywordToken("-",10); // Integer Addition
  parser.DefKeywordToken("*",10); // Integer Addition
  parser.DefKeywordToken("/",10); // Integer Addition
  parser.DefKeywordToken("<=",0); // LEQ
  parser.DefKeywordToken("&",0); // Touple indexing
  parser.DefToken("int", "(~)?[0-9][0-9]*",10); // Base Value: integer
  parser.DefToken("string", "\"[^\"]*\"",10); // Base Value: integer
  // Type keywords
  parser.DefKeywordToken("Int",1);         // Simple type keyword
  parser.DefKeywordToken("String",1);      // Simple type keyword
  parser.DefKeywordToken("Bool",1);        // Simple type keyword
  parser.DefKeywordToken("->",0);          // Communication type
  parser.DefKeywordToken("@",1);           // Used for T @ ( p of n )
  parser.DefKeywordToken("of",1);          // Usex in T @ ( p of n )
  parser.DefKeywordToken("as",1);          // Usex in 1=>2:1<Bool> as x
  parser.DefKeywordToken("forall",1);      // Usex in forall x [[x or y]]
  parser.DefKeywordToken(".",1);           // Used for rec X . G
  parser.DefKeywordToken("rec",1);         // Recursive type keyword
  parser.DefKeywordToken("Lend",1);        // Local type end
  parser.DefKeywordToken("Gend",1);        // Global type end
  parser.DefKeywordToken("<",2);    // Used for message types
  parser.DefKeywordToken(">",2);    // Used for message types
//  parser.DefToken("dvar", "§[a-zA-Z0-9_]+",10);     // Dependant type var
  parser.DefToken("lvar", "%[a-zA-Z0-9_][a-zA-Z0-9_]*",10);     // Local type var
  parser.DefToken("gvar", "$[a-zA-Z0-9_][a-zA-Z0-9_]*",10);   // Global type var
  parser.DefKeywordToken("[[",1);        // Assertion Start
  parser.DefKeywordToken("]]",1);        // Assertion End
  //parser.DefKeywordToken("((",1);        // Double LPar
  //parser.DefKeywordToken("))",1);        // Double RPar
  // Term keywords
  parser.DefKeywordToken(";",1);    // Command separator
  parser.DefKeywordToken("new",0);  // Used to create channels and sessions
  parser.DefKeywordToken("in",1);   // Recursive def
  parser.DefKeywordToken("=",1);    // Used in def
  parser.DefKeywordToken("link",0); // Init Link
  parser.DefKeywordToken("sync",0); // Synchronise
  parser.DefKeywordToken("nu",0);   // Hide
  parser.DefKeywordToken("end",0);  // Terminated Process
  parser.DefKeywordToken(">>",1);   // Receive
  parser.DefKeywordToken("<<",1);   // Send
  parser.DefToken("par","\\|",1);   // Conc
  parser.DefKeywordToken(",,",1);    // Used in branching
  parser.DefKeywordToken(",",2);    // Used in branching
  parser.DefKeywordToken("{",2);    // Used in branching
  parser.DefKeywordToken("}",2);    // Used in branching
  parser.DefToken("COLON",":",1);    // Used in branching
  parser.DefKeywordToken("[",2);    // Used for session channel
  parser.DefKeywordToken("]",2);    // Used for session channel
  parser.DefToken("","[ \n\r\t][ \n\r\t]*",9);            // Whitespace is ignored
  parser.DefToken("","//[^\n]*",9);                // Comments are ignored
  parser.DefToken("id", "[a-z][a-zA-Z0-9_]*",10);   // Variable or channel name
  parser.DefToken("bid", "[#^][a-zA-Z0-9_]*",10);   // Branch id, called label
  parser.DefToken("pvar", "[A-Z][a-zA-Z0-9_]*",10); // Process var
  parser.DefKeywordToken("guisync",2);    // Used for GUI generation
  parser.DefKeywordToken("guivalue",2);   // Used for GUI generation
  parser.DefToken("host", "HOST",2); // Indicate host statement
  parser.DefToken("hostheader", "HOSTHEADER",2); // Indicate host header
  parser.DefToken("system", "SYSTEM",2); // Indicate host header
  parser.DefToken("process", "def", 1); // Init Link
  parser.DefToken("global", "public+global", 1); // Init Link
  parser.DefToken("local", "private+local", 1); // Init Link
  parser.DefToken("session", "ses+session", 1); // Init Link
  parser.DefToken("channel", "ch+channel", 1); // Init Link
  /*** Define grammars ***/
  // Term grammar
  parser.DefType("ch ::= id [ int ]");                          // Session channels
  parser.DefType("branch ::= Label COLON pi");                  // Branch
  parser.DefType("branches ::= branch | branch branches");      // Nonempty Branches list
  parser.DefType("inputbranch ::= Label ( Targs2 ) COLON pi");      // GUIBranch
  parser.DefType("inputbranches ::= inputbranch \
                                     | inputbranch inputbranches"); // Nonempty GUIBranch list
  parser.DefType("args ::= ::args_some args2 | ::args_none");                           // Argument list
  parser.DefType("args2 ::= ::args_last Mtype id | ::args_cons Mtype id , args2");  // Non-empty argument list
  parser.DefType("dargs ::= ::dargs_some < args2 > | ::dargs_none");                       // Optional dependant args
  parser.DefType("dexps ::= ::dexps_some < Exps > | ::dexps_none");                       // Optional dependant args
  parser.DefType("ids ::= | id  ids");                          // Name list
  parser.DefKeywordToken("pure",2);                             // Used for mode declaration
  parser.DefKeywordToken("impure",2);                           // Used for mode declaration
  parser.DefType("mode ::= | pure | impure");                   // Mode declaration used for channel participants
  parser.DefType("participant ::= mode int");                   // Participant def
  parser.DefType("participants ::= participant | participant , participants"); // Mode declaration used for channels and defs
  parser.DefType("send ::= << Exp | << bid");
  parser.DefType("sends ::= send | send sends");
  parser.DefType("recv ::= >> id | >> id @ ( int of int )");
  parser.DefType("recvs ::= recv | recv recvs");
  parser.DefType("dlocal ::= ::dlocal_d \
                              | ::dlocal_l local ");
  parser.DefType("dglobal ::= ::dglobal_d \
                               | ::dglobal_l global ");
  // Processes
  parser.DefType("Pi ::= ::pi_par    stmts par Pi \
                          | ::pi_stmts  stmts \
                          | ::pi_estmts estmts \
                    ");
  // Statements
  parser.DefType("stmt ::= ::stmt_sends ch sends \
                            | ::stmt_recvs ch recvs \
                            | ::stmt_link  dlocal id = new id ( int of int ) \
                            | ::stmt_ch    dlocal Gtype id ( participants ) \
                            | ::stmt_ass   dlocal Mtype id = Exp \
                            | ::stmt_a_ass dlocal id = Exp \
                            | ::stmt_gval  guivalue ( Exps ) \
                            | ::stmt_hosts host ( Exps ) \
                            | ::stmt_hosth hostheader ( Exps ) \
                    ");
  // Terminating statements
  parser.DefType("tstmt ::= ::tstmt_pi   ( Pi ) \
                             | ::tstmt_end  ; \
                             | ::tstmt_def  local pvar dargs ( args ) = stmts stmts \
                             | ::tstmt_call pvar dexps ( Exps ) ; \
                             | ::tstmt_if   if Exp then Pi else stmts \
                             | ::tstmt_br   ch >> { branches } \
                             | ::tstmt_syn  sync ( Exps ) { branches } \
                             | ::tstmt_gsyn guisync ( Exps ) { inputbranches } \
                    ");
  // Statement sequence
  parser.DefType("stmts ::= ::stmts_n tstmt | ::stmts_c stmt ; stmts");
  // Extended statements (for associativity)
  parser.DefType("estmt ::= ::estmt_link  global id = new id ( int of int ) ; Pi \
                             | ::estmt_ch    global Gtype id ( participants ) ; Pi \
                             | ::estmt_def   global pvar dargs ( args ) = stmts Pi \
                             | ::estmt_ass   global Mtype id = Exp ; Pi \
                             | ::estmt_a_ass global id = Exp ; Pi \
                    ");
  parser.DefType("estmts ::= estmt | stmt ; estmts");

} // }}}
MpsTerm *MpsParser::Pi(const parsetree *exp) // {{{
{
  if (exp->type_name == "pi" && exp->case_name == "case1") // pi par pi2 {{{
  {
    MpsTerm *left = MpsTerm::Create(exp->content[0]);
    MpsTerm *right = MpsTerm::Create(exp->content[2]);
    MpsTerm *result = new MpsPar(*left, *right,vector<string>(),vector<string>());
    delete left;
    delete right;
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
  else if (exp->type_name == "pi2" && exp->case_name == "case4") // ch sends ; pi2 {{{
  {
    MpsTerm *succ = MpsTerm::Create(exp->content[3]);
    MpsChannel dest = ParseChannel(exp->content[0]);
    return CreateSends(exp->content[1],dest,succ);
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case5") // ch recvs ; pi2 {{{
  {
    MpsTerm *succ = MpsTerm::Create(exp->content[3]);
    MpsChannel source=ParseChannel(exp->content[0]);
    return CreateRecvs(exp->content[1],source,succ);
  } // }}}
  else if (exp->type_name == "pi2" && (exp->case_name == "case6" || exp->case_name == "case7")) // session id = new ( int of int ) @ id ; pi2 || in pi {{{
  { MpsTerm *succ = MpsTerm::Create(exp->content[12]);
    MpsTerm *result = new MpsLink(exp->content[10]->root.content,
                                  exp->content[1]->root.content,
                                  string2int(exp->content[5]->root.content),
                                  string2int(exp->content[7]->root.content),
                                  *succ,
                                  false);
    delete succ;
    return result;
  } // }}}
  else if (exp->type_name == "pi2" && (exp->case_name == "case8" || exp->case_name == "case9")) // channel id = new { participants } @ Gtype ; pi2 || in pi {{{
  {
    MpsTerm *succ = MpsTerm::Create(exp->content[10]);
    MpsGlobalType *type = MpsGlobalType::Create(exp->content[8]);
    vector<MpsParticipant> participants;
    FindParticipants(exp->content[5],participants);
    //for (int i=0; i<type->GetMaxPid(); ++i) // FIXME: ADD SYNTAX AND PARSER
    //  participants.push_back(MpsParticipant(i+1,int2string(i+1), false));
    if (participants.size()!=type->GetMaxPid())
    {
#if APIMS_DEBUG_LEVEL>1
      cerr << "Participant count does not match type: " << exp->type_name << "." << exp->case_name << endl;
#endif
      return new MpsEnd();
    }

    MpsTerm *result = new MpsNu(participants,exp->content[1]->root.content, *succ, *type);
    delete succ;
    delete type;
    return result;
  } // }}}
  else if (exp->type_name == "pi2" && (exp->case_name == "case10" || exp->case_name == "case11")) // process pvar dargs ( args ) = pi ; pi 2 || in pi {{{
  {
    // FIXME: Handle mode
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
  else if (exp->type_name == "pi2" && exp->case_name == "case12") // Mtype id = exp ; pi2 {{{
  { 
    MpsMsgType *type = MpsMsgType::Create(exp->content[0]);
    MpsExp *value = MpsExp::Create(exp->content[3]);
    MpsTerm *succ = MpsTerm::Create(exp->content[5]);
    MpsTerm *result = new MpsAssign(exp->content[1]->root.content, *value, *type, *succ);
    delete type;
    delete value;
    delete succ;
    return result;
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
      cerr << "Parsing error: Arguments for guivalue must be: int, session, int, Exp, Exp" << endl;
#endif
      return new MpsEnd();
    }
  } // }}}
  else if (exp->type_name == "pi2" && exp->case_name == "case14") // host ( Exps ) ; pi2 {{{
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
  else if (exp->type_name == "pi2" && exp->case_name == "case15") // hostheader ( Exps ) ; pi2 {{{
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

#if APIMS_DEBUG_LEVEL>1
  cerr << "Unknown term parsetree: " << exp->type_name << "." << exp->case_name << endl;
#endif
  return new MpsEnd();
} // }}}
MpsTerm *MpsTerm::Pi(const std::string &str) // {{{
{
  // Create parser
  SlrParser parser("pi");
  //parser.AddSRRule("pi2",false);
  AddParserDef(parser);
  
  parsetree *tree = parser.Parse(str);
  MpsTerm *result=MpsTerm::Create(tree);
  delete tree;
  return result;
} // }}}

// Helper functions
void Tvals(const parsetree *tree, vector<MpsExp*> &dest) // {{{
{
  if (tree->case_name == "tvals_some") // < Exps > {{{
    return Exps(tree->content[1], dest); // }}}
  if (tree->case_name == "tvals_none") // epsilon {{{
    return; // }}}

#if APIMS_DEBUG_LEVEL>1
  cerr << "Unknown Tvals: " << tree->type_name << "." << tree->case_name << endl;
#endif
  return;
} // }}}
void Label(const std::string &id, const parsetree *tree,  map<string,MpsExp*> &assertions) // {{{
{
  if (tree->type_name == "Assertion" && tree->case_name == "case1") // [[ Exp ]]
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
void GBranches(const parsetree *tree, map<string,MpsGlobalType*> &dest, map<string,MpsExp*> &assertions) // {{{
{
  if (tree->type_name == "Gbranches" && tree->case_name == "gbranches_end") // Label : Gtype {{{
  {
    dest[tree->content[0]->content[0]->root.content] = MpsGlobalType::Create(tree->content[2]);
    SetAssertion(tree->content[0],assertions);
  } // }}}
  else if (tree->type_name == "Gbranches" && tree->case_name == "gbranches_cont") // Label : Gtype , Gbranches {{{
  {
    dest[tree->content[0]->content[0]->root.content] = Gtype(tree->content[2]);
    SetAssertion(tree->content[0]->content[1],assertions);
    Gbranches(tree->content[4],dest,assertions);
  } // }}}

#if APIMS_DEBUG_LEVEL>1
    cerr << "Unknown Gbranches constructor: " << tree->type_name << "." << tree->case_name << endl;
#endif
} // }}}










void SetAssertion(const parsetree *tree,  map<string,MpsExp*> &assertions) // {{{
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
MpsChannel ParseChannel(const parsetree *ch) // {{{
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
MpsTerm *CreateSend(const parsetree *exp, MpsChannel dest, MpsTerm *succ) // {{{
{
  if (exp->type_name == "send" && exp->case_name == "case1") // << exp {{{
  { MpsExp *value = MpsExp::Create(exp->content[1]);
    MpsTerm *result = new MpsSnd(dest, *value, *succ, MpsMsgNoType(), false);
    delete value;
    delete succ;
    return result;
  } // }}}
  else if (exp->type_name == "send" && exp->case_name == "case2") // << bid {{{
  { MpsTerm *result = new MpsSelect(dest, exp->content[1]->root.content, *succ, false);
    delete succ;
    return result;
  } // }}}
#if APIMS_DEBUG_LEVEL>1
  cerr << "Unknown send parsetree: " << exp->type_name << "." << exp->case_name << endl;
#endif
  return new MpsEnd();
} // }}}
MpsTerm *CreateSends(const parsetree *exp, MpsChannel dest, MpsTerm *succ) // {{{
{
  if (exp->type_name == "sends" && exp->case_name == "case1") // send {{{
  { return CreateSend(exp->content[0],dest,succ);
  } // }}}
  else if (exp->type_name == "sends" && exp->case_name == "case2") // send sends {{{
  { MpsTerm *tmp=CreateSends(exp->content[1],dest,succ);
    return CreateSend(exp->content[0],dest,tmp);
  } // }}}
#if APIMS_DEBUG_LEVEL>1
  cerr << "Unknown sends parsetree: " << exp->type_name << "." << exp->case_name << endl;
#endif
  return new MpsEnd();
} // }}}
MpsTerm *CreateRecv(const parsetree *exp, MpsChannel dest, MpsTerm *succ) // {{{
{
  if (exp->type_name == "recv" && exp->case_name == "case1") // >> id {{{
  { MpsTerm *result = new MpsRcv(dest, exp->content[1]->root.content, -1, -1, *succ, MpsMsgNoType(), false);

    delete succ;
    return result;
  } // }}}
  else if (exp->type_name == "recv" && exp->case_name == "case2") // >> id @ ( int of int ) {{{
  { MpsTerm *result = new MpsRcv(dest,
                                 exp->content[1]->root.content,
                                 string2int(exp->content[4]->root.content),
                                 string2int(exp->content[6]->root.content),
                                 *succ,
                                 MpsMsgNoType(),
                                 false);
    delete succ;
    return result;
  } // }}}
#if APIMS_DEBUG_LEVEL>1
  cerr << "Unknown recv parsetree: " << exp->type_name << "." << exp->case_name << endl;
#endif
  return new MpsEnd();
} // }}}
MpsTerm *CreateRecvs(const parsetree *exp, MpsChannel dest, MpsTerm *succ) // {{{
{
  if (exp->type_name == "recvs" && exp->case_name == "case1") // recv {{{
  { return CreateRecv(exp->content[0],dest,succ);
  } // }}}
  else if (exp->type_name == "recvs" && exp->case_name == "case2") // recv recvs {{{
  { MpsTerm *tmp=CreateRecvs(exp->content[1],dest,succ);
    return CreateRecv(exp->content[0],dest,tmp);
  } // }}}
#if APIMS_DEBUG_LEVEL>1
  cerr << "Unknown recvs parsetree: " << exp->type_name << "." << exp->case_name << endl;
#endif
  return new MpsEnd();
} // }}}













void FindLocalBranches(const parsetree *tree, map<string,MpsLocalType*> &dest, map<string,MpsExp*> &assertions) // {{{
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
void CreateStateArgs(const parsetree *tree, vector<TypeArg> &dest) // {{{
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
MpsExp *CreateNamedAssertion(const parsetree *tree) // {{{
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
MpsExp *CreateNamedAssertion(const parsetree *tree, bool &used, std::string &name) // {{{
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
MpsGlobalType *MpsGlobalType::Create(const string &str) // {{{
{
  SlrParser parser("Gtype");
  AddParserDef(parser);
  parsetree *tree = parser.Parse(str);
  MpsGlobalType *result=Create(tree);
  delete tree;
  return result;
} // }}}
MpsLocalType *MpsLocalType::Create(const string &str) // {{{
{
  SlrParser parser("Ltype");
  AddParserDef(parser);
  parsetree *tree = parser.Parse(str);
  MpsLocalType *result=Create(tree);
  delete tree;
  return result;
} // }}}
MpsMsgType *MpsMsgType::Create(const string &str) // {{{
{
  SlrParser parser("Mtype");
  AddParserDef(parser);
  parsetree *tree = parser.Parse(str);
  MpsMsgType *result=Create(tree);
  delete tree;
  return result;
} // }}}

