#include <apims/mpsfunction.hpp>
#include <apims/mpsterm.hpp>

#include "common.cpp"

using namespace std;

namespace apims
{
std::string DefEnv2string(const MpsFunctionEnv &env) // {{{
{
  std::string result="global\n  ";
  bool first = true;
  for (MpsFunctionEnv::const_iterator def=env.begin(); def!=env.end(); ++def)
  {
    if (!first)
      result += ",\n  ";
    else 
      first = false;
    result+=def->GetName() + "<";
    // Print state arguments
    for (std::vector<std::string>::const_iterator arg=def->GetStateArgs().begin(); arg!=def->GetStateArgs().end(); ++arg)
    {
      if (arg!=def->GetStateArgs().begin())
        result += ",";
      result += *arg;
    }
    result+=">(";
    // Print arguments
    vector<pair<int,int> >::const_iterator argpids=def->GetArgPids().begin();
    for (std::vector<std::string>::const_iterator arg=def->GetArgs().begin(); arg!=def->GetArgs().end(); ++arg,++argpids)
    {
      if (arg!=def->GetArgs().begin())
        result += ",";
      result += *arg;
      if (argpids->second>0)
        result += "@(" + int2string(argpids->first) + " of " + int2string(argpids->second);
    }
    result += (std::string)")=\n    "
            + def->GetBody().ToString("    ");
  }
  return result;
} // }}}

MpsFunction::MpsFunction(const string &name, // {{{
                         const vector<string> &stateargs,
                         const vector<MpsMsgType*> &statetypes,
                         const vector<string>&args,
                         const vector<MpsMsgType*> &types,
                         const vector<pair<int,int> >&argpids,
                         const MpsTerm &body)
{ myName = name;
  myStateArgs=stateargs;
  myArgs=args;
  myArgPids=argpids;
  myBody = body.Copy();
  for (vector<MpsMsgType*>::const_iterator it=statetypes.begin(); it!=statetypes.end(); ++it)
    myStateTypes.push_back((*it)->Copy());
  for (vector<MpsMsgType*>::const_iterator it=types.begin(); it!=types.end(); ++it)
    myTypes.push_back((*it)->Copy());
} // }}}
MpsFunction::MpsFunction(const MpsFunction &rhs) // {{{
{ myName = rhs.GetName();
  myStateArgs=rhs.GetStateArgs();
  myArgs=rhs.GetArgs();
  myArgPids=rhs.GetArgPids();
  myBody = rhs.GetBody().Copy();
  for (vector<MpsMsgType*>::const_iterator it=rhs.GetStateTypes().begin(); it!=rhs.GetStateTypes().end(); ++it)
    myStateTypes.push_back((*it)->Copy());
  for (vector<MpsMsgType*>::const_iterator it=rhs.GetTypes().begin(); it!=rhs.GetTypes().end(); ++it)
    myTypes.push_back((*it)->Copy());
} // }}}
MpsFunction::~MpsFunction() // {{{
{ delete myBody;
  DeleteVector(myStateTypes);
  DeleteVector(myTypes);
} // }}}
MpsFunction &MpsFunction::operator=(const MpsFunction &rhs) // {{{
{ myName = rhs.GetName();
  myStateArgs=rhs.GetStateArgs();
  myArgs=rhs.GetArgs();
  myArgPids=rhs.GetArgPids();
  delete myBody;
  myBody = rhs.GetBody().Copy();
  DeleteVector(myStateTypes);
  DeleteVector(myTypes);
  for (vector<MpsMsgType*>::const_iterator it=rhs.GetStateTypes().begin(); it!=rhs.GetStateTypes().end(); ++it)
    myStateTypes.push_back((*it)->Copy());
  for (vector<MpsMsgType*>::const_iterator it=rhs.GetTypes().begin(); it!=rhs.GetTypes().end(); ++it)
    myTypes.push_back((*it)->Copy());
} // }}}
const string &MpsFunction::GetName() const // {{{
{ return myName;
} // }}}
const vector<string> &MpsFunction::GetStateArgs() const // {{{
{ return myStateArgs;
} // }}}
const vector<MpsMsgType*> &MpsFunction::GetStateTypes() const // {{{
{ return myStateTypes;
} // }}}
const vector<string> &MpsFunction::GetArgs() const // {{{
{ return myArgs;
} // }}}
const vector<MpsMsgType*> &MpsFunction::GetTypes() const // {{{
{ return myTypes;
} // }}}
const vector<pair<int,int> > &MpsFunction::GetArgPids() const // {{{
{ return myArgPids;
} // }}}
const MpsTerm &MpsFunction::GetBody() const // {{{
{ return *myBody;
} // }}}
}
