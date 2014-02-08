#ifndef MPSCALL_HPP
#define MPSCALL_HPP
#include <apims/mpsterm.hpp>
namespace apims {
// DOCUMENTATION: MpsCall {{{
/*!
 * MpsCall is a procedure-call.
 */
// }}}
class MpsCall : public MpsTerm // {{{
{
  public:
    MpsCall(const std::string &name, const std::vector<MpsExp*> &args, const std::vector<MpsExp*> &state, const std::vector<MpsMsgType*> &types, const std::vector<MpsMsgType*> &statetypes);
    virtual ~MpsCall();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsMsgEnv &Gamma,
                   const MpsProcEnv &Omega);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplyCall(const std::string &path, const std::vector<MpsFunction> &funs) const;
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

  private:
    std::string myName;
    std::vector<MpsExp*> myState;
    std::vector<MpsExp*> myArgs;
    std::vector<MpsMsgType*> myTypes;
    std::vector<MpsMsgType*> myStateTypes;
}; // }}}
}
#endif
