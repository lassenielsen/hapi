#include<apims/mpsguivalue.hpp>
#include<apims/mpsend.hpp>
#include <apims/mpsgui.hpp>
#include <apims/common.hpp>

using namespace std;
using namespace apims;

MpsGuiValue::MpsGuiValue(int maxpid, const std::string &session, int pid, const MpsExp &name, const MpsExp &value, const MpsTerm &succ) // {{{
: mySession(session),
  myMaxpid(maxpid),
  myPid(pid)
{
  mySucc = succ.Copy();
  myValue = value.Copy();
  myName = name.Copy();
} // }}}
MpsGuiValue::~MpsGuiValue() // {{{
{
  delete myName;
  delete myValue;
  delete mySucc;
} // }}}
bool MpsGuiValue::TypeCheck(const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, const string &curPure) // Type check name, value and session {{{
{
  // Check purity constraints
  if (pureStack.size()>0)
    return PrintTypeError("Implementation of pure participant " + int2string(pureStack.begin()->second) + "@" + pureStack.begin()->first + " must be immediately after its decleration",*this,Theta,Gamma,Omega);

  // Verify guivalue
  MpsStringMsgType stringtype;
  // Check label is a string
  MpsMsgType *nametype = myName->TypeCheck(Gamma);
  bool nametypematch = stringtype.Equal(Theta,*nametype);
  delete nametype;
  if (!nametypematch)
    return PrintTypeError("Name for guivalue must be of type String",*this,Theta,Gamma,Omega);
  // Check ownership of session
  MpsMsgEnv::const_iterator var=Gamma.find(mySession);
  // Check that session exists
  if (var==Gamma.end())
    return PrintTypeError((string)"guivalue on unknown session " + mySession,*this,Theta,Gamma,Omega);
  // Check if session type
  const MpsDelegateMsgType *msgType = dynamic_cast<const MpsDelegateMsgType*>(var->second);
  if (msgType==NULL)
    return PrintTypeError((string)"guivalue on non-session type: " + mySession,*this,Theta,Gamma,Omega);

  // Check if correct PID and MaxPID is given
  if (myPid != msgType->GetPid())
    return PrintTypeError((string)"guivalue gives wrong pid for session: " + mySession,*this,Theta,Gamma,Omega);
  if (myMaxpid != msgType->GetMaxpid())
    return PrintTypeError((string)"guivalue gives wrong maxpid for sessoin: " + mySession,*this,Theta,Gamma,Omega);
  // Check that value is welltyped
  MpsMsgType *valType = myValue->TypeCheck(Gamma);
  bool untyped = dynamic_cast<MpsMsgNoType*>(valType)!=NULL;
  delete valType;
  if (untyped)
    return PrintTypeError((string)"guivalue uses untyped expression: " + myValue->ToString(),*this,Theta,Gamma,Omega);

  return mySucc->TypeCheck(Theta,Gamma,Omega,pureStack,curPure);
} // }}}
MpsTerm *MpsGuiValue::ApplyOther(const std::string &path) const // {{{
{ if (path.size()!=0)
    return Error((string)"Applying Other with nonempty path on "+ToString());
  MpsExp *name=myName->Eval();
  MpsExp *value=myValue->Eval();
  MpsStringVal *stringValue = dynamic_cast<MpsStringVal*>(value);
  if (stringValue!=NULL)
    mpsgui::SetValue(mySession,myPid,name->ToString(),stringValue->ToString()); // Inform GUI using string content
  else
    mpsgui::SetValue(mySession,myPid,name->ToString(),value->ToString()); // Inform GUI
  delete name;
  delete value;
  return mySucc->Copy();
} // }}}
bool MpsGuiValue::SubSteps(vector<MpsStep> &dest) // {{{
{
  MpsEvent event;
  event.myType = tau;
  event.mySubType = guivalue;
  event.mySession=mySession;
  //event.myName=myName->ToString();
  event.myPid = myPid;
  event.myValue = myValue->Copy();
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event, paths));
  return false;
} // }}}
MpsTerm *MpsGuiValue::PRename(const string &src, const string &dst) const // {{{
{
  MpsTerm *newSucc = mySucc->PRename(src,dst);
  MpsGuiValue *result = new MpsGuiValue(myMaxpid, mySession, myPid, *myName, *myValue, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsGuiValue::ERename(const string &src, const string &dst) const // {{{
{
  string newSession=mySession==src?dst:mySession;
  MpsTerm *newSucc = mySucc->ERename(src,dst);
  MpsExp *newName = myName->Rename(src,dst);
  MpsExp *newValue = myValue->Rename(src,dst);
  MpsGuiValue *result = new MpsGuiValue(myMaxpid, newSession, myPid, *newName, *newValue, *newSucc);

  // Clean up
  delete newName;
  delete newValue;
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsGuiValue::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  MpsTerm *newSucc = mySucc->ReIndex(session,pid,maxpid);
  MpsGuiValue *result = new MpsGuiValue(myMaxpid, mySession, myPid, *myName, *myValue, *newSucc);

  // Clean up
  delete newSucc;

  return result;
} // }}}
MpsGuiValue *MpsGuiValue::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
{
  MpsTerm *newSucc = mySucc->PSubst(var, exp, args, argpids, stateargs);
  MpsGuiValue *result = new MpsGuiValue(myMaxpid, mySession, myPid, *myName, *myValue, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsGuiValue *MpsGuiValue::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  string newSession=stringESubst(mySession,source,dest);
  MpsTerm *newSucc = mySucc->ESubst(source,dest);
  MpsExp *newName = myName->Subst(source,dest);
  MpsExp *newValue = myValue->Subst(source,dest);
  MpsGuiValue *result = new MpsGuiValue(myMaxpid, newSession, myPid, *newName, *newValue, *newSucc);

  // Clean up
  delete newName;
  delete newValue;
  delete newSucc;

  return result;
} // }}}
MpsGuiValue *MpsGuiValue::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  MpsGuiValue *result = new MpsGuiValue(myMaxpid, mySession, myPid, *myName, *myValue, *newSucc);

  // Clean up
  delete newSucc;

  return result;
} // }}}
MpsGuiValue *MpsGuiValue::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->LSubst(source,dest,args);
  MpsGuiValue *result = new MpsGuiValue(myMaxpid, mySession, myPid, *myName, *myValue, *newSucc);

  // Clean up
  delete newSucc;

  return result;
} // }}}
set<string> MpsGuiValue::FPV() const // {{{
{
  set<string> result = mySucc->FPV();
  return result;
} // }}}
set<string> MpsGuiValue::FEV() const // {{{
{
  set<string> result = mySucc->FEV();
  result.insert(mySession);
  set<string> fv=myName->FV();
  result.insert(fv.begin(),fv.end());
  fv=myValue->FV();
  result.insert(fv.begin(),fv.end());
  return result;
} // }}}
MpsGuiValue *MpsGuiValue::Copy() const // {{{
{
  return new MpsGuiValue(myMaxpid, mySession, myPid, *myName, *myValue, *mySucc);
} // }}}
bool MpsGuiValue::Terminated() const // {{{
{
  return false;
} // }}}
MpsGuiValue *MpsGuiValue::Simplify() const // {{{
{
  // assert mySucc != NULL
  // assert myValue != NULL
  MpsTerm *newSucc = mySucc->Simplify();
  MpsGuiValue *result = new MpsGuiValue(myMaxpid, mySession, myPid, *myName, *myValue, *newSucc);
  delete newSucc;
  return result;
} // }}}
string MpsGuiValue::ToString(string indent) const // {{{
{
  return (string)"guivalue(" + int2string(myMaxpid) + "," + mySession + "," + int2string(myPid) + "," + myName->ToString() + "," + myValue->ToString() + ");\n" + indent + mySucc->ToString(indent);
} // }}}
string MpsGuiValue::ToTex(int indent, int sw) const // {{{
{
  return ToTex_KW("guivalue") + "(" + ToTex_PP(myMaxpid) + ","
       + ToTex_Session(mySession) + ","
       + ToTex_PP(myPid) + ","
       + myName->ToString() + ","
       + myValue->ToString() + ");\\newline\n"
       + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
} // }}}
string MpsGuiValue::ToC() const // {{{
{
  throw (string)"MpsGuiValue::ToC(): guivalue is not implemented yet!";
} // }}}
string MpsGuiValue::ToCHeader() const // {{{
{
  throw (string)"MpsGuiValue::ToC(): guivalue is not implemented yet!";
} // }}}
MpsTerm *MpsGuiValue::RenameAll() const // {{{
{ MpsTerm *newSucc=mySucc->RenameAll();
  MpsTerm *result=new MpsGuiValue(myMaxpid,mySession,myPid,*myName,*myValue,*newSucc);
  
  delete newSucc;

  return result;
} // }}}
bool MpsGuiValue::Parallelize(const MpsTerm &receivers, MpsTerm* &seqTerm, MpsTerm* &parTerm) const // {{{
{
  // Find used vars
  set<string> usedVars = myValue->FV();
  set<string> fv = myName->FV();
  usedVars.insert(fv.begin(),fv.end());
  usedVars.insert(mySession);
  // Split receives using the used vars
  MpsTerm *pre;
  MpsTerm *post;
  receivers.Split(usedVars,pre,post);
  bool opt1=dynamic_cast<const MpsEnd*>(post)==NULL;
  // Parallelize succ with post receives
  MpsTerm *seqSucc;
  MpsTerm *parSucc;
  bool opt2=mySucc->Parallelize(*post,seqSucc,parSucc);
  delete post;
  // Make parallelized term
  MpsTerm *parTmp = new MpsGuiValue(myMaxpid, mySession, myPid, *myName, *myValue, *parSucc);
  delete parSucc;
  parTerm = pre->Append(*parTmp);
  delete pre;
  delete parTmp;
  // Make sequential term
  seqTerm = new MpsGuiValue(myMaxpid, mySession, myPid, *myName, *myValue, *seqSucc);
  delete seqSucc;
  return opt1 || opt2;
} // }}}
MpsTerm *MpsGuiValue::Append(const MpsTerm &term) const // {{{
{
  MpsTerm *newSucc=mySucc->Append(term);
  MpsTerm *result=new MpsGuiValue(myMaxpid, mySession, myPid, *myName, *myValue, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsGuiValue::CloseDefinitions() const // {{{
{
  MpsTerm *newSucc = mySucc->CloseDefinitions();
  MpsTerm *result= new MpsGuiValue(myMaxpid, mySession, myPid, *myName, *myValue, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsGuiValue::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ MpsTerm *newSucc=mySucc->ExtractDefinitions(env);
  MpsTerm *result=new MpsGuiValue(myMaxpid,mySession,myPid,*myName,*myValue,*newSucc);
  
  delete newSucc;

  return result;
} // }}}
