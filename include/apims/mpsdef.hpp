#ifndef MPSDEF_HPP
#define MPSDEF_HPP
#include <apims/mpsterm.hpp>
namespace apims {
// DOCUMENTATION: MpsDef {{{
/*!
 * MpsDef defines a (possibly) recursive procedure.
 */
// }}}
class MpsDef : public MpsTerm // {{{
{
  public:
    MpsDef(const std::string &name, const std::vector<std::string> &args, const std::vector<MpsMsgType*> &types, const std::vector<std::string> &stateargs, const std::vector<MpsMsgType*> &statetypes, const MpsTerm &body, const MpsTerm &succ, const MpsMsgEnv &env);
    virtual ~MpsDef();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsMsgEnv &Gamma,
                   const MpsProcEnv &Omega);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplyDef(const std::string &path, std::vector<MpsFunction> &dest) const;
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
    void Split(const std::set<std::string> &fv, MpsTerm* &pre, MpsTerm* &post) const;
    MpsTerm *CloseDefinitions() const;
    MpsTerm *ExtractDefinitions(MpsFunctionEnv &env) const;
    std::string ToC() const;
    std::string ToCHeader() const;

    std::vector<std::pair<int,int> > GetArgPids() const;

  private:
    static std::vector<std::pair<int,int> > GetArgPids(const std::vector<MpsMsgType*> &argTypes);
    std::string myName;
    std::vector<std::string> myStateArgs;
    std::vector<MpsMsgType*> myStateTypes;
    std::vector<std::string> myArgs;
    std::vector<MpsMsgType*> myTypes;
    MpsMsgEnv myEnv;
    MpsTerm *myBody;
    MpsTerm *mySucc;
}; // }}}
}
#endif
