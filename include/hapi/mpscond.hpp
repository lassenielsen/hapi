#ifndef MPSCOND_HPP
#define MPSCOND_HPP
#include <hapi/mpsterm.hpp>
namespace hapi {
// DOCUMENTATION: MpsCond {{{
/*!
 * MpsCond is a standard if-then-else branching.
 */
// }}}
class MpsCond : public MpsTerm // {{{
{
  public:
    MpsCond(const MpsExp &cond, const MpsTerm &truebranch, const MpsTerm &falsebranch);
    virtual ~MpsCond();

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
    MpsTerm *ReIndex(const std::string &session,
                     int pid, int maxpid) const;
    MpsTerm *PRename(const std::string &src, const std::string &dst) const;
    MpsTerm *ERename(const std::string &src, const std::string &dst) const;
    MpsTerm *MRename(const std::string &src, const std::string &dst) const;
    MpsTerm *PSubst(const std::string &var,
                    const MpsTerm &exp,
                    const std::vector<std::string> &args,
                    const std::vector<std::pair<int,int> > &argpids,
                    const std::vector<std::string> &stateargs) const;
    MpsTerm *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsTerm *MSubst(const std::string &source, const MpsMsgType &dest) const;
    MpsTerm *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsTerm *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> EV() const;
    std::set<std::string> FEV() const;
    MpsTerm *Copy() const;
    bool Terminated() const;
    MpsTerm *Simplify() const;
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

    const MpsExp *GetCond() const { return myCond; }
    MpsExp *GetCond() { return myCond; }
    const MpsTerm *GetTrueBranch() const { return myTrueBranch; }
    MpsTerm *GetTrueBranch() { return myTrueBranch; }
    const MpsTerm *GetFalseBranch() const { return myFalseBranch; }
    MpsTerm *GetFalseBranch() { return myFalseBranch; }

  private:
    MpsExp *myCond;
    MpsTerm *myTrueBranch;
    MpsTerm *myFalseBranch;
}; // }}}
}
#endif
