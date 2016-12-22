#include<hapi/mpsend.hpp>
#include <hapi/common.hpp>

using namespace std;
using namespace hapi;

MpsEnd::MpsEnd() // {{{
{
} // }}}
MpsEnd::~MpsEnd() // {{{
{
} // }}}
void *MpsEnd::TDCompileMain(tdc_pre pre, tdc_post wrap, tdc_error wrap_err, const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, const string &curPure, PureState pureState, bool checkPure) // Use rule Inact {{{
{ map<string,void*> children;
  // Check purity constraints
  if (checkPure)
	{ if (pureStack.size()>0)
      return wrap_err(this,PrintTypeError("Implementation of pure participant " + int2string(pureStack.begin()->second) + "@" + pureStack.begin()->first + " must immediately follow its decleration",*this,Theta,Gamma,Omega),children);

    if (pureState!=CPS_IMPURE && pureState!=CPS_PURE)
      return wrap_err(this,PrintTypeError("Error in implementation of pure participant " + curPure + ". Pure implementations must conform with the structure \n     *   local X()\n	   *   ( global s=new ch(p of n);\n		 *     X();\n		 *     |\n		 *     P\n		 *   )\n		 *   local StartX(Int i)\n		 *   ( if i<=0\n		 *     then X();\n		 *     else X(); | StartX(i-1);\n		 *   )\n		 *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);
  }

  // Verify termination
  // Check that all sessions have been completed
  for (MpsMsgEnv::const_iterator var=Gamma.begin();var!=Gamma.end();++var)
  { const MpsDelegateMsgType *session=dynamic_cast<const MpsDelegateMsgType*>(var->second);
    if (session!=NULL &&
        !session->GetLocalType()->Equal(Theta,MpsLocalEndType()))
      return wrap_err(this,PrintTypeError((string)"Unfinished Session: " + var->first,*this,Theta,Gamma,Omega),children);
  }

  // Wrap result
  return wrap(this,Theta,Gamma,Omega,pureStack,curPure,pureState,checkPure,children);
} // }}}
bool MpsEnd::SubSteps(vector<MpsStep> &dest) // {{{
{
  return false; // No steps
} // }}}
MpsTerm *MpsEnd::PRename(const string &src, const string &dst) const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsEnd::ERename(const string &src, const string &dst) const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsEnd::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsEnd::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsEnd::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsEnd::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsEnd::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  return Copy();
} // }}}
set<string> MpsEnd::FPV() const // {{{
{
  set<string> result;
  result.clear();
  return result;
} // }}}
set<string> MpsEnd::FEV() const // {{{
{
  set<string> result;
  result.clear();
  return result;
} // }}}
MpsTerm *MpsEnd::Copy() const // {{{
{
  return new MpsEnd();
} // }}}
bool MpsEnd::Terminated() const // {{{
{
  return true;
} // }}}
MpsTerm *MpsEnd::Simplify() const // {{{
{
  return Copy();
} // }}}
string MpsEnd::ToString(string indent) const // {{{
{
  return "";
} // }}}
string MpsEnd::ToTex(int indent, int sw) const // {{{
{
  return ToTex_KW("end");
} // }}}
string MpsEnd::ToC() const // {{{
{
  return "  delete _state;\n  return NULL;\n";
} // }}}
string MpsEnd::ToCHeader() const // {{{
{
  return "";
} // }}}
MpsTerm *MpsEnd::FlattenFork(bool normLhs, bool normRhs, bool pureMode) const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsEnd::RenameAll() const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsEnd::CloseDefsPre(const MpsMsgEnv &Gamma)
{
  return this;
} // }}}
MpsTerm *MpsEnd::CopyWrapper(std::map<std::string,void*> &children) const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsEnd::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{
  return Copy();
} // }}}
bool MpsEnd::Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const // {{{
{ parTerm = receives.Copy();
  seqTerm=Copy();
  return dynamic_cast<const MpsEnd*>(&receives)==NULL;
} // }}}
MpsTerm *MpsEnd::Append(const MpsTerm &term) const // {{{
{ return term.Copy();
} // }}}
