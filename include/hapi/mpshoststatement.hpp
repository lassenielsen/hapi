#ifndef MPSHOSTSTATEMENT_HPP
#define MPSHOSTSTATEMENT_HPP
#include <hapi/mpsterm.hpp>
namespace hapi {
// DOCUMENTATION: MpsHostStatement {{{
/*!
 * MpsHostStatement enables inlining of host-language statements that are
 * preserver through compilation.
 *
 * This enables the utilization of the host language and its libraries,
 * although care should be taken as the terms do not enjoy the same type
 * safety as the native hapi terms.
 */
// }}}
class MpsHostStatement : public MpsTerm // {{{
{
  public:
    MpsHostStatement(const std::vector<std::string> &hostParts, const std::vector<MpsExp*> &expParts, const MpsTerm &succ, const std::vector<MpsMsgType*> expTypes, bool pure);
    virtual ~MpsHostStatement();

    void* TDCompileMain(tdc_pre pre,
                        tdc_post wrap,
                        tdc_error wrap_err,
                        const MpsExp &Theta,
                        const MpsMsgEnv &Gamma,
                        const MpsProcEnv &Omega, 
                        const std::set<std::pair<std::string,int> > &pureStack,
                        const std::string &curPure,
                        PureState pureState,
                        bool checkPure=true);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplyOther(const std::string &path) const;
    MpsHostStatement *ReIndex(const std::string &session,
                     int pid, int maxpid) const;
    MpsHostStatement *PRename(const std::string &src, const std::string &dst) const;
    MpsHostStatement *ERename(const std::string &src, const std::string &dst) const;
    MpsHostStatement *MRename(const std::string &src, const std::string &dst) const;
    MpsHostStatement *PSubst(const std::string &var,
                             const MpsTerm &exp,
                             const std::vector<std::string> &args,
                             const std::vector<std::pair<int,int> > &argpids,
                             const std::vector<std::string> &stateargs) const;
    MpsHostStatement *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsHostStatement *MSubst(const std::string &source, const MpsMsgType &dest) const;
    MpsHostStatement *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsHostStatement *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> EV() const;
    std::set<std::string> FEV() const;
    MpsHostStatement *Copy() const;
    bool Terminated() const;
    MpsHostStatement *Simplify() const;
    std::string ToString(std::string indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    MpsTerm *FlattenFork(bool normLhs, bool normRhs, bool pureMode) const;
    MpsHostStatement *RenameAll() const;
    bool Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const;
    MpsTerm *Append(const MpsTerm &term) const;
    MpsTerm *CopyWrapper(std::map<std::string,void*> &children) const;
    MpsTerm *CloseDefsPre(const MpsMsgEnv &Gamma);
    MpsHostStatement *ExtractDefinitions(MpsFunctionEnv &env) const;
    std::string ToC(const std::string &taskType) const;
    std::string ToCHeader() const;
    void ToCConsts(std::vector<std::string> &dest, std::unordered_set<std::string> &existing) const;

  private:
    std::vector<std::string> myHostParts;
    std::vector<MpsExp*> myExpParts;
    std::vector<MpsMsgType*> myTypes;
    MpsTerm *mySucc;
    bool myPure;
}; // }}}
}
#endif
