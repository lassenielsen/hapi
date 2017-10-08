#include <libpi/value.hpp>
#include <libpi/bool.hpp>
#include <libpi/int.hpp>
#include <libpi/string.hpp>
#include <libpi/session.hpp>
#include <libpi/task/link.hpp>
#include <libpi/thread/channel.hpp>
#include <thread>
#include <signal.h>
using namespace std;
// Value declerations
// {{{
shared_ptr<libpi::Int> intval_0(new libpi::Int("0"));
shared_ptr<libpi::Int> intval_1(new libpi::Int("1"));
shared_ptr<libpi::Int> intval_2(new libpi::Int("2"));
shared_ptr<libpi::Int> intval_5(new libpi::Int("5"));
shared_ptr<libpi::Int> intval_10(new libpi::Int("10"));
shared_ptr<libpi::Int> intval_20(new libpi::Int("20"));
shared_ptr<libpi::Int> intval_25(new libpi::Int("25"));
// }}}
std::vector<char*> _args;                        // Store args for use in processes

/* Task types (call stack types) */
class Task_Main : public libpi::task::Task // {{{
{ public:
    shared_ptr<libpi::Value> var_fib;
    shared_ptr<libpi::Value> var_tmp;
    shared_ptr<libpi::Valus> var_s;
}; // }}}
class Task_FibService : public libpi::task::Task // {{{
{ public:
    shared_ptr<libpi::Value> var_fib;
    shared_ptr<libpi::Value> var_tmp;
    shared_ptr<libpi::Value> var_s;
}; // }}}
class Task_Fib : public libpi::task::Task // {{{
{ public:
    shared_ptr<libpi::Value> var_fib;
    shared_ptr<libpi::Value> var_s;
    shared_ptr<libpi::Value> var_n;
    shared_ptr<libpi::Value> var_tmp;
    shared_ptr<libpi::Value> var_s1;
    shared_ptr<libpi::Value> var_f1;
    shared_ptr<libpi::Value> var_s2;
    shared_ptr<libpi::Value> var_f2;
}; // }}}
/* All methods */
inline void *_methods(shared_ptr<libpi::task::Task> &_state) // {{{
{
  void *_label=_state->GetLabel();
  if (_label!=NULL)
    goto *_label;
  { // Main {{{
    ((Task_Main*)_state.get())->var_fib.reset(new libpi::task::Link(2)); // Idx 0=fib
    for (size_t x=0; x<libpi::task::Task::TargetTasks; ++x)
    { libpi::task::Task *newstate = new libpi::task::Task();
      newstate->Values().push_back(_state->Values()[0]); // Idx 0=fib
      newstate->SetLabel(&&method_FibService);
      ++(*libpi::task::Task::ActiveTasks);
      libpi::task::Task::Tasks.Send(shared_ptr<libpi::task::Task>(newstate));
    }
    { // Left process
      // s = new fib(2 of 2)
      _state->Values()[1]=shared_ptr<libpi::Value>(new libpi::task::Channel()); // Idx 1=c
      _state->SetLabel(&&checkpoint_main_100);
      ((libpi::task::Link*)_state->Values()[0].get())->GetChannels()[0]->Send(_state->Values()[1]);
      checkpoint_main_100:
      _state->SetLabel(&&checkpoint_main_101);
      ((libpi::Channel*)_state->Values()[1].get())->Receive(_state,2); // Idx2=s
      checkpoint_main_101:
      _state->Values()[1].reset(); // Erase c, now 0=fib, 2=s
      // s[1] << 5
      _state->SetLabel(&&checkpoint_main_1);
      ((libpi::Session*)_state->Values()[2].get())->Send(0,intval_25);
      checkpoint_main_1:
      // s[1] >> f1
      _state->SetLabel(&&checkpoint_main_10);
      _state->Values().push_back(NULL); // Idx 2=f
      ((libpi::Session*)_state->Values()[2].get())->Receive(0,_state,2);
      checkpoint_main_10:
      // Print result
      cout << _state->Values()[2]->ToString() << endl;
      exit(0);
      throw libpi::task::TaskPauseEvent();
    }
  } // }}}

/* Procedure implementations */
  method_FibService: // {{{
  { _state->Values().resize(3);
    // Wait for connection
    // s = new fib(1 of 2);
    _state->SetLabel(&&checkpoint_FibService_100);
    ((libpi::task::Link*)_state->Values()[0].get())->GetChannels()[0]->Receive(_state,1); // Idx 1=c
    checkpoint_FibService_100:
    // Create both sides of session
    { vector<shared_ptr<libpi::Channel> > p0InChannels(2), p0OutChannels(2), p1InChannels(2), p1OutChannels(2);
      p0InChannels[0].reset(new libpi::task::Channel());
      p0InChannels[1].reset(new libpi::task::Channel());
      p0OutChannels[0]=p0InChannels[0];
      p0OutChannels[1]=dynamic_pointer_cast<libpi::Channel>(_state->Values()[1]);
      p1InChannels[0]=p0OutChannels[1];
      p1InChannels[1].reset(new libpi::task::Channel());
      p1OutChannels[0]=p0InChannels[1];
      p1OutChannels[1]=p1InChannels[1];
      _state->Values()[2].reset(new libpi::Session(0,2,p0InChannels,p0OutChannels)); // Idx 2=s
      shared_ptr<libpi::Value> s2(new libpi::Session(1,2,p1InChannels,p1OutChannels));
      _state->SetLabel(&&checkpoint_FibService_101);
      dynamic_pointer_cast<libpi::Channel>(_state->Values()[1])->Send(s2);
    }
    checkpoint_FibService_101:
    _state->Values()[1].reset(); // Erase c, now 0=fib, 2=s
    // Link complete
    // Create new task (thread) handling connection
    shared_ptr<libpi::task::Task> task2(new libpi::task::Task());
    task2->SetLabel(&&method_Fib);
    task2->Values().resize(2);
    task2->Values()[0]=_state->Values()[0];
    task2->Values()[1]=_state->Values()[2];
    // Call FibService
    _state->Values()[2].reset(); // Erase s, now 0=fib
    _state->SetLabel(&&method_FibService);
    ++(*libpi::task::Task::ActiveTasks);
    libpi::task::Task::Tasks.Send(task2);
    goto method_FibService;
  } // }}}
  method_Fib: // {{{
  { _state->Values().resize(8);
    // Idx: 0=fib,1=s
    _state->SetLabel(&&checkpoint_Fib_10);
    ((libpi::Session*)_state->Values()[1].get())->Receive(1,_state,((Task_Fib*)_state.get())->var_n); // Idx 2=n
    checkpoint_Fib_10:
    if (((*((libpi::Int*)_state->Values()[2].get()))<=(*intval_1))->GetValue())
    { _state->SetLabel(&&checkpoint_Fib_1);
      ((libpi::Session*)_state->Values()[1].get())->Send(1,intval_1);
      checkpoint_Fib_1:
      throw libpi::task::TaskPauseEvent(); // end
    }
    // s1 = new fib(2 of 2)
    _state->Values()[3].reset(new libpi::task::Channel()); // Idx 3=c
    _state->SetLabel(&&checkpoint_Fib_100);
    ((libpi::task::Link*)_state->Values()[0].get())->GetChannels()[0]->Send(_state->Values()[3]);
    checkpoint_Fib_100:
    _state->SetLabel(&&checkpoint_Fib_101);
    ((libpi::Channel*)_state->Values()[3].get())->Receive(_state,4); // Idx 4=s1
    checkpoint_Fib_101:
    _state->Values()[3].reset(); // Erase c, now 0=fib, 1=s, 2=n, 4=s1
    // s1[1] << n-1
    _state->SetLabel(&&checkpoint_Fib_2);
    ((libpi::Session*)_state->Values()[4].get())->Send(0,*((libpi::Int*)_state->Values()[2].get())-*intval_1);
    checkpoint_Fib_2:
    // Test if parallel or sequential
    if ((*libpi::task::Task::ActiveTasks)<libpi::task::Task::TargetTasks)
    { // Parallel
    // s2 = new fib(2 of 2)
    _state->Values()[3].reset(new libpi::task::Channel()); // Idx 3=c
    _state->SetLabel(&&checkpoint_Fib_102);
    ((libpi::task::Link*)_state->Values()[0].get())->GetChannels()[0]->Send(_state->Values()[3]);
    checkpoint_Fib_102:
    _state->SetLabel(&&checkpoint_Fib_103);
    ((libpi::Channel*)_state->Values()[3].get())->Receive(_state,5); // Idx 5=s2
    checkpoint_Fib_103:
    _state->Values()[3].reset(); // Erase c, now 0=fib, 1=s, 2=n, 4=s1, 5=s2
    // s2[1] << n-2
    _state->SetLabel(&&checkpoint_Fib_3);
    ((libpi::Session*)_state->Values()[5].get())->Send(0,*((libpi::Int*)_state->Values()[2].get())-*intval_2);
    checkpoint_Fib_3:
    // s1[1] >> f1
    _state->SetLabel(&&checkpoint_Fib_11);
    ((libpi::Session*)_state->Values()[4].get())->Receive(0,_state,6); // 6=f1
    checkpoint_Fib_11:
    // s2[1] >> f2
    _state->SetLabel(&&checkpoint_Fib_12);
    ((libpi::Session*)_state->Values()[5].get())->Receive(0,_state,7); // 7=f2
    checkpoint_Fib_12:
    // s[2]<<f1+f2
    _state->SetLabel(&&checkpoint_Fib_4);
    ((libpi::Session*)_state->Values()[1].get())->Send(1,(*((libpi::Int*)_state->Values()[6].get()))+(*((libpi::Int*)_state->Values()[7].get())));
    checkpoint_Fib_4:
    // end
    throw libpi::task::TaskPauseEvent();
    }
    else
    { // Sequential, remember 0=fib, 1=s, 2=n, 4=s1
    // s1[1] >> f1
    _state->SetLabel(&&checkpoint_Fib_13);
    ((libpi::Session*)_state->Values()[4].get())->Receive(0,_state,5); // Idx 5=f1
    checkpoint_Fib_13:
    // s2 = new fib(2 of 2)
    _state->Values()[3].reset(new libpi::task::Channel()); // Idx 3=c
    _state->SetLabel(&&checkpoint_Fib_104);
    ((libpi::task::Link*)_state->Values()[0].get())->GetChannels()[0]->Send(_state->Values()[3]);
    checkpoint_Fib_104:
    _state->SetLabel(&&checkpoint_Fib_105);
    ((libpi::Channel*)_state->Values()[3].get())->Receive(_state,6); // Idx 6=s2
    checkpoint_Fib_105:
    _state->Values()[3].reset(); // Erase c, now 0=fib, 1=s, 2=n, 4=s1, 5=f1, 6=s2
    // s2[1] << n-2
    _state->SetLabel(&&checkpoint_Fib_5);
    ((libpi::Session*)_state->Values()[6].get())->Send(0,*((libpi::Int*)_state->Values()[2].get())-*intval_2);
    checkpoint_Fib_5:
    // s2[1] >> f2
    _state->SetLabel(&&checkpoint_Fib_14);
    ((libpi::Session*)_state->Values()[6].get())->Receive(0,_state,7); // Idx 7=f2
    checkpoint_Fib_14:
    // s[2]<<f1+f2
    _state->SetLabel(&&checkpoint_Fib_6);
    ((libpi::Session*)_state->Values()[1].get())->Send(1,(*((libpi::Int*)_state->Values()[5].get()))+(*((libpi::Int*)_state->Values()[7].get())));
    checkpoint_Fib_6:
    // end
    throw libpi::task::TaskPauseEvent();
    }
  } // }}}
} // }}}

