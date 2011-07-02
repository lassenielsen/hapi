// DOCUMENTATION: mpsterm.hpp {{{
/*! \file
 * This file declares the MpsFunction class used to represent procedure
 * declarations, and MpsFuntionEnv used to represent a function
 * envorinment.
 *
 * This enabels a more compact representation of recursion, because the
 * mu-based representation requires some functions to be defined in multiple
 * places. Using a function environment each function only has to be defined
 * once.
 */
// }}}
#ifndef MPSFUNCTION_HPP
#define MPSFUNCTION_HPP

#include <string>
#include <vector>

namespace apims
{

class MpsTerm;

// DOCUMENTATION: MpsFunction {{{
/*!
 * MpsFunction represents the definition of a procedure. These definitions are
 * used to create a function environment used when evaluating a process.
 *
 * The procedures can be called like a function, and allows a compact
 * representation of recursion. This is used to save memory during evaluation.
 */
// }}}
class MpsFunction // {{{
{ public:
    MpsFunction(const std::string &name,
            const std::vector<std::string> &stateargs,
            const std::vector<std::string> &args,
            const MpsTerm &body);
    MpsFunction(const MpsFunction &rhs);
    virtual ~MpsFunction();
    MpsFunction &operator=(const MpsFunction &rhs);

    const std::string &GetName() const;
    const std::vector<std::string> &GetStateArgs() const;
    const std::vector<std::string> &GetArgs() const;
    const MpsTerm &GetBody() const;

  private:
    std::string myName;
    std::vector<std::string> myStateArgs;
    std::vector<std::string> myArgs;
    MpsTerm *myBody;
}; // }}}

// DOCUMENTATION: MpsFunctionEnv {{{
/*!
 * MpsFunctionEnv represents a full function environment, where
 * each procedure is allowed to use all the procedures in the environment.
 */
// }}}
typedef std::vector<MpsFunction> MpsFunctionEnv;

std::string DefEnv2string(const MpsFunctionEnv &env);

}
#endif
