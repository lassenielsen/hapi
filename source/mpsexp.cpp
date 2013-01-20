#include <apims/mpsexp.hpp>
#include <sys/timeb.h>

using namespace std;
using namespace dpl;
using namespace apims;

#include "common.cpp"
int MpsExp::ourNextId = 1;

/* Implementation of MpsExp and subclasses
 */

/* Parsing of MpsExp from a string
 */
extern void FindExps(const parsed_tree *exp, vector<MpsExp*> &dest);

// BNFs {{{
const string MpsExp::BNF_EXP  =
" exp ::= id \
  | int \
  | string \
  | true \
  | false \
  | not exp \
  | if exp then exp else exp \
  | exp + exp \
  | exp - exp \
  | exp * exp \
  | exp / exp \
  | exp and exp \
  | exp or exp \
  | exp = exp \
  | exp <= exp \
  | ( exps ) \
  | exp & int";
const string MpsExp::BNF_EXPS ="exps ::= exps2 |";
const string MpsExp::BNF_EXPS2="exps2 ::= exp | exp , exps";
// }}}
MpsExp *MpsExp::Create(const parsed_tree *exp) // {{{
{
  if (exp->type_name == "exps" && exp->case_name == "case1") // id {{{
  {
    return Create(exp->content[0]);
  } // }}}
  if (exp->type_name == "exps2" && exp->case_name == "case1") // id {{{
  {
    return Create(exp->content[0]);
  } // }}}
  if (exp->type_name == "exp" && exp->case_name == "case1") // id {{{
  {
    return new MpsVarExp(exp->content[0]->root.content);
  } // }}}
  if (exp->type_name == "exp" && exp->case_name == "case2") // int {{{
  {
    mpz_t val;
    mpz_init_set_str(val,exp->content[0]->root.content.c_str(),10);
    MpsExp *exp=new MpsIntVal(val);
    mpz_clear(val);
    return exp;
  } // }}}
  if (exp->type_name == "exp" && exp->case_name == "case3") // string {{{
  {
    return new MpsStringVal(exp->content[0]->root.content.substr(1,exp->content[0]->root.content.size()-2));
  } // }}}
  if (exp->type_name == "exp" && exp->case_name == "case4") // true {{{
  {
    return new MpsBoolVal(true);
  } // }}}
  if (exp->type_name == "exp" && exp->case_name == "case5") // false {{{
  {
    return new MpsBoolVal(false);
  } // }}}
  if (exp->type_name == "exp" && exp->case_name == "case6") // not exp {{{
  {
    MpsExp *sub = MpsExp::Create(exp->content[1]);
    MpsExp *result = new MpsUnOpExp("not",*sub);
    delete sub;
    return result;
  } // }}}
  if (exp->type_name == "exp" && exp->case_name == "case7") // if exp then exp else exp {{{
  {
    MpsExp *cond = MpsExp::Create(exp->content[1]);
    MpsExp *truebranch = MpsExp::Create(exp->content[3]);
    MpsExp *falsebranch = MpsExp::Create(exp->content[5]);
    MpsExp *result = new MpsCondExp(*cond,*truebranch,*falsebranch);
    delete cond;
    delete truebranch;
    delete falsebranch;
    return result;
  } // }}}
  if (exp->type_name == "exp" && exp->case_name == "case8") // exp + exp {{{
  {
    MpsExp *left = MpsExp::Create(exp->content[0]);
    MpsExp *right = MpsExp::Create(exp->content[2]);
    MpsExp *result = new MpsBinOpExp("+",*left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  if (exp->type_name == "exp" && exp->case_name == "case9") // exp - exp {{{
  {
    MpsExp *left = MpsExp::Create(exp->content[0]);
    MpsExp *right = MpsExp::Create(exp->content[2]);
    MpsExp *result = new MpsBinOpExp("-",*left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  if (exp->type_name == "exp" && exp->case_name == "case10") // exp * exp {{{
  {
    MpsExp *left = MpsExp::Create(exp->content[0]);
    MpsExp *right = MpsExp::Create(exp->content[2]);
    MpsExp *result = new MpsBinOpExp("*",*left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  if (exp->type_name == "exp" && exp->case_name == "case11") // exp / exp {{{
  {
    MpsExp *left = MpsExp::Create(exp->content[0]);
    MpsExp *right = MpsExp::Create(exp->content[2]);
    MpsExp *result = new MpsBinOpExp("/",*left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  if (exp->type_name == "exp" && exp->case_name == "case12") // exp and exp {{{
  {
    MpsExp *left = MpsExp::Create(exp->content[0]);
    MpsExp *right = MpsExp::Create(exp->content[2]);
    MpsExp *result = new MpsBinOpExp("and", *left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  if (exp->type_name == "exp" && exp->case_name == "case13") // exp or exp {{{
  {
    MpsExp *left = MpsExp::Create(exp->content[0]);
    MpsExp *right = MpsExp::Create(exp->content[2]);
    MpsExp *result = new MpsBinOpExp("or",*left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  if (exp->type_name == "exp" && exp->case_name == "case14") // exp = exp {{{
  {
    MpsExp *left = MpsExp::Create(exp->content[0]);
    MpsExp *right = MpsExp::Create(exp->content[2]);
    MpsExp *result = new MpsBinOpExp("=", *left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  if (exp->type_name == "exp" && exp->case_name == "case15") // exp <= exp {{{
  {
    MpsExp *left = MpsExp::Create(exp->content[0]);
    MpsExp *right = MpsExp::Create(exp->content[2]);
    MpsExp *result = new MpsBinOpExp("<=", *left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}
  if (exp->type_name == "exp" && exp->case_name == "case16") // ( exps ) {{{
  {
    vector<MpsExp*> elements;
    elements.clear();
    FindExps(exp->content[1],elements);
    if (elements.size() == 1) // Single expression
      return elements[0]; // No Clean-up
    else
    {
      MpsTupleExp *result = new MpsTupleExp(elements);
      // Clean up
      while (elements.size()>0)
      {
        delete *elements.begin();
        elements.erase(elements.begin());
      }
      return result;
    }
  } // }}}
  if (exp->type_name == "exp" && exp->case_name == "case17") // exp & int {{{
  {
    MpsExp *left=MpsExp::Create(exp->content[0]);
    mpz_t val;
    mpz_init_set_str(val,exp->content[2]->root.content.c_str(),10);
    MpsExp *right=new MpsIntVal(val);
    mpz_clear(val);
    MpsExp *result = new MpsBinOpExp("&", *left, *right, MpsMsgNoType(), MpsMsgNoType());
    delete left;
    delete right;
    return result;
  } // }}}

  cerr << "Unknown exp-parsetree: " << exp->type_name << "." << exp->case_name << endl;
  return new MpsVarExp("_ERROR");
} // }}}
MpsExp *MpsExp::Create(const std::string &exp) // {{{
{
  // Create parser
  Parser MpsParser;
  
  /*** Define Tokens ***/
  MpsParser.DefToken("","[ \n\r\t][ \n\r\t]*",9);      // Whitespace is ignored
  // Parenthesis
  MpsParser.DefKeywordToken("(",2);
  MpsParser.DefKeywordToken(")",2);
  // Base expression keywords
  MpsParser.DefKeywordToken("true",0);                 // Base Value: boolean true
  MpsParser.DefKeywordToken("false",0);                // Base Value: boolean false
  MpsParser.DefKeywordToken("and",0);                  // Base Operation: boolean and
  MpsParser.DefKeywordToken("or",0);                   // Base Operation: boolean or
  MpsParser.DefKeywordToken("not",0);                  // Base Operation: boolean not
  MpsParser.DefKeywordToken("if",0);                   // Used for if then else expressions and terms
  MpsParser.DefKeywordToken("then",0);                 // Used for if then else expressions and terms
  MpsParser.DefKeywordToken("else",0);                 // Used for if then else expressions and terms
  MpsParser.DefKeywordToken("+",10);                   // Integer Addition
  MpsParser.DefKeywordToken("-",10);                   // Integer Addition
  MpsParser.DefKeywordToken("*",10);                   // Integer Addition
  MpsParser.DefKeywordToken("/",10);                   // Integer Addition
  MpsParser.DefToken("int", "(~)?[0-9][0-9]*",10);     // Base Value: integer
  MpsParser.DefToken("string", "\"[^\"]*\"",10);       // Base Value: string
  MpsParser.DefToken("id", "[a-z][a-zA-Z0-9_]*",10);   // Variable or channel name
  // Expression Grammar
  MpsParser.DefType(BNF_EXP);
  MpsParser.DefType(BNF_EXPS);
  MpsParser.DefType(BNF_EXPS2);

  parsed_tree *tree = MpsParser.Parse(exp);
  MpsExp *result = MpsExp::Create(tree);
  delete tree;
  return result;
} // }}}
string MpsExp::NewVar(string base) // {{{
{
  string result = (string)"~"+base;
  result += int2string(ourNextId++);
  return result;
} // }}}

/* MpsExp constructors
 */
MpsVarExp::MpsVarExp(const string &name) // {{{
{
  myName = name;
} // }}}
MpsIntVal::MpsIntVal(const mpz_t &value) // {{{
{
  mpz_init_set(myValue,value);
  //myValue = value;
} // }}}
MpsStringVal::MpsStringVal(const string &value) // {{{
{
  myValue = value;
} // }}}
MpsBoolVal::MpsBoolVal(bool value) // {{{
{
  myValue = value;
} // }}}
MpsCondExp::MpsCondExp(const MpsExp &cond, const MpsExp &truebranch, const MpsExp &falsebranch) // {{{
{
  myCond = cond.Copy();
  myTrueBranch = truebranch.Copy();
  myFalseBranch = falsebranch.Copy();
} // }}}
MpsUnOpExp::MpsUnOpExp(const string &name, const MpsExp &right) // {{{
{
  myName = name;
  myRight = right.Copy();
} // }}}
MpsBinOpExp::MpsBinOpExp(const string &name, const MpsExp &left, const MpsExp &right, const MpsMsgType &leftType, const MpsMsgType &rightType) // {{{
{
  myName = name;
  myLeft = left.Copy();
  myRight = right.Copy();
  myLeftType = leftType.Copy();
  myRightType = rightType.Copy();
} // }}}
MpsTupleExp::MpsTupleExp(const vector<MpsExp*> &elements) // {{{
{
  myElements.clear();
  for (vector<MpsExp*>::const_iterator it=elements.begin(); it!=elements.end(); ++it)
    myElements.push_back((*it)->Copy());
} // }}}

/* MpsExp destructors
 */
MpsExp::~MpsExp() // {{{
{
} // }}}
MpsVarExp::~MpsVarExp() // {{{
{
} // }}}
MpsIntVal::~MpsIntVal() // {{{
{
  mpz_clear(myValue);
} // }}}
MpsStringVal::~MpsStringVal() // {{{
{
} // }}}
MpsBoolVal::~MpsBoolVal() // {{{
{
} // }}}
MpsCondExp::~MpsCondExp() // {{{
{
  delete myCond;
  delete myTrueBranch;
  delete myFalseBranch;
} // }}}
MpsUnOpExp::~MpsUnOpExp() // {{{
{
  delete myRight;
} // }}}
MpsBinOpExp::~MpsBinOpExp() // {{{
{
  delete myLeft;
  delete myRight;
  delete myLeftType;
  delete myRightType;
} // }}}
MpsTupleExp::~MpsTupleExp() // {{{
{
  while (myElements.size()>0)
  {
    delete *myElements.begin();
    myElements.erase(myElements.begin());
  }
} // }}}

/* MpsExp Deep Copy
 */
MpsVarExp *MpsVarExp::Copy() const // {{{
{
  return new MpsVarExp(myName);
} // }}}
MpsIntVal *MpsIntVal::Copy() const // {{{
{
  return new MpsIntVal(myValue);
} // }}}
MpsStringVal *MpsStringVal::Copy() const // {{{
{
  return new MpsStringVal(myValue);
} // }}}
MpsBoolVal *MpsBoolVal::Copy() const // {{{
{
  return new MpsBoolVal(myValue);
} // }}}
MpsCondExp *MpsCondExp::Copy() const // {{{
{
  return new MpsCondExp(*myCond, *myTrueBranch, *myFalseBranch);
} // }}}
MpsUnOpExp *MpsUnOpExp::Copy() const // {{{
{
  return new MpsUnOpExp(myName, *myRight);
} // }}}
MpsBinOpExp *MpsBinOpExp::Copy() const // {{{
{
  return new MpsBinOpExp(myName, *myLeft, *myRight, *myLeftType, *myRightType);
} // }}}
MpsTupleExp *MpsTupleExp::Copy() const // {{{
{
  return new MpsTupleExp(myElements);
} // }}}

/* MpsExp Evaluation to value
 */
MpsVarExp *MpsVarExp::Eval() const // {{{
{
  return Copy();
} // }}}
MpsIntVal *MpsIntVal::Eval() const// {{{
{
  return Copy();
} // }}}
MpsStringVal *MpsStringVal::Eval() const// {{{
{
  return Copy();
} // }}}
MpsBoolVal *MpsBoolVal::Eval() const// {{{
{
  return Copy();
} // }}}
MpsExp *MpsCondExp::Eval() const// {{{
{
  MpsExp *condVal = myCond->Eval();
  MpsBoolVal *condBoolVal=dynamic_cast<MpsBoolVal*>(condVal);
  if (condBoolVal==NULL)
  {
    MpsExp *trueVal=myTrueBranch->Eval();
    MpsExp *falseVal=myFalseBranch->Eval();
    MpsExp *result=new MpsCondExp(*condVal,*trueVal,*falseVal);
    delete condVal;
    delete trueVal;
    delete falseVal;
    return result;
  }
  bool cond = condBoolVal->GetValue();
  delete condVal;
  if (cond)
    return myTrueBranch->Eval();
  else
    return myFalseBranch->Eval();
} // }}}
MpsExp *MpsUnOpExp::Eval() const// {{{
{
  MpsExp *rightval = myRight->Eval();
  MpsBoolVal *rightBoolVal = dynamic_cast<MpsBoolVal*>(rightval);
  if (myName=="not" && rightBoolVal!=NULL) // Boolean negation {{{
  {
    int result = not rightBoolVal->GetValue();
    delete rightval;
    return new MpsBoolVal(result);
  } // }}}
  MpsExp *result=new MpsUnOpExp(myName,*rightval);
  delete rightval;
  return result;
} // }}}
MpsExp *MpsBinOpExp::Eval() const// {{{
{
  MpsExp *leftVal = myLeft->Eval();
  MpsExp *rightVal = myRight->Eval();
  MpsIntVal *leftIntVal=dynamic_cast<MpsIntVal*>(leftVal);
  MpsIntVal *rightIntVal=dynamic_cast<MpsIntVal*>(rightVal);
  MpsBoolVal *leftBoolVal=dynamic_cast<MpsBoolVal*>(leftVal);
  MpsBoolVal *rightBoolVal=dynamic_cast<MpsBoolVal*>(rightVal);
  MpsTupleExp *leftTupleVal=dynamic_cast<MpsTupleExp*>(leftVal);
  if (myName=="+" && leftIntVal!=NULL && rightIntVal!=NULL) // Integer addition {{{
  {
    mpz_t sum;
    mpz_init(sum);
    mpz_add(sum,leftIntVal->GetValue(),rightIntVal->GetValue());
    MpsExp *result = new MpsIntVal(sum);
    mpz_clear(sum);
    delete leftVal;
    delete rightVal;
    return result;
  } // }}}
  else if (myName=="-" && leftIntVal!=NULL && rightIntVal!=NULL) // Integer addition {{{
  {
    mpz_t dif;
    mpz_init(dif);
    mpz_sub(dif,leftIntVal->GetValue(),rightIntVal->GetValue());
    MpsExp *result = new MpsIntVal(dif);
    mpz_clear(dif);
    delete leftVal;
    delete rightVal;
    return result;
  } // }}}
  else if (myName=="*" && leftIntVal!=NULL && rightIntVal!=NULL) // Integer addition {{{
  {
    mpz_t prod;
    mpz_init(prod);
    mpz_mul(prod,leftIntVal->GetValue(),rightIntVal->GetValue());
    MpsExp *result = new MpsIntVal(prod);
    mpz_clear(prod);
    delete leftVal;
    delete rightVal;
    return result;
  } // }}}
  else if (myName=="/" && leftIntVal!=NULL && rightIntVal!=NULL) // Integer addition {{{
  {
    mpz_t quot;
    mpz_init(quot);
    mpz_fdiv_q(quot,leftIntVal->GetValue(),rightIntVal->GetValue());
    MpsExp *result = new MpsIntVal(quot);
    mpz_clear(quot);
    delete leftVal;
    delete rightVal;
    return result;
  } // }}}
  else if (myName=="and" && leftBoolVal!=NULL && rightBoolVal!=NULL) // Boolean conjunction {{{
  {
    bool result = leftBoolVal->GetValue() && rightBoolVal->GetValue();
    delete leftVal;
    delete rightVal;
    return new MpsBoolVal(result);
  } // }}}
  else if (myName=="or" && leftBoolVal!=NULL && rightBoolVal!=NULL) // Boolean disjunction {{{
  {
    bool result = leftBoolVal->GetValue() || rightBoolVal->GetValue();
    delete leftVal;
    delete rightVal;
    return new MpsBoolVal(result);
  } // }}}
  else if (myName=="=") // Equality testing {{{
  {
    bool result = (*leftVal)==(*rightVal);
    delete leftVal;
    delete rightVal;
    return new MpsBoolVal(result);
  } // }}}
  else if (myName=="<=" && leftIntVal!=NULL && rightIntVal!=NULL) // INTEGER LEQ {{{
  { bool result = (mpz_cmp(leftIntVal->GetValue(),rightIntVal->GetValue())<=0);
    delete leftVal;
    delete rightVal;
    return new MpsBoolVal(result);
  } // }}}
  else if (myName=="&" && leftTupleVal!=NULL && rightIntVal!=NULL && leftTupleVal->GetSize()>mpz_get_ui(rightIntVal->GetValue())) // Tuple projection {{{
  { MpsExp *result=leftTupleVal->GetElement(mpz_get_ui(rightIntVal->GetValue()))->Copy();
    delete leftVal;
    delete rightVal;
    return result;
  } // }}}
  else // {{{
  { MpsExp *result = new MpsBinOpExp(myName,*leftVal,*rightVal, *myLeftType, *myRightType);
    delete leftVal;
    delete rightVal;
    return result;
  } // }}}
} // }}}
MpsTupleExp *MpsTupleExp::Eval() const// {{{
{
  vector<MpsExp*> elements;
  elements.clear();
  for (vector<MpsExp*>::const_iterator it=myElements.begin(); it!=myElements.end(); ++it)
    elements.push_back((*it)->Eval());
  MpsTupleExp *result = new MpsTupleExp(elements);
  // Clean up
  while (elements.size()>0)
  {
    delete *elements.begin();
    elements.erase(elements.begin());
  }

  return result;
} // }}}

/* Typechecking
 */
MpsMsgType *MpsVarExp::TypeCheck(const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma) // {{{
{
  MpsGlobalEnv::const_iterator it_gamma = Gamma.find(myName);
  if (it_gamma != Gamma.end())
    return new MpsChannelMsgType(*(it_gamma->second));
  MpsLocalEnv::const_iterator it_delta = Delta.find(myName);
  if (it_delta != Delta.end())
    return new MpsDelegateLocalMsgType(*it_delta->second.type, it_delta->second.pid, it_delta->second.maxpid);
  MpsMsgEnv::const_iterator it_sigma = Sigma.find(myName);
  if (it_sigma != Sigma.end())
    return it_sigma->second->Copy();
  return new MpsMsgNoType();
} // }}}
MpsMsgType *MpsIntVal::TypeCheck(const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma) // {{{
{
  return new MpsIntMsgType();
} // }}}
MpsMsgType *MpsStringVal::TypeCheck(const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma) // {{{
{
  return new MpsStringMsgType();
} // }}}
MpsMsgType *MpsBoolVal::TypeCheck(const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma) // {{{
{
  return new MpsBoolMsgType();
} // }}}
MpsMsgType *MpsCondExp::TypeCheck(const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma) // {{{
{
  MpsMsgType *result=NULL;
  MpsMsgType *condtype=myCond->TypeCheck(Gamma,Delta,Sigma);
  if (dynamic_cast<MpsBoolMsgType*>(condtype) != NULL)
  {
    MpsMsgType *truetype=myTrueBranch->TypeCheck(Gamma,Delta,Sigma);
    MpsMsgType *falsetype=myFalseBranch->TypeCheck(Gamma,Delta,Sigma);
    if (truetype->Equal(MpsBoolVal(true),*falsetype))
    {
      result=truetype;
      delete falsetype;
    }
    else
    {
      result=new MpsMsgNoType();
      delete truetype;
      delete falsetype;
    }
  }
  else
    result=new MpsMsgNoType();
  delete condtype;
  return result;
} // }}}
MpsMsgType *MpsUnOpExp::TypeCheck(const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma) // {{{
{
  if (myName == "not") // Boolean operation [bool -> bool] // {{{
  { MpsMsgType *argtype = myRight->TypeCheck(Gamma,Delta,Sigma);
    if (dynamic_cast<MpsBoolMsgType*>(argtype)!=NULL) 
      return argtype;
    else
    { delete argtype;
      return new MpsMsgNoType();
    }
  } // }}}
  else // Unknown operator
    return new MpsMsgNoType();
} // }}}
MpsMsgType *MpsBinOpExp::TypeCheck(const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma) // {{{
{
  delete myLeftType;
  myLeftType=myLeft->TypeCheck(Gamma,Delta,Sigma);
  delete myRightType;
  myRightType=myRight->TypeCheck(Gamma,Delta,Sigma);
  if (myName == "+" || // Integer operation [int -> int -> int] // {{{
      myName == "-" ||
      myName == "*" ||
      myName == "/")
  { if (dynamic_cast<MpsIntMsgType*>(myLeftType) &&
        dynamic_cast<MpsIntMsgType*>(myRightType))
      return new MpsIntMsgType();
    else
      return new MpsMsgNoType();
  } // }}}
  if (myName == "and" || // Boolean operation [bool -> bool -> bool] // {{{
      myName == "or")
  { if (dynamic_cast<MpsBoolMsgType*>(myLeftType) &&
        dynamic_cast<MpsBoolMsgType*>(myRightType))
      return new MpsBoolMsgType;
    else
      return new MpsMsgNoType();
  } // }}}
  if (myName == "=") // Boolean operation [X -> X -> bool] // {{{
  { if (((not dynamic_cast<MpsMsgNoType*>(myLeftType)) ||
         (not dynamic_cast<MpsMsgNoType*>(myRightType))) &&
        myLeftType->Equal(MpsBoolVal(true),*myRightType))
      return new MpsBoolMsgType();
    else
      return new MpsMsgNoType();
  } // }}}
  if (myName == "<=") // Integer operation [int -> int -> bool] // {{{
  { if (dynamic_cast<MpsIntMsgType*>(myLeftType) &&
        dynamic_cast<MpsIntMsgType*>(myRightType))
      return new MpsBoolMsgType();
    else
      return new MpsMsgNoType();
  } // }}}
  if (myName == "&") // Untupeling [(_,...,_,X,_,...,_] -> idx -> X {{{
  {
    MpsTupleMsgType *lhsptr = dynamic_cast<MpsTupleMsgType*>(myLeftType);
    if (lhsptr==NULL)
      return new MpsMsgNoType();
    MpsIntVal *index = dynamic_cast<MpsIntVal*>(myRight);
    if (index==NULL)
      return new MpsMsgNoType();
    unsigned long int idx = mpz_get_ui(index->GetValue());
    MpsMsgType *result = lhsptr->GetElement(idx)->Copy();
    return result;
  } // }}}
  else // Unknown operator
    return new MpsMsgNoType();
} // }}}
MpsMsgType *MpsTupleExp::TypeCheck(const MpsGlobalEnv &Gamma, const MpsLocalEnv &Delta, const MpsMsgEnv &Sigma) // {{{
{
  bool error=false;
  vector<MpsMsgType*> types;
  for (int i=0; i<myElements.size() && not error; ++i)
  {
    MpsMsgType *type=myElements[i]->TypeCheck(Gamma,Delta,Sigma);
    if (dynamic_cast<MpsMsgNoType*>(type))
    { delete type;
      error=true;
    }
    else
      types.push_back(type);
  }
  MpsMsgType *result=NULL;
  if (error)
    result = new MpsMsgNoType();
  else
    result = new MpsTupleMsgType(types);

  // Clean Up
  while (types.size()>0)
  {
    delete types.back();
    types.pop_back();
  }

  return result;
} // }}}

/* Expression equality
 */
bool MpsVarExp::operator==(const MpsExp &rhs) const // {{{
{
  if (typeid(rhs) != typeid(MpsVarExp))
    return false;
  MpsVarExp *rhsptr=(MpsVarExp*)&rhs;
  return (myName == rhsptr->myName);
} // }}}
bool MpsIntVal::operator==(const MpsExp &rhs) const // {{{
{
  if (typeid(rhs) != typeid(MpsIntVal))
    return false;
  MpsIntVal *rhsptr=(MpsIntVal*)&rhs;
  return (mpz_cmp(myValue,rhsptr->myValue)==0);
} // }}}
bool MpsStringVal::operator==(const MpsExp &rhs) const // {{{
{
  if (typeid(rhs) != typeid(MpsStringVal))
    return false;
  MpsStringVal *rhsptr=(MpsStringVal*)&rhs;
  return (myValue == rhsptr->myValue);
} // }}}
bool MpsBoolVal::operator==(const MpsExp &rhs) const // {{{
{
  if (typeid(rhs) != typeid(MpsBoolVal))
    return false;
  MpsBoolVal *rhsptr=(MpsBoolVal*)&rhs;
  return (myValue == rhsptr->myValue);
} // }}}
bool MpsCondExp::operator==(const MpsExp &rhs) const // {{{
{
  if (typeid(rhs) != typeid(MpsCondExp))
    return false;
  MpsCondExp *rhsptr=(MpsCondExp*)&rhs;
  return ((*myCond) == (*rhsptr->myCond)
       && (*myTrueBranch) == (*rhsptr->myTrueBranch)
       && (*myFalseBranch) == (*rhsptr->myFalseBranch));
} // }}}
bool MpsUnOpExp::operator==(const MpsExp &rhs) const // {{{
{
  if (typeid(rhs) != typeid(MpsUnOpExp))
    return false;
  MpsUnOpExp *rhsptr=(MpsUnOpExp*)&rhs;
  return (myName == rhsptr->myName
       && (*myRight) == (*rhsptr->myRight));
} // }}}
bool MpsBinOpExp::operator==(const MpsExp &rhs) const // {{{
{
  if (typeid(rhs) != typeid(MpsBinOpExp))
    return false;
  MpsBinOpExp *rhsptr=(MpsBinOpExp*)&rhs;
  return (myName == rhsptr->myName)
       && (*myLeft) == (*rhsptr->myLeft)
       && (*myRight) == (*rhsptr->myRight);
} // }}}
bool MpsTupleExp::operator==(const MpsExp &rhs) const // {{{
{
  if (typeid(rhs) != typeid(MpsTupleExp))
    return false;
  MpsTupleExp *rhsptr=(MpsTupleExp*)&rhs;
  if (myElements.size() != rhsptr->myElements.size())
    return false;
  for (int i=0; i<myElements.size(); ++i)
    if (!((*myElements[i])==(*rhsptr->myElements[i])))
      return false;
  return true;
} // }}}

/* Free (Expression) Variables
 */
set<string> MpsVarExp::FV() const// {{{
{
  set<string> result;
  result.clear();
  result.insert(myName);
  return result;
} // }}}
set<string> MpsIntVal::FV() const// {{{
{
  set<string> result;
  result.clear();
  return result;
} // }}}
set<string> MpsStringVal::FV() const// {{{
{
  set<string> result;
  result.clear();
  return result;
} // }}}
set<string> MpsBoolVal::FV() const// {{{
{
  set<string> result;
  result.clear();
  return result;
} // }}}
set<string> MpsCondExp::FV() const// {{{
{
  set<string> result;
  result.clear();
  set<string> cfv = myCond->FV();
  set<string> tfv = myTrueBranch->FV();
  set<string> ffv = myFalseBranch->FV();
  result.insert(cfv.begin(),cfv.end()); // Include FV from conditions
  result.insert(tfv.begin(),tfv.end()); // Include FV from true-branch
  result.insert(ffv.begin(),ffv.end()); // Include FV from false-branch
  return result;
} // }}}
set<string> MpsUnOpExp::FV() const// {{{
{
  return myRight->FV();
} // }}}
set<string> MpsBinOpExp::FV() const// {{{
{
  set<string> result;
  result.clear();
  set<string> leftfv = myLeft->FV();
  set<string> rightfv = myRight->FV();
  result.insert(leftfv.begin(),leftfv.end()); // Include FV from left child
  result.insert(rightfv.begin(),rightfv.end()); // Include FV from right child
  return result;
} // }}}
set<string> MpsTupleExp::FV() const// {{{
{
  set<string> result;
  result.clear();
  for (vector<MpsExp*>::const_iterator it=myElements.begin(); it!=myElements.end(); ++it)
  {
    set<string> fv = (*it)->FV();
    result.insert(fv.begin(),fv.end()); // Include FV from element
  }
  return result;
} // }}}

/* Renaming of variable
 */
MpsExp *MpsVarExp::Rename(const string &src, const string &dst) const // {{{
{
  if (myName == src)
    return new MpsVarExp(dst);
  else
    return Copy();
} // }}}
MpsIntVal *MpsIntVal::Rename(const string &src, const string &dst) const // {{{
{
  return Copy();
} // }}}
MpsStringVal *MpsStringVal::Rename(const string &src, const string &dst) const // {{{
{
  return Copy();
} // }}}
MpsBoolVal *MpsBoolVal::Rename(const string &src, const string &dst) const // {{{
{
  return Copy();
} // }}}
MpsCondExp *MpsCondExp::Rename(const string &src, const string &dst) const // {{{
{
  MpsExp *newCond = myCond->Rename(src,dst);
  MpsExp *newTrue = myTrueBranch->Rename(src,dst);
  MpsExp *newFalse = myFalseBranch->Rename(src,dst);
  MpsCondExp *result = new MpsCondExp(*newCond, *newTrue, *newFalse);
  delete newCond;
  delete newTrue;
  delete newFalse;
  return result;
} // }}}
MpsUnOpExp *MpsUnOpExp::Rename(const string &src, const string &dst) const // {{{
{
  MpsExp *newRight = myRight->Rename(src,dst);
  MpsUnOpExp *result = new MpsUnOpExp(myName, *newRight);
  delete newRight;
  return result;
} // }}}
MpsBinOpExp *MpsBinOpExp::Rename(const string &src, const string &dst) const // {{{
{
  MpsExp *newLeft = myLeft->Rename(src,dst);
  MpsExp *newRight = myRight->Rename(src,dst);
  MpsMsgType *newLeftType = myLeftType->ERename(src,dst);
  MpsMsgType *newRightType = myRightType->ERename(src,dst);
  MpsBinOpExp *result = new MpsBinOpExp(myName, *newLeft, *newRight, *newLeftType, *newRightType);
  delete newLeft;
  delete newRight;
  delete newLeftType;
  delete newRightType;
  return result;
} // }}}
MpsTupleExp *MpsTupleExp::Rename(const string &src, const string &dst) const // {{{
{
  vector<MpsExp*> elements;
  elements.clear();
  for (vector<MpsExp*>::const_iterator it=myElements.begin(); it!=myElements.end(); ++it)
    elements.push_back((*it)->Rename(src,dst));
  MpsTupleExp *result = new MpsTupleExp(elements);
  // Clean up
  DeleteVector(elements);

  return result;
} // }}}

/* Substitution (of expression variables)
 */
MpsExp *MpsVarExp::Subst(const string &source, const MpsExp &dest) const// {{{
{
  if (myName == source)
    return dest.Copy();
  return Copy();
} // }}}
MpsIntVal *MpsIntVal::Subst(const string &source, const MpsExp &dest) const// {{{
{
  return Copy();
} // }}}
MpsStringVal *MpsStringVal::Subst(const string &source, const MpsExp &dest) const// {{{
{
  return Copy();
} // }}}
MpsBoolVal *MpsBoolVal::Subst(const string &source, const MpsExp &dest) const// {{{
{
  return Copy();
} // }}}
MpsCondExp *MpsCondExp::Subst(const string &source, const MpsExp &dest) const// {{{
{
  MpsExp *newCond = myCond->Subst(source,dest);
  MpsExp *newTrue = myTrueBranch->Subst(source,dest);
  MpsExp *newFalse = myFalseBranch->Subst(source,dest);
  MpsCondExp *result = new MpsCondExp(*newCond, *newTrue, *newFalse);
  delete newCond;
  delete newTrue;
  delete newFalse;
  return result;
} // }}}
MpsUnOpExp *MpsUnOpExp::Subst(const string &source, const MpsExp &dest) const// {{{
{
  MpsExp *newRight = myRight->Subst(source,dest);
  MpsUnOpExp *result = new MpsUnOpExp(myName, *newRight);
  delete newRight;
  return result;
} // }}}
MpsBinOpExp *MpsBinOpExp::Subst(const string &source, const MpsExp &dest) const// {{{
{
  MpsExp *newLeft = myLeft->Subst(source,dest);
  MpsExp *newRight = myRight->Subst(source,dest);
  MpsMsgType *newLeftType = myLeftType->ESubst(source,dest);
  MpsMsgType *newRightType = myRightType->ESubst(source,dest);
  MpsBinOpExp *result = new MpsBinOpExp(myName, *newLeft, *newRight, *newLeftType, *newRightType);
  delete newLeft;
  delete newRight;
  delete newLeftType;
  delete newRightType;
  return result;
} // }}}
MpsTupleExp *MpsTupleExp::Subst(const string &source, const MpsExp &dest) const// {{{
{
  vector<MpsExp*> elements;
  elements.clear();
  for (vector<MpsExp*>::const_iterator it=myElements.begin(); it!=myElements.end(); ++it)
    elements.push_back((*it)->Subst(source,dest));
  MpsTupleExp *result = new MpsTupleExp(elements);
  // Clean up
  while (elements.size()>0)
  {
    delete *elements.begin();
    elements.erase(elements.begin());
  }

  return result;
} // }}}

/* Create parsable string representation
 */
string MpsVarExp::ToString() const // {{{
{
  return myName;
} // }}}
string MpsIntVal::ToString() const // {{{
{
  char *str=mpz_get_str(NULL,10,myValue);
  string result(str);
  free(str);
  return result;
} // }}}
string MpsStringVal::ToString() const // {{{
{
  return (string)"\"" + myValue + "\"";
} // }}}
string MpsBoolVal::ToString() const // {{{
{
  if (myValue)
    return "true";
  else
    return "false";
} // }}}
string MpsCondExp::ToString() const // {{{
{
  return (string)"if " + myCond->ToString()
              + " then " + myTrueBranch->ToString()
              + " else " + myFalseBranch->ToString();
} // }}}
string MpsUnOpExp::ToString() const // {{{
{
  return myName + " " + myRight->ToString();
} // }}}
string MpsBinOpExp::ToString() const // {{{
{
  return (string)"(" + myLeft->ToString() + " " + myName + " " + myRight->ToString() + ")";
} // }}}
string MpsTupleExp::ToString() const // {{{
{
  string result = "(";
  for (vector<MpsExp*>::const_iterator it=myElements.begin(); it!=myElements.end(); ++it)
  {
    if (it != myElements.begin())
      result += ", ";
    result += (*it)->ToString();
  }
  result += ")";
  return result;
} // }}}

/* Make C++ code that evaluates expression and stores the result in dest
 */
string MpsVarExp::ToC(const string &dest) const // {{{
{
  stringstream result;
  result << "    " << dest << "=" << ToC_Name(myName) << ";" << endl;
  return result.str();
} // }}}
string MpsIntVal::ToC(const string &dest) const // {{{
{
  char *str=mpz_get_str(NULL,10,myValue);
  string val(str);
  free(str);
  stringstream result;
  result << "    mpz_init_set_str(" << dest << ",\"" << val << "\",10);" << endl;
  return result.str();
} // }}}
string MpsStringVal::ToC(const string &dest) const // {{{
{
  stringstream result;
  result << "    " << dest << "=\"" << stuff_string(myValue) << "\";" << endl;
  return result.str();
} // }}}
string MpsBoolVal::ToC(const string &dest) const // {{{
{
  stringstream result;
  result << "    " << dest << "=";
  result << (myValue?(string)"true":(string)"false");
  result << ";" << endl;
  return result.str();
} // }}}
string MpsCondExp::ToC(const string &dest) const // {{{
{
  stringstream result;
  string newName=ToC_Name(MpsExp::NewVar("expcond"));
  result << "    bool " << newName << ";" << endl;
  result << myCond->ToC(newName);
  result << "    if (" << newName << ")" << endl
         << "    {" << endl
         << myTrueBranch->ToC(dest)
         << "    }"
         << "    else"
         << "    {"
         << myFalseBranch->ToC(dest)
         << "    }" << endl;
  return result.str();
} // }}}
string MpsUnOpExp::ToC(const string &dest) const // {{{
{
  stringstream result;
  if (myName == "not")
  { string newName = ToC_Name(MpsExp::NewVar("expnot"));
    result << "    bool " << newName << ";" << endl;
    result << myRight->ToC(newName);
    result << "    " << dest << "=" << "!" << newName << ";" << endl;
    return result.str();
  }
  else
    throw (string)"MpsUnOpExp::ToC: Unknown operation " + myName;
} // }}}
string MpsBinOpExp::ToC(const string &dest) const // {{{
{
  stringstream result;
  string leftName = ToC_Name(MpsExp::NewVar("left"));
  string rightName = ToC_Name(MpsExp::NewVar("right"));
  result << "    " << myLeftType->ToC() << " " << leftName << ";" << endl
         << "    mpz_init(" << leftName << ");" << endl
         << myLeft->ToC(leftName)
         << "    " << myRightType->ToC() << " " << rightName << ";" << endl
         << "    mpz_init(" << rightName << ");" << endl
         << myRight->ToC(rightName);
    
  // For branching on subtree types
  const MpsIntMsgType *lftIntType=dynamic_cast<const MpsIntMsgType*>(myLeftType);
  const MpsIntMsgType *rgtIntType=dynamic_cast<const MpsIntMsgType*>(myRightType);

  if (myName=="+" && lftIntType!=NULL && rgtIntType!=NULL) // Integer Addition {{{
  { result << "    mpz_add(" << dest << "," << leftName << "," << rightName << ");" << endl;
    return result.str();
  } // }}}
  else if (myName=="-" && lftIntType!=NULL && rgtIntType!=NULL) // {{{
  { result << "    mpz_sub(" << dest << "," << leftName << "," << rightName << ");" << endl;
    return result.str();
  } // }}}
  else if (myName=="<=" && lftIntType!=NULL && rgtIntType!=NULL) // {{{
  { string cmpName = ToC_Name(MpsExp::NewVar("cmp"));
    result << "    int " << cmpName << ";" << endl;
    result << "    " << cmpName << "=mpz_cmp(" << leftName << "," << rightName << ");" << endl;
    result << "    " << dest << "=" << cmpName << "<=0;" << endl;
    return result.str();
  } // }}}
  // FIXME: More operators
  return (string)"[[(" + myLeftType->ToString() + ")" + myLeft->ToString() + " " + myName + " (" + myRightType->ToString() + ")" + myRight->ToString() + "]]";
} // }}}
string MpsTupleExp::ToC(const string &dest) const // {{{
{
  stringstream result;
  int pos=0;
  for (vector<MpsExp*>::const_iterator it=myElements.begin(); it!=myElements.end(); ++it)
    result << (*it)->ToC(dest + ".x"+int2string(pos));
  return result.str();
} // }}}

/* Decide if Valid
 */
int __COUNT_PROOFS=0;
int __SUM_ATOMS=0;
int __MAX_ATOMS=-1;
int __MIN_ATOMS=-1;
int __SUM_ANDS=0;
int __SUM_ORS=0;
int __SUM_NOTS=0;
double __MAX_TIME=0;
double __SUM_TIME=0;
set<string> hypatoms(const vector<const MpsExp*> &hyps) // {{{
{ set<string> atoms;
  for (int i=0; i<hyps.size(); ++i)
  { set<string> fv=hyps[i]->FV();
    atoms.insert(fv.begin(),fv.end());
  }
  return atoms;
} // }}}
void CountConnectives(const MpsExp* exp, int &ands, int &ors, int &nots) // {{{
{ const MpsUnOpExp *unop = dynamic_cast<const MpsUnOpExp*>(exp);
  const MpsBinOpExp *binop = dynamic_cast<const MpsBinOpExp*>(exp);
  if (unop!=NULL && unop->GetOp()=="not")
    CountConnectives(&unop->GetRight(),ands,ors,++nots);
  else if (binop!=NULL && binop->GetOp()=="and")
  { CountConnectives(&binop->GetLeft(),++ands,ors,nots);
    CountConnectives(&binop->GetRight(),ands,ors,nots);
  }
  else if (binop!=NULL && binop->GetOp()=="or")
  { CountConnectives(&binop->GetLeft(),ands,++ors,nots);
    CountConnectives(&binop->GetRight(),ands,ors,nots);
  }
  return;
} // }}}
void CountHypConnectives(const vector<const MpsExp*> &hyps, int &ands, int &ors, int &nots) // {{{
{ for (int i=0; i<hyps.size(); ++i)
    CountConnectives(hyps[i],ands,ors,nots);
  return;
} // }}}
double gettime() // {{{
{ struct timeb tb;
  ftime(&tb);
  double result = tb.millitm;
  result /= 1000;
  result+= (tb.time % (60*60));
  return result;
} // }}}
bool MpsExp::ValidExp(vector<const MpsExp*> hyps) const // {{{
{
//  // STATISTICS {{{
//  cout << "PROOFS: " << ++__COUNT_PROOFS << endl;
//  set<string> atoms=hypatoms(hyps);
//  set<string> fv=FV();
//  atoms.insert(fv.begin(),fv.end());
//  int size=atoms.size();
//  __SUM_ATOMS+=size;
//  cout << "ATOMS SUM: " << __SUM_ATOMS << endl;
//  if (__MIN_ATOMS==-1 || size<__MIN_ATOMS)
//    __MIN_ATOMS=size;
//  cout << "MIN ATOMS: " << __MIN_ATOMS << endl;
//  if (size>__MAX_ATOMS)
//    __MAX_ATOMS=size;
//  cout << "MAX ATOMS: " << __MAX_ATOMS << endl;
//  int ands=0;
//  int ors=0;
//  int nots=0;
//  CountHypConnectives(hyps,ands,ors,nots);
//  CountConnectives(this,ands,ors,nots);
//  __SUM_ANDS+=ands;
//  cout << "ANDS SUM: " << __SUM_ANDS << endl;
//  __SUM_ORS+=ors;
//  cout << "ORS SUM: " << __SUM_ORS << endl;
//  __SUM_NOTS+=nots;
//  cout << "NOTS SUM: " << __SUM_NOTS << endl;
//  double starttime=gettime();
//  // END STATISTICS }}}
  bool result=ValidExp_CFLKF(hyps);
//  // STATISTICS {{{
//  double endtime=gettime();
//  double runtime=endtime-starttime;
//  __SUM_TIME+=runtime;
//  if (runtime>__MAX_TIME)
//    __MAX_TIME=runtime;
//  cout << "RUNTIME SUM: " << __SUM_TIME << endl;
//  cout << "RUNTIME MAX: " << __MAX_TIME << endl;
//  // END STATISTICS }}}
  return result;
} // }}}

inline bool isP(const MpsExp *e) // {{{
{ const MpsVarExp *varE = dynamic_cast<const MpsVarExp*>(e);
  const MpsBoolVal *valE = dynamic_cast<const MpsBoolVal*>(e);
  const MpsBinOpExp *binE = dynamic_cast<const MpsBinOpExp*>(e);
  return valE!=NULL || varE!=NULL || (binE!=NULL && binE->GetOp()=="and");
} // }}}
inline bool isL(const MpsExp *e, string &name, bool &negated) // {{{
{ const MpsVarExp *varE = dynamic_cast<const MpsVarExp*>(e);
  if (varE!=NULL)
  { name=varE->ToString();
    negated=false;
    return true;
  }
  const MpsUnOpExp *unE = dynamic_cast<const MpsUnOpExp*>(e);
  if (unE!=NULL && unE->GetOp()=="not")
  { varE = dynamic_cast<const MpsVarExp*>(&unE->GetRight());
    if (varE!=NULL)
    { name = varE->ToString();
      negated=true;
      return true;
    }
    cerr << "isL: Not in NNF" << e->ToString() << endl;
  }
  return false;
} // }}}
inline bool isC(const MpsExp *e) // {{{
{ const MpsVarExp *varE = dynamic_cast<const MpsVarExp*>(e);
  const MpsBoolVal *valE = dynamic_cast<const MpsBoolVal*>(e);
  const MpsBinOpExp *binE = dynamic_cast<const MpsBinOpExp*>(e);
  const MpsUnOpExp *unE = dynamic_cast<const MpsUnOpExp*>(e);
  return valE!=NULL || varE!=NULL || (binE!=NULL && binE->GetOp()=="and") || (unE!=NULL && unE->GetOp()=="not");
} // }}}
inline bool isN(const MpsExp *e) // {{{
{ const MpsBinOpExp *binE = dynamic_cast<const MpsBinOpExp*>(e);
  const MpsUnOpExp *unE = dynamic_cast<const MpsUnOpExp*>(e);
  return (unE!=NULL && unE->GetOp()=="not") || (binE!=NULL && binE->GetOp()=="or");
} // }}}
bool CFLKF(set<string> &theta1, set<string> &theta2, vector<const MpsExp*> &theta, vector<const MpsExp*> &gamma);
// Theta is split into:
// theta1: atoms
// theta2: negated atoms
// theta: positive expressions (conjunctions)
bool CFLKF_Focus(set<string> &theta1, set<string> &theta2, vector<const MpsExp*> &theta, const MpsExp *e) // {{{
{
  const MpsVarExp *varE = dynamic_cast<const MpsVarExp*>(e);
  const MpsBoolVal *valE = dynamic_cast<const MpsBoolVal*>(e);
  const MpsUnOpExp *unE = dynamic_cast<const MpsUnOpExp*>(e);
  const MpsBinOpExp *binE = dynamic_cast<const MpsBinOpExp*>(e);
  
  if (varE!=NULL && theta2.find(varE->ToString())!=theta2.end()) // Id Rule
    return true;
  if (isN(e)) // Release Rule
  { vector<const MpsExp*> gamma;
    gamma.push_back(e);
    bool result = CFLKF(theta1,theta2,theta,gamma);
    gamma.pop_back();
    if (result)
      return true;
  }
  if (valE!=NULL && valE->GetValue()) // True Rule
    return true;
  if (binE!=NULL && binE->GetOp()=="and" && // A+ Rule
      CFLKF_Focus(theta1,theta2,theta,&binE->GetLeft()) &&
      CFLKF_Focus(theta1,theta2,theta,&binE->GetRight()))
    return true;
  return false;
} // }}}
bool CFLKF(set<string> &theta1, set<string> &theta2, vector<const MpsExp*> &theta, vector<const MpsExp*> &gamma) // {{{
{ if (gamma.size()==0)
  { // Use Focus2 Rule
    for (int pos=0; pos<theta.size(); ++pos)
    { const MpsExp *p=theta[pos];
      theta.erase(theta.begin()+pos);
      bool result = isP(p) && CFLKF_Focus(theta1,theta2,theta,p);
      theta.insert(theta.begin()+pos,p);
      if (result)
        return true;
    }
    return false;
  }
  bool test=false;
  const MpsExp *e=gamma.back();
  gamma.pop_back();
  const MpsBoolVal *valE = dynamic_cast<const MpsBoolVal*>(e);
  if ((not test) && valE!=NULL)
  { if (valE->GetValue()) // Use Shortcut
      test=true;
    else if (CFLKF(theta1,theta2,theta,gamma)) // Optimized version of [] where false is not inserted in theta
      test=true;
  }
  { string var_name;
    bool negated;
    if ((not test) && isL(e,var_name,negated)) // Try Id* Rule
    { if ((negated && theta1.find(var_name)!=theta1.end()) ||
          (not negated && theta2.find(var_name)!=theta2.end()))
        test=true;
      if ((not test) && (not negated)) // Try [] Rule
      { bool insert = theta1.find(var_name)==theta1.end();
        if (insert)
          theta1.insert(var_name);
        if (CFLKF(theta1,theta2,theta,gamma))
          test=true;
        if (insert)
          theta1.erase(var_name);
      }
      if ((not test) && negated) // Try [] Rule
      { bool insert = theta2.find(var_name)==theta2.end();
        if (insert)
          theta2.insert(var_name);
        if (CFLKF(theta1,theta2,theta,gamma))
          test=true;
        if (insert)
          theta2.erase(var_name);
      }
    }
  }
  const MpsBinOpExp *binE = dynamic_cast<const MpsBinOpExp*>(e);
  if ((not test) && binE!=NULL && binE->GetOp()=="and")
  { // Try [] Rule
    theta.push_back(e);
    test=CFLKF(theta1,theta2,theta,gamma); // 1,2
    theta.pop_back();
  }
  if ((not test) && (binE!=NULL && binE->GetOp()=="or"))
  { // Try v- Rule
    // OPTIMIZE: Use original left and right without copy and delete
    MpsExp *left=binE->GetLeft().Copy();
    MpsExp *right=binE->GetRight().Copy();
    gamma.push_back(right);
    gamma.push_back(left);
    test=CFLKF(theta1,theta2,theta,gamma); // 0
    gamma.pop_back();
    gamma.pop_back();
    delete left;
    delete right;
  }
  gamma.push_back(e);
  if (test)
    return true;
  
  return false;
} // }}}
bool MpsExp::ValidExp_CFLKF(vector<const MpsExp*> hyps) const // {{{
{ vector<const MpsExp*> theta;
  vector<const MpsExp*> gamma;
  for (vector<const MpsExp*>::const_iterator it=hyps.begin(); it!=hyps.end(); ++it)
  { MpsExp *tmp = (*it)->Negate();
    gamma.push_back(tmp->MakeNNF());
    delete tmp;
  }
  gamma.push_back(MakeNNF());
  set<string> theta1;
  set<string> theta2;
  bool result=CFLKF(theta1,theta2,theta,gamma);
  DeleteVector(theta);
  DeleteVector(gamma);
  return result;
} // }}}

void CopyExpVector(vector<MpsExp*> &lhs, vector<MpsExp*> &rhs) // {{{
{ for (vector<MpsExp*>::const_iterator it=lhs.begin(); it!=lhs.end(); ++it)
    rhs.push_back((*it)->Copy());
} // }}}
bool LK_Axiom(vector<MpsExp*> &exps, set<string> pos, set<string> neg) // {{{
{ while (exps.size()>0)
  { MpsExp *exp = exps.back();
    MpsVarExp *varexp=dynamic_cast<MpsVarExp*>(exp);
    MpsBoolVal *valexp=dynamic_cast<MpsBoolVal*>(exp);
    MpsBinOpExp *binexp=dynamic_cast<MpsBinOpExp*>(exp);
    MpsUnOpExp *unexp=dynamic_cast<MpsUnOpExp*>(exp);
    if (varexp!=NULL) // Expression is a variable {{{
    { if (neg.find(varexp->ToString())!=neg.end()) // negation already found
        return true;
      else
      { exps.pop_back();
        pos.insert(varexp->ToString());
        delete exp;
      }
    } // }}}
    else if (valexp!=NULL) // Expression is a boolean value {{{
    { if (valexp->GetValue())
        return true;
      else
      { exps.pop_back();
        delete exp;
      }
    } // }}}
    else if (binexp!=NULL) // Expression is a binary operation {{{
    { if (binexp->GetOp()=="or") // Or expression
      { exps.pop_back();
        exps.push_back(binexp->GetRight().Copy());
        exps.push_back(binexp->GetLeft().Copy());
        delete exp;
      }
      else if (binexp->GetOp()=="and")
      { exps.pop_back();
        vector<MpsExp*> exps2;
        CopyExpVector(exps,exps2);
        exps2.push_back(binexp->GetLeft().Copy());
        exps.push_back(binexp->GetRight().Copy());
        delete exp;
        bool lhs=LK_Axiom(exps2,pos,neg);
        DeleteVector(exps2);
        if (not lhs)
          return false;
      }
      else
      { cerr << "LK_Axiom: Unknown Binary Operator " << binexp->GetOp() << endl;
        return false;
      }
    } // }}}
    else if (unexp!=NULL) // Expression is a unary operation {{{
    { if (unexp->GetOp()=="not") // Not expression
      {
        
        MpsVarExp *negvarexp=dynamic_cast<MpsVarExp*>(&unexp->GetRight());
        MpsBoolVal *negvalexp=dynamic_cast<MpsBoolVal*>(&unexp->GetRight());
        MpsBinOpExp *negbinexp=dynamic_cast<MpsBinOpExp*>(&unexp->GetRight());
        MpsUnOpExp *negunexp=dynamic_cast<MpsUnOpExp*>(&unexp->GetRight());
        if (negvarexp!=NULL) // SubExpression is a variable {{{
        { if (pos.find(negvarexp->ToString())!=pos.end()) // negation already found
            return true;
          else
          { exps.pop_back();
            neg.insert(negvarexp->ToString());
            delete exp;
          }
        } // }}}
        else if (negvalexp!=NULL) // SubExpression is a boolean value {{{
        { if (not negvalexp->GetValue())
            return true;
          else
          { exps.pop_back();
            delete exp;
          }
        } // }}}
        else if (negbinexp!=NULL) // SubExpression is a binary operation {{{
        { if (negbinexp->GetOp()=="and") // Not and expression
          { exps.pop_back();
            exps.push_back(new MpsUnOpExp("not",negbinexp->GetRight()));
            exps.push_back(new MpsUnOpExp("not",negbinexp->GetLeft()));
            delete exp;
          }
          else if (negbinexp->GetOp()=="or") // Not or expression
          { exps.pop_back();
            vector<MpsExp*> exps2;
            CopyExpVector(exps,exps2);
            exps2.push_back(new MpsUnOpExp("not",negbinexp->GetLeft()));
            exps.push_back(new MpsUnOpExp("not",negbinexp->GetRight()));
            delete exp;
            bool lhs=LK_Axiom(exps2,pos,neg);
            DeleteVector(exps2);
            if (not lhs)
              return false;
          }
          else
          { cerr << "LK_Axiom: Unknown Binary Operator " << binexp->GetOp() << endl;
            return false;
          }
        } // }}}
        else if (negunexp!=NULL) // SubExpression is a unary operation {{{
        { if (negunexp->GetOp()=="not") // Not not expresion
          { exps.pop_back();
            exps.push_back(negunexp->GetRight().Copy());
            delete exp;
          }
          else
          { cerr << "LK_Axiom: Unknown unary Operator " << negunexp->GetOp() << endl;
            return false;
          }
        } // }}}
        else // No Match {{{
        { cerr << "LK_Axiom: Unknown expression " << unexp->GetRight().ToString() << endl;
          return false;
        } // }}}
      }
      else
      { cerr << "LK_Axiom: Unknown unary Operator " << unexp->GetOp() << endl;
        return false;
      }
    } // }}}
    else
    { cerr << "LK_Axiom: Unknown expression " << exp->ToString() << endl;
      return false;
    }
  }
  return false;
} // }}}
bool MpsExp::ValidExp_LK(vector<const MpsExp*> hyps) const // {{{
{ vector<MpsExp*> exps;
  for (vector<const MpsExp*>::const_iterator it=hyps.begin(); it!=hyps.end(); ++it)
    exps.push_back(new MpsUnOpExp("not",*(*it)));
  exps.push_back(Copy());
  bool result=LK_Axiom(exps,set<string>(),set<string>());
  DeleteVector(exps);
  return result;
} // }}}

bool MpsExp::ValidExp_CNF(vector<const MpsExp*> hyps) const // {{{
{
  MpsExp *fullexp = Copy();
  for (int i=0; i<hyps.size(); ++i)
  { MpsExp *tmp=fullexp;
    MpsUnOpExp neg("not",*hyps[i]);
    fullexp=new MpsBinOpExp("or",*fullexp,neg,MpsMsgNoType(),MpsMsgNoType());
    delete tmp;
  }
  // fullexp = (...(e or not hyp1) or ... or not hypn)
  MpsExp *fullcnf=fullexp->MakeCNF();
  delete fullexp;
  bool result = fullcnf->ValidCNF();
  delete fullcnf;
  return result;
} // }}}

/* Negate boolean expression
 */

MpsExp *MpsVarExp::Negate() const// {{{
{ MpsExp *subexp=Copy();
  MpsExp *result = new MpsUnOpExp("not",*subexp);
  delete subexp;
  return result;
} // }}}
MpsExp *MpsIntVal::Negate() const// {{{
{ throw (string)"ERROR: Negate applied to Integer Value";
  return new MpsVarExp("ERROR: Negate applied to Integer Value");
} // }}}
MpsExp *MpsStringVal::Negate() const// {{{
{ throw (string)"ERROR: Negate applied to String Value";
  return new MpsVarExp("ERROR: Negate applied to String Value");
} // }}}
MpsExp *MpsBoolVal::Negate() const// {{{
{
  return new MpsBoolVal(not myValue);
} // }}}
MpsExp *MpsCondExp::Negate() const// {{{
{ throw (string)"ERROR: Negate applied to CondExp - NOT IMPLEMENTED";
  return new MpsVarExp("ERROR: Negate applied to CondExp - NOT IMPLEMENTED");
} // }}}
MpsExp *MpsUnOpExp::Negate() const// {{{
{ if (GetOp()=="not")
    return myRight->Copy();
  throw (string)"ERROR: Negation on unary operator: "+GetOp();
  return new MpsVarExp((string)"ERROR: Negation on unary operator: " + GetOp());
} // }}}
MpsExp *MpsBinOpExp::Negate() const// {{{
{ if (GetOp()=="and")
  { MpsExp *newLeft = myLeft->Negate();
    MpsExp *newRight = myRight->Negate();
    MpsExp *result = new MpsBinOpExp("or",*newLeft,*newRight,*myLeftType, *myRightType);
    delete newLeft;
    delete newRight;
    return result;
  }
  else if (GetOp()=="or")
  { MpsExp *newLeft = myLeft->Negate();
    MpsExp *newRight = myRight->Negate();
    MpsExp *result = new MpsBinOpExp("and",*newLeft,*newRight,*myLeftType,*myRightType);
    delete newLeft;
    delete newRight;
    return result;
  }
  else if (GetOp()=="<=")
  { mpz_t one;
    mpz_init_set_str(one,"1",10);
    MpsExp *newRight = new MpsBinOpExp("+",*myRight,MpsIntVal(one),*myRightType,MpsIntMsgType());
    mpz_clear(one);
    MpsExp *result = new MpsBinOpExp("and",*myLeft,*newRight,*myLeftType,*myRightType);
    delete newRight;
    return result;
  }
  else if (GetOp()=="=")
  { MpsExp *result = new MpsBinOpExp("!=",*myLeft,*myRight,*myLeftType,*myRightType);
    return result;
  }
  else
  {
    throw (string)"ERROR: Negate applied to binary operator: "+GetOp();
    return new MpsVarExp((string)"ERROR: Negate applied to binary operator: " + GetOp());
  }
} // }}}
MpsExp *MpsTupleExp::Negate() const// {{{
{ throw (string)"ERROR: Negate applied to tuple";
  return new MpsVarExp("ERROR: Negate applied to tuple");
} // }}}

/* Convert boolean expressions to CNF
 */

MpsExp *MpsVarExp::MakeCNF() const// {{{
{ return Copy();
} // }}}
MpsExp *MpsIntVal::MakeCNF() const// {{{
{ throw (string)"ERROR: MakeCNF applied to Integer Value";
  return new MpsVarExp("ERROR: MakeCNF applied to Integer Value");
} // }}}
MpsExp *MpsStringVal::MakeCNF() const// {{{
{ throw (string)"ERROR: MakeCNF applied to String Value";
  return new MpsVarExp("ERROR: MakeCNF applied to String Value");
} // }}}
MpsExp *MpsBoolVal::MakeCNF() const// {{{
{ return Copy();
} // }}}
MpsExp *MpsCondExp::MakeCNF() const// {{{
{ throw (string)"ERROR: MakeCNF applied to CondExp - NOT IMPLEMENTED";
  return new MpsVarExp("ERROR: MakeCNF applied to CondExp - NOT IMPLEMENTED");
} // }}}
MpsExp *MpsUnOpExp::MakeCNF() const// {{{
{ if (GetOp()=="not")
  { if (dynamic_cast<MpsVarExp*>(myRight)!=NULL)
      return Copy();
    else
    { MpsExp *tmp=myRight->Negate();
      MpsExp *result=tmp->MakeCNF();
      delete tmp;
      return result;
    }
  }
  throw (string)"ERROR: Negation on unary operator: "+GetOp();
  return new MpsVarExp((string)"ERROR: Negation on unary operator: " + GetOp());
} // }}}
MpsExp *MpsBinOpExp::MakeCNF() const// {{{
{ if (GetOp()=="and")
  { MpsExp *newLeft = myLeft->MakeCNF();
    MpsExp *newRight = myRight->MakeCNF();
    MpsExp *result = new MpsBinOpExp("and",*newLeft,*newRight,*myLeftType,*myRightType);
    delete newLeft;
    delete newRight;
    return result;
  }
  else if (GetOp()=="or")
  { MpsExp *newLeft = myLeft->MakeCNF();
    MpsExp *newRight = myRight->MakeCNF();
    MpsBinOpExp *opLeft = dynamic_cast<MpsBinOpExp*>(newLeft);
    MpsBinOpExp *opRight = dynamic_cast<MpsBinOpExp*>(newRight);
    if (opLeft!=NULL)
    { if (opLeft->GetOp()=="and") // (e1 and e2) or e3
      { MpsExp *tmpLeft=new MpsBinOpExp("or",opLeft->GetLeft(),*newRight,*myLeftType,*myRightType); // e1 or e3
        MpsExp *cnfLeft=tmpLeft->MakeCNF();
        delete tmpLeft;
        MpsExp *tmpRight=new MpsBinOpExp("or",opLeft->GetRight(),*newRight,*myLeftType,*myRightType); // e2 or e3
        MpsExp *cnfRight=tmpRight->MakeCNF();
        delete tmpRight;
        delete newLeft;
        delete newRight;
        MpsExp *result=new MpsBinOpExp("and",*cnfLeft, *cnfRight,*myLeftType,*myRightType);
        delete cnfLeft;
        delete cnfRight;
        return result;
      }
      else if (opLeft->GetOp()!="or")
      { string msg=(string)"ERROR: Make CNF found unexpected binary operator: "+opLeft->GetOp();
        delete newLeft;
        delete newRight;
        throw msg;
        return new MpsVarExp(msg);
      }
    }
    if (opRight!=NULL)
    { if (opRight->GetOp()=="and") // e1 or (e2 and e3)
      { MpsExp *tmpLeft=new MpsBinOpExp("or",*newLeft,opRight->GetLeft(),*myLeftType,*myRightType); // e1 or e2
        MpsExp *cnfLeft=tmpLeft->MakeCNF();
        delete tmpLeft;
        MpsExp *tmpRight=new MpsBinOpExp("or",*newLeft,opRight->GetRight(),*myLeftType,*myRightType); // e1 or e3
        MpsExp *cnfRight=tmpRight->MakeCNF();
        delete tmpRight;
        delete newLeft;
        delete newRight;
        MpsExp *result=new MpsBinOpExp("and",*cnfLeft, *cnfRight, *myLeftType,*myRightType);
        delete cnfLeft;
        delete cnfRight;
        return result;
      }
      else if (opRight->GetOp()!="or")
      { string msg=(string)"ERROR: Make CNF found unexpected binary operator: " + opRight->GetOp();
        delete newLeft;
        delete newRight;
        throw msg;
        return new MpsVarExp(msg);
      }
    }
    // Otherwise just combine left and right side
    MpsExp *result = new MpsBinOpExp("or",*newLeft,*newRight,*myLeftType,*myRightType);
    delete newLeft;
    delete newRight;
    return result;
  }
  else
  { throw (string)"ERROR: MakeCNF applied to binary operator: "+GetOp();
    return new MpsVarExp((string)"ERROR: MakeCNF applied to binary operator: " + GetOp());
  }
} // }}}
MpsExp *MpsTupleExp::MakeCNF() const// {{{
{ throw (string)"ERROR: MakeCNF applied to tuple";
  return new MpsVarExp("ERROR: MakeCNF applied to tuple");
} // }}}

/* Convert boolean expressions to Negation Normal Form
 */

MpsExp *MpsVarExp::MakeNNF(bool negate) const// {{{
{ if (negate)
    return new MpsUnOpExp("not",*this);
  else
    return Copy();
} // }}}
MpsExp *MpsIntVal::MakeNNF(bool negate) const// {{{
{ throw (string)"ERROR: MakeNNF applied to Integer Value";
  return new MpsBoolVal(false);
} // }}}
MpsExp *MpsStringVal::MakeNNF(bool negate) const// {{{
{ throw (string)"ERROR: MakeNNF applied to String Value";
  return new MpsBoolVal(false);
} // }}}
MpsExp *MpsBoolVal::MakeNNF(bool negate) const// {{{
{ if (negate)
    return new MpsBoolVal(not GetValue());
  else
    return Copy();
} // }}}
MpsExp *MpsCondExp::MakeNNF(bool negate) const// {{{
{ cerr << "WARNING: MakeNNF applied to CondExp - NOT IMPLEMENTED - approximating as false!" << endl;
  return new MpsBoolVal(false);
} // }}}
MpsExp *MpsUnOpExp::MakeNNF(bool negate) const// {{{
{
  if (GetOp()=="not")
    return myRight->MakeNNF(not negate);
  throw (string)"ERROR: MakeNNF on unknown unary operator: "+GetOp();
  return new MpsBoolVal(false);
} // }}}
MpsExp *MpsBinOpExp::MakeNNF(bool negate) const// {{{
{ if (GetOp()=="and")
  { MpsExp *newLeft = myLeft->MakeNNF(negate);
    MpsExp *newRight = myRight->MakeNNF(negate);
    MpsExp *result = new MpsBinOpExp(negate?"or":"and",*newLeft,*newRight,*myLeftType,*myRightType);
    delete newLeft;
    delete newRight;
    return result;
  }
  else if (GetOp()=="or")
  { MpsExp *newLeft = myLeft->MakeNNF(negate);
    MpsExp *newRight = myRight->MakeNNF(negate);
    MpsExp *result = new MpsBinOpExp(negate?"and":"or",*newLeft,*newRight,*myLeftType,*myRightType);
    delete newLeft;
    delete newRight;
    return result;
  }
  else
  { cerr << "WARNING: MakeNNF applied to binary operator: "+GetOp() << " - approximating as false!" << endl;
    return new MpsBoolVal(false);
  }
} // }}}
MpsExp *MpsTupleExp::MakeNNF(bool negate) const// {{{
{ throw (string)"ERROR: MakeNNF applied to tuple";
  return new MpsBoolVal(false);
} // }}}

/* Check if CNF expression is valid
 */

bool MpsVarExp::ValidCNF() const// {{{
{ return false;
} // }}}
bool MpsIntVal::ValidCNF() const// {{{
{ throw (string)"ERROR: ValidCNF applied to Integer Value";
  return false;
} // }}}
bool MpsStringVal::ValidCNF() const// {{{
{ throw (string)"ERROR: ValidCNF applied to String Value";
  return false;
} // }}}
bool MpsBoolVal::ValidCNF() const// {{{
{ return myValue;
} // }}}
bool MpsCondExp::ValidCNF() const// {{{
{ throw (string)"ERROR: ValidCNF applied to CondExp";
  return false;
} // }}}
bool MpsUnOpExp::ValidCNF() const// {{{
{ if (GetOp()=="not")
    return false;
  else
  { throw (string)"ERROR: ValidCNF applied to unary operator: "+GetOp();
    return false;
  }
} // }}}
bool MpsBinOpExp::ValidCNF() const// {{{
{ if (GetOp()=="and")
    return myLeft->ValidCNF() && myRight->ValidCNF();
  else if (GetOp()=="or")
  { set<string> lits;
    set<string> neglits;
    return ValidOR(lits,neglits);
  }
  throw (string)"ERROR: ValidCNF applied to binary operator: " + GetOp();
  return false;
} // }}}
bool MpsTupleExp::ValidCNF() const// {{{
{ throw (string)"ERROR: ValidCNF applied to tuple";
  return false;
} // }}}

/* Check if OR expression is valid
 */

bool MpsVarExp::ValidOR(set<string> &pos, set<string> &neg) const// {{{
{ pos.insert(myName);
  return neg.count(myName)>0;
} // }}}
bool MpsIntVal::ValidOR(set<string> &pos, set<string> &neg) const// {{{
{ throw (string)"ERROR: ValidOR applied to Integer Value";
  return false;
} // }}}
bool MpsStringVal::ValidOR(set<string> &pos, set<string> &neg) const// {{{
{ throw (string)"ERROR: ValidOR applied to String Value";
  return false;
} // }}}
bool MpsBoolVal::ValidOR(set<string> &pos, set<string> &neg) const// {{{
{ return myValue;
} // }}}
bool MpsCondExp::ValidOR(set<string> &pos, set<string> &neg) const// {{{
{ throw (string)"ERROR: ValidOR applied to CondExp";
  return false;
} // }}}
bool MpsUnOpExp::ValidOR(set<string> &pos, set<string> &neg) const// {{{
{ if (GetOp()=="not")
  { MpsVarExp *lit=dynamic_cast<MpsVarExp*>(myRight);
    if (lit!=NULL)
    { neg.insert(lit->ToString());
      return pos.count(lit->ToString())>0;
    }
  }
  throw (string)"ERROR: ValidOR applied to unary operator which is not of type (not x)";
  return false;
} // }}}
bool MpsBinOpExp::ValidOR(set<string> &pos, set<string> &neg) const// {{{
{ if (GetOp()=="or")
    return myLeft->ValidOR(pos,neg) || myRight->ValidOR(pos,neg);
  throw (string)"ERROR: ValidOR applied to binary operator: " + GetOp();
  return false;
} // }}}
bool MpsTupleExp::ValidOR(set<string> &pos, set<string> &neg) const// {{{
{ throw (string)"ERROR: ValidOR applied to tuple";
  return false;
} // }}}


/* Subclass specific methods
 */
const mpz_t &MpsIntVal::GetValue() const // {{{
{
  return myValue;
} // }}}
string MpsStringVal::GetValue() const // {{{
{
  return myValue;
} // }}}
bool MpsBoolVal::GetValue() const // {{{
{
  return myValue;
} // }}}
int MpsTupleExp::GetSize() const // {{{
{
  return myElements.size();
} // }}}
const MpsExp *MpsTupleExp::GetElement(int index) const // {{{
{
  if (index < 0 || index >= myElements.size()) // Index out of bounds
    return new MpsVarExp("_Error: Tuple index out of bounds");
  return myElements[index];
} // }}}
string MpsBinOpExp::GetOp() const // {{{
{ return myName;
} // }}}
const MpsExp &MpsBinOpExp::GetLeft() const // {{{
{ return *myLeft;
} // }}}
const MpsExp &MpsBinOpExp::GetRight() const // {{{
{ return *myRight;
} // }}}
string MpsUnOpExp::GetOp() const // {{{
{ return myName;
} // }}}
const MpsExp &MpsUnOpExp::GetRight() const // {{{
{ return *myRight;
} // }}}
MpsExp &MpsUnOpExp::GetRight() // {{{
{ return *myRight;
} // }}}
