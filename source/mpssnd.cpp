#include <hapi/mpssnd.hpp>
#include <hapi/mpsend.hpp>
#include <hapi/common.hpp>

using namespace std;
using namespace hapi;

MpsSnd::MpsSnd(const MpsChannel &channel, const MpsExp &exp, const MpsTerm &succ, const MpsMsgType &type, bool final) // {{{
: myChannel(channel)
, myFinal(final)
{
  mySucc = succ.Copy();
  myExp = exp.Copy();
  myType = type.Copy();
} // }}}
MpsSnd::~MpsSnd() // {{{
{
  delete myType;
  delete mySucc;
  delete myExp;
} // }}}
void *MpsSnd::TDCompileMain(tdc_pre pre, tdc_post wrap, tdc_error wrap_err, const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, const string &curPure, PureState pureState, bool checkPure) // Use rules Send and Deleg (and new rule for delegaing the session itself) {{{
{ map<string,void*> children;
  // Check purity constraints
  if (checkPure)
	{ if (pureStack.size()>0)
      return wrap_err(this,PrintTypeError("Implementation of pure participant " + int2string(pureStack.begin()->second) + "@" + pureStack.begin()->first + " must be immediately after its decleration",*this,Theta,Gamma,Omega),children);

    if (pureState!=CPS_IMPURE && pureState!=CPS_PURE)
      return wrap_err(this,PrintTypeError("Error in implementation of pure participant " + curPure + ". Pure implementations must conform with the structure \n     *   local X()\n	   *   ( global s=new ch(p of n);\n		 *     X();\n		 *     |\n		 *     P\n		 *   )\n		 *   local StartX(Int i)\n		 *   ( if i<=0\n		 *     then X();\n		 *     else X(); | StartX(i-1);\n		 *   )\n		 *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);
  }

  // Verify snd
  MpsMsgEnv::const_iterator session=Gamma.find(myChannel.GetName());
  // Check session is open
  if (session==Gamma.end())
    return wrap_err(this,PrintTypeError((string)"Sending on closed session: " + myChannel.GetName(),*this,Theta,Gamma,Omega),children);
  // Check if session type
  const MpsDelegateMsgType *msgType = dynamic_cast<const MpsDelegateMsgType*>(session->second);
  if (msgType==NULL)
    return wrap_err(this,PrintTypeError((string)"Sending on non-session type: " + myChannel.GetName(),*this,Theta,Gamma,Omega),children);
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
  // Check session has send type
  const MpsLocalSendType *sndType = dynamic_cast<const MpsLocalSendType*>(localMsgType);
  if (sndType==NULL)
  { delete localMsgType;
    return wrap_err(this,PrintTypeError((string)"Sending on session with non-send type: " + myChannel.GetName(),*this,Theta,Gamma,Omega),children);
  }
  // Check channel index is correct
  if (myChannel.GetIndex() != sndType->GetReceiver())
  { delete localMsgType;
    return wrap_err(this,PrintTypeError((string)"Sending on wrong index in session: " + myChannel.ToString(),*this,Theta,Gamma,Omega),children);
  }
  // Make new environment
  MpsLocalType *newType=NULL;
  if (sndType->GetAssertionType())
    newType = sndType->GetSucc()->ESubst(sndType->GetAssertionName(),*myExp);
  else
    newType = sndType->GetSucc()->Copy();
  MpsDelegateLocalMsgType *newMsgType=new MpsDelegateLocalMsgType(*newType,msgType->GetPid(),msgType->GetParticipants());
  delete newType;
  MpsMsgEnv newGamma = Gamma;
  newGamma[myChannel.GetName()]=newMsgType;
  // Check message type
  MpsMsgType *exptype = myExp->TypeCheck(newGamma);
  bool msgtypematch = exptype->Equal(Theta,*sndType->GetMsgType());
  string exptype_str= exptype->ToString("!!");
  delete exptype;
  if (not msgtypematch)
  { string sndTypeStr=sndType->GetMsgType()->ToString("!!");
    delete localMsgType;
    return wrap_err(this,PrintTypeError((string)"Expression does not have type:\n!!" + sndTypeStr + "\nBut type:\n!!" + exptype_str,*this,Theta,Gamma,Omega),children);
  }
  // Check Assertion is fulfilled
  if (sndType->GetAssertionType())
  { if (sndType->GetMsgType()->ToString()!="Bool")
    { string sndTypeStr=sndType->GetMsgType()->ToString("!!");
      delete localMsgType;
      return wrap_err(this,PrintTypeError((string)"Assertions not supported for message-type:\n!!" + sndTypeStr,*this,Theta,Gamma,Omega),children);
    }
    MpsExp *msgAssertion=sndType->GetAssertion().Subst(sndType->GetAssertionName(),*myExp);
    vector<const MpsExp*> hyps;
    hyps.push_back(&Theta);
    bool checkAssertion=msgAssertion->ValidExp(hyps);
    hyps.pop_back();
    delete msgAssertion;
    if (not checkAssertion)
    { delete localMsgType;
      return wrap_err(this,PrintTypeError((string)"Assertion not respected",*this,Theta,Gamma,Omega),children);
    }
  }
  // If delegating remove delegated session
  const MpsVarMsgType *varMsgType=dynamic_cast<const MpsVarMsgType*>(sndType->GetMsgType());
  if (dynamic_cast<const MpsDelegateMsgType*>(sndType->GetMsgType())!=NULL)
    newGamma.erase(newGamma.find(myExp->ToString()));
  else if (varMsgType!=NULL && varMsgType->IsLinear())
    newGamma.erase(newGamma.find(myExp->ToString()));
  // Check rest of program
  children["succ"]=mySucc->TDCompile(pre,wrap,wrap_err,Theta,newGamma,Omega,pureStack,curPure,pureState,checkPure);
  // Store if this is final action in session
  MpsLocalType *localNewMsgType=newMsgType->CopyLocalType();
  myFinal=localNewMsgType->IsDone();
  // Store message type in term
  SetMsgType(*(sndType->GetMsgType()));
  // Clean up
  delete newMsgType;
  delete localNewMsgType;
  delete localMsgType; // Copy that was casted to sndType

  // Wrap result
  return wrap(this,Theta,Gamma,Omega,pureStack,curPure,pureState,checkPure,children);
} // }}}
MpsTerm *MpsSnd::ApplySnd(const std::string &path, MpsExp **val, MpsChannel &ch) const // {{{
{ if (path.size()>0)
    return Error((string)"Applying Snd on "+ToString()+" with nonempty path "+path);
  (*val)=myExp->Eval();
  ch=myChannel;
  return mySucc->Copy();
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
MpsTerm *MpsSnd::PRename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PRename(src,dst);
  MpsTerm *result = new MpsSnd(myChannel, *myExp, *newSucc, GetMsgType(), GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSnd::ERename(const string &src, const string &dst) const // {{{
{
  MpsTerm *newSucc = mySucc->ERename(src,dst);
  MpsExp *newExp=myExp->Rename(src,dst);
  MpsChannel newChannel=myChannel.Rename(src,dst);
  MpsTerm *result = new MpsSnd(newChannel, *newExp, *newSucc, GetMsgType(), GetFinal());
  delete newSucc;
  delete newExp;
  return result;
} // }}}
MpsTerm *MpsSnd::MRename(const string &src, const string &dst) const // {{{
{
  MpsTerm *newSucc = mySucc->MRename(src,dst);
  MpsTerm *result = new MpsSnd(myChannel, *myExp, *newSucc, GetMsgType(), GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSnd::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  MpsTerm *newSucc = mySucc->ReIndex(session,pid,maxpid);
  MpsChannel newChannel=myChannel;
  if (session==myChannel.GetName())
    newChannel=MpsChannel(myChannel.GetName(),myChannel.GetIndex()*maxpid+pid);
  MpsTerm *result = new MpsSnd(newChannel, *myExp, *newSucc, GetMsgType(), GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSnd::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PSubst(var, exp, args, argpids, stateargs);
  MpsTerm *result = new MpsSnd(myChannel, *myExp, *newSucc, GetMsgType(), GetFinal());
  delete newSucc;
  return result;
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
  MpsMsgType *newType = GetMsgType().ESubst(source,dest);
  MpsTerm *result = new MpsSnd(newChannel, *newExp, *newSucc, *newType, GetFinal());
  delete newType;
  delete newSucc;
  delete newExp;
  return result;
} // }}}
MpsTerm *MpsSnd::MSubst(const string &source, const MpsMsgType &dest) const // {{{
{
  MpsTerm *newSucc = mySucc->MSubst(source,dest);
  MpsMsgType *newType = GetMsgType().MSubst(source,dest);
  MpsTerm *result = new MpsSnd(myChannel, *myExp, *newSucc, *newType, GetFinal());

  // Clean Up
  delete newType;
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsSnd::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  MpsMsgType *newType = GetMsgType().GSubst(source,dest,args);
  MpsTerm *result = new MpsSnd(myChannel, *myExp, *newSucc, *newType, GetFinal());

  // Clean Up
  delete newType;
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsSnd::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->LSubst(source,dest,args);
  MpsMsgType *newType = GetMsgType().LSubst(source,dest,args);
  MpsTerm *result = new MpsSnd(myChannel, *myExp, *newSucc, *newType, GetFinal());

  // Clean Up
  delete newType;
  delete newSucc;

  return result;
} // }}}
set<string> MpsSnd::FPV() const // {{{
{
  set<string> result = mySucc->FPV();
  return result;
} // }}}
set<string> MpsSnd::EV() const // {{{
{
  set<string> result = mySucc->EV();
  result.insert(myChannel.GetName());
  set<string> fv=myExp->FV();
  result.insert(fv.begin(),fv.end());
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
MpsTerm *MpsSnd::Copy() const // {{{
{
  // assert mySucc != NULL
  return new MpsSnd(myChannel, *myExp, *mySucc, GetMsgType(), GetFinal());
} // }}}
bool MpsSnd::Terminated() const // {{{
{
  return false;
} // }}}
MpsTerm *MpsSnd::Simplify() const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->Simplify();
  MpsTerm *result = new MpsSnd(myChannel, *myExp, *newSucc, GetMsgType(), GetFinal());
  delete newSucc;
  return result;
} // }}}
string MpsSnd::ToString(string indent) const // {{{
{
  return myChannel.ToString() + " << " + myExp->ToString() + ";\n" + indent + mySucc->ToString(indent);
} // }}}
string MpsSnd::ToTex(int indent, int sw) const // {{{
{
  return myChannel.ToTex() + "$\\ll$" + myExp->ToString() + ";\\newline\n"
       + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
} // }}}
string MpsSnd::ToC(const string &taskType) const // {{{
{
  stringstream result;
  // Create variable name foor the message to send
  const MpsDelegateMsgType *delType=dynamic_cast<const MpsDelegateMsgType*>(&GetMsgType());
  string msgName = ToC_Name(MpsExp::NewVar("send"));
  // Declare variable
  result << ToC_Yield()
         << "    // " << myChannel.GetName() << "[" << myChannel.GetIndex() << "] << " << myExp->ToString() << ";" << endl
         << "    { " << endl;
  string valName=myExp->ToC(result, GetMsgType().ToCPtr()); // Compute message and store in variable valName
  result << "      _this->var_" << ToC_Name(myChannel.GetName()) << "->Send("
         << myChannel.GetIndex()-1 << ",_task,";
  if (GetMsgType().IsSimple())
    result << "(long int)";
  result << valName << ");" << endl; // Send computed value
  if (!GetMsgType().IsSimple())
    result << "      RemoveRef(" << valName << ");" << endl;
  result << "    }" << endl;

  if (myFinal)
  {
    result << "  _this->var_" << ToC_Name(myChannel.GetName()) << "->Close(true);" << endl
           << "  RemoveRef(_this->var_" << ToC_Name(myChannel.GetName()) << ");" << endl
           << "  _this->var_" << ToC_Name(myChannel.GetName()) << "=NULL;" << endl;
  }
  result << mySucc->ToC(taskType);
  return result.str();
} // }}}
string MpsSnd::ToCHeader() const // {{{
{
  return mySucc->ToCHeader();
} // }}}
void MpsSnd::ToCConsts(vector<string> &dest, unordered_set<string> &existing) const // {{{
{ myExp->ToCConsts(dest,existing);
  mySucc->ToCConsts(dest,existing);
} // }}}
MpsTerm *MpsSnd::FlattenFork(bool normLhs, bool normRhs, bool pureMode) const // {{{
{
  MpsTerm *newSucc = mySucc->FlattenFork(normLhs,normRhs,pureMode);
  MpsTerm *result = new MpsSnd(myChannel, *myExp, *newSucc, *myType, GetFinal());
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsSnd::RenameAll() const // {{{
{ MpsTerm *newSucc=mySucc->RenameAll();
  MpsMsgType *newType=myType->RenameAll();
  MpsTerm *result=new MpsSnd(myChannel,*myExp,*newSucc,*newType, GetFinal());
  delete newSucc;
  delete newType;
  return result;
} // }}}
bool MpsSnd::Parallelize(const MpsTerm &receivers, MpsTerm* &seqTerm, MpsTerm* &parTerm) const // {{{
{
  // Find used vars
  set<string> usedVars = myExp->FV();
  usedVars.insert(myChannel.GetName());
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
  MpsTerm *parTmp = new MpsSnd(myChannel, *myExp, *parSucc, GetMsgType(), GetFinal());
  delete parSucc;
  parTerm = pre->Append(*parTmp);
  delete pre;
  delete parTmp;
  // Make sequential term
  seqTerm = new MpsSnd(myChannel, *myExp, *seqSucc, GetMsgType(), GetFinal());
  delete seqSucc;
  return opt1 || opt2;
} // }}}
MpsTerm *MpsSnd::Append(const MpsTerm &term) const // {{{
{
  MpsTerm *newSucc=mySucc->Append(term);
  MpsTerm *result=new MpsSnd(myChannel, *myExp, *newSucc, GetMsgType(), GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSnd::CopyWrapper(std::map<std::string,void*> &children) const // {{{
{
  return new MpsSnd(myChannel, *myExp, *(MpsTerm*)children["succ"], GetMsgType(), GetFinal());
} // }}}
MpsTerm *MpsSnd::CloseDefsPre(const MpsMsgEnv &Gamma) // {{{
{
  return this;
} // }}}
MpsTerm *MpsSnd::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ MpsTerm *newSucc=mySucc->ExtractDefinitions(env);
  MpsTerm *result=new MpsSnd(myChannel,*myExp,*newSucc,*myType, GetFinal());
  delete newSucc;
  return result;
} // }}}
const MpsMsgType &MpsSnd::GetMsgType() const // {{{
{ return *myType;
} // }}}
void MpsSnd::SetMsgType(const MpsMsgType &type) // {{{
{ delete myType;
  myType=type.Copy();
} // }}}
bool MpsSnd::GetFinal() const // {{{
{ return myFinal;
} // }}}
