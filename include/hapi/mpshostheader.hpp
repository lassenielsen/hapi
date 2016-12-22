#ifndef MPSHOSTHEADER_HPP
#define MPSHOSTHEADER_HPP
#include <hapi/mpsterm.hpp>
namespace hapi {
// DOCUMENTATION: MpsHostHeader {{{
/*!
 * MpsHostHeader enables inlining of host-language headers that are
 * included in compilation.
 *
 * Host Headers are always only added once, even when used inside a loop, but
 * ought to be used at the top level - preferably before the actual processes.
 *
 * This enables the utilization of the host language and its libraries,
 * even when additional includes are necessary.
 * Care should be taken as the host terms do not enjoy the same type safety as
 * the native hapi terms.
 */
// }}}
class MpsHostHeader : public MpsTerm // {{{
{
  public:
    MpsHostHeader(const std::string header, const MpsTerm &succ); //
    virtual ~MpsHostHeader();

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
    MpsHostHeader *ReIndex(const std::string &session,
                     int pid, int maxpid) const;
    MpsHostHeader *PRename(const std::string &src, const std::string &dst) const;
    MpsHostHeader *ERename(const std::string &src, const std::string &dst) const;
    MpsHostHeader *PSubst(const std::string &var,
                             const MpsTerm &exp,
                             const std::vector<std::string> &args,
                             const std::vector<std::pair<int,int> > &argpids,
                             const std::vector<std::string> &stateargs) const;
    MpsHostHeader *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsHostHeader *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsHostHeader *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> FEV() const;
    MpsHostHeader *Copy() const;
    bool Terminated() const;
    MpsHostHeader *Simplify() const;
    std::string ToString(std::string indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    MpsTerm *FlattenFork(bool normLhs, bool normRhs, bool pureMode) const;
    MpsHostHeader *RenameAll() const;
    bool Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const;
    MpsTerm *Append(const MpsTerm &term) const;
    MpsTerm *CopyWrapper(std::map<std::string,void*> &children) const;
    MpsTerm *CloseDefsPre(const MpsMsgEnv &Gamma);
    MpsHostHeader *ExtractDefinitions(MpsFunctionEnv &env) const;
    std::string ToC() const;
    std::string ToCHeader() const;

  private:
    std::string myHeader;
    MpsTerm *mySucc;
}; // }}}
}
#endif
