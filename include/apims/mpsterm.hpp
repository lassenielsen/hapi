#ifndef MPSTERM_HPP
#define MPSTERM_HPP

#include <dpl/parser.hpp>
#include <vector>
#include <map>

#include <apims/mpsexp.hpp>
#include <apims/mpschannel.hpp>
#include <apims/mpsevent.hpp>
#include <apims/mpsstep.hpp>
#include <apims/mpstype.hpp>

namespace apims
{

//! MpsTerm represents a process in the pi-calculus
//! with multi party session types.
/*! This is an abstract superclass.
 *  There is a subclass for each process constructor,
 *  including the parallel process constructor P | P.
 */

class MpsTerm // {{{
{
  public:
    virtual ~MpsTerm() {};
    //! Function for creating a term from a parsetree
    static MpsTerm *Create(const std::string &exp);
    //! Function for creating a term from source-code
    static MpsTerm *Create(const dpl::parsed_tree *exp);

    // Static verification of communication safety using explicit types
    bool TypeCheck();
    virtual bool TypeCheck(const MpsExp &Theta,
                           const MpsGlobalEnv &Gamma,
                           const MpsLocalEnv &Delta,
                           const MpsMsgEnv &Sigma,
                           const MpsProcEnv &Omega) = 0;

    // Create C code
    std::string Compile();
    virtual void Compile(std::string &decls, std::string &defs, std::string &main) = 0;

    // Makes random step, returns result and makes changes in env
    MpsTerm *Step(MpsEnv &env, 
                  std::vector<MpsFunction> &defs,
                  int &choice, int &choices);
    // Find possible evaluation steps
    // (env is const, but a technicality makes it unpractical to declare it)
    void Steps(MpsEnv &env,
               const std::vector<MpsFunction> &defs,
               std::vector<MpsStep> &dest);
    virtual bool SubSteps(std::vector<MpsStep> &dest) = 0;

    // Apply different types of steps
    virtual MpsTerm *ApplyRcv(const std::string &path, const MpsExp *val) const; // Receive val at path
    virtual MpsTerm *ApplySnd(const std::string &path, MpsExp **val, MpsChannel &ch) const; // Send val on ch at path
    virtual MpsTerm *ApplyBRcv(const std::string &path, const std::string &label) const; // Receive label at path
    virtual MpsTerm *ApplyBSnd(const std::string &path, std::string &label, MpsChannel &ch) const; // Send label at path
    virtual MpsTerm *ApplyLink(const std::vector<std::string> &paths, const std::string &session) const; // Link as sesseion at paths
    virtual MpsTerm *ApplySync(const std::vector<std::string> &paths, const std::string &label) const; // Synch on label at paths
    virtual MpsTerm *ApplyDef(const std::string &path, std::vector<MpsFunction> &dest) const; // Make definition global to dest at path
    virtual MpsTerm *ApplyCall(const std::string &path, const std::vector<MpsFunction> &funs) const; // Call function from funs at path
    virtual MpsTerm *ApplyOther(const std::string &path) const; // Call function from funs at path

    // Substitution of Process and Expression Variables
    virtual MpsTerm *PRename(const std::string &src,
                             const std::string &dst) const = 0;
    virtual MpsTerm *ERename(const std::string &src,
                             const std::string &dst) const = 0;
    virtual MpsTerm *PSubst(const std::string &var,
                            const MpsTerm &exp,
                            const std::vector<std::string> &args,
                            const std::vector<std::string> &stateargs) const = 0;
    virtual MpsTerm *ESubst(const std::string &source, const MpsExp &dest) const = 0;
    virtual MpsTerm *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const = 0;
    virtual MpsTerm *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const = 0;
    virtual std::set<std::string> FPV() const = 0;
    virtual std::set<std::string> FEV() const = 0;
    virtual MpsTerm *Copy() const = 0; // Make a deep copy
    virtual bool Terminated() const = 0; // Test if term is congruent to MpsEnd
    virtual MpsTerm *Simplify() const = 0; // Simplify using congruence rules
    // Make syntactic correct (parsable) string representation
    virtual std::string ToString(std::string indent="") const = 0;

