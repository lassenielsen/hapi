/* Pre compilation output:
******************* Program *******************
(nu fib:1=>2<Int>;
        2=>1<Int>;
        Gend)
  ( def Fib()=
       link(2,fib,s,2);
       ( Fib()
       | s[1] >> x;
         if (x <= 1)
         then s[1] << 1;
              end
         else link(2,fib,s1,1);
              s1[2] << (x - 1);
              link(2,fib,s2,1);
              s2[2] << (x - 2);
              s1[2] >> f1;
              s2[2] >> f2;
              s[1] << (f1 + f2);
              end
       )
    in Fib()
  | link(2,fib,f,1);
    f[2] << 10;
    f[2] >> f10;
    end
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
int __SIM__Fib1(vector<Channel_MQ> __SIM__fib1);

/* Procedure implementations */
int __SIM__Fib1(vector<Channel_MQ> __SIM__fib1)
{
  Session_MQ __SIM__s2(__SIM__fib1, 1, 2);
  int __SIM__fork16=fork();
  if (__SIM__fork16>0)
  {
  {
  }
  __SIM__Fib1(__SIM__fib1);
return 0;
  }
  else if (__SIM__fork16==0)
  {
  IntValue __SIM__x3;
  { Message __SIM__receive17;
    __SIM__s2.Receive(0,__SIM__receive17);
    __SIM__receive17.GetValue(__SIM__x3);
  }
    IntValue __SIM__intval19("1");
    BoolValue __SIM__binop18(__SIM__x3 <= __SIM__intval19);
  if (__SIM__binop18.GetValue())
  {
  { 
    IntValue __SIM__intval31("1");
    Message  __SIM__send30;
    __SIM__intval31.ToMessage(__SIM__send30);
    __SIM__s2.Send(0,__SIM__send30);
  }
  return 0;
  }
  else
  {
  Session_MQ __SIM__s14(__SIM__fib1, 0, 2);
  { 
    IntValue __SIM__intval22("1");
    IntValue __SIM__binop21(__SIM__x3 - __SIM__intval22);
    Message  __SIM__send20;
    __SIM__binop21.ToMessage(__SIM__send20);
    __SIM__s14.Send(1,__SIM__send20);
  }
  Session_MQ __SIM__s25(__SIM__fib1, 0, 2);
  { 
    IntValue __SIM__intval25("2");
    IntValue __SIM__binop24(__SIM__x3 - __SIM__intval25);
    Message  __SIM__send23;
    __SIM__binop24.ToMessage(__SIM__send23);
    __SIM__s25.Send(1,__SIM__send23);
  }
  IntValue __SIM__f16;
  { Message __SIM__receive26;
    __SIM__s14.Receive(1,__SIM__receive26);
    __SIM__receive26.GetValue(__SIM__f16);
  }
  IntValue __SIM__f27;
  { Message __SIM__receive27;
    __SIM__s25.Receive(1,__SIM__receive27);
    __SIM__receive27.GetValue(__SIM__f27);
  }
  { 
    IntValue __SIM__binop29(__SIM__f16 + __SIM__f27);
    Message  __SIM__send28;
    __SIM__binop29.ToMessage(__SIM__send28);
    __SIM__s2.Send(0,__SIM__send28);
  }
  return 0;
  }
  }
else throw (string)"Error during fork!";
return 0;
}

/* Main process */
int main()
{
  vector<Channel_MQ> __SIM__fib1;
  __SIM__fib1.push_back(Channel_MQ());
  int __SIM__fork12=fork();
  if (__SIM__fork12>0)
  {
  {
  }
  __SIM__Fib1(__SIM__fib1);
return 0;
  }
  else if (__SIM__fork12==0)
  {
  Session_MQ __SIM__f8(__SIM__fib1, 0, 2);
  { 
    IntValue __SIM__intval14("10");
    Message  __SIM__send13;
    __SIM__intval14.ToMessage(__SIM__send13);
    __SIM__f8.Send(1,__SIM__send13);
  }
  IntValue __SIM__f109;
  { Message __SIM__receive15;
    __SIM__f8.Receive(1,__SIM__receive15);
    __SIM__receive15.GetValue(__SIM__f109);
  }
  return 0;
  }
else throw (string)"Error during fork!";
return 0;
}
