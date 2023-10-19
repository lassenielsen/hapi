// DOCUMENTATION: mpsfunction.hpp {{{
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
#include <hapi/mpstype.hpp>

namespace hapi
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
                const std::vector<MpsMsgType*> &statetypes,
                const std::vector<std::string> &args,
                const std::vector<MpsMsgType*> &types,
                const std::vector<std::pair<int,int> > &argpids,
                const MpsTerm &body);
    MpsFunction(const MpsFunction &rhs);
    virtual ~MpsFunction();
    MpsFunction &operator=(const MpsFunction &rhs);

    const std::string &GetName() const;
    const std::vector<std::string> &GetStateArgs() const;
    const std::vector<MpsMsgType*> &GetStateTypes() const;
    const std::vector<std::string> &GetArgs() const;
    const std::vector<MpsMsgType*> &GetTypes() const;
    const std::vector<std::pair<int,int> > &GetArgPids() const;
    const MpsTerm &GetBody() const;

    std::string ToString() const;
 //   std::string ToTex() const;
    std::string ToC() const;
    std::string ToCTaskType();

  private:
    std::string myName;
    std::vector<std::string> myStateArgs;
    std::vector<MpsMsgType*> myStateTypes;
    std::vector<std::string> myArgs;
    std::vector<MpsMsgType*> myTypes;
    std::vector<std::pair<int,int> > myArgPids;
    MpsTerm *myBody;
}; // }}}

// DOCUMENTATION: MpsFunctionEnv {{{
/*!
 * MpsFunctionEnv represents a full function environment, where
 * each procedure is allowed to use all the procedures in the environment.
 */
// }}}
typedef std::vector<MpsFunction> MpsFunctionEnv;

std::string DefEnvToString(const MpsFunctionEnv &env);
std::string DefEnvToC(const MpsFunctionEnv &env);
std::string DefEnvToCHeader(const MpsFunctionEnv &env);

}
#endif
