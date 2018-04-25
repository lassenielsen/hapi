#include<hapi/mpsrcvtype.hpp>
#include<hapi/mpsend.hpp>
#include <hapi/common.hpp>

using namespace std;
using namespace hapi;

MpsRcvType::MpsRcvType(const string &session, const string &dest, const MpsTerm &succ, bool final) // {{{
: mySession(session)
, myDest(dest)
, myFinal(final)
{
  mySucc=succ.Copy();
} // }}}
MpsRcvType::~MpsRcvType() // {{{
{
  delete mySucc;
} // }}}
void *MpsRcvType::TDCompileMain(tdc_pre pre, tdc_post wrap, tdc_error wrap_err, const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, const string &curPure, PureState pureState, bool checkPure) // Use rules Rcv and Srec {{{
{ map<string,void*> children;
  // Check purity constraints
  if (checkPure)
	{ if (pureStack.size()>0)
      return wrap_err(this,PrintTypeError("Implementation of pure participant " + int2string(pureStack.begin()->second) + "@" + pureStack.begin()->first + " must be immediately after its decleration",*this,Theta,Gamma,Omega),children);

    if (pureState!=CPS_IMPURE && pureState!=CPS_PURE)
      return wrap_err(this,PrintTypeError("Error in implementation of pure participant " + curPure + ". Pure implementations must conform with the structure \n     *   local X()\n	   *   ( global s=new ch(p of n);\n		 *     X();\n		 *     |\n		 *     P\n		 *   )\n		 *   local StartX(Int i)\n		 *   ( if i<=0\n		 *     then X();\n		 *     else X(); | StartX(i-1);\n		 *   )\n		 *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);
  }
 
  // Verify rcv
  MpsMsgEnv::const_iterator session=Gamma.find(mySession);
  // Check session is open
  if (session==Gamma.end())
    return wrap_err(this,PrintTypeError((string)"Receiving on closed session: " + mySession,*this,Theta,Gamma,Omega),children);
  // Check if session type
  const MpsDelegateMsgType *msgType = dynamic_cast<const MpsDelegateMsgType*>(session->second);
  if (msgType==NULL)
    return wrap_err(this,PrintTypeError((string)"Sending on non-session type: " + mySession,*this,Theta,Gamma,Omega),children);
  // Check if unfolding is necessary
  const MpsLocalRecType *recType = dynamic_cast<const MpsLocalRecType*>(msgType->GetLocalType());
  if (recType!=NULL)
    return TypeCheckRec(pre, wrap, wrap_err, Theta,Gamma, Omega, pureStack, curPure, pureState, checkPure, *this, session->first);
  const MpsLocalForallType *allType = dynamic_cast<const MpsLocalForallType*>(msgType->GetLocalType());
  if (allType!=NULL)
    return TypeCheckForall(pre, wrap, wrap_err, Theta, Gamma, Omega, pureStack, curPure, pureState, checkPure, *this, session->first);
  // Check session has receive type
  const MpsLocalTypeRcvType *rcvType = dynamic_cast<const MpsLocalTypeRcvType*>(msgType->GetLocalType());
  if (rcvType==NULL)
    return wrap_err(this,PrintTypeError((string)"Type-receiving on session: " + mySession,*this,Theta,Gamma,Omega),children);
  // Make new Gamma
  MpsMsgEnv newGamma;
  for (MpsMsgEnv::const_iterator it=Gamma.begin(); it!=Gamma.end(); ++it)
  { const MpsDelegateMsgType *delType=dynamic_cast<const MpsDelegateMsgType*>(it->second);
    if (delType==NULL) // Not session type
      newGamma[it->first]=it->second;
    else // SESSION TYPE
    { MpsLocalType *newType=NULL;
      if (it->first==mySession)
      { string newId=MpsExp::NewVar(myDest);
        newType=rcvType->GetSucc()->MRename(myDest,newId);
      }
      else
        newType=delType->GetLocalType()->Copy();
      newGamma[it->first] = new MpsDelegateLocalMsgType(*newType,delType->GetPid(),delType->GetParticipants());
      delete newType;
    }
  }
  // Save created type for deletion
  MpsDelegateMsgType *newType=dynamic_cast<MpsDelegateMsgType*>(newGamma[mySession]);

  // Check rest of program
  children["succ"] = mySucc->TDCompile(pre,wrap,wrap_err,Theta,newGamma,Omega,pureStack,curPure,pureState,checkPure);
  // Store if this is final action in session
  myFinal=newType->GetLocalType()->IsDone();
  // Clean Up
  delete newType;

  // Wrap result
  return wrap(this,Theta,Gamma,Omega,pureStack,curPure,pureState,checkPure,children);
} // }}}
bool MpsRcvType::SubSteps(vector<MpsStep> &dest) // {{{
{
  MpsEvent event;
  event.myType = tau;
  event.mySubType = rcvtype;
  event.mySession = mySession;
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event, paths));
  return false;
} // }}}
MpsTerm *MpsRcvType::ApplyOther(const std::string &path) const // {{{
{ if (path.size()>0)
    return Error((string)"Applying RcvType on "+ToString()+" with nonempty path "+path);
  MpsTerm *result=NULL;
  string newVar=MpsExp::NewVar(myDest);
  result = mySucc->MRename(myDest,newVar);
  return result;
} // }}}
MpsTerm *MpsRcvType::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  // assert mySucc != NULL
  if (session == myDest) // No substitution is needed in successor
    return new MpsRcvType(mySession, myDest, *mySucc, GetFinal());

  MpsTerm *newSucc = mySucc->ReIndex(session,pid,maxpid);
  MpsTerm *result = new MpsRcvType(mySession, myDest, *newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsRcvType::PRename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PRename(src,dst);
  MpsTerm *result = new MpsRcvType(mySession, myDest, *newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsRcvType::ERename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  string newSession=mySession==src?dst:mySession;
  if (src == myDest || dst==myDest) // Unexpected variable overlap
    throw string("MpsRcvType::ERename name collision with NLVar: ")+ myDest;

  MpsTerm *newSucc=mySucc->ERename(src,dst);
  MpsTerm *result = new MpsRcvType(newSession, myDest, *newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsRcvType::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
{
  MpsTerm *newSucc = mySucc->PSubst(var, exp, args, argpids, stateargs);
  MpsTerm *result = new MpsRcvType(mySession, myDest, *newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsRcvType::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  string newSession=stringESubst(mySession,source,dest);
  if (source == myDest || dest.FV().count(myDest)>0) // Unexpected variable overlap
    throw string("MpsRcvType::ERename name collision with NLVar: ")+ myDest;

  MpsTerm *newSucc=mySucc->ESubst(source,dest);
  MpsTerm *result=new MpsRcvType(newSession, myDest, *newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsRcvType::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  MpsTerm *result = new MpsRcvType(mySession, myDest, *newSucc, GetFinal());

  // Clean Up
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsRcvType::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc=mySucc->LSubst(source,dest,args);
  MpsTerm *result=new MpsRcvType(mySession, myDest, *newSucc, GetFinal());

  // Clean Up
  delete newSucc;

  return result;
} // }}}
set<string> MpsRcvType::FPV() const // {{{
{
  set<string> result = mySucc->FPV();
  return result;
} // }}}
set<string> MpsRcvType::EV() const // {{{
{
  set<string> result = mySucc->EV();
  // result.insert(myDest); // Not Expression var!
  result.insert(mySession);
  return result;
} // }}}
set<string> MpsRcvType::FEV() const // {{{
{
  set<string> result = mySucc->FEV();
  // result.erase(myDest); // No effect
  result.insert(mySession);
  return result;
} // }}}
MpsTerm *MpsRcvType::Copy() const // {{{
{
  return new MpsRcvType(mySession, myDest, *mySucc, GetFinal());
} // }}}
bool MpsRcvType::Terminated() const // {{{
{
  return false;
} // }}}
MpsTerm *MpsRcvType::Simplify() const // {{{
{
  MpsTerm *newSucc = mySucc->Simplify();
  MpsTerm *result = new MpsRcvType(mySession, myDest, *newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
string MpsRcvType::ToString(string indent) const // {{{
{
  string result = mySession + " >> " + myDest;
  result += ";\n" + indent + mySucc->ToString(indent);
  return result;
} // }}}
string MpsRcvType::ToTex(int indent, int sw) const // {{{
{
  string result = mySession + "$\\gg$" + myDest;
  result += ";\\newline\n"
          + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
  return result;
} // }}}
string MpsRcvType::ToC(const std::string &taskType) const // {{{
{ // No-op statement
  stringstream result;
  result << mySucc->ToC(taskType);
  return result.str();
} // }}}
string MpsRcvType::ToCHeader() const // {{{
{
  return mySucc->ToCHeader();
} // }}}
void MpsRcvType::ToCConsts(vector<string> &dest, unordered_set<string> &existing) const // {{{
{ mySucc->ToCConsts(dest,existing);
} // }}}
MpsTerm *MpsRcvType::FlattenFork(bool normLhs, bool normRhs, bool pureMode) const // {{{
{
  MpsTerm *newSucc = mySucc->FlattenFork(normLhs,normRhs,pureMode);
  MpsTerm *result = new MpsRcvType(mySession, myDest, *newSucc, GetFinal());
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsRcvType::RenameAll() const // {{{
{ string newDest=MpsExp::NewVar(myDest);
  MpsTerm *tmpSucc=mySucc->MRename(myDest,newDest);
  MpsTerm *newSucc=tmpSucc->RenameAll();
  delete tmpSucc;
  MpsTerm *result=new MpsRcvType(mySession,newDest,*newSucc,GetFinal());
  delete newSucc;
  return result;
} // }}}
bool MpsRcvType::Parallelize(const MpsTerm &receivers, MpsTerm* &seqTerm, MpsTerm* &parTerm) const // {{{
{
  // Create updated receivers
  MpsTerm *rcvTerm = new MpsRcvType(mySession, myDest, MpsEnd(), GetFinal());
  MpsTerm *newReceivers = receivers.Append(*rcvTerm);
  delete rcvTerm;
  // Perform parallelization of succ
  MpsTerm *seqSucc;
  bool opt = mySucc->Parallelize(*newReceivers,seqSucc,parTerm);
  delete newReceivers;
  // Create seqTerm as rcv;seqSucc
  seqTerm = new MpsRcvType(mySession, myDest, *seqSucc, GetFinal());
  delete seqSucc;
  return opt;
} // }}}
MpsTerm *MpsRcvType::Append(const MpsTerm &term) const // {{{
{
  MpsTerm *newSucc=mySucc->Append(term);
  MpsTerm *result=new MpsRcvType(mySession, myDest, *newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
void MpsRcvType::Split(const std::set<std::string> &fv, MpsTerm* &pre, MpsTerm* &post) const // {{{
{ if (fv.find(mySession)!=fv.end() ||
      fv.find(myDest)!=fv.end()) // Cannot postpone
  { MpsTerm *succPre;
    mySucc->Split(fv,succPre,post);
    pre = new MpsRcvType(mySession, myDest, *succPre, GetFinal());
    delete succPre;
  }
  else
  { // First split succ
    MpsTerm *succPre, *succPost;
    mySucc->Split(fv,succPre,succPost);
    // Then split pre on the vars used int this receive
    set<string> preFV;
    preFV.insert(mySession);
    preFV.insert(myDest);
    MpsTerm *prePre, *prePost;
    succPre->Split(preFV,prePre,prePost);
    delete prePost;
    if (dynamic_cast<const MpsEnd*>(prePre)==NULL)
    { // This op cannot be moved to post
      delete prePre;
      pre = new MpsRcvType(mySession, myDest, *succPre, GetFinal());
      delete succPre;
      post=succPost;
    }
    else
    { // Move this op to post
      delete prePre;
      post = new MpsRcvType(mySession, myDest, *succPost, GetFinal());
      delete succPost;
      pre = succPre;
    }
  }
} //}}}
MpsTerm *MpsRcvType::CopyWrapper(std::map<std::string,void*> &children) const // {{{
{
  return new MpsRcvType(mySession, myDest, *(MpsTerm*)children["succ"], GetFinal());
} // }}}
MpsTerm *MpsRcvType::CloseDefsPre(const MpsMsgEnv &Gamma) // {{{
{
  return this;
} // }}}
MpsTerm *MpsRcvType::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ MpsTerm *newSucc=mySucc->ExtractDefinitions(env);
  MpsTerm *result=new MpsRcvType(mySession,myDest,*newSucc,GetFinal());
  delete newSucc;
  return result;
} // }}}
bool MpsRcvType::GetFinal() const // {{{
{ return myFinal;
} // }}}
