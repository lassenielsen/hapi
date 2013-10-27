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
              s1[2] >> f1;
              s2[2] << (x - 2);
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
#include <vector>
#include <iostream>
#include <libpi/session_fifo.hpp>
#include <libpi/value.hpp>
using namespace std;
using namespace libpi;


/* Procedure declerations */
int __SIM__Fib1(ChannelsValue &__SIM__fib1);

/* Procedure implementations */
int __SIM__Fib1(ChannelsValue &__SIM__fib1)
{
  Session*__SIM__s2=new Session_FIFO(__SIM__fib1.GetValues(), 1, 2);
  int __SIM__fork17=fork();
  if (__SIM__fork17>0)
  {
  return __SIM__Fib1(__SIM__fib1);
  }
  else if (__SIM__fork17==0)
  {
  IntValue __SIM__x3;
  { Message __SIM__receive18;
    __SIM__s2->Receive(0,__SIM__receive18);
    __SIM__receive18.GetValue(__SIM__x3);
  }
    IntValue __SIM__intval20("1");
    BoolValue __SIM__binop19(__SIM__x3 <= __SIM__intval20);
  if (__SIM__binop19.GetValue())
  {
  { 
    IntValue __SIM__intval32("1");
    Message  __SIM__send31;
    __SIM__intval32.ToMessage(__SIM__send31);
    __SIM__s2->Send(0,__SIM__send31);
  }
  return 0;
  }
  else
  {
  Session*__SIM__s14=new Session_FIFO(__SIM__fib1.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval23("1");
    IntValue __SIM__binop22(__SIM__x3 - __SIM__intval23);
    Message  __SIM__send21;
    __SIM__binop22.ToMessage(__SIM__send21);
    __SIM__s14->Send(1,__SIM__send21);
  }
  Session*__SIM__s25=new Session_FIFO(__SIM__fib1.GetValues(), 0, 2);
  IntValue __SIM__f16;
  { Message __SIM__receive24;
    __SIM__s14->Receive(1,__SIM__receive24);
    __SIM__receive24.GetValue(__SIM__f16);
  }
  { 
    IntValue __SIM__intval27("2");
    IntValue __SIM__binop26(__SIM__x3 - __SIM__intval27);
    Message  __SIM__send25;
    __SIM__binop26.ToMessage(__SIM__send25);
    __SIM__s25->Send(1,__SIM__send25);
  }
  IntValue __SIM__f27;
  { Message __SIM__receive28;
    __SIM__s25->Receive(1,__SIM__receive28);
    __SIM__receive28.GetValue(__SIM__f27);
  }
  { 
    IntValue __SIM__binop30(__SIM__f16 + __SIM__f27);
    Message  __SIM__send29;
    __SIM__binop30.ToMessage(__SIM__send29);
    __SIM__s2->Send(0,__SIM__send29);
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
  try {
  vector<Channel*> __SIM__chvector12;
  __SIM__chvector12.push_back(new Channel_FIFO());
  ChannelsValue __SIM__fib1(__SIM__chvector12);
  int __SIM__fork13=fork();
  if (__SIM__fork13>0)
  {
  return __SIM__Fib1(__SIM__fib1);
  }
  else if (__SIM__fork13==0)
  {
  Session*__SIM__f8=new Session_FIFO(__SIM__fib1.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval15("10");
    Message  __SIM__send14;
    __SIM__intval15.ToMessage(__SIM__send14);
    __SIM__f8->Send(1,__SIM__send14);
  }
  IntValue __SIM__f109;
  { Message __SIM__receive16;
    __SIM__f8->Receive(1,__SIM__receive16);
    __SIM__receive16.GetValue(__SIM__f109);
  }
  return 0;
  }
else throw (string)"Error during fork!";
return 0;
  } catch (const string &error) {
    cerr << "Error: " << error << endl;
  }
}
