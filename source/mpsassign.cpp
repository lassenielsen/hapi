#include<apims/mpsassign.hpp>
#include<apims/mpsend.hpp>
#include <apims/common.hpp>

using namespace std;
using namespace apims;

MpsAssign::MpsAssign(const string &id, const MpsExp &exp, const MpsMsgType &type, const MpsTerm &succ) // {{{
{
  myId = id;
  myExp = exp.Copy();
  myType = type.Copy();
  mySucc = succ.Copy();
} // }}}
MpsAssign::~MpsAssign() // {{{
{
  delete myExp;
  delete myType;
  delete mySucc;
} // }}}
bool MpsAssign::TypeCheck(const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, const string &curPure) // * Check exp has correct type, and check succ in updated sigma {{{
{
  // Check purity constraints
  if (pureStack.size()>0)
    return PrintTypeError("Implementation of pure participant " + int2string(pureStack.begin()->second) + "@" + pureStack.begin()->first + " must be immediately after its decleration",*this,Theta,Gamma,Omega);
  // Verify assign
  if (dynamic_cast<MpsDelegateMsgType*>(myType)!=NULL)
    return PrintTypeError("Assignment type cannot be a session, because it breaks linearity",*this,Theta,Gamma,Omega);
  MpsMsgType *exptype=myExp->TypeCheck(Gamma);
  bool exptypematch = exptype->Equal(Theta,*myType);
  delete exptype;
  if (not exptypematch)
    return PrintTypeError((string)"Expression does not have type: " + myType->ToString(),*this,Theta,Gamma,Omega);
  // Check no session is eclipsed
  MpsMsgEnv::const_iterator var=Gamma.find(myId);
  if (var!=Gamma.end() && dynamic_cast<const MpsDelegateMsgType*>(var->second)!=NULL)
    return PrintTypeError((string)"Session eclipsed by assignment: " + myId,*this,Theta,Gamma,Omega);
  // Make new environment
  string newId = MpsExp::NewVar(myId);
  MpsExp *tmpTheta=Theta.Rename(myId,newId);
  MpsExp *newTheta;
  if (myType->ToString()=="Bool")
  { MpsExp *eq1 = new MpsVarExp(myId,MpsMsgNoType());
    MpsExp *eq2 = myExp->Rename(myId,newId);
    MpsExp *neq1=new MpsUnOpExp("not",*eq1);
    MpsExp *neq2=new MpsUnOpExp("not",*eq2);
    MpsExp *leftExp=new MpsBinOpExp("or",*eq1,*neq2,MpsMsgNoType(),MpsMsgNoType());
    MpsExp *rightExp=new MpsBinOpExp("or",*neq1,*eq2,MpsMsgNoType(),MpsMsgNoType());
    MpsExp *addTheta=new MpsBinOpExp("and",*leftExp,*rightExp,MpsMsgNoType(),MpsMsgNoType());
    newTheta=new MpsBinOpExp("and",*tmpTheta,*addTheta,MpsMsgNoType(),MpsMsgNoType());
    delete eq1;
    delete eq2;
    delete neq1;
    delete neq2;
    delete leftExp;
    delete rightExp;
    delete addTheta;
    delete tmpTheta;
  }
  else
    newTheta=tmpTheta;
  MpsMsgEnv newGamma;
  for (MpsMsgEnv::const_iterator it=Gamma.begin(); it!=Gamma.end(); ++it)
    if (it->first!=myId)
      newGamma[it->first]=it->second->ERename(myId,newId);
  newGamma[myId]=myType->Copy();
  // Check new Successor
  bool result = mySucc->TypeCheck(*newTheta,newGamma,Omega,pureStack,curPure);
  delete newTheta;
  DeleteMap(newGamma);
  return result;
} // }}}
MpsTerm *MpsAssign::ApplyOther(const std::string &path) const // {{{
{ if (path.size()!=0)
    return Error((string)"Applying Other with nonempty path on "+ToString());
  MpsExp *value=myExp->Eval();
  MpsTerm *result = mySucc->ESubst(myId,*value);
  delete value;
  return result;
} // }}}
bool MpsAssign::SubSteps(vector<MpsStep> &dest) // {{{
{
  MpsEvent event;
  event.myType = tau;
  event.mySubType = assign;
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event, paths));
  return false;
} // }}}
MpsAssign *MpsAssign::PRename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PRename(src,dst);
  MpsAssign *result = new MpsAssign(myId, *myExp, *myType, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsAssign *MpsAssign::ERename(const string &src, const string &dst) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->ERename(src,dst);
  MpsExp *newExp = myExp->Rename(src,dst);
  MpsAssign *result = new MpsAssign(myId, *newExp, *myType, *newSucc);
  delete newSucc;
  delete newExp;
  return result;
} // }}}
MpsAssign *MpsAssign::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->ReIndex(session,pid,maxpid);
  MpsAssign *result = new MpsAssign(myId, *myExp, *myType, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsAssign *MpsAssign::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PSubst(var, exp, args, argpids, stateargs);
  MpsAssign *result = new MpsAssign(myId, *myExp, *myType, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsAssign *MpsAssign::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  MpsExp *newExp=myExp->Subst(source,dest);
  MpsTerm *newSucc;
  if (myId==source)
    newSucc=mySucc->Copy();
  else
    newSucc = mySucc->ESubst(source,dest);
  MpsAssign *result = new MpsAssign(myId, *newExp, *myType, *newSucc);
  delete newExp;
  delete newSucc;
  return result;
} // }}}
MpsAssign *MpsAssign::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsMsgType *newType=myType->GSubst(source,dest,args);
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  MpsAssign *result = new MpsAssign(myId, *myExp, *newType, *newSucc);

  // Clean Up
  delete newType;
  delete newSucc;

  return result;
} // }}}
MpsAssign *MpsAssign::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsMsgType *newType=myType->LSubst(source,dest,args);
  MpsTerm *newSucc = mySucc->LSubst(source,dest,args);
  MpsAssign *result = new MpsAssign(myId, *myExp, *newType, *newSucc);

  // Clean Up
  delete newType;
  delete newSucc;

  return result;
} // }}}
set<string> MpsAssign::FPV() const // {{{
{
  return mySucc->FPV();
} // }}}
set<string> MpsAssign::FEV() const // {{{
{
  set<string> result = mySucc->FEV();
  result.erase(myId);
  set<string> fv=myExp->FV();
  result.insert(fv.begin(),fv.end());
  return result;
} // }}}
MpsAssign *MpsAssign::Copy() const // {{{
{
  return new MpsAssign(myId, *myExp, *myType, *mySucc);
} // }}}
bool MpsAssign::Terminated() const // {{{
{
  return false;
} // }}}
MpsAssign *MpsAssign::Simplify() const // {{{
{
  MpsTerm *newSucc = mySucc->Simplify();
  MpsAssign *result = new MpsAssign(myId, *myExp, *myType, *newSucc);
  delete newSucc;
  return result;
} // }}}
string MpsAssign::ToString(string indent) const // {{{
{
  return myType->ToString() + " " + myId + "=" + myExp->ToString() + ";\n" + indent + mySucc->ToString(indent);
} // }}}
string MpsAssign::ToTex(int indent, int sw) const // {{{
{
  return ToTex_Var(myId) + ":" + myType->ToTex(indent + myId.size() + 1) + "=" + myExp->ToString() + ";\\newline\n"
       + ToTex_Hspace(indent,sw) + mySucc->ToTex(indent,sw);
} // }}}
string MpsAssign::ToC() const // {{{
{
  stringstream result;
  result << myType->ToC() << " " << ToC_Name(myId) << ";" << endl;
  string varName = myExp->ToC(result,GetExpType().ToC());
  MpsTerm *tmpSucc = mySucc->ERename(myId,varName);
  result << tmpSucc->ToC();
  delete tmpSucc;
  return result.str();
} // }}}
string MpsAssign::ToCHeader() const // {{{
{
  return mySucc->ToCHeader();
} // }}}
MpsTerm *MpsAssign::RenameAll() const // {{{
{ string newId=MpsExp::NewVar(myId);
  MpsTerm *tmpSucc=mySucc->ERename(myId,newId);
  MpsTerm *newSucc=tmpSucc->RenameAll();
  delete tmpSucc;
  MpsMsgType *newType=myType->RenameAll();
  MpsTerm *result=new MpsAssign(newId,*myExp,*newType,*newSucc);
  
  delete newSucc;
  delete newType;

  return result;
} // }}}
bool MpsAssign::Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const // {{{
{ // Find used vars
  set<string> usedVars = myExp->FV();
  usedVars.insert(myId);
  // Split receives using the used vars
  MpsTerm *pre;
  MpsTerm *post;
  receives.Split(usedVars,pre,post);
  bool opt1=dynamic_cast<const MpsEnd*>(post)==NULL;
  // Parallelize succ with post receives
  MpsTerm *seqSucc;
  MpsTerm *parSucc;
  bool opt2=mySucc->Parallelize(*post,seqSucc,parSucc);
  delete post;
  // Make parallelized term
  MpsTerm *parTmp = new MpsAssign(myId, *myExp, *myType, *parSucc);
  delete parSucc;
  parTerm = pre->Append(*parTmp);
  delete pre;
  delete parTmp;
  // Make sequential term
  seqTerm = new MpsAssign(myId, *myExp, *myType, *seqSucc);
  delete seqSucc;
  return opt1 || opt2;
} // }}}
MpsTerm *MpsAssign::Append(const MpsTerm &term) const // {{{
{ MpsTerm *newSucc=mySucc->Append(term);
  MpsTerm *result=new MpsAssign(myId, *myExp, *myType, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsAssign::CloseDefinitions() const // {{{
{
  MpsTerm *newSucc = mySucc->CloseDefinitions();
  MpsTerm *result= new MpsAssign(myId, *myExp, *myType, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsAssign::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ MpsTerm *newSucc=mySucc->ExtractDefinitions(env);
  MpsTerm *result=new MpsAssign(myId,*myExp,*myType,*newSucc);
  
  delete newSucc;

  return result;
} // }}}
const MpsMsgType &MpsAssign::GetExpType() const // {{{
{ return *myType;
} // }}}
