// DOCUMENTATION: mpsstep.hpp {{{
/*! \file
 * This file declares the MpsStep class used to represent a step of evaluation
 * in a process. This allows the evaluation of a process to find all the
 * possible steps, without producing each destination process which saves a lot
 * of time and memmory when there are many possible steps.
 */
// }}}
#ifndef MPSSTEP_HPP
#define MPSSTEP_HPP

#include <vector>
#include <apims/mpsevent.hpp>
#include <apims/mpsfunction.hpp>

namespace apims
{

class MpsTerm;

// DOCUMENTATION: MpsStep // {{{
/*!
 * MpsStep represents a (partial) step.
 * The MpsTerm function SubSteps returns a list of MpsStep items,
 * which represents the possible steps the given process can
 * immediately perform.
 *
 * The destination process of a step can be found by using the Apply method of
 * a step on the source term.
 */
// }}}
class MpsStep // {{{
{ public:
    MpsStep(const MpsEvent &event, const std::vector<std::string> &paths);
    MpsStep(const MpsStep &rhs);
    virtual ~MpsStep();

    // DOCUMENTATION: GetEvent // {{{
    /*!
     * @result The event emitted by the step.
     */
    // }}}
    const MpsEvent &GetEvent() const;
    // DOCUMENTATION: GetPaths // {{{
    /*!
     * @returns The paths to the activity points in the source process.
     *
     * The activity points are the parts of a process that are active in a
     * step. In some types of steps (such as link and sync) there are multiple
     * activity points. All the activitypoints are returned as a vector of
     * paths is returned.
     */
    // }}}
    const std::vector<std::string> &GetPaths() const;
    // DOCUMENTATION: Apply // {{{
    /*!
     * @param term The source process of the step.
     * @param env The buffer-environment.
     * @param defs The procedure-environment.
     * @result The destination process of the step.
     */
    // }}}
    MpsTerm *Apply(const MpsTerm &term, MpsEnv &env, std::vector<MpsFunction> &defs) const;

  private:
    MpsEvent myEvent;
    std::vector<std::string> myPaths;
}; // }}}

}

#endif
