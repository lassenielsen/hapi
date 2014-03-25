#ifndef MPSTYPE_HPP
#define MPSTYPE_HPP

#include <dpl/parser.hpp>
#include <string>
#include <map>
#include <vector>
#include <apims/common.hpp>

namespace apims
{

class MpsExp;

class MpsGlobalType;
class MpsLocalType;
class MpsMsgType;

class TypeArg // {{{
{
  public:
    TypeArg(const std::string &name, const MpsMsgType &type, const MpsExp &value);
    TypeArg(const TypeArg &rhs);
    TypeArg &operator=(const TypeArg &rhs);
    virtual ~TypeArg();
    std::string ToString(const std::string &indent="") const;

    std::string myName;
    MpsMsgType *myType;
    MpsExp *myValue;
}; // }}}

// DOCUMENTATION: MpsParticipant {{{
//! \brief MpsParticipant represents a participant in a multiparty session type.
/*!
 * MpsParticipantGlobalType represents a participant global or local multiparty
 * session type, and holds all information on that participant such as id, name
 * and parameters.
 */
// }}}
class MpsParticipant // {{{
{
  public:
    MpsParticipant(int id, const std::string &name, bool pure) : myId(id), myName(name), myPure(pure) {}
    virtual ~MpsParticipant() {}

    int GetId() const {return myId;}
    const std::string &GetName() const {return myName;}
    bool IsPure() const {return myPure;}
    bool operator==(const MpsParticipant &rhs) const {return GetId()==rhs.GetId() && GetName()==rhs.GetName() && IsPure()==rhs.IsPure();}
    bool operator!=(const MpsParticipant &rhs) const {return !((*this)==rhs);}

    static MpsParticipant Create(const dpl::parsed_tree &exp) // {{{
    { // participant ::= mode int
      std::string id=exp.content[1]->root.content;
      return MpsParticipant(string2int(id),id,MpsParticipant::CreateMode(*exp.content[0],false));
    } // }}}
    static bool CreateMode(const dpl::parsed_tree &exp, bool d) // {{{
    { // mode ::= | pure | impure
      if (exp.type_name == "mode" && exp.case_name == "case1")
        return d;
      else if (exp.type_name == "mode" && exp.case_name == "case2") // pure
        return true;
      else if (exp.type_name == "mode" && exp.case_name == "case3") // impure
        return false;
      else throw std::string("MpsParticipant::CreateMode: Unknown pconstructor: ") + exp.type_name + " case: " + exp.case_name;
    
      return d;
    } // }}}
    

  private:
    int myId;
    std::string myName;
    bool myPure;
}; // }}}

// DOCUMENTATION: MpsGlobalType {{{
/*!
 * MpsGlobalType represents a multiparty session type describing the
 * communication structure by orchestration.
 *
 * This is an abstract superclass.
 * There is a subclass for each expression constructor.
 */
// }}}
class MpsGlobalType // {{{
{
  public:
    // Parser
    static MpsGlobalType *Create(const dpl::parsed_tree *tree);

    // Constructor, Copy and operator methods
    virtual ~MpsGlobalType();
    virtual MpsGlobalType *Copy() const = 0;
    virtual bool Equal(const MpsExp &Theta, const MpsGlobalType &rhs) const = 0;
    //virtual bool operator==(const MpsGlobalType &rhs) const=0;
    // FIXME: Improve to semantical equality
    // FIXME: Implement subtyping virtual bool operator<=(const MpsGlobalType &rhs) const = 0;

    // Substitution (and helper functions)
    virtual std::set<std::string> FGV() const = 0;
    virtual std::set<std::string> FLV() const = 0;
    virtual std::set<std::string> FEV() const = 0;
    virtual MpsGlobalType *GRename(const std::string &from, const std::string &to) const = 0;
    virtual MpsGlobalType *LRename(const std::string &from, const std::string &to) const = 0;
    virtual MpsGlobalType *ERename(const std::string &source, const std::string &dest) const = 0;
    virtual MpsGlobalType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const = 0;
    virtual MpsGlobalType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const = 0;
    virtual MpsGlobalType *ESubst(const std::string &source, const MpsExp &dest) const = 0;
    virtual MpsGlobalType *RenameAll() const = 0;

    //! Make parsable string representation
    virtual std::string ToString(const std::string &indent="") const = 0;
    //! Make string representation with latex markup
    virtual std::string ToTex(int indent=0, int sw=2) const = 0;
    //! Project type to obtain local type for given participant
    virtual MpsLocalType *Project(int pid) const = 0;
    //! Return process id in type
    virtual int GetMaxPid() const=0;
    //! Generate (totally) new global variable name
    static std::string NewGVar(std::string orig="$X");
    
