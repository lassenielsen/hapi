// DOCUMENTATION: mpsexp.hpp {{{
/*! \file
 * This file declares the MpsExp class used to represent expressions to be used
 * in the HAPI.
 *
 * The MpsExp class is itself an abstract class, with a subclass for each
 * expression constructor.
 *
 * MpsExp objects can be created from a sourcecode string or parsetree using
 * the static Create methods.
 *
 * The operations such as substitution, typechecking, finding the possible
 * steps and finding the destination term after a step is declared as abstract
 * methods in the MpsExp class, and implemented for each subclass.
 *
 * MpsExp objects can be created from a sourcecode string using Create.
 *
 * MpsExp object can be typechecked using the TypeCheck method which returns
 * the infered type.
 *
 * Welltyped MpsExp objects can be evaluated to values using the Eval method.
 */
// }}}

#ifndef MPSEXP_HPP
#define MPSEXP_HPP

#include <typeinfo>
#include <hapi/mpstype.hpp>
#include <gmp.h>
#include <set>
#include <iostream>
#include <sstream>

namespace hapi
{

// DOCUMENTATION: MpsExp {{{
/*!
 * MpsExp represents an expression in HAPI.
 *
 * The MpsExp class is itself an abstract class, with a subclass for each
 * process constructor.
 */
// }}}
class MpsExp // {{{
{
  public:
    virtual ~MpsExp();
    virtual MpsExp *Copy() const = 0; // Make a deep copy
    virtual MpsExp *Eval() const = 0; // Evaluate to a value
    virtual MpsMsgType *TypeCheck(const MpsMsgEnv &Gamma) = 0;
    virtual bool operator==(const MpsExp &rhs) const = 0;

    virtual std::set<std::string> FV() const = 0; // Find free variables in expression
    virtual MpsExp *Rename(const std::string &src, const std::string &dst) const = 0; // Rename variable
    virtual MpsExp *Subst(const std::string &source, const MpsExp &dest) const = 0; // Substitute exp for var

    virtual std::string ToString() const= 0; // Make string representation
    //! Add C++ code that evaluates expression to dest.
    //! Returns the variable name that holds the result
    virtual std::string ToC(std::stringstream &dest, const std::string &typeName) const= 0;
    static std::string NewVar(std::string base="x");

    //! Used for dependent types
    bool ValidExp(std::vector<const MpsExp*> hyps) const;

    bool ValidExp_CFLKF(std::vector<const MpsExp*> hyps) const;

    bool ValidExp_LK(std::vector<const MpsExp*> hyps) const;
    bool ValidExp_CNF(std::vector<const MpsExp*> hyps) const;

    virtual MpsExp *MakeCNF() const = 0;
    virtual MpsExp *MakeNNF(bool negate=false) const = 0;
    virtual MpsExp *Negate() const = 0;

    virtual bool ValidCNF() const = 0;
    virtual bool ValidOR(std::set<std::string> &lits, std::set<std::string> &neglits) const = 0;

    const static std::string BNF_EXP;
    const static std::string BNF_EXPS;
    const static std::string BNF_EXPS2;
  private:
    // Used for generating new names
    static int ourNextId;

}; // }}}
class MpsVarExp : public MpsExp // {{{
{
  public:
    MpsVarExp(const std::string &name, const MpsMsgType &type);
    virtual ~MpsVarExp();
    MpsVarExp *Copy() const;
    MpsVarExp *Eval() const;
    MpsMsgType *TypeCheck(const MpsMsgEnv &Gamma);
    bool operator==(const MpsExp &rhs) const;
    std::set<std::string> FV() const;
    MpsExp *Rename(const std::string &src, const std::string &dst) const;
    MpsExp *Subst(const std::string &source, const MpsExp &dest) const;
    std::string ToString() const;
    std::string ToC(std::stringstream &dest, const std::string &typeName) const;

    MpsExp *Negate() const;
    MpsExp *MakeNNF(bool negate=false) const;
    MpsExp *MakeCNF() const;
    bool ValidOR(std::set<std::string> &lits, std::set<std::string> &neglits) const;
    bool ValidCNF() const;

  private:
    std::string myName;
    MpsMsgType *myType;
}; // }}}
class MpsIntVal : public MpsExp // {{{
{
  public:
    MpsIntVal(const mpz_t &value);
    virtual ~MpsIntVal();
    MpsIntVal *Copy() const;
    MpsIntVal *Eval() const;
    MpsMsgType *TypeCheck(const MpsMsgEnv &Gamma);
    bool operator==(const MpsExp &rhs) const;
    std::set<std::string> FV() const;
    MpsIntVal *Rename(const std::string &src, const std::string &dst) const;
    MpsIntVal *Subst(const std::string &source, const MpsExp &dest) const;
    std::string ToString() const;
    std::string ToC(std::stringstream &dest, const std::string &typeName) const;
    const mpz_t &GetValue() const;

