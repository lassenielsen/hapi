#ifndef MPSEND_HPP
#define MPSEND_HPP
#include <hapi/mpsterm.hpp>

namespace hapi {
// DOCUMENTATION: MpsEnd {{{
/*!
 * MpsEnd objects represent the (sucessfully) terminated process.
 */
// }}}
class MpsEnd : public MpsTerm // {{{
{
  public:
    MpsEnd();
    virtual ~MpsEnd();

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
    MpsTerm *ReIndex(const std::string &session,
                     int pid, int maxpid) const;
    MpsTerm *PRename(const std::string &src, const std::string &dst) const;
    MpsTerm *ERename(const std::string &src, const std::string &dst) const;
    MpsTerm *PSubst(const std::string &var,
                    const MpsTerm &exp,
                    const std::vector<std::string> &args,
                    const std::vector<std::pair<int,int> > &argpids,
                    const std::vector<std::string> &stateargs) const;
    MpsTerm *ESubst(const std::string &source,
                    const MpsExp &dest) const;
    MpsTerm *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsTerm *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
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
}; // }}}
}
#endif
