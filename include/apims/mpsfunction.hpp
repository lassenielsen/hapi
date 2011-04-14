#ifndef MPSFUNCTION_HPP
#define MPSFUNCTION_HPP

#include <string>
#include <vector>

namespace apims
{

class MpsTerm;

/* MpsFunction represents an element in the function environment
 * used in the stepping relation.
 * This can be called like a function, and
 * allows a compact representation of recursion.
 */
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

/* MpsFunctionEnv represents a full function environment, where
 * each function is allowed to call all the functions in the environment.
 */
typedef std::vector<MpsFunction> MpsFunctionEnv;
std::string DefEnv2string(const MpsFunctionEnv &env);

}
#endif
