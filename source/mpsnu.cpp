#include<hapi/mpsnu.hpp>
#include <hapi/common.hpp>

using namespace std;
using namespace hapi;

MpsNu::MpsNu(const vector<MpsParticipant> &participants, const string &channel, const MpsTerm &succ, const MpsGlobalType &type) // {{{
: myParticipants(participants)
, myChannel(channel)
{
  mySucc = succ.Copy();
  myType = type.Copy();
} // }}}
MpsNu::~MpsNu() // {{{
{
  delete mySucc;
  delete myType;
} // }}}
void *MpsNu::TDCompileMain(tdc_pre pre, tdc_post wrap, tdc_error wrap_err, const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, const string &curPure, PureState pureState, bool checkPure) // Use rule Nres {{{
{ map<string,void*> children;
  // Check purity constraints
  if (checkPure)
	{ if (pureState!=CPS_IMPURE && pureState!=CPS_PURE)
      return wrap_err(this,PrintTypeError("Error in implementation of pure participant " + curPure + ". Pure implementations must conform with the structure \n     *   local X()\n	   *   ( global s=new ch(p of n);\n		 *     X();\n		 *     |\n		 *     P\n		 *   )\n		 *   local StartX(Int i)\n		 *   ( if i<=0\n		 *     then X();\n		 *     else X(); | StartX(i-1);\n		 *   )\n		 *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);
  }

  // Verify new channel
  // Check that only completed sessions are hidden
  MpsMsgEnv newGamma = Gamma;
  MpsMsgEnv::iterator var=newGamma.find(myChannel);
  if (var!=newGamma.end())
  { const MpsDelegateMsgType *session=dynamic_cast<const MpsDelegateMsgType*>(var->second);
    if (session!=NULL &&
        !session->GetLocalType()->Equal(Theta,MpsLocalEndType()))
      return wrap_err(this,PrintTypeError((string)"Hiding uncompleted session:" + myChannel,*this,Theta,Gamma,Omega),children);

    // Remove hidden variable
    newGamma.erase(var);
  }
  // Add channel to type env
  newGamma[myChannel] = new MpsChannelMsgType(*myType,myParticipants);
  // Find and add pure participants to pureStack
  set<pair<string,int> > newPureStack=pureStack;
  for (vector<MpsParticipant>::const_iterator p=myParticipants.begin(); p!=myParticipants.end(); ++p)
    if (p->IsPure())
      newPureStack.insert(pair<string,int>(myChannel,p->GetId()));
  children["succ"] = mySucc->TDCompile(pre,wrap,wrap_err,Theta,newGamma,Omega,newPureStack,curPure,pureState,checkPure);
  delete newGamma[myChannel];
  // Wrap result
  return wrap(this,Theta,Gamma,Omega,pureStack,curPure,pureState,checkPure,children);
} // }}}
MpsTerm *MpsNu::ApplyOther(const std::string &path) const // {{{
{ if (path.size()!=0)
    return Error((string)"Applying Other with nonempty path on "+ToString());
  string name = NewName(myChannel);
  return mySucc->ERename(myChannel,name);
} // }}}
bool MpsNu::SubSteps(vector<MpsStep> &dest) // {{{
{
  MpsEvent event;
  event.myType=tau;
  event.mySubType=nu;
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event,paths));
  return false;
} // }}}
MpsTerm *MpsNu::PRename(const string &src, const string &dst) const // {{{
{
  MpsTerm *newSucc = mySucc->PRename(src,dst);
  MpsTerm *result = new MpsNu(myParticipants,myChannel,*newSucc,*myType);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsNu::ERename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  if (myChannel==src) // No substitution necessary
    return Copy();

  MpsTerm *newSucc = NULL;
  string newChannel=myChannel;
  if (myChannel==dst) // Must rename myChannel
  {
    newChannel = MpsExp::NewVar();
    MpsTerm *tmpSucc = mySucc->ERename(myChannel,newChannel);
    newSucc = tmpSucc->ERename(src,dst);
    delete tmpSucc;
  }
  else
    newSucc = mySucc->ERename(src,dst);

  MpsGlobalType *newType=myType->ERename(src,dst);

  MpsTerm *result = new MpsNu(myParticipants, newChannel, *newSucc, *newType);
  delete newSucc;
  delete newType;
  return result;
} // }}}
MpsTerm *MpsNu::MRename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  //if (myChannel==src) // No substitution necessary
  //  return Copy();

  MpsTerm *newSucc = NULL;
  string newChannel=myChannel;
  //if (myChannel==dst) // Must rename myChannel
  //{
  //  newChannel = MpsExp::NewVar();
  //  MpsTerm *tmpSucc = mySucc->ERename(myChannel,newChannel);
  //  newSucc = tmpSucc->ERename(src,dst);
  //  delete tmpSucc;
  //}
  //else
    newSucc = mySucc->MRename(src,dst);

  MpsGlobalType *newType=myType->MRename(src,dst);

  MpsTerm *result = new MpsNu(myParticipants, newChannel, *newSucc, *newType);
  delete newSucc;
  delete newType;
  return result;
} // }}}
MpsTerm *MpsNu::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  // assert mySucc != NULL
  if (myChannel==session) // No substitution necessary
    return Copy();

  MpsTerm *newSucc = mySucc->ReIndex(session,pid,maxpid);
  MpsTerm *result = new MpsNu(myParticipants, myChannel, *newSucc, *myType);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsNu::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
{
  // assert mySucc != NULL
  string newChannel = myChannel;
  
  set<string> fev=exp.FEV();
  MpsTerm *newSucc = NULL;
  if (fev.find(myChannel)!= fev.end()) // Rename channel before subst
  {
    newChannel = MpsExp::NewVar();
    MpsExp *newVar = new MpsVarExp(newChannel,MpsMsgNoType());
    MpsTerm *tmpSucc = mySucc->ESubst(myChannel,*newVar);
    newSucc = tmpSucc->PSubst(var,exp,args,argpids,stateargs);
    delete tmpSucc;
    delete newVar;
  }
  else
    newSucc = mySucc->PSubst(var,exp,args,argpids,stateargs);
  MpsTerm *result = new MpsNu(myParticipants, newChannel, *newSucc, *myType);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsNu::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  MpsGlobalType *newType=myType->ESubst(source,dest);
  // assert mySucc != NULL
  if (myChannel==source) // No substitution necessary
  { MpsTerm *result=new MpsNu(myParticipants, myChannel, *mySucc, *newType);
    delete newType;
    return result;
  }

  MpsTerm *newSucc = NULL;
  set<string> fv = dest.FV();
  string newChannel = myChannel;
  if (fv.find(myChannel) != fv.end()) // Must rename myChannel
  {
    newChannel = MpsExp::NewVar();
    MpsExp *newVar = new MpsVarExp(newChannel,MpsMsgNoType());
    MpsTerm *tmpSucc = mySucc->ESubst(myChannel,*newVar);
    newSucc = tmpSucc->ESubst(source,dest);
    delete tmpSucc;
    delete newVar;
  }
  else
    newSucc = mySucc->ESubst(source,dest);
  MpsTerm *result = new MpsNu(myParticipants, newChannel, *newSucc, *newType);
  delete newType;
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsNu::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsGlobalType *newType = myType->GSubst(source,dest,args);
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  MpsTerm *result =  new MpsNu(myParticipants, myChannel, *newSucc, *newType);

  // Clean Up
  delete newType;
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsNu::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsGlobalType *newType = myType->LSubst(source,dest,args);
  MpsTerm *newSucc = mySucc->LSubst(source,dest,args);
  MpsTerm *result =  new MpsNu(myParticipants, myChannel, *newSucc, *newType);

  // Clean Up
  delete newType;
  delete newSucc;

  return result;
} // }}}
set<string> MpsNu::FPV() const // {{{
{
  set<string> result = mySucc->FPV();
  return result;
} // }}}
set<string> MpsNu::EV() const // {{{
{
  set<string> result = mySucc->EV();
  result.insert(myChannel);
  return result;
} // }}}
set<string> MpsNu::FEV() const // {{{
{
  set<string> result = mySucc->FEV();
  result.erase(myChannel);
  return result;
} // }}}
MpsTerm *MpsNu::Copy() const // {{{
{
  return new MpsNu(myParticipants, myChannel, *mySucc, *myType);
} // }}}
bool MpsNu::Terminated() const // {{{
{
  return mySucc->Terminated();
} // }}}
MpsTerm *MpsNu::Simplify() const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->Simplify();
  MpsTerm *result = new MpsNu(myParticipants, myChannel, *newSucc, *myType);
  delete newSucc;
  return result;
} // }}}
string MpsNu::ToString(string indent) const // {{{
{
  string result = myType->ToString(indent) + " " + myChannel + "(";
  for (int p=0; p<myParticipants.size(); ++p)
  { if (p!=0) // Add separator
      result += ", ";
    result += int2string(myParticipants[p].GetId());
    if (myParticipants[p].IsPure())
      result += " pure";
  }
  result += ");\n" + indent + mySucc->ToString(indent);
  return result;
} // }}}
string MpsNu::ToTex(int indent, int sw) const // {{{
{
  int typeIndent = indent + 5 + myChannel.size();
  return "(" + ToTex_KW("nu") + " " + ToTex_Session(myChannel) + ":" + myType->ToTex(typeIndent,sw) + ")\\newline\n"
       + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
} // }}}
string MpsNu::ToC(const string &taskType) const // {{{
{
  stringstream result;
  result << ToC_Yield()
         << "    _this->var_" << ToC_Name(myChannel) << ".reset(new libpi::task::Link(" << myType->GetMaxPid() << "));" << endl;
  result << mySucc->ToC(taskType);
  return result.str();
} // }}}
string MpsNu::ToCHeader() const // {{{
{
  return mySucc->ToCHeader();
} // }}}
void MpsNu::ToCConsts(vector<string> &dest, unordered_set<string> &existing) const // {{{
{ mySucc->ToCConsts(dest,existing);
} // }}}
MpsTerm *MpsNu::FlattenFork(bool normLhs, bool normRhs, bool pureMode) const // {{{
{
  MpsTerm *newSucc = mySucc->FlattenFork(normLhs,normRhs,pureMode);
  MpsTerm *result= new MpsNu(myParticipants, myChannel, *newSucc, *myType);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsNu::RenameAll() const // {{{
{ string newChannel = MpsExp::NewVar(myChannel);
  MpsTerm *tmpSucc=mySucc->ERename(myChannel,newChannel);
  MpsTerm *newSucc=tmpSucc->RenameAll();
  delete tmpSucc;
  MpsGlobalType *newType=myType->RenameAll();

  MpsTerm *result=new MpsNu(myParticipants, newChannel,*newSucc,*newType);

  delete newSucc;
  delete newType;

  return result;
} // }}}
bool MpsNu::Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const // {{{
{ MpsTerm *seqSucc = mySucc->Parallelize();
  seqTerm=new MpsNu(myParticipants, myChannel, *seqSucc, *myType);
  delete seqSucc;
  parTerm=receives.Append(*seqTerm);
  return false; // All optimizations are guarded
} // }}}
MpsTerm *MpsNu::Append(const MpsTerm &term) const // {{{
{ MpsTerm *newSucc=mySucc->Append(term);
  MpsTerm *result=new MpsNu(myParticipants, myChannel, *newSucc, *myType);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsNu::CopyWrapper(std::map<std::string,void*> &children) const // {{{
{
  return new MpsNu(myParticipants, myChannel, *(MpsTerm*)children["succ"], *myType);
} // }}}
MpsTerm *MpsNu::CloseDefsPre(const MpsMsgEnv &Gamma) // {{{
{ return this;
} // }}}
MpsTerm *MpsNu::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ MpsTerm *newSucc=mySucc->ExtractDefinitions(env);

  MpsTerm *result=new MpsNu(myParticipants, myChannel,*newSucc,*myType);

  delete newSucc;

  return result;
} // }}}
