#ifndef MPSGUIVALUE_HPP
#define MPSGUIVALUE_HPP
#include <hapi/mpsterm.hpp>
namespace hapi {
// DOCUMENTATION: MpsGuiValue {{{
/*!
 * MpsGuiValue sends information to the user via the UI module.
 */
// }}}
class MpsGuiValue : public MpsTerm // {{{
{
  public:
    MpsGuiValue(int maxpid, const std::string &session, int pid, const MpsExp &name, const MpsExp &value, const MpsTerm &succ);
    virtual ~MpsGuiValue();

    void* TDCompile(tdc_wrapper wrap,
                    tdc_wraperr wrap_err,
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
    MpsGuiValue *PSubst(const std::string &var,
                    const MpsTerm &exp,
                    const std::vector<std::string> &args,
                    const std::vector<std::pair<int,int> > &argpids,
                    const std::vector<std::string> &stateargs) const;
    MpsGuiValue *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsGuiValue *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsGuiValue *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> FEV() const;
    MpsGuiValue *Copy() const;
    bool Terminated() const;
    MpsGuiValue *Simplify() const;
    std::string ToString(std::string indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    MpsTerm *FlattenFork(bool normLhs, bool normRhs, bool pureMode) const;
    MpsTerm *RenameAll() const;
    bool Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const;
    MpsTerm *Append(const MpsTerm &term) const;
    MpsTerm *CloseDefsWrapper(const MpsExp &Theta,
                              const MpsMsgEnv &Gamma,
                              const MpsProcEnv &Omega, 
                              const std::set<std::pair<std::string,int> > &pureStack,
                              const std::string &curPure,
                              MpsTerm::PureState pureState,
                              bool checkPure,
                              std::map<std::string,void*> &children);
    MpsTerm *ExtractDefinitions(MpsFunctionEnv &env) const;
    std::string ToC() const;
    std::string ToCHeader() const;

  private:
    MpsExp *myName;
    MpsExp *myValue;
    std::string mySession;
    MpsTerm *mySucc;
    int myMaxpid;
    int myPid;
}; // }}}
}
#endif
