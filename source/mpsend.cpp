#include<apims/mpsend.hpp>
#include <apims/common.hpp>

using namespace std;
using namespace apims;

MpsEnd::MpsEnd() // {{{
{
} // }}}
MpsEnd::~MpsEnd() // {{{
{
} // }}}
bool MpsEnd::TypeCheck(const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, const string &curPure) // Use rule Inact {{{
{
  // Check purity constraints
  if (pureStack.size()>0)
    return PrintTypeError("Implementation of pure participant " + int2string(pureStack.begin()->second) + "@" + pureStack.begin()->first + " must be immediately after its decleration",*this,Theta,Gamma,Omega);

  // Verify termination
  // Check that all sessions have been completed
  for (MpsMsgEnv::const_iterator var=Gamma.begin();var!=Gamma.end();++var)
  { const MpsDelegateMsgType *session=dynamic_cast<const MpsDelegateMsgType*>(var->second);
    if (session!=NULL &&
        !session->GetLocalType()->Equal(Theta,MpsLocalEndType()))
      return PrintTypeError((string)"Unfinished Session: " + var->first,*this,Theta,Gamma,Omega);
  }
  return true;
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
  return "end";
} // }}}
string MpsEnd::ToTex(int indent, int sw) const // {{{
{
  return ToTex_KW("end");
} // }}}
string MpsEnd::ToC() const // {{{
{
  return "  return new Cnt();\n";
} // }}}
string MpsEnd::ToCHeader() const // {{{
{
  return "";
} // }}}
MpsTerm *MpsEnd::RenameAll() const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsEnd::CloseDefinitions() const // {{{
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