    // Grammar
    const static std::string BNF_TVALS;
    const static std::string BNF_TARG;
    const static std::string BNF_TARGS;
    const static std::string BNF_TARGS2;
    const static std::string BNF_TARGS3;
    const static std::string BNF_ASSERTION;
    const static std::string BNF_NAMEDASSERTION;
    const static std::string BNF_LABEL;
    const static std::string BNF_GTYPE;
    const static std::string BNF_GBRANCHES;

  private:
    static int ourNextId;
}; // }}}

class MpsGlobalMsgType : public MpsGlobalType // {{{
{
  public:
    MpsGlobalMsgType(int sender, int receiver, const MpsMsgType &msg, const MpsGlobalType &succ);
    MpsGlobalMsgType(int sender, int receiver, const MpsMsgType &msg, const MpsGlobalType &succ, const MpsExp &assertion, const std::string &id);
    virtual ~MpsGlobalMsgType();
    MpsGlobalMsgType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsGlobalType &rhs) const;
    //bool operator==(const MpsGlobalType &rhs) const;

    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsGlobalMsgType *GRename(const std::string &from, const std::string &to) const;
    MpsGlobalMsgType *LRename(const std::string &from, const std::string &to) const;
    MpsGlobalMsgType *ERename(const std::string &source, const std::string &dest) const;
    MpsGlobalMsgType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsGlobalMsgType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsGlobalMsgType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsGlobalMsgType *RenameAll() const;

    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    MpsLocalType *Project(int pid) const;
    int GetMaxPid() const;

  private:
    int mySender;
    int myReceiver;
    MpsMsgType *myMsgType;
    MpsGlobalType *mySucc;

    bool myAssertionType;
    std::string myId;
    MpsExp *myAssertion;
}; // }}}
class MpsGlobalBranchType : public MpsGlobalType // {{{
{
  public:
    MpsGlobalBranchType(int sender, int receiver, const std::map<std::string,MpsGlobalType*> &branches, const std::map<std::string,MpsExp*> &assertions);
    virtual ~MpsGlobalBranchType();
    MpsGlobalBranchType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsGlobalType &rhs) const;
    //bool operator==(const MpsGlobalType &rhs) const;

    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsGlobalBranchType *GRename(const std::string &from, const std::string &to) const;
    MpsGlobalBranchType *LRename(const std::string &from, const std::string &to) const;
    MpsGlobalBranchType *ERename(const std::string &source, const std::string &dest) const;
    MpsGlobalBranchType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsGlobalBranchType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsGlobalBranchType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsGlobalBranchType *RenameAll() const;

    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    MpsLocalType *Project(int pid) const;
    int GetMaxPid() const;

  private:
    int mySender;
    int myReceiver;
    std::map<std::string,MpsGlobalType*> myBranches;
    std::map<std::string,MpsExp*> myAssertions;
}; // }}}
class MpsGlobalRecType : public MpsGlobalType // {{{
{
  public:
    MpsGlobalRecType(const std::string &name, const MpsGlobalType &succ, const std::vector<TypeArg> &myArgs);
    virtual ~MpsGlobalRecType();
    MpsGlobalRecType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsGlobalType &rhs) const;
    //bool operator==(const MpsGlobalType &rhs) const;

    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsGlobalRecType *GRename(const std::string &from, const std::string &to) const;
    MpsGlobalRecType *LRename(const std::string &from, const std::string &to) const;
    MpsGlobalRecType *ERename(const std::string &source, const std::string &dest) const;
    MpsGlobalRecType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsGlobalRecType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsGlobalRecType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsGlobalRecType *RenameAll() const;

    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    MpsLocalType *Project(int pid) const;
    int GetMaxPid() const;

  private:
    std::string myName;
    MpsGlobalType *mySucc;
    std::vector<TypeArg> myArgs;
}; // }}}
class MpsGlobalVarType : public MpsGlobalType // {{{
{
  public:
    MpsGlobalVarType(const std::string &name, const std::vector<MpsExp*> &args);
    virtual ~MpsGlobalVarType();
    MpsGlobalVarType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsGlobalType &rhs) const;
    //bool operator==(const MpsGlobalType &rhs) const;

    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsGlobalVarType *GRename(const std::string &from, const std::string &to) const;
    MpsGlobalVarType *LRename(const std::string &from, const std::string &to) const;
    MpsGlobalVarType *ERename(const std::string &source, const std::string &dest) const;
    MpsGlobalType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsGlobalVarType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsGlobalVarType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsGlobalVarType *RenameAll() const;

    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    MpsLocalType *Project(int pid) const;
    int GetMaxPid() const;

  private:
    std::string myName;
    std::vector<MpsExp*> myValues;
}; // }}}
class MpsGlobalEndType : public MpsGlobalType // {{{
{
  public:
    MpsGlobalEndType();
    virtual ~MpsGlobalEndType();
    MpsGlobalEndType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsGlobalType &rhs) const;
    //bool operator==(const MpsGlobalType &rhs) const;

    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsGlobalEndType *GRename(const std::string &from, const std::string &to) const;
    MpsGlobalEndType *LRename(const std::string &from, const std::string &to) const;
    MpsGlobalEndType *ERename(const std::string &source, const std::string &dest) const;
    MpsGlobalEndType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsGlobalEndType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsGlobalEndType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsGlobalEndType *RenameAll() const;

    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    MpsLocalType *Project(int pid) const;
    int GetMaxPid() const;
}; // }}}
class MpsGlobalSyncType : public MpsGlobalType // {{{
{
  public:
    MpsGlobalSyncType(const std::map<std::string,MpsGlobalType*> &branches, const std::map<std::string,MpsExp*> &assertions);
    virtual ~MpsGlobalSyncType();
    MpsGlobalSyncType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsGlobalType &rhs) const;
    //bool operator==(const MpsGlobalType &rhs) const;

    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsGlobalSyncType *GRename(const std::string &from, const std::string &to) const;
    MpsGlobalSyncType *LRename(const std::string &from, const std::string &to) const;
    MpsGlobalSyncType *ERename(const std::string &source, const std::string &dest) const;
    MpsGlobalSyncType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsGlobalSyncType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsGlobalSyncType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsGlobalSyncType *RenameAll() const;

    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    MpsLocalType *Project(int pid) const;
    int GetMaxPid() const;

  private:
    std::map<std::string,MpsGlobalType*> myBranches;
    std::map<std::string,MpsExp*> myAssertions;
}; // }}}

