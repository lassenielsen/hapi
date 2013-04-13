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
#include <libpi/session_mq.hpp>
#include <libpi/value.hpp>
using namespace std;
using namespace libpi;


/* Procedure declerations */
int __SIM__X1(IntValue __SIM__i1);

/* Procedure implementations */
int __SIM__X1(IntValue __SIM__i1)
{
  cout << "call: " << __SIM__i1.ToString() << endl;
    IntValue __SIM__intval9("0");
    BoolValue __SIM__binop8(__SIM__i1 <= __SIM__intval9);
  if (__SIM__binop8.GetValue())
  {
  return 0;
  }
  else
  {
    IntValue __SIM__intval11("1");
    IntValue __SIM__binop10(__SIM__i1 - __SIM__intval11);
  return __SIM__X1(__SIM__binop10);
  }
}

/* Main process */
int main()
{
  try {
  int __SIM__fork5=fork();
  if (__SIM__fork5>0)
  {
    IntValue __SIM__intval7("10000000");
  return __SIM__X1(__SIM__intval7);
  }
  else if (__SIM__fork5==0)
  {
    IntValue __SIM__intval6("10000000");
  return __SIM__X1(__SIM__intval6);
  }
else throw (string)"Error during fork!";
return 0;
  } catch (const string &error) {
    cerr << "Error: " << error << endl;
  }
}
