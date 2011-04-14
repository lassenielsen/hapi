/* MpsChannel represents both public and session channels
 *
 */

#ifndef MPSCHANNEL_HPP
#define MPSCHANNEL_HPP

#include <apims/mpsexp.hpp>
#include <string>

namespace apims
{

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

  private:
    std::string myType; // Either "public" or "session"
    std::string myName; // Channel Name
    int myIndex; // Channel Index (0 for public, 1..k for k-channel session)
}; // }}}

}
#endif
