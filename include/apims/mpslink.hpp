#ifndef MPSLINK_HPP
#define MPSLINK_HPP
#include <apims/mpsterm.hpp>
namespace apims {
// DOCUMENTATION: MpsLink {{{
/*!
 * MpsLink creates a new session. If the session involves n participant the process waits for the other (n-1) participants before proceeding.
 */
// }}}
class MpsLink : public MpsTerm // {{{
{
  public:
    MpsLink(const std::string &channel, const std::string &session, int pid, int maxpid, const MpsTerm &succ);
    virtual ~MpsLink();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsMsgEnv &Gamma,
                   const MpsProcEnv &Omega);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplyLink(const std::vector<std::string> &paths, const std::string &session) const;
    MpsTerm *ReIndex(const std::string &session,
                     int pid, int maxpid) const;
    MpsTerm *PRename(const std::string &src, const std::string &dst) const;
    MpsTerm *ERename(const std::string &src, const std::string &dst) const;
    MpsTerm *PSubst(const std::string &var,
                    const MpsTerm &exp,
                    const std::vector<std::string> &args,
                    const std::vector<std::pair<int,int> > &argpids,
                    const std::vector<std::string> &stateargs) const;
    MpsTerm *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsTerm *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsTerm *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> FEV() const;
    MpsTerm *Copy() const;
    bool Terminated() const;
    MpsTerm *Simplify() const;
    std::string ToString(std::string indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    MpsTerm *RenameAll() const;
    MpsTerm *CloseDefinitions() const;
    MpsTerm *ExtractDefinitions(MpsFunctionEnv &env) const;
    std::string ToC() const;

  private:
    std::string myChannel;
    std::string mySession;
    MpsTerm *mySucc;
    int myMaxpid;
    int myPid;
}; // }}}
}
#endif