#include <iostream>
#include <pthread.h>
#include <libpi/int.hpp>
#include <libpi/bool.hpp>
#include <libpi/thread/channel.hpp>
#include <vector>
#include <queue>
#include <sstream>
#include <atomic>
#include <memory>

using namespace std;
//using namespace pi;

class State
{ public:
    void *label;
    vector<shared_ptr<libpi::Value> > values;
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

size_t val=0;
shared_ptr<libpi::Int> one(new libpi::Int(1));
shared_ptr<libpi::Int> two(new libpi::Int(2));

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
  shared_ptr<libpi::thread::Channel> f(new libpi::thread::Channel());
  s1->label=&&method_fib;
  s1->values.push_back(shared_ptr<libpi::Int>(new libpi::Int(val)));
  s1->values.push_back(f);
  spawn(s1);
  shared_ptr<libpi::Value> r=f->Receive();
  cout << "Result: " << r->ToString() << endl;
  pthread_exit(0);

  // fib
  method_fib:
  shared_ptr<libpi::Int> x=dynamic_pointer_cast<libpi::Int>(state->values[0]);
  //cout << "fib(" << x->ToString() << ")" << endl;
  shared_ptr<libpi::thread::Channel> f0=dynamic_pointer_cast<libpi::thread::Channel>(state->values[1]);
  if (((*x) <= (*one))->GetValue())
  { f0->Send(one);
    delete state;
    pthread_exit(0);
  }
  else
  { shared_ptr<libpi::thread::Channel> f1(new libpi::thread::Channel());
    shared_ptr<libpi::Int> n1=(*x)-(*one);
    State *s2=new State();
    s2->label=&&method_fib;
    s2->values.push_back(n1);
    s2->values.push_back(f1);
    spawn(s2);
    shared_ptr<libpi::Value> r1=f1->Receive();
    shared_ptr<libpi::Int> r1ptr=dynamic_pointer_cast<libpi::Int>(r1);

    shared_ptr<libpi::thread::Channel> f2(new libpi::thread::Channel());
    shared_ptr<libpi::Int> n2=(*x)-(*two);
    State *s3=new State();
    s3->label=&&method_fib;
    s3->values.push_back(dynamic_pointer_cast<libpi::Value>(n2));
    s3->values.push_back(f2);
    spawn(s3);
    shared_ptr<libpi::Value> r2=f2->Receive();
    shared_ptr<libpi::Int> r2ptr=dynamic_pointer_cast<libpi::Int>(r2);

    f0->Send((*r1ptr)+(*r2ptr));
    pthread_exit(0);
  }
}

int main(int argc, char **argv)
{
  stringstream ss;
  ss << argv[1];
  ss >> val;
  State *s0=new State();
  s0->label=NULL;
  all_methods(s0);
  return 0;
}
