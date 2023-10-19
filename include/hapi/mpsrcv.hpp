#ifndef MPSRCV_HPP
#define MPSRCV_HPP
#include <hapi/mpsterm.hpp>

namespace hapi {
// DOCUMENTATION: MpsRcv {{{
/*!
 * MpsRcv is a message-receiving process.
 */
// }}}
class MpsRcv : public MpsTerm // {{{
{
  public:
    MpsRcv(const MpsChannel &channel, const std::string &dest, int pid, int maxpid, const MpsTerm &succ, const MpsMsgType &type, bool final);
    virtual ~MpsRcv();

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
    MpsTerm *ApplyRcv(const std::string &path, const MpsExp *val) const;
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
    void Split(const std::set<std::string> &fv, MpsTerm* &pre, MpsTerm* &post) const;
    MpsTerm *CopyWrapper(std::map<std::string,void*> &children) const;
    MpsTerm *CloseDefsPre(const MpsMsgEnv &Gamma);
    MpsTerm *ExtractDefinitions(MpsFunctionEnv &env) const;
    std::string ToC(const std::string &taskType) const;
    std::string ToCHeader() const;
    void ToCConsts(std::vector<std::string> &dest, std::unordered_set<std::string> &existing) const;

    const MpsMsgType &GetMsgType() const;
    void SetMsgType(const MpsMsgType &type);
    const std::string &GetDest() { return myDest; }

    bool GetFinal() const;

  private:
    //! \brief myChannel holds the channel to send on.
    MpsChannel myChannel;
    //! \brief myDest holds the name of the variable to be created and store the received value.
    std::string myDest;
    //! \brief myPid holds the process ID of received session (if it is a
    //! sessios, otherwise -1)
    int myPid;
    //! \brief myMaxPid holds the process count of received session (if it
    //! is a sessios, otherwise -1)
    int myMaxPid;
    //! \brief mySucc holds the actions following the message
    //! transmission.
    MpsTerm *mySucc;
    //! \brief myType holds type of the communicated message.
    //! This is determined by the type-checking.
    MpsMsgType *myType;
    //! \brief myFinal is true if myChannel can be closed after
    //! the transmission. This is determined by the type-checking.
    bool myFinal;
}; // }}}
}
#endif
