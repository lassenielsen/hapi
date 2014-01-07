/* Pre compilation output:
******************* Program *******************
(nu empty_set:rec $set.
              1=>2
              { ^Delete[[true]]:
                  Gend,
                ^Member[[true]]:
                  1=>2<Int>;
                  2=>1<Bool>;
                  $set
              })
  ( def EmptySet()=
       link(2,empty_set,s,2);
       ( EmptySet()
       | def Empty(this: rec %set.
                         1>>
                         { ^Delete[[true]]:
                           Lend,
                           ^Member[[true]]:
                           1 >> <Int>
                           1 << <Bool>;
                           %set
                         }@(2 of 2))=
            this[1] >>
            { ^Delete:
              end,
              ^Member:
              this[1] >> x;
              this[1] << false;
              Empty(this)
            }
         in Empty(s)
       )
    in EmptySet()
  | (nu singleton_set:1=>2<Int>;
                      rec $set.
                      1=>2
                      { ^Delete[[true]]:
                          Gend,
                        ^Member[[true]]:
                          1=>2<Int>;
                          2=>1<Bool>;
                          $set
                      })
      ( def SingletonSet()=
           link(2,singleton_set,s,2);
           ( SingletonSet()
           | s[1] >> elt;
             def Singleton(this: rec %set.
                                 1>>
                                 { ^Delete[[true]]:
                                   Lend,
                                   ^Member[[true]]:
                                   1 >> <Int>
                                   1 << <Bool>;
                                   %set
                                 }@(2 of 2))=
                this[1] >>
                { ^Delete:
                  end,
                  ^Member:
                  this[1] >> x;
                  this[1] << (x = elt);
                  Singleton(this)
                }
             in Singleton(s)
           )
        in SingletonSet()
      | (nu union_set:1=>2<rec %set.
                           2<<
                           { ^Delete[[true]]:
                             Lend,
                             ^Member[[true]]:
                             2 << <Int>;
                             2 >> <Bool>
                             %set
                           }@(1 of 2)>;
                      1=>2<rec %set.
                           2<<
                           { ^Delete[[true]]:
                             Lend,
                             ^Member[[true]]:
                             2 << <Int>;
                             2 >> <Bool>
                             %set
                           }@(1 of 2)>;
                      rec $set.
                      1=>2
                      { ^Delete[[true]]:
                          Gend,
                        ^Member[[true]]:
                          1=>2<Int>;
                          2=>1<Bool>;
                          $set
                      })
          ( def UnionSet()=
               link(2,union_set,s,2);
               ( UnionSet()
               | def Union(this: rec %set.
                                 1>>
                                 { ^Delete[[true]]:
                                   Lend,
                                   ^Member[[true]]:
                                   1 >> <Int>
                                   1 << <Bool>;
                                   %set
                                 }@(2 of 2),
                           set1: rec %set.
                                 2<<
                                 { ^Delete[[true]]:
                                   Lend,
                                   ^Member[[true]]:
                                   2 << <Int>;
                                   2 >> <Bool>
                                   %set
                                 }@(1 of 2),
                           set2: rec %set.
                                 2<<
                                 { ^Delete[[true]]:
                                   Lend,
                                   ^Member[[true]]:
                                   2 << <Int>;
                                   2 >> <Bool>
                                   %set
                                 }@(1 of 2))=
                    this[1] >>
                    { ^Delete:
                      set1[2] << ^Delete;
                      set2[2] << ^Delete;
                      end,
                      ^Member:
                      set1[2] << ^Member;
                      set2[2] << ^Member;
                      this[1] >> x;
                      set1[2] << x;
                      set2[2] << x;
                      set1[2] >> r1;
                      set2[2] >> r2;
                      this[1] << (r1 or r2);
                      Union(this, set1, set2)
                    }
                 in s[1] >> s1@(1 of 2);
                    s[1] >> s2@(1 of 2);
                    Union(s, s1, s2)
               )
            in UnionSet()
          | link(2,singleton_set,s1,1);
            s1[2] << 1;
            link(2,singleton_set,s2,1);
            s2[2] << 2;
            link(2,singleton_set,s3,1);
            s3[2] << 3;
            link(2,singleton_set,s4,1);
            s4[2] << 4;
            link(2,singleton_set,s5,1);
            s5[2] << 5;
            link(2,singleton_set,s6,1);
            s6[2] << 6;
            link(2,singleton_set,s7,1);
            s7[2] << 7;
            link(2,singleton_set,s8,1);
            s8[2] << 8;
            link(2,singleton_set,s9,1);
            s9[2] << 9;
            link(2,union_set,u1,1);
            u1[2] << s1;
            u1[2] << s2;
            link(2,union_set,u2,1);
            u2[2] << s3;
            u2[2] << s4;
            link(2,union_set,u3,1);
            u3[2] << s5;
            u3[2] << s6;
            link(2,union_set,u4,1);
            u4[2] << s7;
            u4[2] << s8;
            link(2,union_set,u5,1);
            u5[2] << s9;
            u5[2] << u1;
            link(2,union_set,u6,1);
            u6[2] << u2;
            u6[2] << u3;
            link(2,union_set,u7,1);
            u7[2] << u4;
            u7[2] << u5;
            link(2,union_set,u8,1);
            u8[2] << u6;
            u8[2] << u7;
            u8[2] << ^Member;
            u8[2] << 6;
            u8[2] >> x;
            u8[2] << ^Delete;
            end
          )
      )
  )
************ Type Checking Program ************
************ Type Check Succeeded! ************
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
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
Cnt *__SIM__Empty2(Session* &__SIM__this3);
class __Cnt____SIM__Empty2 : public Cnt
{
  public:
    __Cnt____SIM__Empty2(Session* &__SIM__this3): __SIM__this3(__SIM__this3)
{}
    virtual ~__Cnt____SIM__Empty2() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__Empty2(__SIM__this3); }
  private:
Session* __SIM__this3;
};
;
Cnt *__SIM__EmptySet1(ChannelsValue &__SIM__empty_set1);
class __Cnt____SIM__EmptySet1 : public Cnt
{
  public:
    __Cnt____SIM__EmptySet1(ChannelsValue &__SIM__empty_set1): __SIM__empty_set1(__SIM__empty_set1)
{}
    virtual ~__Cnt____SIM__EmptySet1() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__EmptySet1(__SIM__empty_set1); }
  private:
ChannelsValue __SIM__empty_set1;
};
;
Cnt *__SIM__Singleton4(Session* &__SIM__this10,IntValue &__SIM__elt9);
class __Cnt____SIM__Singleton4 : public Cnt
{
  public:
    __Cnt____SIM__Singleton4(Session* &__SIM__this10,IntValue &__SIM__elt9): __SIM__this10(__SIM__this10)
, __SIM__elt9(__SIM__elt9)
{}
    virtual ~__Cnt____SIM__Singleton4() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__Singleton4(__SIM__this10, __SIM__elt9); }
  private:
Session* __SIM__this10;
IntValue __SIM__elt9;
};
;
Cnt *__SIM__SingletonSet3(ChannelsValue &__SIM__singleton_set7);
class __Cnt____SIM__SingletonSet3 : public Cnt
{
  public:
    __Cnt____SIM__SingletonSet3(ChannelsValue &__SIM__singleton_set7): __SIM__singleton_set7(__SIM__singleton_set7)
{}
    virtual ~__Cnt____SIM__SingletonSet3() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__SingletonSet3(__SIM__singleton_set7); }
  private:
ChannelsValue __SIM__singleton_set7;
};
;
Cnt *__SIM__Union6(Session* &__SIM__this16,Session* &__SIM__set117,Session* &__SIM__set218);
class __Cnt____SIM__Union6 : public Cnt
{
  public:
    __Cnt____SIM__Union6(Session* &__SIM__this16,Session* &__SIM__set117,Session* &__SIM__set218): __SIM__this16(__SIM__this16)
, __SIM__set117(__SIM__set117)
, __SIM__set218(__SIM__set218)
{}
    virtual ~__Cnt____SIM__Union6() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__Union6(__SIM__this16, __SIM__set117, __SIM__set218); }
  private:
Session* __SIM__this16;
Session* __SIM__set117;
Session* __SIM__set218;
};
;
Cnt *__SIM__UnionSet5(ChannelsValue &__SIM__union_set14);
class __Cnt____SIM__UnionSet5 : public Cnt
{
  public:
    __Cnt____SIM__UnionSet5(ChannelsValue &__SIM__union_set14): __SIM__union_set14(__SIM__union_set14)
{}
    virtual ~__Cnt____SIM__UnionSet5() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__UnionSet5(__SIM__union_set14); }
  private:
ChannelsValue __SIM__union_set14;
};
;

/* Procedure implementations */
Cnt *__SIM__Empty2(Session* &__SIM__this3)
{
  StringValue __SIM__branch152;
  {
    Message __SIM__branch153;
    __SIM__this3->Receive(0, __SIM__branch153);
    __SIM__branch153.GetValue(__SIM__branch152);
  }
  if (__SIM__branch152.ToString()=="^Delete")
  {
  return new Cnt();
  }
  else   if (__SIM__branch152.ToString()=="^Member")
  {
  IntValue __SIM__x6;
  { Message __SIM__receive154;
    __SIM__this3->Receive(0,__SIM__receive154);
    __SIM__receive154.GetValue(__SIM__x6);
  }
  { 
    BoolValue __SIM__boolval156(false);
    Message  __SIM__send155;
    __SIM__boolval156.ToMessage(__SIM__send155);
    __SIM__this3->Send(0,__SIM__send155);
  }
  return new __Cnt____SIM__Empty2(__SIM__this3);
  }
  else throw (string)"Unknown branch: __SIM__branch152";
}Cnt *__SIM__EmptySet1(ChannelsValue &__SIM__empty_set1)
{
  Session*__SIM__s2=new Session_FIFO(__SIM__empty_set1.GetValues(), 1, 2);
  int __SIM__fork157=fork();
  if (__SIM__fork157>0)
  {
    __SIM__s2->Close(false);
    delete __SIM__s2;
  return new __Cnt____SIM__EmptySet1(__SIM__empty_set1);
  }
  else if (__SIM__fork157==0)
  {
  return new __Cnt____SIM__Empty2(__SIM__s2);
  }
else throw (string)"Error during fork!";
return 0;
}Cnt *__SIM__Singleton4(Session* &__SIM__this10,IntValue &__SIM__elt9)
{
  StringValue __SIM__branch158;
  {
    Message __SIM__branch159;
    __SIM__this10->Receive(0, __SIM__branch159);
    __SIM__branch159.GetValue(__SIM__branch158);
  }
  if (__SIM__branch158.ToString()=="^Delete")
  {
  return new Cnt();
  }
  else   if (__SIM__branch158.ToString()=="^Member")
  {
  IntValue __SIM__x13;
  { Message __SIM__receive160;
    __SIM__this10->Receive(0,__SIM__receive160);
    __SIM__receive160.GetValue(__SIM__x13);
  }
  { 
    BoolValue __SIM__binop162(__SIM__x13 == __SIM__elt9);
    Message  __SIM__send161;
    __SIM__binop162.ToMessage(__SIM__send161);
    __SIM__this10->Send(0,__SIM__send161);
  }
  return new __Cnt____SIM__Singleton4(__SIM__this10, __SIM__elt9);
  }
  else throw (string)"Unknown branch: __SIM__branch158";
}Cnt *__SIM__SingletonSet3(ChannelsValue &__SIM__singleton_set7)
{
  Session*__SIM__s8=new Session_FIFO(__SIM__singleton_set7.GetValues(), 1, 2);
  int __SIM__fork163=fork();
  if (__SIM__fork163>0)
  {
    __SIM__s8->Close(false);
    delete __SIM__s8;
  return new __Cnt____SIM__SingletonSet3(__SIM__singleton_set7);
  }
  else if (__SIM__fork163==0)
  {
  IntValue __SIM__elt9;
  { Message __SIM__receive164;
    __SIM__s8->Receive(0,__SIM__receive164);
    __SIM__receive164.GetValue(__SIM__elt9);
  }
  return new __Cnt____SIM__Singleton4(__SIM__s8, __SIM__elt9);
  }
else throw (string)"Error during fork!";
return 0;
}Cnt *__SIM__Union6(Session* &__SIM__this16,Session* &__SIM__set117,Session* &__SIM__set218)
{
  StringValue __SIM__branch165;
  {
    Message __SIM__branch166;
    __SIM__this16->Receive(0, __SIM__branch166);
    __SIM__branch166.GetValue(__SIM__branch165);
  }
  if (__SIM__branch165.ToString()=="^Delete")
  {
  {
    Message  __SIM__select167;
    __SIM__select167.AddData("^Delete", 8);
__SIM__set117->Send(1,__SIM__select167);
  }
  {
    Message  __SIM__select168;
    __SIM__select168.AddData("^Delete", 8);
__SIM__set218->Send(1,__SIM__select168);
  }
  return new Cnt();
  __SIM__set218->Close(true);
  delete __SIM__set218;
  __SIM__set117->Close(true);
  delete __SIM__set117;
  }
  else   if (__SIM__branch165.ToString()=="^Member")
  {
  {
    Message  __SIM__select169;
    __SIM__select169.AddData("^Member", 8);
__SIM__set117->Send(1,__SIM__select169);
  }
  {
    Message  __SIM__select170;
    __SIM__select170.AddData("^Member", 8);
__SIM__set218->Send(1,__SIM__select170);
  }
  IntValue __SIM__x25;
  { Message __SIM__receive171;
    __SIM__this16->Receive(0,__SIM__receive171);
    __SIM__receive171.GetValue(__SIM__x25);
  }
  { 
    Message  __SIM__send172;
    __SIM__x25.ToMessage(__SIM__send172);
    __SIM__set117->Send(1,__SIM__send172);
  }
  { 
    Message  __SIM__send173;
    __SIM__x25.ToMessage(__SIM__send173);
    __SIM__set218->Send(1,__SIM__send173);
  }
  BoolValue __SIM__r126;
  { Message __SIM__receive174;
    __SIM__set117->Receive(1,__SIM__receive174);
    __SIM__receive174.GetValue(__SIM__r126);
  }
  BoolValue __SIM__r227;
  { Message __SIM__receive175;
    __SIM__set218->Receive(1,__SIM__receive175);
    __SIM__receive175.GetValue(__SIM__r227);
  }
  { 
    BoolValue __SIM__binop177(__SIM__r126 || __SIM__r227);
    Message  __SIM__send176;
    __SIM__binop177.ToMessage(__SIM__send176);
    __SIM__this16->Send(0,__SIM__send176);
  }
  return new __Cnt____SIM__Union6(__SIM__this16, __SIM__set117, __SIM__set218);
  }
  else throw (string)"Unknown branch: __SIM__branch165";
}Cnt *__SIM__UnionSet5(ChannelsValue &__SIM__union_set14)
{
  Session*__SIM__s15=new Session_FIFO(__SIM__union_set14.GetValues(), 1, 2);
  int __SIM__fork178=fork();
  if (__SIM__fork178>0)
  {
    __SIM__s15->Close(false);
    delete __SIM__s15;
  return new __Cnt____SIM__UnionSet5(__SIM__union_set14);
  }
  else if (__SIM__fork178==0)
  {
  Session* __SIM__s128;
    __SIM__s128=__SIM__s15->ReceiveSession(0);
  Session* __SIM__s229;
    __SIM__s229=__SIM__s15->ReceiveSession(0);
  return new __Cnt____SIM__Union6(__SIM__s15, __SIM__s128, __SIM__s229);
  }
else throw (string)"Error during fork!";
return 0;
}

