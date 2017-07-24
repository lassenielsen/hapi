#include <iostream>
#include <thread>
#include <cstdlib>
#include <pthread.h>
#include <libpi/value.hpp>
#include <libpi/bool.hpp>
#include <libpi/int.hpp>
#include <libpi/float.hpp>
#include <libpi/quotient.hpp>
#include <libpi/string.hpp>
#include <libpi/tuple.hpp>
#include <libpi/session.hpp>
#include <libpi/thread/channel.hpp>
#include <libpi/thread/link.hpp>
#include <vector>
#include <queue>
#include <sstream>
#include <atomic>
#include <memory>
#include <sys/mman.h>
#include <signal.h>
#include <iostream>
#include <unordered_map>
using namespace std;

// Value declerations
// {{{
shared_ptr<libpi::Int> intval_0(new libpi::Int("0"));
shared_ptr<libpi::String> stringval_d41d8cd98f00b204e9800998ecf8427e(new libpi::String(""));
shared_ptr<libpi::Int> intval_1(new libpi::Int("1"));
shared_ptr<libpi::Int> intval_2(new libpi::Int("2"));
shared_ptr<libpi::String> stringval_3f31902b37cab22976b013dcd26ed920(new libpi::String("What is n?"));
shared_ptr<libpi::String> stringval_7559769f36c2ade263483f982b001498(new libpi::String("Fib(n) is"));
// }}}

// Extensions for libpi
namespace libpi // {{{
{
  typedef std::unordered_map<std::string,std::shared_ptr<Value> > Closure;

  class State : public Value // {{{
  { public:
      State() {}
      virtual ~State();
      virtual std::string GetType() const;
      virtual void ToStream(std::ostream &dest) const;
      //virtual std::shared_ptr<Bool> operator==(const Value &rhs) const;
      //virtual std::shared_ptr<Bool> operator<=(const Value &rhs) const;
      //virtual std::shared_ptr<Bool> operator<(const Value &rhs) const;
      //virtual std::shared_ptr<Bool> operator>=(const Value &rhs) const;
      //virtual std::shared_ptr<Bool> operator>(const Value &rhs) const;
  
      void *GetLabel() {return myLabel;}
      void SetLabel(void *label) {myLabel=label;}
      Closure &Values() {return myValues;}
      size_t &GetSteps() {return mySteps;}

    private:
      void *myLabel;
      Closure myValues;
      std::string dstName; // Used when waiting to receive a value
      size_t mySteps;
  }; // }}}

  State::~State() // {{{
  {
  } // }}}

  void State::ToStream(std::ostream &dest) const // {{{
  { throw std::string("State::ToStream not implemented");
  } // }}}

  std::string State::GetType() const // {{{
  { return "sta";
  } // }}}
} // }}}

// Threads state
inline atomic<int> *_new_shared_int() // {{{
{ return (std::atomic<int>*)mmap(NULL, sizeof(atomic<int>), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); // Actual number of active processes
} // }}}
std::atomic<int> *_aprocs=_new_shared_int(); // Actual number of active processes
int _tprocs=std::thread::hardware_concurrency(); // Target number of active processes
std::vector<char*> _args;   // Store args for use in processes
atomic<unordered_map<Channel*,libpi::thread::Channel > > _tasks;

