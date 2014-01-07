/* Pre compilation output:
******************* Program *******************
(nu a:rec $server.
      1=>2
      { ^add[[true]]:
          1=>2<Int>;
          $server,
        ^done[[true]]:
          2=>1<Int>;
          Gend,
        ^mult[[true]]:
          1=>2<Int>;
          $server,
        ^neg[[true]]:
          $server
      })
  ( def Fork(id: Int)=
       link(2,a,s,2);
       ( Fork((id + 1))
       | def Server(s: rec %server.
                       1>>
                       { ^add[[true]]:
                         1 >> <Int>
                         %server,
                         ^done[[true]]:
                         1 << <Int>;
                         Lend,
                         ^mult[[true]]:
                         1 >> <Int>
                         %server,
                         ^neg[[true]]:
                         %server
                       }@(2 of 2),
                    v: Int)=
            s[1] >>
            { ^add:
              s[1] >> x;
              Server(s, (v + x)),
              ^done:
              s[1] << v;
              end,
              ^mult:
              s[1] >> x;
              Server(s, (v * x)),
              ^neg:
              Server(s, (0 - v))
            }
         in Server(s, 0)
       )
    in Fork(1)
  | link(2,a,s,1);
    s[2] << ^add;
    s[2] << 5;
    s[2] << ^mult;
    s[2] << 3;
    s[2] << ^mult;
    s[2] << 2;
    s[2] << ^neg;
    s[2] << ^done;
    s[2] >> x;
    end
  )
************ Type Checking Program ************
************ Type Check Succeeded! ************
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 1
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
Cnt *__SIM__Server2(Session* &__SIM__s4,IntValue &__SIM__v5);
class __Cnt____SIM__Server2 : public Cnt
{
  public:
    __Cnt____SIM__Server2(Session* &__SIM__s4,IntValue &__SIM__v5): __SIM__s4(__SIM__s4)
, __SIM__v5(__SIM__v5)
{}
    virtual ~__Cnt____SIM__Server2() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__Server2(__SIM__s4, __SIM__v5); }
  private:
Session* __SIM__s4;
IntValue __SIM__v5;
};
;
Cnt *__SIM__Fork1(IntValue &__SIM__id2,ChannelsValue &__SIM__a1);
class __Cnt____SIM__Fork1 : public Cnt
{
  public:
    __Cnt____SIM__Fork1(IntValue &__SIM__id2,ChannelsValue &__SIM__a1): __SIM__id2(__SIM__id2)
, __SIM__a1(__SIM__a1)
{}
    virtual ~__Cnt____SIM__Fork1() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__Fork1(__SIM__id2, __SIM__a1); }
  private:
IntValue __SIM__id2;
ChannelsValue __SIM__a1;
};
;

/* Procedure implementations */
Cnt *__SIM__Server2(Session* &__SIM__s4,IntValue &__SIM__v5)
{
  StringValue __SIM__branch41;
  {
    Message __SIM__branch42;
    __SIM__s4->Receive(0, __SIM__branch42);
    __SIM__branch42.GetValue(__SIM__branch41);
  }
  if (__SIM__branch41.ToString()=="^add")
  {
  IntValue __SIM__x9;
  { Message __SIM__receive43;
    __SIM__s4->Receive(0,__SIM__receive43);
    __SIM__receive43.GetValue(__SIM__x9);
  }
    IntValue __SIM__binop44(__SIM__v5 + __SIM__x9);
  return new __Cnt____SIM__Server2(__SIM__s4, __SIM__binop44);
  }
  else   if (__SIM__branch41.ToString()=="^done")
  {
  { 
    Message  __SIM__send45;
    __SIM__v5.ToMessage(__SIM__send45);
    __SIM__s4->Send(0,__SIM__send45);
  }
  __SIM__s4->Close(true);
  delete __SIM__s4;
  return new Cnt();
  }
  else   if (__SIM__branch41.ToString()=="^mult")
  {
  IntValue __SIM__x10;
  { Message __SIM__receive46;
    __SIM__s4->Receive(0,__SIM__receive46);
    __SIM__receive46.GetValue(__SIM__x10);
  }
    IntValue __SIM__binop47(__SIM__v5 * __SIM__x10);
  return new __Cnt____SIM__Server2(__SIM__s4, __SIM__binop47);
  }
  else   if (__SIM__branch41.ToString()=="^neg")
  {
    IntValue __SIM__intval49("0");
    IntValue __SIM__binop48(__SIM__intval49 - __SIM__v5);
  return new __Cnt____SIM__Server2(__SIM__s4, __SIM__binop48);
  }
  else throw (string)"Unknown branch: __SIM__branch41";
}Cnt *__SIM__Fork1(IntValue &__SIM__id2,ChannelsValue &__SIM__a1)
{
  Session*__SIM__s3=new Session_FIFO(__SIM__a1.GetValues(), 1, 2);
  int __SIM__fork50=fork();
  if (__SIM__fork50>0)
  {
    __SIM__s3->Close(false);
    delete __SIM__s3;
    IntValue __SIM__intval52("1");
    IntValue __SIM__binop51(__SIM__id2 + __SIM__intval52);
  return new __Cnt____SIM__Fork1(__SIM__binop51, __SIM__a1);
  }
  else if (__SIM__fork50==0)
  {
    IntValue __SIM__intval53("0");
  return new __Cnt____SIM__Server2(__SIM__s3, __SIM__intval53);
  }
else throw (string)"Error during fork!";
return 0;
}

