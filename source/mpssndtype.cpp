#include <hapi/mpssndtype.hpp>
#include <hapi/mpsend.hpp>

using namespace std;
using namespace hapi;

MpsSndType::MpsSndType(const string &session, const MpsMsgType &type, const MpsTerm &succ, bool final) // {{{
: mySession(session)
, myFinal(final)
{
  mySucc = succ.Copy();
  myType = type.Copy();
} // }}}
MpsSndType::~MpsSndType() // {{{
{
  delete myType;
  delete mySucc;
} // }}}
void *MpsSndType::TDCompileMain(tdc_pre pre, tdc_post wrap, tdc_error wrap_err, const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, const string &curPure, PureState pureState, bool checkPure) // Use rules Send and Deleg (and new rule for delegaing the session itself) {{{
{ map<string,void*> children;
  // Check purity constraints
  if (checkPure)
  { if (pureStack.size()>0)
    return wrap_err(this,PrintTypeError("Implementation of pure participant " + int2string(pureStack.begin()->second) + "@" + pureStack.begin()->first + " must be immediately after its decleration",*this,Theta,Gamma,Omega),children);

    if (pureState!=CPS_IMPURE && pureState!=CPS_PURE)
      return wrap_err(this,PrintTypeError("Error in implementation of pure participant " + curPure + ". Pure implementations must conform with the structure \n     *   local X()\n	   *   ( global s=new ch(p of n);\n		 *     X();\n		 *     |\n		 *     P\n		 *   )\n		 *   local StartX(Int i)\n		 *   ( if i<=0\n		 *     then X();\n		 *     else X(); | StartX(i-1);\n		 *   )\n		 *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);
  }

  // Verify snd
  MpsMsgEnv::const_iterator session=Gamma.find(mySession);
  // Check session is open
  if (session==Gamma.end())
    return wrap_err(this,PrintTypeError((string)"Sending on closed session: " + mySession,*this,Theta,Gamma,Omega),children);
  // Check if session type
  const MpsDelegateMsgType *msgType = dynamic_cast<const MpsDelegateMsgType*>(session->second);
  if (msgType==NULL)
    return wrap_err(this,PrintTypeError((string)"Sending on non-session type: " + mySession,*this,Theta,Gamma,Omega),children);
  MpsLocalType *localMsgType=msgType->CopyLocalType();
  const MpsLocalRecType *recType = dynamic_cast<const MpsLocalRecType*>(localMsgType);
  // Check if unfolding is necessary
  if (recType!=NULL)
  { delete localMsgType;
    return TypeCheckRec(pre, wrap, wrap_err, Theta, Gamma, Omega, pureStack, curPure, pureState, checkPure, *this, session->first);
  }
  const MpsLocalForallType *allType = dynamic_cast<const MpsLocalForallType*>(localMsgType);
  if (allType!=NULL)
  { delete localMsgType;
    return TypeCheckForall(pre, wrap, wrap_err, Theta, Gamma, Omega, pureStack, curPure, pureState, checkPure, *this, session->first);
  }
  // Check session has correct type
  const MpsLocalTypeSendType *sndType = dynamic_cast<const MpsLocalTypeSendType*>(localMsgType);
  if (sndType==NULL)
  { delete localMsgType;
    return wrap_err(this,PrintTypeError((string)"Sending type on session with non-send type: " + mySession,*this,Theta,Gamma,Omega),children);
  }
  // Check linearity constraint
  if (!sndType->IsLinear())
  { const MpsDelegateMsgType *delType=dynamic_cast<const MpsDelegateMsgType*>(myType);
    if (delType!=NULL) // Using linear type in (possibly) non linear setting
    { string dest=sndType->GetDest();
      delete localMsgType;
      return wrap_err(this,PrintTypeError((string)"Applying session type to non-linear polymorphic type: " + dest,*this,Theta,Gamma,Omega),children);
    }
  }
  // Make new environment
  MpsLocalType *newType=sndType->GetSucc()->MSubst(sndType->GetDest(),*myType);
  // Store if this is final action in session
  myFinal=newType->IsDone();
  MpsDelegateLocalMsgType *newMsgType=new MpsDelegateLocalMsgType(*newType,msgType->GetPid(),msgType->GetParticipants());
  delete newType;
  MpsMsgEnv newGamma = Gamma;
  newGamma[mySession]=newMsgType;
  // Check rest of program
  children["succ"]=mySucc->TDCompile(pre,wrap,wrap_err,Theta,newGamma,Omega,pureStack,curPure,pureState,checkPure);
  // Clean Up
  delete newMsgType;
  delete localMsgType;

  // Wrap result
  return wrap(this,Theta,Gamma,Omega,pureStack,curPure,pureState,checkPure,children);
} // }}}
bool MpsSndType::SubSteps(vector<MpsStep> &dest) // {{{
{
  MpsEvent event;
  event.myType = sndtype;
  event.mySession = mySession;
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event, paths));
  return false;
} // }}}
MpsTerm *MpsSndType::ApplyOther(const std::string &path) const // {{{
{ return mySucc->Copy();
} // }}}
MpsTerm *MpsSndType::PRename(const string &src, const string &dst) const // {{{
{
  MpsTerm *newSucc = mySucc->PRename(src,dst);
  MpsTerm *result = new MpsSndType(mySession, *myType, *newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSndType::ERename(const string &src, const string &dst) const // {{{
{
  MpsTerm *newSucc = mySucc->ERename(src,dst);
  MpsMsgType *newType=myType->ERename(src,dst);
  string newSession=mySession==src?dst:mySession;
  MpsTerm *result = new MpsSndType(newSession, *newType, *newSucc, GetFinal());
  delete newSucc;
  delete newType;
  return result;
} // }}}
MpsTerm *MpsSndType::MRename(const string &src, const string &dst) const // {{{
{
  MpsTerm *newSucc = mySucc->MRename(src,dst);
  MpsMsgType *newType=myType->MRename(src,dst);
  MpsTerm *result = new MpsSndType(mySession, *newType, *newSucc, GetFinal());
  delete newSucc;
  delete newType;
  return result;
} // }}}
MpsTerm *MpsSndType::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  MpsTerm *newSucc = mySucc->ReIndex(session,pid,maxpid);
  MpsTerm *result = new MpsSndType(mySession, *myType, *newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSndType::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
{
  MpsTerm *newSucc = mySucc->PSubst(var, exp, args, argpids, stateargs);
  MpsTerm *result = new MpsSndType(mySession, *myType, *newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSndType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  string newSession=stringESubst(mySession,source,dest);
  MpsTerm *newSucc = mySucc->ESubst(source,dest);
  MpsMsgType *newType=myType->ESubst(source,dest);
  MpsTerm *result = new MpsSndType(newSession, *newType, *newSucc, GetFinal());
  delete newType;
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSndType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  MpsMsgType *newType = myType->GSubst(source,dest,args);
  MpsTerm *result = new MpsSndType(mySession, *newType, *newSucc, GetFinal());

  // Clean Up
  delete newType;
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsSndType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->LSubst(source,dest,args);
  MpsMsgType *newType = myType->LSubst(source,dest,args);
  MpsTerm *result = new MpsSndType(mySession, *newType, *newSucc, GetFinal());

  // Clean Up
  delete newType;
  delete newSucc;

  return result;
} // }}}
set<string> MpsSndType::FPV() const // {{{
{
  set<string> result = mySucc->FPV();
  return result;
} // }}}
set<string> MpsSndType::EV() const // {{{
{
  set<string> result = mySucc->EV();
  result.insert(mySession);
  set<string> fv=myType->FEV();
  result.insert(fv.begin(),fv.end());
  return result;
} // }}}
set<string> MpsSndType::FEV() const // {{{
{
  set<string> result = mySucc->FEV();
  result.insert(mySession);
  set<string> fv=myType->FEV();
  result.insert(fv.begin(),fv.end());
  return result;
} // }}}
MpsTerm *MpsSndType::Copy() const // {{{
{
  MpsTerm *result = new MpsSndType(mySession, *myType, *mySucc, GetFinal());
  return result;
} // }}}
bool MpsSndType::Terminated() const // {{{
{
  return false;
} // }}}
MpsTerm *MpsSndType::Simplify() const // {{{
{
  MpsTerm *newSucc = mySucc->Simplify();
  MpsTerm *result = new MpsSndType(mySession, *myType, *newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
string MpsSndType::ToString(string indent) const // {{{
{
  return mySession + " << " + myType->ToString() + ";\n" + indent + mySucc->ToString(indent);
} // }}}
string MpsSndType::ToTex(int indent, int sw) const // {{{
{
  return mySession + "$\\ll$" + myType->ToString() + ";\\newline\n"
       + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
} // }}}
string MpsSndType::ToC(const string &taskType) const // {{{
{
  // Thid is a no-op statement, simply perform succ
  return mySucc->ToC(taskType);
} // }}}
string MpsSndType::ToCHeader() const // {{{
{
  return mySucc->ToCHeader();
} // }}}
void MpsSndType::ToCConsts(vector<string> &dest, unordered_set<string> &existing) const // {{{
{ mySucc->ToCConsts(dest,existing);
} // }}}
MpsTerm *MpsSndType::FlattenFork(bool normLhs, bool normRhs, bool pureMode) const // {{{
{
  MpsTerm *newSucc = mySucc->FlattenFork(normLhs,normRhs,pureMode);
  MpsTerm *result = new MpsSndType(mySession, *myType, *newSucc, GetFinal());
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsSndType::RenameAll() const // {{{
{ MpsTerm *newSucc=mySucc->RenameAll();
  MpsMsgType *newType=myType->RenameAll();
  MpsTerm *result = new MpsSndType(mySession, *newType, *newSucc, GetFinal());
  delete newSucc;
  delete newType;
  return result;
} // }}}
bool MpsSndType::Parallelize(const MpsTerm &receivers, MpsTerm* &seqTerm, MpsTerm* &parTerm) const // {{{
{
  // Find used vars
  set<string> usedVars;
  usedVars.insert(mySession);
  // Split receives using the used vars
  MpsTerm *pre;
  MpsTerm *post;
  receivers.Split(usedVars,pre,post);
  bool opt1=dynamic_cast<const MpsEnd*>(post)==NULL;
  // Parallelize succ with post receives
  MpsTerm *seqSucc;
  MpsTerm *parSucc;
  bool opt2=mySucc->Parallelize(*post,seqSucc,parSucc);
  delete post;
  // Make parallelized term
  MpsTerm *parTmp = new MpsSndType(mySession, *myType, *parSucc, GetFinal());
  delete parSucc;
  parTerm = pre->Append(*parTmp);
  delete pre;
  delete parTmp;
  // Make sequential term
  seqTerm = new MpsSndType(mySession, *myType, *seqSucc, GetFinal());
  delete seqSucc;
  return opt1 || opt2;
} // }}}
MpsTerm *MpsSndType::Append(const MpsTerm &term) const // {{{
{
  MpsTerm *newSucc=mySucc->Append(term);
  MpsTerm *result=new MpsSndType(mySession, *myType, *newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSndType::CopyWrapper(std::map<std::string,void*> &children) const // {{{
{
  return new MpsSndType(mySession, *myType, *(MpsTerm*)children["succ"], GetFinal());
} // }}}
MpsTerm *MpsSndType::CloseDefsPre(const MpsMsgEnv &Gamma) // {{{
{
  return this;
} // }}}
MpsTerm *MpsSndType::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ MpsTerm *newSucc=mySucc->ExtractDefinitions(env);
  MpsTerm *result = new MpsSndType(mySession, *myType, *newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
bool MpsSndType::GetFinal() const // {{{
{ return false;
} // }}}
