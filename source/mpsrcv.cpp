#include<hapi/mpsrcv.hpp>
#include<hapi/mpsend.hpp>
#include <hapi/common.hpp>

using namespace std;
using namespace hapi;

MpsRcv::MpsRcv(const MpsChannel &channel, const string &dest, int pid, int maxpid, const MpsTerm &succ, const MpsMsgType &type, bool final) // {{{
: myChannel(channel)
, myDest(dest)
, myPid(pid)
, myMaxPid(maxpid)
, myFinal(final)
{
  mySucc=succ.Copy();
  myType=type.Copy();
} // }}}
MpsRcv::~MpsRcv() // {{{
{
  delete myType;
  delete mySucc;
} // }}}
void *MpsRcv::TDCompileMain(tdc_pre pre, tdc_post wrap, tdc_error wrap_err, const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, const string &curPure, PureState pureState, bool checkPure) // Use rules Rcv and Srec {{{
{ map<string,void*> children;
  // Check purity constraints
  if (checkPure)
	{ if (pureStack.size()>0)
      return wrap_err(this,PrintTypeError("Implementation of pure participant " + int2string(pureStack.begin()->second) + "@" + pureStack.begin()->first + " must be immediately after its decleration",*this,Theta,Gamma,Omega),children);

    if (pureState!=CPS_IMPURE && pureState!=CPS_PURE)
      return wrap_err(this,PrintTypeError("Error in implementation of pure participant " + curPure + ". Pure implementations must conform with the structure \n     *   local X()\n	   *   ( global s=new ch(p of n);\n		 *     X();\n		 *     |\n		 *     P\n		 *   )\n		 *   local StartX(Int i)\n		 *   ( if i<=0\n		 *     then X();\n		 *     else X(); | StartX(i-1);\n		 *   )\n		 *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);
  }
 
  // Verify rcv
  MpsMsgEnv::const_iterator session=Gamma.find(myChannel.GetName());
  // Check session is open
  if (session==Gamma.end())
    return wrap_err(this,PrintTypeError((string)"Receiving on closed session: " + myChannel.GetName(),*this,Theta,Gamma,Omega),children);
  // Check if session type
  const MpsDelegateMsgType *msgType = dynamic_cast<const MpsDelegateMsgType*>(session->second);
  if (msgType==NULL)
    return wrap_err(this,PrintTypeError((string)"Sending on non-session type: " + myChannel.GetName(),*this,Theta,Gamma,Omega),children);
  // Check if unfolding is necessary
  MpsLocalType *localMsgType=msgType->CopyLocalType();
  const MpsLocalRecType *recType = dynamic_cast<const MpsLocalRecType*>(localMsgType);
  if (recType!=NULL)
  { delete localMsgType;
    return TypeCheckRec(pre, wrap, wrap_err, Theta,Gamma, Omega, pureStack, curPure, pureState, checkPure, *this, session->first);
  }
  const MpsLocalForallType *allType = dynamic_cast<const MpsLocalForallType*>(localMsgType);
  if (allType!=NULL)
  { delete localMsgType;
    return TypeCheckForall(pre, wrap, wrap_err, Theta, Gamma, Omega, pureStack, curPure, pureState, checkPure, *this, session->first);
  }
  // Check session has receive type
  const MpsLocalRcvType *rcvType = dynamic_cast<const MpsLocalRcvType*>(localMsgType);
  if (rcvType==NULL)
  { delete localMsgType;
    return wrap_err(this,PrintTypeError((string)"Receiving on session: " + myChannel.GetName(),*this,Theta,Gamma,Omega),children);
  }
  // Check channel index is correct
  if (myChannel.GetIndex() != rcvType->GetSender())
  { delete localMsgType;
    return wrap_err(this,PrintTypeError((string)"Receiving on session(wrong index): " + myChannel.ToString(),*this,Theta,Gamma,Omega),children);
  }
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
      { MpsLocalType *localDelType=delType->CopyLocalType();
        if (rename)
        { newType=localDelType->ERename(myDest,newDest);
          delete localDelType;
        }
        else
          newType=localDelType;
      }
      newGamma[it->first] = new MpsDelegateLocalMsgType(*newType,delType->GetPid(),delType->GetParticipants());
      delete newType;
    }
  }
  // Save created type for deletion
  MpsDelegateMsgType *newType=dynamic_cast<MpsDelegateMsgType*>(newGamma[myChannel.GetName()]);
  // Check if assertion domain is respected
  if (rcvType->GetAssertionType() && typeid(*rcvType->GetMsgType()) != typeid(MpsBoolMsgType))
  { string rcvTypeStr=rcvType->ToString("!!!!!      ");
    delete localMsgType;
    return wrap_err(this,PrintTypeError((string)"Assertion of non-boolean type: " + rcvTypeStr,*this,Theta,Gamma,Omega),children);
  }

  // Check not overwriting unfinished session
  MpsMsgEnv::const_iterator dstVar=Gamma.find(myDest);
  if (dstVar!=Gamma.end() &&
      dynamic_cast<const MpsDelegateMsgType*>(dstVar->second)!=NULL)
  { MpsLocalType *localDstVar=dynamic_cast<const MpsDelegateMsgType*>(dstVar->second)->CopyLocalType();
    bool isClosed=localDstVar->Equal(Theta,MpsLocalEndType());
    delete localDstVar;
    if (!isClosed)
    { delete localMsgType;
      return wrap_err(this,PrintTypeError((string)"Overwriting open session: " + myDest,*this,Theta,Gamma,Omega),children);
    }
  }
  // Check specification of pid and maxpid
  const MpsDelegateMsgType *delRcvType=dynamic_cast<const MpsDelegateMsgType*>(rcvType->GetMsgType());
  if (delRcvType!=NULL)
  { if (myPid==-1)
      myPid=delRcvType->GetPid();
    else if (delRcvType->GetPid()!=myPid)
    { delete localMsgType;
      return wrap_err(this,PrintTypeError((string)"Receiving session with pid different than specified",*this,Theta,Gamma,Omega),children);
    }
    if (myMaxPid==-1)
      myMaxPid=delRcvType->GetMaxpid();
    else if (delRcvType->GetMaxpid()!=myMaxPid)
    { delete localMsgType;
      return wrap_err(this,PrintTypeError((string)"Receiving session with maxpid different than specified",*this,Theta,Gamma,Omega),children);
    }
  }
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
  children["succ"] = mySucc->TDCompile(pre,wrap,wrap_err,*newTheta,newGamma,Omega,pureStack,curPure,pureState,checkPure);
  // Store if this is final action in session
  MpsLocalType *localNewType=newType->CopyLocalType();
  myFinal=localNewType->IsDone();
  delete localNewType;
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
  // Wrap result
  delete localMsgType;
  return wrap(this,Theta,Gamma,Omega,pureStack,curPure,pureState,checkPure,children);
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
MpsTerm *MpsRcv::MRename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  MpsMsgType *newType = GetMsgType().MRename(src,dst);

  MpsTerm *newSucc=mySucc->MRename(src,dst);
  MpsTerm *result = new MpsRcv(myChannel, myDest, myPid, myMaxPid, *newSucc, *newType, GetFinal());
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
set<string> MpsRcv::EV() const // {{{
{
  set<string> result = mySucc->EV();
  result.insert(myDest);
  result.insert(myChannel.GetName());
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
    result += "(" + int2string(myPid) + " of " + int2string(myMaxPid) + ")";
  result += ";\n" + indent + mySucc->ToString(indent);
  return result;
} // }}}
string MpsRcv::ToTex(int indent, int sw) const // {{{
{
  string result = myChannel.ToTex() + "$\\gg$" + myDest;
  if (myMaxPid!=-1)
    result += "\\at (" + ToTex_PP(myPid) + " " + ToTex_KW("def") + " " + ToTex_PP(myMaxPid) + ")";
  result += ";\\newline\n"
          + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
  return result;
} // }}}
string MpsRcv::ToC(const std::string &taskType) const // {{{
{
  string rcvLabel = ToC_Name(MpsExp::NewVar("receive"));
  stringstream result;
  result
    << ToC_Yield()
    << "    // " << myChannel.GetName() << "[" << myChannel.GetIndex() << "] >> " << myDest << ";" << endl
    << "    _task->SetLabel(&&" << rcvLabel << ");" << endl
    << "    if (!((libpi::Session*)_this->var_" << ToC_Name(myChannel.GetName()) << ".get())->Receive(" << myChannel.GetIndex()-1 << ",_task,_this->var_" << ToC_Name(myDest) << "))" << endl
    << "      return false;" << endl
    << "    " << rcvLabel << ":" << endl;
  if (myFinal)
  {
    result
      << "    // Session complete" << endl
      << "    ((libpi::Session*)_this->var_" << ToC_Name(myChannel.GetName()) << ".get())->Close(true);" << endl
      << "    _this->var_" << ToC_Name(myChannel.GetName()) << ".reset();" << endl;
  }
  result << mySucc->ToC(taskType);
  return result.str();
} // }}}
string MpsRcv::ToCHeader() const // {{{
{
  return mySucc->ToCHeader();
} // }}}
void MpsRcv::ToCConsts(vector<string> &dest, unordered_set<string> &existing) const // {{{
{ mySucc->ToCConsts(dest,existing);
} // }}}
MpsTerm *MpsRcv::FlattenFork(bool normLhs, bool normRhs, bool pureMode) const // {{{
{
  MpsTerm *newSucc = mySucc->FlattenFork(normLhs,normRhs,pureMode);
  MpsTerm *result = new MpsRcv(myChannel, myDest, myPid, myMaxPid, *newSucc, *myType, GetFinal());
  delete newSucc;

  return result;
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
bool MpsRcv::Parallelize(const MpsTerm &receivers, MpsTerm* &seqTerm, MpsTerm* &parTerm) const // {{{
{
  // Create updated receivers
  MpsTerm *rcvTerm = new MpsRcv(myChannel, myDest, myPid, myMaxPid, MpsEnd(), GetMsgType(), GetFinal());
  MpsTerm *newReceivers = receivers.Append(*rcvTerm);
  delete rcvTerm;
  // Perform parallelization of succ
  MpsTerm *seqSucc;
  bool opt = mySucc->Parallelize(*newReceivers,seqSucc,parTerm);
  delete newReceivers;
  // Create seqTerm as rcv;seqSucc
  seqTerm = new MpsRcv(myChannel, myDest, myPid, myMaxPid, *seqSucc, GetMsgType(), GetFinal());
  delete seqSucc;
  return opt;
} // }}}
MpsTerm *MpsRcv::Append(const MpsTerm &term) const // {{{
{
  MpsTerm *newSucc=mySucc->Append(term);
  MpsTerm *result=new MpsRcv(myChannel, myDest, myPid, myMaxPid, *newSucc, GetMsgType(), GetFinal());
  delete newSucc;
  return result;
} // }}}
void MpsRcv::Split(const std::set<std::string> &fv, MpsTerm* &pre, MpsTerm* &post) const // {{{
{ if (fv.find(myChannel.GetName())!=fv.end() ||
      fv.find(myDest)!=fv.end()) // Cannot postpone
  { MpsTerm *succPre;
    mySucc->Split(fv,succPre,post);
    pre = new MpsRcv(myChannel, myDest, myPid, myMaxPid, *succPre, GetMsgType(), GetFinal());
    delete succPre;
  }
  else
  { // First split succ
    MpsTerm *succPre, *succPost;
    mySucc->Split(fv,succPre,succPost);
    // Then split pre on the vars used int this receive
    set<string> preFV;
    preFV.insert(myChannel.GetName());
    preFV.insert(myDest);
    MpsTerm *prePre, *prePost;
    succPre->Split(preFV,prePre,prePost);
    delete prePost;
    if (dynamic_cast<const MpsEnd*>(prePre)==NULL)
    { // This op cannot be moved to post
      delete prePre;
      pre = new MpsRcv(myChannel, myDest, myPid, myMaxPid, *succPre, GetMsgType(), GetFinal());
      delete succPre;
      post=succPost;
    }
    else
    { // Move this op to post
      delete prePre;
      post = new MpsRcv(myChannel, myDest, myPid, myMaxPid, *succPost, GetMsgType(), GetFinal());
      delete succPost;
      pre = succPre;
    }
  }
} //}}}
MpsTerm *MpsRcv::CopyWrapper(std::map<std::string,void*> &children) const // {{{
{
  return new MpsRcv(myChannel, myDest, myPid, myMaxPid, *(MpsTerm*)children["succ"], GetMsgType(), GetFinal());
} // }}}
MpsTerm *MpsRcv::CloseDefsPre(const MpsMsgEnv &Gamma) // {{{
{
  return this;
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
