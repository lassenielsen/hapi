#ifndef MPSTERM_HPP
#define MPSTERM_HPP
// DOCUMENTATION: Main page {{{
/*! \mainpage Introduction
 * This is the documentation for the apims library. It is generated from the
 * sourcecode comments using the Doxygen tool.
 *
 * \section libapims What is libapims?
 * libapims is a library written in C++, offering parsing, typechecking,
 * stepping and compiling capabilities for the <em>apims</em> language.
 *
 * \section apims What is apims?
 * <em>apims</em> is an abreviation of Asynchronous PI-calculus with Multiparty
 * session types and Symmetric synchronization. It is a language based on the
 * pi-calculus.
 *
 * \section why Why apims?
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
 * not the case. <b>apims</b> will fix this ...
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

#include <dpl/parser.hpp>
#include <vector>
#include <map>

#include <apims/mpsexp.hpp>
#include <apims/mpschannel.hpp>
#include <apims/mpsevent.hpp>
#include <apims/mpsstep.hpp>
#include <apims/mpstype.hpp>

// DOCUMENTATION: apims {{{
/*! \namespace apims
 * apims is the namespace containing the basic functionality of the libapims
 * library.
 *
 * apims contains the classes for representing channels, expressions, events,
 * types, terms (processes), procedures and steps.
 *
 * Extra functionality such as GUI implementations should use a different
 * namespace such as mpsgui.
 */
// }}}
namespace apims
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
    virtual ~MpsTerm() {};
    // DOCUMENTATION: MpsTerm::Create {{{
    /*!
     * Creates a MpsTerm object frem a sourcecode string
     * @param exp The sourcecode string describing a process.
     * @result A MpsTerm object representing the described process.
     */
    // }}}
    static MpsTerm *Create(const std::string &exp);
    // DOCUMENTATION: MpsTerm::Create {{{
    /*!
     * Creates a MpsTerm object from a parsetree
     * @param exp The parsetree describing a process.
     * @result A MpsTerm object representing the described process.
     */
    // }}}
    static MpsTerm *Create(const dpl::parsed_tree *exp);

    // DOCUMENTATION: MpsTerm::TypeCheck {{{
    /*!
     * Static verification of communication safety using explicit types
     * @result Returns true if the process is well typed, and false otherwise.
     */
    // }}}
    bool TypeCheck();
    // DOCUMENTATION: MpsTerm::TypeCheck {{{
    /*!
     * Static verification of communication safety using explicit types
     *
     * Implementation of the judgement: Theta;Gamma |- P |> Delta.
     * Notice that the Gamma from the judgement has been split into
     * Gamma containing the global typeed names,
     * Sigma containing the simply typped names and
     * Omega containing the procedure names and types.
     *
     * @param Theta The current assertion (the currently asserted expression)
     * @param Gamma The global channel environment
     * @param Delta The session environment
     * @param Sigma The simple type environment
     * @param Omega The process environment
     * @result Returns true if the process is well typed, and false otherwise.
     */
    // }}}
    virtual bool TypeCheck(const MpsExp &Theta,
                           const MpsMsgEnv &Gamma,
                           const MpsProcEnv &Omega) = 0;

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
    //! Make a deep copy of the object
    virtual MpsTerm *Copy() const = 0; // Make a deep copy
    //! Returns true if the process is congruent to the terminated (end) process.
    virtual bool Terminated() const = 0;
    //! Simplify removes redundant (unused) parts of the process. This can be considered as garbage collection.
    virtual MpsTerm *Simplify() const = 0; // Simplify using congruence rules
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
    virtual MpsTerm *CloseDefinitions() const=0;
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
  protected:
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

}

#endif
