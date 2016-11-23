#include<hapi/mpshostheader.hpp>
#include<hapi/mpsend.hpp>
#include <hapi/common.hpp>

using namespace std;
using namespace hapi;

MpsHostHeader::MpsHostHeader(const string header, const MpsTerm &succ) // {{{
: myHeader(header)
{
  mySucc = succ.Copy();
} // }}}
MpsHostHeader::~MpsHostHeader() // {{{
{
  delete mySucc;
} // }}}
void *MpsHostHeader::TDCompile(tdc_wrapper wrap, tdc_wraperr wrap_err, const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, const string &curPure, PureState pureState, bool checkPure) // Use rule Nres {{{
{ map<string,void*> children;
 
  if (checkPure)
	{ // Check purity constraints
    if (pureState!=CPS_IMPURE && pureState!=CPS_PURE)
      return wrap_err(this,PrintTypeError("Error in implementation of pure participant " + curPure + ". Pure implementations must conform with the structure \n     *   local X()\n	   *   ( global s=new ch(p of n);\n		 *     X();\n		 *     |\n		 *     P\n		 *   )\n		 *   local StartX(Int i)\n		 *   ( if i<=0\n		 *     then X();\n		 *     else X(); | StartX(i-1);\n		 *   )\n		 *   StartX( E ); |" ,*this,Theta,Gamma,Omega));

    if (pureState==CPS_PURE)
      return wrap_err(this,PrintTypeError("HOST statements not allowed in pure context",*this,Theta,Gamma,Omega));
  }

  // Verify hostheader
  children["succ"] = mySucc->TDCompile(wrap,wrap_err,Theta,Gamma,Omega,pureStack,curPure, pureState, checkPure);
  // Wrap result
  return wrap(this,Theta,Gamma,Omega,pureStack,curPure,pureState,checkPure,children);
} // }}}
MpsTerm *MpsHostHeader::ApplyOther(const std::string &path) const // {{{
{ if (path.size()!=0)
    return Error((string)"Applying Other with nonempty path on "+ToString());
  MpsTerm *result = mySucc->Copy();
  return result;
} // }}}
bool MpsHostHeader::SubSteps(vector<MpsStep> &dest) // {{{
{
  MpsEvent event;
  event.myType = tau;
  event.mySubType = host;
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event, paths));
  return false;
} // }}}
MpsHostHeader *MpsHostHeader::PRename(const string &src, const string &dst) const // {{{
{
  MpsTerm *newSucc = mySucc->PRename(src,dst);
  MpsHostHeader *result = new MpsHostHeader(myHeader, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsHostHeader *MpsHostHeader::ERename(const string &src, const string &dst) const // {{{
{
  // Rename in succ
  MpsTerm *newSucc = mySucc->ERename(src,dst);

  MpsHostHeader *result = new MpsHostHeader(myHeader, *newSucc);

  // Clean up
  delete newSucc;

  return result;
} // }}}
MpsHostHeader *MpsHostHeader::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  MpsTerm *newSucc = mySucc->ReIndex(session,pid,maxpid);
  MpsHostHeader *result = new MpsHostHeader(myHeader, *newSucc);
  // Clean up
  delete newSucc;
  return result;
} // }}}
MpsHostHeader *MpsHostHeader::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
{
  MpsTerm *newSucc = mySucc->PSubst(var, exp, args, argpids, stateargs);
  MpsHostHeader *result = new MpsHostHeader(myHeader, *newSucc);
  // Clean up
  delete newSucc;
  return result;
} // }}}
MpsHostHeader *MpsHostHeader::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  MpsTerm *newSucc = mySucc->ESubst(source,dest);
  MpsHostHeader *result = new MpsHostHeader(myHeader, *newSucc);
  // Clean up
  delete newSucc;

  return result;
} // }}}
MpsHostHeader *MpsHostHeader::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  MpsHostHeader *result = new MpsHostHeader(myHeader, *newSucc);
  // Clean up
  delete newSucc;
  return result;
} // }}}
MpsHostHeader *MpsHostHeader::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->LSubst(source,dest,args);
  MpsHostHeader *result = new MpsHostHeader(myHeader, *newSucc);
  // Clean up
  delete newSucc;
  return result;
} // }}}
set<string> MpsHostHeader::FPV() const // {{{
{
  return mySucc->FPV();
} // }}}
set<string> MpsHostHeader::FEV() const // {{{
{
  return mySucc->FEV();
} // }}}
MpsHostHeader *MpsHostHeader::Copy() const // {{{
{
  return new MpsHostHeader(myHeader, *mySucc);
} // }}}
bool MpsHostHeader::Terminated() const // {{{
{
  return false;
} // }}}
MpsHostHeader *MpsHostHeader::Simplify() const // {{{
{
  MpsTerm *newSucc = mySucc->Simplify();
  MpsHostHeader *result = new MpsHostHeader(myHeader, *newSucc);
  // Clean up
  delete newSucc;
  return result;
} // }}}
string MpsHostHeader::ToString(string indent) const // {{{
{
  stringstream result;
  result << "HOSTHEADER(\""
         << myHeader
         << "\");" << endl
         << indent << mySucc->ToString(indent);
  return result.str();
} // }}}
string MpsHostHeader::ToTex(int indent, int sw) const // {{{
{
  stringstream result;
  result << "HOSTHEADER(\""
         << myHeader
         << "\");" << endl
         << ToTex_Hspace(indent,sw) << mySucc->ToTex(indent,sw);
  return result.str();
} // }}}
string MpsHostHeader::ToC() const // {{{
{
  return mySucc->ToC();
} // }}}
string MpsHostHeader::ToCHeader() const // {{{
{
  return unwrap_string(myHeader) + "\n" + mySucc->ToCHeader();
} // }}}
MpsTerm *MpsHostHeader::FlattenFork(bool normLhs, bool normRhs, bool pureMode) const // {{{
{
  MpsTerm *newSucc = mySucc->FlattenFork(normLhs,normRhs,pureMode);
  MpsHostHeader *result= new MpsHostHeader(myHeader, *newSucc);
  // Clean up
  delete newSucc;
  return result;
} // }}}
MpsHostHeader *MpsHostHeader::RenameAll() const // {{{
{ MpsTerm *newSucc=mySucc->RenameAll();
  MpsHostHeader *result=new MpsHostHeader(myHeader, *newSucc);
  // Clean up
  delete newSucc;
  return result;
} // }}}
bool MpsHostHeader::Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const // {{{
{ // Split receives using the used vars
  MpsTerm *pre;
  MpsTerm *post;
  receives.Split(set<string>(),pre,post);
  bool opt1=dynamic_cast<const MpsEnd*>(post)==NULL;
  // Parallelize succ with post receives
  MpsTerm *seqSucc;
  MpsTerm *parSucc;
  bool opt2=mySucc->Parallelize(*post,seqSucc,parSucc);
  delete post;
  // Make parallelized term
  MpsTerm *parTmp = new MpsHostHeader(myHeader, *parSucc);
  delete parSucc;
  parTerm = pre->Append(*parTmp);
  delete pre;
  delete parTmp;
  // Make sequential term
  seqTerm = new MpsHostHeader(myHeader, *seqSucc);
  delete seqSucc;
  return opt1 || opt2;
} // }}}
MpsTerm *MpsHostHeader::Append(const MpsTerm &term) const // {{{
{ MpsTerm *newSucc=mySucc->Append(term);
  MpsTerm *result=new MpsHostHeader(myHeader, *newSucc);
  delete newSucc;
  return result;
} // }}}
MpsHostHeader *MpsHostHeader::CloseDefinitions(const MpsMsgEnv &Gamma) const // {{{
{
  MpsTerm *newSucc = mySucc->CloseDefinitions(Gamma);
  MpsHostHeader *result= new MpsHostHeader(myHeader, *newSucc);
  // Clean up
  delete newSucc;
  return result;
} // }}}
MpsHostHeader *MpsHostHeader::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ MpsTerm *newSucc=mySucc->ExtractDefinitions(env);
  MpsHostHeader *result=new MpsHostHeader(myHeader, *newSucc);
  // Clean up
  delete newSucc;
  return result;
} // }}}
