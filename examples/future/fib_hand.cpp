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
    shared_ptr<libpi::Value> var_s;
    shared_ptr<libpi::Value> var_f;
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
inline bool _methods(shared_ptr<libpi::task::Task> &_state) // {{{
{
  void *_label=_state->GetLabel();
  if (_label!=NULL)
    goto *_label;
  { // Main {{{
    ((Task_Main*)_state.get())->var_fib.reset(new libpi::task::Link(2)); // Idx 0=fib
    for (size_t x=0; x<libpi::task::Task::TargetTasks; ++x)
    { Task_FibService *newstate = new Task_FibService();
      newstate->var_fib=((Task_Main*)_state.get())->var_fib; // Idx 0=fib
      newstate->SetLabel(&&method_FibService);
      ++(*libpi::task::Task::ActiveTasks);
      libpi::task::Task::Tasks.Send(shared_ptr<libpi::task::Task>(newstate));
    }
    { // Left process
      // s = new fib(2 of 2)
      ((Task_Main*)_state.get())->var_tmp.reset(new libpi::task::Channel()); // Idx 1=c
      _state->SetLabel(&&checkpoint_main_100);
      ((libpi::task::Link*)((Task_Main*)_state.get())->var_fib.get())->GetChannels()[0]->Send(((Task_Main*)_state.get())->var_tmp);
      checkpoint_main_100:
      _state->SetLabel(&&checkpoint_main_101);
      if (!((libpi::Channel*)((Task_Main*)_state.get())->var_tmp.get())->Receive(_state,((Task_Main*)_state.get())->var_s)) // Idx2=s
        return false;
      checkpoint_main_101:
      ((Task_Main*)_state.get())->var_tmp.reset(); // Erase c, now 0=fib, 2=s
      // s[1] << 5
      _state->SetLabel(&&checkpoint_main_1);
      ((libpi::Session*)((Task_Main*)_state.get())->var_s.get())->Send(0,intval_25);
      checkpoint_main_1:
      // s[1] >> f1
      _state->SetLabel(&&checkpoint_main_10);
      if (!((libpi::Session*)((Task_Main*)_state.get())->var_s.get())->Receive(0,_state,((Task_Main*)_state.get())->var_f))
        return false;
      checkpoint_main_10:
      // Print result
      cout << ((Task_Main*)_state.get())->var_f->ToString() << endl;
      exit(0);
      return false;
    }
  } // }}}

/* Procedure implementations */
  method_FibService: // {{{
  { // Wait for connection
    // s = new fib(1 of 2);
    _state->SetLabel(&&checkpoint_FibService_100);
    if (!((libpi::task::Link*)((Task_FibService*)_state.get())->var_fib.get())->GetChannels()[0]->Receive(_state,((Task_FibService*)_state.get())->var_tmp)) // Idx 1=c
      return false;
    checkpoint_FibService_100:
    // Create both sides of session
    { vector<shared_ptr<libpi::Channel> > p0InChannels(2), p0OutChannels(2), p1InChannels(2), p1OutChannels(2);
      p0InChannels[0].reset(new libpi::task::Channel());
      p0InChannels[1].reset(new libpi::task::Channel());
      p0OutChannels[0]=p0InChannels[0];
      p0OutChannels[1]=dynamic_pointer_cast<libpi::Channel>(((Task_FibService*)_state.get())->var_tmp);
      p1InChannels[0]=p0OutChannels[1];
      p1InChannels[1].reset(new libpi::task::Channel());
      p1OutChannels[0]=p0InChannels[1];
      p1OutChannels[1]=p1InChannels[1];
      ((Task_FibService*)_state.get())->var_s.reset(new libpi::Session(0,2,p0InChannels,p0OutChannels)); // Idx 2=s
      shared_ptr<libpi::Value> s2(new libpi::Session(1,2,p1InChannels,p1OutChannels));
      _state->SetLabel(&&checkpoint_FibService_101);
      dynamic_pointer_cast<libpi::Channel>(((Task_FibService*)_state.get())->var_tmp)->Send(s2);
    }
    checkpoint_FibService_101:
    ((Task_FibService*)_state.get())->var_tmp.reset(); // Erase c, now 0=fib, 2=s
    // Link complete
    // Create new task handling connection
    { Task_Fib *_task2 = new Task_Fib();
      _task2->var_fib=((Task_FibService*)_state.get())->var_fib;
      _task2->var_s=((Task_FibService*)_state.get())->var_s;
      _task2->SetLabel(&&method_Fib);
      ++(*libpi::task::Task::ActiveTasks);
      libpi::task::Task::Tasks.Send(shared_ptr<libpi::task::Task>(_task2));
    }
    ((Task_FibService*)_state.get())->var_s.reset(); // Erase s, now 0=fib
    _state->SetLabel(&&method_FibService);
    goto method_FibService;
  } // }}}
  method_Fib: // {{{
  { // Idx: 0=fib,1=s
    _state->SetLabel(&&checkpoint_Fib_10);
    if (!((libpi::Session*)((Task_Fib*)_state.get())->var_s.get())->Receive(1,_state,((Task_Fib*)_state.get())->var_n)) // Idx 2=n
      return false;
    checkpoint_Fib_10:
    if (((*((libpi::Int*)((Task_Fib*)_state.get())->var_n.get()))<=(*intval_1))->GetValue())
    { _state->SetLabel(&&checkpoint_Fib_1);
      ((libpi::Session*)((Task_Fib*)_state.get())->var_s.get())->Send(1,intval_1);
      checkpoint_Fib_1:
      return false; // end
    }
    // s1 = new fib(2 of 2)
    ((Task_Fib*)_state.get())->var_tmp.reset(new libpi::task::Channel()); // Idx 3=c
    _state->SetLabel(&&checkpoint_Fib_100);
    ((libpi::task::Link*)((Task_Fib*)_state.get())->var_fib.get())->GetChannels()[0]->Send(((Task_Fib*)_state.get())->var_tmp);
    checkpoint_Fib_100:
    _state->SetLabel(&&checkpoint_Fib_101);
    if (!((libpi::Channel*)((Task_Fib*)_state.get())->var_tmp.get())->Receive(_state,((Task_Fib*)_state.get())->var_s1)) // Idx 4=s1
      return false;
    checkpoint_Fib_101:
    ((Task_Fib*)_state.get())->var_tmp.reset(); // Erase c, now 0=fib, 1=s, 2=n, 4=s1
    // s1[1] << n-1
    _state->SetLabel(&&checkpoint_Fib_2);
    ((libpi::Session*)((Task_Fib*)_state.get())->var_s1.get())->Send(0,*((libpi::Int*)((Task_Fib*)_state.get())->var_n.get())-*intval_1);
    checkpoint_Fib_2:
    // Test if parallel or sequential
    if ((*libpi::task::Task::ActiveTasks)<libpi::task::Task::TargetTasks)
    { // Parallel
    // s2 = new fib(2 of 2)
    ((Task_Fib*)_state.get())->var_tmp.reset(new libpi::task::Channel()); // Idx 3=c
    _state->SetLabel(&&checkpoint_Fib_102);
    ((libpi::task::Link*)((Task_Fib*)_state.get())->var_fib.get())->GetChannels()[0]->Send(((Task_Fib*)_state.get())->var_tmp);
    checkpoint_Fib_102:
    _state->SetLabel(&&checkpoint_Fib_103);
    if (!((libpi::Channel*)((Task_Fib*)_state.get())->var_tmp.get())->Receive(_state,((Task_Fib*)_state.get())->var_s2)) // Idx 5=s2
      return false;
    checkpoint_Fib_103:
    ((Task_Fib*)_state.get())->var_tmp.reset(); // Erase c, now 0=fib, 1=s, 2=n, 4=s1, 5=s2
    // s2[1] << n-2
    _state->SetLabel(&&checkpoint_Fib_3);
    ((libpi::Session*)((Task_Fib*)_state.get())->var_s2.get())->Send(0,*((libpi::Int*)((Task_Fib*)_state.get())->var_n.get())-*intval_2);
    checkpoint_Fib_3:
    // s1[1] >> f1
    _state->SetLabel(&&checkpoint_Fib_11);
    if (!((libpi::Session*)((Task_Fib*)_state.get())->var_s1.get())->Receive(0,_state,((Task_Fib*)_state.get())->var_f1)) // 6=f1
      return false;
    checkpoint_Fib_11:
    // s2[1] >> f2
    _state->SetLabel(&&checkpoint_Fib_12);
    if (!((libpi::Session*)((Task_Fib*)_state.get())->var_s2.get())->Receive(0,_state,((Task_Fib*)_state.get())->var_f2)) // 7=f2
      return false;
    checkpoint_Fib_12:
    // s[2]<<f1+f2
    _state->SetLabel(&&checkpoint_Fib_4);
    ((libpi::Session*)((Task_Fib*)_state.get())->var_s.get())->Send(1,(*((libpi::Int*)((Task_Fib*)_state.get())->var_f1.get()))+(*((libpi::Int*)((Task_Fib*)_state.get())->var_f2.get())));
    checkpoint_Fib_4:
    // end
    return false;
    }
    else
    { // Sequential, remember 0=fib, 1=s, 2=n, 4=s1
    // s1[1] >> f1
    _state->SetLabel(&&checkpoint_Fib_13);
    if (!((libpi::Session*)((Task_Fib*)_state.get())->var_s1.get())->Receive(0,_state,((Task_Fib*)_state.get())->var_f1)) // Idx 5=f1
      return false;
    checkpoint_Fib_13:
    // s2 = new fib(2 of 2)
    ((Task_Fib*)_state.get())->var_tmp.reset(new libpi::task::Channel()); // Idx 3=c
    _state->SetLabel(&&checkpoint_Fib_104);
    ((libpi::task::Link*)((Task_Fib*)_state.get())->var_fib.get())->GetChannels()[0]->Send(((Task_Fib*)_state.get())->var_tmp);
    checkpoint_Fib_104:
    _state->SetLabel(&&checkpoint_Fib_105);
    if (!((libpi::Channel*)((Task_Fib*)_state.get())->var_tmp.get())->Receive(_state,((Task_Fib*)_state.get())->var_s2)) // Idx 6=s2
      return false;
    checkpoint_Fib_105:
    ((Task_Fib*)_state.get())->var_tmp.reset(); // Erase c, now 0=fib, 1=s, 2=n, 4=s1, 5=f1, 6=s2
    // s2[1] << n-2
    _state->SetLabel(&&checkpoint_Fib_5);
    ((libpi::Session*)((Task_Fib*)_state.get())->var_s2.get())->Send(0,*((libpi::Int*)((Task_Fib*)_state.get())->var_n.get())-*intval_2);
    checkpoint_Fib_5:
    // s2[1] >> f2
    _state->SetLabel(&&checkpoint_Fib_14);
    if (!((libpi::Session*)((Task_Fib*)_state.get())->var_s2.get())->Receive(0,_state,((Task_Fib*)_state.get())->var_f2)) // Idx 7=f2
      return false;
    checkpoint_Fib_14:
    // s[2]<<f1+f2
    _state->SetLabel(&&checkpoint_Fib_6);
    ((libpi::Session*)((Task_Fib*)_state.get())->var_s.get())->Send(1,(*((libpi::Int*)((Task_Fib*)_state.get())->var_f1.get()))+(*((libpi::Int*)((Task_Fib*)_state.get())->var_f2.get())));
    checkpoint_Fib_6:
    // end
    return false;
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
      if (_methods(task))
        libpi::task::Task::Tasks.Send(task);
      else
        --(*libpi::task::Task::ActiveTasks);
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
    shared_ptr<libpi::task::Task> s0(new Task_Main());
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
