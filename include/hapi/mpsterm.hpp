#ifndef MPSTERM_HPP
#define MPSTERM_HPP
// DOCUMENTATION: Main page {{{
/*! \mainpage Introduction
 * This is the documentation for the hapi library. It is generated from the
 * sourcecode comments using the Doxygen tool.
 *
 * \section libhapi What is libhapi?
 * libhapi is a library written in C++, offering parsing, typechecking,
 * stepping and compiling capabilities for the <em>hapi</em> language.
 *
 * \section hapi What is hapi?
 * <em>hapi</em> is an abreviation of Asynchronous PI-calculus with Multiparty
 * session types and Symmetric synchronization. It is a language based on the
 * pi-calculus.
 *
 * \section why Why hapi?
 * In programming there are four fundamental classes of programming languages.
 *
 * Imperative languages, formalised by turing machines. <br>
 * Functional languages, formalised by the lambda-calculus. <br>
 * Logic languages, formalized by LF, and <br>
 * Process oriented languages, formalised by the pi-calculus.
 *
 * Imperative languages are have state, and are structured much like the
 * hardware that is used to execute the programs. This allows the programmer to
 * write many explicit optimizations, because the the structure of the program
 * is preserved by the compilation. Imperative languages include <b>C</b>,
 * <b>Java</b> and <b>Visual Basic</b>.
 *
 * Functional languages avoids overspecification, which allows code that is
 * more compact, easier to understand and - surprisingly - easier to
 * automatically optimize for specific purposes.  There are many mature
 * functional languages, including <b>SML</b>, <b>Haskell</b> and <b>F#</b>.
 *
 * Logic languages are not used for general purpose programming, but for
 * special domains such as writing theorem provers or type-checkers. Still
 * there are a number of logic languages including <b>Prolog</b> and
 * <b>Twelf</b>.
 *
 * Process oriented languages are inherently distributed and uses channel based
 * communication.  These features are becomming more and more important. The
 * utilization of multiple cores, (GP)GPUs, multiple CPUs, supercomputers or
 * even multiple servers across the internet is becomming more and more
 * important, as the core frequencies have stalled, while the number of cores
 * is growing rapidly.
 * With the importance of this class of languages, on might suspect that there
 * has been developed a lot of languages based on the pi-calculus, but this is
 * not the case. <b>hapi</b> will fix this ...
 *
 * 
 * \section readme Readme
 * \verbinclude Readme
 */
// }}}
// DOCUMENTATION: mpsterm.hpp {{{
/*! \file
 * This file declares the MpsTerm class used to represent terms (processes) in
 * the Asynchronous PI-calculus with multiparty sessions ans symmetric
 * synchronization.
 *
 * The MpsTerm class is itself an abstract class, with a subclass for each
 * process constructor.
 *
 * MpsTerm objects can be created from a sourcecode string or parsetree using
 * the static Create methods.
 *
 * The operations such as substitution, typechecking, finding the possible
 * steps and finding the destination term after a step is declared as abstract
 * methods in the MpsTerm class, and implemented for each subclass.
 *
 * The typical lifespan of MpsTerm objects is to be created from a sourcecode
 * string using Create.
 * Then the object it typechecked using the TypeCheck method.
 * If the typechecking succeeds, then the possible steps are found.
 * If the term can step, one of the possible steps is selected (posibly
 * randomly), and the destination term is found using the Apply method on the
 * step object. This is repeated until there are no possible steps.
 */
// }}}

#define MPSUSEGUI

#include <vector>
#include <string>
#include <map>
#include <functional>
#include <algorithm>

#include <hapi/mpsexp.hpp>
#include <hapi/mpschannel.hpp>
#include <hapi/mpsevent.hpp>
#include <hapi/mpsstep.hpp>
#include <hapi/mpstype.hpp>

// DOCUMENTATION: hapi {{{
/*! \namespace hapi
 * hapi is the namespace containing the basic functionality of the libhapi
 * library.
 *
 * hapi contains the classes for representing channels, expressions, events,
 * types, terms (processes), procedures and steps.
 *
 * Extra functionality such as GUI implementations should use a different
 * namespace such as mpsgui.
 */
// }}}
namespace hapi
{
// DOCUMENTATION: MpsTerm {{{
/*!
 * MpsTerm represents a process in the asynchronous pi-calculus with multi
 * party session types.
 *
 * The MpsTerm class is itself an abstract class, with a subclass for each
 * process constructor.
 */
// }}}
class MpsTerm // {{{
{
  public:
    //! Make a deep copy of the object
    virtual MpsTerm *Copy() const = 0; // Make a deep copy

