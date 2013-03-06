/* Pre compilation output:
******************* Program *******************
def X(i: Int)=
   if (i <= 0)
   then end
   else X((i - 1))
in ( X(100)
   | X(100)
   )
************ Type Checking Program ************
************ Type Check Succeeded! ************
*/
#include <unistd.h>
#include <gmp.h>
#include <vector>
#include <libpi/session_mq.hpp>
#include <libpi/value.hpp>
using namespace std;
using namespace libpi;


/* Procedure declerations */
int __SIM__X1(IntValue __SIM__i1);

/* Procedure implementations */
int __SIM__X1(IntValue __SIM__i1)
{
    IntValue __SIM__intval9("0");
    BoolValue __SIM__binop8(__SIM__i1 <= __SIM__intval9);
  if (__SIM__binop8.GetValue())
  {
  return 0;
  }
  else
  {
  {
    IntValue __SIM__intval11("1");
    IntValue __SIM__binop10(__SIM__i1 - __SIM__intval11);
  }
  __SIM__X1(__SIM__binop10);
return 0;
  }
}

/* Main process */
int main()
{
  int __SIM__fork5=fork();
  if (__SIM__fork5>0)
  {
  {
    IntValue __SIM__intval7("100");
  }
  __SIM__X1(__SIM__intval7);
return 0;
  }
  else if (__SIM__fork5==0)
  {
  {
    IntValue __SIM__intval6("100");
  }
  __SIM__X1(__SIM__intval6);
return 0;
  }
else throw (string)"Error during fork!";
return 0;
}
