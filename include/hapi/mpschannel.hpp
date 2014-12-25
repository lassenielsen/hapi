// DOCUMENTATION: mpschannel.hpp {{{
/*! \file
 * This file declares the MpsChannel class used to represent both public and session channels.
 */
// }}}

#ifndef MPSCHANNEL_HPP
#define MPSCHANNEL_HPP

#include <hapi/mpsexp.hpp>
#include <string>
#include <hapi/common.hpp>

namespace hapi
{

// DOCUMENTATION: MpsChannel {{{
/*!
 * MpsChannel can represent both public (unindexed) channels and session channels.
 *
 * MpsChannel has a type field whic is used to identify wether the channel
 * represented is a public channel or a session channel.
 *
 * A public channel is identified simply by its channel name.
 * Therefore the only fields relevant for public channels are myType wich
 * should be "public" and myName which should be the name of the channel.
 *
 * A session channel is identified by the session name, and the channel index.
 * Therefore the fields relevant for session channels are myType which should
 * be "session", myName which should be the name of the session and myIndex
 * which should be the index of the channel in the session.
 */
// }}}
class MpsChannel // {{{
{
  public:
    MpsChannel(const std::string &name="_ERROR");
    MpsChannel(const std::string &name, int index);
    virtual ~MpsChannel();
    bool operator==(const MpsChannel &rhs) const; // Equality
    bool operator<(const MpsChannel &rhs) const; // Ordering required by map
    std::string ToString() const; // Return parsable string representation
    std::string GetName() const;  // Returns the name-part of the channel
    std::string GetType() const;  // Returns "either" public or "session"
    int GetIndex() const;         // Returns the index (0 if public channel)
    // Perform substitution
    MpsChannel Subst(const std::string &source, const MpsExp &dest) const;
    MpsChannel Rename(const std::string &src, const std::string &dst) const;
    inline std::string ToTex() const // {{{
    { if (GetType()=="session")
        return ToTex_Session(GetName()) + "[" + ToTex_SID(GetIndex()) + "]";
      else
        return ToTex_ChName(GetName());
    } // }}}

  private:
    std::string myType; // Either "public" or "session"
    std::string myName; // Channel Name
    int myIndex; // Channel Index (0 for public, 1..k for k-channel session)
}; // }}}

}
#endif