// DOCUMENTATION: MpsLocalType {{{
/*!
 * MpsLocalType represents a session type for a single participant.
 *
 * This is an abstract superclass.
 * There is a subclass for each expression constructor.
 */
// }}}
class MpsLocalType // {{{
{
  public:
    // Parser
    static MpsLocalType *Create(const dpl::parsed_tree *tree);

    // Constructor, Copy and operator methods
    virtual ~MpsLocalType();
    virtual MpsLocalType *Copy() const=0;
    virtual bool Equal(const MpsExp &Theta, const MpsLocalType &rhs) const=0;
    //! \brief IsDone determines if the local type is equivalent to
    //! Lend.
    virtual bool IsDone() const=0;
    //FIXME: virtual bool operator==(const MpsLocalType &rhs) const=0;
    //FIXME: virtual bool operator<=(const MpsLocalType &rhs) const=0;

    // Substitution (and helper functions)
    virtual std::set<std::string> FGV() const = 0;
    virtual std::set<std::string> FLV() const = 0;
    virtual std::set<std::string> FEV() const = 0;
    virtual MpsLocalType *GRename(const std::string &from, const std::string &to) const = 0;
    virtual MpsLocalType *LRename(const std::string &from, const std::string &to) const = 0;
    virtual MpsLocalType *ERename(const std::string &from, const std::string &to) const = 0;
    virtual MpsLocalType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const = 0;
    virtual MpsLocalType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const = 0;
    virtual MpsLocalType *ESubst(const std::string &source, const MpsExp &dest) const = 0;
    virtual MpsLocalType *RenameAll() const=0;

    // Merge two types to make least supertype
    virtual MpsLocalType *Merge(MpsLocalType &rhs) const = 0;
    virtual std::string ToString(const std::string &indent="") const = 0;
    virtual std::string ToTex(int indent=0, int sw=2) const = 0;
    static std::string NewLVar(std::string basename="FUN");

    // Grammar
    const static std::string BNF_LTYPE;
    const static std::string BNF_LBRANCHES;

  private:
    static int ourNextId;
}; // }}}

