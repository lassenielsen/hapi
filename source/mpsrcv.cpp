#include<apims/mpsrcv.hpp>
#include "common.cpp"

using namespace std;
using namespace apims;

MpsRcv::MpsRcv(const MpsChannel &channel, const string &dest, int pid, int maxpid, const MpsTerm &succ, const MpsMsgType &type, bool final) // {{{
: myChannel(channel)
, myDest(dest)
, myPid(pid)
, myMaxPid(maxpid)
, myFinal(final)
{
  mySucc = succ.Copy();
  myType=type.Copy();
} // }}}
MpsRcv::~MpsRcv() // {{{
{
  delete myType;
  delete mySucc;
} // }}}
bool MpsRcv::TypeCheck(const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega) // Use rules Rcv and Srec {{{
{
  MpsMsgEnv::const_iterator session=Gamma.find(myChannel.GetName());
  // Check session is open
  if (session==Gamma.end())
    return PrintTypeError((string)"Receiving on closed session: " + myChannel.GetName(),*this,Theta,Gamma,Omega);
  // Check if session type
  const MpsDelegateMsgType *msgType = dynamic_cast<const MpsDelegateMsgType*>(session->second);
  if (msgType==NULL)
    return PrintTypeError((string)"Sending on non-session type: " + myChannel.GetName(),*this,Theta,Gamma,Omega);
  const MpsLocalRecType *recType = dynamic_cast<const MpsLocalRecType*>(msgType->GetLocalType());
  // Check if unfolding is necessary
  if (recType!=NULL)
    return TypeCheckRec(Theta,Gamma, Omega, *this, session->first);
  const MpsLocalForallType *allType = dynamic_cast<const MpsLocalForallType*>(msgType->GetLocalType());
  if (allType!=NULL)
    return TypeCheckForall(Theta, Gamma, Omega, *this, session->first);
  // Check session has receive type
  const MpsLocalRcvType *rcvType = dynamic_cast<const MpsLocalRcvType*>(msgType->GetLocalType());
  if (rcvType==NULL)
    return PrintTypeError((string)"Receiving on session: " + myChannel.GetName(),*this,Theta,Gamma,Omega);
  // Check channel index is correct
  if (myChannel.GetIndex() != rcvType->GetSender())
    return PrintTypeError((string)"Receiving on session(wrong index): " + myChannel.ToString(),*this,Theta,Gamma,Omega);
  // Is renaming of myDest necessary?
  bool rename = false;
  if (rcvType->GetAssertionType())
  { set<string> fv=Theta.FV();
    if (fv.find(myDest)!=fv.end())
      rename=true;
    for (MpsMsgEnv::const_iterator it=Gamma.begin(); (not rename) && it!=Gamma.end(); ++it)
    { fv = it->second->FEV();
      if (fv.find(myDest)!=fv.end())
        rename=true;
    }
  }
  string newDest=rename?MpsExp::NewVar(myDest):myDest;
  // Make new Gamma
  MpsMsgEnv newGamma;
  for (MpsMsgEnv::const_iterator it=Gamma.begin(); it!=Gamma.end(); ++it)
  { const MpsDelegateMsgType *delType=dynamic_cast<const MpsDelegateMsgType*>(it->second);
    if (delType==NULL) // Not session type
      newGamma[it->first]=it->second;
    else // SESSION TYPE
    { MpsLocalType *newType=NULL;
      if (it->first==myChannel.GetName())
      { if (rcvType->GetAssertionType())
        { MpsLocalType *tmpType=NULL;
          if (rename && rcvType->GetAssertionName()!=myDest) // Rename in succ
            tmpType=rcvType->GetSucc()->ERename(myDest,newDest);
          else
            tmpType=rcvType->GetSucc()->Copy();
          newType=tmpType->ERename(rcvType->GetAssertionName(),myDest);
        }
        else
          newType=rcvType->GetSucc()->Copy();
      }
      else
      { if (rename)
          newType=delType->GetLocalType()->ERename(myDest,newDest);
        else
          newType=delType->GetLocalType()->Copy();
      }
      newGamma[it->first] = new MpsDelegateLocalMsgType(*newType,delType->GetPid(),delType->GetMaxpid());
      delete newType;
    }
  }
  // Save created type for deletion
  MpsDelegateMsgType *newType=dynamic_cast<MpsDelegateMsgType*>(newGamma[myChannel.GetName()]);
  // Check if assertion domain is respected
  if (rcvType->GetAssertionType() && typeid(*rcvType->GetMsgType()) != typeid(MpsBoolMsgType))
    return PrintTypeError((string)"Assertion of non-boolean type: " + rcvType->ToString("!!!!!      "),*this,Theta,Gamma,Omega);

  // Check not overwriting unfinished session
  MpsMsgEnv::const_iterator dstVar=Gamma.find(myDest);
  if (dstVar!=Gamma.end() &&
      dynamic_cast<const MpsDelegateMsgType*>(dstVar->second)!=NULL &&
      !dynamic_cast<const MpsDelegateMsgType*>(dstVar->second)->GetLocalType()->Equal(Theta,MpsLocalEndType()))
    return PrintTypeError((string)"Overwriting open session: " + myDest,*this,Theta,Gamma,Omega);
  // Check specification of pid and maxpid
  if (dynamic_cast<const MpsDelegateMsgType*>(rcvType->GetMsgType())!=NULL &&
      (dynamic_cast<const MpsDelegateMsgType*>(rcvType->GetMsgType())->GetPid()!=myPid ||
       dynamic_cast<const MpsDelegateMsgType*>(rcvType->GetMsgType())->GetMaxpid()!=myMaxPid) )
      return PrintTypeError((string)"Receiving session with pid or maxpid different than specified",*this,Theta,Gamma,Omega);
  newGamma[myDest]=rcvType->GetMsgType()->Copy();
  // Create new Assumptions
  MpsExp *newTheta=NULL;
  if (rcvType->GetAssertionType())
  { MpsExp *newAssertion=NULL;
    if (rcvType->GetAssertionName()!=myDest)
    { MpsExp *tmpAssertion=NULL;
      if (rename)
        tmpAssertion=rcvType->GetAssertion().Rename(myDest,newDest);
      else
        tmpAssertion=rcvType->GetAssertion().Copy();
      newAssertion=tmpAssertion->Rename(rcvType->GetAssertionName(),myDest);
      delete tmpAssertion;
    }
    else
      newAssertion=rcvType->GetAssertion().Copy();
    newTheta=new MpsBinOpExp("and",Theta,*newAssertion,MpsBoolMsgType(),MpsBoolMsgType());
    delete newAssertion;
  }
  else
    newTheta=Theta.Copy();
  // Check rest of program
  bool result = mySucc->TypeCheck(*newTheta,newGamma,Omega);
  // Store if this is final action in session
  myFinal=newType->GetLocalType()->IsDone();
  // Clean Up
  delete newTheta;
  if (rename)
  {
    while (newGamma.size()>0)
    { const MpsDelegateMsgType *delType=dynamic_cast<const MpsDelegateMsgType*>(newGamma.begin()->second);
      if (delType!=NULL || newGamma.begin()->first==myDest) // Is session type
        delete newGamma.begin()->second;
      newGamma.erase(newGamma.begin());
    }
  }
  else
  {
    delete newGamma[session->first];
    delete newGamma[myDest];
  }

  // Store message type in term
  SetMsgType(*(rcvType->GetMsgType()));
  return result;
} // }}}
MpsTerm *MpsRcv::ApplyRcv(const std::string &path, const MpsExp *val) const // {{{
{ if (path.size()>0)
    return Error((string)"Applying Rcv on "+ToString()+" with nonempty path "+path);
  MpsTerm *result=NULL;
  if (myMaxPid!=-1) // Reindex channels in received session
  { MpsTerm *tmpResult = mySucc->ReIndex(myDest,myPid,myMaxPid);
    result=tmpResult->ESubst(myDest,*val);
    delete tmpResult;
  }
  else
    result = mySucc->ESubst(myDest,*val);
 return result;
} // }}}
bool MpsRcv::SubSteps(vector<MpsStep> &dest) // {{{
{
  MpsEvent event;
  event.myType = rcv;
  event.myChannel = myChannel;
  event.myValue = new MpsVarExp(MpsExp::NewVar(), MpsMsgNoType());
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event, paths));
  return false;
} // }}}
MpsTerm *MpsRcv::PRename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PRename(src,dst);
  MpsTerm *result = new MpsRcv(myChannel, myDest, myPid, myMaxPid, *newSucc, GetMsgType(), GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsRcv::ERename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  MpsChannel newChannel=myChannel.Rename(src,dst);
  MpsMsgType *newType = GetMsgType().ERename(src,dst);
  if (src == myDest) // No substitution is needed in successor
  { MpsTerm *result = new MpsRcv(newChannel, myDest, myPid, myMaxPid, *mySucc, *newType, GetFinal());
    delete newType;
    return result;
  }

  MpsTerm *newSucc=NULL;
  string newDest=myDest;
  if (dst==myDest) // Must rename to avoid capturing
  {
    newDest = MpsExp::NewVar(myDest);
    MpsTerm *tmpSucc = mySucc->ERename(myDest,newDest);
    newSucc = tmpSucc->ERename(src,dst);
    delete tmpSucc;
  }
  else
    newSucc = mySucc->ERename(src,dst);
  MpsTerm *result = new MpsRcv(newChannel, newDest, myPid, myMaxPid, *newSucc, *newType, GetFinal());
  delete newType;
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsRcv::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  // assert mySucc != NULL
  MpsChannel newChannel=myChannel;
  if (session==myChannel.GetName())
    newChannel=MpsChannel(myChannel.GetName(),myChannel.GetIndex()+maxpid*pid);
  if (session == myDest) // No substitution is needed in successor
    return new MpsRcv(newChannel, myDest, myPid, myMaxPid, *mySucc, GetMsgType(), GetFinal());

  MpsTerm *newSucc = mySucc->ReIndex(session,pid,maxpid);
  MpsTerm *result = new MpsRcv(newChannel, myDest, myPid, myMaxPid, *newSucc, GetMsgType(), GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsRcv::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PSubst(var, exp, args, argpids, stateargs);
  MpsTerm *result = new MpsRcv(myChannel, myDest, myPid, myMaxPid, *newSucc, GetMsgType(), GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsRcv::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  // assert mySucc != NULL
  MpsChannel newChannel=myChannel.Subst(source,dest);
  MpsMsgType *newType=GetMsgType().ESubst(source,dest);
  if (source == myDest) // No substitution is needed in successor
  { MpsTerm *result = new MpsRcv(newChannel, myDest, myPid, myMaxPid, *mySucc, *newType, GetFinal());
    delete newType;
    return result;
  }

  MpsTerm *newSucc = NULL;
  string newDest = myDest;
  set<string> fv = dest.FV();
  if (fv.find(myDest) != fv.end()) // Must rename to avoid capturing
  {
    newDest = MpsExp::NewVar();
    MpsExp *newVar = new MpsVarExp(newDest,MpsMsgNoType());
    MpsTerm *tmpSucc = mySucc->ESubst(myDest,*newVar);
    newSucc = tmpSucc->ESubst(source,dest);
    delete tmpSucc;
    delete newVar;
  }
  else
    newSucc = mySucc->ESubst(source,dest);
  MpsTerm *result = new MpsRcv(newChannel, newDest, myPid, myMaxPid, *newSucc, *newType, GetFinal());
  delete newType;
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsRcv::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  MpsMsgType *newType = GetMsgType().GSubst(source,dest,args);
  MpsTerm *result = new MpsRcv(myChannel, myDest, myPid, myMaxPid, *newSucc, *newType, GetFinal());

  // Clean Up
  delete newType;
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsRcv::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->LSubst(source,dest,args);
  MpsMsgType * newType = GetMsgType().LSubst(source,dest,args);
  MpsTerm *result = new MpsRcv(myChannel, myDest, myPid, myMaxPid, *newSucc, *newType, GetFinal());

  // Clean Up
  delete newType;
  delete newSucc;

  return result;
} // }}}
set<string> MpsRcv::FPV() const // {{{
{
  set<string> result = mySucc->FPV();
  return result;
} // }}}
set<string> MpsRcv::FEV() const // {{{
{
  set<string> result = mySucc->FEV();
  result.erase(myDest);
  result.insert(myChannel.GetName());
  return result;
} // }}}
MpsTerm *MpsRcv::Copy() const // {{{
{
  // assert mySucc != NULL
  return new MpsRcv(myChannel, myDest, myPid, myMaxPid, *mySucc, GetMsgType(), GetFinal());
} // }}}
bool MpsRcv::Terminated() const // {{{
{
  return false;
} // }}}
MpsTerm *MpsRcv::Simplify() const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->Simplify();
  MpsTerm *result = new MpsRcv(myChannel, myDest, myPid, myMaxPid, *newSucc, GetMsgType(), GetFinal());
  delete newSucc;
  return result;
} // }}}
string MpsRcv::ToString(string indent) const // {{{
{
  string result = myChannel.ToString() + " >> " + myDest;
  if (myMaxPid!=-1)
    result += "@(" + int2string(myPid) + " of " + int2string(myMaxPid) + ")";
  result += ";\n" + indent + mySucc->ToString(indent);
  return result;
} // }}}
string MpsRcv::ToTex(int indent, int sw) const // {{{
{
  string result = ToTex_Channel(myChannel) + "$\\gg$" + myDest;
  if (myMaxPid!=-1)
    result += "\\at (" + ToTex_PP(myPid) + " " + ToTex_KW("def") + " " + ToTex_PP(myMaxPid) + ")";
  result += ";\\newline\n"
          + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
  return result;
} // }}}
string MpsRcv::ToC() const // {{{
{
  stringstream result;
  const MpsDelegateMsgType *delType=dynamic_cast<const MpsDelegateMsgType*>(&GetMsgType());
  result << "  " << GetMsgType().ToC() << " " << ToC_Name(myDest) << ";" << endl; // Declare variable
  if (delType!=NULL)
  {
    result << "    --__system_aprocs;" << endl
           << "    " << ToC_Name(myDest) << "=" << ToC_Name(myChannel.GetName()) << "->ReceiveSession(" << int2string(myChannel.GetIndex()-1) << ");" << endl
           << "    ++__system_aprocs;" << endl;
  }
  else
  {
    string msgName = ToC_Name(MpsExp::NewVar("receive")); // Create variable name foor the mmessagee to send
    result << "  { Message " << msgName << ";" << endl  // Declare message variable
           << "    --__system_aprocs;" << endl
           << "    " << ToC_Name(myChannel.GetName()) << "->Receive(" << int2string(myChannel.GetIndex()-1) << "," << msgName << ");" << endl // Receive value
           << "    ++__system_aprocs;" << endl
           << "    " << msgName << ".GetValue(" << ToC_Name(myDest) << ");" << endl
           << "  }" << endl;
  }
  if (myFinal)
  {
    result << "  " << ToC_Name(myChannel.GetName()) << "->Close(true);" << endl
           << "  delete " << ToC_Name(myChannel.GetName()) << ";" << endl;
  }
  result << mySucc->ToC();
  return result.str();
} // }}}
string MpsRcv::ToCHeader() const // {{{
{
  return mySucc->ToCHeader();
} // }}}
MpsTerm *MpsRcv::RenameAll() const // {{{
{ string newDest=MpsExp::NewVar(myDest);
  MpsTerm *tmpSucc=mySucc->ERename(myDest,newDest);
  MpsTerm *newSucc=tmpSucc->RenameAll();
  delete tmpSucc;
  MpsMsgType *newType=myType->RenameAll();
  MpsTerm *result=new MpsRcv(myChannel,newDest,myPid,myMaxPid,*newSucc,*newType, GetFinal());
  delete newSucc;
  delete newType;
  return result;
} // }}}
MpsTerm *MpsRcv::CloseDefinitions() const // {{{
{
  MpsTerm *newSucc = mySucc->CloseDefinitions();
  MpsTerm *result = new MpsRcv(myChannel, myDest, myPid, myMaxPid, *newSucc, *myType, GetFinal());
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsRcv::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ MpsTerm *newSucc=mySucc->ExtractDefinitions(env);
  MpsTerm *result=new MpsRcv(myChannel,myDest,myPid,myMaxPid,*newSucc,*myType, GetFinal());
  delete newSucc;
  return result;
} // }}}
const MpsMsgType &MpsRcv::GetMsgType() const // {{{
{ return *myType;
} // }}}
void MpsRcv::SetMsgType(const MpsMsgType &type) // {{{
{ delete myType;
  myType=type.Copy();
} // }}}
bool MpsRcv::GetFinal() const // {{{
{ return myFinal;
} // }}}
