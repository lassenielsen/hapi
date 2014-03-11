#include<apims/mpsselect.hpp>
#include<apims/mpsend.hpp>
#include "common.cpp"

using namespace std;
using namespace apims;

MpsSelect::MpsSelect(const MpsChannel &channel, const string &label, const MpsTerm &succ, bool final) // {{{
: myChannel(channel)
, myLabel(label)
, myFinal(final)
{
  // Assert succ!=NULL
  mySucc = succ.Copy();
} // }}}
MpsSelect::~MpsSelect() // {{{
{
  delete mySucc;
} // }}}
bool MpsSelect::TypeCheck(const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega) // Use rule Sel {{{
{
  MpsMsgEnv::const_iterator session=Gamma.find(myChannel.GetName());
  // Check session is open
  if (session==Gamma.end())
  {
    return PrintTypeError((string)"Typechecking error - Selecting on closed session: " + myChannel.GetName(),*this,Theta,Gamma,Omega);
  }
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
  // Check session has select type
  const MpsLocalSelectType *selType = dynamic_cast<const MpsLocalSelectType*>(msgType->GetLocalType());
  if (selType==NULL)
    return PrintTypeError((string)"Typechecking error - Selecting on session: " + myChannel.GetName() + "with type: " + msgType->GetLocalType()->ToString("           "),*this,Theta,Gamma,Omega);
  // Check channel index is correct
  if (myChannel.GetIndex() != selType->GetReceiver())
    return PrintTypeError((string)"Typechecking error - Sending on session: " + myChannel.ToString() + "with type: " + selType->ToString("           "),*this,Theta,Gamma,Omega);
  // Check label ok
  map<string,MpsLocalType*>::const_iterator branch=selType->GetBranches().find(myLabel);
  if (branch==selType->GetBranches().end())
    return PrintTypeError((string)"Typechecking error - Sending label: " + myLabel + "on session: " + myChannel.ToString() + "with type: " + selType->ToString("           "),*this,Theta,Gamma,Omega);
  // Check label is active (assertion valid)
  vector<const MpsExp*> hyps;
  hyps.push_back(&Theta);
  map<string,MpsExp*>::const_iterator assertion=selType->GetAssertions().find(myLabel);
  if (assertion==selType->GetAssertions().end())
    return PrintTypeError((string)"Label has no assertion",*this,Theta,Gamma,Omega);
  if (not assertion->second->ValidExp(hyps))
    return PrintTypeError((string)"Assertion not respected",*this,Theta,Gamma,Omega);
  
  // Make new environment
  MpsMsgEnv newGamma = Gamma;
  newGamma[myChannel.GetName()] = new MpsDelegateLocalMsgType(*branch->second,msgType->GetPid(),msgType->GetParticipants());

  // Store if this is final action in session
  myFinal=branch->second->IsDone();
  
  // Check rest of program
  bool result = mySucc->TypeCheck(Theta,newGamma,Omega);

  // Clean up
  delete newGamma[myChannel.GetName()];

  return result;
} // }}}
MpsTerm *MpsSelect::ApplyBSnd(const std::string &path, std::string &label, MpsChannel &ch) const // {{{
{ if (path.size()>0)
    return Error((string)"Applying BSnd on "+ToString()+" with nonempty path "+path);
  label=myLabel;
  ch=myChannel;
  return mySucc->Copy();
} // }}}
bool MpsSelect::SubSteps(vector<MpsStep> &dest) // {{{
{
  MpsEvent event;
  event.myType = bsnd;
  event.myChannel = myChannel;
  event.myLabel = myLabel;
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event, paths));
  return false;
} // }}}
MpsTerm *MpsSelect::PRename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PRename(src, dst);
  MpsTerm *result = new MpsSelect(myChannel, myLabel, *newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSelect::ERename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  // assert myChannel != dest
  MpsChannel newChannel=myChannel.Rename(src,dst);
  MpsTerm *newSucc = mySucc->ERename(src,dst);
  MpsTerm *result = new MpsSelect(newChannel, myLabel, *newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSelect::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  // assert mySucc != NULL
  // assert myChannel != dest
  MpsChannel newChannel=myChannel;
  if (session==myChannel.GetName())
    newChannel=MpsChannel(myChannel.GetName(),myChannel.GetIndex()*maxpid+pid);
  MpsTerm *newSucc = mySucc->ReIndex(session,pid,maxpid);
  MpsTerm *result = new MpsSelect(newChannel, myLabel, *newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSelect::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PSubst(var, exp, args, argpids, stateargs);
  MpsTerm *result = new MpsSelect(myChannel, myLabel, *newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSelect::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  // assert mySucc != NULL
  // assert myChannel != dest
  MpsChannel newChannel=myChannel.Subst(source,dest);
  string newLabel = myLabel;
  MpsTerm *newSucc = mySucc->ESubst(source,dest);
  MpsTerm *result = new MpsSelect(newChannel, newLabel, *newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSelect::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  MpsTerm *result = new MpsSelect(myChannel, myLabel, *newSucc, GetFinal());

  // Clean Up
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsSelect::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->LSubst(source,dest,args);
  MpsTerm *result = new MpsSelect(myChannel, myLabel, *newSucc, GetFinal());

  // Clean Up
  delete newSucc;

  return result;
} // }}}
set<string> MpsSelect::FPV() const // {{{
{
  set<string> result = mySucc->FPV();
  return result;
} // }}}
set<string> MpsSelect::FEV() const // {{{
{
  set<string> result = mySucc->FEV();
  result.insert(myChannel.GetName());
  return result;
} // }}}
MpsTerm *MpsSelect::Copy() const // {{{
{
  // assert mySucc != NULL
  return new MpsSelect(myChannel, myLabel, *mySucc, GetFinal());
} // }}}
bool MpsSelect::Terminated() const // {{{
{
  return false;
} // }}}
MpsTerm *MpsSelect::Simplify() const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->Simplify();
  MpsTerm *result = new MpsSelect(myChannel, myLabel, *newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
string MpsSelect::ToString(string indent) const // {{{
{
  return myChannel.ToString() + " << " + myLabel + ";\n" + indent + mySucc->ToString(indent);
} // }}}
string MpsSelect::ToTex(int indent, int sw) const // {{{
{
  return ToTex_Channel(myChannel) + "$\\ll$ "
       + ToTex_Label(myLabel) + ";\\newline\n"
       + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
} // }}}
string MpsSelect::ToC() const // {{{
{
  stringstream result;
  string msgName = ToC_Name(MpsExp::NewVar("select")); // Create variable name foor the mmessagee to send
  result << "  {" << endl
         << "    Message " << " " << msgName << ";" << endl
         << "    " << msgName << ".AddData(" << "\"" << myLabel << "\", " << int2string(myLabel.size()+1) << ");" << endl
         << ToC_Name(myChannel.GetName()) << "->Send(" << int2string(myChannel.GetIndex()-1) << "," << msgName << ");" << endl
         << "  }" << endl; // Send label
  result << mySucc->ToC();
  if (myFinal)
  {
    result << "  " << ToC_Name(myChannel.GetName()) << "->Close(true);" << endl
           << "  delete " << ToC_Name(myChannel.GetName()) << ";" << endl;
  }
  return result.str();
} // }}}
string MpsSelect::ToCHeader() const // {{{
{
  return mySucc->ToCHeader();
} // }}}
MpsTerm *MpsSelect::RenameAll() const // {{{
{ MpsTerm *newSucc=mySucc->RenameAll();
  MpsTerm *result=new MpsSelect(myChannel,myLabel,*newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
bool MpsSelect::Parallelize(const MpsTerm &receivers, MpsTerm* &seqTerm, MpsTerm* &parTerm) const // {{{
{
  // Find used vars
  set<string> usedVars;
  usedVars.insert(myChannel.GetName());
  // Split receives using the used vars
  MpsTerm *pre;
  MpsTerm *post;
  receivers.Split(usedVars,pre,post);
  bool opt1=dynamic_cast<MpsEnd*>(post)==NULL;
  // Parallelize succ with post receives
  MpsTerm *seqSucc;
  MpsTerm *parSucc;
  bool opt2=mySucc->Parallelize(*post,seqSucc,parSucc);
  delete post;
  // Make parallelized term
  MpsTerm *parTmp = new MpsSelect(myChannel, myLabel, *parSucc, GetFinal());
  delete parSucc;
  parTerm = pre->Append(*parTmp);
  delete pre;
  delete parTmp;
  // Make sequential term
  seqTerm = new MpsSelect(myChannel, myLabel, *seqSucc, GetFinal());
  delete seqSucc;
  return opt1 || opt2;
} // }}}
MpsTerm *MpsSelect::Append(const MpsTerm &term) const // {{{
{
  MpsTerm *newSucc=mySucc->Append(term);
  MpsTerm *result = new MpsSelect(myChannel, myLabel, *newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsSelect::CloseDefinitions() const // {{{
{
  MpsTerm *newSucc = mySucc->CloseDefinitions();
  MpsTerm *result=new MpsSelect(myChannel,myLabel,*newSucc, GetFinal());
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsSelect::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ MpsTerm *newSucc=mySucc->ExtractDefinitions(env);
  MpsTerm *result=new MpsSelect(myChannel,myLabel,*newSucc, GetFinal());
  delete newSucc;
  return result;
} // }}}
bool MpsSelect::GetFinal() const // {{{
{ return myFinal;
} // }}}