class MpsLocalSendType : public MpsLocalType // {{{
{
  public:
    MpsLocalSendType(int receiver, const MpsMsgType &msgtype, const MpsLocalType &succ);
    MpsLocalSendType(int receiver, const MpsMsgType &msgtype, const MpsLocalType &succ, const MpsExp &assertion, const std::string &id);
    virtual ~MpsLocalSendType();
    MpsLocalSendType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsLocalType &rhs) const;
    bool IsDone() const;

    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsLocalSendType *GRename(const std::string &from, const std::string &to) const;
    MpsLocalSendType *LRename(const std::string &from, const std::string &to) const;
    MpsLocalSendType *ERename(const std::string &from, const std::string &to) const;
    MpsLocalSendType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsLocalSendType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsLocalSendType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsLocalSendType *RenameAll() const;

    MpsLocalType *Merge(MpsLocalType &rhs) const;
    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;

    // Accessors
    int GetReceiver() const;
    const MpsLocalType *GetSucc() const;
    const MpsMsgType *GetMsgType() const;
    bool GetAssertionType() const;
    const std::string &GetAssertionName() const;
    const MpsExp &GetAssertion() const;

  private:
    int myReceiver;
    MpsMsgType *myMsgType;
    MpsLocalType *mySucc;

    bool myAssertionType;
    std::string myId;
    MpsExp *myAssertion;
    
}; // }}}
class MpsLocalRcvType : public MpsLocalType // {{{
{
  public:
    MpsLocalRcvType(int sender, const MpsMsgType &msgtype, const MpsLocalType &succ);
    MpsLocalRcvType(int sender, const MpsMsgType &msgtype, const MpsLocalType &succ, const MpsExp &assertion, const std::string &id);
    virtual ~MpsLocalRcvType();
    MpsLocalRcvType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsLocalType &rhs) const;
    bool IsDone() const;

    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsLocalRcvType *GRename(const std::string &from, const std::string &to) const;
    MpsLocalRcvType *LRename(const std::string &from, const std::string &to) const;
    MpsLocalRcvType *ERename(const std::string &from, const std::string &to) const;
    MpsLocalRcvType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsLocalRcvType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsLocalRcvType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsLocalRcvType *RenameAll() const;

    MpsLocalType *Merge(MpsLocalType &rhs) const;
    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;

    // Accessors
    const int GetSender() const;
    const MpsLocalType *GetSucc() const;
    const MpsMsgType *GetMsgType() const;
    bool GetAssertionType() const;
    const std::string &GetAssertionName() const;
    const MpsExp &GetAssertion() const;

  private:
    int mySender;
    MpsMsgType *myMsgType;
    MpsLocalType *mySucc;

    bool myAssertionType;
    MpsExp *myAssertion;
    std::string myId;

}; // }}}
class MpsLocalForallType : public MpsLocalType // {{{
{
  public:
    MpsLocalForallType(const std::string &name, const MpsExp &assertion, const MpsLocalType &succ);
    virtual ~MpsLocalForallType();
    MpsLocalForallType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsLocalType &rhs) const;
    bool IsDone() const;

    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsLocalForallType *GRename(const std::string &from, const std::string &to) const;
    MpsLocalForallType *LRename(const std::string &from, const std::string &to) const;
    MpsLocalForallType *ERename(const std::string &from, const std::string &to) const;
    MpsLocalForallType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsLocalForallType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsLocalForallType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsLocalForallType *RenameAll() const;

    MpsLocalType *Merge(MpsLocalType &rhs) const;
    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;

    // Accessors
    const std::string &GetName() const;
    const MpsExp &GetAssertion() const;
    const MpsLocalType *GetSucc() const;

  private:
    std::string myName;
    MpsExp *myAssertion;
    MpsLocalType *mySucc;

}; // }}}
class MpsLocalSelectType : public MpsLocalType // {{{
{
  public:
    MpsLocalSelectType(int receiver, const std::map<std::string,MpsLocalType*> &branches, const std::map<std::string,MpsExp*> &assertions);
    virtual ~MpsLocalSelectType();
    MpsLocalSelectType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsLocalType &rhs) const;
    bool IsDone() const;

    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsLocalSelectType *GRename(const std::string &from, const std::string &to) const;
    MpsLocalSelectType *LRename(const std::string &from, const std::string &to) const;
    MpsLocalSelectType *ERename(const std::string &from, const std::string &to) const;
    MpsLocalSelectType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsLocalSelectType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsLocalSelectType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsLocalSelectType *RenameAll() const;

    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    MpsLocalType *Merge(MpsLocalType &rhs) const;

    // Accessors
    int GetReceiver() const;
    std::map<std::string,MpsLocalType*> &GetBranches();
    const std::map<std::string,MpsLocalType*> &GetBranches() const;
    std::map<std::string,MpsExp*> &GetAssertions();
    const std::map<std::string,MpsExp*> &GetAssertions() const;

  private:
    int myReceiver;
    std::map<std::string,MpsLocalType*> myBranches;
    std::map<std::string,MpsExp*> myAssertions;
}; // }}}
class MpsLocalBranchType : public MpsLocalType // {{{
{
  public:
    MpsLocalBranchType(int sender, const std::map<std::string,MpsLocalType*> &branches, const std::map<std::string, MpsExp*> &assertions);
    virtual ~MpsLocalBranchType();
    MpsLocalBranchType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsLocalType &rhs) const;
    bool IsDone() const;

    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsLocalBranchType *GRename(const std::string &from, const std::string &to) const;
    MpsLocalBranchType *LRename(const std::string &from, const std::string &to) const;
    MpsLocalBranchType *ERename(const std::string &from, const std::string &to) const;
    MpsLocalBranchType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsLocalBranchType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsLocalBranchType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsLocalBranchType *RenameAll() const;

    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    MpsLocalType *Merge(MpsLocalType &rhs) const;

    // Accessors
    int GetSender() const;
    std::map<std::string,MpsLocalType*> &GetBranches();
    const std::map<std::string,MpsLocalType*> &GetBranches() const;
    std::map<std::string,MpsExp*> &GetAssertions();
    const std::map<std::string,MpsExp*> &GetAssertions() const;

  private:
    int mySender;
    std::map<std::string,MpsLocalType*> myBranches;
    std::map<std::string,MpsExp*> myAssertions;
}; // }}}
class MpsLocalRecType : public MpsLocalType // {{{
{
  public:
    MpsLocalRecType(std::string name, const MpsLocalType &succ, const std::vector<TypeArg> &args);
    virtual ~MpsLocalRecType();
    MpsLocalRecType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsLocalType &rhs) const;
    bool IsDone() const;

    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsLocalRecType *GRename(const std::string &from, const std::string &to) const;
    MpsLocalRecType *LRename(const std::string &from, const std::string &to) const;
    MpsLocalRecType *ERename(const std::string &from, const std::string &to) const;
    MpsLocalRecType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsLocalRecType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsLocalRecType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsLocalRecType *RenameAll() const;

    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    MpsLocalType *Merge(MpsLocalType &rhs) const;

    // Accessors
    const MpsLocalType *GetSucc() const;
    const std::string &GetName() const;
    const std::vector<TypeArg> &GetArgs() const ;

  private:
    std::string myName;
    MpsLocalType *mySucc;
    std::vector<TypeArg> myArgs;
}; // }}}
class MpsLocalVarType : public MpsLocalType // {{{
{
  public:
    MpsLocalVarType(std::string name, const std::vector<MpsExp*> &values);
    virtual ~MpsLocalVarType();
    MpsLocalVarType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsLocalType &rhs) const;
    bool IsDone() const;

    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsLocalVarType *GRename(const std::string &from, const std::string &to) const;
    MpsLocalVarType *LRename(const std::string &from, const std::string &to) const;
    MpsLocalVarType *ERename(const std::string &from, const std::string &to) const;
    MpsLocalVarType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsLocalType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsLocalVarType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsLocalVarType *RenameAll() const;

    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    MpsLocalType *Merge(MpsLocalType &rhs) const;

  private:
    std::vector<MpsExp*> myValues;
    std::string myName;
}; // }}}
class MpsLocalEndType : public MpsLocalType // {{{
{
  public:
    MpsLocalEndType();
    virtual ~MpsLocalEndType();
    MpsLocalEndType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsLocalType &rhs) const;
    bool IsDone() const;

    std::set<std::string> FEV() const;
    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    MpsLocalEndType *GRename(const std::string &from, const std::string &to) const;
    MpsLocalEndType *LRename(const std::string &from, const std::string &to) const;
    MpsLocalEndType *ERename(const std::string &from, const std::string &to) const;
    MpsLocalEndType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsLocalEndType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsLocalEndType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsLocalEndType *RenameAll() const;

    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    MpsLocalType *Merge(MpsLocalType &rhs) const;
}; // }}}
class MpsLocalSyncType : public MpsLocalType // {{{
{
  public:
    MpsLocalSyncType(const std::map<std::string,MpsLocalType*> &branches, const std::map<std::string,MpsExp*> &assertions);
    virtual ~MpsLocalSyncType();
    MpsLocalSyncType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsLocalType &rhs) const;
    bool IsDone() const;

    std::set<std::string> FEV() const;
    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    MpsLocalSyncType *GRename(const std::string &from, const std::string &to) const;
    MpsLocalSyncType *LRename(const std::string &from, const std::string &to) const;
    MpsLocalSyncType *ERename(const std::string &from, const std::string &to) const;
    MpsLocalSyncType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsLocalSyncType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsLocalSyncType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsLocalSyncType *RenameAll() const;

    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    MpsLocalType *Merge(MpsLocalType &rhs) const;

    // Accessors
    std::map<std::string,MpsLocalType*> &GetBranches();
    const std::map<std::string,MpsLocalType*> &GetBranches() const;
    std::map<std::string,MpsExp*> &GetAssertions();
    const std::map<std::string,MpsExp*> &GetAssertions() const;

  private:
    std::map<std::string,MpsLocalType*> myBranches;
    std::map<std::string,MpsExp*> myAssertions;
}; // }}}