    static std::string NewName(std::string="X");
    static MpsTerm *Error(const std::string &msg);
  protected:
    static int ourNextId;
}; // }}}
class MpsEnd : public MpsTerm // {{{
{
  public:
    MpsEnd();
    virtual ~MpsEnd();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsGlobalEnv &Gamma,
                   const MpsLocalEnv &Delta,
                   const MpsMsgEnv &Sigma,
                   const MpsProcEnv &Omega);
    void Compile(std::string &decls, std::string &defs, std::string &main);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *PRename(const std::string &src, const std::string &dst) const;
    MpsTerm *ERename(const std::string &src, const std::string &dst) const;
    MpsTerm *PSubst(const std::string &var,
                    const MpsTerm &exp,
                    const std::vector<std::string> &args,
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
}; // }}}
class MpsSnd : public MpsTerm // {{{
{
  public:
    MpsSnd(const MpsChannel &channel, const MpsExp &exp, const MpsTerm &succ);
    virtual ~MpsSnd();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsGlobalEnv &Gamma,
                   const MpsLocalEnv &Delta,
                   const MpsMsgEnv &Sigma,
                   const MpsProcEnv &Omega);
    void Compile(std::string &decls, std::string &defs, std::string &main);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplySnd(const std::string &path, MpsExp **val, MpsChannel &ch) const;
    MpsTerm *PRename(const std::string &src, const std::string &dst) const;
    MpsTerm *ERename(const std::string &src, const std::string &dst) const;
    MpsTerm *PSubst(const std::string &var, const MpsTerm &exp, const std::vector<std::string> &args, const std::vector<std::string> &stateargs) const;
    MpsTerm *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsTerm *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsTerm *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> FEV() const;
    MpsTerm *Copy() const;
    bool Terminated() const;
    MpsTerm *Simplify() const;
    std::string ToString(std::string indent="") const;

