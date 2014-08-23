#include<apims/mpsguisync.hpp>
#include <apims/mpsgui.hpp>
#include <apims/common.hpp>

using namespace std;
using namespace apims;

MpsGuiSync::MpsGuiSync(int maxpid, const std::string &session, int pid, const std::map<std::string, inputbranch> &branches) // {{{
: mySession(session),
  myMaxpid(maxpid),
  myPid(pid)
{
  myBranches.clear();
  for (map<string,inputbranch>::const_iterator it=branches.begin(); it!=branches.end(); ++it)
  {
    inputbranch branch;
    branch.term = it->second.term->Copy();
    branch.assertion = it->second.assertion->Copy();
    branch.names = it->second.names;
    branch.args = it->second.args;
    branch.types.clear();
    for (vector<MpsMsgType*>::const_iterator type=it->second.types.begin(); type!=it->second.types.end(); ++type)
      branch.types.push_back((*type)->Copy());
    branch.values.clear();
    for (vector<MpsExp*>::const_iterator value=it->second.values.begin(); value!=it->second.values.end(); ++value)
      branch.values.push_back((*value)->Copy());
    myBranches[it->first] = branch;
  }
} // }}}
MpsGuiSync::~MpsGuiSync() // {{{
{
  while (myBranches.size()>0)
  {
    delete myBranches.begin()->second.term;
    delete myBranches.begin()->second.assertion;
    while (myBranches.begin()->second.types.size()>0)
    {
      delete *myBranches.begin()->second.types.begin();
      myBranches.begin()->second.types.erase(myBranches.begin()->second.types.begin());
    }
    while (myBranches.begin()->second.values.size()>0)
    {
      delete *myBranches.begin()->second.values.begin();
      myBranches.begin()->second.values.erase(myBranches.begin()->second.values.begin());
    }
    myBranches.erase(myBranches.begin());
  }
} // }}}
bool MpsGuiSync::TypeCheck(const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, bool reqPure) // * Use rule Sync (extended) {{{
{
  // Check purity constraints
  if (pureStack.size()>0)
    return PrintTypeError("Implementation of pure participant " + int2string(pureStack.begin()->second) + "@" + pureStack.begin()->first + " must be immediately after its decleration",*this,Theta,Gamma,Omega);

  // Verify guisync
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
  if (myPid != msgType->GetPid())
    return PrintTypeError((string)"Synchronising with wrong participant ID",*this,Theta,Gamma,Omega);
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
      map<string,inputbranch>::const_iterator myBranch=myBranches.find(branch->first);
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
        MpsExp *notAssertion = new MpsUnOpExp("not",*assertion->second);
        MpsExp *implication = new MpsBinOpExp("or",*notAssertion,*myBranch->second.assertion,MpsBoolMsgType(),MpsBoolMsgType());
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
  for (map<string,inputbranch>::const_iterator myBranch=myBranches.begin();myBranch!=myBranches.end();++myBranch)
  {
    // Typecheck arguments
    for (int brancharg=0; brancharg<myBranch->second.args.size(); ++brancharg)
    {
      MpsMsgType *branchargtype = myBranch->second.values[brancharg]->TypeCheck(Gamma);
      bool branchargtypematch = branchargtype->Equal(Theta,*myBranch->second.types[brancharg]);
      delete branchargtype;
      if (!branchargtypematch)
        return PrintTypeError((string)"Ill typed argument: " + myBranch->second.args[brancharg] + " in branch: " + myBranch->first,*this,Theta,Gamma,Omega);
    }
    // Check Label Inclusion
    map<string,MpsLocalType*>::const_iterator type=branches.find(myBranch->first);
    if (type==branches.end())
      return PrintTypeError((string)"Synchronisation accepts untyped label: " + myBranch->first,*this,Theta,Gamma,Omega);
    // TypeCheck Assertion
    MpsMsgType *assertionType=myBranch->second.assertion->TypeCheck(Gamma);
    bool checkAssertionType = dynamic_cast<MpsBoolMsgType*>(assertionType)!=NULL;
    delete assertionType;
    if (not checkAssertionType)
      return PrintTypeError((string)"Synchronisation has untyped assertion for branch: " + myBranch->first,*this,Theta,Gamma,Omega);
    // Check Assertion Implication
    map<string,MpsExp*>::const_iterator assertion=assertions.find(myBranch->first);
    if (assertion==assertions.end())
        return PrintTypeError((string)"Synchronisation type has no assertion for branch: " + myBranch->first,*this,Theta,Gamma,Omega);
    MpsExp *notAssertion = new MpsUnOpExp("not",*myBranch->second.assertion);
    MpsExp *implication = new MpsBinOpExp("or",*notAssertion,*assertion->second,MpsBoolMsgType(),MpsBoolMsgType());
    delete notAssertion;
    bool checkImplication = implication->ValidExp(hyps);
    delete implication;
    if (not checkImplication)
        return PrintTypeError((string)"Synchronisation may accept inactive branch: " + myBranch->first,*this,Theta,Gamma,Omega);
    // Make new Gamma
    // Prepare renaming
    map<string,string> renaming;
    for (int brancharg=0; brancharg<myBranch->second.args.size(); ++brancharg)
    { string name=myBranch->second.args[brancharg];
      renaming[name]=MpsExp::NewVar(name);
    }
    MpsMsgEnv newGamma;
    for (MpsMsgEnv::const_iterator gamma=Gamma.begin(); gamma!=Gamma.end(); ++gamma)
    { const MpsDelegateMsgType *gammaDel=dynamic_cast<const MpsDelegateMsgType*>(gamma->second);
      if (gammaDel!=NULL) // gamma is a session
      { MpsLocalType *tmp1=NULL;
        if (gamma->first==mySession) // Use branch type for mySession
          tmp1 = type->second->Copy();
        else
          tmp1 = gammaDel->GetLocalType()->Copy();
        for (map<string,string>::const_iterator tr=renaming.begin(); tr!=renaming.end(); ++tr)
        { MpsLocalType *tmp2=tmp1->ERename(tr->first,tr->second);
          delete tmp1;
          tmp1=tmp2;
        }
        newGamma[gamma->first]=new MpsDelegateLocalMsgType(*tmp1,gammaDel->GetPid(),gammaDel->GetParticipants());
      }
      else
      { newGamma[gamma->first]=gamma->second; // COPY?
      }
    }
    // Check argument cont
    if (myBranch->second.args.size() != myBranch->second.types.size() ||
        myBranch->second.args.size() != myBranch->second.names.size())
      return PrintTypeError((string)"Number of arguments, types and names inconsistent in branch: " + myBranch->first,*this,Theta,Gamma,Omega);
    // Add argument types
    for (int i=0; i<myBranch->second.args.size(); ++i)
      newGamma[myBranch->second.args[i] ] = myBranch->second.types[i]; // Only simple types
    // Make new Theta
    MpsExp *newTheta=new MpsBinOpExp("and",Theta,*myBranch->second.assertion,MpsBoolMsgType(),MpsBoolMsgType());
    // Rename args in Theta
    for (map<string,string>::const_iterator tr=renaming.begin(); tr!=renaming.end(); ++tr)
    { MpsExp *tmpTheta=newTheta->Rename(tr->first,tr->second);
      delete newTheta;
      newTheta=tmpTheta;
    }
    // Check Branch
    bool checkBranch=myBranch->second.term->TypeCheck(*newTheta,newGamma,Omega,pureStack,reqPure);
    delete newTheta;
    while (newGamma.size()>0)
    { const MpsDelegateMsgType *gammaDel=dynamic_cast<const MpsDelegateMsgType*>(newGamma.begin()->second);
      if (gammaDel!=NULL)
        delete gammaDel;
      newGamma.erase(newGamma.begin());
    }
    if (not checkBranch)
      return false;
  }
  // All checks passed
  return true;
} // }}}
MpsTerm *MpsGuiSync::ApplySync(const std::vector<std::string> &paths, const std::string &label) const // {{{
{ if (paths.size()==0)
    return Copy();
  if (paths.size()>1)
    return Error((string)"Applying Sync with multiple paths on "+ToString());

  map<string,inputbranch>::const_iterator branch=myBranches.find(label);
  if (branch==myBranches.end())
    return Error((string)"Applying Sync with unknown label: "+label+" on "+ToString());

  vector<mpsgui::Choice> choices=mpsgui::GetChoices(mySession,myPid);
  for (vector<mpsgui::Choice>::const_iterator choice=choices.begin(); choice!=choices.end(); ++choice)
  { if (choice->name==label)
    { if (choice->accept)
      { MpsTerm *result = branch->second.term->Copy();
        for (vector<mpsgui::ChoiceArg>::const_iterator arg=choice->args.begin(); arg!=choice->args.end(); ++arg)
        { MpsTerm *tmp=result->ESubst(arg->name,*(arg->value));
          delete result;
          result=tmp;
        }
        return result;
      }
      else
        return Error((string)"Applying Sync with rejected label: "+label+" on "+ToString());
    }
  }
  return Error((string)"Applying Sync with inactive label: "+label+" on "+ToString());
} // }}}
bool MpsGuiSync::SubSteps(vector<MpsStep> &dest) // {{{
{
  // Get Choices from GUI
  vector<mpsgui::Choice> choices = mpsgui::GetChoices(mySession, myPid);
  if (choices.size()==0) // If choices not registered
  { // Register choices
    // Consider each choice
    for (map<string,inputbranch>::const_iterator choice=myBranches.begin(); choice!=myBranches.end(); ++choice)
    { // Check if choice is active
      MpsExp *accept=choice->second.assertion->Eval();
      if (typeid(*accept)==typeid(MpsBoolVal) && ((MpsBoolVal*)accept)->GetValue())
      { // Add choice
        mpsgui::Choice newChoice;
        newChoice.name = choice->first;
        for (int i=0; i<choice->second.names.size(); ++i)
        {
          MpsExp *value = choice->second.values[i]->Eval();
          mpsgui::ChoiceArg newArg(choice->second.names[i],
              *choice->second.types[i],
              *value);
          delete value;
          newChoice.args.push_back(newArg);
        }
        newChoice.accept=(choice->second.names.size()==0); // Auto accept, if no input required
        choices.push_back(newChoice);
      }
      delete accept;
    }
    // Update GUI with new choices
    mpsgui::SetChoices(mySession, myPid, choices);
  }
  // Create a possible step for each choice
  for (vector<mpsgui::Choice>::const_iterator choice=choices.begin(); choice!=choices.end(); ++choice)
  {
    map<string,inputbranch>::const_iterator branch=myBranches.find(choice->name);
    if (branch!=myBranches.end()) // choice is legal
    {
      if (choice->accept)
      {
        MpsEvent event;
        event.myType = sync;
        event.mySession = mySession;
        event.myLabel = branch->first;
        event.myMaxpid = myMaxpid;
        vector<string> paths;
        paths.push_back("");
        dest.push_back(MpsStep(event, paths));
      }
    }
    else
    {
#if APIMS_DEBUG_LEVEL>10
      cerr << "Illegal choice: " << choice->name << endl;
#endif
    }
  }
  return true;
} // }}}
MpsTerm *MpsGuiSync::PRename(const string &src, const string &dst) const // {{{
{
  map<string, inputbranch> newBranches;
  newBranches.clear();
  // PSubst each branch
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    inputbranch newBranch;
    newBranch.term = it->second.term->PRename(src,dst);
    newBranch.assertion = it->second.assertion->Copy();
    newBranch.args = it->second.args;
    newBranch.names = it->second.names;
    newBranch.types.clear();
    for (vector<MpsMsgType*>::const_iterator type=it->second.types.begin(); type!=it->second.types.end(); ++type)
      newBranch.types.push_back((*type)->Copy());
    newBranch.values.clear();
    for (vector<MpsExp*>::const_iterator value=it->second.values.begin(); value!=it->second.values.end(); ++value)
      newBranch.values.push_back((*value)->Copy());
    newBranches[it->first] = newBranch;
  }
  MpsGuiSync *result = new MpsGuiSync(myMaxpid, mySession, myPid, newBranches);
  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second.term;
    delete newBranches.begin()->second.assertion;
    DeleteVector(newBranches.begin()->second.types);
    DeleteVector(newBranches.begin()->second.values);
    newBranches.erase(newBranches.begin());
  }

  return result;
} // }}}
MpsTerm *MpsGuiSync::ERename(const string &src, const string &dst) const // {{{
{
  map<string, inputbranch> newBranches;
  newBranches.clear();
  string newSession=mySession==src?dst:mySession;
  // ERename each branch
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    inputbranch newBranch;
    newBranch.term = it->second.term->Copy(); // Copy branch-term
    newBranch.assertion = it->second.assertion->Rename(src,dst);
    newBranch.names = it->second.names;
    newBranch.types.clear();
    for (vector<MpsMsgType*>::const_iterator type=it->second.types.begin(); type!=it->second.types.end(); ++type)
      newBranch.types.push_back((*type)->Copy());
    newBranch.values.clear();
    for (vector<MpsExp*>::const_iterator value=it->second.values.begin(); value!=it->second.values.end(); ++value)
      newBranch.values.push_back((*value)->Rename(src,dst));
    // vector<string>::find
    bool found=false;
    for (vector<string>::const_iterator arg=it->second.args.begin(); arg!=it->second.args.end(); ++arg)
      if (*arg == src)
        found=true;
    if (found) // Do not substitute in body and no argument renaming
    {
      newBranch.args = it->second.args;
    }
    else
    {
      // Find new arguments, and rename if necessary
      newBranch.args.clear();
      for (vector<string>::const_iterator arg=it->second.args.begin(); arg!=it->second.args.end(); ++arg)
      {
        if (dst==*arg) //Argument must be renamed
        {
          string newArg = MpsExp::NewVar(); // Make unused name
          MpsTerm *tmpTerm = newBranch.term->ERename(*arg,newArg); // Replace old variable with new one
          delete newBranch.term;
          newBranch.term=tmpTerm;
          newBranch.args.push_back(newArg); // Add new argument name
        }
        else
          newBranch.args.push_back(*arg); // Add original argument name
      }
      MpsTerm *tmpTerm = newBranch.term->ERename(src,dst); // Make substitution in body
      delete newBranch.term;
      newBranch.term=tmpTerm;
    }

    newBranches[it->first] = newBranch;
  }
  MpsGuiSync *result = new MpsGuiSync(myMaxpid, newSession, myPid, newBranches);

  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second.term;
    delete newBranches.begin()->second.assertion;
    while (newBranches.begin()->second.types.size()>0)
    {
      delete *newBranches.begin()->second.types.begin();
      newBranches.begin()->second.types.erase(newBranches.begin()->second.types.begin());
    }
    while (newBranches.begin()->second.values.size()>0)
    {
      delete *newBranches.begin()->second.values.begin();
      newBranches.begin()->second.values.erase(newBranches.begin()->second.values.begin());
    }
    newBranches.erase(newBranches.begin());
  }

  return result;
} // }}}
MpsTerm *MpsGuiSync::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  map<string, inputbranch> newBranches;
  newBranches.clear();
  // ReIndex each branch
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    inputbranch newBranch;
    newBranch.term = it->second.term->Copy();
    newBranch.assertion = it->second.assertion->Copy();
    newBranch.names = it->second.names;
    newBranch.types.clear();
    for (vector<MpsMsgType*>::const_iterator type=it->second.types.begin(); type!=it->second.types.end(); ++type)
      newBranch.types.push_back((*type)->Copy());
    newBranch.values.clear();
    for (vector<MpsExp*>::const_iterator value=it->second.values.begin(); value!=it->second.values.end(); ++value)
      newBranch.values.push_back((*value)->Copy());
    // vector<string>::find
    bool found=false;
    for (vector<string>::const_iterator arg=it->second.args.begin(); arg!=it->second.args.end(); ++arg)
      if (*arg == session)
        found=true;
    newBranch.args = it->second.args;
    if (!found) // Do not substitute in body and no argument renaming
    {
      MpsTerm *tmpTerm = newBranch.term->ReIndex(session,pid,maxpid); // Make substitution in body
      delete newBranch.term;
      newBranch.term=tmpTerm;
    }

    newBranches[it->first] = newBranch;
  }
  MpsGuiSync *result = new MpsGuiSync(myMaxpid, mySession, myPid, newBranches);

  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second.term;
    delete newBranches.begin()->second.assertion;
    while (newBranches.begin()->second.types.size()>0)
    {
      delete *newBranches.begin()->second.types.begin();
      newBranches.begin()->second.types.erase(newBranches.begin()->second.types.begin());
    }
    while (newBranches.begin()->second.values.size()>0)
    {
      delete *newBranches.begin()->second.values.begin();
      newBranches.begin()->second.values.erase(newBranches.begin()->second.values.begin());
    }
    newBranches.erase(newBranches.begin());
  }

  return result;
} // }}}
MpsGuiSync *MpsGuiSync::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
{
  map<string, inputbranch> newBranches;
  newBranches.clear();
  // PSubst each branch
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    inputbranch newBranch;
    newBranch.term = it->second.term->PSubst(var,exp,args,argpids,stateargs);
    newBranch.assertion = it->second.assertion->Copy();
    newBranch.args = it->second.args;
    newBranch.names = it->second.names;
    newBranch.types.clear();
    for (vector<MpsMsgType*>::const_iterator type=it->second.types.begin(); type!=it->second.types.end(); ++type)
      newBranch.types.push_back((*type)->Copy());
    newBranch.values.clear();
    for (vector<MpsExp*>::const_iterator value=it->second.values.begin(); value!=it->second.values.end(); ++value)
      newBranch.values.push_back((*value)->Copy());
    newBranches[it->first] = newBranch;
  }
  MpsGuiSync *result = new MpsGuiSync(myMaxpid, mySession, myPid, newBranches);
  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second.term;
    delete newBranches.begin()->second.assertion;
    while (newBranches.begin()->second.types.size()>0)
    {
      delete *newBranches.begin()->second.types.begin();
      newBranches.begin()->second.types.erase(newBranches.begin()->second.types.begin());
    }
    while (newBranches.begin()->second.values.size()>0)
    {
      delete *newBranches.begin()->second.values.begin();
      newBranches.begin()->second.values.erase(newBranches.begin()->second.values.begin());
    }
    newBranches.erase(newBranches.begin());
  }

  return result;
} // }}}
MpsGuiSync *MpsGuiSync::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  map<string, inputbranch> newBranches;
  newBranches.clear();
  string newSession=stringESubst(mySession,source,dest);
  // ESubst each branch
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    inputbranch newBranch;
    newBranch.term = it->second.term->Copy(); // Copy branch-term
    newBranch.assertion = it->second.assertion->Subst(source,dest);
    newBranch.names = it->second.names;
    newBranch.types.clear();
    for (vector<MpsMsgType*>::const_iterator type=it->second.types.begin(); type!=it->second.types.end(); ++type)
      newBranch.types.push_back((*type)->Copy());
    newBranch.values.clear();
    for (vector<MpsExp*>::const_iterator value=it->second.values.begin(); value!=it->second.values.end(); ++value)
      newBranch.values.push_back((*value)->Subst(source,dest));
    // vector<string>::find
    bool found=false;
    for (vector<string>::const_iterator arg=it->second.args.begin(); arg!=it->second.args.end(); ++arg)
      if (*arg == source)
        found=true;
    if (found/*myArgs.find(source) != myArgs.end()*/) // Do not substitute in body and no argument renaming
    {
      newBranch.args = it->second.args;
    }
    else
    {
      // Find new arguments, and rename if necessary
      newBranch.args.clear();
      set<string> fv=dest.FV();
      for (vector<string>::const_iterator arg=it->second.args.begin(); arg!=it->second.args.end(); ++arg)
      {
        if (fv.find(*arg)!=fv.end()) //Argument must be renamed
        {
          string newArg = MpsExp::NewVar(); // Make unused name
          MpsExp *newVar = new MpsVarExp(newArg,MpsMsgNoType()); // Make variable with the new name
          MpsTerm *tmpTerm = newBranch.term->ESubst(*arg,*newVar); // Replace old variable with new one
          delete newVar;
          delete newBranch.term;
          newBranch.term=tmpTerm;
          newBranch.args.push_back(newArg); // Add new argument name
        }
        else
          newBranch.args.push_back(*arg); // Add original argument name
      }
      MpsTerm *tmpTerm = newBranch.term->ESubst(source,dest); // Make substitution in body
      delete newBranch.term;
      newBranch.term=tmpTerm;
    }

    newBranches[it->first] = newBranch;
  }
  MpsGuiSync *result = new MpsGuiSync(myMaxpid, newSession, myPid, newBranches);

  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second.term;
    delete newBranches.begin()->second.assertion;
    while (newBranches.begin()->second.types.size()>0)
    {
      delete *newBranches.begin()->second.types.begin();
      newBranches.begin()->second.types.erase(newBranches.begin()->second.types.begin());
    }
    while (newBranches.begin()->second.values.size()>0)
    {
      delete *newBranches.begin()->second.values.begin();
      newBranches.begin()->second.values.erase(newBranches.begin()->second.values.begin());
    }
    newBranches.erase(newBranches.begin());
  }

  return result;
} // }}}
MpsGuiSync *MpsGuiSync::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  map<string, inputbranch> newBranches;
  newBranches.clear();
  // GSubst each branch
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    inputbranch newBranch;
    newBranch.term = it->second.term->GSubst(source,dest,args);
    newBranch.assertion = it->second.assertion->Copy();
    newBranch.names = it->second.names;
    newBranch.args = it->second.args;
    newBranch.types.clear();
    for (vector<MpsMsgType*>::const_iterator type=it->second.types.begin(); type!=it->second.types.end(); ++type)
      newBranch.types.push_back((*type)->GSubst(source,dest,args));
    newBranch.values.clear();
    for (vector<MpsExp*>::const_iterator value=it->second.values.begin(); value!=it->second.values.end(); ++value)
      newBranch.values.push_back((*value)->Copy());

    newBranches[it->first] = newBranch;
  }
  MpsGuiSync *result = new MpsGuiSync(myMaxpid, mySession, myPid, newBranches);

  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second.term;
    delete newBranches.begin()->second.assertion;
    while (newBranches.begin()->second.types.size()>0)
    {
      delete *newBranches.begin()->second.types.begin();
      newBranches.begin()->second.types.erase(newBranches.begin()->second.types.begin());
    }
    while (newBranches.begin()->second.values.size()>0)
    {
      delete *newBranches.begin()->second.values.begin();
      newBranches.begin()->second.values.erase(newBranches.begin()->second.values.begin());
    }
    newBranches.erase(newBranches.begin());
  }

  return result;
} // }}}
MpsGuiSync *MpsGuiSync::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  map<string, inputbranch> newBranches;
  newBranches.clear();
  // LSubst each branch
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    inputbranch newBranch;
    newBranch.term = it->second.term->LSubst(source,dest,args);
    newBranch.assertion = it->second.assertion->Copy();
    newBranch.names = it->second.names;
    newBranch.args = it->second.args;
    newBranch.types.clear();
    for (vector<MpsMsgType*>::const_iterator type=it->second.types.begin(); type!=it->second.types.end(); ++type)
      newBranch.types.push_back((*type)->LSubst(source,dest,args));
    newBranch.values.clear();
    for (vector<MpsExp*>::const_iterator value=it->second.values.begin(); value!=it->second.values.end(); ++value)
      newBranch.values.push_back((*value)->Copy());

    newBranches[it->first] = newBranch;
  }
  MpsGuiSync *result = new MpsGuiSync(myMaxpid, mySession, myPid, newBranches);

  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second.term;
    delete newBranches.begin()->second.assertion;
    while (newBranches.begin()->second.types.size()>0)
    {
      delete *newBranches.begin()->second.types.begin();
      newBranches.begin()->second.types.erase(newBranches.begin()->second.types.begin());
    }
    while (newBranches.begin()->second.values.size()>0)
    {
      delete *newBranches.begin()->second.values.begin();
      newBranches.begin()->second.values.erase(newBranches.begin()->second.values.begin());
    }
    newBranches.erase(newBranches.begin());
  }

  return result;
} // }}}
set<string> MpsGuiSync::FPV() const // {{{
{
  set<string> result;
  result.clear();
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    set<string> sub = it->second.term->FPV();
    result.insert(sub.begin(), sub.end());
  }
  return result;
} // }}}
set<string> MpsGuiSync::FEV() const // {{{
{
  set<string> result;
  result.clear();
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    set<string> sub = it->second.term->FEV();
    for (vector<string>::const_iterator arg=it->second.args.begin(); arg!=it->second.args.end(); ++arg)
      sub.erase(*arg);
    result.insert(sub.begin(), sub.end());
  }
  result.insert(mySession);
  return result;
} // }}}
MpsGuiSync *MpsGuiSync::Copy() const // {{{
{
  return new MpsGuiSync(myMaxpid, mySession, myPid, myBranches);
} // }}}
bool MpsGuiSync::Terminated() const // {{{
{
  return false;
} // }}}
MpsGuiSync *MpsGuiSync::Simplify() const // {{{
{
  map<string,inputbranch> newBranches;
  newBranches.clear();
  for (map<string,inputbranch>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  {
    inputbranch newBranch;
    newBranch.term = it->second.term->Simplify();
    newBranch.assertion = it->second.assertion->Copy();
    newBranch.names = it->second.names;
    newBranch.args = it->second.args;
    newBranch.types.clear();
    for (vector<MpsMsgType*>::const_iterator type=it->second.types.begin(); type!=it->second.types.end(); ++type)
      newBranch.types.push_back((*type)->Copy());
    newBranch.values.clear();
    for (vector<MpsExp*>::const_iterator value=it->second.values.begin(); value!=it->second.values.end(); ++value)
      newBranch.values.push_back((*value)->Copy());
    newBranches[it->first] = newBranch;
  }
  MpsGuiSync *result = new MpsGuiSync(myMaxpid,mySession, myPid, newBranches);
  // Clean up
  while (newBranches.size()>0)
  {
    delete newBranches.begin()->second.term;
    delete newBranches.begin()->second.assertion;
    while (newBranches.begin()->second.types.size()>0)
    {
      delete *newBranches.begin()->second.types.begin();
      newBranches.begin()->second.types.erase(newBranches.begin()->second.types.begin());
    }
    while (newBranches.begin()->second.values.size()>0)
    {
      delete *newBranches.begin()->second.values.begin();
      newBranches.begin()->second.values.erase(newBranches.begin()->second.values.begin());
    }
    newBranches.erase(newBranches.begin());
  }
  return result;
} // }}}
string MpsGuiSync::ToString(string indent) const // {{{
{
  string newIndent = indent + "    ";
  string result = (string)"guisync(" + int2string(myMaxpid) + "," + mySession + "," + int2string(myPid) + ")\n" + indent + "{ ";
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    if (it != myBranches.begin())
      result += ",\n" + indent + "  ";
    result += it->first + "[[" + it->second.assertion->ToString() + "]](";
    for (int i=0; i<it->second.args.size() && i<it->second.names.size() && i<it->second.types.size(); ++i)
    {
      if (i>0)
        result += ",";
      result += it->second.args[i] + "=" + it->second.values[i]->ToString() + ": " +it->second.types[i]->ToString();
    }
    result += "):\n" + newIndent + it->second.term->ToString(newIndent);
  }
  result += "\n" + indent + "}";
  return result;
} // }}}
string MpsGuiSync::ToTex(int indent, int sw) const // {{{
{
  int newIndent = indent + 4;
  string result = ToTex_KW("guisync") + "(" + ToTex_PP(myMaxpid) + "," + ToTex_Session(mySession) + "," + ToTex_PP(myPid) + ")\\newline\n"
                + ToTex_Hspace(indent,sw) + "\\{ ";
  for (map<string,inputbranch>::const_iterator it = myBranches.begin(); it != myBranches.end(); ++it)
  {
    if (it != myBranches.begin())
      result += ",\\newline\n"
              + ToTex_Hspace(indent+2,sw);
    result += ToTex_Label(it->first);
    if (it->second.assertion->ToString()!="true")
      result += "$\\llbracket$" + it->second.assertion->ToString() + "$\\rrbracket$";
    result += "(";
    for (int i=0; i<it->second.args.size() && i<it->second.names.size() && i<it->second.types.size(); ++i)
    {
      if (i>0)
        result += ",";
      result += it->second.args[i] + "=" + it->second.values[i]->ToString() + ": " +it->second.types[i]->ToString();
    }
    result += "):\\newline\n"
            + ToTex_Hspace(newIndent,sw) + it->second.term->ToTex(newIndent,sw);
  }
  result += "\\newline\n"
          + ToTex_Hspace(indent,sw) + "\\}";
  return result;
} // }}}
string MpsGuiSync::ToC() const // {{{
{
  throw (string)"MpsGuiSync::ToC(): guisync is not implemented yet!";
} // }}}
string MpsGuiSync::ToCHeader() const // {{{
{
  throw (string)"MpsGuiSync::ToC(): guisync is not implemented yet!";
} // }}}
MpsTerm *MpsGuiSync::RenameAll() const // {{{
{ map<string,inputbranch> newBranches;
  for (map<string,inputbranch>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  { inputbranch newBranch;
    // Copy names
    newBranch.names=it->second.names;
    // Copy assertion
    newBranch.assertion=it->second.assertion->Copy();
    // Copy values
    for (vector<MpsExp*>::const_iterator val=it->second.values.begin();val!=it->second.values.end(); ++val)
      newBranch.values.push_back((*val)->Copy());
    // Perform RenameAll on types
    for (vector<MpsMsgType*>::const_iterator type=it->second.types.begin();type!=it->second.types.end(); ++type)
      newBranch.types.push_back((*type)->RenameAll());
    // Rename args
    for (vector<string>::const_iterator arg=it->second.args.begin();arg!=it->second.args.end(); ++arg)
      newBranch.args.push_back(MpsExp::NewVar(*arg));
    // Generate term with renames args
    newBranch.term=it->second.term->RenameAll();
    for (int i=0;i<it->second.args.size(); ++i)
    { MpsTerm *tmpTerm=newBranch.term->ERename(it->second.args[i],newBranch.args[i]);
      delete newBranch.term;
      newBranch.term=tmpTerm;
    }
    // Add to newBranches
    newBranches[it->first]=newBranch;
  }
  MpsTerm *result = new MpsGuiSync(myMaxpid,mySession,myPid,newBranches);

  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second.term;
    delete newBranches.begin()->second.assertion;
    DeleteVector(newBranches.begin()->second.types);
    DeleteVector(newBranches.begin()->second.values);
    newBranches.erase(newBranches.begin());
  }

  return result;
} // }}}
bool MpsGuiSync::Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const // {{{
{ map<string,inputbranch> newBranches;
  for (map<string,inputbranch>::const_iterator branch=myBranches.begin(); branch!=myBranches.end(); ++branch)
  {
    newBranches[branch->first].term=branch->second.term->Parallelize();
    newBranches[branch->first].assertion=branch->second.assertion->Copy();
    newBranches[branch->first].names=branch->second.names;
    newBranches[branch->first].args=branch->second.args;
    newBranches[branch->first].types=CopyVector(branch->second.types);
    newBranches[branch->first].values=CopyVector(branch->second.values);
  }
  seqTerm = new MpsGuiSync(myMaxpid, mySession, myPid, newBranches);
  parTerm = receives.Append(*seqTerm);
  // Clean up
  while (newBranches.size()>0)
  { delete newBranches.begin()->second.term;
    delete newBranches.begin()->second.assertion;
    DeleteVector(newBranches.begin()->second.types);
    DeleteVector(newBranches.begin()->second.values);
    newBranches.erase(newBranches.begin());
  }
  return false; // All optimizations are guarded
} // }}}
MpsTerm *MpsGuiSync::Append(const MpsTerm &term) const // {{{
{ map<string,inputbranch> newBranches;
  for (map<string,inputbranch>::const_iterator branch=myBranches.begin(); branch!=myBranches.end(); ++branch)
  {
    newBranches[branch->first].term=branch->second.term->Append(term);
    newBranches[branch->first].assertion=branch->second.assertion->Copy();
    newBranches[branch->first].names=branch->second.names;
    newBranches[branch->first].args=branch->second.args;
    newBranches[branch->first].types=CopyVector(branch->second.types);
    newBranches[branch->first].values=CopyVector(branch->second.values);
  }
  MpsTerm *result = new MpsGuiSync(myMaxpid, mySession, myPid, newBranches);
  // Clean up
  while (newBranches.size()>0)
  { delete newBranches.begin()->second.term;
    delete newBranches.begin()->second.assertion;
    DeleteVector(newBranches.begin()->second.types);
    DeleteVector(newBranches.begin()->second.values);
    newBranches.erase(newBranches.begin());
  }

  return result;
} // }}}
MpsTerm *MpsGuiSync::CloseDefinitions() const // {{{
{
  // Create new branches
  map<string,inputbranch> newBranches;
  for (map<string,inputbranch>::const_iterator br=myBranches.begin(); br!=myBranches.end(); ++br)
  { inputbranch newBr;
    newBr.term=br->second.term->CloseDefinitions();
    newBr.assertion=br->second.assertion->Copy();
    newBr.names=br->second.names;
    newBr.args=br->second.args;
    for (vector<MpsMsgType*>::const_iterator t=br->second.types.begin(); t!=br->second.types.end(); ++t)
      newBr.types.push_back((*t)->Copy());
    for (vector<MpsExp*>::const_iterator v=br->second.values.begin(); v!=br->second.values.end(); ++v)
      newBr.values.push_back((*v)->Copy());
    newBranches[br->first]=newBr;
  }
  // Create result
  MpsTerm *result=new MpsGuiSync(myMaxpid, mySession, myPid, newBranches);
  // Clean up
  while (newBranches.size()>0)
  {
    delete newBranches.begin()->second.term;
    delete newBranches.begin()->second.assertion;
    DeleteVector(newBranches.begin()->second.types);
    DeleteVector(newBranches.begin()->second.values);
    newBranches.erase(newBranches.begin());
  }
  // return
  return result;
} // }}}
MpsTerm *MpsGuiSync::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ map<string,inputbranch> newBranches;
  for (map<string,inputbranch>::const_iterator it=myBranches.begin(); it!=myBranches.end(); ++it)
  { inputbranch newBranch;
    // Remove defs from term
    newBranch.term=it->second.term->ExtractDefinitions(env);
    // Copy the rest
    newBranch.assertion=it->second.assertion->Copy();
    newBranch.names=it->second.names;
    newBranch.args=it->second.args;
    for (vector<MpsMsgType*>::const_iterator type=it->second.types.begin();type!=it->second.types.end(); ++type)
      newBranch.types.push_back((*type)->Copy());
    for (vector<MpsExp*>::const_iterator val=it->second.values.begin();val!=it->second.values.end(); ++val)
      newBranch.values.push_back((*val)->Copy());
    // Add to newBranches
    newBranches[it->first]=newBranch;
  }
  MpsTerm *result = new MpsGuiSync(myMaxpid,mySession,myPid,newBranches);

  // Clean up
  while (newBranches.size() > 0)
  {
    delete newBranches.begin()->second.term;
    delete newBranches.begin()->second.assertion;
    DeleteVector(newBranches.begin()->second.types);
    DeleteVector(newBranches.begin()->second.values);
    newBranches.erase(newBranches.begin());
  }

  return result;
} // }}}