// DOCUMENTATION: MpsMsgType {{{
/*!
 * MpsMsgType represents the type of a single message.
 *
 * This is an abstract superclass.
 * There is a subclass for each expression constructor.
 */
// }}}
class MpsMsgType // {{{
{
  public:
    // Parser
    static MpsMsgType *Create(const dpl::parsed_tree *tree);

    // Constructor, Copy and operator methods
    virtual ~MpsMsgType();
    virtual MpsMsgType *Copy() const=0;
    virtual bool Equal(const MpsExp &Theta, const MpsMsgType &rhs) const=0;
    //virtual bool operator==(const MpsMsgType &rhs) const=0;
    // TODO: virtual bool operator<=(const MpsMsgType &rhs) const = 0;

    // Substitution methods
    virtual std::set<std::string> FLV() const=0;
    virtual std::set<std::string> FGV() const=0;
    virtual std::set<std::string> FEV() const=0;
    virtual MpsMsgType *GRename(const std::string &source, const std::string &dest) const=0;
    virtual MpsMsgType *LRename(const std::string &source, const std::string &dest) const=0;
    virtual MpsMsgType *ERename(const std::string &source, const std::string &dest) const=0;
    virtual MpsMsgType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const=0;
    virtual MpsMsgType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const=0;
    virtual MpsMsgType *ESubst(const std::string &source, const MpsExp &dest) const=0;
    virtual MpsMsgType *RenameAll() const=0;

