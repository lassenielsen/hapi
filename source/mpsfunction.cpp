#include <apims/mpsfunction.hpp>
#include <apims/mpsterm.hpp>

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
    for (std::vector<std::string>::const_iterator arg=def->GetArgs().begin(); arg!=def->GetArgs().end(); ++arg)
    {
      if (arg!=def->GetArgs().begin())
        result += ",";
      result += *arg;
    }
    result += (std::string)")=\n    "
            + def->GetBody().ToString("    ");
  }
  return result;
} // }}}

MpsFunction::MpsFunction(const string &name, const vector<string> &stateargs, const vector<string>&args, const MpsTerm &body) // {{{
{ myName = name;
  myStateArgs=stateargs;
  myArgs=args;
  myBody = body.Copy();
} // }}}
MpsFunction::MpsFunction(const MpsFunction &rhs) // {{{
{ myName = rhs.GetName();
  myStateArgs=rhs.GetStateArgs();
  myArgs=rhs.GetArgs();
  myBody = rhs.GetBody().Copy();
} // }}}
MpsFunction::~MpsFunction() // {{{
{ delete myBody;
} // }}}
MpsFunction &MpsFunction::operator=(const MpsFunction &rhs) // {{{
{ myName = rhs.GetName();
  myStateArgs=rhs.GetStateArgs();
  myArgs=rhs.GetArgs();
  delete myBody;
  myBody = rhs.GetBody().Copy();
} // }}}
const string &MpsFunction::GetName() const // {{{
{ return myName;
} // }}}
const vector<string> &MpsFunction::GetStateArgs() const // {{{
{ return myStateArgs;
} // }}}
const vector<string> &MpsFunction::GetArgs() const // {{{
{ return myArgs;
} // }}}
const MpsTerm &MpsFunction::GetBody() const // {{{
{ return *myBody;
} // }}}
}
