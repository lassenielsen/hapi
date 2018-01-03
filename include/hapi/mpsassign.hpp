#ifndef MPSASSIGN_HPP
#define MPSASSIGN_HPP
#include <hapi/mpsterm.hpp>
namespace hapi {
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
    MpsTerm *FlattenFork(bool normLhs, bool normRhs, bool pureMode) const;
    MpsTerm *RenameAll() const;
    bool Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const;
    MpsTerm *Append(const MpsTerm &term) const;
    MpsTerm *CopyWrapper(std::map<std::string,void*> &children) const;
    MpsTerm *CloseDefsPre(const MpsMsgEnv &Gamma);
    MpsTerm *ExtractDefinitions(MpsFunctionEnv &env) const;
    std::string GenerateC(const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const std::map<std::string,void*> &children) const;
    std::string GetConsts(const MpsMsgEnv &Gamma, const MpsProcEnv &Omega, const std::map<std::string,void*> &children) const;
    std::string ToC(const std::string &taskType) const;
    std::string ToCHeader() const;
    void ToCConsts(std::vector<std::string> &dest, std::unordered_set<std::string> &existing) const;

    const MpsMsgType &GetExpType() const;

  private:
    std::string myId;
    MpsExp *myExp;
    MpsMsgType *myType;
    MpsTerm *mySucc;
}; // }}}
}
#endif
