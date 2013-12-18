#ifndef MPSASSIGN_HPP
#define MPSASSIGN_HPP
#include <apims/mpsterm.hpp>
namespace apims {
// DOCUMENTATION: MpsAssign {{{
/*!
 * MpsAssign represents a let x=E in P process.
 *
 * The assign construct is not explicit in the formal pi-calculus, but can be
 * represented in the formal pi-calculus by the term P[E/x].
 * The assign construct allows some processes to be constructed simpler and
 * more intuitively.
 */
// }}}
class MpsAssign : public MpsTerm // {{{
{
  public:
    MpsAssign(const std::string &id, const MpsExp &exp, const MpsMsgType &type, const MpsTerm &succ);
    virtual ~MpsAssign();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsMsgEnv &Gamma,
                   const MpsProcEnv &Omega);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplyOther(const std::string &path) const;
    MpsAssign *ReIndex(const std::string &session,
                     int pid, int maxpid) const;
    MpsAssign *PRename(const std::string &src, const std::string &dst) const;
    MpsAssign *ERename(const std::string &src, const std::string &dst) const;
    MpsAssign *PSubst(const std::string &var,
                      const MpsTerm &exp,
                      const std::vector<std::string> &args,
                      const std::vector<std::pair<int,int> > &argpids,
                      const std::vector<std::string> &stateargs) const;
    MpsAssign *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsAssign *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsAssign *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> FEV() const;
    MpsAssign *Copy() const;
    bool Terminated() const;
    MpsAssign *Simplify() const;
    std::string ToString(std::string indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    MpsTerm *RenameAll() const;
    MpsTerm *CloseDefinitions() const;
    MpsTerm *ExtractDefinitions(MpsFunctionEnv &env) const;
    std::string ToC() const;

    const MpsMsgType &GetExpType() const;

  private:
    std::string myId;
    MpsExp *myExp;
    MpsMsgType *myType;
    MpsTerm *mySucc;
}; // }}}
}
#endif
