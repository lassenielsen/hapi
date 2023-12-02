/* Pre compilation output:
******************* Program *******************
1->2:Bool;
2->1:Bool;
$end; a(1, 2);
( s= new a(1 of 2);
  s[2] << true;
  s[2] >> b;
  
| t= new a(2 of 2);
  t[1] >> c;
  t[1] << not c;
  
)
************ Type Checking Program ************
************ Type Check Succeeded! ************
****** Parallelization Optimized Program ******
1->2:Bool;
2->1:Bool;
$end; a(1, 2);
( if (SYSTEM & "tprocs" <= SYSTEM & "aprocs")
  then s= new a(1 of 2);
       s[2] << true;
       s[2] >> b;
       
  else s= new a(1 of 2);
       s[2] << true;
       s[2] >> b;
       
| t= new a(2 of 2);
  t[1] >> c;
  t[1] << not c;
  
)
*/
/* ==== ORIGINAL ====
1->2:Bool;
2->1:Bool;
$end; a(1, 2);
( if (SYSTEM & "tprocs" <= SYSTEM & "aprocs")
  then s= new a(1 of 2);
       s[2] << true;
       s[2] >> b;
       
  else s= new a(1 of 2);
       s[2] << true;
       s[2] >> b;
       
| t= new a(2 of 2);
  t[1] >> c;
  t[1] << not c;
  
)
*/
/* ==== RENAMED ====
1->2:Bool;
2->1:Bool;
$end; ~a1(1, 2);
( if (SYSTEM & "tprocs" <= SYSTEM & "aprocs")
  then ~s2= new ~a1(1 of 2);
       ~s2[2] << true;
       ~s2[2] >> ~b3;
       
  else ~s4= new ~a1(1 of 2);
       ~s4[2] << true;
       ~s4[2] >> ~b5;
       
| ~t6= new ~a1(2 of 2);
  ~t6[1] >> ~c7;
  ~t6[1] << not ~c7;
  
)
*/
/* ==== FLATTENFORKED ====
1->2:Bool;
2->1:Bool;
$end; ~a1(1, 2);
( if (SYSTEM & "tprocs" <= SYSTEM & "aprocs")
  then ~s2= new ~a1(1 of 2);
       ~s2[2] << true;
       ~s2[2] >> ~b3;
       
  else ~s4= new ~a1(1 of 2);
       ~s4[2] << true;
       ~s4[2] >> ~b5;
       
| local ~FlatRight1()
  ( ~t6= new ~a1(2 of 2);
    ~t6[1] >> ~c7;
    ~t6[1] << not ~c7;
    
  )
  ~FlatRight1();
)
*/
/* ==== CLOSEDEFED ====
1->2:Bool;
2->1:Bool;
$end; ~a1(1, 2);
( if (SYSTEM & "tprocs" <= SYSTEM & "aprocs")
  then ~s2= new ~a1(1 of 2);
       ~s2[2] << true;
       ~s2[2] >> ~b3;
       
  else ~s4= new ~a1(1 of 2);
       ~s4[2] << true;
       ~s4[2] >> ~b5;
       
| local ~FlatRight1(1->2:Bool;
                    2->1:Bool;
                    $end;(1, 2) ~a1)
  ( ~t6= new ~a1(2 of 2);
    ~t6[1] >> ~c7;
    ~t6[1] << not ~c7;
    
  )
  ~FlatRight1(~a1);
)
*/
#include <libpi/value.hpp>
#include <libpi/bool.hpp>
#include <libpi/int.hpp>
#include <libpi/float.hpp>
#include <libpi/string.hpp>
#include <libpi/session.hpp>
#include <libpi/task/link.hpp>
#include <libpi/task/worker.hpp>
#include <thread>
#include <signal.h>
using namespace std;
// HEADERS {{{
// }}}
// Value declerations {{{
std::vector<char*> _args;
// }}}
// Task Types {{{
class TaskMain : public libpi::task::Task
{ public:
    libpi::task::Link* var___SIM__a1;
    bool var___SIM__b3;
    bool var___SIM__b5;
    libpi::Session* var___SIM__s2;
    libpi::Session* var___SIM__s4;
};
class Task___SIM__FlatRight1 : public libpi::task::Task
{ public:
    Task___SIM__FlatRight1()
    { 
      var___SIM__a1=NULL;
      var___SIM__t6=NULL;
    }
    virtual ~Task___SIM__FlatRight1()
    {
      if (var___SIM__a1)
      { var___SIM__a1->RemoveRef();
        var___SIM__a1=NULL;
      }
      if (var___SIM__t6)
      { var___SIM__t6->RemoveRef();
        var___SIM__t6=NULL;
      }
    }
    libpi::task::Link* var___SIM__a1;
    bool var___SIM__c7;
    libpi::Session* var___SIM__t6;
    inline void SetArg0(libpi::task::Link* val) { var___SIM__a1=val; var___SIM__a1->AddRef(); }
};
// }}}
// All Methods {{{
inline bool _methods(libpi::task::Task*& _task)
{ size_t _steps=0;
  void *_label=_task->GetLabel();
  if (_label!=NULL)
    goto *_label;
  method_Main:
  #define _this ((TaskMain*)_task)
  { // Main {{{

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_12);
      return true;
    }