// Threads functionality
inline void _inc_aprocs() // {{{
{ ++(*_aprocs);
} // }}}
inline void _dec_aprocs() // {{{
{ --(*_aprocs);
} // }}}
void *_methods(void *arg); // Declare implementation
void *_workerthread(void *_arg) // {{{
{ while (true) // Continue until termination
  { // Wait for task
    shared_pointer<libpi::State> task=dynamic_pointer_cast<libpi::State>(_tasks[NULL].Receive());
    if (task.get()==NULL)
      break;
    _methods(task.get());
    if (task->GetLabel()!=NULL)
      _tasks[NULL].Send(task);
  }
} // }}}
inline void _workers() // {{{
{ pthread_t x;
  pthread_attr_t y;
  pthread_attr_init(&y);
  pthread_attr_setstacksize(&y,16384);
  pthread_attr_setdetachstate(&y,PTHREAD_CREATE_DETACHED);
  for (size_t wc=0; wc+1<_pthreads; ++wc)
    pthread_create(&x,&y,_workerthread,NULL);
  _workerthread(NULL); // FIXME: Start and monitor in stead
} // }}}
inline void _spawn_thread(libpi::State *state) // {{{
{ pthread_t x;
  pthread_attr_t y;
  pthread_attr_init(&y);
  pthread_attr_setstacksize(&y,16384);
  pthread_attr_setdetachstate(&y,PTHREAD_CREATE_DETACHED);
  pthread_create(&x,&y,_methods,(void*)state);
} // }}}
inline void _spawn_task(shared_pointer<libpi::State> state) // {{{
{ _tasks[NULL].Send(state);
} // }}}


/* All methods */
#define _state ((State*)(_arg))
void *_methods(void *_arg) // {{{
{
  if (_state==NULL) // Error
  { std::cerr << "Error in call to all_methods: null state provided" << endl;
    return NULL;
  }  
  if (_state->GetLabel()!=NULL)
    goto *_state->GetLabel();
  { // Main
    _state->Values()["__SIM_console3"]=shared_ptr<thread::Link>(new thread::Link(2));
    _inc_aprocs();
    { State *__SIM__state146 = new State();
      __SIM__state146->Values()["__SIM_console3"]=static_pointer_cast<libpi::Value>(_state->Values()["__SIM__console3"]);
      __SIM__state146->SetLabel(&&method___SIM__FlatRight7);
      _spawn_task(shared_ptr<libpi::State>(__SIM__state146));
    }
    { // Left process
    _state->values.clear();
    _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__console3));
    _state->label=&&method___SIM__Console1;
    goto *_state->label;
    }
  }

