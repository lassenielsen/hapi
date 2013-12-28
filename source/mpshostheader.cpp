#include<apims/mpshostheader.hpp>
#include "common.cpp"

using namespace std;
using namespace apims;

MpsHostHeader::MpsHostHeader(const string header, const MpsTerm &succ) // {{{
: myHeader(header)
{
  mySucc = succ.Copy();
} // }}}
MpsHostHeader::~MpsHostHeader() // {{{
{
  delete mySucc;
} // }}}
bool MpsHostHeader::TypeCheck(const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega) // Use rule Nres {{{
{
  return mySucc->TypeCheck(Theta,Gamma,Omega);
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
MpsHostHeader *MpsHostHeader::RenameAll() const // {{{
{ MpsTerm *newSucc=mySucc->RenameAll();
  MpsHostHeader *result=new MpsHostHeader(myHeader, *newSucc);
  // Clean up
  delete newSucc;
  return result;
} // }}}
MpsHostHeader *MpsHostHeader::CloseDefinitions() const // {{{
{
  MpsTerm *newSucc = mySucc->CloseDefinitions();
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
