/* Common contains the help-functions for the picalc-mps library
 */

#include <apims/mpsexp.hpp>
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
#define APIMS_DEBUG_LEVEL 60


/* Simple help functions
 */
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
void DeleteVector(std::vector<T*> &container) // {{{
{
  while (container.size() > 0)
  {
    delete container.back();
    container.pop_back();
  }
} // }}}
template <class T>
void DeleteMap(std::map<std::string,T*> &container) // {{{
{
  while (container.size() > 0)
  {
    delete container.begin()->second;
    container.erase(container.begin());
  }
} // }}}
