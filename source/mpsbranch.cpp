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
void *MpsBranch::TDCompile(tdc_wrapper wrap, tdc_wraperr wrap_err, const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, const string &curPure, PureState pureState, bool checkPure) // Use rule Branch {{{
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
  const MpsLocalRecType *recType = dynamic_cast<const MpsLocalRecType*>(msgType->GetLocalType());
  // Check if unfolding is necessary
  if (recType!=NULL)
    return TypeCheckRec(wrap, wrap_err, Theta, Gamma, Omega, pureStack, curPure, pureState, checkPure, *this, session->first);
  const MpsLocalForallType *allType = dynamic_cast<const MpsLocalForallType*>(msgType->GetLocalType());
  if (allType!=NULL)
    return TypeCheckForall(wrap, wrap_err, Theta, Gamma, Omega, pureStack, curPure, pureState, checkPure, *this, session->first);
  // Check session has select type
  const MpsLocalBranchType *branchType = dynamic_cast<const MpsLocalBranchType*>(msgType->GetLocalType());
  if (branchType==NULL)
    return wrap_err(this,PrintTypeError((string)"Typechecking error - Selecting on session: " + myChannel.GetName() + "with type: " + msgType->GetLocalType()->ToString("           "),*this,Theta,Gamma,Omega),children);
  // Check channel index is correct
  if (myChannel.GetIndex() != branchType->GetSender())
    return wrap_err(this,PrintTypeError((string)"Branching on wrong index: " + myChannel.ToString(),*this,Theta,Gamma,Omega),children);
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
      return wrap_err(this,PrintTypeError((string)"Branching cannot receive label: " + branch->first,*this,Theta,Gamma,Omega),children);
    // Make new Theta
    map<string,MpsExp*>::const_iterator assertion=branchType->GetAssertions().find(branch->first);
    if (assertion==branchType->GetAssertions().end())
      return wrap_err(this,PrintTypeError((string)"Branch has no assertion: " + branch->first,*this,Theta,Gamma,Omega),children);
    MpsExp *newTheta = new MpsBinOpExp("and",Theta,*assertion->second,MpsBoolMsgType(),MpsBoolMsgType());
    // Typecheck Branch
    children[branch->first] = succ->second->TDCompile(wrap,wrap_err,*newTheta,newGamma,Omega, pureStack, curPure, pureState, checkPure);

    // Clean up
    delete newTheta;
    delete newGamma[myChannel.GetName()];
  }
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
string MpsBranch::ToC() const // {{{
{
  stringstream result;
  string lblName = ToC_Name(MpsExp::NewVar("branch")); // Create variable name for the received value
  result << "  { _dec_aprocs();" << endl
         << "    shared_ptr<libpi::String> " << lblName << " = static_pointer_cast<libpi::String>(" << ToC_Name(myChannel.GetName()) << "->Receive(" << int2string(myChannel.GetIndex()-1) << "));" << endl
         << "    _inc_aprocs();" << endl;
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    if (it != myBranches.begin())
      result << "    else " << endl;
    result << "    if (" << lblName << "->GetValue()==\"" << it->first << "\")" << endl
           << "    {" << endl;
    if (find(myFinalBranches.begin(),myFinalBranches.end(),it->first)!=myFinalBranches.end()) {
      result << "      " << ToC_Name(myChannel.GetName()) << "->Close(true);" << endl
             << "      " << ToC_Name(myChannel.GetName()) << " = NULL;" << endl;
    }
    result << it->second->ToC();
    result << "    }" << endl;
  }
  result << "    else throw (string)\"Unknown branch: " << lblName << "\";" << endl;
  result << "  }" << endl;
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
MpsTerm *MpsBranch::CloseDefsWrapper(const MpsExp &Theta, // {{{
                                  const MpsMsgEnv &Gamma,
                                  const MpsProcEnv &Omega, 
                                  const std::set<std::pair<std::string,int> > &pureStack,
                                  const std::string &curPure,
                                  MpsTerm::PureState pureState,
                                  bool checkPure,
                                  std::map<std::string,void*> &children)
{ map<string,MpsTerm*> newBranches;
  for (map<string,MpsTerm*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
    newBranches[it->first]=(MpsTerm*)children[it->first];

  return new MpsBranch(myChannel, newBranches, GetFinalBranches());
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
