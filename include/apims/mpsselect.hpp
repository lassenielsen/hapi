#ifndef MPSSELECT_HPP
#define MPSSELECT_HPP
#include <apims/mpsterm.hpp>
namespace apims {
// DOCUMENTATION: MpsSelect {{{
/*!
 * MpsSelect is a label-sending process.
 */
// }}}
class MpsSelect : public MpsTerm // {{{
{
  public:
    MpsSelect(const MpsChannel &channel, const std::string &label, const MpsTerm &succ, bool final);
    virtual ~MpsSelect();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsMsgEnv &Gamma,
                   const MpsProcEnv &Omega,
                   const std::set<std::pair<std::string,int> > &pureStack,
                   bool reqPure);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplyBSnd(const std::string &path, std::string &label, MpsChannel &ch) const;
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
    bool Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const;
    MpsTerm *Append(const MpsTerm &term) const;
    MpsTerm *CloseDefinitions() const;
    MpsTerm *ExtractDefinitions(MpsFunctionEnv &env) const;
    std::string ToC() const;
    std::string ToCHeader() const;

    bool GetFinal() const;

  private:
    MpsChannel myChannel;
    std::string myLabel;
    MpsTerm *mySucc;
    //! \brief myFinal is true if myChannel can be closed after
    //! the transmission. This is determined by the type-checking.
    bool myFinal;
}; // }}}
}
#endif