/* Procedure implementations */
  method___SIM__ConsoleSession2: // {{{
  {
    shared_ptr<libpi::thread::Link > __SIM__console3 = static_pointer_cast<libpi::thread::Link>(_state->values.back());
    _state->values.pop_back();
    shared_ptr<libpi::Session > __SIM__s5 = static_pointer_cast<libpi::Session>(_state->values.back());
    _state->values.pop_back();
  { _dec_aprocs();
    shared_ptr<libpi::String> __SIM__branch99 = static_pointer_cast<libpi::String>(__SIM__s5->Receive(1));
    _inc_aprocs();
    if (__SIM__branch99->GetValue()=="^End")
    {
      __SIM__s5->Close(true);
      __SIM__s5 = NULL;
  _state->values.clear();
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__console3));
  _state->label=&&method___SIM__Console1;
  goto *_state->label;
    }
    else 
    if (__SIM__branch99->GetValue()=="^GetInt")
    {
shared_ptr<libpi::Int > __SIM__val10(intval_0);
{ long _l; std::cin >> _l;  __SIM__val10 =shared_ptr<libpi::Int>(new libpi::Int(_l)); }  { 
    __SIM__s5->Send(1,static_pointer_cast<libpi::Value>(__SIM__val10));
  }
  _state->values.clear();
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__s5));
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__console3));
  _state->label=&&method___SIM__ConsoleSession2;
  goto *_state->label;
    }
    else 
    if (__SIM__branch99->GetValue()=="^GetString")
    {
shared_ptr<libpi::String > __SIM__val11(stringval_d41d8cd98f00b204e9800998ecf8427e);
{ std::string _s; std::cin >> _s;  __SIM__val11 =shared_ptr<libpi::String>(new libpi::String(_s)); }  { 
    __SIM__s5->Send(1,static_pointer_cast<libpi::Value>(__SIM__val11));
  }
  _state->values.clear();
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__s5));
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__console3));
  _state->label=&&method___SIM__ConsoleSession2;
  goto *_state->label;
    }
    else 
    if (__SIM__branch99->GetValue()=="^PutInt")
    {
  shared_ptr<libpi::Int > __SIM__i12;
  _dec_aprocs();
  __SIM__i12 = static_pointer_cast<libpi::Int >(__SIM__s5->Receive(1));
  _inc_aprocs();
std::cout << ( __SIM__i12 )->ToString() << std::endl;  _state->values.clear();
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__s5));
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__console3));
  _state->label=&&method___SIM__ConsoleSession2;
  goto *_state->label;
    }
    else 
    if (__SIM__branch99->GetValue()=="^PutString")
    {
  shared_ptr<libpi::String > __SIM__str13;
  _dec_aprocs();
  __SIM__str13 = static_pointer_cast<libpi::String >(__SIM__s5->Receive(1));
  _inc_aprocs();
std::cout << ( __SIM__str13 )->GetValue() << std::endl;  _state->values.clear();
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__s5));
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__console3));
  _state->label=&&method___SIM__ConsoleSession2;
  goto *_state->label;
    }
    else 
    if (__SIM__branch99->GetValue()=="^Stop")
    {
      __SIM__s5->Close(true);
      __SIM__s5 = NULL;
  _dec_aprocs();
  delete _state;
  return NULL;
    }
    else throw (string)"Unknown branch: __SIM__branch99";
  }
  } // }}}
  method___SIM__Console1: // {{{
  {
    shared_ptr<libpi::thread::Link > __SIM__console3 = static_pointer_cast<libpi::thread::Link>(_state->values.back());
    _state->values.pop_back();
  _dec_aprocs();
  shared_ptr<Session> __SIM__s4 = __SIM__console3->Connect(0, 2);
  _inc_aprocs();
  _state->values.clear();
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__s4));
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__console3));
  _state->label=&&method___SIM__ConsoleSession2;
  goto *_state->label;
  } // }}}
  method___SIM__FlatRight5: // {{{
  {
    shared_ptr<libpi::Session > __SIM__s15 = static_pointer_cast<libpi::Session>(_state->values.back());
    _state->values.pop_back();
    shared_ptr<libpi::thread::Link > __SIM__fib14 = static_pointer_cast<libpi::thread::Link>(_state->values.back());
    _state->values.pop_back();
  shared_ptr<libpi::Int > __SIM__x16;
  _dec_aprocs();
  __SIM__x16 = static_pointer_cast<libpi::Int >(__SIM__s15->Receive(1));
  _inc_aprocs();
    shared_ptr<libpi::Bool> __SIM__binop102((*__SIM__x16) <= (*intval_1));
  if (__SIM__binop102->GetValue())
  {
  { 
    __SIM__s15->Send(1,static_pointer_cast<libpi::Value>(intval_1));
  }
  __SIM__s15->Close(true);
  __SIM__s15=NULL;
  _dec_aprocs();
  delete _state;
  return NULL;
  }
  else
  {
    shared_ptr<libpi::Int> __SIM__sysval104(new libpi::Int(_tprocs));
    shared_ptr<libpi::Int> __SIM__sysval105(new libpi::Int(*_aprocs));
    shared_ptr<libpi::Bool> __SIM__binop103((*__SIM__sysval104) <= (*__SIM__sysval105));
  if (__SIM__binop103->GetValue())
  {
  _dec_aprocs();
  shared_ptr<Session> __SIM__s117 = __SIM__fib14->Connect(1, 2);
  _inc_aprocs();
  { 
    shared_ptr<libpi::Int> __SIM__binop113((*__SIM__x16) - (*intval_1));
    __SIM__s117->Send(0,static_pointer_cast<libpi::Value>(__SIM__binop113));
  }
  shared_ptr<libpi::Int > __SIM__f118;
  _dec_aprocs();
  __SIM__f118 = static_pointer_cast<libpi::Int >(__SIM__s117->Receive(0));
  _inc_aprocs();
  __SIM__s117->Close(true);
  __SIM__s117=NULL;
  _dec_aprocs();
  shared_ptr<Session> __SIM__s219 = __SIM__fib14->Connect(1, 2);
  _inc_aprocs();
  { 
    shared_ptr<libpi::Int> __SIM__binop115((*__SIM__x16) - (*intval_2));
    __SIM__s219->Send(0,static_pointer_cast<libpi::Value>(__SIM__binop115));
  }
  shared_ptr<libpi::Int > __SIM__f220;
  _dec_aprocs();
  __SIM__f220 = static_pointer_cast<libpi::Int >(__SIM__s219->Receive(0));
  _inc_aprocs();
  __SIM__s219->Close(true);
  __SIM__s219=NULL;
  { 
    shared_ptr<libpi::Int> __SIM__binop117((*__SIM__f118) + (*__SIM__f220));
    __SIM__s15->Send(1,static_pointer_cast<libpi::Value>(__SIM__binop117));
  }
  __SIM__s15->Close(true);
  __SIM__s15=NULL;
  _dec_aprocs();
  delete _state;
  return NULL;
  }
  else
  {
  _dec_aprocs();
  shared_ptr<Session> __SIM__s121 = __SIM__fib14->Connect(1, 2);
  _inc_aprocs();
  { 
    shared_ptr<libpi::Int> __SIM__binop107((*__SIM__x16) - (*intval_1));
    __SIM__s121->Send(0,static_pointer_cast<libpi::Value>(__SIM__binop107));
  }
  _dec_aprocs();
  shared_ptr<Session> __SIM__s222 = __SIM__fib14->Connect(1, 2);
  _inc_aprocs();
  { 
    shared_ptr<libpi::Int> __SIM__binop109((*__SIM__x16) - (*intval_2));
    __SIM__s222->Send(0,static_pointer_cast<libpi::Value>(__SIM__binop109));
  }
  shared_ptr<libpi::Int > __SIM__f123;
  _dec_aprocs();
  __SIM__f123 = static_pointer_cast<libpi::Int >(__SIM__s121->Receive(0));
  _inc_aprocs();
  __SIM__s121->Close(true);
  __SIM__s121=NULL;
  shared_ptr<libpi::Int > __SIM__f224;
  _dec_aprocs();
  __SIM__f224 = static_pointer_cast<libpi::Int >(__SIM__s222->Receive(0));
  _inc_aprocs();
  __SIM__s222->Close(true);
  __SIM__s222=NULL;
  { 
    shared_ptr<libpi::Int> __SIM__binop111((*__SIM__f123) + (*__SIM__f224));
    __SIM__s15->Send(1,static_pointer_cast<libpi::Value>(__SIM__binop111));
  }
  __SIM__s15->Close(true);
  __SIM__s15=NULL;
  _dec_aprocs();
  delete _state;
  return NULL;
  }
  }
  } // }}}
  method___SIM__Fib3: // {{{
  {
    shared_ptr<libpi::thread::Link > __SIM__fib14 = static_pointer_cast<libpi::thread::Link>(_state->values.back());
    _state->values.pop_back();
  _dec_aprocs();
  shared_ptr<Session> __SIM__s15 = __SIM__fib14->Connect(0, 2);
  _inc_aprocs();
  _inc_aprocs();
  { State *__SIM__state119 = new State();
  __SIM__state119->values.push_back( static_pointer_cast<libpi::Value>(__SIM__fib14));
  __SIM__state119->values.push_back( static_pointer_cast<libpi::Value>(__SIM__s15));
  __SIM__state119->label=&&method___SIM__FlatRight5;
    _spawn_thread(__SIM__state119);
  }
  { // Left process
  _state->values.clear();
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__fib14));
  _state->label=&&method___SIM__Fib3;
  goto *_state->label;
  }
  } // }}}
  method___SIM__StartFib4: // {{{
  {
    shared_ptr<libpi::thread::Link > __SIM__fib14 = static_pointer_cast<libpi::thread::Link>(_state->values.back());
    _state->values.pop_back();
    shared_ptr<libpi::Int > __SIM__i25 = static_pointer_cast<libpi::Int>(_state->values.back());
    _state->values.pop_back();
    shared_ptr<libpi::Bool> __SIM__binop120((*__SIM__i25) <= (*intval_0));
  if (__SIM__binop120->GetValue())
  {
  _state->values.clear();
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__fib14));
  _state->label=&&method___SIM__Fib3;
  goto *_state->label;
  }
  else
  {
  _inc_aprocs();
  { State *__SIM__state121 = new State();
    shared_ptr<libpi::Int> __SIM__binop122((*__SIM__i25) - (*intval_1));
  __SIM__state121->values.push_back( static_pointer_cast<libpi::Value>(__SIM__binop122));
  __SIM__state121->values.push_back( static_pointer_cast<libpi::Value>(__SIM__fib14));
  __SIM__state121->label=&&method___SIM__StartFib4;
    _spawn_thread(__SIM__state121);
  }
  { // Left process
  _state->values.clear();
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__fib14));
  _state->label=&&method___SIM__Fib3;
  goto *_state->label;
  }
  }
  } // }}}
  method___SIM__FlatRight6: // {{{
  {
    shared_ptr<libpi::thread::Link > __SIM__fib14 = static_pointer_cast<libpi::thread::Link>(_state->values.back());
    _state->values.pop_back();
    shared_ptr<libpi::thread::Link > __SIM__console3 = static_pointer_cast<libpi::thread::Link>(_state->values.back());
    _state->values.pop_back();
    shared_ptr<libpi::Int> __SIM__sysval124(new libpi::Int(_tprocs));
    shared_ptr<libpi::Int> __SIM__sysval125(new libpi::Int(*_aprocs));
    shared_ptr<libpi::Bool> __SIM__binop123((*__SIM__sysval124) <= (*__SIM__sysval125));
  if (__SIM__binop123->GetValue())
  {
  _dec_aprocs();
  shared_ptr<Session> __SIM__c26 = __SIM__console3->Connect(1, 2);
  _inc_aprocs();
  shared_ptr<libpi::String> __SIM__select135(new libpi::String("^PutString"));
  __SIM__c26->Send(0,__SIM__select135);
  { 
    __SIM__c26->Send(0,static_pointer_cast<libpi::Value>(stringval_3f31902b37cab22976b013dcd26ed920));
  }
  shared_ptr<libpi::String> __SIM__select137(new libpi::String("^GetInt"));
  __SIM__c26->Send(0,__SIM__select137);
  shared_ptr<libpi::Int > __SIM__v27;
  _dec_aprocs();
  __SIM__v27 = static_pointer_cast<libpi::Int >(__SIM__c26->Receive(0));
  _inc_aprocs();
  _dec_aprocs();
  shared_ptr<Session> __SIM__f28 = __SIM__fib14->Connect(1, 2);
  _inc_aprocs();
  { 
    __SIM__f28->Send(0,static_pointer_cast<libpi::Value>(__SIM__v27));
  }
  shared_ptr<libpi::Int > __SIM__r29;
  _dec_aprocs();
  __SIM__r29 = static_pointer_cast<libpi::Int >(__SIM__f28->Receive(0));
  _inc_aprocs();
  __SIM__f28->Close(true);
  __SIM__f28=NULL;
  shared_ptr<libpi::String> __SIM__select139(new libpi::String("^PutString"));
  __SIM__c26->Send(0,__SIM__select139);
  { 
    __SIM__c26->Send(0,static_pointer_cast<libpi::Value>(stringval_7559769f36c2ade263483f982b001498));
  }
  shared_ptr<libpi::String> __SIM__select141(new libpi::String("^PutInt"));
  __SIM__c26->Send(0,__SIM__select141);
  { 
    __SIM__c26->Send(0,static_pointer_cast<libpi::Value>(__SIM__r29));
  }
  shared_ptr<libpi::String> __SIM__select143(new libpi::String("^Stop"));
  __SIM__c26->Send(0,__SIM__select143);
  __SIM__c26->Close(true);
  __SIM__c26=NULL;
  _dec_aprocs();
  delete _state;
  return NULL;
  }
  else
  {
  _dec_aprocs();
  shared_ptr<Session> __SIM__c30 = __SIM__console3->Connect(1, 2);
  _inc_aprocs();
  shared_ptr<libpi::String> __SIM__select126(new libpi::String("^PutString"));
  __SIM__c30->Send(0,__SIM__select126);
  { 
    __SIM__c30->Send(0,static_pointer_cast<libpi::Value>(stringval_3f31902b37cab22976b013dcd26ed920));
  }
  shared_ptr<libpi::String> __SIM__select128(new libpi::String("^GetInt"));
  __SIM__c30->Send(0,__SIM__select128);
  _dec_aprocs();
  shared_ptr<Session> __SIM__f31 = __SIM__fib14->Connect(1, 2);
  _inc_aprocs();
  shared_ptr<libpi::Int > __SIM__v32;
  _dec_aprocs();
  __SIM__v32 = static_pointer_cast<libpi::Int >(__SIM__c30->Receive(0));
  _inc_aprocs();
  { 
    __SIM__f31->Send(0,static_pointer_cast<libpi::Value>(__SIM__v32));
  }
  shared_ptr<libpi::String> __SIM__select130(new libpi::String("^PutString"));
  __SIM__c30->Send(0,__SIM__select130);
  { 
    __SIM__c30->Send(0,static_pointer_cast<libpi::Value>(stringval_7559769f36c2ade263483f982b001498));
  }
  shared_ptr<libpi::String> __SIM__select132(new libpi::String("^PutInt"));
  __SIM__c30->Send(0,__SIM__select132);
  shared_ptr<libpi::Int > __SIM__r33;
  _dec_aprocs();
  __SIM__r33 = static_pointer_cast<libpi::Int >(__SIM__f31->Receive(0));
  _inc_aprocs();
  __SIM__f31->Close(true);
  __SIM__f31=NULL;
  { 
    __SIM__c30->Send(0,static_pointer_cast<libpi::Value>(__SIM__r33));
  }
  shared_ptr<libpi::String> __SIM__select134(new libpi::String("^Stop"));
  __SIM__c30->Send(0,__SIM__select134);
  __SIM__c30->Close(true);
  __SIM__c30=NULL;
  _dec_aprocs();
  delete _state;
  return NULL;
  }
  } // }}}
  method___SIM__FlatRight7: // {{{
  {
    shared_ptr<libpi::thread::Link > __SIM__console3 = static_pointer_cast<libpi::thread::Link>(_state->values.back());
    _state->values.pop_back();
  shared_ptr<thread::Link> __SIM__fib14(new thread::Link(2));
  _inc_aprocs();
  { State *__SIM__state144 = new State();
  __SIM__state144->values.push_back( static_pointer_cast<libpi::Value>(__SIM__console3));
  __SIM__state144->values.push_back( static_pointer_cast<libpi::Value>(__SIM__fib14));
  __SIM__state144->label=&&method___SIM__FlatRight6;
    _spawn_thread(__SIM__state144);
  }
  { // Left process
  _state->values.clear();
    shared_ptr<libpi::Int> __SIM__sysval145(new libpi::Int(_tprocs));
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__sysval145));
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__fib14));
  _state->label=&&method___SIM__StartFib4;
  goto *_state->label;
  }
  } // }}}
} // }}}


/*Start process, and its continuations */
int main(int argc, char **argv) // {{{
{ // PARSE ARGS!!
  for (int i=0; i<argc; ++i)
  { if (string(argv[i])=="-pi_tprocs" && i+1<argc)
      _tprocs=atoi(argv[++i]);
    else
      _args.push_back(argv[i]);
  }
  (*_aprocs)=1;
  try
  { signal(SIGCHLD, SIG_IGN); // Fork optimization
    // Create main task
    shared_ptr<libpi::State> s0(new State());
    s0->SetLabel(NULL);
    _spawn_task(s0);
    // Start workers
    _workers();
    //munmap(_aprocs,sizeof(int));
  } catch (const string &error)
  { cerr << "Error: " << error << endl;
    return 1;
  }
  return 0;
} // }}}