    // Other methods
    virtual std::string ToString(const std::string &indent="") const = 0;
    virtual std::string ToTex(int indent=0, int sw=2) const = 0;
    virtual std::string ToC() const = 0;

    // Grammar
    const static std::string BNF_STYPE;
    const static std::string BNF_STYPES;
    const static std::string BNF_STYPES2;
    const static std::string BNF_MTYPE;
}; // }}}

class MpsMsgNoType : public MpsMsgType // {{{
{
  public:
    MpsMsgNoType();
    virtual ~MpsMsgNoType();
    MpsMsgNoType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsMsgType &rhs) const;
    //bool operator==(const MpsMsgType &rhs) const;

    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsMsgNoType *GRename(const std::string &source, const std::string &dest) const;
    MpsMsgNoType *LRename(const std::string &source, const std::string &dest) const;
    MpsMsgNoType *ERename(const std::string &source, const std::string &dest) const;
    MpsMsgNoType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsMsgNoType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsMsgNoType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsMsgNoType *RenameAll() const;

    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    std::string ToC() const;
}; // }}}
class MpsIntMsgType : public MpsMsgType // {{{
{
  public:
    MpsIntMsgType();
    virtual ~MpsIntMsgType();
    MpsIntMsgType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsMsgType &rhs) const;
    //bool operator==(const MpsMsgType &rhs) const;

    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsIntMsgType *GRename(const std::string &source, const std::string &dest) const;
    MpsIntMsgType *LRename(const std::string &source, const std::string &dest) const;
    MpsIntMsgType *ERename(const std::string &source, const std::string &dest) const;
    MpsIntMsgType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsIntMsgType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsIntMsgType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsIntMsgType *RenameAll() const;

    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    std::string ToC() const;
}; // }}}
class MpsStringMsgType : public MpsMsgType // {{{
{
  public:
    MpsStringMsgType();
    virtual ~MpsStringMsgType();
    MpsStringMsgType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsMsgType &rhs) const;
    //bool operator==(const MpsMsgType &rhs) const;

    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsStringMsgType *GRename(const std::string &source, const std::string &dest) const;
    MpsStringMsgType *LRename(const std::string &source, const std::string &dest) const;
    MpsStringMsgType *ERename(const std::string &source, const std::string &dest) const;
    MpsStringMsgType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsStringMsgType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsStringMsgType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsStringMsgType *RenameAll() const;

    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    std::string ToC() const;
}; // }}}
class MpsBoolMsgType : public MpsMsgType // {{{
{
  public:
    MpsBoolMsgType();
    virtual ~MpsBoolMsgType();
    MpsBoolMsgType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsMsgType &rhs) const;
    //bool operator==(const MpsMsgType &rhs) const;

    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsBoolMsgType *GRename(const std::string &source, const std::string &dest) const;
    MpsBoolMsgType *LRename(const std::string &source, const std::string &dest) const;
    MpsBoolMsgType *ERename(const std::string &source, const std::string &dest) const;
    MpsBoolMsgType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsBoolMsgType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsBoolMsgType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsBoolMsgType *RenameAll() const;

    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    std::string ToC() const;
}; // }}}
class MpsTupleMsgType : public MpsMsgType // {{{
{
  public:
    MpsTupleMsgType(const std::vector<MpsMsgType*> &elements);
    virtual ~MpsTupleMsgType();
    MpsTupleMsgType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsMsgType &rhs) const;
    //bool operator==(const MpsMsgType &rhs) const;

    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsTupleMsgType *GRename(const std::string &source, const std::string &dest) const;
    MpsTupleMsgType *LRename(const std::string &source, const std::string &dest) const;
    MpsTupleMsgType *ERename(const std::string &source, const std::string &dest) const;
    MpsTupleMsgType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsTupleMsgType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsTupleMsgType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsTupleMsgType *RenameAll() const;

    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    std::string ToC() const;

    int GetSize() const;
    const MpsMsgType *GetElement(int index) const;

  private:
    std::vector<MpsMsgType*> myElements;
}; // }}}
class MpsChannelMsgType : public MpsMsgType // {{{
{
  public:
    MpsChannelMsgType(const MpsGlobalType &type, const std::vector<MpsParticipant> &participants);
    virtual ~MpsChannelMsgType();
    MpsChannelMsgType *Copy() const;
    bool Equal(const MpsExp &Theta, const MpsMsgType &rhs) const;
    //bool operator==(const MpsMsgType &rhs) const;

    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsChannelMsgType *GRename(const std::string &source, const std::string &dest) const;
    MpsChannelMsgType *LRename(const std::string &source, const std::string &dest) const;
    MpsChannelMsgType *ERename(const std::string &source, const std::string &dest) const;
    MpsChannelMsgType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsChannelMsgType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsChannelMsgType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsChannelMsgType *RenameAll() const;

    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;
    std::string ToC() const;

