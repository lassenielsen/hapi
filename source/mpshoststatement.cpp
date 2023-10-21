#include<hapi/mpshoststatement.hpp>
#include<hapi/mpsend.hpp>
#include<hapi/common.hpp>

using namespace std;
using namespace hapi;

MpsHostStatement::MpsHostStatement(const vector<string> &hostParts, const vector<MpsExp*> &expParts, const MpsTerm &succ, vector<MpsMsgType*> types, bool pure) // {{{
: myHostParts(hostParts)
, myPure(pure)
{
  for (vector<MpsExp*>::const_iterator exp=expParts.begin(); exp!=expParts.end(); ++exp)
    myExpParts.push_back((*exp)->Copy());
  for (vector<MpsMsgType*>::const_iterator tit=types.begin(); tit!=types.end(); ++tit)
    myTypes.push_back((*tit)->Copy());

  mySucc = succ.Copy();
} // }}}
MpsHostStatement::~MpsHostStatement() // {{{
{
  delete mySucc;
  DeleteVector(myExpParts);
  DeleteVector(myTypes);
} // }}}
void *MpsHostStatement::TDCompileMain(tdc_pre pre, tdc_post wrap, tdc_error wrap_err, const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, const string &curPure, PureState pureState, bool checkPure) // Use rule Nres {{{
{ map<string,void*> children;
 
  if (checkPure)
	{ // Check purity constraints
    if (pureState!=CPS_IMPURE && pureState!=CPS_PURE)
      return wrap_err(this,PrintTypeError("Error in implementation of pure participant " + curPure + ". Pure implementations must conform with the structure \n     *   local X()\n	   *   ( global s=new ch(p of n);\n		 *     X();\n		 *     |\n		 *     P\n		 *   )\n		 *   local StartX(Int i)\n		 *   ( if i<=0\n		 *     then X();\n		 *     else X(); | StartX(i-1);\n		 *   )\n		 *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);

    if (pureState==CPS_PURE && !myPure)
      return wrap_err(this,PrintTypeError("Impure hoststatement in pure setting." ,*this,Theta,Gamma,Omega),children);
  }

  // Verify hoststatement
  // Clear infered typing info
  DeleteVector(myTypes);

  // Check that var-parts are wekk typed
  for (vector<MpsExp*>::const_iterator part=myExpParts.begin(); part!=myExpParts.end(); ++part)
  { MpsMsgType *partType=(*part)->TypeCheck(Gamma);
    myTypes.push_back(partType);
    if (dynamic_cast<MpsMsgNoType*>(partType)!=NULL)
      return wrap_err(this,PrintTypeError("Host Language Statement uses untypable expression: " + (*part)->ToString(),*this,Theta,Gamma,Omega),children);
    if (dynamic_cast<const MpsDelegateMsgType*>(partType)!=NULL)
      return wrap_err(this,PrintTypeError("Host Language Statement uses session variable in expression: " + (*part)->ToString(),*this,Theta,Gamma,Omega),children);
  }

  children["succ"] = mySucc->TDCompile(pre,wrap,wrap_err,Theta,Gamma,Omega,pureStack,curPure, pureState, checkPure);
  // Wrap result
  return wrap(this,Theta,Gamma,Omega,pureStack,curPure,pureState,checkPure,children);
} // }}}
MpsTerm *MpsHostStatement::ApplyOther(const std::string &path) const // {{{
{ if (path.size()!=0)
    return Error((string)"Applying Other with nonempty path on "+ToString());
  MpsTerm *result = mySucc->Copy();
  return result;
} // }}}
bool MpsHostStatement::SubSteps(vector<MpsStep> &dest) // {{{
{
  MpsEvent event;
  event.myType = tau;
  event.mySubType = host;
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event, paths));
  return false;
} // }}}
MpsHostStatement *MpsHostStatement::PRename(const string &src, const string &dst) const // {{{
{
  MpsTerm *newSucc = mySucc->PRename(src,dst);
  MpsHostStatement *result = new MpsHostStatement(myHostParts, myExpParts, *newSucc, myTypes, myPure);
  delete newSucc;
  return result;
} // }}}
MpsHostStatement *MpsHostStatement::ERename(const string &src, const string &dst) const // {{{
{
  // Rename in succ
  MpsTerm *newSucc = mySucc->ERename(src,dst);
  // Rename in exps
  vector<MpsExp*> newExpParts;
  for (vector<MpsExp*>::const_iterator part=myExpParts.begin(); part!=myExpParts.end(); ++part)
    newExpParts.push_back((*part)->Rename(src,dst));

  MpsHostStatement *result = new MpsHostStatement(myHostParts, newExpParts, *newSucc, myTypes, myPure);
  // Clean up
  delete newSucc;
  DeleteVector(newExpParts);

  return result;
} // }}}
MpsHostStatement *MpsHostStatement::MRename(const string &src, const string &dst) const // {{{
{
  // Rename in succ
  MpsTerm *newSucc = mySucc->MRename(src,dst);

  MpsHostStatement *result = new MpsHostStatement(myHostParts, myExpParts, *newSucc, myTypes, myPure);
  // Clean up
  delete newSucc;

  return result;
} // }}}
MpsHostStatement *MpsHostStatement::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->ReIndex(session,pid,maxpid);
  // myExpParts cannot use sessions!
  MpsHostStatement *result = new MpsHostStatement(myHostParts, myExpParts, *newSucc, myTypes, myPure);
  // Clean up
  delete newSucc;
  return result;
} // }}}
MpsHostStatement *MpsHostStatement::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PSubst(var, exp, args, argpids, stateargs);
  MpsHostStatement *result = new MpsHostStatement(myHostParts, myExpParts, *newSucc, myTypes, myPure);
  delete newSucc;
  return result;
} // }}}
MpsHostStatement *MpsHostStatement::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  // Substitute in succ
  MpsTerm *newSucc = mySucc->ESubst(source,dest);
  // Substitute in expParts
  vector<MpsExp*> newExpParts;
  for (vector<MpsExp*>::const_iterator part=myExpParts.begin(); part!=myExpParts.end(); ++part)
    newExpParts.push_back((*part)->Subst(source,dest));
  // Create result
  MpsHostStatement *result = new MpsHostStatement(myHostParts, newExpParts, *newSucc, myTypes, myPure);
  // Clean up
  delete newSucc;
  DeleteVector(newExpParts);

  return result;
} // }}}
MpsHostStatement *MpsHostStatement::MSubst(const string &source, const MpsMsgType &dest) const // {{{
{
  MpsTerm *newSucc = mySucc->MSubst(source,dest);
  vector<MpsMsgType*> newTypes;
  for (vector<MpsMsgType*>::const_iterator tit=myTypes.begin(); tit!=myTypes.end(); ++tit)
    newTypes.push_back((*tit)->MSubst(source,dest));

  MpsHostStatement *result = new MpsHostStatement(myHostParts, myExpParts, *newSucc,newTypes, myPure);

  // Clean up
  delete newSucc;
  DeleteVector(newTypes);

  return result;
} // }}}
MpsHostStatement *MpsHostStatement::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  vector<MpsMsgType*> newTypes;
  for (vector<MpsMsgType*>::const_iterator tit=myTypes.begin(); tit!=myTypes.end(); ++tit)
    newTypes.push_back((*tit)->GSubst(source,dest,args));

  MpsHostStatement *result = new MpsHostStatement(myHostParts, myExpParts, *newSucc,newTypes, myPure);

  // Clean up
  delete newSucc;
  DeleteVector(newTypes);

  return result;
} // }}}
MpsHostStatement *MpsHostStatement::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->LSubst(source,dest,args);
  vector<MpsMsgType*> newTypes;
  for (vector<MpsMsgType*>::const_iterator tit=myTypes.begin(); tit!=myTypes.end(); ++tit)
    newTypes.push_back((*tit)->LSubst(source,dest,args));
  MpsHostStatement *result = new MpsHostStatement(myHostParts, myExpParts, *newSucc, newTypes, myPure);

  // Clean up
  delete newSucc;
  DeleteVector(newTypes);

  return result;
} // }}}
set<string> MpsHostStatement::FPV() const // {{{
{
  return mySucc->FPV();
} // }}}
set<string> MpsHostStatement::EV() const // {{{
{
  set<string> result = mySucc->EV();
  for (vector<MpsExp*>::const_iterator exp=myExpParts.begin(); exp!=myExpParts.end(); ++exp)
  { set<string> fv = (*exp)->FV();
    result.insert(fv.begin(),fv.end());
  }
  return result;
} // }}}
set<string> MpsHostStatement::FEV() const // {{{
{
  set<string> result = mySucc->FEV();
  for (vector<MpsExp*>::const_iterator exp=myExpParts.begin(); exp!=myExpParts.end(); ++exp)
  { set<string> fv = (*exp)->FV();
    result.insert(fv.begin(),fv.end());
  }
  return result;
} // }}}
MpsHostStatement *MpsHostStatement::Copy() const // {{{
{
  return new MpsHostStatement(myHostParts, myExpParts, *mySucc, myTypes, myPure);
} // }}}
bool MpsHostStatement::Terminated() const // {{{
{
  return false;
} // }}}
MpsHostStatement *MpsHostStatement::Simplify() const // {{{
{
  MpsTerm *newSucc = mySucc->Simplify();
  MpsHostStatement *result = new MpsHostStatement(myHostParts, myExpParts, *newSucc, myTypes, myPure);
  delete newSucc;
  return result;
} // }}}
string MpsHostStatement::ToString(string indent) const // {{{
{
  stringstream result;
  result << "HOST(\"";
  for (int i=0; i<myHostParts.size(); ++i)
  { result << myHostParts[i];
    if (myExpParts.size()>i)
      result << "\", " << myExpParts[i]->ToString() << "\", \"";
  }
  result << "\");" << endl
         << indent << mySucc->ToString(indent);
  return result.str();
} // }}}
string MpsHostStatement::ToTex(int indent, int sw) const // {{{
{
  stringstream result;
  result << "HOST(\"";
  for (int i=0; i<myHostParts.size(); ++i)
  { result << myHostParts[i];
    if (myExpParts.size()>i)
      result << "\", " << myExpParts[i]->ToString() << ", \"";
  }
  result << "\");" << endl
         << ToTex_Hspace(indent,sw) << mySucc->ToTex(indent,sw);
  return result.str();
} // }}}
string MpsHostStatement::ToC(const string &taskType) const // {{{
{
  stringstream prestmt;
  stringstream stmt;
  for (int i=0; i<myHostParts.size(); ++i)
  { stmt << myHostParts[i];
    if (i<myExpParts.size())
    { string newName = myExpParts[i]->ToC(prestmt, myTypes[i]->ToCPtr());
      stmt << " " << newName << " ";
    }
  }
  return ToC_Yield() + string("  // HOST STATEMENT BEGIN\n    ") + prestmt.str() + stmt.str() + "  // HOST STATEMENT END\n" + mySucc->ToC(taskType);
} // }}}
string MpsHostStatement::ToCHeader() const // {{{
{
  return mySucc->ToCHeader();
} // }}}
void MpsHostStatement::ToCConsts(vector<string> &dest, unordered_set<string> &existing) const // {{{
{ for (vector<MpsExp*>::const_iterator part=myExpParts.begin(); part!=myExpParts.end(); ++part)
    (*part)->ToCConsts(dest,existing);
  mySucc->ToCConsts(dest,existing);
} // }}}
MpsTerm *MpsHostStatement::FlattenFork(bool normLhs, bool normRhs, bool pureMode) const // {{{
{
  MpsTerm *newSucc = mySucc->FlattenFork(normLhs,normRhs,pureMode);
  MpsHostStatement *result= new MpsHostStatement(myHostParts, myExpParts, *newSucc, myTypes, myPure);
  delete newSucc;
  return result;
} // }}}
MpsHostStatement *MpsHostStatement::RenameAll() const // {{{
{ MpsTerm *newSucc=mySucc->RenameAll();
  vector<MpsMsgType*> newTypes;
  for (vector<MpsMsgType*>::const_iterator tit=myTypes.begin(); tit!=myTypes.end(); ++tit)
    newTypes.push_back((*tit)->RenameAll());

  MpsHostStatement *result=new MpsHostStatement(myHostParts, myExpParts, *newSucc, newTypes, myPure);
  // Clean up
  delete newSucc;
  DeleteVector(newTypes);

  return result;
} // }}}
bool MpsHostStatement::Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const // {{{
{ // Find used vars
  set<string> usedVars;
  for (vector<MpsExp*>::const_iterator exp=myExpParts.begin(); exp!=myExpParts.end(); ++exp)
  { set<string> fv = (*exp)->FV();
    usedVars.insert(fv.begin(),fv.end());
  }
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
  MpsTerm *parTmp = new MpsHostStatement(myHostParts, myExpParts, *parSucc, myTypes, myPure);
  delete parSucc;
  parTerm = pre->Append(*parTmp);
  delete pre;
  delete parTmp;
  // Make sequential term
  seqTerm = new MpsHostStatement(myHostParts, myExpParts, *seqSucc, myTypes, myPure);
  delete seqSucc;
  return opt1 || opt2;
} // }}}
MpsTerm *MpsHostStatement::Append(const MpsTerm &term) const // {{{
{ MpsTerm *newSucc=mySucc->Append(term);
  MpsTerm *result=new MpsHostStatement(myHostParts, myExpParts, *newSucc, myTypes, myPure);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsHostStatement::CopyWrapper(std::map<std::string,void*> &children) const // {{{
{ return new MpsHostStatement(myHostParts, myExpParts, *(MpsTerm*)children["succ"], myTypes, myPure);
} // }}}
MpsTerm *MpsHostStatement::CloseDefsPre(const MpsMsgEnv &Gamma) // {{{
{ return this;
} // }}}
MpsHostStatement *MpsHostStatement::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ MpsTerm *newSucc=mySucc->ExtractDefinitions(env);
  MpsHostStatement *result=new MpsHostStatement(myHostParts, myExpParts, *newSucc, myTypes, myPure);
  delete newSucc;
  return result;
} // }}}