    // DOCUMENTATION: MpsTerm::PureState {{{
    /*!
		 * PureState is amended to TypeCheck, to ensure that pure participants have
		 * syntactically ensured pure implementations.
     *
		 * CheckPure ensures the following
		 * - Implementations of pure participants are of the form
		 *   local pure X()
	   *   ( global s=new ch(p of n);
		 *     X();
		 *     |
		 *     P
		 *   )
		 *   local pure StartX(Int i)
		 *   ( if i<=0
		 *     then X();
		 *     else X(); | StartX(i-1);
		 *   )
		 *   StartX( E ); |
     *
		 * Standard typechecking ensures that
		 * - All pure participants have an immediate implementation
     * - Only one implementation of each pure participants exists (pure
     *   participants are not used in linking outside the pure implementation
     *   provided immediately.
		 * - The implementation bodies ( P ) of pure participants does not break purity ie
		 *   - Linking only with (not necessarily as) pure participants
     *   - Calling only pure defs
     *   - No impure host statements (trusting provided tags)
     *   - Communicating only on pure sessions
     */
    // }}}
    enum PureState {CPS_IMPURE=0,
		                CPS_SERVICE_DEF,
                    CPS_SERVICE_LINK,
                    CPS_SERVICE_FORK,
                    CPS_SERVICE_CALL,
                    CPS_PURE, // Also known as CPS_SERVICE_BODY,
                    CPS_INIT_DEF,
                    CPS_INIT_BRANCH,
                    CPS_INIT_BRANCH1_FORK,
                    CPS_INIT_BRANCH1_CALL1,
                    CPS_INIT_BRANCH1_CALL2,
                    CPS_INIT_BRANCH2_CALL,
                    CPS_INIT_CALL
                   };

    typedef std::function<void *(MpsTerm *term,
                                 const MpsExp &Theta,
                                 const MpsMsgEnv &Gamma,
                                 const MpsProcEnv &Omega, 
                                 const std::set<std::pair<std::string,int> > &pureStack,
                                 const std::string &curPure,
                                 PureState pureState,
                                 bool checkPure,
                                 std::map<std::string,void*> &children)> tdc_wrapper;
    typedef std::function<void *(MpsTerm *term,std::string msg,std::map<std::string,void*> &children)> tdc_wraperr;
    /************************************************
     ********** Type Driven Compilation *************
     ************************************************/
    virtual void* TDCompile(tdc_wrapper wrap,
                            tdc_wraperr wrap_err,
                            const MpsExp &Theta,
                            const MpsMsgEnv &Gamma,
                            const MpsProcEnv &Omega, 
                            const std::set<std::pair<std::string,int> > &pureStack,
                            const std::string &curPure,
                            PureState pureState,
                            bool checkPure=true) = 0;

    /************************************************
     *************** Type Checking ******************
     ************************************************/
    // DOCUMENTATION: MpsTerm::TypeCheck {{{
    /*!
     * Static verification of communication safety using explicit types
     *
     * Implementation of the judgement: Theta;Gamma |- P |> Delta.
     * Notice that Delta has been merged into Gamma. This is possible, because
     * the type domains are non-overlapping.
     *
     * @param Theta The current assertion (the currently asserted expression)
     * @param Gamma The type environment
     * @param Omega The process environment
     * @result Returns true if the process is well typed, and false otherwise.
     * @result Returns true if the process is well typed, and false otherwise.
     */
    // }}}
    bool TypeCheck();

    /************************************************
     ***************** Interpreter ******************
     ************************************************/
    // DOCUMENTATION: MpsTerm::Step {{{
    /*! 
     * Selects one of the possible steps, and return the reached process.
     * The environments are updated according to the selected step.
     * @param env The environment of the process.
     * @param choice If choice is between 0 and the number of possible steps,
     * then the given step is used. Otherwise a random step is used, and choice
     * is updated to the used index.
     * @param choices When returning, choices is set to the number of possible
     * steps.
     * @result A term representing the destination process. This is a new
     * object, and not the original MpsTerm object with updates.
     */
    // }}}
    MpsTerm *Step(MpsEnv &env, 
                  std::vector<MpsFunction> &defs,
                  int &choice, int &choices);
    // DOCUMENTATION: MpsTerm::Steps {{{
    /*!
     * Create a complete list of possible steps.
     * (env is const, but a technicality makes it unpractical to declare it)
     */
    // }}}
    void Steps(MpsEnv &env,
               const std::vector<MpsFunction> &defs,
               std::vector<MpsStep> &dest);
    // DOCUMENTATION: MpsTerm::SubSteps {{{
    /*!
     * Create a complete list of all possible partial steps.
     * (env is const, but a technicality makes it unpractical to declare it)
     */
    // }}}
    virtual bool SubSteps(std::vector<MpsStep> &dest) = 0;