    const MpsGlobalType *GetGlobalType() const {return myType;}
    MpsGlobalType *GetGlobalType() {return myType;}
    const std::vector<MpsParticipant> GetParticipants() const {return myParticipants;}
    int GetMaxPid() const {return myParticipants.size();}

  private:
    MpsGlobalType *myType;
    std::vector<MpsParticipant> myParticipants;
}; // }}}
class MpsDelegateMsgType : public MpsMsgType // {{{
{
  public:
    MpsDelegateMsgType(int pid, const std::vector<MpsParticipant> &participants);
    virtual ~MpsDelegateMsgType();
    virtual MpsDelegateMsgType *Copy() const=0;
    bool Equal(const MpsExp &Theta, const MpsMsgType &rhs) const;
    //bool operator==(const MpsMsgType &rhs) const;

    virtual std::set<std::string> FGV() const=0;
    virtual std::set<std::string> FLV() const=0;
    virtual std::set<std::string> FEV() const=0;
    virtual MpsDelegateMsgType *GRename(const std::string &source, const std::string &dest) const=0;
    virtual MpsDelegateMsgType *LRename(const std::string &source, const std::string &dest) const=0;
    virtual MpsDelegateMsgType *ERename(const std::string &source, const std::string &dest) const=0;
    virtual MpsDelegateMsgType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const=0;
    virtual MpsDelegateMsgType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const=0;
    virtual MpsDelegateMsgType *ESubst(const std::string &source, const MpsExp &dest) const=0;
    virtual MpsDelegateMsgType *RenameAll() const=0;

    virtual std::string ToString(const std::string &indent="") const=0;
    std::string ToTex(int indent=0, int sw=2) const=0;
    std::string ToC() const;

    virtual const MpsLocalType *GetLocalType() const=0;
    virtual MpsLocalType *GetLocalType()=0;
    int GetPid() const {return myPid;}
    int GetMaxpid() const {return myParticipants.size();}
    const std::vector<MpsParticipant> GetParticipants() const {return myParticipants;}

  private:
    int myPid;
    std::vector<MpsParticipant> myParticipants;
}; // }}}
class MpsDelegateLocalMsgType : public MpsDelegateMsgType // {{{
{
  public:
    MpsDelegateLocalMsgType(const MpsLocalType &myType, int pid, const std::vector<MpsParticipant> &participants);
    virtual ~MpsDelegateLocalMsgType();

