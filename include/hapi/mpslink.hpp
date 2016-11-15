#ifndef MPSLINK_HPP
#define MPSLINK_HPP
#include <hapi/mpsterm.hpp>
namespace hapi {
// DOCUMENTATION: MpsLink {{{
/*!
 * MpsLink creates a new session. If the session involves n participant the process waits for the other (n-1) participants before proceeding.
 */
// }}}
class MpsLink : public MpsTerm // {{{
{
  public:
    MpsLink(const std::string &channel, const std::string &session, int pid, int maxpid, const MpsTerm &succ, bool pure);
    virtual ~MpsLink();

    void* TDCompile(std::function<void *(MpsTerm *term,
                                         const MpsExp &Theta,
                                         const MpsMsgEnv &Gamma,
                                         const MpsProcEnv &Omega, 
                                         const std::set<std::pair<std::string,int> > &pureStack,
                                         const std::string &curPure,
                                         PureState pureState,
				                                 bool checkPure,
                                         std::map<std::string,void*> children)> wrap,
                    std::function<void *(std::string &msg)> wrap_err,
                    const MpsExp &Theta,
                    const MpsMsgEnv &Gamma,
                    const MpsProcEnv &Omega, 
                    const std::set<std::pair<std::string,int> > &pureStack,
                    const std::string &curPure,
                    PureState pureState,
				            bool checkPure=true);
    bool TypeCheck(const MpsExp &Theta,
                   const MpsMsgEnv &Gamma,
                   const MpsProcEnv &Omega,
                   const std::set<std::pair<std::string,int> > &pureStack,
                   const std::string &curPure,
									 PureState pureState,
									 bool checkPure=true);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplyLink(const std::vector<std::string> &paths, const std::string &session) const;
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
    MpsTerm *CloseDefinitions(const MpsMsgEnv &Gamma) const;
    MpsTerm *ExtractDefinitions(MpsFunctionEnv &env) const;
    std::string ToC() const;
    std::string ToCHeader() const;

    const MpsTerm *GetSucc() const { return mySucc; }
    MpsTerm *GetSucc() { return mySucc; }
    const std::string &GetChannel() const {return myChannel; }
    const std::string &GetSession() const {return mySession; }
    int GetPid() const { return myPid; }
    int GetMaxpid() const { return myMaxpid; }

  private:
    std::string myChannel;
    std::string mySession;
    MpsTerm *mySucc;
    int myMaxpid;
    int myPid;
    bool myPure; //<! The link is to a pure service, and allows certain optimizations
}; // }}}
}
#endif