    //! Apply a receive-message step.
    virtual MpsTerm *ApplyRcv(const std::string &path, const MpsExp *val) const; // Receive val at path
    //! Apply a send-message step.
    virtual MpsTerm *ApplySnd(const std::string &path, MpsExp **val, MpsChannel &ch) const; // Send val on ch at path
    //! Apply a receive-branch step.
    virtual MpsTerm *ApplyBRcv(const std::string &path, const std::string &label) const; // Receive label at path
    //! Apply a send-branch step.
    virtual MpsTerm *ApplyBSnd(const std::string &path, std::string &label, MpsChannel &ch) const; // Send label at path
    //! Apply a create-session step.
    virtual MpsTerm *ApplyLink(const std::vector<std::string> &paths, const std::string &session) const; // Link as sesseion at paths
    //! Apply a symmetric-synchronization step.
    virtual MpsTerm *ApplySync(const std::vector<std::string> &paths, const std::string &label) const; // Synch on label at paths
    //! Apply a define-procedure step.
    virtual MpsTerm *ApplyDef(const std::string &path, std::vector<MpsFunction> &dest) const; // Make definition global to dest at path
    //! Apply a call-procedure step.
    virtual MpsTerm *ApplyCall(const std::string &path, const std::vector<MpsFunction> &funs) const; // Call function from funs at path
    //! Apply another type of step.
    virtual MpsTerm *ApplyOther(const std::string &path) const; // Call function from funs at path
    //! Returns true if the process is congruent to the terminated (end) process.
    virtual bool Terminated() const = 0;
    //! Simplify removes redundant (unused) parts of the process. This can be considered as garbage collection.
    virtual MpsTerm *Simplify() const = 0; // Simplify using congruence rules

    /************************************************
     ********* Renaming and Substitution ************
     ************************************************/
    //! Reindex session, so each sender/receiver combination has its own channel
    //! This way session[i] << e becomes session[i*maxpid+pid], and
    //! session[i] >> e becomes session[i+maxpid*pid].
    virtual MpsTerm *ReIndex(const std::string &session,
                             int pid, int maxpid) const = 0;
    //! Process variable renaming
    virtual MpsTerm *PRename(const std::string &src,
                             const std::string &dst) const = 0;
    //! Expression variable renaming
    virtual MpsTerm *ERename(const std::string &src,
                             const std::string &dst) const = 0;
    //! Process variable substitution
    virtual MpsTerm *PSubst(const std::string &var,
                            const MpsTerm &exp,
                            const std::vector<std::string> &args,
                            const std::vector<std::pair<int,int> > &argpids,
                            const std::vector<std::string> &stateargs) const = 0;
    //! Expression variable substitution
    virtual MpsTerm *ESubst(const std::string &source, const MpsExp &dest) const = 0;
    //! Global type variable substitution
    virtual MpsTerm *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const = 0;
    //! Local type variable substitution
    virtual MpsTerm *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const = 0;
    //! Free process variables
    virtual std::set<std::string> FPV() const = 0;
    //! Free expression variables
    virtual std::set<std::string> FEV() const = 0;