/* Main process */
Cnt *__MAIN__()
{
  vector<Channel*> __SIM__chvector26;
  __SIM__chvector26.push_back(new Channel_FIFO());
  ChannelsValue __SIM__a1(__SIM__chvector26);
  int __SIM__fork27=fork();
  if (__SIM__fork27>0)
  {
    IntValue __SIM__intval28("1");
  return new __Cnt____SIM__Fork1(__SIM__intval28, __SIM__a1);
  }
  else if (__SIM__fork27==0)
  {
  Session*__SIM__s11=new Session_FIFO(__SIM__a1.GetValues(), 0, 2);
  {
    Message  __SIM__select29;
    __SIM__select29.AddData("^add", 5);
__SIM__s11->Send(1,__SIM__select29);
  }
  { 
    IntValue __SIM__intval31("5");
    Message  __SIM__send30;
    __SIM__intval31.ToMessage(__SIM__send30);
    __SIM__s11->Send(1,__SIM__send30);
  }
  {
    Message  __SIM__select32;
    __SIM__select32.AddData("^mult", 6);
__SIM__s11->Send(1,__SIM__select32);
  }
  { 
    IntValue __SIM__intval34("3");
    Message  __SIM__send33;
    __SIM__intval34.ToMessage(__SIM__send33);
    __SIM__s11->Send(1,__SIM__send33);
  }
  {
    Message  __SIM__select35;
    __SIM__select35.AddData("^mult", 6);
__SIM__s11->Send(1,__SIM__select35);
  }
  { 
    IntValue __SIM__intval37("2");
    Message  __SIM__send36;
    __SIM__intval37.ToMessage(__SIM__send36);
    __SIM__s11->Send(1,__SIM__send36);
  }
  {
    Message  __SIM__select38;
    __SIM__select38.AddData("^neg", 5);
__SIM__s11->Send(1,__SIM__select38);
  }
  {
    Message  __SIM__select39;
    __SIM__select39.AddData("^done", 6);
__SIM__s11->Send(1,__SIM__select39);
  }
  IntValue __SIM__x12;
  { Message __SIM__receive40;
    __SIM__s11->Receive(1,__SIM__receive40);
    __SIM__receive40.GetValue(__SIM__x12);
  }
  cout << __SIM__x12.ToString() << endl;
  __SIM__s11->Close(true);
  delete __SIM__s11;
  return new Cnt();
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
