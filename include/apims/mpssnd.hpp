#ifndef MPSSND_HPP
#define MPSSND_HPP
#include <apims/mpsterm.hpp>

namespace apims {
// DOCUMENTATION: MpsSnd {{{
/*!
 * MpsSnd is a message-sending process.
 */
// }}}
class MpsSnd : public MpsTerm // {{{
{
  public:
    MpsSnd(const MpsChannel &channel, const MpsExp &exp, const MpsTerm &succ, const MpsMsgType &type, bool final);
    virtual ~MpsSnd();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsMsgEnv &Gamma,
                   const MpsProcEnv &Omega);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplySnd(const std::string &path, MpsExp **val, MpsChannel &ch) const;
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
    MpsTerm *CloseDefinitions() const;
    MpsTerm *ExtractDefinitions(MpsFunctionEnv &env) const;
    std::string ToC() const;
    std::string ToCHeader() const;

    const MpsMsgType &GetMsgType() const;
    void SetMsgType(const MpsMsgType &type);

    bool GetFinal() const;

  private:
    //! \brief myChannel holds the channel to send on.
    MpsChannel myChannel;
    //! \brief myExp holds the expression used to calculate the value to transmit.
    MpsExp *myExp;
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
