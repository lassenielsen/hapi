#include<apims/mpsbranch.hpp>
#include "common.cpp"

using namespace std;
using namespace apims;

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
bool MpsBranch::TypeCheck(const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega) // Use rule Branch {{{
{
  MpsMsgEnv::const_iterator session=Gamma.find(myChannel.GetName());
  // Check session is open
  if (session==Gamma.end())
  {
    return PrintTypeError((string)"Typechecking error - Branching on closed session: " + myChannel.GetName(),*this,Theta,Gamma,Omega);
  }
  // Check if session type
  const MpsDelegateMsgType *msgType = dynamic_cast<const MpsDelegateMsgType*>(session->second);
  if (msgType==NULL)
    return PrintTypeError((string)"Branching on non-session type: " + myChannel.GetName(),*this,Theta,Gamma,Omega);
  const MpsLocalRecType *recType = dynamic_cast<const MpsLocalRecType*>(msgType->GetLocalType());
  // Check if unfolding is necessary
  if (recType!=NULL)
    return TypeCheckRec(Theta,Gamma, Omega, *this, session->first);
  const MpsLocalForallType *allType = dynamic_cast<const MpsLocalForallType*>(msgType->GetLocalType());
  if (allType!=NULL)
    return TypeCheckForall(Theta, Gamma, Omega, *this, session->first);
  // Check session has select type
  const MpsLocalBranchType *branchType = dynamic_cast<const MpsLocalBranchType*>(msgType->GetLocalType());
  if (branchType==NULL)
    return PrintTypeError((string)"Typechecking error - Selecting on session: " + myChannel.GetName() + "with type: " + msgType->GetLocalType()->ToString("           "),*this,Theta,Gamma,Omega);
  // Check channel index is correct
  if (myChannel.GetIndex() != branchType->GetSender())
    return PrintTypeError((string)"Branching on wrong index: " + myChannel.ToString(),*this,Theta,Gamma,Omega);
  // Check label ok
  const map<string,MpsLocalType*> &branches=branchType->GetBranches();
  for (map<string,MpsLocalType*>::const_iterator branch=branches.begin();branch!=branches.end();++branch)
  {
    // Add branch to myFinalBranches if type is Done
    if (branch->second->IsDone())
      myFinalBranches.push_back(branch->first);
    // Create new session environment
    MpsMsgEnv newGamma = Gamma;
    newGamma[myChannel.GetName()] = new MpsDelegateLocalMsgType(*branch->second, msgType->GetPid(), msgType->GetMaxpid());
    map<string,MpsTerm*>::const_iterator succ = myBranches.find(branch->first);
    if (succ==myBranches.end())
      return PrintTypeError((string)"Branching cannot receive label: " + branch->first,*this,Theta,Gamma,Omega);
    // Make new Theta
    map<string,MpsExp*>::const_iterator assertion=branchType->GetAssertions().find(branch->first);
    if (assertion==branchType->GetAssertions().end())
      return PrintTypeError((string)"Branch has no assertion: " + branch->first,*this,Theta,Gamma,Omega);
    MpsExp *newTheta = new MpsBinOpExp("and",Theta,*assertion->second,MpsBoolMsgType(),MpsBoolMsgType());
    // Typecheck Branch
    bool brcheck=succ->second->TypeCheck(*newTheta,newGamma,Omega);

    // Clean up
    delete newTheta;
    delete newGamma[myChannel.GetName()];

    if (not brcheck)
      return false;
  }
  // All checks passed
  return true;
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
  string result = ToTex_Channel(myChannel) + "$\\gg$\\newline\n"
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
  string msgName = ToC_Name(MpsExp::NewVar("branch")); // Create variable name for the received message
  result << "  StringValue " << lblName << ";" << endl
         << "  {" << endl
         << "    Message " << msgName << ";" << endl // Declare message variable
         << "    DecAprocs();" << endl
         << "    " << ToC_Name(myChannel.GetName()) << "->Receive(" << int2string(myChannel.GetIndex()-1) << ", " << msgName << ");" << endl // Receive message
         << "    IncAprocs();" << endl
         << "    " << msgName << ".GetValue(" << lblName << ");" << endl
         << "  }" << endl;
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    if (it != myBranches.begin())
      result << "  else ";
    result << "  if (" << lblName << ".ToString()==\"" << it->first << "\")" << endl
           << "  {" << endl;
    if (find(myFinalBranches.begin(),myFinalBranches.end(),lblName)!=myFinalBranches.end()) {
      result << "    " << ToC_Name(myChannel.GetName()) << "->Close(true);" << endl
             << "    delete " << ToC_Name(myChannel.GetName()) << ";" << endl;
    }
    result << it->second->ToC();
    result << "  }" << endl;
  }
  result << "  else throw (string)\"Unknown branch: " << lblName << "\";" << endl;
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
  parTerm = new MpsBranch(myChannel,newBranches, GetFinalBranches());
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
MpsTerm *MpsBranch::CloseDefinitions() const // {{{
{
  map<string,MpsTerm*> newBranches;
  for (map<string,MpsTerm*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
    newBranches[it->first]=it->second->CloseDefinitions();

  MpsTerm *result=new MpsBranch(myChannel,newBranches, GetFinalBranches());
  DeleteMap(newBranches);

  return result;
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