    /************************************************
     **************** Compilation *******************
     ************************************************/
    //! Make syntactic correct (parsable) string representation of the process
    virtual std::string ToString(std::string indent="") const = 0;
    //! Make string representation of the process with latex markup
    virtual std::string ToTex(int indent=0, int sw=2) const = 0;
    // DOCUMENTATION: MpsTerm::MakeC {{{
    /*!
     * Compilation to C++ code is perfomed in 4 steps:
     * 1. Rename all variables, so no name is introduced more than once
     *    (avoid things like s>>x;t>>x;...)
     * 2. Add all free variables in function bodies to the function
     *    argument list and call agruments where it is used.
     * 3. Move all functions to the global function environment, also
     *    functions defined in function bodies
     * 4. Translate each function separately, and translate the main
     *    process as the function 'main'.
     */
    // }}}
    std::string MakeC() const;
    // DOCUMENTATION: MpsTerm::FlattenFork {{{
    /*!
     * FlattenFork rewrites terms like P1 | P2 to P1 | (def X() (P2) X()).
     * After performing CloseDefinitions and ExtractDefinitions this
     * will look like P1 | X(...), and can be implemented using
     * frameworks like pthread_create, with an API significnatly
     * different from fork.
     */
    // }}}
    virtual MpsTerm *FlattenFork(bool normLhs, bool normRhs, bool pureMode) const=0;
    // DOCUMENTATION: MpsTerm::RenameAll {{{
    /*!
     * RenameAll renames all bound variables (Process, logical and
     * expression) to a new unique name, to avoid any form of name
     * collision.
     */
    // }}}
    virtual MpsTerm *RenameAll() const=0;
    // DOCUMENTATION: MpsTerm::CloseDefinitions {{{
    /*!
     * CloseDefinitions adds all the free variables of function bodies
     * to the argument list, and adds these arguments to where the
     * function is called.
     */
    // }}}
    //MpsTerm *CloseDefinitions() { return CloseDefinitions(MpsMsgEnv()); }
    MpsTerm *CloseDefs();
    virtual MpsTerm *CloseDefsWrapper(const MpsExp &Theta,
                                      const MpsMsgEnv &Gamma,
                                      const MpsProcEnv &Omega, 
                                      const std::set<std::pair<std::string,int> > &pureStack,
                                      const std::string &curPure,
                                      MpsTerm::PureState pureState,
                                      bool checkPure,
                                      std::map<std::string,void*> &children)=0;
    // DOCUMENTATION: MpsTerm::ExtractDefinitions {{{
    /*!
     * ExtractDefinitions extracts all function definitions from the
     * term, and inserts them in the provided finction-environment.
     * This is performed bottom-up such that the definitions added to
     * the environment will themselves be free of nested definitions.
     */
    // }}}
    virtual MpsTerm *ExtractDefinitions(MpsFunctionEnv &env) const=0;
    // DOCUMENTATION: MpsTerm::ToC {{{
    /*!
     * ToC generates C++ code performing the tasks of the term.
     * This assumes that there are no function definition in term, and
     * these should be moved to a global environment beforehand using
     * ExtractDefinitions before calling ToC.
     */
    // }}}
    virtual std::string ToC() const=0;
    virtual std::string ToCHeader() const=0;
    void FreeLink(const std::string &name);
    const std::vector<std::string> &GetFreeLinks() const;

    // DOCUMENTATION: MpsTerm::NewName {{{
    /*!
     * Create a (completely) new process name. This is obtained by creating
     * names that cannot be used as process names in the syntax.
     * @param base The basename used to generate the new name. The generated
     * name will contain the basename, and this is used for fast and intuitively
     * tracking a variable though steps involving alpha-renaming.
     */
    // }}}
    static std::string NewName(std::string base="X");
    static MpsTerm *Error(const std::string &msg);

    /************************************************
     ************ Optimization methods **************
     ************************************************/
    // DOCUMENTATION: MpsTerm::Parallelize {{{
    /*!
     * Parallelize rewrites terms, to enable more parallelism (dynamically
     * using aprocs, to hit the tprocs number of parallel processes).
     * The rewriting used for this is:
     * s[n]>>x;t[m]<<e;P ->
     * if SYSTEM&"aprocs"<=SYSTEM&"tprocs"
     * then t[m]<<e;s[n]>>x;P
     * else s[n]>>x;t[m]<<e;P
     * when x not in fv(e).
     */
    // }}}
    MpsTerm *Parallelize() const;
    // DOCUMENTATION: MpsTerm::Parallelize {{{
    /*!
     * Parallelize generates two terms from the called term and returns them in the pointer references @parTerm and @seqTerm.
     * @parTerm holds the term optimized for parallelization - thus
     * postponing receives when possible.
     * @seqTerm holds the term without the parallelization optimization,
     * but where subterms (as in @parTerm) dynamically selects a
     * sequential or parallelized version depending on the number of
     * active processes and target.
     * The returned boolean value reports if any (unguarded) optimizations have
     * been performed.
     */
    // }}}
    virtual bool Parallelize(const MpsTerm &receives, MpsTerm* &seqTerm, MpsTerm* &parTerm) const=0;
    // DOCUMENTATION: MpsTerm::Split {{{
    /*!
     * Splits the term in the part that can must go before (@pre) and after
     * (@post) the free variables provided (@fv).
     */
    // }}}
    virtual void Split(const std::set<std::string> &fv, MpsTerm* &pre, MpsTerm* &post) const;
    // DOCUMENTATION: MpsTerm::Append {{{
    /*!
     * Appends the argument @term to the current term and returns the
     * result.
     */
    // }}}
    virtual MpsTerm *Append(const MpsTerm &term) const=0;

