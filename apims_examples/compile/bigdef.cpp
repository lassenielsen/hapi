/* Pre compilation output:
******************* Program *******************
def X(i: Int)=
   if (i <= 0)
   then end
   else X((i - 1))
in ( X(10000000)
   | X(10000000)
   )
************ Type Checking Program ************
************ Type Check Succeeded! ************
*/
#include <unistd.h>
#include <vector>
#include <iostream>
#include <libpi/session_fifo.hpp>
#include <libpi/value.hpp>
using namespace std;
using namespace libpi;


/* Procedure declerations */
class Cnt
{
  public:
    Cnt() {}
    virtual ~Cnt() {}
    virtual bool IsEmpty() { return true; }
    virtual Cnt *Run() { return new Cnt(); }
};
Cnt *__SIM__X1(IntValue &__SIM__i1);
class __Cnt____SIM__X1 : public Cnt
{
  public:
    __Cnt____SIM__X1(IntValue &__SIM__i1): __SIM__i1(__SIM__i1)
{}
    virtual ~__Cnt____SIM__X1() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__X1(__SIM__i1); }
  private:
IntValue __SIM__i1;
};
;

/* Procedure implementations */
Cnt *__SIM__X1(IntValue &__SIM__i1)
{
    IntValue __SIM__intval9("0");
    BoolValue __SIM__binop8(__SIM__i1 <= __SIM__intval9);
  if (__SIM__binop8.GetValue())
  {
  return new Cnt();
  }
  else
  {
    IntValue __SIM__intval11("1");
    IntValue __SIM__binop10(__SIM__i1 - __SIM__intval11);
  return new __Cnt____SIM__X1(__SIM__binop10);
  }
}

/* Main process */
Cnt *__MAIN__()
{
  int __SIM__fork5=fork();
  if (__SIM__fork5>0)
  {
    IntValue __SIM__intval6("10000000");
  return new __Cnt____SIM__X1(__SIM__intval6);
  }
  else if (__SIM__fork5==0)
  {
    IntValue __SIM__intval7("10000000");
  return new __Cnt____SIM__X1(__SIM__intval7);
  }
else throw (string)"Error during fork!";
return 0;
}

/*Start process, and its continuations */
int main()
{ try {
    Cnt *cnt = __MAIN__();
    while (!cnt->IsEmpty())
    { Cnt *cnt2=cnt->Run();
      delete cnt;
      cnt=cnt2;
    }
    delete cnt;
  } catch (const string &error) {
    cerr << "Error: " << error << endl;
    return 1;
  }
  return 0;
}
