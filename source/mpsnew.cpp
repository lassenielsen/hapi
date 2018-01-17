#include<hapi/mpsnew.hpp>
#include <hapi/common.hpp>
#include <hapi/mpsgui.hpp>

using namespace std;
using namespace hapi;

MpsNew::MpsNew(const std::vector<std::string> &names, const MpsGlobalType &type, const MpsTerm &succ) // {{{
: myNames(names)
{
  mySucc = succ.Copy();
  myType = type.Copy();
} // }}}
MpsNew::~MpsNew() // {{{
{
  delete mySucc;
  delete myType;
} // }}}
void *MpsNew::TDCompileMain(tdc_pre pre, tdc_post wrap, tdc_error wrap_err, const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, const string &curPure, PureState pureState, bool checkPure) // Use rule Nres {{{
{ map<string,void*> children;
  // Check purity constraints
  if (checkPure)
  { if (pureStack.size()>0)
      return wrap_err(this,PrintTypeError("Implementation of pure participant " + int2string(pureStack.begin()->second) + "@" + pureStack.begin()->first + " must be immediately after its decleration",*this,Theta,Gamma,Omega),children);
     if (pureState!=CPS_IMPURE && pureState!=CPS_PURE)
      return wrap_err(this,PrintTypeError("Error in implementation of pure participant " + curPure + ". Pure implementations must conform with the structure \n     *   local X()\n	   *   ( global s=new ch(p of n);\n		 *     X();\n		 *     |\n		 *     P\n		 *   )\n		 *   local StartX(Int i)\n		 *   ( if i<=0\n		 *     then X();\n		 *     else X(); | StartX(i-1);\n		 *   )\n		 *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);
  }

  // Check correct number of participants
  if (myNames.size()!=myType->GetMaxPid())
    return wrap_err(this,PrintTypeError((string)"Number of participants mismatch",*this,Theta,Gamma,Omega),children);
  // Check that only completed sessions are hidden
  MpsMsgEnv newGamma = Gamma;
  for (int i=0; i<myNames.size(); ++i)
  { MpsMsgEnv::iterator var=newGamma.find(myNames[i]);
    if (var!=newGamma.end())
    { const MpsDelegateMsgType *session=dynamic_cast<const MpsDelegateMsgType*>(var->second);
      if (session!=NULL &&
          !session->GetLocalType()->Equal(Theta,MpsLocalEndType()))
        return wrap_err(this,PrintTypeError((string)"Hiding uncompleted session:" + myNames[i],*this,Theta,Gamma,Omega),children);

      // Remove hidden variable
      newGamma.erase(var);
    }
  }
  // Add sessions to type env
  for (int i=0; i<myNames.size(); ++i)
  { // Create local type
    MpsLocalType *newType=myType->Project(i+1);
    set<string> fv = newType->FEV();
    // Rename all free variables
    for (set<string>::const_iterator it=fv.begin(); it!=fv.end(); ++it)
    { MpsLocalType *tmpType=newType->ERename(*it,MpsExp::NewVar(*it));
      delete newType;
      newType=tmpType;
    }
    // Create Gamma with new session
    vector<MpsParticipant> participants;
    for (int i=0; i<myNames.size(); ++i)
      participants.push_back(MpsParticipant(i+1,to_string(i+1),false));
    newGamma[myNames[i]] = new MpsDelegateLocalMsgType(*newType,i+1,participants);
    delete newType;
  }
  children["succ"] = mySucc->TDCompile(pre,wrap,wrap_err,Theta,newGamma,Omega,pureStack,curPure,pureState,checkPure);
  // Cleanup
  for (int i=0; i<myNames.size(); ++i)
  { delete newGamma[myNames[i]];
  }
  // Wrap result
  return wrap(this,Theta,Gamma,Omega,pureStack,curPure,pureState,checkPure,children);
} // }}}
MpsTerm *MpsNew::ApplyOther(const std::string &path) const // {{{
{ if (path.size()!=0)
    return Error((string)"Applying Other with nonempty path on "+ToString());

  string session = MpsExp::NewVar(myNames[0]);
  mpsgui::CreateSession(session,myNames.size()); // Inform GUI

  MpsTerm *result=mySucc->Copy();
  // Reindexing and renaming
  for (int i=0; i<myNames.size(); ++i)
  { MpsTerm *tmpResult = result->ReIndex(myNames[i],i+1,myNames.size());
    delete result;
    result=tmpResult->ERename(myNames[i],session);
    delete tmpResult;
  }
  return result;
} // }}}
bool MpsNew::SubSteps(vector<MpsStep> &dest) // {{{
{
  MpsEvent event;
  event.myType=tau;
  event.mySubType=create;
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event,paths));
  return false;
} // }}}
MpsTerm *MpsNew::PRename(const string &src, const string &dst) const // {{{
{
  MpsTerm *newSucc = mySucc->PRename(src,dst);
  MpsTerm *result = new MpsNew(myNames,*myType,*newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsNew::ERename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  for (int i=0; i<myNames.size(); ++i)
    if (myNames[i]==src) // No substitution necessary
      return Copy();

  MpsTerm *newSucc = mySucc->Copy();
  vector<string> newNames=myNames;
  for (int i=0; i<newNames.size(); ++i)
    if (myNames[i]==dst) // Must rename myChannel
    {
      newNames[i] = MpsExp::NewVar(myNames[i]);
      MpsTerm *tmpSucc = newSucc->ERename(myNames[i],newNames[i]);
      delete newSucc;
      newSucc=tmpSucc;
    }
  MpsTerm *tmpSucc = newSucc->ERename(src,dst);
  delete newSucc;
  newSucc=tmpSucc;
  MpsTerm *result = new MpsNew(newNames, *myType, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsNew::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  for (int i=0; i<myNames.size(); ++i)
    if (myNames[i]==session) // No substitution necessary
      return Copy();

  MpsTerm *newSucc = mySucc->ReIndex(session,pid,maxpid);
  MpsTerm *result = new MpsNew(myNames, *myType, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsNew::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
{
  vector<string> newNames=myNames;
  
  set<string> fev=exp.FEV();
  MpsTerm *newSucc = mySucc->Copy();
  for (int i=0; i<myNames.size(); ++i)
  { if (fev.find(myNames[i])!= fev.end()) // Rename channel before subst
    { newNames[i] = MpsExp::NewVar();
      MpsExp *newVar = new MpsVarExp(myNames[i],MpsMsgNoType());
      MpsTerm *tmpSucc = newSucc->ESubst(myNames[i],*newVar);
      delete newSucc;
      newSucc=tmpSucc;
      delete newVar;
    }
  }
  MpsTerm *tmpSucc= newSucc->PSubst(var,exp,args,argpids,stateargs);
  delete newSucc;
  newSucc=tmpSucc;

  MpsTerm *result = new MpsNew(newNames, *myType, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsNew::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  MpsGlobalType *newType=myType->ESubst(source,dest);

  // Test if subst is blocked
  for (int i=0; i<myNames.size(); ++i)
  { if (source==myNames[i])
    { MpsTerm *result = new MpsNew(myNames, *newType, *mySucc);
      delete newType;
      return result;
    }
  }
  // Perform renaming if necessary
  vector<string> newNames=myNames;
  set<string> fev=dest.FV();
  MpsTerm *newSucc = mySucc->Copy();
  for (int i=0; i<myNames.size(); ++i)
  { if (fev.find(myNames[i])!= fev.end()) // Rename channel before subst
    { newNames[i] = MpsExp::NewVar();
      MpsExp *newVar = new MpsVarExp(myNames[i],MpsMsgNoType());
      MpsTerm *tmpSucc = newSucc->ESubst(myNames[i],*newVar);
      delete newSucc;
      newSucc=tmpSucc;
      delete newVar;
    }
  }

  MpsTerm *tmpSucc = newSucc->ESubst(source,dest);
  delete newSucc;
  newSucc=tmpSucc;

  MpsTerm *result = new MpsNew(newNames, *newType, *newSucc);
  delete newType;
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsNew::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsGlobalType *newType = myType->GSubst(source,dest,args);
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  MpsTerm *result =  new MpsNew(myNames, *newType, *newSucc);

  // Clean Up
  delete newType;
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsNew::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsGlobalType *newType = myType->LSubst(source,dest,args);
  MpsTerm *newSucc = mySucc->LSubst(source,dest,args);
  MpsTerm *result =  new MpsNew(myNames, *newType, *newSucc);

  // Clean Up
  delete newType;
  delete newSucc;

  return result;
} // }}}
set<string> MpsNew::FPV() const // {{{
{
  set<string> result = mySucc->FPV();
  return result;
} // }}}
set<string> MpsNew::EV() const // {{{
{
  set<string> result = mySucc->EV();
  set<string> result2 = myType->FEV();
  result.insert(result2.begin(),result2.end());
  for (vector<string>::const_iterator name=myNames.begin(); name!=myNames.end(); ++name)
    result.insert(*name);
  return result;
} // }}}
set<string> MpsNew::FEV() const // {{{
{
  set<string> result = mySucc->FEV();
  set<string> result2 = myType->FEV();
  result.insert(result2.begin(),result2.end());
  for (vector<string>::const_iterator name=myNames.begin(); name!=myNames.end(); ++name)
    result.erase(*name);
  return result;
} // }}}
MpsTerm *MpsNew::Copy() const // {{{
{
  return new MpsNew(myNames, *myType, *mySucc);
} // }}}
bool MpsNew::Terminated() const // {{{
{
  return false;
} // }}}
MpsTerm *MpsNew::Simplify() const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->Simplify();
  MpsTerm *result = new MpsNew(myNames, *myType, *newSucc);
  delete newSucc;
  return result;
} // }}}
string MpsNew::ToString(string indent) const // {{{
{
  stringstream result;
  for (int i=0; i<myNames.size(); ++i)
  { if (i!=0)
      result << ", ";
    result << myNames[i];
  }
  result << " = new " << myType->ToString(indent);
  result << ";\n" + indent + mySucc->ToString(indent);
  return result.str();
} // }}}
string MpsNew::ToTex(int indent, int sw) const // {{{
{
  int typeIndent = indent + 5;
  stringstream result;
  for (int i=0; i<myNames.size(); ++i)
  { if (i!=0)
      result << ", ";
    result << ToTex_Session(myNames[i]);
  }
  result << " = " << ToTex_KW("new") << " " << myType->ToTex(typeIndent,sw);
  result << ";\\newline\n" + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
  return result.str();
} // }}}
string MpsNew::ToC(const string &taskType) const // {{{
{
  stringstream result;
  result << ToC_Yield()
         << "    {" << endl;
  result << "      _task->tmps.clear();" << endl;
  // Create all channels
  for (int i=0; i<myNames.size(); ++i)
    for (int j=0; j<myNames.size(); ++j)
      result << "      _task->tmps.push_back(std::shared_ptr<libpi::Value>(new libpi::task::Channel()));" << endl;
  // For each participant, create filtered channels vector, and create session
  for (int i=0; i<myNames.size(); ++i)
  { string sesInChannels = ToC_Name(MpsExp::NewVar(myNames[i]+"_in"));
    string sesOutChannels = ToC_Name(MpsExp::NewVar(myNames[i]+"_out"));
    result << "    vector<shared_ptr<libpi::Channel >> " << sesInChannels << ";" << endl;
    for (int j=0; j<myNames.size(); ++j)
      result << "    " << sesInChannels << ".push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[" << (j+i*myNames.size()) << "]));" << endl;
    result << "    vector<shared_ptr<libpi::Channel> > " << sesOutChannels << ";" << endl;
    for (int j=0; j<myNames.size(); ++j)
      result << "    " << sesOutChannels << ".push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[" << (i+j*myNames.size()) << "]));" << endl;
  
    result << "      _this->tmps.clear();" << endl
           << "      _this->var_" << ToC_Name(myNames[i]) << ".reset(new libpi::Session(" << (i+1) << ", " << myNames.size() << ", " << sesInChannels << "," << sesOutChannels << "));" << endl;
  }
  result << "    }" << endl;

  result << mySucc->ToC(taskType);
  return result.str();
} // }}}
string MpsNew::ToCHeader() const // {{{
{
  return mySucc->ToCHeader();
} // }}}
void MpsNew::ToCConsts(vector<string> &dest, unordered_set<string> &existing) const // {{{
{ mySucc->ToCConsts(dest,existing);
} // }}}
MpsTerm *MpsNew::FlattenFork(bool normLhs, bool normRhs, bool pureMode) const // {{{
{
  MpsTerm *newSucc = mySucc->FlattenFork(normLhs,normRhs,pureMode);
  MpsTerm *result= new MpsNew(myNames, *myType, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsNew::RenameAll() const // {{{
{ vector<string> newNames;
  for (int i=0; i<myNames.size(); ++i)
    newNames.push_back(MpsExp::NewVar(myNames[i]));
  MpsGlobalType *newType=myType->RenameAll();
  MpsTerm *newSucc=mySucc->Copy();
  for (int i=0; i<myNames.size(); ++i)
  { MpsTerm *tmpSucc=newSucc->ERename(myNames[i],newNames[i]);
    delete newSucc;
    newSucc=tmpSucc;
  }

  MpsTerm *result=new MpsNew(newNames,*newType,*newSucc);

  delete newSucc;
  delete newType;

  return result;
} // }}}
bool MpsNew::Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const // {{{
{ MpsTerm *seqSucc = mySucc->Parallelize();
  seqTerm=new MpsNew(myNames, *myType, *seqSucc);
  delete seqSucc;
  parTerm=receives.Append(*seqTerm);
  return false; // All optimizations are guarded
} // }}}
MpsTerm *MpsNew::Append(const MpsTerm &term) const // {{{
{ MpsTerm *newSucc=mySucc->Append(term);
  MpsTerm *result=new MpsNew(myNames, *myType, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsNew::CopyWrapper(std::map<std::string,void*> &children) const // {{{
{ return new MpsNew(myNames, *myType, *(MpsTerm*)children["succ"]);
} // }}}
MpsTerm *MpsNew::CloseDefsPre(const MpsMsgEnv &Gamma) // {{{
{ return this;
} // }}}
MpsTerm *MpsNew::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ MpsTerm *newSucc=mySucc->ExtractDefinitions(env);
  MpsTerm *result=new MpsNew(myNames, *myType, *newSucc);
  delete newSucc;
  return result;
} // }}}