void *_workerthread(void *_arg) // {{{
{ try
  { while (true) // Continue until termination
    { // Wait for task
      shared_ptr<libpi::task::Task> task=dynamic_pointer_cast<libpi::task::Task>(libpi::task::Task::Tasks.Receive());
      if (!task)
        break;
      try
      { _methods(task);
        libpi::task::Task::Tasks.Send(task);
      }
      catch (libpi::task::TaskPauseEvent e)
      { --(*libpi::task::Task::ActiveTasks);
      }
    }
  } catch (const string &error)
  { cerr << "Error: " << error << endl;
  }
} // }}}
inline void _workers() // {{{
{ pthread_t x;
  pthread_attr_t y;
  pthread_attr_init(&y);
  pthread_attr_setstacksize(&y,16384);
  pthread_attr_setdetachstate(&y,PTHREAD_CREATE_DETACHED);
  for (size_t wc=0; wc+1<libpi::task::Task::TargetTasks; ++wc)
    pthread_create(&x,&y,_workerthread,NULL);
  _workerthread(NULL); // FIXME: Start and monitor instead
} // }}}

/*Start process, and its continuations */
int main(int argc, char **argv) // {{{
{ // PARSE ARGS!!
  for (int i=0; i<argc; ++i)
  { if (string(argv[i])=="-pi_tprocs" && i+1<argc)
    libpi::task::Task::TargetTasks=atoi(argv[++i]);
    else
      _args.push_back(argv[i]);
  }
  try
  { signal(SIGCHLD, SIG_IGN); // Fork optimization
    // Create main task
    shared_ptr<libpi::task::Task> s0(new libpi::task::Task());
    s0->SetLabel(NULL);
    ++(*libpi::task::Task::ActiveTasks);
    libpi::task::Task::Tasks.Send(s0);
    // Start workers
    _workers();
    //munmap(_aprocs,sizeof(int));
  } catch (const string &error)
  { cerr << "Error: " << error << endl;
    return 1;
  }
  return 0;
} // }}}
