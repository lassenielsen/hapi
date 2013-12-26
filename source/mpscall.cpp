#include<apims/mpscall.hpp>
#include<apims/mpsend.hpp>
#include "common.cpp"

using namespace std;
using namespace apims;

MpsCall::MpsCall(const string &name, const vector<MpsExp*> &args, const vector<MpsExp*> &state, const vector<MpsMsgType*> &types, const vector<MpsMsgType*> &statetypes) // {{{
: myName(name)
{
  myArgs.clear();
  for (vector<MpsExp*>::const_iterator it=args.begin(); it!=args.end(); ++it)
    myArgs.push_back((*it)->Copy());
  myState.clear();
  for (vector<MpsExp*>::const_iterator it=state.begin(); it!=state.end(); ++it)
    myState.push_back((*it)->Copy());
  myTypes.clear();
  for (vector<MpsMsgType*>::const_iterator it=types.begin(); it!=types.end(); ++it)
    myTypes.push_back((*it)->Copy());
  myStateTypes.clear();
  for (vector<MpsMsgType*>::const_iterator it=statetypes.begin(); it!=statetypes.end(); ++it)
    myStateTypes.push_back((*it)->Copy());
} // }}}
MpsCall::~MpsCall() // {{{
{
  DeleteVector(myArgs);
  DeleteVector(myState);
  DeleteVector(myTypes);
  DeleteVector(myStateTypes);
} // }}}
bool MpsCall::TypeCheck(const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega) // * Use rule Var {{{
{
  // Check variable is defined
  MpsProcEnv::const_iterator omega = Omega.find(myName);
  if (omega == Omega.end())
    return PrintTypeError((string)"Process Variable not defined: " + myName,*this,Theta,Gamma,Omega);
  // Check correct number of arguments
  if (omega->second.stypes.size() != myState.size() ||
      omega->second.stypes.size() != myState.size() ||
      omega->second.types.size() != myArgs.size())
    return PrintTypeError((string)"Process Variable wrong argument-count: " + myName,*this,Theta,Gamma,Omega);
  // Check argument-types and remove used sessions from endGamma
  MpsMsgEnv endGamma=Gamma;
  DeleteVector(myStateTypes);
  for (int i=0;i<myState.size();++i)
  {
    MpsMsgType *statetype = myState[i]->TypeCheck(Gamma);
#if APIMS_DEBUG_LEVEL>99
    cerr << ">>>>Comparing: " << endl
         << ">>>>>>>>Theta: " << Theta.ToString() << endl
         << ">>>>>>>>>>LHS: " << statetype->ToString(">>>>>>>>>>>>>>") << endl
         << ">>>>>>>>>>RHS: " << omega->second.stypes[i]->ToString(">>>>>>>>>>>>>>") << endl;
#endif
    bool statetypematch = statetype->Equal(Theta,*omega->second.stypes[i]);
    delete statetype;    
    if (!statetypematch)
      return PrintTypeError((string)"State argument does not have type: " + omega->second.stypes[i]->ToString(),*this,Theta,Gamma,Omega);
    // Store type for compilation
    myStateTypes.push_back(omega->second.stypes[i]->Copy());
  }
  DeleteVector(myTypes);
  for (int i=0;i<myArgs.size();++i)
  {
    MpsMsgType *argType = myArgs[i]->TypeCheck(endGamma);
    MpsMsgType *callType = omega->second.types[i]->Copy();
    for (int j=0; j<myState.size(); ++j)
    {
      MpsMsgType *tmpType=callType->ESubst(omega->second.snames[j],*myState[j]);
      delete callType;
      callType=tmpType;
    }
#if APIMS_DEBUG_LEVEL>99
    cerr << ">>>>Comparing: " << endl
         << ">>>>>>>>Theta: " << Theta.ToString() << endl
         << ">>>>>>>>>>LHS: " << argType->ToString(">>>>>>>>>>>>>>") << endl
         << ">>>>>>>>>>RHS: " << callType->ToString(">>>>>>>>>>>>>>") << endl;
#endif
    bool argtypematch = argType->Equal(Theta,*callType);
    string callTypeString = callType->ToString();

    // Store type for compilation
    myTypes.push_back(callType->Copy());
    delete argType;
    delete callType;
    if (not argtypematch)
      return PrintTypeError((string)"Argument does not have type: " + callTypeString,*this,Theta,Gamma,Omega);
    if (dynamic_cast<MpsDelegateMsgType*>(omega->second.types[i]) != NULL)
    {
      if (typeid(*myArgs[i]) != typeid(MpsVarExp))
        return PrintTypeError((string)"Argument must be session: " + myArgs[i]->ToString(),*this,Theta,Gamma,Omega);
      else
      {
        MpsVarExp *var=(MpsVarExp*)myArgs[i];
        MpsMsgEnv::iterator session=endGamma.find(var->ToString());
        if (session == endGamma.end())
          return PrintTypeError((string)"Argument session not defined or used more than once: " + var->ToString(),*this,Theta,Gamma,Omega);
        // Remove the used session (linearity)
        endGamma.erase(session);
      }
    }
  }
  // Check that endGamma is completed
  for (MpsMsgEnv::const_iterator var=endGamma.begin();var!=endGamma.end();++var)
  { const MpsDelegateMsgType *session=dynamic_cast<const MpsDelegateMsgType*>(var->second);
    if (session!=NULL &&
        !session->GetLocalType()->Equal(Theta,MpsLocalEndType()))
      return PrintTypeError((string)"Unfinished Session: " + var->first,*this,Theta,Gamma,Omega);
  }
  return true;
} // }}}
MpsTerm *MpsCall::ApplyCall(const std::string &path, const std::vector<MpsFunction> &funs) const // {{{
{ if (path.size()!=0)
    return Error((string)"Applying Call on "+ToString());
  for (vector<MpsFunction>::const_iterator fun=funs.begin(); fun!=funs.end(); ++fun)
  { if (fun->GetName()==myName)
      return this->PSubst(myName,fun->GetBody(),fun->GetArgs(), fun->GetArgPids(), fun->GetStateArgs());
  }
  return Error((string)"Applying Call, but no definition "+myName+" in env");
} // }}}
bool MpsCall::SubSteps(vector<MpsStep> &dest) // {{{
{
  // Create tau event
  MpsEvent event;
  event.myType=tau;
  event.mySubType = funcall;
  event.myStateArgs = myState.size();
  event.myArgs = myArgs.size();
  // Add step to dest
  vector<string> paths;
  paths.push_back("");
  dest.push_back(MpsStep(event, paths));
  // Does not depend on GUI
  return false;
} // }}}
MpsTerm *MpsCall::PRename(const string &src, const string &dst) const // {{{
{
  if (myName != src)
    return Copy();
  else
    return new MpsCall(dst,myArgs,myState,myTypes,myStateTypes);
} // }}}
MpsTerm *MpsCall::ERename(const string &src, const string &dst) const // {{{
{
  vector<MpsExp*> newArgs;
  for (vector<MpsExp*>::const_iterator it=myArgs.begin();it!=myArgs.end();++it)
    newArgs.push_back((*it)->Rename(src,dst));
  vector<MpsExp*> newState;
  for (vector<MpsExp*>::const_iterator it=myState.begin();it!=myState.end();++it)
    newState.push_back((*it)->Rename(src,dst));
  vector<MpsMsgType*> newTypes;
  for (vector<MpsMsgType*>::const_iterator it=myTypes.begin();it!=myTypes.end();++it)
    newTypes.push_back((*it)->ERename(src,dst));
  vector<MpsMsgType*> newStateTypes;
  for (vector<MpsMsgType*>::const_iterator it=myStateTypes.begin();it!=myStateTypes.end();++it)
    newStateTypes.push_back((*it)->ERename(src,dst));
  MpsTerm *result = new MpsCall(myName,newArgs,newState,newTypes,newStateTypes);
  DeleteVector(newArgs);
  DeleteVector(newState);
  DeleteVector(newTypes);
  DeleteVector(newStateTypes);
  return result;
} // }}}
MpsTerm *MpsCall::ReIndex(const string &session, int pid, int maxpid) const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsCall::PSubst(const string &var, const MpsTerm &exp, const vector<string> &args, const vector<pair<int,int> > &argpids, const vector<string> &stateargs) const // {{{
{
  if (myName == var)
  {
    if (myState.size() != stateargs.size())
    {
#if APIMS_DEBUG_LEVEL>2
      cerr << "MpsCall::PSubst wrong number of state arguments in match" << endl;
#endif
      return new MpsEnd();
    }
    if (myArgs.size() != args.size())
    {
#if APIMS_DEBUG_LEVEL>2
      cerr << "MpsCall::PSubst wrong number of arguments in match" << endl;
#endif
      return new MpsEnd();
    }
    // Create new Variable Names
    vector<string> stateNames;
    stateNames.clear();
    vector<string> argNames;
    argNames.clear();
    MpsTerm *newTerm=exp.Copy();
    for (vector<string>::const_iterator it=stateargs.begin();it!=stateargs.end();++it)
    {
      // Create new variable
      MpsExp *newVar=new MpsVarExp(MpsExp::NewVar(),MpsMsgNoType());
      MpsTerm *tmpTerm=newTerm->ESubst(*it,*newVar); // Rename statearg to new name
      stateNames.push_back(newVar->ToString());
      delete newVar;
      delete newTerm;
      newTerm=tmpTerm;
    }
    for (vector<string>::const_iterator it=args.begin();it!=args.end();++it)
    {
      // Create new variable
      MpsExp *newVar=new MpsVarExp(MpsExp::NewVar(),MpsMsgNoType());
      MpsTerm *tmpTerm=newTerm->ESubst(*it,*newVar); // Rename arg to new name
      argNames.push_back(newVar->ToString());
      delete newVar;
      delete newTerm;
      newTerm=tmpTerm;
    }
    // Now the args and stateargs cannot capture any variables in myArgs and myState
    
    vector<MpsExp*>::const_iterator stateExp=myState.begin();
    for (vector<string>::const_iterator stateName=stateNames.begin();
         stateName!=stateNames.end() && stateExp!=myState.end();
         ++stateName,++stateExp)
    { // Substitute the argNames for the arguments one by one
      MpsExp *value=(*stateExp)->Eval();
      MpsTerm *tmpTerm=newTerm->ESubst(*stateName,*value);
      delete value;
      delete newTerm;
      newTerm = tmpTerm;
    }
    vector<MpsExp*>::const_iterator argExp=myArgs.begin();
    vector<pair<int,int> >::const_iterator argPid=argpids.begin();
    for (vector<string>::const_iterator argName=argNames.begin();
         argName!=argNames.end() && argExp!=myArgs.end() && argPid!=argpids.end();
         ++argName,++argExp,++argPid)
    { // Substitute the argNames for the arguments one by one
      MpsExp *value=(*argExp)->Eval();
      MpsTerm *tmpTerm2;
      if (argPid->second>0)
        tmpTerm2=newTerm->ReIndex(*argName,argPid->first,argPid->second);
      else
        tmpTerm2=newTerm->Copy();
      MpsTerm *tmpTerm=tmpTerm2->ESubst(*argName,*value);
      delete value;
      delete tmpTerm2;
      delete newTerm;
      newTerm = tmpTerm;
    }
    return newTerm;
  }
  else
    return new MpsCall(myName,myArgs,myState,myTypes,myStateTypes);
} // }}}
MpsTerm *MpsCall::ESubst(const string &source, const MpsExp &dest) const // {{{
{
  vector<MpsExp*> newArgs;
  for (vector<MpsExp*>::const_iterator it=myArgs.begin();it!=myArgs.end();++it)
    newArgs.push_back((*it)->Subst(source,dest));
  vector<MpsExp*> newState;
  for (vector<MpsExp*>::const_iterator it=myState.begin();it!=myState.end();++it)
    newState.push_back((*it)->Subst(source,dest));
  vector<MpsMsgType*> newTypes;
  for (vector<MpsMsgType*>::const_iterator it=myTypes.begin();it!=myTypes.end();++it)
    newTypes.push_back((*it)->ESubst(source,dest));
  vector<MpsMsgType*> newStateTypes;
  for (vector<MpsMsgType*>::const_iterator it=myStateTypes.begin();it!=myStateTypes.end();++it)
    newStateTypes.push_back((*it)->ESubst(source,dest));
  MpsTerm *result = new MpsCall(myName,newArgs,newState,newTypes,newStateTypes);
  DeleteVector(newArgs);
  DeleteVector(newState);
  DeleteVector(newTypes);
  DeleteVector(newStateTypes);
  return result;
} // }}}
MpsTerm *MpsCall::GSubst(const string &source, const MpsGlobalType &dest, const vector<string> &args) const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsCall::LSubst(const string &source, const MpsLocalType &dest, const vector<string> &args) const // {{{
{
  return Copy();
} // }}}
set<string> MpsCall::FPV() const // {{{
{
  set<string> result;
  result.clear();
  result.insert(myName);
  return result;
} // }}}
set<string> MpsCall::FEV() const // {{{
{
  set<string> result;
  result.clear();
  for (vector<MpsExp*>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  {
    set<string> fv=(*it)->FV();
    result.insert(fv.begin(),fv.end());
  }
  return result;
} // }}}
MpsTerm *MpsCall::Copy() const // {{{
{
  // assert mySucc != NULL
  return new MpsCall(myName, myArgs, myState, myTypes, myStateTypes);
} // }}}
bool MpsCall::Terminated() const // {{{
{
  return false;
} // }}}
MpsTerm *MpsCall::Simplify() const // {{{
{
  return new MpsCall(myName, myArgs, myState, myTypes, myStateTypes);
} // }}}
string MpsCall::ToString(string indent) const // {{{
{
  string result = myName;
  if (myState.size()>0)
  {
    result += "<";
    for (vector<MpsExp*>::const_iterator it=myState.begin(); it!=myState.end(); ++it)
    {
      if (it != myState.begin())
        result += ", ";
      result += (*it)->ToString();
    }
    result += ">";
  }
  result += "(";
  for (vector<MpsExp*>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  {
    if (it != myArgs.begin())
      result += ", ";
    result += (*it)->ToString();
  }
  result += ")";
  return result;
} // }}}
string MpsCall::ToTex(int indent, int sw) const // {{{
{
  string result = ToTex_Var(myName);
  if (myState.size()>0)
  {
    result += "$\\langle$";
    for (vector<MpsExp*>::const_iterator it=myState.begin(); it!=myState.end(); ++it)
    {
      if (it != myState.begin())
        result += ", ";
      result += (*it)->ToString();
    }
    result += "$\\rangle$";
  }
  result += "(";
  for (vector<MpsExp*>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
  {
    if (it != myArgs.begin())
      result += ", ";
    result += (*it)->ToString();
  }
  result += ")";
  return result;
} // }}}
string MpsCall::ToC() const // {{{
{
  stringstream precall;
  stringstream call;
  call << "  return new __Cnt__" << ToC_Name(myName) << "(";
  vector<MpsMsgType*>::const_iterator tit=myStateTypes.begin();
  for (vector<MpsExp*>::const_iterator it=myState.begin(); it!=myState.end(); ++it, ++tit)
  { string newName = (*it)->ToC(precall, (*tit)->ToC());
    if (it != myState.begin())
      call << ", ";
    call << newName;
  }
  tit=myTypes.begin();
  for (vector<MpsExp*>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it, ++tit)
  { string newName = (*it)->ToC(precall, (*tit)->ToC());
    if (it != myArgs.begin() || myState.size()>0)
      call << ", ";
    call << newName;
  }
  call << ");" << endl;
  return precall.str() + call.str();
} // }}}
MpsTerm *MpsCall::RenameAll() const // {{{
{ return Copy();
} // }}}
MpsTerm *MpsCall::CloseDefinitions() const // {{{
{ return Copy();
} // }}}
MpsTerm *MpsCall::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ return Copy();
} // }}}