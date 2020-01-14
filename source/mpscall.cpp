#include<hapi/mpscall.hpp>
#include<hapi/mpsend.hpp>
#include <hapi/common.hpp>

using namespace std;
using namespace hapi;

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
void *MpsCall::TDCompileMain(tdc_pre pre, tdc_post wrap, tdc_error wrap_err, const MpsExp &Theta, const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const set<pair<string,int> > &pureStack, const string &curPure, PureState pureState, bool checkPure) // * Use rule Var {{{
{ map<string,void*> children;
  if (checkPure)
  { // Check purity constraints
    if (pureStack.size()>0)
      return wrap_err(this,PrintTypeError("Implementation of pure participant " + int2string(pureStack.begin()->second) + "@" + pureStack.begin()->first + " must be immediately after its decleration",*this,Theta,Gamma,Omega),children);

    if (pureState!=CPS_IMPURE && pureState!=CPS_PURE && pureState!=CPS_SERVICE_CALL && pureState!=CPS_INIT_BRANCH1_CALL1 && pureState!=CPS_INIT_BRANCH1_CALL2 && pureState!=CPS_INIT_BRANCH2_CALL && pureState!=CPS_INIT_CALL)
      return wrap_err(this,PrintTypeError("Error in implementation of pure participant " + curPure + ". Pure implementations must conform with the structure \n     *   local X()\n	   *   ( global s=new ch(p of n);\n		 *     X();\n		 *     |\n		 *     P\n		 *   )\n		 *   local StartX(Int i)\n		 *   ( if i<=0\n		 *     then X();\n		 *     else X(); | StartX(i-1);\n		 *   )\n		 *   StartX( E ); |" ,*this,Theta,Gamma,Omega),children);
  }

  // Verify call
  // Check variable is defined
  MpsProcEnv::const_iterator omega = Omega.find(myName);
  if (omega == Omega.end())
    return wrap_err(this,PrintTypeError((string)"Process Variable not defined: " + myName,*this,Theta,Gamma,Omega),children);

  if (checkPure)
  { // Verify purity constraint
    if (pureState!=CPS_IMPURE && !omega->second.pure)
      return wrap_err(this,PrintTypeError("Calling impure method " + myName + " from a pure setting.",*this,Theta,Gamma,Omega),children);
  }
  // Check correct number of arguments
  if (omega->second.stypes.size() != myState.size() ||
      omega->second.snames.size() != myState.size() ||
      omega->second.types.size() != myArgs.size())
    return wrap_err(this,PrintTypeError((string)"Process Variable wrong argument-count: " + myName,*this,Theta,Gamma,Omega),children);
  // Check argument-types and remove used sessions from endGamma
  MpsMsgEnv endGamma=Gamma;
  DeleteVector(myStateTypes);
  for (int i=0;i<myState.size();++i)
  {
    MpsMsgType *statetype = myState[i]->TypeCheck(Gamma);
    bool statetypematch = statetype->Equal(Theta,*omega->second.stypes[i]);
    delete statetype;    
    if (!statetypematch)
      return wrap_err(this,PrintTypeError((string)"State argument does not have type: " + omega->second.stypes[i]->ToString("!!!!                                      "),*this,Theta,Gamma,Omega),children);
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
    bool argtypematch = argType->Equal(Theta,*callType);
    string callTypeString = callType->ToString("!!!!                                      ");

    // Store type for compilation
    myTypes.push_back(callType->Copy());
    delete argType;
    delete callType;
    if (not argtypematch)
      return wrap_err(this,PrintTypeError((string)"Argument does not have type: " + callTypeString,*this,Theta,Gamma,Omega),children);
    if (dynamic_cast<MpsDelegateMsgType*>(omega->second.types[i]) != NULL)
    {
      if (typeid(*myArgs[i]) != typeid(MpsVarExp))
        return wrap_err(this,PrintTypeError((string)"Argument must be session: " + myArgs[i]->ToString(),*this,Theta,Gamma,Omega),children);
      else
      {
        MpsVarExp *var=(MpsVarExp*)myArgs[i];
        MpsMsgEnv::iterator session=endGamma.find(var->ToString());
        if (session == endGamma.end())
          return wrap_err(this,PrintTypeError((string)"Argument session not defined or used more than once: " + var->ToString(),*this,Theta,Gamma,Omega),children);
        // Remove the used session (linearity)
        endGamma.erase(session);
      }
    }
  }
  // Check that endGamma is completed
  for (MpsMsgEnv::const_iterator var=endGamma.begin();var!=endGamma.end();++var)
  { const MpsDelegateMsgType *session=dynamic_cast<const MpsDelegateMsgType*>(var->second);
    if (session!=NULL)
    { MpsLocalType *localSession=session->CopyLocalType();
      bool isDone=localSession->IsDone();
      delete localSession;
      if (!isDone)
        return wrap_err(this,PrintTypeError((string)"Unfinished Session: " + var->first,*this,Theta,Gamma,Omega),children);
    }
  }
  // Wrap result
  return wrap(this,Theta,Gamma,Omega,pureStack,curPure,pureState,checkPure,children);
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
MpsTerm *MpsCall::MRename(const string &src, const string &dst) const // {{{
{
  //vector<MpsExp*> newArgs;
  //for (vector<MpsExp*>::const_iterator it=myArgs.begin();it!=myArgs.end();++it)
  //  newArgs.push_back((*it)->Rename(src,dst));
  //vector<MpsExp*> newState;
  //for (vector<MpsExp*>::const_iterator it=myState.begin();it!=myState.end();++it)
  //  newState.push_back((*it)->Rename(src,dst));
  vector<MpsMsgType*> newTypes;
  for (vector<MpsMsgType*>::const_iterator it=myTypes.begin();it!=myTypes.end();++it)
    newTypes.push_back((*it)->MRename(src,dst));
  vector<MpsMsgType*> newStateTypes;
  for (vector<MpsMsgType*>::const_iterator it=myStateTypes.begin();it!=myStateTypes.end();++it)
    newStateTypes.push_back((*it)->MRename(src,dst));
  MpsTerm *result = new MpsCall(myName,myArgs,myState,newTypes,newStateTypes);
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
#if HAPI_DEBUG_LEVEL>2
      cerr << "MpsCall::PSubst wrong number of state arguments in match" << endl;
#endif
      return new MpsEnd();
    }
    if (myArgs.size() != args.size())
    {
#if HAPI_DEBUG_LEVEL>2
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
set<string> MpsCall::EV() const // {{{
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
  result += ");";
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
string MpsCall::ToC_prepare(const string &dest) const // {{{
{ stringstream result;
  vector<MpsMsgType*>::const_iterator tit=myStateTypes.begin();
  for (vector<MpsExp*>::const_iterator it=myState.begin(); it!=myState.end(); ++it, ++tit)
  { string name=(*it)->ToC(result, (*tit)->ToC());
    result << "      " << dest << "->SetStateArg" << distance(myState.begin(),it) << "(" << name << ");" << endl;
  }
  tit=myTypes.begin();
  for (vector<MpsExp*>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it, ++tit)
  { string name=(*it)->ToC(result, (*tit)->ToC());
    result << "      " << dest << "->SetArg" << myState.size()+distance(myArgs.begin(),it) << "(" << name << ");" << endl;
  }
  result << "      " << dest << "->SetLabel(&&method_" << ToC_Name(myName) << ");" << endl;

  return result.str();
} // }}}
string MpsCall::ToC(const string &taskType) const // {{{
{ string newName = ToC_Name(MpsExp::NewVar("task")); // Create variable name foor the new state
  stringstream result;
  result << ToC_Yield()
         << "    { Task_" << ToC_Name(myName) << " *" << newName << " = new Task_" << ToC_Name(myName) << "(&_this->GetWorker());" << endl
         << ToC_prepare(newName)
         << "      delete _this;" << endl
         << "      _task=" << newName << ";" << endl
         << "    }" << endl
         << "    goto method_" << ToC_Name(myName) << ";" << endl;
  return result.str();
} // }}}
string MpsCall::ToCHeader() const // {{{
{
  return "";
} // }}}
void MpsCall::ToCConsts(std::vector<std::string> &dest, std::unordered_set<std::string> &existing) const // {{{
{ for (vector<MpsExp*>::const_iterator it=myState.begin(); it!=myState.end(); ++it)
    (*it)->ToCConsts(dest,existing);
  for (vector<MpsExp*>::const_iterator it=myArgs.begin(); it!=myArgs.end(); ++it)
    (*it)->ToCConsts(dest,existing);
} // }}}
MpsTerm *MpsCall::FlattenFork(bool normLhs, bool normRhs, bool pureMode) const // {{{
{ return Copy();
} // }}}
MpsTerm *MpsCall::RenameAll() const // {{{
{ return Copy();
} // }}}
bool MpsCall::Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const // {{{
{ seqTerm=Copy();
  parTerm=receives.Append(*seqTerm);
  return false; // No optimizations
} // }}}
MpsTerm *MpsCall::Append(const MpsTerm &term) const // {{{
{ throw (string)"Append applied to call term - not implemented";
} // }}}
MpsTerm *MpsCall::CopyWrapper(std::map<std::string,void*> &children) const // {{{
{
  return Copy();
} // }}}
MpsTerm *MpsCall::CloseDefsPre(const MpsMsgEnv &Gamma) // {{{
{ return this;
} // }}}
MpsTerm *MpsCall::ExtractDefinitions(MpsFunctionEnv &env) const // {{{
{ return Copy();
} // }}}
