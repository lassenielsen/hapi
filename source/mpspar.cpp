#include<hapi/mpspar.hpp>
#include<hapi/mpsdef.hpp>
#include<hapi/mpslink.hpp>
#include<hapi/mpscall.hpp>
#include<hapi/common.hpp>

using namespace std;
using namespace hapi;

MpsPar::MpsPar(const MpsTerm &left, const MpsTerm &right, const vector<string> &leftFinal, const vector<string> &rightFinal) // {{{
: myLeftFinal(leftFinal)
, myRightFinal(rightFinal)
{
  // Assert left != NULL
  // Assert right != NULL
  myLeft = left.Copy();
  myRight = right.Copy();
} // }}}
MpsPar::~MpsPar() // {{{
{
  // assert myLeft and myRight != NULL
  delete myLeft;
  delete myRight;
} // }}}
bool MpsPar::TypeCheck(const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, const string &curPure, PureState pureState, bool checkPure) // Use rule Par {{{
{
  // Check purity constraionts
  PureState nextState=pureState;
  if (checkPure)
  { if (pureState!=CPS_IMPURE && pureState!=CPS_PURE)
      return PrintTypeError("Error in implementation of pure participant " + curPure + ". Pure implementations must conform with the structure \n     *   local X()\n	   *   ( global s=new ch(p of n);\n		 *     X();\n		 *     |\n		 *     P\n		 *   )\n		 *   local StartX(Int i)\n		 *   ( if i<=0\n		 *     then X();\n		 *     else X(); | StartX(i-1);\n		 *   )\n		 *   StartX( E ); |" ,*this,Theta,Gamma,Omega);
    if (pureStack.size()>0)
    { MpsDef *pureDef=dynamic_cast<MpsDef*>(myLeft);
      if (pureDef==NULL)
        return PrintTypeError("Implementation of pure participant " + int2string(pureStack.begin()->second) + "@" + pureStack.begin()->first + " must be immediately after its decleration",*this,Theta,Gamma,Omega);
    // Move to Def
    //if (pureDef->GetArgs().size()>0 || pureDef->GetStateArgs().size()>0)
    //  return PrintTypeError("Implementation of pure participant " + pureDef->GetName() + " must have no arguments and state",*this,Theta,Gamma,Omega);
    //MpsCall *succCall=dynamic_cast<MpsCall*>(pureDef->GetSucc());
    //if (succCall==NULL || succCall->GetName()!=pureDef->GetName() || succCall->GetArgs().size()>0 || succCall->GetState().size()>0)
    //  return PrintTypeError("Implementation of pure participant " + pureDef->GetName() + " must be immediately followed by a direct invocation of implementation process (def X() = ... in X())",*this,Theta,Gamma,Omega);
      // Extract implemented participant
      MpsLink *bodyLink=dynamic_cast<MpsLink*>(pureDef->GetBody());
      if (bodyLink==NULL)
        return PrintTypeError("Implementation of pure participant " + pureDef->GetName() + " must start by linking as the implemented participant (def X() = link ...)",*this,Theta,Gamma,Omega);
      set<pair<string,int> > newPureStack=pureStack;
        set<pair<string,int> >::iterator impl=newPureStack.find(pair<string,int>(bodyLink->GetChannel(),bodyLink->GetPid()));
        if (impl==newPureStack.end())
          return PrintTypeError("Expected implementation of pure participant but linking as " + int2string(bodyLink->GetPid()) + "@" + bodyLink->GetChannel(),*this,Theta,Gamma,Omega);
        newPureStack.erase(impl);
      }
      else
        pureState=CPS_SERVICE_DEF
    MpsPar *bodyPar=dynamic_cast<MpsPar*>(bodyLink->GetSucc());
    if (bodyPar==NULL)
      return PrintTypeError("Implementation of pure participant " + pureDef->GetName() + " must start by linking and forking (def X() = ses ... in X() | ...)",*this,Theta,Gamma,Omega);
    const MpsCall *bodyCall=dynamic_cast<const MpsCall*>(bodyPar->myLeft);
    if (bodyCall==NULL || bodyCall->GetName()!=pureDef->GetName())
      return PrintTypeError("Implementation of pure participant " + pureDef->GetName() + " must start by linking and forking and invocation of implementation process (def X() = ses ... in X() | ...)",*this,Theta,Gamma,Omega);

    // Structure alright, now invoke typechecking of body and succ
    MpsMsgEnv pureGamma;
    for (MpsMsgEnv::const_iterator var = Gamma.begin(); var!=Gamma.end(); ++var)
      if (dynamic_cast<const MpsDelegateMsgType*>(var->second)==NULL) // Not session type
        pureGamma[var->first]=var->second;
      else if (!dynamic_cast<const MpsDelegateMsgType*>(var->second)->GetLocalType()->IsDone())
        myLeftFinal.push_back(var->first);

    // Store env (for pureDef) for later (compilation)
    DeleteMap(pureDef->GetEnv());
    for (MpsMsgEnv::const_iterator it=pureGamma.begin(); it!=pureGamma.end(); ++it)
      pureDef->GetEnv()[it->first]=it->second->Copy();

    // Check linking on available channel
    MpsMsgEnv::iterator var=pureGamma.find(bodyLink->GetChannel());
    if (var==pureGamma.end())
      return PrintTypeError((string)"Linking on unknown channel:" + bodyLink->GetChannel(),*this,Theta,Gamma,Omega);
    const MpsChannelMsgType *channel=dynamic_cast<const MpsChannelMsgType*>(var->second);
    if (channel==NULL)
      return PrintTypeError((string)"Linking on non-channel:" + bodyLink->GetChannel(),*this,Theta,Gamma,Omega);
    // Check correct maxpid
    if (bodyLink->GetMaxpid() != channel->GetGlobalType()->GetMaxPid())
      return PrintTypeError((string)"MaxPID is different from:" + int2string(channel->GetGlobalType()->GetMaxPid()),*this,Theta,Gamma,Omega);
    // Create local type
    MpsLocalType *newType=channel->GetGlobalType()->Project(bodyLink->GetPid());
    set<string> fv = newType->FEV();
    // Rename all free variables
    for (set<string>::const_iterator it=fv.begin(); it!=fv.end(); ++it)
    { MpsLocalType *tmpType=newType->ERename(*it,MpsExp::NewVar(*it));
      delete newType;
      newType=tmpType;
    }
    // Create Gamma with new session
    pureGamma[bodyLink->GetSession()] = new MpsDelegateLocalMsgType(*newType,bodyLink->GetPid(),channel->GetParticipants());
    bodyPar->myLeftFinal.push_back(bodyLink->GetSession());

    MpsProcEnv pureOmega;
    bool result;
    MpsDef *subDef=dynamic_cast<MpsDef*>(bodyPar->myRight);
    if (subDef==NULL)
    {
      result = bodyPar->myRight->TypeCheck(Theta, pureGamma, pureOmega, set<pair<string,int> >(), "$") &&
               myRight->TypeCheck(Theta, Gamma, Omega, newPureStack, curPure);
    }
    else
    { // FIXME: Check only one arg with correct type
      if (subDef->GetArgs().size()!=1 || pureDef->GetStateArgs().size()>0)
        return PrintTypeError("Recursive implementation of pure participant " + subDef->GetName() + " must have exactly one argument and no state",*this,Theta,Gamma,Omega);
      // FIXME: Check body is direct call with pure session as only arg
      const MpsCall *subCall=dynamic_cast<const MpsCall*>(subDef->GetSucc());
      if (subCall==NULL || subCall->GetName()!=subDef->GetName() || subCall->GetArgs().size()!=1 || subCall->GetState().size()>0 || subCall->GetArgs()[0]->ToString()!=bodyLink->GetSession())
        return PrintTypeError("Recursive implementation of pure participant " + subDef->GetName() + " must be immediately followed by a direct invocation of implementation process (def X(G s) = ... in X(s))",*this,Theta,Gamma,Omega);
      // Store env (for pureDef) for later (compilation)
      DeleteMap(subDef->GetEnv());
      for (MpsMsgEnv::const_iterator it=pureGamma.begin(); it!=pureGamma.end(); ++it)
        subDef->GetEnv()[it->first]=it->second->Copy();
      // Add subDef to pureOmega
      pureOmega[subDef->GetName()].types = subDef->GetTypes();
      pureOmega[subDef->GetName()].snames = subDef->GetStateArgs();
      pureOmega[subDef->GetName()].stypes = subDef->GetStateTypes();
      result = subDef->GetBody()->TypeCheck(Theta, pureGamma, pureOmega, set<pair<string,int> >(), subDef->GetName()) &&
               myRight->TypeCheck(Theta, Gamma, Omega, newPureStack, curPure);
    }
    
    // Clean up
    delete pureGamma[bodyLink->GetSession()];

    return result;
  }

  // Normal fork
  // Split Gammma
  MpsMsgEnv leftGamma;
  MpsMsgEnv rightGamma;
  set<string> leftSessions = myLeft->FEV();
  for (MpsMsgEnv::const_iterator var=Gamma.begin(); var!=Gamma.end(); ++var)
  {
    const MpsDelegateMsgType *delType=dynamic_cast<const MpsDelegateMsgType*>(var->second);
    if (delType==NULL) // Is not session type
    { leftGamma[var->first]=var->second;
      rightGamma[var->first]=var->second;
    }
    else
    { if (leftSessions.find(var->first)!=leftSessions.end())
      {
        leftGamma[var->first]=var->second;
        if (!delType->GetLocalType()->IsDone())
          myRightFinal.push_back(var->first);
      }
      else
      {
        rightGamma[var->first]=var->second;
        if (!delType->GetLocalType()->IsDone())
          myLeftFinal.push_back(var->first);
      }
    }
  }

  // Check each sub-process with the split Gamma
  return myLeft->TypeCheck(Theta,leftGamma,Omega,pureStack,curPure) &&
         myRight->TypeCheck(Theta,rightGamma,Omega,pureStack,curPure);
} // }}}
MpsTerm *MpsPar::ApplyRcv(const std::string &path, const MpsExp *val) const // {{{
{ if (path.size()==0)
    return Error((string)"Applying Rcv on "+ToString());
  MpsTerm *left=NULL;
  MpsTerm *right=NULL;
  if (path[0]=='l') // left
  { left=myLeft->ApplyRcv(path.substr(1),val);
    right=myRight->Copy();
  }
  else // assume right
  { left=myLeft->Copy();
    right=myRight->ApplyRcv(path.substr(1),val);
  }
  MpsTerm *result = new MpsPar(*left,*right,vector<string>(),vector<string>());
  delete left;
  delete right;
  return result;
} // }}}
MpsTerm *MpsPar::ApplySnd(const std::string &path, MpsExp **val, MpsChannel &ch) const // {{{
{ if (path.size()==0)
    return Error((string)"Applying Snd on "+ToString());
  MpsTerm *left=NULL;
  MpsTerm *right=NULL;
  if (path[0]=='l') // left
  { left=myLeft->ApplySnd(path.substr(1),val,ch);
    right=myRight->Copy();
  }
  else // assume right
  { left=myLeft->Copy();
    right=myRight->ApplySnd(path.substr(1),val,ch);
  }
  MpsTerm *result = new MpsPar(*left,*right,vector<string>(),vector<string>());
  delete left;
  delete right;
  return result;
} // }}}
MpsTerm *MpsPar::ApplyBRcv(const std::string &path, const std::string &label) const // {{{
{ if (path.size()==0)
    return Error((string)"Applying BRcv on "+ToString());
  MpsTerm *left=NULL;
  MpsTerm *right=NULL;
  if (path[0]=='l') // left
  { left=myLeft->ApplyBRcv(path.substr(1),label);
    right=myRight->Copy();
  }
  else // assume right
  { left=myLeft->Copy();
    right=myRight->ApplyBRcv(path.substr(1),label);
  }
  MpsTerm *result = new MpsPar(*left,*right,vector<string>(),vector<string>());
  delete left;
  delete right;
  return result;
} // }}}
MpsTerm *MpsPar::ApplyBSnd(const std::string &path, std::string &label, MpsChannel &ch) const // {{{
{ if (path.size()==0)
    return Error((string)"Applying BSnd on "+ToString());
  MpsTerm *left=NULL;
  MpsTerm *right=NULL;
  if (path[0]=='l') // left
  { left=myLeft->ApplyBSnd(path.substr(1),label,ch);
    right=myRight->Copy();
  }
  else // assume right
  { left=myLeft->Copy();
    right=myRight->ApplyBSnd(path.substr(1),label,ch);
  }
  MpsTerm *result=new MpsPar(*left,*right,vector<string>(),vector<string>());
  // Clean up
  delete left;
  delete right;
  // Return
  return result;
} // }}}
MpsTerm *MpsPar::ApplyLink(const std::vector<std::string> &paths, const std::string &session) const // {{{
{ vector<string> pleft;
  vector<string> pright;
  // split paths
  for (vector<string>::const_iterator path=paths.begin();path!=paths.end(); ++path)
  { if (path->size()==0)
      return Error((string)"Applying Link on "+ToString());
    if ((*path)[0]=='l') // left
      pleft.push_back(path->substr(1));
    else // assume right
      pright.push_back(path->substr(1));
  }
  // Create subterms
  MpsTerm *left=myLeft->ApplyLink(pleft,session);
  MpsTerm *right=myRight->ApplyLink(pright,session);
  // Create result
  MpsTerm *result=new MpsPar(*left,*right,vector<string>(),vector<string>());
  // Clean up
  delete left;
  delete right;
  return result;
} // }}}
MpsTerm *MpsPar::ApplySync(const std::vector<std::string> &paths, const std::string &label) const // {{{
{ vector<string> pleft;
  vector<string> pright;
  // split paths
  for (vector<string>::const_iterator path=paths.begin();path!=paths.end(); ++path)
  { if (path->size()==0)
      return Error((string)"Applying Sync on "+ToString());
    if ((*path)[0]=='l') // left
      pleft.push_back(path->substr(1));
    else // assume right
      pright.push_back(path->substr(1));
  }
  // Create subterms
  MpsTerm *left=myLeft->ApplySync(pleft,label);
  MpsTerm *right=myRight->ApplySync(pright,label);
  // Create result
  MpsTerm *result=new MpsPar(*left,*right,vector<string>(),vector<string>());
  // Clean up
  delete left;
  delete right;
  return result;
} // }}}
MpsTerm *MpsPar::ApplyDef(const std::string &path, std::vector<MpsFunction> &dest) const // {{{
{ if (path.size()==0)
    return Error((string)"Applying Def on "+ToString());
  MpsTerm *left=NULL;
  MpsTerm *right=NULL;
  if (path[0]=='l') // left
  { left=myLeft->ApplyDef(path.substr(1),dest);
    right=myRight->Copy();
  }
  else // assume right
  { left=myLeft->Copy();
    right=myRight->ApplyDef(path.substr(1),dest);
  }
  MpsTerm *result=new MpsPar(*left,*right,vector<string>(),vector<string>());
  // Clean up
  delete left;
  delete right;
  // Return
  return result;
} // }}}
MpsTerm *MpsPar::ApplyCall(const std::string &path, const std::vector<MpsFunction> &funs) const // {{{
{ if (path.size()==0)
    return Error((string)"Applying Call on "+ToString());
  MpsTerm *left=NULL;
  MpsTerm *right=NULL;
  if (path[0]=='l') // left
  { left=myLeft->ApplyCall(path.substr(1),funs);
    right=myRight->Copy();
  }
  else // assume right
  { left=myLeft->Copy();
    right=myRight->ApplyCall(path.substr(1),funs);
  }
  MpsTerm *result=new MpsPar(*left,*right,vector<string>(),vector<string>());
  // Clean up
  delete left;
  delete right;
  // Return
  return result;
} // }}}
MpsTerm *MpsPar::ApplyOther(const std::string &path) const // {{{
{ if (path.size()==0)
    return Error((string)"Applying Other on "+ToString());
  MpsTerm *left=NULL;
  MpsTerm *right=NULL;
  if (path[0]=='l') // left
  { left=myLeft->ApplyOther(path.substr(1));
    right=myRight->Copy();
  }
  else // assume right
  { left=myLeft->Copy();
    right=myRight->ApplyOther(path.substr(1));
  }
  MpsTerm *result=new MpsPar(*left,*right,vector<string>(),vector<string>());
  // Clean up
  delete left;
  delete right;
  // Return
  return result;
} // }}}
bool MpsPar::SubSteps(vector<MpsStep> &dest) // {{{
{
  vector<MpsStep> leftsteps;
  leftsteps.clear();
  bool wait_left=myLeft->SubSteps(leftsteps);
  vector<MpsStep> rightsteps;
  rightsteps.clear();
  bool wait_right=myRight->SubSteps(rightsteps);
  // Add all steps from left process
  for (vector<MpsStep>::const_iterator it = leftsteps.begin(); it != leftsteps.end(); ++it)
  { vector<string> paths;
    for (vector<string>::const_iterator path=it->GetPaths().begin(); path!=it->GetPaths().end(); ++path)
      paths.push_back((string)"l"+(*path));
    dest.push_back(MpsStep(it->GetEvent(), paths));
  }
  // Add all steps from right process
  for (vector<MpsStep>::const_iterator it = rightsteps.begin(); it != rightsteps.end(); ++it)
  { vector<string> paths;
    for (vector<string>::const_iterator path=it->GetPaths().begin(); path!=it->GetPaths().end(); ++path)
      paths.push_back((string)"r"+(*path));
    dest.push_back(MpsStep(it->GetEvent(), paths));
  }
  // Clean up
  leftsteps.clear();
  rightsteps.clear();
  return wait_left || wait_right;
} // }}}
MpsTerm *MpsPar::PRename(const string &src, const string &dst) const // {{{
{
  // assert myLeft != NULL
  // assert myRight != NULL
  MpsTerm *newLeft = myLeft->PRename(src,dst);
  MpsTerm *newRight = myRight->PRename(src,dst);
  MpsTerm *result = new MpsPar(*newLeft, *newRight, GetLeftFinal(), GetRightFinal());
  delete newLeft;
  delete newRight;
  return result;
} // }}}
MpsTerm *MpsPar::ERename(const string &src, const string &dst) const // {{{
{
  MpsTerm *newLeft = myLeft->ERename(src,dst); // Rename in left term
  MpsTerm *newRight = myRight->ERename(src,dst); // Rename in right term
  vector<string> leftFinal;
  for (vector<string>::const_iterator it=GetLeftFinal().begin(); it!=GetLeftFinal().end(); ++it)
    if (*it==src)
      leftFinal.push_back(dst);
    else
      leftFinal.push_back(*it);
  vector<string> rightFinal;
  for (vector<string>::const_iterator it=GetRightFinal().begin(); it!=GetRightFinal().end(); ++it)
    if (*it==src)
      rightFinal.push_back(dst);
    else
      rightFinal.push_back(*it);
  MpsTerm *result = new MpsPar(*newLeft, *newRight, leftFinal, rightFinal); // Combine renamed terms
  delete newLeft;
  delete newRight;
  return result;
} // }}}
MpsTerm *MpsPar::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  MpsTerm *newLeft = myLeft->ReIndex(session,pid,maxpid); // Rename in left term
  MpsTerm *newRight = myRight->ReIndex(session,pid,maxpid); // Rename in right term
  MpsTerm *result = new MpsPar(*newLeft, *newRight, GetLeftFinal(), GetRightFinal()); // Combine renamed terms
  delete newLeft;
  delete newRight;
  return result;
} // }}}
MpsTerm *MpsPar::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
{
  // assert myLeft != NULL
  // assert myRight != NULL
  MpsTerm *newLeft = myLeft->PSubst(var,exp,args,argpids,stateargs);
  MpsTerm *newRight = myRight->PSubst(var,exp,args,argpids,stateargs);
  MpsTerm *result = new MpsPar(*newLeft, *newRight, GetLeftFinal(), GetRightFinal());
  delete newLeft;
  delete newRight;
  return result;
} // }}}
MpsTerm *MpsPar::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  // assert myLeft != NULL
  // assert myRight != NULL
  MpsTerm *newLeft = myLeft->ESubst(source,dest); // Substitute left term
  MpsTerm *newRight = myRight->ESubst(source,dest); // Substitute right term
  MpsTerm *result = new MpsPar(*newLeft, *newRight, GetLeftFinal(), GetRightFinal()); // Combine substituted terms
  delete newLeft;
  delete newRight;
  return result;
} // }}}
MpsTerm *MpsPar::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newLeft = myLeft->GSubst(source,dest,args); // Substitute left term
  MpsTerm *newRight = myRight->GSubst(source,dest,args); // Substitute right term
  MpsTerm *result = new MpsPar(*newLeft, *newRight, GetLeftFinal(), GetRightFinal()); // Combine substituted terms

  // Clean Up
  delete newLeft;
  delete newRight;

  return result;
} // }}}
MpsTerm *MpsPar::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newLeft = myLeft->LSubst(source,dest,args); // Substitute left term
  MpsTerm *newRight = myRight->LSubst(source,dest,args); // Substitute right term
  MpsTerm *result = new MpsPar(*newLeft, *newRight, GetLeftFinal(), GetRightFinal()); // Combine substituted terms

  // Clean Up
  delete newLeft;
  delete newRight;

  return result;
} // }}}
set<string> MpsPar::FPV() const // {{{
{
  set<string> result = myLeft->FPV();
  set<string> sub = myRight->FPV();
  result.insert(sub.begin(),sub.end());
  return result;
} // }}}
set<string> MpsPar::FEV() const // {{{
{
  set<string> result = myLeft->FEV();
  set<string> sub = myRight->FEV();
  result.insert(sub.begin(),sub.end());
  return result;
} // }}}
MpsTerm *MpsPar::Copy() const // {{{
{
  return new MpsPar(*myLeft, *myRight, GetLeftFinal(), GetRightFinal());
} // }}}
bool MpsPar::Terminated() const // {{{
{
  return myLeft->Terminated() && myRight->Terminated();
} // }}}
MpsTerm *MpsPar::Simplify() const // {{{
{
  MpsTerm *newLeft = myLeft->Simplify();
  MpsTerm *newRight = myRight->Simplify();
  MpsTerm *result = NULL;
  if (newLeft->ToString() == "")
    result = newRight->Copy();
  else if (newRight->ToString() == "")
    result = newLeft->Copy();
  else
    result = new MpsPar(*newLeft, *newRight, GetLeftFinal(), GetRightFinal());
  delete newLeft;
  delete newRight;
  return result;
} // }}}
string MpsPar::ToString(string indent) const // {{{
{
  string newIndent = indent + "  ";
  return (string)"( " + myLeft->ToString(newIndent) + "\n" + indent + "| " + myRight->ToString(newIndent) + "\n" + indent + ")";
} // }}}
string MpsPar::ToTex(int indent, int sw) const // {{{
{
  return (string)"( " + myLeft->ToTex(indent+2,sw) + "\\newline\n"
       + ToTex_Hspace(indent,sw) + "| " + myRight->ToTex(indent+2,sw) + "\\newline\n"
       + ToTex_Hspace(indent,sw) + ")";
} // }}}
string MpsPar::ToC() const // {{{
{
  stringstream result;
  string newName = ToC_Name(MpsExp::NewVar("fork")); // Create variable name foor the mmessagee to send
  result << "  IncAprocs();" << endl
         << "  int " << newName << "=fork();" << endl
         << "  if (" << newName << ">0)" << endl
         << "  {" << endl;
  for (vector<string>::const_iterator it=myLeftFinal.begin(); it!=myLeftFinal.end(); ++it) {
    result << "    " << ToC_Name(*it) << "->Close(false);" << endl
           << "    delete " << ToC_Name(*it) << ";" << endl;
  }
  result <<  myLeft->ToC()
         << "  }" << endl
         << "  else if (" << newName << "==0)" << endl
         << "  {" << endl;
  for (vector<string>::const_iterator it=myRightFinal.begin(); it!=myRightFinal.end(); ++it) {
    result << "    " << ToC_Name(*it) << "->Close(false);" << endl
           << "    delete " << ToC_Name(*it) << ";" << endl;
  }
  result <<  myRight->ToC()
         << "  }" << endl
         << "else throw (string)\"Error during fork!\";" << endl
         << "return new Cnt();" << endl;
  return result.str();
} // }}}
string MpsPar::ToCHeader() const // {{{
{
  stringstream result;
  result << myLeft->ToCHeader();
  result << myRight->ToCHeader();
  return result.str();
} // }}}
MpsTerm *MpsPar::RenameAll() const // {{{
{ MpsTerm *newLeft=myLeft->RenameAll();
  MpsTerm *newRight=myRight->RenameAll();
  MpsTerm *result=new MpsPar(*newLeft,*newRight, GetLeftFinal(), GetRightFinal());
  delete newLeft;
  delete newRight;
  return result;
} // }}}
bool MpsPar::Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const // {{{
{ MpsTerm *seqLeft = myLeft->Parallelize();
  MpsTerm *seqRight = myRight->Parallelize();
  seqTerm=new MpsPar(*seqLeft, *seqRight, GetLeftFinal(), GetRightFinal());
  delete seqLeft;
  delete seqRight;
  parTerm=receives.Append(*seqTerm);
  return false; // All optimizations are guarded
} // }}}
MpsTerm *MpsPar::Append(const MpsTerm &term) const // {{{
{ throw (string)"Error: Appending to parallell terms not implemented";
} // }}}
MpsTerm *MpsPar::CloseDefinitions() const // {{{
{
  MpsTerm *newLeft = myLeft->CloseDefinitions();
  MpsTerm *newRight = myRight->CloseDefinitions();

  MpsTerm *result=new MpsPar(*newLeft,*newRight, GetLeftFinal(), GetRightFinal());

  delete newLeft;
  delete newRight;

  return result;
} // }}}
MpsTerm *MpsPar::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ MpsTerm *newLeft=myLeft->ExtractDefinitions(env);
  MpsTerm *newRight=myRight->ExtractDefinitions(env);
  MpsTerm *result=new MpsPar(*newLeft,*newRight, GetLeftFinal(), GetRightFinal());
  delete newLeft;
  delete newRight;
  return result;
} // }}}
const vector<string> &MpsPar::GetLeftFinal() const // {{{
{ return myLeftFinal;
} // }}}
const vector<string> &MpsPar::GetRightFinal() const // {{{
{ return myRightFinal;
} // }}}
