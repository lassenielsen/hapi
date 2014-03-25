#include <apims/mpschannel.hpp>
#include <apims/common.hpp>

using namespace apims;
using namespace std;

/* MpsChannel implementation
 */
MpsChannel::MpsChannel(const string &name) // {{{
{
  myName = name;
  myType = "public";
  myIndex = 0;
} // }}}
MpsChannel::MpsChannel(const string &name, int index) // {{{
{
  myName = name;
  myType = "session";
  myIndex = index;
} // }}}
MpsChannel::~MpsChannel() // {{{
{
} // }}}
bool MpsChannel::operator==(const MpsChannel &rhs) const // {{{
{
  if (myType == "session" && rhs.myType == "session")
    return myName == rhs.myName && myIndex == rhs.myIndex;

  if (myType != "session" && rhs.myType != "session")
    return myName == rhs.myName;

  return false;
} // }}}
bool MpsChannel::operator<(const MpsChannel &rhs) const // {{{
{
  if (myName < rhs.myName)
    return true;

  if (myType == "session" && rhs.myType == "session" &&
      myName == rhs.myName && myIndex < rhs.myIndex)
    return true;

  return false;
} // }}}
string MpsChannel::ToString() const // {{{
{
  if (myType == "session")
    return myName + "[" + int2string(myIndex) + "]";
  return myName;
} // }}}
string MpsChannel::GetName() const // {{{
{
  return myName;
} // }}}
int MpsChannel::GetIndex() const // {{{
{
  if (myType == "session")
    return myIndex;
  else
    return 0;
} // }}}
string MpsChannel::GetType() const // {{{
{
  return myType;
} // }}}
MpsChannel MpsChannel::Subst(const string &source, const MpsExp &dest) const // {{{
{
  if (myType == "session")
    return MpsChannel(stringESubst(GetName(),source,dest),myIndex);
  else
    return MpsChannel(stringESubst(GetName(),source,dest));
} // }}}
MpsChannel MpsChannel::Rename(const string &src, const string &dst) const // {{{
{
  string newName=GetName()==src?dst:GetName();
  if (myType == "session")
    return MpsChannel(newName,myIndex);
  else
    return MpsChannel(newName);
} // }}}

