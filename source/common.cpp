/* Common contains the help-functions for the picalc-mps library
 */

#include <apims/mpsexp.hpp>
#include <apims/mpschannel.hpp>
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