    MpsDelegateLocalMsgType *Copy() const;
    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsDelegateLocalMsgType *GRename(const std::string &source, const std::string &dest) const;
    MpsDelegateLocalMsgType *LRename(const std::string &source, const std::string &dest) const;
    MpsDelegateLocalMsgType *ERename(const std::string &source, const std::string &dest) const;
    MpsDelegateLocalMsgType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsDelegateLocalMsgType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsDelegateLocalMsgType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsDelegateLocalMsgType *RenameAll() const;
    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;

    // Accessors
    const MpsLocalType *GetLocalType() const; // {return myType;}
    MpsLocalType *GetLocalType(); // {return myType;}

  private:
    MpsLocalType *myType;
}; // }}}
class MpsDelegateGlobalMsgType : public MpsDelegateMsgType // Represents Delegation with local type given as global type projectoon {{{
{
  public:
    MpsDelegateGlobalMsgType(const MpsGlobalType &type, int pid, const std::vector<MpsParticipant> &participants);
    virtual ~MpsDelegateGlobalMsgType();

    MpsDelegateGlobalMsgType *Copy() const;
    std::set<std::string> FGV() const;
    std::set<std::string> FLV() const;
    std::set<std::string> FEV() const;
    MpsDelegateGlobalMsgType *GRename(const std::string &source, const std::string &dest) const;
    MpsDelegateGlobalMsgType *LRename(const std::string &source, const std::string &dest) const;
    MpsDelegateGlobalMsgType *ERename(const std::string &source, const std::string &dest) const;
    MpsDelegateGlobalMsgType *GSubst(const std::string &source, const MpsGlobalType &dest, const std::vector<std::string> &args) const;
    MpsDelegateGlobalMsgType *LSubst(const std::string &source, const MpsLocalType &dest, const std::vector<std::string> &args) const;
    MpsDelegateGlobalMsgType *ESubst(const std::string &source, const MpsExp &dest) const;
    MpsDelegateGlobalMsgType *RenameAll() const;
    std::string ToString(const std::string &indent="") const;
    std::string ToTex(int indent=0, int sw=2) const;

    // Accessors
    const MpsGlobalType *GetGlobalType() const {return myGlobalType;}
    MpsGlobalType *GetGlobalType() {return myGlobalType;}
    const MpsLocalType *GetLocalType() const;
    MpsLocalType *GetLocalType();

  private:
    MpsGlobalType *myGlobalType;
    MpsLocalType *myLocalType;
}; // }}}

// Define environments
struct omega // {{{
{
  std::vector<MpsMsgType*> stypes;
  std::vector<std::string> snames;
  std::vector<MpsMsgType*> types;
}; // }}}

//typedef std::map<std::string,MpsGlobalType*> MpsGlobalEnv;
//typedef std::map<std::string,delta> MpsLocalEnv;
typedef std::map<std::string,MpsMsgType*> MpsMsgEnv;
typedef std::map<std::string,omega> MpsProcEnv;

inline std::string PrintGamma(const apims::MpsMsgEnv &Gamma, const std::string &indent) // {{{
{
  std::string result="";
  for (apims::MpsMsgEnv::const_iterator it=Gamma.begin();it!=Gamma.end();++it)
  {
    if (it!=Gamma.begin())
      result += ",\n" + indent;
    std::string newIndent = indent + "  ";
    for (int i=0; i<it->first.size(); ++i)
      newIndent += " ";
    result += it->first + ": " + it->second->ToString(newIndent);
  }
  return result;
} // }}}
inline std::string PrintOmega(const apims::MpsProcEnv &Omega, const std::string &indent) // {{{
{
  std::string result = "";
  for (apims::MpsProcEnv::const_iterator it=Omega.begin();it!=Omega.end();++it)
  {
    if (it!=Omega.begin())
      result += ",\n" + indent;
    result += it->first + "< ";
    std::string newIndent = indent + "  ";
    for (int i=0;i < it->first.size();++i)
      newIndent += " ";
    for (int i=0; i<it->second.stypes.size(); ++i)
    { if (i>0)
        result += ",\n" + newIndent;
      result += it->second.snames[i] + ": " + it->second.stypes[i]->ToString(newIndent);
    }
    result += " >\n"+newIndent+"( ";
    for (std::vector<apims::MpsMsgType*>::const_iterator arg=it->second.types.begin(); arg!=it->second.types.end(); ++arg)
    {
      if (arg!=it->second.types.begin())
        result += ",\n" + newIndent;
      result += (*arg)->ToString(newIndent);
    }
    result += " )";
  }
  return result;
} // }}}
}

#endif
