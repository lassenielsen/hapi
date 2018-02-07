#include <hapi/mpslink.hpp>
#include <hapi/mpscond.hpp>
#include <hapi/mpsdef.hpp>
#include <hapi/mpscall.hpp>
#include <hapi/mpsend.hpp>
#include <hapi/mpspar.hpp>
#include <hapi/common.hpp>

using namespace std;
using namespace hapi;

MpsDef::MpsDef(const string &name, const std::vector<std::string> &args, const vector<MpsMsgType*> &types, const std::vector<std::string> &stateargs, const vector<MpsMsgType*> &statetypes, const MpsTerm &body, const MpsTerm &succ, bool pure) // {{{
: myName(name),
  myArgs(args),
  myStateArgs(stateargs),
  myPure(pure)
{
  myTypes.clear();
  for (vector<MpsMsgType*>::const_iterator type=types.begin(); type!=types.end(); ++type)
    myTypes.push_back((*type)->Copy());
  myStateTypes.clear();
  for (vector<MpsMsgType*>::const_iterator type=statetypes.begin(); type!=statetypes.end(); ++type)
    myStateTypes.push_back((*type)->Copy());
  // Assert body!=NULL
  // Assert succ!=NULL
  myBody = body.Copy();
  mySucc = succ.Copy();
} // }}}
MpsDef::~MpsDef() // {{{
{
  // assert mySucc != NULL
  // assert myBody != NULL
  DeleteVector(myTypes);
  DeleteVector(myStateTypes);
  delete mySucc;
  delete myBody;
} // }}}
void *MpsDef::TDCompileMain(tdc_pre pre, tdc_post wrap, tdc_error wrap_err, const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, const string &curPure, PureState pureState, bool checkPure) // * Use rule Def {{{
{ map<string,void*> children;
  PureState succState=pureState;
  PureState bodyState=myPure?CPS_PURE:CPS_IMPURE;

  if (checkPure) // Check purity constraints {{{
  { if (pureState!=CPS_IMPURE && pureState!=CPS_PURE && pureState!=CPS_SERVICE_DEF && pureState!=CPS_INIT_DEF)
      return wrap_err(this,PrintTypeError("Error in implementation of pure participant " + curPure + ". Pure implementations must conform with the structure \n     *   local X()\n    *   ( global s=new ch(p of n);\n     *     X();\n       *     |\n       *     P\n       *   )\n       *   local StartX(Int i)\n       *   ( if i<=0\n       *     then X();\n       *     else X(); | StartX(i-1);\n       *   )\n       *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);

    if (pureStack.size()>0)
      return wrap_err(this,PrintTypeError("Implementation of pure participant " + int2string(pureStack.begin()->second) + "@" + pureStack.begin()->first + " must be immediately after its decleration",*this,Theta,Gamma,Omega),children);

    if (pureState==CPS_INIT_DEF)
    { bodyState=CPS_INIT_BRANCH;
      succState=CPS_INIT_CALL;
    }
    else if (pureState==CPS_SERVICE_DEF)
    { bodyState=CPS_SERVICE_LINK;
      succState=CPS_INIT_DEF;
      // Test structure
      if (GetArgs().size()>0 || GetStateArgs().size()>0) // No args or state
        return wrap_err(this,PrintTypeError("Implementation of pure participant " + GetName() + " must have no arguments and state",*this,Theta,Gamma,Omega),children);

      // Body structure
      const MpsLink *bodyLink=dynamic_cast<const MpsLink*>(GetBody());
      if (bodyLink==NULL) // Link immediately
        return wrap_err(this,PrintTypeError("Implementation of pure participant " + GetName() + ". Pure implementations must start with linking as implemented participant.\n     *   local X()\n      !   ( global s=new ch(p of n);\n       *     X();\n       *     |\n       *     P\n       *   )\n       *   local StartX(Int i)\n       *   ( if i<=0\n       *     then X();\n       *     else X(); | StartX(i-1);\n       *   )\n       *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);
      const MpsPar *bodyPar=dynamic_cast<const MpsPar*>(bodyLink->GetSucc());
      if (bodyPar==NULL) // Fork after linking
        return wrap_err(this,PrintTypeError("Implementation of pure participant " + GetName() + ". Pure implementations must form imediately after linking.\n     *   local X()\n      *   ( global s=new ch(p of n);\n       *     X();\n       !     |\n       *     P\n       *   )\n       *   local StartX(Int i)\n       *   ( if i<=0\n       *     then X();\n       *     else X(); | StartX(i-1);\n       *   )\n       *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);
      const MpsCall *bodyCall=dynamic_cast<const MpsCall*>(bodyPar->GetLeft());
      if (bodyCall==NULL || bodyCall->GetName()!=GetName()) // Call immediately (after fork)
        return wrap_err(this,PrintTypeError("Implementation of pure participant " + GetName() + ". Pure implementations must reinitiate service immediately after linking and forking.\n     *   local X()\n      *   ( global s=new ch(p of n);\n       !     X();\n       *     |\n       *     P\n       *   )\n       *   local StartX(Int i)\n       *   ( if i<=0\n       *     then X();\n       *     else X(); | StartX(i-1);\n       *   )\n       *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);

      // Init structure
      // local pure StartX(Int i)
      const MpsDef *initDef=dynamic_cast<const MpsDef*>(GetSucc());
      if (initDef==NULL) // Define initiating process
        return wrap_err(this,PrintTypeError("Initialization of service " + GetName() + ". Pure implementations must use pure def to initiate service.\n     *   local X()\n      *   ( global s=new ch(p of n);\n       *     X();\n       *     |\n       *     P\n       *   )\n       !   local StartX(Int i)\n       *   ( if i<=0\n       *     then X();\n       *     else X(); | StartX(i-1);\n       *   )\n       *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);
     if (initDef->GetStateArgs().size()!=0)
        return wrap_err(this,PrintTypeError("Initialization of service " + GetName() + ". Pure implementations pure def to must have no state.\n     *   local X()\n      *   ( global s=new ch(p of n);\n       *     X();\n       *     |\n       *     P\n       *   )\n       !   local StartX(Int i)\n       *   ( if i<=0\n       *     then X();\n       *     else X(); | StartX(i-1);\n       *   )\n       *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);
     if (initDef->GetTypes().size()!=1 || dynamic_cast<const MpsIntMsgType*>(initDef->GetTypes()[0])==NULL)
        return wrap_err(this,PrintTypeError("Initialization of service " + GetName() + ". Pure implementations pure def must have exactly one argument of type Int.\n     *   local X()\n      *   ( global s=new ch(p of n);\n       *     X();\n       *     |\n       *     P\n       *   )\n       !   local StartX(Int i)\n       *   ( if i<=0\n       *     then X();\n       *     else X(); | StartX(i-1);\n       *   )\n       *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);

      const MpsCond *initCond=dynamic_cast<const MpsCond*>(initDef->GetBody());
      if (initCond==NULL) // if ...
        return wrap_err(this,PrintTypeError("Initialization of service " + GetName() + ". Pure implementation must branch.\n     *   local X()\n      *   ( global s=new ch(p of n);\n       *     X();\n       *     |\n       *     P\n       *   )\n       *   local StartX(Int i)\n       !   ( if i<=0\n       *     then X();\n       *     else X(); | StartX(i-1);\n       *   )\n       *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);
      const MpsBinOpExp *initCondExp=dynamic_cast<const MpsBinOpExp*>(initCond->GetCond());
      if (initCondExp==NULL || initCondExp->GetOp()!="<=" || initCondExp->GetLeft().ToString()!=initDef->GetArgs()[0] || initCondExp->GetRight().ToString()!="0")
        return wrap_err(this,PrintTypeError("Initialization of service " + GetName() + ". Pure implementation initialization uses wrong expression for branching.\n     *   local X()\n      *   ( global s=new ch(p of n);\n       *     X();\n       *     |\n       *     P\n       *   )\n       *   local StartX(Int i)\n       !   ( if i<=0\n       *     then X();\n       *     else X(); | StartX(i-1);\n       *   )\n       *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);

      const MpsCall *initThenCall=dynamic_cast<const MpsCall*>(initCond->GetTrueBranch());
      if (initThenCall==NULL || initThenCall->GetName()!=GetName() )
        return wrap_err(this,PrintTypeError("Purity error, expected bekow structure.\n     *   local X()\n      *   ( global s=new ch(p of n);\n       *     X();\n       *     |\n       *     P\n       *   )\n       *   local StartX(Int i)\n       *   ( if i<=0\n       !     then X();\n       *     else X(); | StartX(i-1);\n       *   )\n       *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);

      const MpsPar *initElsePar=dynamic_cast<const MpsPar*>(initCond->GetFalseBranch());
      if (initElsePar==NULL )
        return wrap_err(this,PrintTypeError("Purity error, expected bekow structure.\n     *   local X()\n      *   ( global s=new ch(p of n);\n       *     X();\n       *     |\n       *     P\n       *   )\n       *   local StartX(Int i)\n       *   ( if i<=0\n       *     then X();\n       !     else X(); | StartX(i-1);\n       *   )\n       *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);

      const MpsCall *initElseCall1=dynamic_cast<const MpsCall*>(initElsePar->GetLeft());
      if (initElseCall1==NULL || initElseCall1->GetName()!=GetName() )
        return wrap_err(this,PrintTypeError("Purity error, expected bekow structure.\n     *   local X()\n      *   ( global s=new ch(p of n);\n       *     X();\n       *     |\n       *     P\n       *   )\n       *   local StartX(Int i)\n       *   ( if i<=0\n       *     then X();\n       !     else X(); | StartX(i-1);\n       *   )\n       *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);

      const MpsCall *initElseCall2=dynamic_cast<const MpsCall*>(initElsePar->GetRight());
      if (initElseCall2==NULL || initElseCall2->GetName()!=initDef->GetName() )
        return wrap_err(this,PrintTypeError("Purity error, expected bekow structure.\n     *   local X()\n      *   ( global s=new ch(p of n);\n       *     X();\n       *     |\n       *     P\n       *   )\n       *   local StartX(Int i)\n       *   ( if i<=0\n       *     then X();\n       !     else X(); | StartX(i-1);\n       *   )\n       *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);

      // Test no open sessions
      for (MpsMsgEnv::const_iterator var = Gamma.begin(); var!=Gamma.end(); ++var)
        if (dynamic_cast<const MpsDelegateMsgType*>(var->second)!=NULL) // Session type
          return wrap_err(this,PrintTypeError("Implementation of pure participant " + GetName() + " uses open session " + var->first + " breaking purity." ,*this,Theta,Gamma,Omega),children);
    }
  } // }}}
  // Verify def
  // Check if def is sound
  if (myArgs.size() != myTypes.size())
    return wrap_err(this,PrintTypeError((string)"Bad def: difference in number of arguments and number of types",*this,Theta,Gamma,Omega),children);
  if (myStateArgs.size() != myStateTypes.size())
    return wrap_err(this,PrintTypeError((string)"Bad def: difference in number of state arguments and number of state types",*this,Theta,Gamma,Omega),children);
  // Make new process-environment
  MpsProcEnv newOmega = Omega;
  newOmega[myName].pure = myPure;
  newOmega[myName].types = myTypes;
  newOmega[myName].snames = myStateArgs;
  for (vector<MpsMsgType*>::const_iterator it=myStateTypes.begin(); it!=myStateTypes.end(); ++it)
    if (dynamic_cast<const MpsBoolMsgType*>(*it)==NULL)
      return wrap_err(this,PrintTypeError((string)"State argument of unsupported type: " + (*it)->ToString(),*this,Theta,Gamma,Omega),children);
  newOmega[myName].stypes = myStateTypes;
  // Make new environments for body
  MpsMsgEnv newGamma;
  for (MpsMsgEnv::const_iterator var = Gamma.begin(); var!=Gamma.end(); ++var)
    if (dynamic_cast<const MpsDelegateMsgType*>(var->second)==NULL) // Not session type
      newGamma[var->first]=var->second;
  // Update environments from state-arguments
  set<string> usedArgs;
  for (int i=0; i<myStateArgs.size() /* && i<myStateTypes.size()*/; ++i)
  { // Only type bool already verified
    newGamma[myStateArgs[i]]=myStateTypes[i];
    usedArgs.insert(myStateArgs[i]);
  }
  // Update environments from arguments
  for (int i=0; i<myArgs.size() /* && i<myTypes.size()*/; ++i)
  {
    if (usedArgs.find(myArgs[i]) != usedArgs.end())
      return wrap_err(this,PrintTypeError((string)"Bad def: cannot type multiple input of channel: " + myArgs[i],*this,Theta,Gamma,Omega),children);
    newGamma[myArgs[i]]=myTypes[i];
    usedArgs.insert(myArgs[i]);
  }
  // Make subcalls
  children["succ"]=mySucc->TDCompile(pre,wrap,wrap_err,Theta,Gamma,newOmega,pureStack,curPure, succState, checkPure);
  children["body"]=myBody->TDCompile(pre,wrap,wrap_err,Theta,newGamma,newOmega,pureStack,curPure, bodyState, checkPure);
  // Wrap result
  return wrap(this,Theta,Gamma,Omega,pureStack,curPure,pureState,checkPure,children);
} // }}}
MpsTerm *MpsDef::ApplyDef(const std::string &path, std::vector<MpsFunction> &dest) const // {{{
{ if (path.size()>0)
    return Error((string)"Applying Def on "+ToString()+" with nonempty pathh"+path);
  string name = NewName(myName);
  MpsTerm *newBody=myBody->PRename(myName,name);
  MpsFunction newdef(name,myStateArgs,myStateTypes,myArgs,myTypes,GetArgPids(),*newBody);
  dest.push_back(newdef);
  delete newBody;
  return mySucc->PRename(myName,name);
} // }}}
bool MpsDef::SubSteps(vector<MpsStep> &dest) // {{{
{
  // Make empty event
  MpsEvent event;
  event.myType = tau;
  event.mySubType = fundef;

  // Add step
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event, paths));

  return false;
} // }}}
MpsTerm *MpsDef::PRename(const string &src, const string &dst) const // {{{
{
  if (src == myName)
    return Copy();
  
  MpsTerm *newBody = myBody->PRename(src,dst);
  MpsTerm *newSucc = mySucc->PRename(src,dst);
  MpsTerm *result = new MpsDef(myName,myArgs,myTypes,myStateArgs,myStateTypes,*newBody,*newSucc, myPure);
  delete newBody;
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsDef::ERename(const string &src, const string &dst) const // {{{
{
  MpsTerm *newSucc = mySucc->ERename(src,dst); // Succ is substituted immediately
  MpsTerm *newBody = myBody->Copy(); // Body is substituted afer possible argument renamings

  bool body_hidden=false;

  // Process StateArgs
  vector<string> newStateArgs;
  for (vector<string>::const_iterator arg=myStateArgs.begin(); arg!=myStateArgs.end(); ++arg)
  { if (src==*arg)
    { body_hidden=true;
      newStateArgs.push_back(*arg);
    }
    else if (dst==*arg) // rename arg in body
    { string newArg = MpsExp::NewVar(*arg);
      MpsTerm *tmpBody = newBody->ERename(*arg,newArg);
      delete newBody;
      newBody=tmpBody;
      newStateArgs.push_back(newArg);
    }
    else
      newStateArgs.push_back(*arg);
  }

  // Process Args
  vector<string> newArgs;
  for (vector<string>::const_iterator arg=myArgs.begin(); arg!=myArgs.end(); ++arg)
  { if (src==*arg)
    { body_hidden=true;
      newArgs.push_back(*arg);
    }
    else if (dst==*arg) // rename arg in body
    { string newArg = MpsExp::NewVar(*arg);
      MpsTerm *tmpBody = newBody->ERename(*arg,newArg);
      delete newBody;
      newBody=tmpBody;
      newArgs.push_back(newArg);
    }
    else
      newArgs.push_back(*arg);
  }
      
  if (not body_hidden)
  { MpsTerm *tmpBody = newBody->ERename(src,dst);
    delete newBody;
    newBody=tmpBody;
  }

  // Create result
  MpsTerm *result = new MpsDef(myName,newArgs,myTypes,newStateArgs,myStateTypes,*newBody,*newSucc,myPure);

  delete newBody;
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsDef::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  MpsTerm *newSucc = mySucc->ReIndex(session,pid,maxpid); // Succ is substituted immediately
  MpsTerm *newBody = myBody->Copy(); // Body is substituted afer possible argument renamings

  bool body_hidden=false;

  // Process StateArgs
  for (vector<string>::const_iterator arg=myStateArgs.begin(); arg!=myStateArgs.end(); ++arg)
  { if (session==*arg)
      body_hidden=true;
  }

  // Process Args
  for (vector<string>::const_iterator arg=myArgs.begin(); arg!=myArgs.end(); ++arg)
  { if (session==*arg)
      body_hidden=true;
  }
      
  if (not body_hidden)
  { MpsTerm *tmpBody = newBody->ReIndex(session,pid,maxpid);
    delete newBody;
    newBody=tmpBody;
  }

  MpsTerm *result = new MpsDef(myName,myArgs,myTypes,myStateArgs,myStateTypes,*newBody,*newSucc,myPure);
  delete newBody;
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsDef::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
{
  // assert(mySucc != NULL);
  // assert myName not in exp.FPV()
  if (var == myName)
    return Copy();

  string newName = myName;
  MpsTerm *newSucc = NULL;
  MpsTerm *newBody = NULL;

  // Check if renaming of myName is necessary
  set<string> fpv=exp.FPV();
  if (fpv.find(newName) != fpv.end()) // Must rename myName
  {
    newName = NewName();
    vector<MpsExp*> tmpArgs;
    tmpArgs.clear();
    for (vector<string>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
      tmpArgs.push_back(new MpsVarExp(*it,MpsMsgNoType()));
    vector<MpsExp*> tmpState;
    tmpState.clear();
    for (vector<string>::const_iterator it=myStateArgs.begin(); it!=myStateArgs.end(); ++it)
      tmpState.push_back(new MpsVarExp(*it,MpsMsgNoType()));
    MpsTerm *newVar = new MpsCall(newName,tmpArgs,tmpState,myTypes,myStateTypes);
    DeleteVector(tmpArgs);
    DeleteVector(tmpState);
    newSucc = mySucc->PSubst(myName,*newVar,myArgs,GetArgPids(),myStateArgs); // Initialise newSucc
    newBody = myBody->PSubst(myName,*newVar,myArgs,GetArgPids(),myStateArgs); // Initialise newBody
    delete newVar;
  }
  else // Renaming of myName not necessary
  {
    newSucc = mySucc->Copy(); // Initialise newSucc
    newBody = myBody->Copy(); // Initialise newBody
  }

  set<string> fev=exp.FEV();
  vector<string> newArgs;
  newArgs.clear();
  for (vector<string>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  {
    if (fev.find(*it) != fev.end()) // Must rename argument
    {
      string newArg = MpsExp::NewVar(); // Make new name
      MpsExp *newVar = new MpsVarExp(newArg,MpsMsgNoType());
      MpsTerm *tmpBody = newBody->ESubst(*it,*newVar); // Rename argument
      delete newVar;
      delete newBody;
      newBody = tmpBody;
      newArgs.push_back(newArg); // Add the argument
    }
    else
      newArgs.push_back(*it); // Add the argument
  }
  { // Make substitution
    MpsTerm *tmpSucc = newSucc->PSubst(var,exp,args,argpids,stateargs);
    delete newSucc;
    newSucc = tmpSucc;

    MpsTerm *tmpBody = newBody->PSubst(var,exp,args,argpids,stateargs);
    delete newBody;
    newBody = tmpBody;
  }
  // Make result
  MpsTerm *result = new MpsDef(newName, newArgs, myTypes, myStateArgs, myStateTypes, *newBody, *newSucc, myPure);
  delete newBody;
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsDef::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  // assert(mySucc != NULL);
  // Renaming myName not necessary, as dest cannot contain process variables
  string newName = myName; // Copy name
  MpsTerm *newSucc = mySucc->ESubst(source,dest); // Succ is substituted immediately
  MpsTerm *newBody = myBody->Copy(); // Body is substituted afer possible argument renamings
  vector<MpsMsgType*> newTypes;
  for (vector<MpsMsgType*>::const_iterator it=myTypes.begin(); it!=myTypes.end(); ++it)
    newTypes.push_back((*it)->ESubst(source,dest));
  vector<MpsMsgType*> newStateTypes;
  for (vector<MpsMsgType*>::const_iterator it=myStateTypes.begin(); it!=myStateTypes.end(); ++it)
    newStateTypes.push_back((*it)->ESubst(source,dest));

  MpsTerm *result = NULL; // Find result
  // vector<string>::find
  bool found=false;
  for (vector<string>::const_iterator it=myStateArgs.begin(); it!=myStateArgs.end(); ++it)
    if (*it == source)
      found=true;
  for (vector<string>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
    if (*it == source)
      found=true;
  if (found) // Do not substitute in body
    result = new MpsDef(newName, myArgs, newTypes, myStateArgs, newStateTypes, *newBody, *newSucc, myPure);
  else
  {
    vector<string> newStateArgs; // Find new state arguments, and rename if necessary
    vector<string> newArgs; // Find new arguments, and rename if necessary
    set<string> fv=dest.FV();
    for (vector<string>::const_iterator it=myStateArgs.begin(); it!=myStateArgs.end(); ++it)
    {
      if (fv.find(*it)!=fv.end()) //Argument must be renamed
      {
        string newArg = MpsExp::NewVar(*it); // Make unused name
        MpsTerm *tmpBody = newBody->ERename(*it,newArg); // Replace old variable with new one
        delete newBody;
        newBody=tmpBody;
        newStateArgs.push_back(newArg); // Add argument name
      }
      else
        newStateArgs.push_back(*it); // Add argument name
    }
    for (vector<string>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
    {
      if (fv.find(*it)!=fv.end()) //Argument must be renamed
      {
        string newArg = MpsExp::NewVar(*it); // Make unused name
        MpsTerm *tmpBody = newBody->ERename(*it,newArg); // Replace old variable with new one
        delete newBody;
        newBody=tmpBody;
        newArgs.push_back(newArg); // Add argument name
      }
      else
        newArgs.push_back(*it); // Add argument name
    }
    MpsTerm *tmpBody = newBody->ESubst(source,dest); // Make substitution in body
    delete newBody;
    newBody=tmpBody;
    result = new MpsDef(newName, newArgs, newTypes, newStateArgs, newStateTypes, *newBody, *newSucc, myPure);
  }
  delete newBody;
  delete newSucc;
  DeleteVector(newTypes);
  DeleteVector(newStateTypes);
  return result;
} // }}}
MpsTerm *MpsDef::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->GSubst(source,dest,args);
  MpsTerm *newBody = myBody->GSubst(source,dest,args);

  vector<MpsMsgType*> newStateTypes;
  newStateTypes.clear();
  for (vector<MpsMsgType*>::const_iterator type=myStateTypes.begin(); type!=myStateTypes.end(); ++type)
    newStateTypes.push_back((*type)->GSubst(source,dest,args));
  vector<MpsMsgType*> newTypes;
  newTypes.clear();
  for (vector<MpsMsgType*>::const_iterator type=myTypes.begin(); type!=myTypes.end(); ++type)
    newTypes.push_back((*type)->GSubst(source,dest,args));
  
  MpsTerm *result = new MpsDef(myName,myArgs,newTypes,myStateArgs,newStateTypes,*newBody,*newSucc,myPure);

  // Clean Up
  DeleteVector(newStateTypes);
  DeleteVector(newTypes);
  delete newBody;
  delete newSucc;

  return result;
} // }}}
MpsTerm *MpsDef::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  MpsTerm *newSucc = mySucc->LSubst(source,dest,args);
  MpsTerm *newBody = myBody->LSubst(source,dest,args);

  vector<MpsMsgType*> newStateTypes;
  newStateTypes.clear();
  for (vector<MpsMsgType*>::const_iterator type=myStateTypes.begin(); type!=myStateTypes.end(); ++type)
    newStateTypes.push_back((*type)->LSubst(source,dest,args));
  vector<MpsMsgType*> newTypes;
  newTypes.clear();
  for (vector<MpsMsgType*>::const_iterator type=myTypes.begin(); type!=myTypes.end(); ++type)
    newTypes.push_back((*type)->LSubst(source,dest,args));
  
  MpsTerm *result = new MpsDef(myName,myArgs,newTypes,myStateArgs,newStateTypes,*myBody,*mySucc,myPure);

  // Clean Up
  DeleteVector(newStateTypes);
  DeleteVector(newTypes);
  delete newBody;
  delete newSucc;

  return result;
} // }}}
set<string> MpsDef::FPV() const // {{{
{
  set<string> result = mySucc->FPV();
  set<string> result2 = myBody->FPV();
  result.insert(result2.begin(),result2.end());
  result.erase(myName);
  return result;
} // }}}
set<string> MpsDef::EV() const // {{{
{
  set<string> result = mySucc->EV();
  set<string> result2 = myBody->EV();
  result.insert(result2.begin(),result2.end());
  for (vector<string>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
    result.insert(*it);
  return result;
} // }}}
set<string> MpsDef::FEV() const // {{{
{
  set<string> result = mySucc->FEV();
  set<string> result2 = myBody->FEV();
  for (vector<string>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
    result2.erase(*it);
  result.insert(result2.begin(),result2.end());
  return result;
} // }}}
MpsTerm *MpsDef::Copy() const // {{{
{
  return new MpsDef(myName, myArgs, myTypes, myStateArgs, myStateTypes, *myBody, *mySucc, myPure);
} // }}}
bool MpsDef::Terminated() const // {{{
{
  return false;
} // }}}
MpsTerm *MpsDef::Simplify() const // {{{
{
  // assert mySucc != NULL
  MpsTerm *newSucc = mySucc->Simplify();
  MpsTerm *newBody = myBody->Simplify();
  MpsTerm *result=NULL;
  if (newSucc->ToString() == "end")
    result = new MpsEnd();
  else
    result = new MpsDef(myName, myArgs, myTypes, myStateArgs, myStateTypes, *newBody, *newSucc, myPure);
  delete newSucc;
  delete newBody;
  return result;
} // }}}
string MpsDef::ToString(string indent) const // {{{
{
  string newIndent = indent + "  ";
  string typeIndent = indent + "       ";
  for (int i=0; i<myName.size();++i)
    typeIndent+=" ";
  string result = (string)"local " + myName;
  if (myStateArgs.size()>0)
  {
    result += "<";
    for (int i=0; i<myStateArgs.size() && i<myStateTypes.size(); ++i)
    {
      if (i>0)
        result += ",\n" + typeIndent;
      result += myStateTypes[i]->ToString(typeIndent) + " " + myStateArgs[i];
    }
    result += ">\n" + typeIndent.substr(1);
  }
  result += "(";
  for (int i=0; i<myArgs.size() && i<myTypes.size(); ++i)
  {
    if (i>0)
      result += ",\n" + typeIndent;
    result += myTypes[i]->ToString(typeIndent) + " " + myArgs[i];
  }
  result += ")";
  result += (string)"\n" + indent + "( " + myBody->ToString(newIndent);
  result += (string)"\n" + indent + ")";
  result += (string)"\n" + indent + mySucc->ToString(newIndent);
  return result;
} // }}}
string MpsDef::ToTex(int indent, int sw) const // {{{
{
  int typeIndent = indent + 5 + myName.size();
  string result = ToTex_KW("def") + " " + ToTex_Var(myName);
  if (myStateArgs.size()>0)
  {
    result += "$\\langle$";
    for (int i=0; i<myStateArgs.size() && i<myStateTypes.size(); ++i)
    {
      if (i>0)
        result += ",\\newline\n" + ToTex_Hspace(typeIndent,sw);
      int newTypeIndent = typeIndent + 2 + myStateArgs[i].size();
      result += myStateArgs[i] + ": " + myStateTypes[i]->ToTex(newTypeIndent,sw);
    }
    result += "$\\rangle$\\newline\n" + ToTex_Hspace(typeIndent-1,sw);
  }
  if (myArgs.size()>0)
  {
    result += "(";
    for (int i=0; i<myArgs.size() && i<myTypes.size(); ++i)
    {
      if (i>0)
        result += ",\\newline\n" + ToTex_Hspace(typeIndent,sw);
      int newTypeIndent = typeIndent + 2 + myArgs[i].size();
      result += myArgs[i] + ": " + myTypes[i]->ToTex(newTypeIndent,sw);
    }
    result += ")";
  }
  result += (string)"=\\newline\n"
          + ToTex_Hspace(indent+2,sw) + myBody->ToTex(indent+2,sw) + "\\newline\n"
          + ToTex_Hspace(indent,sw) + ToTex_KW("in") + " " + mySucc->ToTex(indent+3,sw);
  return result;
} // }}}
string MpsDef::ToC(const string &taskType) const // {{{
{
  throw (string)"MpsDef::ToC(): Local definitions not supported in compilation, must be moved to global procedure";
} // }}}
string MpsDef::ToCHeader() const // {{{
{
  stringstream result;
  result << myBody->ToCHeader();
  result << mySucc->ToCHeader();
  return result.str();
} // }}}
void MpsDef::ToCConsts(std::vector<std::string> &dest, std::unordered_set<std::string> &existing) const // {{{
{
  myBody->ToCConsts(dest,existing);
  mySucc->ToCConsts(dest,existing);
} // }}}
MpsTerm *MpsDef::FlattenFork(bool normLhs, bool normRhs, bool pureMode) const // {{{
{ MpsTerm *flatSucc = mySucc->FlattenFork(normLhs,normRhs,pureMode);
  MpsTerm *flatBody = myBody->FlattenFork(normLhs,normRhs,pureMode||myPure);
  MpsTerm *flatTerm=new MpsDef(myName, myArgs, myTypes, myStateArgs, myStateTypes, *flatBody, *flatSucc, myPure);
  delete flatSucc;
  delete flatBody;
  return flatTerm;
} // }}}
MpsTerm *MpsDef::RenameAll() const // {{{
{ string newName=MpsTerm::NewName(myName);
  // Create new statearg names
  vector<string> newStateArgs;
  for (vector<string>::const_iterator it=myStateArgs.begin(); it!=myStateArgs.end(); ++it)
    newStateArgs.push_back(MpsExp::NewVar(*it));
  // Create new statearg types
  vector<MpsMsgType*> newStateTypes;
  for (vector<MpsMsgType*>::const_iterator it=myStateTypes.begin(); it!=myStateTypes.end(); ++it)
    newStateTypes.push_back((*it)->RenameAll());
  // Create new arg names
  vector<string> newArgs;
  for (vector<string>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
    newArgs.push_back(MpsExp::NewVar(*it));
  // Create new state types
  vector<MpsMsgType*> newTypes;
  for (vector<MpsMsgType*>::const_iterator it=myTypes.begin(); it!=myTypes.end(); ++it)
    newTypes.push_back((*it)->RenameAll());
  // Create new body
  MpsTerm *tmpBody=myBody->PRename(myName,newName);
  MpsTerm *newBody=tmpBody->RenameAll();
  delete tmpBody;
  for (int i=0;i<myStateArgs.size();++i)
  { MpsTerm *tmpBody=newBody->ERename(myStateArgs[i],newStateArgs[i]);
    delete newBody;
    newBody=tmpBody;
  }
  for (int i=0;i<myArgs.size();++i)
  { MpsTerm *tmpBody=newBody->ERename(myArgs[i],newArgs[i]);
    delete newBody;
    newBody=tmpBody;
  }
  // Create new succ
  MpsTerm *tmpSucc=mySucc->PRename(myName,newName);
  MpsTerm *newSucc=tmpSucc->RenameAll();
  delete tmpSucc;

  MpsTerm *result=new MpsDef(newName,
                             newArgs,
                             newTypes,
                             newStateArgs,
                             newStateTypes,
                             *newBody,
                             *newSucc,
                             myPure);

  // Cleanup
  delete newSucc;
  delete newBody;
  DeleteVector(newTypes);
  DeleteVector(newStateTypes);

  return result;
} // }}}
bool MpsDef::Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const // {{{
{ MpsTerm *seqSucc = mySucc->Parallelize();
  MpsTerm *seqBody = myBody->Parallelize();
  seqTerm=new MpsDef(myName, myArgs, myTypes, myStateArgs, myStateTypes, *seqBody, *seqSucc, myPure);
  delete seqSucc;
  delete seqBody;
  parTerm=receives.Append(*seqTerm);
  return false; // All optimizations are guarded
} // }}}
MpsTerm *MpsDef::Append(const MpsTerm &term) const // {{{
{ MpsTerm *newSucc=mySucc->Append(term);
  MpsTerm *result=new MpsDef(myName, myArgs, myTypes, myStateArgs, myStateTypes, *myBody, *newSucc, myPure);
  delete newSucc;
  return result;
} // }}}
MpsTerm *MpsDef::CopyWrapper(std::map<std::string,void*> &children) const // {{{
{
  MpsTerm *result=new MpsDef(myName, myArgs, myTypes, myStateArgs, myStateTypes, *(MpsTerm*)children["body"], *(MpsTerm*)children["succ"], myPure);
  return result;
} // }}}
MpsTerm *MpsDef::CloseDefsPre(const MpsMsgEnv &Gamma) // {{{
{ // Copy current args and types
  vector<string> newArgs=myArgs;
  vector<MpsMsgType*> newTypes;
  for (vector<MpsMsgType*>::const_iterator t=myTypes.begin(); t!=myTypes.end(); ++t)
    newTypes.push_back((*t)->Copy());

  // Find missing args
  set<string> fvs=myBody->FEV();
  for (vector<string>::const_iterator bv=myStateArgs.begin(); bv!=myStateArgs.end(); ++bv)
    fvs.erase(*bv);
  for (vector<string>::const_iterator bv=myArgs.begin(); bv!=myArgs.end(); ++bv)
    fvs.erase(*bv);
  // Add missing args, and their types from Gamma
  for (set<string>::const_iterator fv=fvs.begin(); fv!=fvs.end(); ++fv)
  { newArgs.push_back(*fv);
    MpsMsgEnv::const_iterator gt=Gamma.find(*fv);
    if (gt==Gamma.end())
      newTypes.push_back(new MpsMsgNoType());
    else
      newTypes.push_back(gt->second->Copy());
  }

  // Create call term for substitution
  vector<MpsExp*> callArgs;
  for (vector<string>::const_iterator it=newArgs.begin(); it!=newArgs.end(); ++it)
    callArgs.push_back(new MpsVarExp(*it,MpsMsgNoType()));
  vector<MpsExp*> callStateArgs;
  for (vector<string>::const_iterator it=myStateArgs.begin(); it!=myStateArgs.end(); ++it)
    callStateArgs.push_back(new MpsVarExp(*it,MpsMsgNoType()));
  MpsTerm *newCall = new MpsCall(myName,callArgs,callStateArgs,newTypes,myStateTypes);
  DeleteVector(callArgs);
  DeleteVector(callStateArgs);
  // Create new succ
  MpsTerm *newSucc = mySucc->PSubst(myName,*newCall,myArgs,GetArgPids(),myStateArgs);
  // Create new body
  MpsTerm *newBody = myBody->PSubst(myName,*newCall,myArgs,GetArgPids(),myStateArgs);
  // Create result
  MpsTerm *result=new MpsDef(myName,newArgs,newTypes,myStateArgs,myStateTypes,*newBody,*newSucc,myPure);

  delete newBody;
  delete newSucc;
  DeleteVector(newTypes);
  
  return result;
} // }}}
MpsTerm *MpsDef::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ // This is where the action happens!
  // Remove definitions from body, so it is definition free
  MpsTerm *newBody = myBody->ExtractDefinitions(env);
  // Add definition with cleaned body to global env
  env.push_back(MpsFunction(myName,myStateArgs,myStateTypes,myArgs,myTypes,GetArgPids(),*newBody));
  // Clean up
  delete newBody;
  // Return term without local definition
  return mySucc->ExtractDefinitions(env);
} // }}}
vector<pair<int,int> > MpsDef::GetArgPids(const std::vector<MpsMsgType*> &argTypes) // {{{
{
  vector<pair<int,int> > argPids; // Build argPid vector for function def.
  for (vector<MpsMsgType*>::const_iterator argType=argTypes.begin();
       argType!=argTypes.end(); ++argType)
  { const MpsDelegateMsgType *delType=dynamic_cast<const MpsDelegateMsgType*>(*argType);
    if (delType!=NULL)
      argPids.push_back(pair<int,int>(delType->GetPid(), delType->GetMaxpid()));
    else
      argPids.push_back(pair<int,int>(0,0));
  }
  return argPids;
} // }}}
vector<pair<int,int> > MpsDef::GetArgPids() const // {{{
{ return GetArgPids(myTypes);
} // }}}
