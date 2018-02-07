#include<hapi/mpslink.hpp>
#include <hapi/mpsend.hpp>
#include <hapi/mpsgui.hpp>
#include <hapi/common.hpp>

using namespace std;
using namespace hapi;

MpsLink::MpsLink(const string &channel, const std::string &session, int pid, int maxpid, const MpsTerm &succ, bool pure) // {{{
: myChannel(channel),
  mySession(session),
  myMaxpid(maxpid),
  myPid(pid),
  myPure(pure)
{
  // Assert succ!=NULL
  mySucc = succ.Copy();
} // }}}
MpsLink::~MpsLink() // {{{
{
  // assert mySucc != NULL
  delete mySucc;
} // }}}
void *MpsLink::TDCompileMain(tdc_pre pre, tdc_post wrap, tdc_error wrap_err, const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, const string &curPure, PureState pureState, bool checkPure) // * Use rules Mcast and Macc {{{
{ map<string,void*> children;
  // Verify link
  MpsMsgEnv newGamma = Gamma;
  // Check linking on available channel
  MpsMsgEnv::iterator var=newGamma.find(myChannel);
  if (var==newGamma.end())
    return wrap_err(this,PrintTypeError((string)"Linking on unknown channel:" + myChannel,*this,Theta,Gamma,Omega),children);
  const MpsChannelMsgType *channel=dynamic_cast<const MpsChannelMsgType*>(var->second);
  if (channel==NULL)
    return wrap_err(this,PrintTypeError((string)"Linking on non-channel:" + myChannel,*this,Theta,Gamma,Omega),children);

  // Store purity of channel
  myPure=true;
  for (int i=0; myPure && i<channel->GetMaxPid(); ++i)
    if (i+1!=myPid && !channel->GetParticipants()[i].IsPure())
      myPure=false;

  // Check purity constraints
  PureState nextState=pureState;
  if (checkPure)
  { if (pureState!=CPS_IMPURE && pureState!=CPS_PURE && pureState!=CPS_SERVICE_LINK)
      return wrap_err(this,PrintTypeError("Error in implementation of pure participant " + curPure + ". Pure implementations must conform with the structure \n     *   local X()\n       *   ( global s=new ch(p of n);\n         *     X();\n         *     |\n         *     P\n         *   )\n         *   local StartX(Int i)\n         *   ( if i<=0\n         *     then X();\n         *     else X(); | StartX(i-1);\n         *   )\n         *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);

    if (pureState==CPS_SERVICE_LINK) // Apply special purity rule
    { nextState=CPS_SERVICE_FORK;
      if (!channel->GetParticipants()[myPid-1].IsPure())
        return wrap_err(this,PrintTypeError((string)"Linking as impure participant not allowed here",*this,Theta,Gamma,Omega),children);
    }
    else
    { // Check if linking breaks purity
      if (pureState==CPS_IMPURE && channel->GetParticipants()[myPid-1].IsPure())
        return wrap_err(this,PrintTypeError((string)"Linking as pure participant not allowed here",*this,Theta,Gamma,Omega),children);

      if (pureStack.size()>0)
        return wrap_err(this,PrintTypeError("Implementation of pure participant " + int2string(pureStack.begin()->second) + "@" + pureStack.begin()->first + " must be immediately after its decleration",*this,Theta,Gamma,Omega),children);

      if (pureState==CPS_PURE && !myPure)
        return wrap_err(this,PrintTypeError((string)"Unpure link in pure context is not allowed",*this,Theta,Gamma,Omega),children);
    }
  }

  // Check correct maxpid
  if (myMaxpid != channel->GetGlobalType()->GetMaxPid())
    return wrap_err(this,PrintTypeError((string)"MaxPID is different from:" + int2string(channel->GetGlobalType()->GetMaxPid()),*this,Theta,Gamma,Omega),children);

  // Check that only completed sessions are hidden
  var=newGamma.find(mySession);
  if (var!=newGamma.end())
  { const MpsDelegateMsgType *session=dynamic_cast<const MpsDelegateMsgType*>(var->second);
    if (session!=NULL &&
        !session->GetLocalType()->Equal(Theta,MpsLocalEndType()))
      return wrap_err(this,PrintTypeError((string)"Linking on open session:" + mySession,*this,Theta,Gamma,Omega),children);

    newGamma.erase(var);
  }
  // Create local type
  MpsLocalType *newType=channel->GetGlobalType()->Project(myPid);
  set<string> fv = newType->FEV();
  // Rename all free variables
  for (set<string>::const_iterator it=fv.begin(); it!=fv.end(); ++it)
  { MpsLocalType *tmpType=newType->ERename(*it,MpsExp::NewVar(*it));
    delete newType;
    newType=tmpType;
  }
  // Create Gamma with new session
  newGamma[mySession] = new MpsDelegateLocalMsgType(*newType,myPid,channel->GetParticipants());
  delete newType;

  children["succ"] = mySucc->TDCompile(pre,wrap,wrap_err,Theta,newGamma,Omega,pureStack,curPure,nextState,checkPure);

  // Clean up
  delete newGamma[mySession];

  // Wrap result
  return wrap(this,Theta,Gamma,Omega,pureStack,curPure,pureState,checkPure,children);
} // }}}
MpsTerm *MpsLink::ApplyLink(const std::vector<std::string> &paths, const std::string &session) const // {{{
{ if (paths.size()==0)
    return Copy();
  else if (paths.size()>1)
    return Error((string)"Applying Link on "+ToString());

  if (myPid==1) // Only first participant registers session
    mpsgui::CreateSession(session,myMaxpid); // Inform GUI
  // Renamimg session channel to common name
  MpsTerm *tmpResult = mySucc->ERename(mySession,session);
  // Reindexing session
  // session[i] << exp becomes session[myMaxpid*i+myPid] << exp, and
  // session[i] >> var becomes session[myMaxpid*myPid+i] >> var.
  MpsTerm *result = tmpResult->ReIndex(session,myPid,myMaxpid);
  delete tmpResult;
  return result;
} // }}}
bool MpsLink::SubSteps(vector<MpsStep> &dest) // {{{
{
  MpsEvent event;
  event.myType    = link;
  event.myChannel = myChannel;
  event.myMaxpid  = myMaxpid;
  event.myPid     = myPid;
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event,paths));
  return false;
} // }}}
MpsTerm *MpsLink::PRename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PRename(src,dst);
  MpsTerm *result = new MpsLink(myChannel, mySession, myPid, myMaxpid, *newSucc, myPure);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsLink::ERename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  string newChannel = myChannel==src?dst:myChannel;
  if (mySession == src) // No further substitution needed
    return new MpsLink(newChannel, mySession, myPid, myMaxpid, *mySucc, myPure);

  string newSession = mySession;
  MpsTerm *newSucc = NULL;
  if (mySession==dst)
  {
    newSession = MpsExp::NewVar(mySession);
    MpsTerm *tmpSucc = mySucc->ERename(mySession, newSession);
    newSucc = tmpSucc->ERename(src,dst);
    delete tmpSucc;
  }
  else
    newSucc = mySucc->ERename(src,dst);

  MpsTerm *result = new MpsLink(newChannel, newSession, myPid, myMaxpid, *newSucc, myPure);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsLink::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  // assert mySucc != NULL
  if (mySession == session) // No further substitution needed
    return new MpsLink(myChannel, mySession, myPid, myMaxpid, *mySucc, myPure);

  MpsTerm *newSucc =  mySucc->ReIndex(session,pid,maxpid);

  MpsTerm *result = new MpsLink(myChannel, mySession, myPid, myMaxpid, *newSucc, myPure);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsLink::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PSubst(var,exp,args,argpids,stateargs);
  MpsTerm *result = new MpsLink(myChannel, mySession, myPid, myMaxpid, *newSucc, myPure);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsLink::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  // assert mySucc != NULL
  string newChannel = stringESubst(myChannel,source,dest);
  if (mySession == source) // No further substitution needed
    return new MpsLink(newChannel, mySession, myPid, myMaxpid, *mySucc, myPure);

  set<string> fv=dest.FV();
  string newSession = mySession;
  MpsTerm *newSucc = NULL;
  if (fv.find(mySession) != fv.end())
  {
    newSession = MpsExp::NewVar();
    MpsExp *newVar = new MpsVarExp(newSession,MpsMsgNoType());
    MpsTerm *tmpSucc = mySucc->ESubst(mySession, *newVar);
    newSucc = tmpSucc->ESubst(source,dest);
    delete newVar;
    delete tmpSucc;
  }
  else
    newSucc = mySucc->ESubst(source,dest);

  MpsTerm *result = new MpsLink(newChannel, newSession, myPid, myMaxpid, *newSucc, myPure);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsLink::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  MpsTerm *result = new MpsLink(myChannel,mySession,myPid,myMaxpid,*newSucc, myPure);

  // Clean Up
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsLink::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->LSubst(source,dest,args);
  MpsTerm *result = new MpsLink(myChannel,mySession,myPid,myMaxpid,*newSucc, myPure);

  // Clean Up
  delete newSucc;

  return result;
} // }}}
set<string> MpsLink::FPV() const // {{{
{
  set<string> result = mySucc->FPV();
  return result;
} // }}}
set<string> MpsLink::EV() const // {{{
{
  set<string> result = mySucc->EV();
  result.insert(mySession);
  result.insert(myChannel);
  return result;
} // }}}
set<string> MpsLink::FEV() const // {{{
{
  set<string> result = mySucc->FEV();
  result.erase(mySession);
  result.insert(myChannel);
  return result;
} // }}}
MpsTerm *MpsLink::Copy() const // {{{
{
return new MpsLink(myChannel, mySession, myPid, myMaxpid, *mySucc, myPure);
} // }}}
bool MpsLink::Terminated() const // {{{
{
  return false;
} // }}}
MpsTerm *MpsLink::Simplify() const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->Simplify();
  MpsTerm *result = new MpsLink(myChannel, mySession, myPid, myMaxpid, *newSucc, myPure);
  delete newSucc;
  return result;
} // }}}
string MpsLink::ToString(string indent) const // {{{
{
  return mySession + "= new " + myChannel + "(" + int2string(myPid) + " of " + int2string(myMaxpid) + ");\n"
       + indent + mySucc->ToString(indent);
} // }}}
string MpsLink::ToTex(int indent, int sw) const // {{{
{
  return ToTex_KW("link") + "(" + ToTex_PP(myMaxpid) + "," + ToTex_ChName(myChannel) + "," + ToTex_Session(mySession) + "," + ToTex_PP(myPid) + ");\\newline\n"
       + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
} // }}}
string MpsLink::ToC(const string &taskType) const // {{{
{
  stringstream result;
  result << ToC_Yield();
  if (myPid==1) // Orchestrate linking
  {
    vector<string> rcvLabels;
    for (size_t i=0; i<myMaxpid-1; ++i)
      rcvLabels.push_back(ToC_Name(MpsExp::NewVar(string("receive_")+taskType)));
    result
    << "    { // link" << endl
    << "      _task->tmps.clear();" << endl;
    for (size_t i=0; i<myMaxpid-1; ++i)
      result
      << "        _task->SetLabel(&&" << rcvLabels[i] << ");" << endl
      << "        if (!((libpi::task::Link*)_this->var_" << ToC_Name(myChannel) << ".get())->GetChannels()[" << i << "]->Receive(_task,_task->tmp))" << endl
      << "          return false;" << endl
      << "        " << rcvLabels[i] << ":" << endl
      << "        _task->tmps.push_back(_task->tmp);" << endl;
    result
    << "      vector<vector<shared_ptr<libpi::Channel> > > inChannels(" << myMaxpid << ");" << endl
    << "      vector<vector<shared_ptr<libpi::Channel> > > outChannels(" << myMaxpid << ");" << endl
    << "      // Optimize vector inserts" << endl;
    for (size_t i=0; i<myMaxpid; ++i)
    { result
      << "        inChannels[" << i << "].resize(" << myMaxpid << ");" << endl
      << "        outChannels[" << i << "].resize(" << myMaxpid << ");" << endl;
    }
    result
    << "      // Create channels" << endl;
    for (size_t i=0; i<myMaxpid; ++i)
      for (size_t j=0; j<myMaxpid; ++j)
        if (i>0 && j==0)
          result << "      inChannels[" << i << "][" << j << "]=dynamic_pointer_cast<libpi::Channel>(_task->tmps[" << i-1 << "]);" << endl;
        else
          result << "      inChannels[" << i << "][" << j<< "].reset(new libpi::task::Channel());" << endl;
    for (size_t i=0; i<myMaxpid; ++i)
      for (size_t j=0; j<myMaxpid; ++j)
        result << "      outChannels[" << i << "][" << j << "]=inChannels[" << j << "][" << i << "];" << endl;
    result
    << "      // Send sessions" << endl;
    for (size_t i=1; i<myMaxpid; ++i)
      result
      << "      { shared_ptr<libpi::Session> _s(new libpi::Session(" << i << "," << myMaxpid << ", inChannels[" << i << "], outChannels[" << i << "]));" << endl
      << "        ((libpi::Channel*)_task->tmps[" << i-1 << "].get())->Send(_task,_s);" << endl
      << "      }" << endl;
    result
    << "      // Create local session" << endl
    << "      _this->var_" << ToC_Name(mySession) << ".reset(new libpi::Session(0," << myMaxpid << ",inChannels[0],outChannels[0]));" << endl
    << "    }" << endl;
  }
  else // Send Channel and receive session
  {
    string lblRcv(ToC_Name(MpsExp::NewVar(string("receive_")+taskType)));
    result
    << "    { _task->tmp.reset(new libpi::task::Channel);" << endl
    << "      ((libpi::task::Link*)(_this->var_" << ToC_Name(myChannel) << ".get()))->GetChannels()[" << myPid-2 << "]->Send(_task,_task->tmp);" << endl
    << "      _task->SetLabel(&&" << lblRcv << ");" << endl
    << "      if (!((libpi::task::Channel*)_task->tmp.get())->Receive(_task,_this->var_" << ToC_Name(mySession) << "))" << endl
    << "        return false;" << endl
    << "      " << lblRcv << ":" << endl
    << "      _task->tmp.reset();" << endl
    << "    }" << endl;
  }
  result << mySucc->ToC(taskType);
  return result.str();
} // }}}
string MpsLink::ToCHeader() const // {{{
{
  return mySucc->ToCHeader();
} // }}}
void MpsLink::ToCConsts(vector<string> &dest, unordered_set<string> &existing) const // {{{
{ mySucc->ToCConsts(dest,existing);
} // }}}
MpsTerm *MpsLink::FlattenFork(bool normLhs, bool normRhs, bool pureMode) const // {{{
{
  MpsTerm *newSucc = mySucc->FlattenFork(normLhs,normRhs,pureMode);
  MpsTerm *result= new MpsLink(myChannel, mySession, myPid, myMaxpid, *newSucc, myPure);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsLink::RenameAll() const // {{{
{ string newSession=MpsExp::NewVar(mySession);
  MpsTerm *tmpSucc=mySucc->ERename(mySession,newSession);
  MpsTerm *newSucc=tmpSucc->RenameAll();
  delete tmpSucc;
  MpsTerm *result=new MpsLink(myChannel,newSession,myPid,myMaxpid,*newSucc, myPure);
  
  delete newSucc;

  return result;
} // }}}
bool MpsLink::Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const // {{{
{ MpsTerm *pre;
  MpsTerm *post;
  if (myPure) // Only optimize if pure
  { set<string> fv;
    fv.insert(myChannel);
    fv.insert(mySession);
    receives.Split(fv,pre,post);
  }
  else
  { pre=receives.Copy();
    post=new MpsEnd();
  }
  bool opt1 = dynamic_cast<const MpsEnd*>(post)==NULL;
  // Parallelize succ
  MpsTerm *seqSucc;
  MpsTerm *parSucc;
  bool opt2 = mySucc->Parallelize(*post,seqSucc,parSucc);
  delete post;
  // Create seqTerm as link;seqSucc
  seqTerm=new MpsLink(myChannel, mySession, myPid, myMaxpid, *seqSucc, myPure);
  delete seqSucc;
  // Create parTerm as pre;link;parSucc
  MpsTerm *parTmp=new MpsLink(myChannel, mySession, myPid, myMaxpid, *parSucc, myPure);
  delete parSucc;
  parTerm=pre->Append(*parTmp);
  delete parTmp;
  delete pre;
  return opt1 || opt2;
} // }}}
MpsTerm *MpsLink::Append(const MpsTerm &term) const // {{{
{ MpsTerm *newSucc=mySucc->Append(term);
  MpsTerm *result=new MpsLink(myChannel, mySession, myPid, myMaxpid, *newSucc, myPure);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsLink::CopyWrapper(std::map<std::string,void*> &children) const // {{{
{
  return new MpsLink(myChannel, mySession, myPid, myMaxpid, *(MpsTerm*)children["succ"], myPure);
} // }}}
MpsTerm *MpsLink::CloseDefsPre(const MpsMsgEnv &Gamma) // {{{
{ return this;
} // }}}
MpsTerm *MpsLink::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ MpsTerm *newSucc=mySucc->ExtractDefinitions(env);
  MpsTerm *result=new MpsLink(myChannel,mySession,myPid,myMaxpid,*newSucc, myPure);
  
  delete newSucc;

  return result;
} // }}}
