#include <libpi/value.hpp>
#include <libpi/bool.hpp>
#include <libpi/int.hpp>
#include <libpi/float.hpp>
#include <libpi/string.hpp>
#include <libpi/session.hpp>
#include <libpi/task/link.hpp>
#include <libpi/task/worker.hpp>
#include <algorithm>
#include <thread>
#include <signal.h>
#include <unistd.h>
using namespace std;
// HEADERS {{{
#include <iostream>
// }}}
// Value declerations {{{
std::vector<char*> _args;
libpi::Int *intval_0(new libpi::Int("0", NULL));
libpi::String *stringval_d41d8cd98f00b204e9800998ecf8427e(new libpi::String("", NULL));
libpi::Int *intval_1(new libpi::Int("1", NULL));
libpi::Int *intval_2(new libpi::Int("2", NULL));
libpi::String *stringval_1390f34ad3ce7e5037f7845236b1d188(new libpi::String("What is n? ", NULL));
libpi::String *stringval_7a454c9c018e5406a652176541eeb989(new libpi::String("Fib(", NULL));
libpi::String *stringval_0d8554966afe372ebbc81f405ba2a1da(new libpi::String(")=", NULL));
libpi::String *label_186e35ef3dd9939615c442c30b801a33(new libpi::String("^end",NULL));
libpi::String *label_7a11449475519daf7b8b8621e5b6eacc(new libpi::String("^nl",NULL));
libpi::String *label_64f10e23bd5ddafc2b83ba3a50254359(new libpi::String("^int",NULL));
libpi::String *label_cf6b4c5b94c4f75325166dbaeb4a0aae(new libpi::String("^str",NULL));
libpi::String *label_8e3aa78e57ec3533858349d5f34c05ab(new libpi::String("^input",NULL));
// }}}
// Task Types {{{
class TaskMain : public libpi::task::Task
{ public:
    TaskMain(libpi::task::Worker *worker) : libpi::task::Task(worker) { SetWorker(worker); }

    libpi::Value *var___SIM__console3;
    void Mark(std::unordered_set<Value*> &marks)
    {
      if (marks.count(this)>0)
        return;
      marks.insert(this);
      if (var___SIM__console3!=NULL) var___SIM__console3->Mark(marks);
    }
};
class Task___SIM__ConsoleSession2 : public libpi::task::Task
{ public:
    Task___SIM__ConsoleSession2(libpi::task::Worker *worker) : libpi::task::Task(worker) { SetWorker(worker); }

    libpi::Value *var___SIM__console3;
    libpi::Value *var___SIM__f11;
    libpi::Value *var___SIM__i14;
    libpi::Value *var___SIM__s5;
    libpi::Value *var___SIM__str15;
    libpi::Value *var___SIM__val12;
    libpi::Value *var___SIM__val13;
    inline void SetArg0(libpi::Value *val) {var___SIM__s5=val;}
    inline void SetArg1(libpi::Value *val) {var___SIM__console3=val;}
    void Mark(std::unordered_set<Value*> &marks)
    {
      if (marks.count(this)>0)
        return;
      marks.insert(this);
      if (var___SIM__console3!=NULL) var___SIM__console3->Mark(marks);
      if (var___SIM__f11!=NULL) var___SIM__f11->Mark(marks);
      if (var___SIM__i14!=NULL) var___SIM__i14->Mark(marks);
      if (var___SIM__s5!=NULL) var___SIM__s5->Mark(marks);
      if (var___SIM__str15!=NULL) var___SIM__str15->Mark(marks);
      if (var___SIM__val12!=NULL) var___SIM__val12->Mark(marks);
      if (var___SIM__val13!=NULL) var___SIM__val13->Mark(marks);
    }
};
class Task___SIM__Console1 : public libpi::task::Task
{ public:
    Task___SIM__Console1(libpi::task::Worker *worker) : libpi::task::Task(worker) { SetWorker(worker); }

    libpi::Value *var___SIM__console3;
    libpi::Value *var___SIM__s4;
    inline void SetArg0(libpi::Value *val) {var___SIM__console3=val;}
    void Mark(std::unordered_set<Value*> &marks)
    {
      if (marks.count(this)>0)
        return;
      marks.insert(this);
      if (var___SIM__console3!=NULL) var___SIM__console3->Mark(marks);
      if (var___SIM__s4!=NULL) var___SIM__s4->Mark(marks);
    }
};
class Task___SIM__FlatRight5 : public libpi::task::Task
{ public:
    Task___SIM__FlatRight5(libpi::task::Worker *worker) : libpi::task::Task(worker) { SetWorker(worker); }

    libpi::Value *var___SIM__f120;
    libpi::Value *var___SIM__f125;
    libpi::Value *var___SIM__f222;
    libpi::Value *var___SIM__f226;
    libpi::Value *var___SIM__fib16;
    libpi::Value *var___SIM__s119;
    libpi::Value *var___SIM__s123;
    libpi::Value *var___SIM__s17;
    libpi::Value *var___SIM__s221;
    libpi::Value *var___SIM__s224;
    libpi::Value *var___SIM__x18;
    inline void SetArg0(libpi::Value *val) {var___SIM__fib16=val;}
    inline void SetArg1(libpi::Value *val) {var___SIM__s17=val;}
    void Mark(std::unordered_set<Value*> &marks)
    {
      if (marks.count(this)>0)
        return;
      marks.insert(this);
      if (var___SIM__f120!=NULL) var___SIM__f120->Mark(marks);
      if (var___SIM__f125!=NULL) var___SIM__f125->Mark(marks);
      if (var___SIM__f222!=NULL) var___SIM__f222->Mark(marks);
      if (var___SIM__f226!=NULL) var___SIM__f226->Mark(marks);
      if (var___SIM__fib16!=NULL) var___SIM__fib16->Mark(marks);
      if (var___SIM__s119!=NULL) var___SIM__s119->Mark(marks);
      if (var___SIM__s123!=NULL) var___SIM__s123->Mark(marks);
      if (var___SIM__s17!=NULL) var___SIM__s17->Mark(marks);
      if (var___SIM__s221!=NULL) var___SIM__s221->Mark(marks);
      if (var___SIM__s224!=NULL) var___SIM__s224->Mark(marks);
      if (var___SIM__x18!=NULL) var___SIM__x18->Mark(marks);
    }
};
class Task___SIM__Fib3 : public libpi::task::Task
{ public:
    Task___SIM__Fib3(libpi::task::Worker *worker) : libpi::task::Task(worker) { SetWorker(worker); }

    libpi::Value *var___SIM__fib16;
    libpi::Value *var___SIM__s17;
    inline void SetArg0(libpi::Value *val) {var___SIM__fib16=val;}
    void Mark(std::unordered_set<Value*> &marks)
    {
      if (marks.count(this)>0)
        return;
      marks.insert(this);
      if (var___SIM__fib16!=NULL) var___SIM__fib16->Mark(marks);
      if (var___SIM__s17!=NULL) var___SIM__s17->Mark(marks);
    }
};
class Task___SIM__StartService4 : public libpi::task::Task
{ public:
    Task___SIM__StartService4(libpi::task::Worker *worker) : libpi::task::Task(worker) { SetWorker(worker); }

