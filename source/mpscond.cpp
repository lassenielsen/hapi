#include<hapi/mpscond.hpp>
#include <hapi/common.hpp>

using namespace std;
using namespace hapi;

MpsCond::MpsCond(const MpsExp &cond, const MpsTerm &truebranch, const MpsTerm &falsebranch) // {{{
{
  myCond = cond.Copy();
  myTrueBranch = truebranch.Copy();
  myFalseBranch = falsebranch.Copy();
} // }}}
MpsCond::~MpsCond() // {{{
{
  delete myCond;
  delete myTrueBranch;
  delete myFalseBranch;
} // }}}
void *MpsCond::TDCompileMain(tdc_pre pre, tdc_post wrap, tdc_error wrap_err, const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, const string &curPure, PureState pureState, bool checkPure) // Use rule Cond {{{
{ map<string,void*> children;
  PureState trueState=pureState;
  PureState falseState=pureState;
  if (checkPure)
	{ // Check purity constraints
    if (pureState!=CPS_IMPURE && pureState!=CPS_PURE && pureState!=CPS_INIT_BRANCH)
      return wrap_err(this,PrintTypeError("Error in implementation of pure participant " + curPure + ". Pure implementations must conform with the structure \n     *   local X()\n	   *   ( global s=new ch(p of n);\n		 *     X();\n		 *     |\n		 *     P\n		 *   )\n		 *   local StartX(Int i)\n		 *   ( if i<=0\n		 *     then X();\n		 *     else X(); | StartX(i-1);\n		 *   )\n		 *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);

    if (pureState==CPS_INIT_BRANCH)
    { trueState=CPS_INIT_BRANCH2_CALL;
      falseState=CPS_INIT_BRANCH1_FORK;
    }
  }

  MpsBoolMsgType booltype;
  MpsMsgType *condtype = myCond->TypeCheck(Gamma);
  bool condtypematch = booltype.Equal(Theta,*condtype);
  delete condtype;
  if (!condtypematch)
    return wrap_err(this,PrintTypeError("Condition not of type Bool",*this,Theta,Gamma,Omega),children);
  // Make new Thetas
  MpsExp *trueTheta = new MpsBinOpExp("and",Theta,*myCond,MpsBoolMsgType(),MpsBoolMsgType());
  MpsExp *notCond = new MpsUnOpExp("not",*myCond);
  MpsExp *falseTheta = new MpsBinOpExp("and",Theta,*notCond,MpsBoolMsgType(),MpsBoolMsgType());
  delete notCond;
  
  children["true"] = myTrueBranch->TDCompile(pre,wrap,wrap_err,*trueTheta,Gamma,Omega,pureStack,curPure, trueState, checkPure);
  children["false"] = myFalseBranch->TDCompile(pre,wrap,wrap_err,*falseTheta,Gamma,Omega,pureStack,curPure, falseState, checkPure);
  // Clean Up
  delete trueTheta;
  delete falseTheta;

  // Wrap result
  return wrap(this,Theta,Gamma,Omega,pureStack,curPure,pureState,checkPure,children);
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
MpsTerm *MpsCond::PRename(const string &src, const string &dst) const // {{{
{
  MpsTerm *newTrueBranch = myTrueBranch->PRename(src,dst);
  MpsTerm *newFalseBranch = myFalseBranch->PRename(src,dst);
  MpsCond *result = new MpsCond(*myCond, *newTrueBranch, *newFalseBranch);
  delete newTrueBranch;
  delete newFalseBranch;
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
MpsTerm *MpsCond::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  MpsTerm *newTrueBranch = myTrueBranch->ReIndex(session,pid,maxpid);
  MpsTerm *newFalseBranch = myFalseBranch->ReIndex(session,pid,maxpid);
  MpsCond *result = new MpsCond(*myCond, *newTrueBranch, *newFalseBranch);
  delete newTrueBranch;
  delete newFalseBranch;
  return result;
} // }}}
MpsTerm *MpsCond::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
{
  MpsTerm *newTrueBranch = myTrueBranch->PSubst(var,exp,args,argpids,stateargs);
  MpsTerm *newFalseBranch = myFalseBranch->PSubst(var,exp,args,argpids,stateargs);
  MpsCond *result = new MpsCond(*myCond, *newTrueBranch, *newFalseBranch);
  delete newTrueBranch;
  delete newFalseBranch;
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
set<string> MpsCond::EV() const // {{{
{
  set<string> result;
  result.clear();
  set<string> cfv=myCond->FV();
  set<string> tfev=myTrueBranch->EV();
  set<string> ffev=myFalseBranch->EV();
  result.insert(cfv.begin(), cfv.end());
  result.insert(tfev.begin(), tfev.end());
  result.insert(ffev.begin(), ffev.end());
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
MpsTerm *MpsCond::Copy() const // {{{
{
  return new MpsCond(*myCond, *myTrueBranch, *myFalseBranch);
} // }}}
bool MpsCond::Terminated() const // {{{
{
  return false;
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
string MpsCond::ToString(string indent) const // {{{
{
  string result = (string)"if " + myCond->ToString() + "\n"
                + indent + "then " + myTrueBranch->ToString(indent + "     ") + "\n"
                + indent + "else " + myFalseBranch->ToString(indent + "     ");
  return result;
} // }}}
string MpsCond::ToTex(int indent, int sw) const // {{{
{
  string result = ToTex_KW("if") +" " + myCond->ToString() + "\\newline\n"
                + ToTex_Hspace(indent,sw) + ToTex_KW("then") + " " + myTrueBranch->ToTex(indent + 5,sw) + "\\newline\n"
                + ToTex_Hspace(indent,sw) + ToTex_KW("else") + " " + myFalseBranch->ToTex(indent + 5,sw);
  return result;
} // }}}
string MpsCond::ToC(const string &taskType) const // {{{
{
  stringstream result;
  string trueLabel = ToC_Name(MpsExp::NewVar(taskType+"_true"));
  string falseLabel = ToC_Name(MpsExp::NewVar(taskType+"_false"));
  result << ToC_Yield()
         << "    {" << endl;
  string newName = myCond->ToC(result,"libpi::Bool");
  result << "      if (((libpi::Bool*)" << newName << ".get())->GetValue())" << endl
         << "        goto " << trueLabel << ";" << endl
         << "      else" << endl
         << "        goto " << falseLabel << ";" << endl
         << "    }" << endl;
  result << "    " << trueLabel << ":" << endl
         << myTrueBranch->ToC(taskType)
         << "    " << falseLabel << ":" << endl
         << myFalseBranch->ToC(taskType);
  return result.str();
} // }}}
string MpsCond::ToCHeader() const // {{{
{
  stringstream result;
  result << myTrueBranch->ToCHeader();
  result << myFalseBranch->ToCHeader();
  return result.str();
} // }}}
void MpsCond::ToCConsts(std::vector<std::string> &dest, std::unordered_set<std::string> &existing) const // {{{
{
  myCond->ToCConsts(dest,existing);
  myTrueBranch->ToCConsts(dest,existing);
  myFalseBranch->ToCConsts(dest,existing);
} // }}}
MpsTerm *MpsCond::FlattenFork(bool normLhs, bool normRhs, bool pureMode) const // {{{
{
  MpsTerm *newTrueBranch=myTrueBranch->FlattenFork(normLhs,normRhs,pureMode);
  MpsTerm *newFalseBranch=myFalseBranch->FlattenFork(normLhs,normRhs,pureMode);

  MpsTerm *result=new MpsCond(*myCond, *newTrueBranch, *newFalseBranch);

  delete newTrueBranch;
  delete newFalseBranch;

  return result;
} // }}}
MpsTerm *MpsCond::RenameAll() const // {{{
{ MpsTerm *newTrueBranch=myTrueBranch->RenameAll();
  MpsTerm *newFalseBranch=myFalseBranch->RenameAll();

  MpsTerm *result=new MpsCond(*myCond,*newTrueBranch,*newFalseBranch);

  delete newTrueBranch;
  delete newFalseBranch;

  return result;
} // }}}
bool MpsCond::Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const // {{{
{ MpsTerm *seqTrueBranch = myTrueBranch->Parallelize();
  MpsTerm *seqFalseBranch = myFalseBranch->Parallelize();
  seqTerm=new MpsCond(*myCond, *seqTrueBranch, *seqFalseBranch);
  parTerm=receives.Append(*seqTerm);
  return false; // All optimizations are guarded
} // }}}
MpsTerm *MpsCond::Append(const MpsTerm &term) const // {{{
{ MpsTerm *newTrueBranch=myTrueBranch->Append(term);
  MpsTerm *newFalseBranch=myFalseBranch->Append(term);
  MpsTerm *result=new MpsCond(*myCond, *newTrueBranch, *newFalseBranch);
  delete newTrueBranch;
  delete newFalseBranch;
  return result;
} // }}}
MpsTerm *MpsCond::CopyWrapper(std::map<std::string,void*> &children) const // {{{
{
  return new MpsCond(*myCond, *(MpsTerm*)children["true"], *(MpsTerm*)children["false"]);
} // }}}
MpsTerm *MpsCond::CloseDefsPre(const MpsMsgEnv &Gamma) // {{{
{
  return this;
} // }}}
MpsTerm *MpsCond::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ MpsTerm *newTrueBranch=myTrueBranch->ExtractDefinitions(env);
  MpsTerm *newFalseBranch=myFalseBranch->ExtractDefinitions(env);

  MpsTerm *result=new MpsCond(*myCond,*newTrueBranch,*newFalseBranch);

  delete newTrueBranch;
  delete newFalseBranch;

  return result;
} // }}}
