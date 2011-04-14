#ifndef MPSSTEP_HPP
#define MPSSTEP_HPP

#include <vector>
#include <apims/mpsevent.hpp>
#include <apims/mpsfunction.hpp>

namespace apims
{

class MpsTerm;

/* MpsStep represents a (partial) step.
 * The MpsTerm function SubSteps returns a list of MpsStep items,
 * which represents the possible steps the given process can
 * immediately perform.
 */
class MpsStep // {{{
{ public:
    MpsStep(const MpsEvent &event, const std::vector<std::string> &paths);
    MpsStep(const MpsStep &rhs);
    ~MpsStep();

    const MpsEvent &GetEvent() const;
    const std::vector<std::string> &GetPaths() const;

    MpsTerm *Apply(const MpsTerm &term, MpsEnv &env, std::vector<MpsFunction> &defs) const;

  private:
    MpsEvent myEvent;
    std::vector<std::string> myPaths;
}; // }}}

}

#endif