    libpi::Value *var___SIM__fib16;
    libpi::Value *var___SIM__i27;
    inline void SetArg0(libpi::Value *val) {var___SIM__i27=val;}
    inline void SetArg1(libpi::Value *val) {var___SIM__fib16=val;}
    void Mark(std::unordered_set<Value*> &marks)
    {
      if (marks.count(this)>0)
        return;
      marks.insert(this);
      if (var___SIM__fib16!=NULL) var___SIM__fib16->Mark(marks);
      if (var___SIM__i27!=NULL) var___SIM__i27->Mark(marks);
    }
};
class Task___SIM__FlatRight6 : public libpi::task::Task
{ public:
    Task___SIM__FlatRight6(libpi::task::Worker *worker) : libpi::task::Task(worker) { SetWorker(worker); }

    libpi::Value *var___SIM__c28;
    libpi::Value *var___SIM__c32;
    libpi::Value *var___SIM__console3;
    libpi::Value *var___SIM__f30;
    libpi::Value *var___SIM__f33;
    libpi::Value *var___SIM__fib16;
    libpi::Value *var___SIM__r31;
    libpi::Value *var___SIM__r35;
    libpi::Value *var___SIM__v29;
    libpi::Value *var___SIM__v34;
    inline void SetArg0(libpi::Value *val) {var___SIM__console3=val;}
    inline void SetArg1(libpi::Value *val) {var___SIM__fib16=val;}
    void Mark(std::unordered_set<Value*> &marks)
    {
      if (marks.count(this)>0)
        return;
      marks.insert(this);
      if (var___SIM__c28!=NULL) var___SIM__c28->Mark(marks);
      if (var___SIM__c32!=NULL) var___SIM__c32->Mark(marks);
      if (var___SIM__console3!=NULL) var___SIM__console3->Mark(marks);
      if (var___SIM__f30!=NULL) var___SIM__f30->Mark(marks);
      if (var___SIM__f33!=NULL) var___SIM__f33->Mark(marks);
      if (var___SIM__fib16!=NULL) var___SIM__fib16->Mark(marks);
      if (var___SIM__r31!=NULL) var___SIM__r31->Mark(marks);
      if (var___SIM__r35!=NULL) var___SIM__r35->Mark(marks);
      if (var___SIM__v29!=NULL) var___SIM__v29->Mark(marks);
      if (var___SIM__v34!=NULL) var___SIM__v34->Mark(marks);
    }
};
class Task___SIM__FlatRight7 : public libpi::task::Task
{ public:
    Task___SIM__FlatRight7(libpi::task::Worker *worker) : libpi::task::Task(worker) { SetWorker(worker); }

