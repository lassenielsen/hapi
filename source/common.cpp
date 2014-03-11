/* Common contains the help-functions for the picalc-mps library
 */

#include <apims/mpsexp.hpp>
#include <apims/mpschannel.hpp>
#include <apims/mpstype.hpp>
#include <apims/mpsterm.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <iostream>
#include <typeinfo>
#include <algorithm>

// FIXME: Move to actual Debug Class
// DEBUG_LEVEL:
// 0: ABSOLUTELY NO OUTPUT
// 1: OUTPUT FATAL (PARSER, TYPE, ...) ERRORS
// 2: ALSO OUTPUT SOUNDNESS ERRORS
// 3: ALSO OUTPUT COMPLETENESS ERRORS
// 4: ALSO OUTPUT UI MESSAGES
//10: ALSO NON FATAL ERRORS
//50: ALSO OUTPUT PARSED PROGRAM
//59: ALSO OUTPUT EACH STEP IN EXECUTION
//99: ALSO OUTPUT CALTREE TRACING WITH BEBUG INFORMATION, ...
#define APIMS_DEBUG_LEVEL 5


/* Simple help functions
 */
inline std::string double2string(double d) // {{{
{
  std::stringstream ss;
  ss.clear();
  ss << d;
  std::string s = ss.str();
  return s;
} // }}}
inline double string2double(std::string s) // {{{
{
  std::stringstream ss;
  ss.clear();
  ss << s;
  double result;
  ss >> result;
  return result;
} // }}}
inline std::string int2string(int i) // {{{
{
  std::stringstream ss;
  ss.clear();
  ss << i;
  std::string s = ss.str();
  std::replace(s.begin(),s.end(),'-','~');
  return s;
} // }}}
inline int string2int(std::string s) // {{{
{
  // Allow the use of '~' as minus
  std::replace(s.begin(),s.end(),'~','-');
  std::stringstream ss;
  ss.clear();
  ss << s;
  int result;
  ss >> result;
  return result;
} // }}}
inline std::string stringreplace(const std::string &source, const std::string &from, const std::string &to) // {{{
{
  if (from=="")
    return source;
  std::string dest="";
  int pos = 0;
  while (pos < source.size())
  {
    int newpos = source.find(from,pos);
    if (newpos!=std::string::npos)
    {
      dest += source.substr(pos,newpos-pos);
      dest += to;
      pos = newpos + from.size();
    }
    else
    {
      dest += source.substr(pos);
      pos = source.size();
    }
  }
  return dest;
} // }}}
inline std::string stringESubst(const std::string &name, const std::string &source, const apims::MpsExp &dest) // {{{
{
  if (name == source) // perform substitution
  {
    if (typeid(dest) != typeid(apims::MpsVarExp)) // Error: match with non channel expression
    {
      std::cerr << "Error Channel substitution ignoring substitution match with non channel expression: "
                << dest.ToString()
                << std::endl;
      return "_ERROR";
    }
    return dest.ToString();
  }
  return name;
} // }}}
inline std::string stuff_string(std::string input) // {{{
{ return stringreplace(stringreplace(input,"\\","\\\\"),"\"","\\\"");
} // }}}
inline std::string wrap_string(std::string input) // {{{
{ return (std::string)"\""+stuff_string(input)+"\"";
} // }}}
inline std::string unwrap_string(std::string input) // {{{
{ std::string value = input.substr(1,input.size()-2);
  return stringreplace(stringreplace(value,"\\\\","\\"),"\\\"","\"");
} // }}}

inline bool disjoint(const std::set<int> &lhs, const std::set<int> &rhs) // {{{
{
  for (std::set<int>::const_iterator it=lhs.begin(); it!=lhs.end(); ++it)
    if (rhs.find(*it) != rhs.end()) // Not disjoint (*it common elt)
      return false;
  return true;
} // }}}

template <class T>
inline std::vector<T*> CopyVector(const std::vector<T*> &container) // {{{
{ std::vector<T*> result;
  for (typename std::vector<T*>::const_iterator it=container.begin(); it!=container.end(); ++it)
    result.push_back((*it)->Copy());
  return result;
} // }}}
template <class T>
inline void DeleteVector(std::vector<T*> &container) // {{{
{
  while (container.size() > 0)
  {
    delete container.back();
    container.pop_back();
  }
} // }}}
template <class T>
inline void DeleteMap(std::map<std::string,T*> &container) // {{{
{
  while (container.size() > 0)
  {
    delete container.begin()->second;
    container.erase(container.begin());
  }
} // }}}