__SIM__checkpoint_12:
    _this->var___SIM__a1=new libpi::task::Link(2);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_13);
      return true;
    }
__SIM__checkpoint_13:
    { Task___SIM__FlatRight1 *__SIM__task14(new Task___SIM__FlatRight1());
      __SIM__task14->SetWorker(&_task->GetWorker());
      libpi::task::Link* arg0(_this->var___SIM__a1);
      __SIM__task14->SetArg0( arg0);
      arg0->RemoveRef();
      arg0=NULL;
      __SIM__task14->SetLabel(&&method___SIM__FlatRight1);
      _task->GetWorker().AddTask(__SIM__task14);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_17);
      return true;
    }
__SIM__checkpoint_17:
    {
      libpi::Int* __SIM__systemexp19(new libpi::Int(libpi::task::Worker::TargetTasks));
      libpi::Int* __SIM__systemexp20(new libpi::Int(libpi::task::Worker::ActiveTasks));
      bool __SIM__binop18((*__SIM__systemexp19) <= (*__SIM__systemexp20));
      if (__SIM__binop18)
        _task->SetLabel(&&__SIM__TaskMain_true15);
      else
        _task->SetLabel(&&__SIM__TaskMain_false16);
      __SIM__systemexp19->RemoveRef();
      __SIM__systemexp19=NULL;
      __SIM__systemexp20->RemoveRef();
      __SIM__systemexp20=NULL;
    }
    goto *_task->GetLabel();
    __SIM__TaskMain_true15:
    // ~s2 = new ~a1(1 of 2)

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_21);
      return true;
    }
__SIM__checkpoint_21:
    { // link
      _task->tmps.clear();
        _task->SetLabel(&&__SIM__receive_TaskMain22);
        if (!_this->var___SIM__a1->GetChannels()[0]->Receive(_task,&_task->tmp))
          return false;
        __SIM__receive_TaskMain22:
        _task->tmps.push_back(_task->tmp);
      vector<vector<libpi::Channel*> > inChannels(2);
      vector<vector<libpi::Channel*> > outChannels(2);
      // Optimize vector inserts
        inChannels[0].resize(2);
        outChannels[0].resize(2);
        inChannels[1].resize(2);
        outChannels[1].resize(2);
      // Create channels
      inChannels[0][0]=new libpi::task::Channel();
      inChannels[0][1]=new libpi::task::Channel();
      inChannels[1][0]=dynamic_cast<libpi::Channel*>(_task->tmps[0]);
      inChannels[1][1]=new libpi::task::Channel();
      outChannels[0][0]=inChannels[0][0];
      outChannels[0][1]=inChannels[1][0];
      outChannels[1][0]=inChannels[0][1];
      outChannels[1][1]=inChannels[1][1];
      // Send sessions
      { libpi::Session* _s(new libpi::Session(1,2, inChannels[1], outChannels[1]));
        ((libpi::Channel*)_task->tmps[0])->Send(_task,_s);
        _s->RemoveRef();
      }
      // Create local session
      _this->var___SIM__s2=new libpi::Session(0,2,inChannels[0],outChannels[0]);
      // Clean up
      while (!inChannels.empty())
      { while (inChannels.back().empty())
        { inChannels.back().back()->RemoveRef();
          inChannels.back().pop_back();
        }
        inChannels.pop_back();
      }
      // outChannels refs inChannels and should not be unreferenced
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_24);
      return true;
    }
