#ifndef MPSHOSTSTATEMENT_HPP
#define MPSHOSTSTATEMENT_HPP
#include <apims/mpsterm.hpp>
namespace apims {
// DOCUMENTATION: MpsHostStatement {{{
/*!
 * MpsHostStatement enables inlining of host-language statements that are
 * preserver through compilation.
 *
 * This enables the utilization of the host language and its libraries,
 * although care should be taken as the terms do not enjoy the same type
 * safety as the native apims terms.
 */
// }}}
class MpsHostStatement : public MpsTerm // {{{
{
  public:
    MpsHostStatement(const std::vector<std::string> &hostParts, const std::vector<MpsExp*> &expParts, const MpsTerm &succ, const std::vector<MpsMsgType*> expTypes); //
    virtual ~MpsHostStatement();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsMsgEnv &Gamma,
                   const MpsProcEnv &Omega);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplyOther(const std::string &path) const;
    MpsHostStatement *ReIndex(const std::string &session,
                     int pid, int maxpid) const;
    MpsHostStatement *PRename(const std::string &src, const std::string &dst) const;
    MpsHostStatement *ERename(const std::string &src, const std::string &dst) const;
    MpsHostStatement *PSubst(const std::string &var,
                             const MpsTerm &exp,
                             const std::vector<std::string> &args,
                             const std::vector<std::pair<int,int> > &argpids,
                             const std::vector<std::string> &stateargs) const;
    MpsHostStatement *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsHostStatement *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsHostStatement *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> FEV() const;
    MpsHostStatement *Copy() const;
    bool Terminated() const;
    MpsHostStatement *Simplify() const;
    std::string ToString(std::string indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    MpsHostStatement *RenameAll() const;
    MpsHostStatement *CloseDefinitions() const;
    MpsHostStatement *ExtractDefinitions(MpsFunctionEnv &env) const;
    std::string ToC() const;
    std::string ToCHeader() const;

  private:
    std::vector<std::string> myHostParts;
    std::vector<MpsExp*> myExpParts;
    std::vector<MpsMsgType*> myTypes;
    MpsTerm *mySucc;
}; // }}}
}
#endif