// ToTex helper functions
inline std::string ToTex_Hspace(int indent, int spacelength) // {{{
{ return "$ $\\hspace*{" + double2string(((double)indent*spacelength)/100) + "mm}";
} // }}}
inline std::string ToTex_PP(int id) // {{{
{ return "{\\tt\\color{green}" + int2string(id) + "}";
} // }}}
inline std::string ToTex_SID(int id) // {{{
{ return "{\\tt\\color{purple}" + int2string(id) + "}";
} // }}}
inline std::string ToTex_KW(std::string name) // {{{
{ return "{\\tt\\color{blue}" + name + "}";
} // }}}
inline std::string ToTex_Label(std::string name) // {{{
{ if (name[0]=='^')
    return "\\underline{\\tt\\color{orange}" + name.substr(1) + "}";
  else
    return "{\\tt\\color{orange}" + name.substr(1) + "}";
} // }}}
inline std::string ToTex_Var(std::string name) // {{{
{ if (name == "")
    return "";
  else if (name[0]=='$')
    return "{\\tt\\color{BlueGreen}" + name.substr(1) + "}";
  else if (name[0]=='%')
    return "{\\tt\\color{Turquoise}" + name.substr(1) + "}";
  else
    return "{\\tt\\color{Cyan}" + name + "}";
} // }}}
inline std::string ToTex_ChName(std::string s) // {{{
{ return (std::string)"{\\tt\\color{blue}" + s + "}";
} // }}}
inline std::string ToTex_Session(std::string s) // {{{
{ return (std::string)"{\\tt\\color{Green}" + s + "}";
} // }}}
inline std::string ToTex_Channel(apims::MpsChannel ch) // {{{
{ if (ch.GetType()=="session")
    return ToTex_Session(ch.GetName()) + "[" + ToTex_SID(ch.GetIndex()) + "]";
  else
    return ToTex_ChName(ch.GetName());
} // }}}

// ToC helper functions
inline std::string ToC_Name(const std::string &name) // {{{
{ return stringreplace(name,"~","__SIM__");
} // }}}

// Typechecking output functions
inline std::string PrintGamma(const apims::MpsMsgEnv &Gamma, const std::string &indent) // {{{
{
  std::string result="";
  for (apims::MpsMsgEnv::const_iterator it=Gamma.begin();it!=Gamma.end();++it)
  {
    if (it!=Gamma.begin())
      result += ",\n" + indent;
    std::string newIndent = indent + "  ";
    for (int i=0; i<it->first.size(); ++i)
      newIndent += " ";
    result += it->first + ": " + it->second->ToString(newIndent);
  }
  return result;
} // }}}
inline std::string PrintOmega(const apims::MpsProcEnv &Omega, const std::string &indent) // {{{
{
  std::string result = "";
  for (apims::MpsProcEnv::const_iterator it=Omega.begin();it!=Omega.end();++it)
  {
    if (it!=Omega.begin())
      result += ",\n" + indent;
    result += it->first + "< ";
    std::string newIndent = indent + "  ";
    for (int i=0;i < it->first.size();++i)
      newIndent += " ";
    for (int i=0; i<it->second.stypes.size(); ++i)
    { if (i>0)
        result += ",\n" + newIndent;
      result += it->second.snames[i] + ": " + it->second.stypes[i]->ToString(newIndent);
    }
    result += " >\n"+newIndent+"( ";
    for (std::vector<apims::MpsMsgType*>::const_iterator arg=it->second.types.begin(); arg!=it->second.types.end(); ++arg)
    {
      if (arg!=it->second.types.begin())
        result += ",\n" + newIndent;
      result += (*arg)->ToString(newIndent);
    }
    result += " )";
  }
  return result;
} // }}}
inline bool PrintTypeError(const std::string &message, const apims::MpsTerm &term, const apims::MpsExp &Theta,  const apims::MpsMsgEnv &Gamma, const apims::MpsProcEnv &Omega) // {{{
{
#if APIMS_DEBUG_LEVEL>1
  std::cerr << "!!!!!!!!!!!!!!! Type Error: !!!!!!!!!!!!!!!" << std::endl
            << "!!!!!!!Term: " << term.ToString("!!!!!        ") << std::endl
            << "!Assertions: " << Theta.ToString() << std::endl
            << "!!!Type Env: " << PrintGamma(Gamma,"!!!!         ") << std::endl
            << "!Method Env: " << PrintOmega(Omega,"!!!!         ") << std::endl
            << "!!!!Message: " << message << std::endl;
#endif
  return false;
} // }}}