__SIM__checkpoint_24:
    // ~s2[2] << true;
    { 
      _this->var___SIM__s2->Send(1,_task,(long int)true);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_26);
      return true;
    }
__SIM__checkpoint_26:
    // ~s2[2] >> ~b3;
    _task->SetLabel(&&__SIM__receive25);
    if (!_this->var___SIM__s2->Receive(1,_task,(long int*)&_this->tmpInt))
      return false;
    __SIM__receive25:
    // Session complete
    _this->var___SIM__s2->Close(true);
    _this->var___SIM__s2->RemoveRef();
    _this->var___SIM__s2=NULL;
    _this->var___SIM__b3=(bool)_this->tmpInt;
    _task->RemoveRef();
    return false;

    __SIM__TaskMain_false16:
    // ~s4 = new ~a1(1 of 2)

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_27);
      return true;
    }
__SIM__checkpoint_27:
    { // link
      _task->tmps.clear();
        _task->SetLabel(&&__SIM__receive_TaskMain28);
        if (!_this->var___SIM__a1->GetChannels()[0]->Receive(_task,&_task->tmp))
          return false;
        __SIM__receive_TaskMain28:
        _task->tmps.push_back(_task->tmp);
      vector<vector<libpi::Channel*> > inChannels(2);
      vector<vector<libpi::Channel*> > outChannels(2);
      // Optimize vector inserts
        inChannels[0].resize(2);
        outChannels[0].resize(2);
        inChannels[1].resize(2);
        outChannels[1].resize(2);
      // Create channels
      inChannels[0][0]=new libpi::task::Channel();
      inChannels[0][1]=new libpi::task::Channel();
      inChannels[1][0]=dynamic_cast<libpi::Channel*>(_task->tmps[0]);
      inChannels[1][1]=new libpi::task::Channel();
      outChannels[0][0]=inChannels[0][0];
      outChannels[0][1]=inChannels[1][0];
      outChannels[1][0]=inChannels[0][1];
      outChannels[1][1]=inChannels[1][1];
      // Send sessions
      { libpi::Session* _s(new libpi::Session(1,2, inChannels[1], outChannels[1]));
        ((libpi::Channel*)_task->tmps[0])->Send(_task,_s);
        _s->RemoveRef();
      }
      // Create local session
      _this->var___SIM__s4=new libpi::Session(0,2,inChannels[0],outChannels[0]);
      // Clean up
      while (!inChannels.empty())
      { while (inChannels.back().empty())
        { inChannels.back().back()->RemoveRef();
          inChannels.back().pop_back();
        }
        inChannels.pop_back();
      }
      // outChannels refs inChannels and should not be unreferenced
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_30);
      return true;
    }
__SIM__checkpoint_30:
    // ~s4[2] << true;
    { 
      _this->var___SIM__s4->Send(1,_task,(long int)true);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_32);
      return true;
    }
__SIM__checkpoint_32:
    // ~s4[2] >> ~b5;
    _task->SetLabel(&&__SIM__receive31);
    if (!_this->var___SIM__s4->Receive(1,_task,(long int*)&_this->tmpInt))
      return false;
    __SIM__receive31:
    // Session complete
    _this->var___SIM__s4->Close(true);
    _this->var___SIM__s4->RemoveRef();
    _this->var___SIM__s4=NULL;
    _this->var___SIM__b5=(bool)_this->tmpInt;
    _task->RemoveRef();
    return false;

  } // }}}

