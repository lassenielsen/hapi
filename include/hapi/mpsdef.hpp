#ifndef MPSDEF_HPP
#define MPSDEF_HPP
#include <hapi/mpsterm.hpp>
namespace hapi {
// DOCUMENTATION: MpsDef {{{
/*!
 * MpsDef defines a (possibly) recursive procedure.
 */
// }}}
class MpsDef : public MpsTerm // {{{
{
  public:
    MpsDef(const std::string &name, const std::vector<std::string> &args, const std::vector<MpsMsgType*> &types, const std::vector<std::string> &stateargs, const std::vector<MpsMsgType*> &statetypes, const MpsTerm &body, const MpsTerm &succ, bool pure);
    virtual ~MpsDef();

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

    std::vector<std::pair<int,int> > GetArgPids() const;
    const std::vector<std::string> &GetArgs() const { return myArgs; }
    const std::vector<std::string> &GetStateArgs() const { return myStateArgs; }
    const std::vector<MpsMsgType*> &GetTypes() const { return myTypes; }
    const std::vector<MpsMsgType*> &GetStateTypes() const { return myStateTypes; }
    const std::string &GetName() const { return myName; }
    const MpsTerm *GetBody() const { return myBody; }
    MpsTerm *GetBody() { return myBody; }
    const MpsTerm *GetSucc() const { return mySucc; }
    MpsTerm *GetSucc() { return mySucc; }
    bool IsPure() { return myPure; }

  private:
    static std::vector<std::pair<int,int> > GetArgPids(const std::vector<MpsMsgType*> &argTypes);
    std::string myName;
    std::vector<std::string> myStateArgs;
    std::vector<MpsMsgType*> myStateTypes;
    std::vector<std::string> myArgs;
    std::vector<MpsMsgType*> myTypes;
    MpsTerm *myBody;
    MpsTerm *mySucc;
    bool myPure;
}; // }}}
}
#endif