// Helpfunction for when unfolding is necessary
inline bool TypeCheckRec(const apims::MpsExp &Theta, const apims::MpsMsgEnv &Gamma, const apims::MpsProcEnv &Omega, apims::MpsTerm &term, const std::string &session) // Using new rule unfold (or eq) {{{
{
  apims::MpsMsgEnv::const_iterator it=Gamma.find(session);
  if (it==Gamma.end())
    return PrintTypeError((std::string)"Unfolding closed session: " + session,term,Theta,Gamma,Omega);
  const apims::MpsDelegateMsgType *delType = dynamic_cast<const apims::MpsDelegateMsgType*>(it->second);
  const apims::MpsLocalRecType *type = dynamic_cast<const apims::MpsLocalRecType*>(delType->GetLocalType());
  if (type==NULL)
    return PrintTypeError((std::string)"Unfolding non-rec type: " + it->second->ToString(),term,Theta,Gamma,Omega);
  apims::MpsMsgEnv newGamma = Gamma;
  // Create type for substitution
  std::vector<apims::TypeArg> args;
  std::vector<std::string> argnames;
  for (std::vector<apims::TypeArg>::const_iterator arg=type->GetArgs().begin(); arg!=type->GetArgs().end(); ++arg)
  { apims::MpsExp *newValue = new apims::MpsVarExp(arg->myName, apims::MpsMsgNoType());
    apims::TypeArg newArg(arg->myName, *arg->myType,  *newValue);
    delete newValue;
    args.push_back(newArg);
    argnames.push_back(newArg.myName);
  }
  apims::MpsLocalType *substType=new apims::MpsLocalRecType(type->GetName(), *type->GetSucc(), args);
  apims::MpsLocalType *newType = type->GetSucc()->LSubst(type->GetName(),*substType,argnames);
  delete substType;
  for (std::vector<apims::TypeArg>::const_iterator arg=type->GetArgs().begin(); arg!=type->GetArgs().end(); ++arg)
  { apims::MpsLocalType *tmpType = newType->ESubst(arg->myName,*arg->myValue);
    delete newType;
    newType=tmpType;
  }
  apims::MpsDelegateLocalMsgType *newMsgType=new apims::MpsDelegateLocalMsgType(*newType,delType->GetPid(),delType->GetParticipants());
  newGamma[session] = newMsgType;
  bool result = false;
  if (dynamic_cast<apims::MpsLocalRecType*>(newType)==NULL)
    result = term.TypeCheck(Theta,newGamma,Omega);
  else
    result = PrintTypeError((std::string)"Using non-contractive type: " + it->second->ToString(),term,Theta,Gamma,Omega);
  delete newType;
  delete newMsgType;
  return result;
} // }}}
inline bool TypeCheckForall(const apims::MpsExp &Theta, const apims::MpsMsgEnv &Gamma, const apims::MpsProcEnv &Omega, apims::MpsTerm &term, const std::string &session) // Using new rule forall {{{
{
  apims::MpsMsgEnv::const_iterator it=Gamma.find(session);
  if (it==Gamma.end())
    return PrintTypeError((std::string)"Forall on closed session: " + session,term,Theta,Gamma,Omega);
  const apims::MpsDelegateMsgType *delType = dynamic_cast<const apims::MpsDelegateMsgType*>(it->second);
  const apims::MpsLocalForallType *type = dynamic_cast<const apims::MpsLocalForallType*>(delType->GetLocalType());
  if (type==NULL)
    return PrintTypeError((std::string)"Forall on non-forall type: " + it->second->ToString(),term,Theta,Gamma,Omega);
  // Find new name for bound variable
  std::string newName = apims::MpsExp::NewVar(type->GetName());
  // Create type for substitution
  apims::MpsLocalType *newType = type->GetSucc()->ERename(type->GetName(),newName);
  apims::MpsDelegateLocalMsgType *newMsgType=new apims::MpsDelegateLocalMsgType(*newType,delType->GetPid(),delType->GetParticipants());
  delete newType;
  // Create new Theta
  apims::MpsExp *newAssertion = type->GetAssertion().Rename(type->GetName(),newName);
  apims::MpsExp *newTheta=new apims::MpsBinOpExp("and",Theta,*newAssertion,apims::MpsBoolMsgType(),apims::MpsBoolMsgType());
  delete newAssertion;
  // Create new Gamma
  apims::MpsMsgEnv newGamma = Gamma;
  newGamma[session] = newMsgType;
  bool result = term.TypeCheck(*newTheta,newGamma,Omega);
  // Clean Up
  delete newTheta;
  return result;
} // }}}

