#include <apims/mpsterm.hpp>
#include <dpl/parser.hpp>
#include <iostream>
#include <sys/timeb.h>

using namespace std;
using namespace apims;
using namespace dpl;

MpsExp *Create(parsed_tree *tree) // {{{
{
  if (tree->type_name=="exp" && tree->case_name=="case1") // exp <=> exp2 {{{
  { MpsExp *exp1=Create(tree->content[0]);
    MpsExp *exp2=Create(tree->content[2]);
    MpsExp *notexp1=new MpsUnOpExp("not",*exp1);
    MpsExp *notexp2=new MpsUnOpExp("not",*exp2);
    MpsExp *imp1=new MpsBinOpExp("or",*notexp1,*exp2); // not exp1 or exp2
    MpsExp *imp2=new MpsBinOpExp("or",*notexp2,*exp1); // not exp2 or exp1
    MpsExp *result=new MpsBinOpExp("and",*imp1,*imp2);
    delete exp1;
    delete exp2;
    delete notexp1;
    delete notexp2;
    return result;
  } // }}}
  else if (tree->type_name=="exp" && tree->case_name=="case2") // exp2 {{{
  { return Create(tree->content[0]);
  } // }}}
  else if (tree->type_name=="exp2" && tree->case_name=="case1") // exp => exp2 {{{
  { MpsExp *exp1=Create(tree->content[0]);
    MpsExp *exp2=Create(tree->content[2]);
    MpsExp *notexp1=new MpsUnOpExp("not",*exp1);
    delete exp1;
    MpsExp *result=new MpsBinOpExp("or",*notexp1,*exp2); // not exp1 or exp2
    delete exp2;
    delete notexp1;
    return result;
  } // }}}
  else if (tree->type_name=="exp2" && tree->case_name=="case2") // exp3 {{{
  { return Create(tree->content[0]);
  } // }}}
  else if (tree->type_name=="exp3" && tree->case_name=="case1") // exp <= exp2 {{{
  { MpsExp *exp1=Create(tree->content[0]);
    MpsExp *exp2=Create(tree->content[2]);
    MpsExp *notexp2=new MpsUnOpExp("not",*exp2);
    delete exp2;
    MpsExp *result=new MpsBinOpExp("or",*notexp2,*exp1); // not exp2 or exp1
    delete exp1;
    delete notexp2;
    return result;
  } // }}}
  else if (tree->type_name=="exp3" && tree->case_name=="case2") // exp4 {{{
  { return Create(tree->content[0]);
  } // }}}
  else if (tree->type_name=="exp4" && tree->case_name=="case1") // exp AND exp2 {{{
  { MpsExp *exp1=Create(tree->content[0]);
    MpsExp *exp2=Create(tree->content[2]);
    MpsExp *result=new MpsBinOpExp("and",*exp1,*exp2); // exp1 and exp2
    delete exp1;
    delete exp2;
    return result;
  } // }}}
  else if (tree->type_name=="exp4" && tree->case_name=="case2") // exp5 {{{
  { return Create(tree->content[0]);
  } // }}}
  else if (tree->type_name=="exp5" && tree->case_name=="case1") // exp OR exp2 {{{
  { MpsExp *exp1=Create(tree->content[0]);
    MpsExp *exp2=Create(tree->content[2]);
    MpsExp *result=new MpsBinOpExp("or",*exp1,*exp2); // exp1 and exp2
    delete exp1;
    delete exp2;
    return result;
  } // }}}
  else if (tree->type_name=="exp5" && tree->case_name=="case2") // exp6 {{{
  { return Create(tree->content[0]);
  } // }}}
  else if (tree->type_name=="exp6" && tree->case_name=="case1") // NOT exp1 {{{
  { MpsExp *exp1=Create(tree->content[1]);
    MpsExp *result=new MpsUnOpExp("not",*exp1); // not exp1
    delete exp1;
    return result;
  } // }}}
  else if (tree->type_name=="exp6" && tree->case_name=="case2") // exp7 {{{
  { return Create(tree->content[0]);
  } // }}}
  else if (tree->type_name=="exp7" && tree->case_name=="case1") // T {{{
  { return new MpsBoolVal(true);
  } // }}}
  else if (tree->type_name=="exp7" && tree->case_name=="case2") // F {{{
  { return new MpsBoolVal(false);
  } // }}}
  else if (tree->type_name=="exp7" && tree->case_name=="case3") // id {{{
  { return new MpsVarExp(tree->content[0]->root.content);
  } // }}}
  else if (tree->type_name=="exp7" && tree->case_name=="case4") // ( exp1 ) {{{
  { return Create(tree->content[1]);
  } // }}}
  // else
  cerr << "Unknown parsetree: " << tree->type_name << "." << tree->case_name << endl;
  return new MpsBoolVal(true);
} // }}}

double gettime() // {{{
{ struct timeb tb;
  ftime(&tb);
  double result = tb.millitm;
  result /= 1000;
  result+= (tb.time % (60*60));
  return result;
} // }}}

int main(int argc, char** argv)
{
  if (argc!=2)
  { cerr << "Syntax: " << argv[0] << " EXP" << endl;
    return 1;
  }
      
  Parser p;
  p.DefToken("","[ \t\r\n][ \t\r\n]*",10);
  p.DefToken("id","[a-z][a-z0-9]*",10);
  p.DefKeywordToken("=>",2);
  p.DefKeywordToken("<=",2);
  p.DefKeywordToken("<=>",1);
  p.DefToken("AND","AND+and+&",1);
  p.DefToken("OR","OR+or+|",1);
  p.DefToken("NOT","NOT+not+~",1);
  p.DefToken("T","T+true",1);
  p.DefToken("F","F+false",1);
  p.DefKeywordToken("(",1);
  p.DefKeywordToken(")",1);
  p.DefType("exp ::= exp <=> exp2 | exp2");
  p.DefType("exp2 ::= exp2 => exp3 | exp3");
  p.DefType("exp3 ::= exp3 <= exp4 | exp4");
  p.DefType("exp4 ::= exp4 AND exp5 | exp5");
  p.DefType("exp5 ::= exp5 OR exp6 | exp6");
  p.DefType("exp6 ::= NOT exp6 | exp7");
  p.DefType("exp7 ::= T | F | id | ( exp )");
  parsed_tree *tree=p.Parse(argv[1]);
  MpsExp *e = Create(tree);
  vector<const MpsExp*> hyps;
//  double lk_start=gettime();
//  cout << "LK: " << e->ValidExp_LK(hyps) << endl;
//  double lk_end=gettime();
//  cout << "LK-TIME: " << lk_end-lk_start << endl;
  double cflkf_start=gettime();
  cout << "CFLKF: " << e->ValidExp_CFLKF(hyps) << endl;
  double cflkf_end=gettime();
  cout << "CFLKF-TIME: " << cflkf_end-cflkf_start << endl;
//  cout << e->ToString() << endl;
//  MpsExp *cnf_e = e->MakeCNF();
  delete e;
//  cout << cnf_e->ToString() << endl;
//  cout << "CNF: " << cnf_e->ValidCNF() << endl;
//  delete cnf_e;
  return 0;
}
