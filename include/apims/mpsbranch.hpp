#ifndef MPSBRANCH_HPP
#define MPSBRANCH_HPP
#include <apims/mpsterm.hpp>
namespace apims {
// DOCUMENTATION: MpsBranch {{{
/*!
 * MpsBranch is a label-receiving (and branching, like a case) process.
 */
// }}}
class MpsBranch : public MpsTerm // {{{
{
  public:
    MpsBranch(const MpsChannel &channel, const std::map<std::string, MpsTerm*> &branches, const std::vector<std::string> &finalBranches);
    virtual ~MpsBranch();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsMsgEnv &Gamma,
                   const MpsProcEnv &Omega);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplyBRcv(const std::string &path, const std::string &label) const;
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
    void Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const;
    MpsTerm *Append(const MpsTerm &term) const;
    MpsTerm *CloseDefinitions() const;
    MpsTerm *ExtractDefinitions(MpsFunctionEnv &env) const;
    std::string ToC() const;
    std::string ToCHeader() const;

    const std::vector<std::string> &GetFinalBranches() const;

  private:
    MpsChannel myChannel;
    std::map< std::string, MpsTerm*> myBranches;
    //! \brief myFinalBranches holds the names of the branches where
    //! myChannel can be closed after the transmission. This is
    //! determined by the type-checking.
    std::vector<std::string> myFinalBranches;
}; // }}}
}
#endif
