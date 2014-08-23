#include<apims/mpssync.hpp>
#include <apims/common.hpp>

using namespace std;
using namespace apims;

MpsSync::MpsSync(int maxpid, const std::string &session, const std::map<std::string, MpsTerm*> &branches, const std::map<std::string, MpsExp*> &assertions) // {{{
: mySession(session),
  myMaxpid(maxpid)
{
  myBranches.clear();
  for (map<string,MpsTerm*>::const_iterator it = branches.begin(); it != branches.end(); ++it)
    myBranches[it->first] = it->second->Copy();
  myAssertions.clear();
  for (map<string,MpsExp*>::const_iterator it=assertions.begin(); it!=assertions.end(); ++it)
    myAssertions[it->first] = it->second->Copy();
} // }}}
MpsSync::~MpsSync() // {{{
{
  DeleteMap(myBranches);
  DeleteMap(myAssertions);
} // }}}
bool MpsSync::TypeCheck(const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, bool reqPure) // Use rule Sync {{{
{
  // Check purity constraints
  if (pureStack.size()>0)
    return PrintTypeError("Implementation of pure participant " + int2string(pureStack.begin()->second) + "@" + pureStack.begin()->first + " must be immediately after its decleration",*this,Theta,Gamma,Omega);

  // Verify sync
  MpsMsgEnv::const_iterator var=Gamma.find(mySession);
  // Check that session exists
  if (var==Gamma.end())
    return PrintTypeError((string)"Synchonising on unknown session " + mySession,*this,Theta,Gamma,Omega);
  // Check if session type
  const MpsDelegateMsgType *msgType = dynamic_cast<const MpsDelegateMsgType*>(var->second);
  if (msgType==NULL)
    return PrintTypeError((string)"Synchronising on non-session type: " + mySession,*this,Theta,Gamma,Omega);

  // Check if unfolding is necessary
  const MpsLocalRecType *recType = dynamic_cast<const MpsLocalRecType*>(msgType->GetLocalType());
  if (recType!=NULL)
    return TypeCheckRec(Theta,Gamma, Omega, pureStack, reqPure, *this, var->first);
  const MpsLocalForallType *allType = dynamic_cast<const MpsLocalForallType*>(msgType->GetLocalType());
  if (allType!=NULL)
    return TypeCheckForall(Theta, Gamma, Omega, pureStack, reqPure, *this, var->first);

  // Check session has sync type
  const MpsLocalSyncType *syncType = dynamic_cast<const MpsLocalSyncType*>(msgType->GetLocalType());
  if (syncType==NULL)
    return PrintTypeError((string)"Synchronising on non-sync session: " + mySession,*this,Theta,Gamma,Omega);
  // Check maxpid
  if (myMaxpid != msgType->GetMaxpid())
    return PrintTypeError((string)"Synchronising with wrong participant count",*this,Theta,Gamma,Omega);
  // Check if mandatory labels are accepted
  const map<string,MpsLocalType*> &branches=syncType->GetBranches();
  const map<string,MpsExp*> &assertions=syncType->GetAssertions();
  vector<const MpsExp*> hyps;
  hyps.push_back(&Theta);
  MpsExp *mandatoryOr=new MpsBoolVal(false); // FIXME: This is necessary because Global Type Validity Check is Missing
  for (map<string,MpsLocalType*>::const_iterator branch=branches.begin();branch!=branches.end();++branch)
  {
    if (branch->first[0]=='^') // Mandatory branch
    {
      map<string,MpsTerm*>::const_iterator myBranch=myBranches.find(branch->first);
      map<string,MpsExp*>::const_iterator assertion=assertions.find(branch->first);
      if (assertion==assertions.end())
        return PrintTypeError((string)"Synchronisation type has no assertion for branch: " + branch->first,*this,Theta,Gamma,Omega);
      MpsExp *tmpOr = new MpsBinOpExp("or",*mandatoryOr,*assertion->second,MpsBoolMsgType(),MpsBoolMsgType());
      delete mandatoryOr;
      mandatoryOr=tmpOr;
      // Check Label Inclusion
      if (myBranch == myBranches.end())
      { MpsExp *notAssertion = new MpsUnOpExp("not",*assertion->second); // Do not require inactive branches
        bool inactive = notAssertion->ValidExp(hyps);
        delete notAssertion;
        if (not inactive)
          return PrintTypeError((string)"Synchronisation missing mandatory branch: " + branch->first,*this,Theta,Gamma,Omega);
      }
      else
      {
        // Check Assertion Implication
        map<string,MpsExp*>::const_iterator myAssertion=myAssertions.find(branch->first);
        if (myAssertion==myAssertions.end())
          return PrintTypeError((string)"Synchronisation process has no assertion for branch: " + branch->first,*this,Theta,Gamma,Omega);
        MpsExp *notAssertion = new MpsUnOpExp("not",*assertion->second);
        MpsExp *implication = new MpsBinOpExp("or",*notAssertion,*myAssertion->second,MpsBoolMsgType(),MpsBoolMsgType());
        delete notAssertion;
        bool checkImplication=implication->ValidExp(hyps);
        delete implication;
        if (not checkImplication)
          return PrintTypeError((string)"Synchronisation may not accept mandatory branch: " + branch->first,*this,Theta,Gamma,Omega);
      }
    }
  }
  bool checkMandatory=mandatoryOr->ValidExp(hyps);
  delete mandatoryOr;
  if (not checkMandatory)
    return PrintTypeError((string)"Synchronisation may have no mandatory branches: " + mySession,*this,Theta,Gamma,Omega);

  // Check typing of all branches in the process
  for (map<string,MpsTerm*>::const_iterator branch=myBranches.begin();branch!=myBranches.end();++branch)
  {
    // Check Label Inclusion
    map<string,MpsLocalType*>::const_iterator type=branches.find(branch->first);
    if (type==branches.end())
      return PrintTypeError((string)"Synchronisation accepts untyped label: " + branch->first,*this,Theta,Gamma,Omega);
    // TypeCheck Assertion
    map<string,MpsExp*>::const_iterator myAssertion=myAssertions.find(branch->first);
    if (myAssertion==myAssertions.end())
      return PrintTypeError((string)"Synchronisation process has no assertion for branch: " + branch->first,*this,Theta,Gamma,Omega);
    MpsMsgType *assertionType=myAssertion->second->TypeCheck(Gamma);
    bool checkAssertionType = dynamic_cast<MpsBoolMsgType*>(assertionType)!=NULL;
    delete assertionType;
    if (not checkAssertionType)
      return PrintTypeError((string)"Synchronisation has untyped assertion for branch: " + branch->first,*this,Theta,Gamma,Omega);
    // Check Assertion Implication
    map<string,MpsExp*>::const_iterator assertion=assertions.find(branch->first);
    if (assertion==assertions.end())
        return PrintTypeError((string)"Synchronisation type has no assertion for branch: " + branch->first,*this,Theta,Gamma,Omega);
    MpsExp *notAssertion = new MpsUnOpExp("not",*myAssertion->second);
    MpsExp *implication = new MpsBinOpExp("or",*notAssertion,*assertion->second,MpsBoolMsgType(),MpsBoolMsgType());
    delete notAssertion;
    bool checkImplication = implication->ValidExp(hyps);
    delete implication;
    if (not checkImplication)
        return PrintTypeError((string)"Synchronisation may accept inactive branch: " + branch->first,*this,Theta,Gamma,Omega);
    // Make new Gamma
    MpsMsgEnv newGamma = Gamma;
    newGamma[mySession] = new MpsDelegateLocalMsgType(*type->second,msgType->GetPid(), msgType->GetParticipants());
    // Make new Theta
    MpsExp *newTheta=new MpsBinOpExp("and",Theta,*myAssertion->second,MpsBoolMsgType(),MpsBoolMsgType());
    // Check Branch
    bool checkBranch=branch->second->TypeCheck(*newTheta,newGamma,Omega,pureStack,reqPure);
    delete newTheta;
    if (not checkBranch)
      return false;
  }
  // All checks passed
  return true;
} // }}}
MpsTerm *MpsSync::ApplySync(const std::vector<std::string> &paths, const std::string &label) const // {{{
{ if (paths.size()==0)
    return Copy();
  if (paths.size()>1)
    return Error((string)"Applying Sync with multiple paths on "+ToString());

  map<string,MpsTerm*>::const_iterator branch=myBranches.find(label);
  if (branch==myBranches.end())
    return Error((string)"Applying Sync with unknown label: "+label+" on "+ToString());
  else
    return branch->second->Copy();
} // }}}
bool MpsSync::SubSteps(vector<MpsStep> &dest) // {{{
{
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    MpsExp *include=myAssertions[it->first]->Eval(); //Evaluate assertion
    if (typeid(*include) != typeid(MpsBoolVal))
    { delete include;
      continue;
    }
    MpsBoolVal *boolval = (MpsBoolVal*)include;
    if (boolval->GetValue()) // If assertion is true, then include branch
    {
      MpsEvent event;
      event.myType = sync;
      event.mySession = mySession;
      event.myLabel = it->first;
      event.myMaxpid = myMaxpid;
      vector<string> paths;
      paths.push_back("");
      dest.push_back(MpsStep(event,paths));
    }
    delete include;
  }
  return false;
} // }}}
MpsTerm *MpsSync::PRename(const string &src, const string &dst) const // {{{
{
  map<string, MpsTerm*> newBranches;
  newBranches.clear();
  // PReplace each branch
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    MpsTerm *newBranch = it->second->PRename(src,dst);
    newBranches[it->first] = newBranch;
  }
  MpsTerm *result = new MpsSync(myMaxpid, mySession, newBranches, myAssertions);
  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsTerm *MpsSync::ERename(const string &src, const string &dst) const // {{{
{
  string newSession = mySession==src?dst:mySession;
  map<string, MpsTerm*> newBranches;
  // ERename each branch
  for (map<string,MpsTerm*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    MpsTerm *newBranch = it->second->ERename(src,dst);
    newBranches[it->first] = newBranch;
  }

  map<string, MpsExp*> newAssertions;
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin(); it!=myAssertions.end(); ++it)
  {
    MpsExp *newAssertion = it->second->Rename(src,dst);
    newAssertions[it->first] = newAssertion;
  }

  MpsTerm *result = new MpsSync(myMaxpid, newSession, newBranches, newAssertions);

  // Clean up
  DeleteMap(newBranches);
  DeleteMap(newAssertions);

  return result;
} // }}}
MpsTerm *MpsSync::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  map<string, MpsTerm*> newBranches;
  // ReIndex each branch
  for (map<string,MpsTerm*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    MpsTerm *newBranch = it->second->ReIndex(session,pid,maxpid);
    newBranches[it->first] = newBranch;
  }

  MpsTerm *result = new MpsSync(myMaxpid, mySession, newBranches, myAssertions);

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsTerm *MpsSync::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
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
  MpsTerm *result = new MpsSync(myMaxpid, mySession, newBranches, myAssertions);
  // Clean up
  // FIXME: Use DeleteVector(newBranches);
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second;
    newBranches.erase(newBranches.begin());
  }

  return result;
} // }}}
MpsTerm *MpsSync::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  string newSession = stringESubst(mySession,source,dest);

  // ESubst each branch
  map<string, MpsTerm*> newBranches;
  for (map<string,MpsTerm*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    MpsTerm *newBranch = it->second->ESubst(source,dest);
    newBranches[it->first] = newBranch;
  }

  // Subst each assertion
  map<string, MpsExp*> newAssertions;
  for (map<string,MpsExp*>::const_iterator it=myAssertions.begin(); it!=myAssertions.end(); ++it)
  {
    MpsExp *newAssertion = it->second->Subst(source,dest);
    newAssertions[it->first] = newAssertion;
  }
  MpsTerm *result = new MpsSync(myMaxpid, newSession, newBranches, newAssertions);

  // Clean up
  DeleteMap(newBranches);
  DeleteMap(newAssertions);

  return result;
} // }}}
MpsTerm *MpsSync::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  // GSubst each branch
  map<string, MpsTerm*> newBranches;
  for (map<string,MpsTerm*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    MpsTerm *newBranch = it->second->GSubst(source,dest,args);
    newBranches[it->first] = newBranch;
  }

  MpsTerm *result = new MpsSync(myMaxpid, mySession, newBranches, myAssertions);

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
MpsTerm *MpsSync::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  // LSubst each branch
  map<string, MpsTerm*> newBranches;
  for (map<string,MpsTerm*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    MpsTerm *newBranch = it->second->LSubst(source,dest,args);
    newBranches[it->first] = newBranch;
  }

  MpsTerm *result = new MpsSync(myMaxpid, mySession, newBranches, myAssertions);

  // Clean up
  DeleteMap(newBranches);

  return result;
} // }}}
set<string> MpsSync::FPV() const // {{{
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
set<string> MpsSync::FEV() const // {{{
{
  set<string> result;
  result.clear();
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    set<string> sub = it->second->FEV();
    result.insert(sub.begin(), sub.end());
  }
  result.insert(mySession);
  return result;
} // }}}
MpsTerm *MpsSync::Copy() const // {{{
{
  return new MpsSync(myMaxpid, mySession,myBranches, myAssertions);
} // }}}
bool MpsSync::Terminated() const // {{{
{
  return false;
} // }}}
MpsTerm *MpsSync::Simplify() const // {{{
{
  map<string,MpsTerm*> newBranches;
  newBranches.clear();
  for (map<string,MpsTerm*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
    newBranches[it->first] = it->second->Simplify();
  MpsTerm *result = new MpsSync(myMaxpid,mySession,newBranches,myAssertions);
  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second;
    newBranches.erase(newBranches.begin());
  }
  return result;
} // }}}
string MpsSync::ToString(string indent) const // {{{
{
  string newIndent = indent + "    ";
  string result = (string)"sync(" + int2string(myMaxpid) + "," + mySession + ")\n" + indent + "{ ";
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    if (it != myBranches.begin())
      result += ",\n" + indent + "  ";
    result += it->first;
    map<string,MpsExp*>::const_iterator ass=myAssertions.find(it->first);
    if (ass != myAssertions.end())
      result += "[[" + ass->second->ToString() + "]]";
    result += (string)":\n" + newIndent + it->second->ToString(newIndent);
  }
  result += "\n" + indent + "}";
  return result;
} // }}}
string MpsSync::ToTex(int indent, int sw) const // {{{
{
  int newIndent = indent + 4;
  string result = ToTex_KW("sync") + "(" + ToTex_PP(myMaxpid) + "," + ToTex_Session(mySession) + ")\\newline\n"
                + ToTex_Hspace(indent,sw) + "\\{ ";
  for (map<string,MpsTerm*>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    if (it != myBranches.begin())
      result += ",\\newline\n"
              + ToTex_Hspace(indent+2,sw);
    result += ToTex_Label(it->first);
    map<string,MpsExp*>::const_iterator ass=myAssertions.find(it->first);
    if (ass != myAssertions.end() && ass->second->ToString()!="true")
      result += "$\\llbracket$" + ass->second->ToString() + "$\\rrbracket$";
    result += (string)":\\newline\n"
            + ToTex_Hspace(newIndent,sw) + it->second->ToTex(newIndent,sw);
  }
  result += "\\newline\n"
          + ToTex_Hspace(indent,sw) + "\\}";
  return result;
} // }}}
string MpsSync::ToC() const // {{{
{
  throw (string)"MpsSync::ToC(): Symmetric synchronization is not supported yet!";
} // }}}
string MpsSync::ToCHeader() const // {{{
{
  throw (string)"MpsSync::ToC(): Symmetric synchronization is not supported yet!";
} // }}}
MpsTerm *MpsSync::RenameAll() const // {{{
{ map<string,MpsTerm*> newBranches;
  for (map<string,MpsTerm*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
    newBranches[it->first]=it->second->RenameAll();

  MpsTerm *result=new MpsSync(myMaxpid,mySession,newBranches,myAssertions);

  DeleteMap(newBranches);

  return result;
} // }}}
bool MpsSync::Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const // {{{
{ map<string,MpsTerm*> newBranches;
  for (map<string,MpsTerm*>::const_iterator branch=myBranches.begin(); branch!=myBranches.end(); ++branch)
    newBranches[branch->first]=branch->second->Parallelize();
  seqTerm = new MpsSync(myMaxpid, mySession,newBranches, myAssertions);
  parTerm = receives.Append(*seqTerm);
  DeleteMap(newBranches);
  return false; // All optimizations are guarded
} // }}}
MpsTerm *MpsSync::Append(const MpsTerm &term) const // {{{
{ map<string,MpsTerm*> newBranches;
  for (map<string,MpsTerm*>::const_iterator branch=myBranches.begin(); branch!=myBranches.end(); ++branch)
    newBranches[branch->first]=branch->second->Append(term);
  MpsTerm *result = new MpsSync(myMaxpid, mySession,newBranches, myAssertions);
  DeleteMap(newBranches);
  return result;
} // }}}
MpsTerm *MpsSync::CloseDefinitions() const // {{{
{
  map<string,MpsTerm*> newBranches;
  for (map<string,MpsTerm*>::const_iterator br=myBranches.begin(); br!=myBranches.end(); ++br)
    newBranches[br->first]=br->second->CloseDefinitions();

  MpsTerm *result=new MpsSync(myMaxpid, mySession, newBranches, myAssertions);

  DeleteMap(newBranches);

  return result;
} // }}}
MpsTerm *MpsSync::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ map<string,MpsTerm*> newBranches;
  for (map<string,MpsTerm*>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
    newBranches[it->first]=it->second->ExtractDefinitions(env);

  MpsTerm *result=new MpsSync(myMaxpid,mySession,newBranches,myAssertions);

  DeleteMap(newBranches);

  return result;
} // }}}