    MpsExp *Negate() const;
    MpsExp *MakeNNF(bool negate=false) const;
    MpsExp *MakeCNF() const;
    bool ValidOR(std::set<std::string> &lits, std::set<std::string> &neglits) const;
    bool ValidCNF() const;

  private:
//    int myValue;
    mpz_t myValue;

}; // }}}
class MpsFloatVal : public MpsExp // {{{
{
  public:
    MpsFloatVal(const mpf_t &value);
    virtual ~MpsFloatVal();
    MpsFloatVal *Copy() const;
    MpsFloatVal *Eval() const;
    MpsMsgType *TypeCheck(const MpsMsgEnv &Gamma);
    bool operator==(const MpsExp &rhs) const;
    std::set<std::string> FV() const;
    MpsFloatVal *Rename(const std::string &src, const std::string &dst) const;
    MpsFloatVal *Subst(const std::string &source, const MpsExp &dest) const;
    std::string ToString() const;
    std::string ToC(std::stringstream &dest, const std::string &typeName) const;
    const mpf_t &GetValue() const;

    MpsExp *Negate() const;
    MpsExp *MakeNNF(bool negate=false) const;
    MpsExp *MakeCNF() const;
    bool ValidOR(std::set<std::string> &lits, std::set<std::string> &neglits) const;
    bool ValidCNF() const;

  private:
//    int myValue;
    mpf_t myValue;

}; // }}}
class MpsStringVal : public MpsExp // {{{
{
  public:
    MpsStringVal(const std::string &value);
    virtual ~MpsStringVal();
    MpsStringVal *Copy() const;
    MpsStringVal *Eval() const;
    MpsMsgType *TypeCheck(const MpsMsgEnv &Gamma);
    bool operator==(const MpsExp &rhs) const;
    std::set<std::string> FV() const;
    MpsStringVal *Rename(const std::string &src, const std::string &det) const;
    MpsStringVal *Subst(const std::string &source, const MpsExp &dest) const;
    std::string ToString() const;
    std::string ToC(std::stringstream &dest, const std::string &typeName) const;
    std::string GetValue() const;

    MpsExp *Negate() const;
    MpsExp *MakeNNF(bool negate=false) const;
    MpsExp *MakeCNF() const;
    bool ValidOR(std::set<std::string> &lits, std::set<std::string> &neglits) const;
    bool ValidCNF() const;

  private:
    std::string myValue;

}; // }}}
class MpsBoolVal : public MpsExp // {{{
{
  public:
    MpsBoolVal(bool value);
    virtual ~MpsBoolVal();
    MpsBoolVal *Copy() const;
    MpsBoolVal *Eval() const;
    MpsMsgType *TypeCheck(const MpsMsgEnv &Gamma);
    bool operator==(const MpsExp &rhs) const;
    std::set<std::string> FV() const;
    MpsBoolVal *Rename(const std::string &src, const std::string &dst) const;
    MpsBoolVal *Subst(const std::string &source, const MpsExp &dest) const;
    std::string ToString() const;
    std::string ToC(std::stringstream &dest, const std::string &typeName) const;
    bool GetValue() const;

    MpsExp *Negate() const;
    MpsExp *MakeNNF(bool negate=false) const;
    MpsExp *MakeCNF() const;
    bool ValidOR(std::set<std::string> &lits, std::set<std::string> &neglits) const;
    bool ValidCNF() const;

  private:
    bool myValue;

}; // }}}
class MpsCondExp : public MpsExp // {{{
{
  public:
    MpsCondExp(const MpsExp &cond, const MpsExp &truebranch, const MpsExp &falsebranch);
    virtual ~MpsCondExp();
    MpsCondExp *Copy() const;
    MpsExp *Eval() const;
    MpsMsgType *TypeCheck(const MpsMsgEnv &Gamma);
    bool operator==(const MpsExp &rhs) const;
    std::set<std::string> FV() const;
    MpsCondExp *Rename(const std::string &src, const std::string &dst) const;
    MpsCondExp *Subst(const std::string &source, const MpsExp &dest) const;
    std::string ToString() const;
    std::string ToC(std::stringstream &dest, const std::string &typeName) const;

    MpsExp *Negate() const;
    MpsExp *MakeNNF(bool negate=false) const;
    MpsExp *MakeCNF() const;
    bool ValidOR(std::set<std::string> &lits, std::set<std::string> &neglits) const;
    bool ValidCNF() const;

