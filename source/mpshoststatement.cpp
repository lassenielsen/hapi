#include<apims/mpshoststatement.hpp>
#include<apims/mpsend.hpp>
#include "common.cpp"

using namespace std;
using namespace apims;

MpsHostStatement::MpsHostStatement(const vector<string> &hostParts, const vector<MpsExp*> &expParts, const MpsTerm &succ, vector<MpsMsgType*> types) // {{{
: myHostParts(hostParts)
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
bool MpsHostStatement::TypeCheck(const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega) // Use rule Nres {{{
{
  // Clear infered typing info
  DeleteVector(myTypes);

  // Check that var-parts are wekk typed
  for (vector<MpsExp*>::const_iterator part=myExpParts.begin(); part!=myExpParts.end(); ++part)
  { MpsMsgType *partType=(*part)->TypeCheck(Gamma);
    myTypes.push_back(partType);
    if (dynamic_cast<MpsMsgNoType*>(partType)!=NULL)
      return PrintTypeError("Host Language Statement uses expression untypable expression: " + (*part)->ToString(),*this,Theta,Gamma,Omega);
    if (dynamic_cast<const MpsDelegateMsgType*>(partType)!=NULL)
      return PrintTypeError("Host Language Statement uses session variable in expression: " + (*part)->ToString(),*this,Theta,Gamma,Omega);
  }

  return mySucc->TypeCheck(Theta,Gamma,Omega);
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
  MpsHostStatement *result = new MpsHostStatement(myHostParts, myExpParts, *newSucc, myTypes);
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

  MpsHostStatement *result = new MpsHostStatement(myHostParts, newExpParts, *newSucc, myTypes);
  // Clean up
  delete newSucc;
  DeleteVector(newExpParts);

  return result;
} // }}}
MpsHostStatement *MpsHostStatement::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->ReIndex(session,pid,maxpid);
  // myExpParts cannot use sessions!
  MpsHostStatement *result = new MpsHostStatement(myHostParts, myExpParts, *newSucc, myTypes);
  // Clean up
  delete newSucc;
  return result;
} // }}}
MpsHostStatement *MpsHostStatement::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->PSubst(var, exp, args, argpids, stateargs);
  MpsHostStatement *result = new MpsHostStatement(myHostParts, myExpParts, *newSucc, myTypes);
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
  MpsHostStatement *result = new MpsHostStatement(myHostParts, newExpParts, *newSucc, myTypes);
  // Clean up
  delete newSucc;
  DeleteVector(newExpParts);

  return result;
} // }}}
MpsHostStatement *MpsHostStatement::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  vector<MpsMsgType*> newTypes;
  for (vector<MpsMsgType*>::const_iterator tit=myTypes.begin(); tit!=myTypes.end(); ++tit)
    newTypes.push_back((*tit)->GSubst(source,dest,args));

  MpsHostStatement *result = new MpsHostStatement(myHostParts, myExpParts, *newSucc,newTypes);

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
  MpsHostStatement *result = new MpsHostStatement(myHostParts, myExpParts, *newSucc, newTypes);

  // Clean up
  delete newSucc;
  DeleteVector(newTypes);

  return result;
} // }}}
set<string> MpsHostStatement::FPV() const // {{{
{
  return mySucc->FPV();
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
  return new MpsHostStatement(myHostParts, myExpParts, *mySucc, myTypes);
} // }}}
bool MpsHostStatement::Terminated() const // {{{
{
  return false;
} // }}}
MpsHostStatement *MpsHostStatement::Simplify() const // {{{
{
  MpsTerm *newSucc = mySucc->Simplify();
  MpsHostStatement *result = new MpsHostStatement(myHostParts, myExpParts, *newSucc, myTypes);
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
  result << "\")" << endl
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
string MpsHostStatement::ToC() const // {{{
{
  stringstream prestmt;
  stringstream stmt;
  for (int i=0; i<myHostParts.size(); ++i)
  { stmt << myHostParts[i];
    if (i<myExpParts.size())
    { string newName = myExpParts[i]->ToC(prestmt, myTypes[i]->ToC());
      stmt << " " << newName << " ";
    }
  }
  return prestmt.str() + stmt.str() + mySucc->ToC();
} // }}}
string MpsHostStatement::ToCHeader() const // {{{
{
  return mySucc->ToCHeader();
} // }}}
MpsHostStatement *MpsHostStatement::RenameAll() const // {{{
{ MpsTerm *newSucc=mySucc->RenameAll();
  vector<MpsMsgType*> newTypes;
  for (vector<MpsMsgType*>::const_iterator tit=myTypes.begin(); tit!=myTypes.end(); ++tit)
    newTypes.push_back((*tit)->RenameAll());

  MpsHostStatement *result=new MpsHostStatement(myHostParts, myExpParts, *newSucc, newTypes);
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
  MpsTerm *parTmp = new MpsHostStatement(myHostParts, myExpParts, *parSucc, myTypes);
  delete parSucc;
  parTerm = pre->Append(*parTmp);
  delete pre;
  delete parTmp;
  // Make sequential term
  seqTerm = new MpsHostStatement(myHostParts, myExpParts, *seqSucc, myTypes);
  delete seqSucc;
  return opt1 || opt2;
} // }}}
MpsTerm *MpsHostStatement::Append(const MpsTerm &term) const // {{{
{ MpsTerm *newSucc=mySucc->Append(term);
  MpsTerm *result=new MpsHostStatement(myHostParts, myExpParts, *newSucc, myTypes);
  delete newSucc;
  return result;
} // }}}
MpsHostStatement *MpsHostStatement::CloseDefinitions() const // {{{
{
  MpsTerm *newSucc = mySucc->CloseDefinitions();
  MpsHostStatement *result= new MpsHostStatement(myHostParts, myExpParts, *newSucc, myTypes);
  delete newSucc;
  return result;
} // }}}
MpsHostStatement *MpsHostStatement::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ MpsTerm *newSucc=mySucc->ExtractDefinitions(env);
  MpsHostStatement *result=new MpsHostStatement(myHostParts, myExpParts, *newSucc, myTypes);
  delete newSucc;
  return result;
} // }}}