  protected:
    void CheckPure(PureState pureState, std::vector<PureState> allowedStates, std::string curPure, std::vector<std::string> &errors) // {{{
    { if (std::find(allowedStates.begin(),allowedStates.end(),pureState)==allowedStates.end())
        errors.push_back("Error in implementation of pure participant " + curPure + ". Pure implementations must conform with the structure \n     *   local X()\n	   *   ( global s=new ch(p of n);\n		 *     X();\n		 *     |\n		 *     P\n		 *   )\n		 *   local StartX(Int i)\n		 *   ( if i<=0\n		 *     then X();\n		 *     else X(); | StartX(i-1);\n		 *   )\n		 *   StartX( E ); |");
    } // }}}
    static int ourNextId;
    std::vector<std::string> myFreeLinks;
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

#define PRINTTYPEERROR(_m) PrintTypeError(_m,*this,Theta,Gamma,Omega)
inline std::string PrintTypeError(const std::string &message, const hapi::MpsTerm &term, const hapi::MpsExp &Theta,  const hapi::MpsMsgEnv &Gamma, const hapi::MpsProcEnv &Omega) // {{{
{
  std::stringstream ss;
  ss << "!!!!!!!!!!!!!!! Type Error: !!!!!!!!!!!!!!!" << std::endl
     << "!!!!!!!Term: " << term.ToString("!!!!!        ") << std::endl
     << "!Assertions: " << Theta.ToString() << std::endl
     << "!!!Type Env: " << PrintGamma(Gamma,"!!!!         ") << std::endl
     << "!Method Env: " << PrintOmega(Omega,"!!!!         ") << std::endl
     << "!!!!Message: " << message << std::endl;
  return ss.str();
} // }}}
inline void *TypeCheckRec(MpsTerm::tdc_wrapper wrap, MpsTerm::tdc_wraperr wrap_err, const hapi::MpsExp &Theta, const hapi::MpsMsgEnv &Gamma, const hapi::MpsProcEnv &Omega, const std::set<std::pair<std::string,int> > &pureStack, const std::string &curPure, MpsTerm::PureState pureState, bool checkPure, hapi::MpsTerm &term, const std::string &session) // Using new rule unfold (or eq) {{{
{ std::map<std::string,void*> children; // Dummy object for passing to wrapper functions
  hapi::MpsMsgEnv::const_iterator it=Gamma.find(session);
  if (it==Gamma.end())
  { return wrap_err(&term,PrintTypeError((std::string)"Unfolding closed session: " + session,term,Theta,Gamma,Omega),children);
  }
  const hapi::MpsDelegateMsgType *delType = dynamic_cast<const hapi::MpsDelegateMsgType*>(it->second);
  const hapi::MpsLocalRecType *type = dynamic_cast<const hapi::MpsLocalRecType*>(delType->GetLocalType());
  if (type==NULL)
  { return wrap_err(&term,PrintTypeError((std::string)"Unfolding non-rec type: " + it->second->ToString(),term,Theta,Gamma,Omega),children);
  }
  hapi::MpsMsgEnv newGamma = Gamma;
  // Create type for substitution
  std::vector<hapi::TypeArg> args;
  std::vector<std::string> argnames;
  // Create args with (x1=x1,x2=x2,...) for initial substituttion
  for (std::vector<hapi::TypeArg>::const_iterator arg=type->GetArgs().begin(); arg!=type->GetArgs().end(); ++arg)
  { hapi::MpsExp *newValue = new hapi::MpsVarExp(arg->myName, hapi::MpsMsgNoType());
    hapi::TypeArg newArg(arg->myName, *arg->myType,  *newValue);
    delete newValue;
    args.push_back(newArg);
    argnames.push_back(newArg.myName);
  }
  hapi::MpsLocalType *substType=new hapi::MpsLocalRecType(type->GetName(), *type->GetSucc(), args);
  hapi::MpsLocalType *newType = type->GetSucc()->LSubst(type->GetName(),*substType,argnames);
  delete substType;
  for (std::vector<hapi::TypeArg>::const_iterator arg=type->GetArgs().begin(); arg!=type->GetArgs().end(); ++arg)
  { hapi::MpsLocalType *tmpType = newType->ESubst(arg->myName,*arg->myValue);
    delete newType;
    newType=tmpType;
  }
  hapi::MpsDelegateLocalMsgType *newMsgType=new hapi::MpsDelegateLocalMsgType(*newType,delType->GetPid(),delType->GetParticipants());
  newGamma[session] = newMsgType;
  void *result = NULL;
  if (dynamic_cast<hapi::MpsLocalRecType*>(newType)==NULL)
    result = term.TDCompile(wrap,wrap_err,Theta,newGamma,Omega,pureStack,curPure,pureState,checkPure);
  else
    result = wrap_err(&term,PrintTypeError((std::string)"Using non-contractive type: " + it->second->ToString(),term,Theta,Gamma,Omega),children);
  delete newType;
  delete newMsgType;
  return result;
} // }}}
inline void *TypeCheckForall(MpsTerm::tdc_wrapper wrap, MpsTerm::tdc_wraperr wrap_err, const hapi::MpsExp &Theta, const hapi::MpsMsgEnv &Gamma, const hapi::MpsProcEnv &Omega, const std::set<std::pair<std::string,int> > &pureStack, const std::string &curPure, MpsTerm::PureState pureState, bool checkPure, hapi::MpsTerm &term, const std::string &session) // Using new rule forall {{{
{ std::map<std::string,void*> children; // Dummy object for passing to wrapper functions
  hapi::MpsMsgEnv::const_iterator it=Gamma.find(session);
  if (it==Gamma.end())
    return wrap_err(&term,PrintTypeError((std::string)"Forall on closed session: " + session,term,Theta,Gamma,Omega),children);
  const hapi::MpsDelegateMsgType *delType = dynamic_cast<const hapi::MpsDelegateMsgType*>(it->second);
  const hapi::MpsLocalForallType *type = dynamic_cast<const hapi::MpsLocalForallType*>(delType->GetLocalType());
  if (type==NULL)
    return wrap_err(&term,PrintTypeError((std::string)"Forall on non-forall type: " + it->second->ToString(),term,Theta,Gamma,Omega),children);
  // Find new name for bound variable
  std::string newName = hapi::MpsExp::NewVar(type->GetName());
  // Create type for substitution
  hapi::MpsLocalType *newType = type->GetSucc()->ERename(type->GetName(),newName);
  hapi::MpsDelegateLocalMsgType *newMsgType=new hapi::MpsDelegateLocalMsgType(*newType,delType->GetPid(),delType->GetParticipants());
  delete newType;
  // Create new Theta
  hapi::MpsExp *newAssertion = type->GetAssertion().Rename(type->GetName(),newName);
  hapi::MpsExp *newTheta=new hapi::MpsBinOpExp("and",Theta,*newAssertion,hapi::MpsBoolMsgType(),hapi::MpsBoolMsgType());
  delete newAssertion;
  // Create new Gamma
  hapi::MpsMsgEnv newGamma = Gamma;
  newGamma[session] = newMsgType;
  void *result = term.TDCompile(wrap,wrap_err,*newTheta,newGamma,Omega,pureStack,curPure,pureState,checkPure);
  // Clean Up
  delete newTheta;
  return result;
} // }}}

namespace tdc_wrap
{
void *check(MpsTerm *term,
            const MpsExp &Theta,
            const MpsMsgEnv &Gamma,
            const MpsProcEnv &Omega, 
            const std::set<std::pair<std::string,int> > &pureStack,
            const std::string &curPure,
            MpsTerm::PureState pureState,
            bool checkPure,
            std::map<std::string,void*> &children);
void *check_err(MpsTerm *term,
                std::string msg,
                std::map<std::string,void*> &children);
void *closedefs(MpsTerm *term,
                const MpsExp &Theta,
                const MpsMsgEnv &Gamma,
                const MpsProcEnv &Omega, 
                const std::set<std::pair<std::string,int> > &pureStack,
                const std::string &curPure,
                MpsTerm::PureState pureState,
                bool checkPure,
                std::map<std::string,void*> &children);
void *closedefs_err(MpsTerm *term,
                    std::string msg,
                    std::map<std::string,void*> &children);
}
}

#endif