    libpi::Value *var___SIM__console3;
    libpi::Value *var___SIM__fib16;
    inline void SetArg0(libpi::Value *val) {var___SIM__console3=val;}
    void Mark(std::unordered_set<Value*> &marks)
    {
      if (marks.count(this)>0)
        return;
      marks.insert(this);
      if (var___SIM__console3!=NULL) var___SIM__console3->Mark(marks);
      if (var___SIM__fib16!=NULL) var___SIM__fib16->Mark(marks);
    }
};
// }}}
// All Methods {{{
inline bool _methods(libpi::task::Task *&_task)
{ size_t _steps=0;
  void *_label=_task->GetLabel();
  if (_label!=NULL)
    goto *_label;
  method_Main:
  #define _this ((TaskMain*)_task)
  { // Main {{{

    _task->SetLabel(&&__SIM__checkpoint_316);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_316:

    _this->var___SIM__console3=new libpi::task::Link(2,&_task->GetWorker());

    _task->SetLabel(&&__SIM__checkpoint_317);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_317:

    { Task___SIM__FlatRight7 *__SIM__task318(new Task___SIM__FlatRight7(&_task->GetWorker()));
      __SIM__task318->SetArg0(_this->var___SIM__console3);
      __SIM__task318->SetLabel(&&method___SIM__FlatRight7);
      _task->GetWorker().AddTask(__SIM__task318);
    }

    _task->SetLabel(&&__SIM__checkpoint_320);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_320:

    { Task___SIM__Console1 *__SIM__task319 = new Task___SIM__Console1(&_this->GetWorker());
      __SIM__task319->SetArg0(_this->var___SIM__console3);
      __SIM__task319->SetLabel(&&method___SIM__Console1);
      _task=__SIM__task319;
    }
    goto method___SIM__Console1;
  } // }}}

/* Procedure implementations */
  method___SIM__ConsoleSession2: // {{{
  #undef _this
  #define _this ((Task___SIM__ConsoleSession2*)_task)
  {

    _task->SetLabel(&&__SIM__checkpoint_124);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_124:

    _task->SetLabel(&&__SIM__checkpoint___SIM__ConsoleSession2123);
    if (!((libpi::Session*)_this->var___SIM__s5)->Receive(1,_task,_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__ConsoleSession2123:
    if (((libpi::String*)_task->tmp)->GetValue()=="^end")
    {
      _task->tmp=NULL;
    ((libpi::Session*)_this->var___SIM__s5)->Close(true);
    _this->var___SIM__s5=NULL;

    _task->SetLabel(&&__SIM__checkpoint_126);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_126:

    { Task___SIM__Console1 *__SIM__task125 = new Task___SIM__Console1(&_this->GetWorker());
      __SIM__task125->SetArg0(_this->var___SIM__console3);
      __SIM__task125->SetLabel(&&method___SIM__Console1);
      _task=__SIM__task125;
    }
    goto method___SIM__Console1;
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^flt")
    {
      _task->tmp=NULL;

    _task->SetLabel(&&__SIM__checkpoint_128);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_128:

    // ~s5[2] >> ~f11;
    _task->SetLabel(&&__SIM__receive127);
    if (!((libpi::Session*)_this->var___SIM__s5)->Receive(1,_task,_this->var___SIM__f11))
      return false;
    __SIM__receive127:

    _task->SetLabel(&&__SIM__checkpoint_131);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_131:

  // HOST STATEMENT BEGIN
    std::cout << ((libpi::Float*) (_this->var___SIM__f11) )->ToString() << std::flush;  // HOST STATEMENT END

    _task->SetLabel(&&__SIM__checkpoint_130);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_130:

    { Task___SIM__ConsoleSession2 *__SIM__task129 = new Task___SIM__ConsoleSession2(&_this->GetWorker());
      __SIM__task129->SetArg0(_this->var___SIM__s5);
      __SIM__task129->SetArg1(_this->var___SIM__console3);
      __SIM__task129->SetLabel(&&method___SIM__ConsoleSession2);
      _task=__SIM__task129;
    }
    goto method___SIM__ConsoleSession2;
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^input")
    {
      _task->tmp=NULL;

    _task->SetLabel(&&__SIM__checkpoint_133);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_133:

    _task->SetLabel(&&__SIM__checkpoint___SIM__ConsoleSession2132);
    if (!((libpi::Session*)_this->var___SIM__s5)->Receive(1,_task,_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__ConsoleSession2132:
    if (((libpi::String*)_task->tmp)->GetValue()=="^int")
    {
      _task->tmp=NULL;

    _task->SetLabel(&&__SIM__checkpoint_134);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_134:

    {
      _this->var___SIM__val12=intval_0;
    }

    _task->SetLabel(&&__SIM__checkpoint_139);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_139:

  // HOST STATEMENT BEGIN
    { long _l; std::cin >> _l;  (_this->var___SIM__val12) =new libpi::Int(_l,&_task->GetWorker()); }  // HOST STATEMENT END

    _task->SetLabel(&&__SIM__checkpoint_136);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_136:

    // ~s5[2] << ~val12;
    { 
      ((libpi::Session*)_this->var___SIM__s5)->Send(1,_task,_this->var___SIM__val12);
      _task->GetWorker().GCMark(_this->var___SIM__val12);
    }

    _task->SetLabel(&&__SIM__checkpoint_138);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_138:

    { Task___SIM__ConsoleSession2 *__SIM__task137 = new Task___SIM__ConsoleSession2(&_this->GetWorker());
      __SIM__task137->SetArg0(_this->var___SIM__s5);
      __SIM__task137->SetArg1(_this->var___SIM__console3);
      __SIM__task137->SetLabel(&&method___SIM__ConsoleSession2);
      _task=__SIM__task137;
    }
    goto method___SIM__ConsoleSession2;
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^str")
    {
      _task->tmp=NULL;

    _task->SetLabel(&&__SIM__checkpoint_140);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_140:

    {
      _this->var___SIM__val13=stringval_d41d8cd98f00b204e9800998ecf8427e;
    }

    _task->SetLabel(&&__SIM__checkpoint_145);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_145:

  // HOST STATEMENT BEGIN
    { std::string _s; std::cin >> _s;  (_this->var___SIM__val13) =new libpi::String(_s,&_task->GetWorker()); }  // HOST STATEMENT END

    _task->SetLabel(&&__SIM__checkpoint_142);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_142:

    // ~s5[2] << ~val13;
    { 
      ((libpi::Session*)_this->var___SIM__s5)->Send(1,_task,_this->var___SIM__val13);
      _task->GetWorker().GCMark(_this->var___SIM__val13);
    }

    _task->SetLabel(&&__SIM__checkpoint_144);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_144:

    { Task___SIM__ConsoleSession2 *__SIM__task143 = new Task___SIM__ConsoleSession2(&_this->GetWorker());
      __SIM__task143->SetArg0(_this->var___SIM__s5);
      __SIM__task143->SetArg1(_this->var___SIM__console3);
      __SIM__task143->SetLabel(&&method___SIM__ConsoleSession2);
      _task=__SIM__task143;
    }
    goto method___SIM__ConsoleSession2;
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^int")
    {
      _task->tmp=NULL;

    _task->SetLabel(&&__SIM__checkpoint_147);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_147:

    // ~s5[2] >> ~i14;
    _task->SetLabel(&&__SIM__receive146);
    if (!((libpi::Session*)_this->var___SIM__s5)->Receive(1,_task,_this->var___SIM__i14))
      return false;
    __SIM__receive146:

    _task->SetLabel(&&__SIM__checkpoint_150);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_150:

  // HOST STATEMENT BEGIN
    std::cout << ((libpi::Int*) (_this->var___SIM__i14) )->ToString() << std::flush;  // HOST STATEMENT END

    _task->SetLabel(&&__SIM__checkpoint_149);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_149:

    { Task___SIM__ConsoleSession2 *__SIM__task148 = new Task___SIM__ConsoleSession2(&_this->GetWorker());
      __SIM__task148->SetArg0(_this->var___SIM__s5);
      __SIM__task148->SetArg1(_this->var___SIM__console3);
      __SIM__task148->SetLabel(&&method___SIM__ConsoleSession2);
      _task=__SIM__task148;
    }
    goto method___SIM__ConsoleSession2;
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^nl")
    {
      _task->tmp=NULL;

    _task->SetLabel(&&__SIM__checkpoint_153);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_153:

  // HOST STATEMENT BEGIN
    std::cout << std::endl;  // HOST STATEMENT END

    _task->SetLabel(&&__SIM__checkpoint_152);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_152:

    { Task___SIM__ConsoleSession2 *__SIM__task151 = new Task___SIM__ConsoleSession2(&_this->GetWorker());
      __SIM__task151->SetArg0(_this->var___SIM__s5);
      __SIM__task151->SetArg1(_this->var___SIM__console3);
      __SIM__task151->SetLabel(&&method___SIM__ConsoleSession2);
      _task=__SIM__task151;
    }
    goto method___SIM__ConsoleSession2;
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^str")
    {
      _task->tmp=NULL;

    _task->SetLabel(&&__SIM__checkpoint_155);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_155:

    // ~s5[2] >> ~str15;
    _task->SetLabel(&&__SIM__receive154);
    if (!((libpi::Session*)_this->var___SIM__s5)->Receive(1,_task,_this->var___SIM__str15))
      return false;
    __SIM__receive154:

    _task->SetLabel(&&__SIM__checkpoint_158);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_158:

  // HOST STATEMENT BEGIN
    std::cout << ((libpi::String*) (_this->var___SIM__str15) )->GetValue() << std::flush;  // HOST STATEMENT END

    _task->SetLabel(&&__SIM__checkpoint_157);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_157:

    { Task___SIM__ConsoleSession2 *__SIM__task156 = new Task___SIM__ConsoleSession2(&_this->GetWorker());
      __SIM__task156->SetArg0(_this->var___SIM__s5);
      __SIM__task156->SetArg1(_this->var___SIM__console3);
      __SIM__task156->SetLabel(&&method___SIM__ConsoleSession2);
      _task=__SIM__task156;
    }
    goto method___SIM__ConsoleSession2;
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
  } // }}}
  method___SIM__Console1: // {{{
  #undef _this
  #define _this ((Task___SIM__Console1*)_task)
  {

    _task->SetLabel(&&__SIM__checkpoint_159);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_159:

    { // link
      _task->tmps.clear();
        _task->SetLabel(&&__SIM__receive___SIM__Console1160);
        if (!((libpi::task::Link*)_this->var___SIM__console3)->GetChannels()[0]->Receive(_task,_task->tmp))
          return false;
        __SIM__receive___SIM__Console1160:
        _task->tmps.push_back(_task->tmp);
      vector<vector<libpi::Channel*> > inChannels(2);
      vector<vector<libpi::Channel*> > outChannels(2);
      // Optimize vector inserts
        inChannels[0].resize(2);
        outChannels[0].resize(2);
        inChannels[1].resize(2);
        outChannels[1].resize(2);
      // Create channels
      inChannels[0][0]=new libpi::task::Channel(&_task->GetWorker());
      inChannels[0][1]=new libpi::task::Channel(&_task->GetWorker());
      inChannels[1][0]=((libpi::Channel*)_task->tmps[0]);
      inChannels[1][1]=new libpi::task::Channel(&_task->GetWorker());
      outChannels[0][0]=inChannels[0][0];
      outChannels[0][1]=inChannels[1][0];
      outChannels[1][0]=inChannels[0][1];
      outChannels[1][1]=inChannels[1][1];
      // Send sessions
      { libpi::Session *_s(new libpi::Session(1,2, inChannels[1], outChannels[1],&_task->GetWorker()));
        ((libpi::Channel*)_task->tmps[0])->Send(_task,_s);
        _task->GetWorker().GCMark(_s);
      }
      _task->tmps.clear(); // Will be deleted by receivers
      // Create local session
      _this->var___SIM__s4=new libpi::Session(0,2,inChannels[0],outChannels[0],&_task->GetWorker());
    }

    _task->SetLabel(&&__SIM__checkpoint_162);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_162:

    { Task___SIM__ConsoleSession2 *__SIM__task161 = new Task___SIM__ConsoleSession2(&_this->GetWorker());
      __SIM__task161->SetArg0(_this->var___SIM__s4);
      __SIM__task161->SetArg1(_this->var___SIM__console3);
      __SIM__task161->SetLabel(&&method___SIM__ConsoleSession2);
      _task=__SIM__task161;
    }
    goto method___SIM__ConsoleSession2;
  } // }}}
  method___SIM__FlatRight5: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight5*)_task)
  {

    _task->SetLabel(&&__SIM__checkpoint_164);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_164:

    // ~s17[2] >> ~x18;
    _task->SetLabel(&&__SIM__receive163);
    if (!((libpi::Session*)_this->var___SIM__s17)->Receive(1,_task,_this->var___SIM__x18))
      return false;
    __SIM__receive163:

    _task->SetLabel(&&__SIM__checkpoint_167);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_167:

    {
      libpi::Bool *__SIM__binop168=(*dynamic_cast<libpi::Int*>(_this->var___SIM__x18)) <= (*dynamic_cast<libpi::Int*>(intval_1));      if (((libpi::Bool*)__SIM__binop168)->GetValue())
        goto __SIM____SIM__FlatRight5_true165;
      else
        goto __SIM____SIM__FlatRight5_false166;
    }
    __SIM____SIM__FlatRight5_true165:

    _task->SetLabel(&&__SIM__checkpoint_210);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_210:

    // ~s17[2] << 1;
    { 
      ((libpi::Session*)_this->var___SIM__s17)->Send(1,_task,intval_1);
      _task->GetWorker().GCMark(intval_1);
    }
  ((libpi::Session*)_this->var___SIM__s17)->Close(true);
  _this->var___SIM__s17=NULL;
    return false;

    __SIM____SIM__FlatRight5_false166:

    _task->SetLabel(&&__SIM__checkpoint_171);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_171:

    {
      libpi::Int *__SIM__systemexp173(new libpi::Int(libpi::task::Worker::TargetTasks,&_task->GetWorker()));
      libpi::Int *__SIM__systemexp174(new libpi::Int(libpi::task::Worker::ActiveTasks,&_task->GetWorker()));
      libpi::Bool *__SIM__binop172=(*dynamic_cast<libpi::Int*>(__SIM__systemexp173)) <= (*dynamic_cast<libpi::Int*>(__SIM__systemexp174));      if (((libpi::Bool*)__SIM__binop172)->GetValue())
        goto __SIM____SIM__FlatRight5_true169;
      else
        goto __SIM____SIM__FlatRight5_false170;
    }
    __SIM____SIM__FlatRight5_true169:

    _task->SetLabel(&&__SIM__checkpoint_192);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_192:

    { _task->tmp=new libpi::task::Channel(&_task->GetWorker());
      ((libpi::task::Link*)(_this->var___SIM__fib16))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__FlatRight5193);
      if (!((libpi::task::Channel*)_task->tmp)->Receive(_task,_this->var___SIM__s119))
        return false;
      __SIM__receive___SIM__FlatRight5193:
      // Do not delete _task->tmp as it is reused in session!
      _task->tmp=NULL;
    }

    _task->SetLabel(&&__SIM__checkpoint_195);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_195:

    // ~s119[1] << (~x18 - 1);
    { 
      libpi::Int *__SIM__binop196(new libpi::Int(*dynamic_cast<libpi::Int*>(_this->var___SIM__x18), *dynamic_cast<libpi::Int*>(intval_1), libpi::Int::OP_SUB,&_task->GetWorker()));
      ((libpi::Session*)_this->var___SIM__s119)->Send(0,_task,__SIM__binop196);
      _task->GetWorker().GCMark(__SIM__binop196);
    }

    _task->SetLabel(&&__SIM__checkpoint_198);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_198:

    // ~s119[1] >> ~f120;
    _task->SetLabel(&&__SIM__receive197);
    if (!((libpi::Session*)_this->var___SIM__s119)->Receive(0,_task,_this->var___SIM__f120))
      return false;
    __SIM__receive197:
    // Session complete
    ((libpi::Session*)_this->var___SIM__s119)->Close(true);
    _this->var___SIM__s119=NULL;

    _task->SetLabel(&&__SIM__checkpoint_199);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_199:

    { _task->tmp=new libpi::task::Channel(&_task->GetWorker());
      ((libpi::task::Link*)(_this->var___SIM__fib16))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__FlatRight5200);
      if (!((libpi::task::Channel*)_task->tmp)->Receive(_task,_this->var___SIM__s221))
        return false;
      __SIM__receive___SIM__FlatRight5200:
      // Do not delete _task->tmp as it is reused in session!
      _task->tmp=NULL;
    }

    _task->SetLabel(&&__SIM__checkpoint_202);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_202:

    // ~s221[1] << (~x18 - 2);
    { 
      libpi::Int *__SIM__binop203(new libpi::Int(*dynamic_cast<libpi::Int*>(_this->var___SIM__x18), *dynamic_cast<libpi::Int*>(intval_2), libpi::Int::OP_SUB,&_task->GetWorker()));
      ((libpi::Session*)_this->var___SIM__s221)->Send(0,_task,__SIM__binop203);
      _task->GetWorker().GCMark(__SIM__binop203);
    }

    _task->SetLabel(&&__SIM__checkpoint_205);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_205:

    // ~s221[1] >> ~f222;
    _task->SetLabel(&&__SIM__receive204);
    if (!((libpi::Session*)_this->var___SIM__s221)->Receive(0,_task,_this->var___SIM__f222))
      return false;
    __SIM__receive204:
    // Session complete
    ((libpi::Session*)_this->var___SIM__s221)->Close(true);
    _this->var___SIM__s221=NULL;

    _task->SetLabel(&&__SIM__checkpoint_207);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_207:

    // ~s17[2] << (~f120 + ~f222);
    { 
      libpi::Int *__SIM__binop208(new libpi::Int(*dynamic_cast<libpi::Int*>(_this->var___SIM__f120), *dynamic_cast<libpi::Int*>(_this->var___SIM__f222), libpi::Int::OP_ADD,&_task->GetWorker()));
      ((libpi::Session*)_this->var___SIM__s17)->Send(1,_task,__SIM__binop208);
      _task->GetWorker().GCMark(__SIM__binop208);
    }
  ((libpi::Session*)_this->var___SIM__s17)->Close(true);
  _this->var___SIM__s17=NULL;
    return false;

    __SIM____SIM__FlatRight5_false170:

    _task->SetLabel(&&__SIM__checkpoint_175);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_175:

    { _task->tmp=new libpi::task::Channel(&_task->GetWorker());
      ((libpi::task::Link*)(_this->var___SIM__fib16))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__FlatRight5176);
      if (!((libpi::task::Channel*)_task->tmp)->Receive(_task,_this->var___SIM__s123))
        return false;
      __SIM__receive___SIM__FlatRight5176:
      // Do not delete _task->tmp as it is reused in session!
      _task->tmp=NULL;
    }

    _task->SetLabel(&&__SIM__checkpoint_178);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_178:

    // ~s123[1] << (~x18 - 1);
    { 
      libpi::Int *__SIM__binop179(new libpi::Int(*dynamic_cast<libpi::Int*>(_this->var___SIM__x18), *dynamic_cast<libpi::Int*>(intval_1), libpi::Int::OP_SUB,&_task->GetWorker()));
      ((libpi::Session*)_this->var___SIM__s123)->Send(0,_task,__SIM__binop179);
      _task->GetWorker().GCMark(__SIM__binop179);
    }

    _task->SetLabel(&&__SIM__checkpoint_180);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_180:

    { _task->tmp=new libpi::task::Channel(&_task->GetWorker());
      ((libpi::task::Link*)(_this->var___SIM__fib16))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__FlatRight5181);
      if (!((libpi::task::Channel*)_task->tmp)->Receive(_task,_this->var___SIM__s224))
        return false;
      __SIM__receive___SIM__FlatRight5181:
      // Do not delete _task->tmp as it is reused in session!
      _task->tmp=NULL;
    }

    _task->SetLabel(&&__SIM__checkpoint_183);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_183:

    // ~s224[1] << (~x18 - 2);
    { 
      libpi::Int *__SIM__binop184(new libpi::Int(*dynamic_cast<libpi::Int*>(_this->var___SIM__x18), *dynamic_cast<libpi::Int*>(intval_2), libpi::Int::OP_SUB,&_task->GetWorker()));
      ((libpi::Session*)_this->var___SIM__s224)->Send(0,_task,__SIM__binop184);
      _task->GetWorker().GCMark(__SIM__binop184);
    }

    _task->SetLabel(&&__SIM__checkpoint_186);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_186:

    // ~s123[1] >> ~f125;
    _task->SetLabel(&&__SIM__receive185);
    if (!((libpi::Session*)_this->var___SIM__s123)->Receive(0,_task,_this->var___SIM__f125))
      return false;
    __SIM__receive185:
    // Session complete
    ((libpi::Session*)_this->var___SIM__s123)->Close(true);
    _this->var___SIM__s123=NULL;

    _task->SetLabel(&&__SIM__checkpoint_188);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_188:

    // ~s224[1] >> ~f226;
    _task->SetLabel(&&__SIM__receive187);
    if (!((libpi::Session*)_this->var___SIM__s224)->Receive(0,_task,_this->var___SIM__f226))
      return false;
    __SIM__receive187:
    // Session complete
    ((libpi::Session*)_this->var___SIM__s224)->Close(true);
    _this->var___SIM__s224=NULL;

    _task->SetLabel(&&__SIM__checkpoint_190);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_190:

    // ~s17[2] << (~f125 + ~f226);
    { 
      libpi::Int *__SIM__binop191(new libpi::Int(*dynamic_cast<libpi::Int*>(_this->var___SIM__f125), *dynamic_cast<libpi::Int*>(_this->var___SIM__f226), libpi::Int::OP_ADD,&_task->GetWorker()));
      ((libpi::Session*)_this->var___SIM__s17)->Send(1,_task,__SIM__binop191);
      _task->GetWorker().GCMark(__SIM__binop191);
    }
  ((libpi::Session*)_this->var___SIM__s17)->Close(true);
  _this->var___SIM__s17=NULL;
    return false;

  } // }}}
  method___SIM__Fib3: // {{{
  #undef _this
  #define _this ((Task___SIM__Fib3*)_task)
  {

    _task->SetLabel(&&__SIM__checkpoint_211);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_211:

    { // link
      _task->tmps.clear();
        _task->SetLabel(&&__SIM__receive___SIM__Fib3212);
        if (!((libpi::task::Link*)_this->var___SIM__fib16)->GetChannels()[0]->Receive(_task,_task->tmp))
          return false;
        __SIM__receive___SIM__Fib3212:
        _task->tmps.push_back(_task->tmp);
      vector<vector<libpi::Channel*> > inChannels(2);
      vector<vector<libpi::Channel*> > outChannels(2);
      // Optimize vector inserts
        inChannels[0].resize(2);
        outChannels[0].resize(2);
        inChannels[1].resize(2);
        outChannels[1].resize(2);
      // Create channels
      inChannels[0][0]=new libpi::task::Channel(&_task->GetWorker());
      inChannels[0][1]=new libpi::task::Channel(&_task->GetWorker());
      inChannels[1][0]=((libpi::Channel*)_task->tmps[0]);
      inChannels[1][1]=new libpi::task::Channel(&_task->GetWorker());
      outChannels[0][0]=inChannels[0][0];
      outChannels[0][1]=inChannels[1][0];
      outChannels[1][0]=inChannels[0][1];
      outChannels[1][1]=inChannels[1][1];
      // Send sessions
      { libpi::Session *_s(new libpi::Session(1,2, inChannels[1], outChannels[1],&_task->GetWorker()));
        ((libpi::Channel*)_task->tmps[0])->Send(_task,_s);
        _task->GetWorker().GCMark(_s);
      }
      _task->tmps.clear(); // Will be deleted by receivers
      // Create local session
      _this->var___SIM__s17=new libpi::Session(0,2,inChannels[0],outChannels[0],&_task->GetWorker());
    }

    _task->SetLabel(&&__SIM__checkpoint_213);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_213:

    { Task___SIM__FlatRight5 *__SIM__task214(new Task___SIM__FlatRight5(&_task->GetWorker()));
      __SIM__task214->SetArg0(_this->var___SIM__fib16);
      __SIM__task214->SetArg1(_this->var___SIM__s17);
      __SIM__task214->SetLabel(&&method___SIM__FlatRight5);
      _task->GetWorker().AddTask(__SIM__task214);
    }

    _task->SetLabel(&&__SIM__checkpoint_216);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_216:

    { Task___SIM__Fib3 *__SIM__task215 = new Task___SIM__Fib3(&_this->GetWorker());
      __SIM__task215->SetArg0(_this->var___SIM__fib16);
      __SIM__task215->SetLabel(&&method___SIM__Fib3);
      _task=__SIM__task215;
    }
    goto method___SIM__Fib3;
  } // }}}
  method___SIM__StartService4: // {{{
  #undef _this
  #define _this ((Task___SIM__StartService4*)_task)
  {

    _task->SetLabel(&&__SIM__checkpoint_219);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_219:

    {
      libpi::Bool *__SIM__binop220=(*dynamic_cast<libpi::Int*>(_this->var___SIM__i27)) <= (*dynamic_cast<libpi::Int*>(intval_0));      if (((libpi::Bool*)__SIM__binop220)->GetValue())
        goto __SIM____SIM__StartService4_true217;
      else
        goto __SIM____SIM__StartService4_false218;
    }
    __SIM____SIM__StartService4_true217:

    _task->SetLabel(&&__SIM__checkpoint_227);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_227:

    { Task___SIM__Fib3 *__SIM__task226 = new Task___SIM__Fib3(&_this->GetWorker());
      __SIM__task226->SetArg0(_this->var___SIM__fib16);
      __SIM__task226->SetLabel(&&method___SIM__Fib3);
      _task=__SIM__task226;
    }
    goto method___SIM__Fib3;
    __SIM____SIM__StartService4_false218:

    _task->SetLabel(&&__SIM__checkpoint_221);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_221:

    { Task___SIM__StartService4 *__SIM__task222(new Task___SIM__StartService4(&_task->GetWorker()));
      libpi::Int *__SIM__binop225(new libpi::Int(*dynamic_cast<libpi::Int*>(_this->var___SIM__i27), *dynamic_cast<libpi::Int*>(intval_1), libpi::Int::OP_SUB,&_task->GetWorker()));
      __SIM__task222->SetArg0(__SIM__binop225);
      __SIM__task222->SetArg1(_this->var___SIM__fib16);
      __SIM__task222->SetLabel(&&method___SIM__StartService4);
      _task->GetWorker().AddTask(__SIM__task222);
    }

    _task->SetLabel(&&__SIM__checkpoint_224);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_224:

    { Task___SIM__Fib3 *__SIM__task223 = new Task___SIM__Fib3(&_this->GetWorker());
      __SIM__task223->SetArg0(_this->var___SIM__fib16);
      __SIM__task223->SetLabel(&&method___SIM__Fib3);
      _task=__SIM__task223;
    }
    goto method___SIM__Fib3;
  } // }}}
  method___SIM__FlatRight6: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight6*)_task)
  {

    _task->SetLabel(&&__SIM__checkpoint_230);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_230:

    {
      libpi::Int *__SIM__systemexp232(new libpi::Int(libpi::task::Worker::TargetTasks,&_task->GetWorker()));
      libpi::Int *__SIM__systemexp233(new libpi::Int(libpi::task::Worker::ActiveTasks,&_task->GetWorker()));
      libpi::Bool *__SIM__binop231=(*dynamic_cast<libpi::Int*>(__SIM__systemexp232)) <= (*dynamic_cast<libpi::Int*>(__SIM__systemexp233));      if (((libpi::Bool*)__SIM__binop231)->GetValue())
        goto __SIM____SIM__FlatRight6_true228;
      else
        goto __SIM____SIM__FlatRight6_false229;
    }
    __SIM____SIM__FlatRight6_true228:

    _task->SetLabel(&&__SIM__checkpoint_272);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_272:

    { _task->tmp=new libpi::task::Channel(&_task->GetWorker());
      ((libpi::task::Link*)(_this->var___SIM__console3))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__FlatRight6273);
      if (!((libpi::task::Channel*)_task->tmp)->Receive(_task,_this->var___SIM__c28))
        return false;
      __SIM__receive___SIM__FlatRight6273:
      // Do not delete _task->tmp as it is reused in session!
      _task->tmp=NULL;
    }

    _task->SetLabel(&&__SIM__checkpoint_275);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_275:

    // ~c28[1] << ^str;
    ((libpi::Session*)_this->var___SIM__c28)->Send(0,_task,label_cf6b4c5b94c4f75325166dbaeb4a0aae);

    _task->SetLabel(&&__SIM__checkpoint_277);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_277:

    // ~c28[1] << "What is n? ";
    { 
      ((libpi::Session*)_this->var___SIM__c28)->Send(0,_task,stringval_1390f34ad3ce7e5037f7845236b1d188);
      _task->GetWorker().GCMark(stringval_1390f34ad3ce7e5037f7845236b1d188);
    }

    _task->SetLabel(&&__SIM__checkpoint_279);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_279:

    // ~c28[1] << ^input;
    ((libpi::Session*)_this->var___SIM__c28)->Send(0,_task,label_8e3aa78e57ec3533858349d5f34c05ab);

    _task->SetLabel(&&__SIM__checkpoint_281);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_281:

    // ~c28[1] << ^int;
    ((libpi::Session*)_this->var___SIM__c28)->Send(0,_task,label_64f10e23bd5ddafc2b83ba3a50254359);

    _task->SetLabel(&&__SIM__checkpoint_283);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_283:

    // ~c28[1] >> ~v29;
    _task->SetLabel(&&__SIM__receive282);
    if (!((libpi::Session*)_this->var___SIM__c28)->Receive(0,_task,_this->var___SIM__v29))
      return false;
    __SIM__receive282:

    _task->SetLabel(&&__SIM__checkpoint_284);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_284:

    { _task->tmp=new libpi::task::Channel(&_task->GetWorker());
      ((libpi::task::Link*)(_this->var___SIM__fib16))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__FlatRight6285);
      if (!((libpi::task::Channel*)_task->tmp)->Receive(_task,_this->var___SIM__f30))
        return false;
      __SIM__receive___SIM__FlatRight6285:
      // Do not delete _task->tmp as it is reused in session!
      _task->tmp=NULL;
    }

    _task->SetLabel(&&__SIM__checkpoint_287);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_287:

    // ~f30[1] << ~v29;
    { 
      ((libpi::Session*)_this->var___SIM__f30)->Send(0,_task,_this->var___SIM__v29);
      _task->GetWorker().GCMark(_this->var___SIM__v29);
    }

    _task->SetLabel(&&__SIM__checkpoint_289);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_289:

    // ~f30[1] >> ~r31;
    _task->SetLabel(&&__SIM__receive288);
    if (!((libpi::Session*)_this->var___SIM__f30)->Receive(0,_task,_this->var___SIM__r31))
      return false;
    __SIM__receive288:
    // Session complete
    ((libpi::Session*)_this->var___SIM__f30)->Close(true);
    _this->var___SIM__f30=NULL;

    _task->SetLabel(&&__SIM__checkpoint_291);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_291:

    // ~c28[1] << ^str;
    ((libpi::Session*)_this->var___SIM__c28)->Send(0,_task,label_cf6b4c5b94c4f75325166dbaeb4a0aae);

    _task->SetLabel(&&__SIM__checkpoint_293);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_293:

    // ~c28[1] << "Fib(";
    { 
      ((libpi::Session*)_this->var___SIM__c28)->Send(0,_task,stringval_7a454c9c018e5406a652176541eeb989);
      _task->GetWorker().GCMark(stringval_7a454c9c018e5406a652176541eeb989);
    }

    _task->SetLabel(&&__SIM__checkpoint_295);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_295:

    // ~c28[1] << ^int;
    ((libpi::Session*)_this->var___SIM__c28)->Send(0,_task,label_64f10e23bd5ddafc2b83ba3a50254359);

    _task->SetLabel(&&__SIM__checkpoint_297);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_297:

    // ~c28[1] << ~v29;
    { 
      ((libpi::Session*)_this->var___SIM__c28)->Send(0,_task,_this->var___SIM__v29);
      _task->GetWorker().GCMark(_this->var___SIM__v29);
    }

    _task->SetLabel(&&__SIM__checkpoint_299);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_299:

    // ~c28[1] << ^str;
    ((libpi::Session*)_this->var___SIM__c28)->Send(0,_task,label_cf6b4c5b94c4f75325166dbaeb4a0aae);

    _task->SetLabel(&&__SIM__checkpoint_301);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_301:

    // ~c28[1] << ")=";
    { 
      ((libpi::Session*)_this->var___SIM__c28)->Send(0,_task,stringval_0d8554966afe372ebbc81f405ba2a1da);
      _task->GetWorker().GCMark(stringval_0d8554966afe372ebbc81f405ba2a1da);
    }

    _task->SetLabel(&&__SIM__checkpoint_303);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_303:

    // ~c28[1] << ^int;
    ((libpi::Session*)_this->var___SIM__c28)->Send(0,_task,label_64f10e23bd5ddafc2b83ba3a50254359);

    _task->SetLabel(&&__SIM__checkpoint_305);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_305:

    // ~c28[1] << ~r31;
    { 
      ((libpi::Session*)_this->var___SIM__c28)->Send(0,_task,_this->var___SIM__r31);
      _task->GetWorker().GCMark(_this->var___SIM__r31);
    }

    _task->SetLabel(&&__SIM__checkpoint_307);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_307:

    // ~c28[1] << ^nl;
    ((libpi::Session*)_this->var___SIM__c28)->Send(0,_task,label_7a11449475519daf7b8b8621e5b6eacc);

    _task->SetLabel(&&__SIM__checkpoint_309);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_309:

    // ~c28[1] << ^end;
    ((libpi::Session*)_this->var___SIM__c28)->Send(0,_task,label_186e35ef3dd9939615c442c30b801a33);
    ((libpi::Session*)_this->var___SIM__c28)->Close(true);
    _this->var___SIM__c28=NULL;
    return false;

    __SIM____SIM__FlatRight6_false229:

    _task->SetLabel(&&__SIM__checkpoint_234);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_234:

    { _task->tmp=new libpi::task::Channel(&_task->GetWorker());
      ((libpi::task::Link*)(_this->var___SIM__console3))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__FlatRight6235);
      if (!((libpi::task::Channel*)_task->tmp)->Receive(_task,_this->var___SIM__c32))
        return false;
      __SIM__receive___SIM__FlatRight6235:
      // Do not delete _task->tmp as it is reused in session!
      _task->tmp=NULL;
    }

    _task->SetLabel(&&__SIM__checkpoint_237);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_237:

    // ~c32[1] << ^str;
    ((libpi::Session*)_this->var___SIM__c32)->Send(0,_task,label_cf6b4c5b94c4f75325166dbaeb4a0aae);

    _task->SetLabel(&&__SIM__checkpoint_239);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_239:

    // ~c32[1] << "What is n? ";
    { 
      ((libpi::Session*)_this->var___SIM__c32)->Send(0,_task,stringval_1390f34ad3ce7e5037f7845236b1d188);
      _task->GetWorker().GCMark(stringval_1390f34ad3ce7e5037f7845236b1d188);
    }

    _task->SetLabel(&&__SIM__checkpoint_241);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_241:

    // ~c32[1] << ^input;
    ((libpi::Session*)_this->var___SIM__c32)->Send(0,_task,label_8e3aa78e57ec3533858349d5f34c05ab);

    _task->SetLabel(&&__SIM__checkpoint_243);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_243:

    // ~c32[1] << ^int;
    ((libpi::Session*)_this->var___SIM__c32)->Send(0,_task,label_64f10e23bd5ddafc2b83ba3a50254359);

    _task->SetLabel(&&__SIM__checkpoint_244);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_244:

    { _task->tmp=new libpi::task::Channel(&_task->GetWorker());
      ((libpi::task::Link*)(_this->var___SIM__fib16))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__FlatRight6245);
      if (!((libpi::task::Channel*)_task->tmp)->Receive(_task,_this->var___SIM__f33))
        return false;
      __SIM__receive___SIM__FlatRight6245:
      // Do not delete _task->tmp as it is reused in session!
      _task->tmp=NULL;
    }

    _task->SetLabel(&&__SIM__checkpoint_247);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_247:

    // ~c32[1] >> ~v34;
    _task->SetLabel(&&__SIM__receive246);
    if (!((libpi::Session*)_this->var___SIM__c32)->Receive(0,_task,_this->var___SIM__v34))
      return false;
    __SIM__receive246:

    _task->SetLabel(&&__SIM__checkpoint_249);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_249:

    // ~f33[1] << ~v34;
    { 
      ((libpi::Session*)_this->var___SIM__f33)->Send(0,_task,_this->var___SIM__v34);
      _task->GetWorker().GCMark(_this->var___SIM__v34);
    }

    _task->SetLabel(&&__SIM__checkpoint_251);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_251:

    // ~c32[1] << ^str;
    ((libpi::Session*)_this->var___SIM__c32)->Send(0,_task,label_cf6b4c5b94c4f75325166dbaeb4a0aae);

    _task->SetLabel(&&__SIM__checkpoint_253);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_253:

    // ~c32[1] << "Fib(";
    { 
      ((libpi::Session*)_this->var___SIM__c32)->Send(0,_task,stringval_7a454c9c018e5406a652176541eeb989);
      _task->GetWorker().GCMark(stringval_7a454c9c018e5406a652176541eeb989);
    }

    _task->SetLabel(&&__SIM__checkpoint_255);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_255:

    // ~c32[1] << ^int;
    ((libpi::Session*)_this->var___SIM__c32)->Send(0,_task,label_64f10e23bd5ddafc2b83ba3a50254359);

    _task->SetLabel(&&__SIM__checkpoint_257);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_257:

    // ~c32[1] << ~v34;
    { 
      ((libpi::Session*)_this->var___SIM__c32)->Send(0,_task,_this->var___SIM__v34);
      _task->GetWorker().GCMark(_this->var___SIM__v34);
    }

    _task->SetLabel(&&__SIM__checkpoint_259);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_259:

    // ~c32[1] << ^str;
    ((libpi::Session*)_this->var___SIM__c32)->Send(0,_task,label_cf6b4c5b94c4f75325166dbaeb4a0aae);

    _task->SetLabel(&&__SIM__checkpoint_261);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_261:

    // ~c32[1] << ")=";
    { 
      ((libpi::Session*)_this->var___SIM__c32)->Send(0,_task,stringval_0d8554966afe372ebbc81f405ba2a1da);
      _task->GetWorker().GCMark(stringval_0d8554966afe372ebbc81f405ba2a1da);
    }

    _task->SetLabel(&&__SIM__checkpoint_263);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_263:

    // ~c32[1] << ^int;
    ((libpi::Session*)_this->var___SIM__c32)->Send(0,_task,label_64f10e23bd5ddafc2b83ba3a50254359);

    _task->SetLabel(&&__SIM__checkpoint_265);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_265:

    // ~f33[1] >> ~r35;
    _task->SetLabel(&&__SIM__receive264);
    if (!((libpi::Session*)_this->var___SIM__f33)->Receive(0,_task,_this->var___SIM__r35))
      return false;
    __SIM__receive264:
    // Session complete
    ((libpi::Session*)_this->var___SIM__f33)->Close(true);
    _this->var___SIM__f33=NULL;

    _task->SetLabel(&&__SIM__checkpoint_267);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_267:

    // ~c32[1] << ~r35;
    { 
      ((libpi::Session*)_this->var___SIM__c32)->Send(0,_task,_this->var___SIM__r35);
      _task->GetWorker().GCMark(_this->var___SIM__r35);
    }

    _task->SetLabel(&&__SIM__checkpoint_269);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_269:

    // ~c32[1] << ^nl;
    ((libpi::Session*)_this->var___SIM__c32)->Send(0,_task,label_7a11449475519daf7b8b8621e5b6eacc);

    _task->SetLabel(&&__SIM__checkpoint_271);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_271:

    // ~c32[1] << ^end;
    ((libpi::Session*)_this->var___SIM__c32)->Send(0,_task,label_186e35ef3dd9939615c442c30b801a33);
    ((libpi::Session*)_this->var___SIM__c32)->Close(true);
    _this->var___SIM__c32=NULL;
    return false;

  } // }}}
  method___SIM__FlatRight7: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight7*)_task)
  {

    _task->SetLabel(&&__SIM__checkpoint_310);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_310:

    _this->var___SIM__fib16=new libpi::task::Link(2,&_task->GetWorker());

    _task->SetLabel(&&__SIM__checkpoint_311);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_311:

    { Task___SIM__FlatRight6 *__SIM__task312(new Task___SIM__FlatRight6(&_task->GetWorker()));
      __SIM__task312->SetArg0(_this->var___SIM__console3);
      __SIM__task312->SetArg1(_this->var___SIM__fib16);
      __SIM__task312->SetLabel(&&method___SIM__FlatRight6);
      _task->GetWorker().AddTask(__SIM__task312);
    }

    _task->SetLabel(&&__SIM__checkpoint_315);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_315:

    { Task___SIM__StartService4 *__SIM__task313 = new Task___SIM__StartService4(&_this->GetWorker());
      libpi::Int *__SIM__systemexp314(new libpi::Int(libpi::task::Worker::TargetTasks,&_task->GetWorker()));
      __SIM__task313->SetArg0(__SIM__systemexp314);
      __SIM__task313->SetArg1(_this->var___SIM__fib16);
      __SIM__task313->SetLabel(&&method___SIM__StartService4);
      _task=__SIM__task313;
    }
    goto method___SIM__StartService4;
  } // }}}
} // }}}
void libpi::task::Worker::Work() // {{{
{ try
  { while (true) // Continue until termination
    { // Find next task
      while (true)
      { if (myGCFlag) // Produce Mark results for Mark-Sweep
        { myGCFlag=false;
          //cout << "Worker " << this << ": Marking..." << endl;
          myGCValues=myGCNewValues;
          myGCMarks.clear();
          for (std::list<Task*>::iterator task=myActiveTasks.begin(); task!=myActiveTasks.end(); ++task)
            (*task)->Mark(myGCMarks);
          myGCReady=true; // GC results are ready
          //cout << "Worker " << this << ": Marked" << endl;
          myGCNewValues.clear();
        }
        if (!myActiveTasks.empty())
          break;
        ourIdleWorkersLock.Lock();
        ourIdleWorkers.push_back(this);
        ++ourIdleWorkersSize;
        ourIdleWorkersLock.Release();
        //cout << "Worker " << this << ": Idle" << endl;
        myWaitLock.Lock();
        //cout << "Worker " << this << ": Activated" << endl;
      }
      Task *task=myActiveTasks.front();
      myActiveTasks.pop_front();

      resume_task:
      if (!task)
      { //cout << "Worker " << this << ": Terminating" << endl;
        break;
      }
      if (_methods(task))
      { if (myActiveTasks.empty())
          goto resume_task;
        else QueueTask(task);
      }
      else
        --ActiveTasks;
    }
  } catch (const string &error)
  { cerr << "Error: " << error << endl;
  }
  return;
} // }}}
void *_start_worker(void *arg) // {{{
{ libpi::task::Worker *worker=(libpi::task::Worker*)arg;
  worker->Work();
} // }}}
inline void _start_workers() // {{{
{ 
  pthread_t x[libpi::task::Worker::Workers];
  pthread_attr_t y;
  pthread_attr_init(&y);
  pthread_attr_setstacksize(&y,16384);
  //pthread_attr_setdetachstate(&y,PTHREAD_CREATE_DETACHED);
  vector<libpi::task::Worker*> workers;
  for (size_t wc=0; wc<libpi::task::Worker::Workers; ++wc)
  { libpi::task::Worker *_worker=new libpi::task::Worker();
    if (wc==0)
    { // Create main task
      libpi::task::Task *_main(new TaskMain(_worker));
      _main->SetLabel(NULL);
      _worker->AddTask(_main);
    }
    // Bind thread to specific core
    //cpu_set_t cpuset;
    //CPU_ZERO(&cpuset);
    //CPU_SET(wc%cores, &cpuset);
    //pthread_setaffinity_np(x[wc],sizeof(cpu_set_t), &cpuset);
    pthread_create(&(x[wc]),&y,_start_worker,(void*)_worker);
    workers.push_back(_worker);
  }
  bool done=false;
  set<libpi::Value*> values;
  while (!done)
  { usleep(100000);
    //cout << "GC: Running..." << endl;
    set<libpi::task::Worker*> remaining;
    for (vector<libpi::task::Worker*>::iterator w=workers.begin(); w!=workers.end(); ++w)
      remaining.insert(*w);
    for (vector<libpi::task::Worker*>::iterator w=workers.begin(); w!=workers.end(); ++w)
      (*w)->GCTask();
    //cout << "GC: Workers Tasked" << endl;
    while(!remaining.empty()) // Continue until all marks are in
    { //libpi::task::Worker::GetIdleWorkersLock().Lock();
      //for (auto idleWorker=libpi::task::Worker::GetIdleWorkers().begin(); idleWorker!=libpi::task::Worker::GetIdleWorkers().end(); ++idleWorker)
      //{ 
        //(*idleWorker->GCUntask();
      //  remaining.erase(*idleWorker); 
        //while (idleWorker!=libpi::task::Worker::GetIdleWorkers().end() && !(*idleWorker)->GCReady())
        //{ cout << "GC: Activating " << *idleWorker << endl;
        //  (*idleWorker)->Activate();
        //  idleWorker=libpi::task::Worker::GetIdleWorkers().erase(idleWorker);
        //  --libpi::task::Worker::GetIdleWorkersSize();
        //}
      //}
      //libpi::task::Worker::GetIdleWorkersLock().Release();
      usleep(10000);
      //cout << "GC: Checking Workers Status" << endl;
      libpi::task::Worker::GetIdleWorkersLock().Lock();
      set<libpi::task::Worker*>::iterator wit=remaining.begin();
      while(wit!=remaining.end())
      { if ((*wit)->GCReady())
        { //cout << "GC: Worker " << *wit << "ready" << endl;
          wit=remaining.erase(wit);
        }
        else if (std::find(libpi::task::Worker::GetIdleWorkers().begin(),libpi::task::Worker::GetIdleWorkers().end(),*wit)!=libpi::task::Worker::GetIdleWorkers().end())
        { //cout << "GC: Worker " << *wit << "idle" << endl;
          wit=remaining.erase(wit);
          //(*wit)->GCUntask();
        }
        else
        { //cout << "GC: Worker " << *wit << " not ready" << endl;
          ++wit;
        }
      }
      libpi::task::Worker::GetIdleWorkersLock().Release();
      //cout << "GC: " << remaining.size() << " workers remaining" << endl;
    }
    set<libpi::Value*> marks;
    for (vector<libpi::task::Worker*>::iterator w=workers.begin(); w!=workers.end(); ++w)
    { values.insert((*w)->GCValues().begin(),(*w)->GCValues().end());
      marks.insert((*w)->GCMarks().begin(),(*w)->GCMarks().end());
    }
    set<libpi::Value*> garbage=values;
    for (set<libpi::Value*>::const_iterator mark=marks.begin(); mark!=marks.end(); ++mark)
      garbage.erase(*mark);
    for (set<libpi::Value*>::const_iterator junk=garbage.begin(); junk!=garbage.end(); ++junk)
      values.erase(*junk);
    for (set<libpi::Value*>::iterator val=garbage.begin(); val!=garbage.end(); ++val)
      delete *val;
    // Detect termination
    if (libpi::task::Worker::GetIdleWorkersSize()==libpi::task::Worker::Workers) // Test if program is complete
    { //cout << "GC: Terminating Workers" << endl;
      libpi::task::Worker::GetIdleWorkersLock().Lock();
      while (!libpi::task::Worker::GetIdleWorkers().empty())
      { //cout << "GC: Terminating " << libpi::task::Worker::GetIdleWorkers().front() << endl;
        libpi::task::Worker::GetIdleWorkers().front()->EmployTask(NULL);
        libpi::task::Worker::GetIdleWorkers().pop_front();
      }
      libpi::task::Worker::GetIdleWorkersSize()=0;
      libpi::task::Worker::GetIdleWorkersLock().Release();
      done=true;
    }
  }
  //cout << "GC: Waiting for workers" << endl;
  for (size_t wc=0; wc<libpi::task::Worker::TargetTasks; ++wc)
  { pthread_join(x[wc],NULL);
    delete workers[wc];
  }
  //cout << "GC: Done" << endl;
} // }}}
/*Start process, and its continuations */
int main(int argc, char **argv) // {{{
{ // PARSE ARGS!!
  for (int i=0; i<argc; ++i)
  { if (string(argv[i])=="-pi_tprocs" && i+1<argc)
      libpi::task::Worker::TargetTasks=atoi(argv[++i]);
    if (string(argv[i])=="-pi_workers" && i+1<argc)
      libpi::task::Worker::Workers=atoi(argv[++i]);
    else
      _args.push_back(argv[i]);
  }
  try
  { signal(SIGCHLD, SIG_IGN); // Fork optimization
    // Start workers
    _start_workers();
    //munmap(_aprocs,sizeof(int));
  } catch (const string &error)
  { cerr << "Error: " << error << endl;
    return 1;
  }
  return 0;
} // }}}