  private:
    MpsExp *myCond;
    MpsExp *myTrueBranch;
    MpsExp *myFalseBranch;

}; // }}}
class MpsUnOpExp : public MpsExp // {{{
{
  public:
    MpsUnOpExp(const std::string &name, const MpsExp &right);
    virtual ~MpsUnOpExp();
    MpsUnOpExp *Copy() const;
    MpsExp *Eval() const;
    MpsMsgType *TypeCheck(const MpsMsgEnv &Gamma);
    bool operator==(const MpsExp &rhs) const;
    std::set<std::string> FV() const;
    MpsUnOpExp *Rename(const std::string &src, const std::string &dst) const;
    MpsUnOpExp *Subst(const std::string &source, const MpsExp &dest) const;
    std::string ToString() const;
    std::string ToC(std::stringstream &dest, const std::string &typeName) const;
    std::string GetOp() const;
    const MpsExp &GetRight() const;
    MpsExp &GetRight();

    MpsExp *Negate() const;
    MpsExp *MakeNNF(bool negate=false) const;
    MpsExp *MakeCNF() const;
    bool ValidOR(std::set<std::string> &lits, std::set<std::string> &neglits) const;
    bool ValidCNF() const;

  private:
    std::string myName; // One of: not
    MpsExp *myRight;

}; // }}}
class MpsBinOpExp : public MpsExp // {{{
{
  public:
    MpsBinOpExp(const std::string &name, const MpsExp &left, const MpsExp &right, const MpsMsgType &leftType, const MpsMsgType &rightType);
    virtual ~MpsBinOpExp();
    MpsBinOpExp *Copy() const;
    MpsExp *Eval() const;
    MpsMsgType *TypeCheck(const MpsMsgEnv &Gamma);
    bool operator==(const MpsExp &rhs) const;
    std::set<std::string> FV() const;
    MpsBinOpExp *Rename(const std::string &src, const std::string &dst) const;
    MpsBinOpExp *Subst(const std::string &source, const MpsExp &dest) const;
    std::string ToString() const;
    std::string ToC(std::stringstream &dest, const std::string &typeName) const;
    std::string GetOp() const;
    const MpsExp &GetLeft() const;
    const MpsExp &GetRight() const;

    MpsExp *Negate() const;
    MpsExp *MakeNNF(bool negate=false) const;
    MpsExp *MakeCNF() const;
    bool ValidOR(std::set<std::string> &lits, std::set<std::string> &neglits) const;
    bool ValidCNF() const;

  private:
    std::string myName; // One of: +,-,*,/,and,or,=,<=,&
    MpsExp *myLeft;
    MpsExp *myRight;
    MpsMsgType *myLeftType;
    MpsMsgType *myRightType;
}; // }}}
class MpsTupleExp : public MpsExp // {{{
{
  public:
    MpsTupleExp(const std::vector<MpsExp*> &elements);
    virtual ~MpsTupleExp();
    MpsTupleExp *Copy() const;
    MpsTupleExp *Eval() const;
    MpsMsgType *TypeCheck(const MpsMsgEnv &Gamma);
    bool operator==(const MpsExp &rhs) const;
    std::set<std::string> FV() const;
    MpsTupleExp *Rename(const std::string &src, const std::string &dst) const;
    MpsTupleExp *Subst(const std::string &source, const MpsExp &dest) const;
    std::string ToString() const;
    std::string ToC(std::stringstream &dest, const std::string &typeName) const;
    int GetSize() const;
    const MpsExp *GetElement(int index) const;

    MpsExp *Negate() const;
    MpsExp *MakeNNF(bool negate=false) const;
    MpsExp *MakeCNF() const;
    bool ValidOR(std::set<std::string> &lits, std::set<std::string> &neglits) const;
    bool ValidCNF() const;

  private:
    std::vector<MpsExp*> myElements;

}; // }}}
class MpsSystemExp : public MpsExp // {{{
{
  public:
    MpsSystemExp(const std::string &field);
    virtual ~MpsSystemExp();
    MpsSystemExp *Copy() const;
    MpsExp *Eval() const;
    MpsMsgType *TypeCheck(const MpsMsgEnv &Gamma);
    bool operator==(const MpsExp &rhs) const;
    std::set<std::string> FV() const;
    MpsSystemExp *Rename(const std::string &src, const std::string &dst) const;
    MpsSystemExp *Subst(const std::string &source, const MpsExp &dest) const;
    std::string ToString() const;
    std::string ToC(std::stringstream &dest, const std::string &typeName) const;

    MpsExp *Negate() const;
    MpsExp *MakeNNF(bool negate=false) const;
    MpsExp *MakeCNF() const;
    bool ValidOR(std::set<std::string> &lits, std::set<std::string> &neglits) const;
    bool ValidCNF() const;

  private:
    std::string myField;
}; // }}}

inline std::string stringESubst(const std::string &name, const std::string &source, const hapi::MpsExp &dest) // {{{
{
  if (name == source) // perform substitution
  {
    if (typeid(dest) != typeid(hapi::MpsVarExp)) // Error: match with non channel expression
    {
      std::cerr << "Error Channel substitution ignoring substitution match with non channel expression: "
                << dest.ToString()
                << std::endl;
      return "_ERROR";
    }
    return dest.ToString();
  }
  return name;
} // }}}
}

#endif
