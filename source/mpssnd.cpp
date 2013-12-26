#include<apims/mpssnd.hpp>
#include "common.cpp"

using namespace std;
using namespace apims;

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
bool MpsSnd::TypeCheck(const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega) // Use rules Send and Deleg (and new rule for delegaing the session itself) {{{
{
  MpsMsgEnv::const_iterator session=Gamma.find(myChannel.GetName());
  // Check session is open
  if (session==Gamma.end())
    return PrintTypeError((string)"Sending on closed session: " + myChannel.GetName(),*this,Theta,Gamma,Omega);
  // Check if session type
  const MpsDelegateMsgType *msgType = dynamic_cast<const MpsDelegateMsgType*>(session->second);
  if (msgType==NULL)
    return PrintTypeError((string)"Sending on non-session type: " + myChannel.GetName(),*this,Theta,Gamma,Omega);
  const MpsLocalRecType *recType = dynamic_cast<const MpsLocalRecType*>(msgType->GetLocalType());
  // Check if unfolding is necessary
  if (recType!=NULL)
    return TypeCheckRec(Theta, Gamma, Omega, *this, session->first);
  const MpsLocalForallType *allType = dynamic_cast<const MpsLocalForallType*>(msgType->GetLocalType());
  if (allType!=NULL)
    return TypeCheckForall(Theta, Gamma, Omega, *this, session->first);
  // Check session has send type
  const MpsLocalSendType *sndType = dynamic_cast<const MpsLocalSendType*>(msgType->GetLocalType());
  if (sndType==NULL)
    return PrintTypeError((string)"Sending on session with non-send type: " + myChannel.GetName(),*this,Theta,Gamma,Omega);
  // Check channel index is correct
  if (myChannel.GetIndex() != sndType->GetReceiver())
    return PrintTypeError((string)"Sending on wrong index in session: " + myChannel.ToString(),*this,Theta,Gamma,Omega);
  // Make new environment
  MpsLocalType *newType=NULL;
  if (sndType->GetAssertionType())
    newType = sndType->GetSucc()->ESubst(sndType->GetAssertionName(),*myExp);
  else
    newType = sndType->GetSucc()->Copy();
  MpsDelegateLocalMsgType *newMsgType=new MpsDelegateLocalMsgType(*newType,msgType->GetPid(),msgType->GetMaxpid());
  delete newType;
  MpsMsgEnv newGamma = Gamma;
  newGamma[myChannel.GetName()]=newMsgType;
  // Check message type
  MpsMsgType *exptype = myExp->TypeCheck(newGamma);
  bool msgtypematch = exptype->Equal(Theta,*sndType->GetMsgType());
  delete exptype;
  if (not msgtypematch)
    return PrintTypeError((string)"Message does not have type:\n!!" + sndType->GetMsgType()->ToString("!!"),*this,Theta,Gamma,Omega);
  // Check Assertion is fulfilled
  if (sndType->GetAssertionType())
  { if (sndType->GetMsgType()->ToString()!="Bool")
      return PrintTypeError((string)"Assertions not supported for message-type: " + sndType->GetMsgType()->ToString(),*this,Theta,Gamma,Omega);
    MpsExp *msgAssertion=sndType->GetAssertion().Subst(sndType->GetAssertionName(),*myExp);
    vector<const MpsExp*> hyps;
    hyps.push_back(&Theta);
    bool checkAssertion=msgAssertion->ValidExp(hyps);
    hyps.pop_back();
    delete msgAssertion;
    if (not checkAssertion)
      return PrintTypeError((string)"Assertion not respected",*this,Theta,Gamma,Omega);
  }
  // If delegating remove delegated session
  if (dynamic_cast<const MpsDelegateMsgType*>(sndType->GetMsgType())!=NULL)
    newGamma.erase(newGamma.find(myExp->ToString()));
  // Check rest of program
  bool result = mySucc->TypeCheck(Theta,newGamma,Omega);
  // Store if this is final action in session
  myFinal=newMsgType->GetLocalType()->IsDone();
  // Clean Up
  delete newMsgType;
  // Store message type in term
  SetMsgType(*(sndType->GetMsgType()));
  return result;
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
  return ToTex_Channel(myChannel) + "$\\ll$" + myExp->ToString() + ";\\newline\n"
       + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
} // }}}
string MpsSnd::ToC() const // {{{
{
  stringstream result;
  // Create variable name foor the message to send
  const MpsDelegateMsgType *delType=dynamic_cast<const MpsDelegateMsgType*>(&GetMsgType());
  string msgName = ToC_Name(MpsExp::NewVar("send"));
  // Declare variable
  result << "  { " << endl;
  string valName=myExp->ToC(result, GetMsgType().ToC()); // Compute message and store in variable valName
  if (delType!=NULL)
  {
    result << "    " << ToC_Name(myChannel.GetName()) << "->Delegate(" << int2string(myChannel.GetIndex()-1) << ", *" << valName << ");" << endl;
  }
  else
  {
    result << "    Message " << " " << msgName << ";" << endl
           << "    " << valName << ".ToMessage(" << msgName << ");" << endl;
    result << "    " << ToC_Name(myChannel.GetName()) << "->Send("
                     << int2string(myChannel.GetIndex()-1) << ","
                     << msgName << ");" << endl; // Send computed value
  }
  result << "  }" << endl;
  if (delType!=NULL)
  {
    result << "  " << valName << "->Close(false);" << endl
           << "  delete " << valName << ";" << endl;
  }
  if (myFinal)
  {
    result << "  " << ToC_Name(myChannel.GetName()) << "->Close(true);" << endl
           << "  delete " << ToC_Name(myChannel.GetName()) << ";" << endl;
  }
  result << mySucc->ToC();
  return result.str();
} // }}}
MpsTerm *MpsSnd::RenameAll() const // {{{
{ MpsTerm *newSucc=mySucc->RenameAll();
  MpsMsgType *newType=myType->RenameAll();
  MpsTerm *result=new MpsSnd(myChannel,*myExp,*newSucc,*newType, GetFinal());
  delete newSucc;
  delete newType;
  return result;
} // }}}
MpsTerm *MpsSnd::CloseDefinitions() const // {{{
{
  MpsTerm *newSucc = mySucc->CloseDefinitions();
  MpsTerm *result = new MpsSnd(myChannel, *myExp, *newSucc, *myType, GetFinal());
  delete newSucc;

  return result;
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