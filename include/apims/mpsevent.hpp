/* MpsEvent is an event type that allows
 * sending to and receiving from a queue including branching,
 * requesting and accepting multiparty sesssions on a channel
 * The content of an event is
 * The event type
 * The channel for communication
 * The value or label being communicated
 * For session init the participants currently accepted, and the session name
 * For session sync the number of participants, and the number of required participants
 *
 * The Subtype property was added in order to send events to the GUI module
 * when certain steps are performed (linking, syncing and guivalue)
 */

#ifndef MPSEVENT_HPP
#define MPSEVENT_HPP

#include <apims/mpschannel.hpp>
#include <string>
#include <list>
#include <map>
#include <iostream>

namespace apims
{

enum event_type {tau=0, snd=10, rcv=11, bsnd=12, brcv=13, link=20, sync=21, guivalue=30, fundef=40, funcall=41, nu=42, cond=43, assign=44};

class MpsEvent // {{{
{
  public:
    MpsEvent();
    MpsEvent(const MpsEvent &rhs);
    virtual ~MpsEvent();

    MpsEvent &operator=(const MpsEvent &rhs);
    std::string ToString() const;

    // Data
    event_type myType; // Holds the event type
    event_type mySubType; // Holds the event type
    MpsChannel myChannel; // For snd, rcv, bsnd, brcv, link and sync
    MpsExp *myValue; // For snd, rcv and guivalue
    std::string myLabel; // For bsnd, brcv and sync
    std::string mySession; // For link and guivalue
    int myMaxpid; // For link and sync
    int myPid; // For guivalue,link
    std::string myName; // For guivalue
    int myArgs; // For Call event
    int myStateArgs; // For Call event
}; // }}}

/* MpsQueue represents a message queue in a runtime process
 */
typedef std::list<MpsEvent> MpsQueue;

/* MpsEnv represents an environment in which to evaluate a term
 */
typedef std::map<MpsChannel,MpsQueue> MpsEnv;
inline std::string Env2string(const MpsEnv &env) // {{{
{
  std::string result="[[";
  bool first = true;
  for (MpsEnv::const_iterator chn=env.begin(); chn!=env.end(); ++chn)
  {
    if (chn->second.size() > 0)
    {
      if (!first)
        result += "\n |";
      else 
        first = false;
      result+=chn->first.ToString() + ": ";
      for (MpsQueue::const_iterator msg=chn->second.begin(); msg!=chn->second.end(); ++msg)
      {
        if (msg!=chn->second.begin())
          result += ",";
        result += msg->ToString();
      }
    }
  }
  result += "]]";
  return result;
} // }}}

}
#endif
