#include <iostream>
#include <pthread.h>
#include <libpi/value.hpp>
#include <libpi/thread/channel.hpp>
#include <vector>
#include <queue>
#include <sstream>
#include <atomic>

using namespace std;
//using namespace pi;

class State
{ public:
    virtual ~State()
    { while (values.size()>0)
      { delete values.back();
        values.pop_back();
      }
    }
    void *label;
    vector<libpi::Value*> values;
};

// Declare implementation
void *all_methods(void *arg);

inline void spawn(State *state)
{ pthread_t x;
  pthread_attr_t y;
  pthread_attr_init(&y);
  pthread_attr_setstacksize(&y,16384);
  pthread_attr_setdetachstate(&y,PTHREAD_CREATE_DETACHED);
  //cout << "label: " << state->label << endl;
  pthread_create(&x,&y,all_methods,(void*)state);
}

//void *args;
#define state ((State*)(arg))
void *all_methods(void *arg)
{ if (state==NULL) // Error
  { cerr << "Error in call to all_methods: null state provided" << endl;
    return NULL;
  }  
  if (state->label!=NULL) // Main
    goto *state->label;
  // Main
  delete state;
  State *s1=new State();
  libpi::thread::Channel *f(new libpi::thread::Channel());
  s1->label=&&method_fib;
  s1->values.push_back(new libpi::IntValue(20));
  s1->values.push_back(f->Copy());
  spawn(s1);
  libpi::Value *r=f->Rcv();
  cout << "Result: " << r->ToString() << endl;
  pthread_exit(0);

  // fib
  method_fib:
  libpi::IntValue *x=dynamic_cast<libpi::IntValue*>(state->values[0]);
  libpi::thread::Channel *f0=dynamic_cast<libpi::thread::Channel*>(state->values[1]);
  if (*x <= libpi::IntValue(1))
  { f0->Snd(new libpi::IntValue(1));
    delete state;
    pthread_exit(0);
  }
  else
  { libpi::thread::Channel *f1(new libpi::thread::Channel());
    libpi::IntValue *n1(new libpi::IntValue((*x)-libpi::IntValue(1)));
    State *s2=new State();
    s2->label=&&method_fib;
    s2->values.push_back(n1);
    s2->values.push_back(f1->Copy());
    spawn(s2);
    libpi::Value *r1=f1->Rcv();
    delete f1;
    libpi::IntValue *r1ptr=dynamic_cast<libpi::IntValue*>(r1);

    libpi::thread::Channel *f2(new libpi::thread::Channel());
    libpi::IntValue *n2(new libpi::IntValue((*x)-libpi::IntValue(2)));
    State *s3=new State();
    s3->label=&&method_fib;
    s3->values.push_back(dynamic_cast<libpi::Value*>(n2));
    s3->values.push_back(f2->Copy());
    spawn(s3);
    libpi::Value *r2=f2->Rcv();
    delete f2;
    libpi::IntValue *r2ptr=dynamic_cast<libpi::IntValue*>(r2);

    f0->Snd(new libpi::IntValue((*r1ptr)+(*r2ptr)));
    delete r1;
    delete r2;
    delete state;
    pthread_exit(0);
  }
}

int main(int argc, char **argv)
{
  State *s0=new State();
  s0->label=NULL;
  all_methods(s0);
  return 0;
}
