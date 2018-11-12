#include <hapi/mpsfunction.hpp>
#include <hapi/mpsterm.hpp>
#include <sstream>

#include <hapi/common.hpp>

using namespace std;

namespace hapi
{
std::string DefEnvToString(const MpsFunctionEnv &env) // {{{
{
  std::string result="global\n  ";
  for (MpsFunctionEnv::const_iterator def=env.begin(); def!=env.end(); ++def)
  {
    if (def!=env.begin())
      result += ",\n  ";
    result += def->ToString();
  }
  return result;
} // }}}
std::string DefEnvToC(const MpsFunctionEnv &env) // {{{
{
  stringstream ss;
  ss << endl << "/* Procedure implementations */" << endl;
  for (MpsFunctionEnv::const_iterator def=env.begin(); def!=env.end(); ++def)
  {
    ss << def->ToC() << endl;
  }
  return ss.str();
} // }}}
std::string DefEnvToCHeader(const MpsFunctionEnv &env) // {{{
{
  stringstream ss;
  for (MpsFunctionEnv::const_iterator def=env.begin(); def!=env.end(); ++def)
  {
    ss << def->GetBody().ToCHeader();
  }
  return ss.str();
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
string MpsFunction::ToString() const // {{{
{
  stringstream ss;
  ss << GetName() << "<";
  // Print state arguments
  vector<MpsMsgType*>::const_iterator type=GetStateTypes().begin();
  for (vector<string>::const_iterator arg=GetStateArgs().begin();
       arg!=GetStateArgs().end() &&
       type!=GetStateTypes().end();
       ++arg, ++type)
  {
    if (arg!=GetStateArgs().begin())
      ss << ",";
    ss << *arg;
    ss << ": ";
    ss << (*type)->ToString("        ");
  }
  ss << ">(";
  // Print arguments
  vector<pair<int,int> >::const_iterator argpids=GetArgPids().begin();
  type=GetTypes().begin();
  for (std::vector<std::string>::const_iterator arg=GetArgs().begin(); arg!=GetArgs().end(); ++arg,++argpids,++type)
  {
    if (arg!=GetArgs().begin())
      ss << ",";
    ss << *arg;
    if (argpids->second>0)
      ss << "@(" << int2string(argpids->first) << " of " << int2string(argpids->second) << ")";
    ss << ": " << (*type)->ToString("        ");
  }
  ss << (std::string)")=\n    "
     << GetBody().ToString("    ");
  return ss.str();
} // }}}
string MpsFunction::ToC() const // {{{
{
  stringstream ss;
  string taskType=ToC_Name(GetName());
  ss << "  method_" << taskType << ": // {{{" << endl
     << "  #undef _this" << endl
     << "  #define _this ((Task_" << taskType << "*)_task)" << endl
     << "  {" << endl;
  ss << GetBody().ToC(taskType)
     << "  } // }}}";
  return ss.str();
} // }}}
string MpsFunction::ToCTaskType() const // {{{
{ set<string> ids=GetBody().EV();
  ids.insert(GetStateArgs().begin(),GetStateArgs().end());
  ids.insert(GetArgs().begin(),GetArgs().end());

  stringstream ss;
  ss
  << "class Task_" << ToC_Name(GetName()) << " : public libpi::task::Task" << endl
  << "{ public:" << endl
  << "    Task_" << ToC_Name(GetName()) << "(libpi::task::Worker *worker) : libpi::task::Task(worker) { SetWorker(worker); }" << endl
  << endl;
  for (set<string>::const_iterator id=ids.begin(); id!=ids.end(); ++id)
    ss << "    libpi::Value *var_" << ToC_Name(*id) << ";" << endl;
  for (vector<string>::const_iterator arg=GetStateArgs().begin(); arg!=GetStateArgs().end(); ++arg)
    ss << "    inline void SetStateArg" << std::distance(GetStateArgs().begin(),arg) << "(libpi::Value *val) {var_" << ToC_Name(*arg) << "=val;}" << endl;
  for (vector<string>::const_iterator arg=GetArgs().begin(); arg!=GetArgs().end(); ++arg)
    ss << "    inline void SetArg" << std::distance(GetArgs().begin(),arg) << "(libpi::Value *val) {var_" << ToC_Name(*arg) << "=val;}" << endl;
  ss
  << "    void Mark(std::unordered_set<Value*> &marks)" << endl
  << "    {" << endl
  << "      if (marks.count(this)>0)" << endl
  << "        return;" << endl;
  for (set<string>::const_iterator id=ids.begin(); id!=ids.end(); ++id)
    ss << "      if (var_" << ToC_Name(*id) << "!=NULL) var_" << ToC_Name(*id) << "->Mark(marks);" << endl;
  ss
  << "    }" << endl
  << "};" << endl;
  return ss.str();
} // }}}
}