/* Main process */
Cnt *__MAIN__()
{
  vector<Channel*> __SIM__chvector107;
  __SIM__chvector107.push_back(new Channel_FIFO());
  ChannelsValue __SIM__empty_set1(__SIM__chvector107);
  int __SIM__fork108=fork();
  if (__SIM__fork108>0)
  {
  return new __Cnt____SIM__EmptySet1(__SIM__empty_set1);
  }
  else if (__SIM__fork108==0)
  {
  vector<Channel*> __SIM__chvector109;
  __SIM__chvector109.push_back(new Channel_FIFO());
  ChannelsValue __SIM__singleton_set7(__SIM__chvector109);
  int __SIM__fork110=fork();
  if (__SIM__fork110>0)
  {
  return new __Cnt____SIM__SingletonSet3(__SIM__singleton_set7);
  }
  else if (__SIM__fork110==0)
  {
  vector<Channel*> __SIM__chvector111;
  __SIM__chvector111.push_back(new Channel_FIFO());
  ChannelsValue __SIM__union_set14(__SIM__chvector111);
  int __SIM__fork112=fork();
  if (__SIM__fork112>0)
  {
  return new __Cnt____SIM__UnionSet5(__SIM__union_set14);
  }
  else if (__SIM__fork112==0)
  {
  Session*__SIM__s134=new Session_FIFO(__SIM__singleton_set7.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval114("1");
    Message  __SIM__send113;
    __SIM__intval114.ToMessage(__SIM__send113);
    __SIM__s134->Send(1,__SIM__send113);
  }
  Session*__SIM__s235=new Session_FIFO(__SIM__singleton_set7.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval116("2");
    Message  __SIM__send115;
    __SIM__intval116.ToMessage(__SIM__send115);
    __SIM__s235->Send(1,__SIM__send115);
  }
  Session*__SIM__s336=new Session_FIFO(__SIM__singleton_set7.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval118("3");
    Message  __SIM__send117;
    __SIM__intval118.ToMessage(__SIM__send117);
    __SIM__s336->Send(1,__SIM__send117);
  }
  Session*__SIM__s437=new Session_FIFO(__SIM__singleton_set7.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval120("4");
    Message  __SIM__send119;
    __SIM__intval120.ToMessage(__SIM__send119);
    __SIM__s437->Send(1,__SIM__send119);
  }
  Session*__SIM__s538=new Session_FIFO(__SIM__singleton_set7.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval122("5");
    Message  __SIM__send121;
    __SIM__intval122.ToMessage(__SIM__send121);
    __SIM__s538->Send(1,__SIM__send121);
  }
  Session*__SIM__s639=new Session_FIFO(__SIM__singleton_set7.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval124("6");
    Message  __SIM__send123;
    __SIM__intval124.ToMessage(__SIM__send123);
    __SIM__s639->Send(1,__SIM__send123);
  }
  Session*__SIM__s740=new Session_FIFO(__SIM__singleton_set7.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval126("7");
    Message  __SIM__send125;
    __SIM__intval126.ToMessage(__SIM__send125);
    __SIM__s740->Send(1,__SIM__send125);
  }
  Session*__SIM__s841=new Session_FIFO(__SIM__singleton_set7.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval128("8");
    Message  __SIM__send127;
    __SIM__intval128.ToMessage(__SIM__send127);
    __SIM__s841->Send(1,__SIM__send127);
  }
  Session*__SIM__s942=new Session_FIFO(__SIM__singleton_set7.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval130("9");
    Message  __SIM__send129;
    __SIM__intval130.ToMessage(__SIM__send129);
    __SIM__s942->Send(1,__SIM__send129);
  }
  Session*__SIM__u143=new Session_FIFO(__SIM__union_set14.GetValues(), 0, 2);
  { 
    __SIM__u143->Delegate(1, *__SIM__s134);
  }
  __SIM__s134->Close(false);
  delete __SIM__s134;
  { 
    __SIM__u143->Delegate(1, *__SIM__s235);
  }
  __SIM__s235->Close(false);
  delete __SIM__s235;
  Session*__SIM__u244=new Session_FIFO(__SIM__union_set14.GetValues(), 0, 2);
  { 
    __SIM__u244->Delegate(1, *__SIM__s336);
  }
  __SIM__s336->Close(false);
  delete __SIM__s336;
  { 
    __SIM__u244->Delegate(1, *__SIM__s437);
  }
  __SIM__s437->Close(false);
  delete __SIM__s437;
  Session*__SIM__u345=new Session_FIFO(__SIM__union_set14.GetValues(), 0, 2);
  { 
    __SIM__u345->Delegate(1, *__SIM__s538);
  }
  __SIM__s538->Close(false);
  delete __SIM__s538;
  { 
    __SIM__u345->Delegate(1, *__SIM__s639);
  }
  __SIM__s639->Close(false);
  delete __SIM__s639;
  Session*__SIM__u446=new Session_FIFO(__SIM__union_set14.GetValues(), 0, 2);
  { 
    __SIM__u446->Delegate(1, *__SIM__s740);
  }
  __SIM__s740->Close(false);
  delete __SIM__s740;
  { 
    __SIM__u446->Delegate(1, *__SIM__s841);
  }
  __SIM__s841->Close(false);
  delete __SIM__s841;
  Session*__SIM__u547=new Session_FIFO(__SIM__union_set14.GetValues(), 0, 2);
  { 
    __SIM__u547->Delegate(1, *__SIM__s942);
  }
  __SIM__s942->Close(false);
  delete __SIM__s942;
  { 
    __SIM__u547->Delegate(1, *__SIM__u143);
  }
  __SIM__u143->Close(false);
  delete __SIM__u143;
  Session*__SIM__u648=new Session_FIFO(__SIM__union_set14.GetValues(), 0, 2);
  { 
    __SIM__u648->Delegate(1, *__SIM__u244);
  }
  __SIM__u244->Close(false);
  delete __SIM__u244;
  { 
    __SIM__u648->Delegate(1, *__SIM__u345);
  }
  __SIM__u345->Close(false);
  delete __SIM__u345;
  Session*__SIM__u749=new Session_FIFO(__SIM__union_set14.GetValues(), 0, 2);
  { 
    __SIM__u749->Delegate(1, *__SIM__u446);
  }
  __SIM__u446->Close(false);
  delete __SIM__u446;
  { 
    __SIM__u749->Delegate(1, *__SIM__u547);
  }
  __SIM__u547->Close(false);
  delete __SIM__u547;
  Session*__SIM__u850=new Session_FIFO(__SIM__union_set14.GetValues(), 0, 2);
  { 
    __SIM__u850->Delegate(1, *__SIM__u648);
  }
  __SIM__u648->Close(false);
  delete __SIM__u648;
  { 
    __SIM__u850->Delegate(1, *__SIM__u749);
  }
  __SIM__u749->Close(false);
  delete __SIM__u749;
  {
    Message  __SIM__select147;
    __SIM__select147.AddData("^Member", 8);
__SIM__u850->Send(1,__SIM__select147);
  }
  { 
    IntValue __SIM__intval149("6");
    Message  __SIM__send148;
    __SIM__intval149.ToMessage(__SIM__send148);
    __SIM__u850->Send(1,__SIM__send148);
  }
  BoolValue __SIM__x51;
  { Message __SIM__receive150;
    __SIM__u850->Receive(1,__SIM__receive150);
    __SIM__receive150.GetValue(__SIM__x51);
  }
  cout << __SIM__x51.ToString() << endl;
  {
    Message  __SIM__select151;
    __SIM__select151.AddData("^Delete", 8);
__SIM__u850->Send(1,__SIM__select151);
  }
  return new Cnt();
  __SIM__u850->Close(true);
  delete __SIM__u850;
  }
else throw (string)"Error during fork!";
return 0;
  }
else throw (string)"Error during fork!";
return 0;
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