  private:
    MpsChannel myChannel;
    MpsExp *myExp;
    MpsTerm *mySucc;
}; // }}}
class MpsRcv : public MpsTerm // {{{
{
  public:
    MpsRcv(const MpsChannel &channel, const std::string &dest, const MpsTerm &succ);
    virtual ~MpsRcv();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsGlobalEnv &Gamma,
                   const MpsLocalEnv &Delta,
                   const MpsMsgEnv &Sigma,
                   const MpsProcEnv &Omega);
    void Compile(std::string &decls, std::string &defs, std::string &main);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplyRcv(const std::string &path, const MpsExp *val) const;
    MpsTerm *PRename(const std::string &src, const std::string &dst) const;
    MpsTerm *ERename(const std::string &src, const std::string &dst) const;
    MpsTerm *PSubst(const std::string &var, const MpsTerm &exp, const std::vector<std::string> &args, const std::vector<std::string> &stateargs) const;
    MpsTerm *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsTerm *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsTerm *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> FEV() const;
    MpsTerm *Copy() const;
    bool Terminated() const;
    MpsTerm *Simplify() const;
    std::string ToString(std::string indent="") const;

  private:
    MpsChannel myChannel;
    std::string myDest;
    MpsTerm *mySucc;
}; // }}}
class MpsSelect : public MpsTerm // {{{
{
  public:
    MpsSelect(const MpsChannel &channel, const std::string &label, const MpsTerm &succ);
    virtual ~MpsSelect();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsGlobalEnv &Gamma,
                   const MpsLocalEnv &Delta,
                   const MpsMsgEnv &Sigma,
                   const MpsProcEnv &Omega);
    void Compile(std::string &decls, std::string &defs, std::string &main);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplyBSnd(const std::string &path, std::string &label, MpsChannel &ch) const;
    MpsTerm *PRename(const std::string &src, const std::string &dst) const;
    MpsTerm *ERename(const std::string &src, const std::string &dst) const;
    MpsTerm *PSubst(const std::string &var, const MpsTerm &exp, const std::vector<std::string> &args, const std::vector<std::string> &stateargs) const;
    MpsTerm *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsTerm *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsTerm *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> FEV() const;
    MpsTerm *Copy() const;
    bool Terminated() const;
    MpsTerm *Simplify() const;
    std::string ToString(std::string indent="") const;

  private:
    MpsChannel myChannel;
    std::string myLabel;
    MpsTerm *mySucc;
}; // }}}
class MpsBranch : public MpsTerm // {{{
{
  public:
    MpsBranch(const MpsChannel &channel, const std::map< std::string, MpsTerm*> &branches, const std::map<std::string,MpsExp*> &assertions);
    virtual ~MpsBranch();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsGlobalEnv &Gamma,
                   const MpsLocalEnv &Delta,
                   const MpsMsgEnv &Sigma,
                   const MpsProcEnv &Omega);
    void Compile(std::string &decls, std::string &defs, std::string &main);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplyBRcv(const std::string &path, const std::string &label) const;
    MpsTerm *PRename(const std::string &src, const std::string &dst) const;
    MpsTerm *ERename(const std::string &src, const std::string &dst) const;
    MpsTerm *PSubst(const std::string &var, const MpsTerm &exp, const std::vector<std::string> &args, const std::vector<std::string> &stateargs) const;
    MpsTerm *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsTerm *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsTerm *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> FEV() const;
    MpsTerm *Copy() const;
    bool Terminated() const;
    MpsTerm *Simplify() const;
    std::string ToString(std::string indent="") const;

  private:
    MpsChannel myChannel;
    std::map< std::string, MpsTerm*> myBranches;
    std::map< std::string, MpsExp*> myAssertions;
}; // }}}
class MpsPar : public MpsTerm // {{{
{
  public:
    MpsPar(const MpsTerm &left, const MpsTerm &right);
    virtual ~MpsPar();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsGlobalEnv &Gamma,
                   const MpsLocalEnv &Delta,
                   const MpsMsgEnv &Sigma,
                   const MpsProcEnv &Omega);
    void Compile(std::string &decls, std::string &defs, std::string &main);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplyRcv(const std::string &path, const MpsExp *val) const;
    MpsTerm *ApplySnd(const std::string &path, MpsExp **val, MpsChannel &ch) const;
    MpsTerm *ApplyBRcv(const std::string &path, const std::string &label) const;
    MpsTerm *ApplyBSnd(const std::string &path, std::string &label, MpsChannel &ch) const;
    MpsTerm *ApplyLink(const std::vector<std::string> &paths, const std::string &session) const;
    MpsTerm *ApplySync(const std::vector<std::string> &paths, const std::string &label) const;
    MpsTerm *ApplyDef(const std::string &path, std::vector<MpsFunction> &dest) const;
    MpsTerm *ApplyCall(const std::string &path, const std::vector<MpsFunction> &funs) const;
    MpsTerm *ApplyOther(const std::string &path) const;
    MpsTerm *PRename(const std::string &src, const std::string &dst) const;
    MpsTerm *ERename(const std::string &src, const std::string &dst) const;
    MpsTerm *PSubst(const std::string &var, const MpsTerm &exp, const std::vector<std::string> &args, const std::vector<std::string> &stateargs) const;
    MpsTerm *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsTerm *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsTerm *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> FEV() const;
    MpsTerm *Copy() const;
    bool Terminated() const;
    MpsTerm *Simplify() const;
    std::string ToString(std::string indent="") const;

  private:
    MpsTerm *myLeft;
    MpsTerm *myRight;
}; // }}}
class MpsDef : public MpsTerm // {{{
{
  public:
    MpsDef(const std::string &name, const std::vector<std::string> &args, const std::vector<MpsMsgType*> &types, const std::vector<std::string> &stateargs, const std::vector<MpsMsgType*> &statetypes, const MpsTerm &body, const MpsTerm &succ);
    virtual ~MpsDef();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsGlobalEnv &Gamma,
                   const MpsLocalEnv &Delta,
                   const MpsMsgEnv &Sigma,
                   const MpsProcEnv &Omega);
    void Compile(std::string &decls, std::string &defs, std::string &main);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplyDef(const std::string &path, std::vector<MpsFunction> &dest) const;
    MpsTerm *PRename(const std::string &src, const std::string &dst) const;
    MpsTerm *ERename(const std::string &src, const std::string &dst) const;
    MpsTerm *PSubst(const std::string &var, const MpsTerm &exp, const std::vector<std::string> &args, const std::vector<std::string> &stateargs) const;
    MpsTerm *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsTerm *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsTerm *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> FEV() const;
    MpsTerm *Copy() const;
    bool Terminated() const;
    MpsTerm *Simplify() const;
    std::string ToString(std::string indent="") const;

  private:
    std::string myName;
    std::vector<std::string> myStateArgs;
    std::vector<MpsMsgType*> myStateTypes;
    std::vector<std::string> myArgs;
    std::vector<MpsMsgType*> myTypes;
    MpsTerm *myBody;
    MpsTerm *mySucc;
}; // }}}
class MpsCall : public MpsTerm // {{{
{
  public:
    MpsCall(const std::string &name, const std::vector<MpsExp*> &args, const std::vector<MpsExp*> &state);
    virtual ~MpsCall();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsGlobalEnv &Gamma,
                   const MpsLocalEnv &Delta,
                   const MpsMsgEnv &Sigma,
                   const MpsProcEnv &Omega);
    void Compile(std::string &decls, std::string &defs, std::string &main);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplyCall(const std::string &path, const std::vector<MpsFunction> &funs) const;
    MpsTerm *PRename(const std::string &src, const std::string &dst) const;
    MpsTerm *ERename(const std::string &src, const std::string &dst) const;
    MpsTerm *PSubst(const std::string &var, const MpsTerm &exp, const std::vector<std::string> &args, const std::vector<std::string> &stateargs) const;
    MpsTerm *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsTerm *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsTerm *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> FEV() const;
    MpsTerm *Copy() const;
    bool Terminated() const;
    MpsTerm *Simplify() const;
    std::string ToString(std::string indent="") const;

  private:
    std::string myName;
    std::vector<MpsExp*> myState;
    std::vector<MpsExp*> myArgs;
}; // }}}
class MpsNu : public MpsTerm // {{{
{
  public:
    MpsNu(const std::string &channel, const MpsTerm &succ, const MpsGlobalType &type);
    virtual ~MpsNu();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsGlobalEnv &Gamma,
                   const MpsLocalEnv &Delta,
                   const MpsMsgEnv &Sigma,
                   const MpsProcEnv &Omega);
    void Compile(std::string &decls, std::string &defs, std::string &main);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplyOther(const std::string &path) const;
    MpsTerm *PRename(const std::string &src, const std::string &dst) const;
    MpsTerm *ERename(const std::string &src, const std::string &dst) const;
    MpsTerm *PSubst(const std::string &var, const MpsTerm &exp, const std::vector<std::string> &args, const std::vector<std::string> &stateargs) const;
    MpsTerm *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsTerm *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsTerm *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> FEV() const;

    MpsTerm *Copy() const;
    bool Terminated() const;
    MpsTerm *Simplify() const;
    std::string ToString(std::string indent="") const;

  private:
    std::string myChannel;
    MpsTerm *mySucc;
    MpsGlobalType *myType;
}; // }}}
class MpsLink : public MpsTerm // {{{
{
  public:
    MpsLink(const std::string &channel, const std::string &session, int pid, int maxpid, const MpsTerm &succ);
    virtual ~MpsLink();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsGlobalEnv &Gamma,
                   const MpsLocalEnv &Delta,
                   const MpsMsgEnv &Sigma,
                   const MpsProcEnv &Omega);
    void Compile(std::string &decls, std::string &defs, std::string &main);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplyLink(const std::vector<std::string> &paths, const std::string &session) const;
    MpsTerm *PRename(const std::string &src, const std::string &dst) const;
    MpsTerm *ERename(const std::string &src, const std::string &dst) const;
    MpsTerm *PSubst(const std::string &var, const MpsTerm &exp, const std::vector<std::string> &args, const std::vector<std::string> &stateargs) const;
    MpsTerm *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsTerm *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsTerm *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> FEV() const;
    MpsTerm *Copy() const;
    bool Terminated() const;
    MpsTerm *Simplify() const;
    std::string ToString(std::string indent="") const;

  private:
    std::string myChannel;
    std::string mySession;
    MpsTerm *mySucc;
    int myMaxpid;
    int myPid;
}; // }}}
class MpsSync : public MpsTerm // {{{
{
  public:
    MpsSync(int maxpid, const std::string &session,  const std::map< std::string, MpsTerm*> &branches, const std::map<std::string,MpsExp*> &assertions);
    virtual ~MpsSync();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsGlobalEnv &Gamma,
                   const MpsLocalEnv &Delta,
                   const MpsMsgEnv &Sigma,
                   const MpsProcEnv &Omega);
    void Compile(std::string &decls, std::string &defs, std::string &main);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplySync(const std::vector<std::string> &paths, const std::string &label) const;
    MpsTerm *PRename(const std::string &src, const std::string &dst) const;
    MpsTerm *ERename(const std::string &src, const std::string &dst) const;
    MpsTerm *PSubst(const std::string &var, const MpsTerm &exp, const std::vector<std::string> &args, const std::vector<std::string> &stateargs) const;
    MpsTerm *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsTerm *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsTerm *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> FEV() const;
    MpsTerm *Copy() const;
    bool Terminated() const;
    MpsTerm *Simplify() const;
    std::string ToString(std::string indent="") const;

  private:
    std::string mySession;
    std::map<std::string, MpsTerm*> myBranches;
    int myMaxpid;
    std::map<std::string, MpsExp*> myAssertions;
}; // }}}
class MpsCond : public MpsTerm // {{{
{
  public:
    MpsCond(const MpsExp &cond, const MpsTerm &truebranch, const MpsTerm &falsebranch);
    virtual ~MpsCond();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsGlobalEnv &Gamma,
                   const MpsLocalEnv &Delta,
                   const MpsMsgEnv &Sigma,
                   const MpsProcEnv &Omega);
    void Compile(std::string &decls, std::string &defs, std::string &main);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplyOther(const std::string &path) const;
    MpsTerm *PRename(const std::string &src, const std::string &dst) const;
    MpsTerm *ERename(const std::string &src, const std::string &dst) const;
    MpsTerm *PSubst(const std::string &var, const MpsTerm &exp, const std::vector<std::string> &args, const std::vector<std::string> &stateargs) const;
    MpsTerm *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsTerm *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsTerm *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> FEV() const;
    MpsTerm *Copy() const;
    bool Terminated() const;
    MpsTerm *Simplify() const;
    std::string ToString(std::string indent="") const;

  private:
    MpsExp *myCond;
    MpsTerm *myTrueBranch;
    MpsTerm *myFalseBranch;
}; // }}}
class inputbranch // {{{
{
  public:
    MpsTerm *term;
    MpsExp *assertion;
    // names are used to keep the original variable name of field for the gui input label
    std::vector< std::string > names;
    // arg : type = value are the fields of the branch input definitions
    std::vector< std::string > args;
    std::vector< MpsMsgType* > types;
    std::vector< MpsExp* > values;
}; // }}}
class MpsGuiSync : public MpsTerm // {{{
{
  public:
    MpsGuiSync(int maxpid, const std::string &session, int pid, const std::map< std::string, inputbranch> &branches);
    virtual ~MpsGuiSync();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsGlobalEnv &Gamma,
                   const MpsLocalEnv &Delta,
                   const MpsMsgEnv &Sigma,
                   const MpsProcEnv &Omega);
    void Compile(std::string &decls, std::string &defs, std::string &main);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplySync(const std::vector<std::string> &paths, const std::string &label) const;
    MpsTerm *PRename(const std::string &src, const std::string &dst) const;
    MpsTerm *ERename(const std::string &src, const std::string &dst) const;
    MpsGuiSync *PSubst(const std::string &var, const MpsTerm &exp, const std::vector<std::string> &args, const std::vector<std::string> &stateargs) const;
    MpsGuiSync *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsGuiSync *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsGuiSync *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> FEV() const;
    MpsGuiSync *Copy() const;
    bool Terminated() const;
    MpsGuiSync *Simplify() const;
    std::string ToString(std::string indent="") const;

  private:
    std::string mySession;
    std::map< std::string, inputbranch > myBranches;
    int myMaxpid;
    int myPid;
}; // }}}
class MpsGuiValue : public MpsTerm // {{{
{
  public:
    MpsGuiValue(int maxpid, const std::string &session, int pid, const MpsExp &name, const MpsExp &value, const MpsTerm &succ);
    virtual ~MpsGuiValue();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsGlobalEnv &Gamma,
                   const MpsLocalEnv &Delta,
                   const MpsMsgEnv &Sigma,
                   const MpsProcEnv &Omega);
    void Compile(std::string &decls, std::string &defs, std::string &main);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplyOther(const std::string &path) const;
    MpsTerm *PRename(const std::string &src, const std::string &dst) const;
    MpsTerm *ERename(const std::string &src, const std::string &dst) const;
    MpsGuiValue *PSubst(const std::string &var, const MpsTerm &exp, const std::vector<std::string> &args, const std::vector<std::string> &stateargs) const;
    MpsGuiValue *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsGuiValue *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsGuiValue *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> FEV() const;
    MpsGuiValue *Copy() const;
    bool Terminated() const;
    MpsGuiValue *Simplify() const;
    std::string ToString(std::string indent="") const;

  private:
    MpsExp *myName;
    MpsExp *myValue;
    std::string mySession;
    MpsTerm *mySucc;
    int myMaxpid;
    int myPid;
}; // }}}
class MpsAssign : public MpsTerm // {{{
{
  public:
    MpsAssign(const std::string &id, const MpsExp &exp, const MpsMsgType &type, const MpsTerm &succ);
    virtual ~MpsAssign();

    bool TypeCheck(const MpsExp &Theta,
                   const MpsGlobalEnv &Gamma,
                   const MpsLocalEnv &Delta,
                   const MpsMsgEnv &Sigma,
                   const MpsProcEnv &Omega);
    void Compile(std::string &decls, std::string &defs, std::string &main);
    bool SubSteps(std::vector<MpsStep> &dest);
    MpsTerm *ApplyOther(const std::string &path) const;
    MpsAssign *PRename(const std::string &src, const std::string &dst) const;
    MpsAssign *ERename(const std::string &src, const std::string &dst) const;
    MpsAssign *PSubst(const std::string &var, const MpsTerm &exp, const std::vector<std::string> &args, const std::vector<std::string> &stateargs) const;
    MpsAssign *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsAssign *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsAssign *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    std::set<std::string> FPV() const;
    std::set<std::string> FEV() const;
    MpsAssign *Copy() const;
    bool Terminated() const;
    MpsAssign *Simplify() const;
    std::string ToString(std::string indent="") const;

  private:
    std::string myId;
    MpsExp *myExp;
    MpsMsgType *myType;
    MpsTerm *mySucc;
}; // }}}

}

#endif