/* Procedure implementations */
  method___SIM__FlatRight1: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight1*)_task)
  {
    // ~t6 = new ~a1(2 of 2)

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_33);
      return true;
    }
__SIM__checkpoint_33:
    { _task->tmp=new libpi::task::Channel();
      _this->var___SIM__a1->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__FlatRight134);
      if (!((libpi::task::Channel*)_task->tmp)->Receive(_task,(libpi::Value**)&_this->var___SIM__t6))
        return false;
      __SIM__receive___SIM__FlatRight134:
      _task->tmp->RemoveRef();
      _task->tmp=NULL;
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_36);
      return true;
    }
__SIM__checkpoint_36:
    // ~t6[1] >> ~c7;
    _task->SetLabel(&&__SIM__receive35);
    if (!_this->var___SIM__t6->Receive(0,_task,(long int*)&_this->tmpInt))
      return false;
    __SIM__receive35:
    _this->var___SIM__c7=(bool)_this->tmpInt;

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_38);
      return true;
    }
__SIM__checkpoint_38:
    // ~t6[1] << not ~c7;
    { 
      bool __SIM__unop39= !_this->var___SIM__c7;
      _this->var___SIM__t6->Send(0,_task,(long int)__SIM__unop39);
    }
  _this->var___SIM__t6->Close(true);
  _this->var___SIM__t6->RemoveRef();
  _this->var___SIM__t6=NULL;
    _task->RemoveRef();
    return false;

  } // }}}
} // }}}
void libpi::task::Worker_Pool::Work() // {{{
{ try
  { while (true) // Continue until termination
    { // Find next task
      while (myActiveTasks.empty())
      { ourIdleWorkersLock.Lock();
        // Test if program is complete
        if (ourIdleWorkersSize==libpi::task::Worker::TargetTasks-1)
        { Task *nullTask=NULL;
          while (!ourIdleWorkers.empty())
          { ourIdleWorkers.front()->EmployTask(nullTask);
            ourIdleWorkers.pop();
          }
          myActiveTasks.push(nullTask);
          ourIdleWorkersSize=0;
          ourIdleWorkersLock.Release();
        }
        else
        { ourIdleWorkers.push(this);
          ++ourIdleWorkersSize;
          ourIdleWorkersLock.Release();
          myWaitLock.Lock();
        }
      }
      Task* task=myActiveTasks.front();
      myActiveTasks.pop();

      resume_task:
      if (!task)
        break;
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
  delete worker;
  return NULL;
} // }}}
inline void _start_workers() // {{{
{ pthread_t x[libpi::task::Worker::TargetTasks];
  pthread_attr_t y;
  pthread_attr_init(&y);
  pthread_attr_setstacksize(&y,16384);
  //pthread_attr_setdetachstate(&y,PTHREAD_CREATE_DETACHED);
  for (size_t wc=0; wc<libpi::task::Worker::TargetTasks; ++wc)
  { libpi::task::Worker *_worker=new libpi::task::Worker_Pool();
    if (wc==0)
    { // Create main task
      libpi::task::Task* _main(new TaskMain());
      _main->SetLabel(NULL);
      _main->SetWorker(_worker);
      _worker->AddTask(_main);
    }
    // Bind thread to specific core
    //cpu_set_t cpuset;
    //CPU_ZERO(&cpuset);
    //CPU_SET(wc%cores, &cpuset);
    //pthread_setaffinity_np(x[wc],sizeof(cpu_set_t), &cpuset);
    pthread_create(&(x[wc]),&y,_start_worker,(void*)_worker);
  }
  for (size_t wc=0; wc<libpi::task::Worker::TargetTasks; ++wc)
    pthread_join(x[wc],NULL);
} // }}}
/*Start process, and its continuations */
int main(int argc, char **argv) // {{{
{ // PARSE ARGS!!
  for (int i=0; i<argc; ++i)
  { if (string(argv[i])=="-pi_tprocs" && i+1<argc)
    libpi::task::Worker::TargetTasks=atoi(argv[++i]);
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
