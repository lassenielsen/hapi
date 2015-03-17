#include <hapi/mpsevent.hpp>

using namespace hapi;
using namespace std;

/* MpsEvent constructor and destructor
 */
MpsEvent::MpsEvent() // {{{
: myChannel("_NONE_")
{
  myType = tau;
  myValue = NULL;
} // }}}
MpsEvent::MpsEvent(const MpsEvent &rhs) // {{{
: myType(rhs.myType),
  mySubType(rhs.mySubType),
  myChannel(rhs.myChannel),
  myValue(NULL),
  myLabel(rhs.myLabel),
  mySession(rhs.mySession),
  myMaxpid(rhs.myMaxpid),
  myPid(rhs.myPid),
  myName(rhs.myName)
{
  if (rhs.myValue != NULL)
    myValue = rhs.myValue->Copy();
} // }}}
MpsEvent::~MpsEvent() // {{{
{
  if (myValue != NULL)
    delete myValue;
  myValue = NULL;
} // }}}
MpsEvent &MpsEvent::operator=(const MpsEvent &rhs) // {{{
{
  if (myValue != NULL)
    delete myValue;
  myValue = NULL;

  myType = rhs.myType;
  mySubType = rhs.mySubType;
  myChannel = rhs.myChannel;
  if (rhs.myValue != NULL)
    myValue = rhs.myValue->Copy();
  myLabel = rhs.myLabel;
  mySession = rhs.mySession;
  myMaxpid = rhs.myMaxpid;
  myPid = rhs.myPid;
  myName = rhs.myName;
} // }}}
string MpsEvent::ToString() const // {{{
{
  switch (myType)
  {
    case snd:
      return (string)"<" + myValue->ToString();
      break;
    case rcv:
      return (string)">" + myValue->ToString();
      break;
    case bsnd:
      return (string)"<" + myLabel;
      break;
    case brcv:
      return (string)">" + myLabel;
      break;
    default:
      return "?";
  }
} // }}}
