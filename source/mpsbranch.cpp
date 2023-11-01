#include<hapi/mpsbranch.hpp>
#include <hapi/common.hpp>

using namespace std;
using namespace hapi;

MpsBranch::MpsBranch(const MpsChannel &channel, const map<string,MpsTerm*> &branches, const vector<string> &finalBranches) // {{{
: myChannel(channel)
, myFinalBranches(finalBranches)
{
  myBranches.clear();
  for (map<string,MpsTerm*>::const_iterator it = branches.begin(); it != branches.end(); ++it)
  {
    // assert it->second != NULL
    MpsTerm *branch = it->second->Copy();
    myBranches[it->first] = branch;
  }
} // }}}
MpsBranch::~MpsBranch() // {{{
{
  while (myBranches.size()>0)
  {
    // assert myBranches.begin()->second != NULL
    delete myBranches.begin()->second;
    myBranches.erase(myBranches.begin());
  }
} // }}}
void *MpsBranch::TDCompileMain(tdc_pre pre, tdc_post wrap, tdc_error wrap_err, const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, const string &curPure, PureState pureState, bool checkPure) // Use rule Branch {{{
{ map<string,void*> children;
  // Check purity constraints
  if (checkPure)
	{ if (pureStack.size()>0)
      return wrap_err(this,PrintTypeError("Implementation of pure participant " + int2string(pureStack.begin()->second) + "@" + pureStack.begin()->first + " must be immediately after its decleration",*this,Theta,Gamma,Omega),children);

    if (pureState!=CPS_IMPURE && pureState!=CPS_PURE)
      return wrap_err(this,PrintTypeError("Error in implementation of pure participant " + curPure + ". Pure implementations must conform with the structure \n     *   local X()\n	   *   ( global s=new ch(p of n);\n		 *     X();\n		 *     |\n		 *     P\n		 *   )\n		 *   local StartX(Int i)\n		 *   ( if i<=0\n		 *     then X();\n		 *     else X(); | StartX(i-1);\n		 *   )\n		 *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);
  }

  // Verify branch
  MpsMsgEnv::const_iterator session=Gamma.find(myChannel.GetName());
  // Check session is open
  if (session==Gamma.end())
  {
    return wrap_err(this,PrintTypeError((string)"Typechecking error - Branching on closed session: " + myChannel.GetName(),*this,Theta,Gamma,Omega),children);
  }
  // Check if session type
  const MpsDelegateMsgType *msgType = dynamic_cast<const MpsDelegateMsgType*>(session->second);
  if (msgType==NULL)
    return wrap_err(this,PrintTypeError((string)"Branching on non-session type: " + myChannel.GetName(),*this,Theta,Gamma,Omega),children);
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
  // Check session has select type
  const MpsLocalBranchType *branchType = dynamic_cast<const MpsLocalBranchType*>(localMsgType);
  if (branchType==NULL)
  { string msgTypeStr=localMsgType->ToString("           ");
    delete localMsgType;
    return wrap_err(this,PrintTypeError((string)"Typechecking error - Selecting on session: " + myChannel.GetName() + "with type: " + msgTypeStr,*this,Theta,Gamma,Omega),children);
  }
  // Check channel index is correct
  if (myChannel.GetIndex() != branchType->GetSender())
  { delete localMsgType;
    return wrap_err(this,PrintTypeError((string)"Branching on wrong index: " + myChannel.ToString(),*this,Theta,Gamma,Omega),children);
  }
  // Check label ok
  const map<string,MpsLocalType*> &branches=branchType->GetBranches();
  for (map<string,MpsLocalType*>::const_iterator branch=branches.begin();branch!=branches.end();++branch)
  {
    // Add branch to myFinalBranches if type is Done
    if (branch->second->IsDone()) {
      myFinalBranches.push_back(branch->first);
    }
    // Create new session environment
    MpsMsgEnv newGamma = Gamma;
    newGamma[myChannel.GetName()] = new MpsDelegateLocalMsgType(*branch->second, msgType->GetPid(), msgType->GetParticipants());
    map<string,MpsTerm*>::const_iterator succ = myBranches.find(branch->first);
    if (succ==myBranches.end())
    { delete localMsgType;
      return wrap_err(this,PrintTypeError((string)"Branching cannot receive label: " + branch->first,*this,Theta,Gamma,Omega),children);
    }
    // Make new Theta
    map<string,MpsExp*>::const_iterator assertion=branchType->GetAssertions().find(branch->first);
    if (assertion==branchType->GetAssertions().end())
    { delete localMsgType;
      return wrap_err(this,PrintTypeError((string)"Branch has no assertion: " + branch->first,*this,Theta,Gamma,Omega),children);
    }
    MpsExp *newTheta = new MpsBinOpExp("and",Theta,*assertion->second,MpsBoolMsgType(),MpsBoolMsgType());
    // Typecheck Branch
    children[branch->first] = succ->second->TDCompile(pre,wrap,wrap_err,*newTheta,newGamma,Omega, pureStack, curPure, pureState, checkPure);

    // Clean up
    delete newTheta;
    delete newGamma[myChannel.GetName()];
  }
  // Clean up
  delete localMsgType;
  // Wrap result
  return wrap(this,Theta,Gamma,Omega,pureStack,curPure,pureState,checkPure,children);
} // }}}
MpsTerm *MpsBranch::ApplyBRcv(const std::string &path, const std::string &label) const // {{{
{ if (path.size()>0)
    return Error((string)"Applying BRcv on "+ToString()+" with nonempty path "+path);
  map<string,MpsTerm*>::const_iterator branch = myBranches.find(label);
  if (branch==myBranches.end())
    return Error((string)"Applying BRcv on "+ToString()+" with unknown label "+label);
  return branch->second->Copy();
} // }}}
bool MpsBranch::SubSteps(vector<MpsStep> &dest) // {{{
{
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    MpsEvent event;
    event.myType = brcv;
    event.myChannel = myChannel;
    event.myLabel = it->first;
    vector<string> paths;
    paths.push_back("");
    dest.push_back(MpsStep(event,paths));
  }
  return false;
} // }}}
MpsTerm *MpsBranch::PRename(const string &src, const string &dst) const // {{{
{
  map<string, MpsTerm*> newBranches;
  newBranches.clear();
  // PRename each branch
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    // assert it->second != NULL
    MpsTerm *newBranch = it->second->PRename(src,dst);
    newBranches[it->first] = newBranch;
  }
  MpsTerm *result = new MpsBranch(myChannel, newBranches, GetFinalBranches());
  // Clean up
  DeleteMap(newBranches);
  return result;
} // }}}
MpsTerm *MpsBranch::ERename(const string &src, const string &dst) const // {{{
{
  MpsChannel newChannel=myChannel.Rename(src,dst);
  map<string, MpsTerm*> newBranches;
  newBranches.clear();
  // ERename each branch
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    MpsTerm *newBranch = it->second->ERename(src,dst);
    newBranches[it->first] = newBranch;
  }
  MpsTerm *result = new MpsBranch(newChannel, newBranches, GetFinalBranches());
  // Clean up
  DeleteMap(newBranches);
  return result;
} // }}}
MpsTerm *MpsBranch::MRename(const string &src, const string &dst) const // {{{
{
  //MpsChannel newChannel=myChannel.MRename(src,dst);
  map<string, MpsTerm*> newBranches;
  newBranches.clear();
  // ERename each branch
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    MpsTerm *newBranch = it->second->MRename(src,dst);
    newBranches[it->first] = newBranch;
  }
  MpsTerm *result = new MpsBranch(myChannel, newBranches, GetFinalBranches());
  // Clean up
  DeleteMap(newBranches);
  return result;
} // }}}
MpsTerm *MpsBranch::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  MpsChannel newChannel=myChannel;
  if (session==myChannel.GetName())
    newChannel=MpsChannel(myChannel.GetName(),myChannel.GetIndex()+maxpid*pid);
  map<string, MpsTerm*> newBranches;
  newBranches.clear();
  // ReIndex each branch
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    MpsTerm *newBranch = it->second->ReIndex(session,pid,maxpid);
    newBranches[it->first] = newBranch;
  }
  MpsTerm *result = new MpsBranch(newChannel, newBranches, GetFinalBranches());
  // Clean up
  DeleteMap(newBranches);
  return result;
} // }}}
MpsTerm *MpsBranch::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
{
  map<string, MpsTerm*> newBranches;
  newBranches.clear();
  // PSubst each branch
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    // assert it->second != NULL
    MpsTerm *newBranch = it->second->PSubst(var,exp,args,argpids,stateargs);
    newBranches[it->first] = newBranch;
  }
  MpsTerm *result = new MpsBranch(myChannel, newBranches, GetFinalBranches());
  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second;
    newBranches.erase(newBranches.begin());
  }
  return result;
} // }}}
MpsTerm *MpsBranch::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  MpsChannel newChannel=myChannel.Subst(source,dest);
  map<string, MpsTerm*> newBranches;
  newBranches.clear();
  // ESubst each branch
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    // assert it->second != NULL
    MpsTerm *newBranch = it->second->ESubst(source,dest);
    newBranches[it->first] = newBranch;
  }
  MpsTerm *result = new MpsBranch(newChannel, newBranches, GetFinalBranches());
  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second;
    newBranches.erase(newBranches.begin());
  }
  return result;
} // }}}
MpsTerm *MpsBranch::MSubst(const string &source, const MpsMsgType &dest) const // {{{
{
  map<string, MpsTerm*> newBranches;
  newBranches.clear();
  // GSubst each branch
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
    newBranches[it->first] = it->second->MSubst(source,dest);
  MpsTerm *result = new MpsBranch(myChannel, newBranches, GetFinalBranches());

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsTerm *MpsBranch::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  map<string, MpsTerm*> newBranches;
  newBranches.clear();
  // GSubst each branch
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
    newBranches[it->first] = it->second->GSubst(source,dest,args);
  MpsTerm *result = new MpsBranch(myChannel, newBranches, GetFinalBranches());

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsTerm *MpsBranch::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  map<string, MpsTerm*> newBranches;
  newBranches.clear();
  // LSubst each branch
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
    newBranches[it->first] = it->second->LSubst(source,dest,args);
  MpsTerm *result = new MpsBranch(myChannel, newBranches, GetFinalBranches());

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
set<string> MpsBranch::FPV() const // {{{
{
  set<string> result;
  result.clear();
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    set<string> sub = it->second->FPV();
    result.insert(sub.begin(), sub.end());
  }
  return result;
} // }}}
set<string> MpsBranch::EV() const // {{{
{
  set<string> result;
  result.clear();
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    set<string> sub = it->second->EV();
    result.insert(sub.begin(), sub.end());
  }
  result.insert(myChannel.GetName());
  return result;
} // }}}
set<string> MpsBranch::FEV() const // {{{
{
  set<string> result;
  result.clear();
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    set<string> sub = it->second->FEV();
    result.insert(sub.begin(), sub.end());
  }
  result.insert(myChannel.GetName());
  return result;
} // }}}
MpsTerm *MpsBranch::Copy() const // {{{
{
  return new MpsBranch(myChannel,myBranches, GetFinalBranches());
} // }}}
bool MpsBranch::Terminated() const // {{{
{
  return false;
} // }}}
MpsTerm *MpsBranch::Simplify() const // {{{
{
  map<string,MpsTerm*> newBranches;
  newBranches.clear();
  for (map<string,MpsTerm*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
    newBranches[it->first] = it->second->Simplify();
  MpsTerm *result = new MpsBranch(myChannel,newBranches, GetFinalBranches());
  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second;
    newBranches.erase(newBranches.begin());
  }
  return result;
} // }}}
string MpsBranch::ToString(string indent) const // {{{
{
  string newIndent = indent + "  ";
  string result = myChannel.ToString() + " >>\n" + indent + "{ ";
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    if (it != myBranches.begin())
      result += ",\n" + newIndent;
    result += it->first + ":\n"
            + newIndent + it->second->ToString(newIndent);
  }
  result += "\n" + indent + "}";
  return result;
} // }}}
string MpsBranch::ToTex(int indent, int sw) const // {{{
{
  string result = myChannel.ToTex() + "$\\gg$\\newline\n"
                + ToTex_Hspace(indent,sw) + "\\{ ";
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    if (it != myBranches.begin())
      result += ",\\newline\n"
              + ToTex_Hspace(indent+2,sw);
    result += ToTex_Label(it->first);
    result += (string)":\\newline\n"
            + ToTex_Hspace(indent+2,sw) + it->second->ToTex(indent + 2,sw);
  }
  result += ToTex_Hspace(indent,sw) + "\\}";
  return result;
} // }}}
string MpsBranch::ToC(const string &taskType) const // {{{
{
  stringstream result;
  map<string,string> labels;
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
    labels[it->first]=ToC_Name(MpsExp::NewVar(taskType+it->first.substr(1)));
  
  string lblRcvName = ToC_Name(MpsExp::NewVar(string("checkpoint_")+taskType));
  result
    << ToC_Yield()
    << "    _task->SetLabel(&&" << lblRcvName << ");" << endl
    << "    if (!_this->var_" << ToC_Name(myChannel.GetName()) << "->Receive(" << myChannel.GetIndex()-1 << ",_task,&_task->tmp)) // Receive label to tmp" << endl
    << "      return false;" << endl
    << "    " << lblRcvName << ":" << endl;
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    if (it != myBranches.begin())
      result << "    else " << endl;
    // Fixme: Use goto *branchmap[((libpi::String*)_task->tmp)->GetValue()];
    result << "    if (((libpi::String*)_task->tmp)->GetValue()==\"" << it->first << "\")" << endl
           << "    {" << endl
           << "      _task->tmp->RemoveRef();" << endl
           << "      _task->tmp=NULL;" << endl;
    if (find(myFinalBranches.begin(),myFinalBranches.end(),it->first)!=myFinalBranches.end()) {
      result << "      _this->var_" << ToC_Name(myChannel.GetName()) << "->Close(true);" << endl
             << "      _this->var_" << ToC_Name(myChannel.GetName()) << "->RemoveRef();" << endl;
    }
    result << "      _this->SetLabel(&&" << labels[it->first] << ");" << endl
           << "    }" << endl;
  }
  result << "    else throw string(\"Unknown branch: \")+((libpi::String*)_task->tmp)->GetValue();" << endl
         << "    goto *_this->GetLabel();" << endl;
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  { result << labels[it->first] << ":" << endl;
    result << it->second->ToC(taskType);
  }
  return result.str();
} // }}}
string MpsBranch::ToCHeader() const // {{{
{
  stringstream result;
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    result << it->second->ToCHeader();
  }
  return result.str();
} // }}}
void MpsBranch::ToCConsts(std::vector<std::string> &dest, std::unordered_set<std::string> &existing) const // {{{
{
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
    it->second->ToCConsts(dest,existing);
} // }}}
MpsTerm *MpsBranch::FlattenFork(bool normLhs, bool normRhs, bool pureMode) const // {{{
{
  map<string,MpsTerm*> newBranches;
  for (map<string,MpsTerm*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
    newBranches[it->first]=it->second->FlattenFork(normLhs,normRhs,pureMode);

  MpsTerm *result=new MpsBranch(myChannel,newBranches, GetFinalBranches());
  DeleteMap(newBranches);

  return result;
} // }}}
MpsTerm *MpsBranch::RenameAll() const // {{{
{ map<string,MpsTerm*> newBranches;
  for (map<string,MpsTerm*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
    newBranches[it->first]=it->second->RenameAll();
  MpsTerm *result=new MpsBranch(myChannel,newBranches, GetFinalBranches());
  DeleteMap(newBranches);
  return result;
} // }}}
bool MpsBranch::Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const // {{{
{ map<string,MpsTerm*> newBranches;
  for (map<string,MpsTerm*>::const_iterator branch=myBranches.begin(); branch!=myBranches.end(); ++branch)
    newBranches[branch->first]=branch->second->Parallelize();
  seqTerm = new MpsBranch(myChannel,newBranches, GetFinalBranches());
  parTerm = receives.Append(*seqTerm);
  DeleteMap(newBranches);
  return false; // All optimizations are guarded
} // }}}
MpsTerm *MpsBranch::Append(const MpsTerm &term) const // {{{
{ map<string,MpsTerm*> newBranches;
  for (map<string,MpsTerm*>::const_iterator branch=myBranches.begin(); branch!=myBranches.end(); ++branch)
    newBranches[branch->first]=branch->second->Append(term);
  MpsTerm *result = new MpsBranch(myChannel,newBranches, GetFinalBranches());
  DeleteMap(newBranches);
  return result;
} // }}}
MpsTerm *MpsBranch::CopyWrapper(std::map<std::string,void*> &children) const // {{{
{
  map<string,MpsTerm*> branches;
  for (map<string,void*>::iterator it=children.begin(); it!=children.end(); ++it)
    branches[it->first]=(MpsTerm*)it->second;

  return new MpsBranch(myChannel, branches, GetFinalBranches());
} // }}}
MpsTerm *MpsBranch::CloseDefsPre(const MpsMsgEnv &Gamma) // {{{
{ return this;
} // }}}
MpsTerm *MpsBranch::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ map<string,MpsTerm*> newBranches;
  for (map<string,MpsTerm*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
    newBranches[it->first]=it->second->ExtractDefinitions(env);
  MpsTerm *result=new MpsBranch(myChannel,newBranches, GetFinalBranches());
  DeleteMap(newBranches);
  return result;
} // }}}
const vector<string> &MpsBranch::GetFinalBranches() const // {{{
{ return myFinalBranches;
} // }}}
