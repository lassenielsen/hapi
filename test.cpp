#include <iostream>
#include <pthread.h>
#include <libpi/value.hpp>
#include <vector>
#include <queue>
#include <sstream>
#include <atomic>

namespace libpi {
  class Mutex
  { public:
      Mutex(bool locked=false)
      { pthread_mutex_init(&myMutex,NULL);
        if (locked)
          pthread_mutex_lock(&myMutex);
      }
      Mutex(const Mutex &init) {throw std::string("Copying a lock is forbidden"); }
      ~Mutex() { pthread_mutex_destroy(&myMutex); }

      Mutex &operator=(const Mutex &rhs) {throw std::string("Assigning a lock is forbidden"); }
      void Lock() {pthread_mutex_lock(&myMutex);}
      void Release() {pthread_mutex_unlock(&myMutex);}

    private:
      pthread_mutex_t myMutex;
  };

  namespace thread {
    class Channel : public Value
    { public:
        Channel() // {{{
        : msgs(new std::queue<libpi::Value*>())
        , sync(new Mutex(true))
        , lock(new Mutex())
        , msg_count(new std::atomic<int>(0))
        , ref_count(new std::atomic<int>(1))
        {} // }}}
        Channel(const Channel &rhs) // {{{
        : msgs(rhs.msgs)
        , sync(rhs.sync)
        , lock(rhs.lock)
        , msg_count(rhs.msg_count)
        , ref_count(rhs.ref_count)
        { ++(*ref_count);
        } // }}}
        virtual ~Channel() { Detach(); }
        void Snd(Value *msg)
        { lock->Lock();
          ++(*msg_count);
          msgs->push(msg);
          if (msg_count<=0)
            sync->Release();
          lock->Release();
        }
        Value *Rcv()
        { lock->Lock();
          --(*msg_count);
          if (msg_count<0)
          { lock->Release();
            sync->Lock();
            lock->Lock();
          }

          Value *result=msgs->front();
          msgs->pop();
          lock->Release();
          return result;
        }
    
        std::string ToString() const
        { std::stringstream ss;
          ss << msgs;
          return ss.str();
        }
    
        Channel *Copy() const
        { return new Channel(*this);
        }

        bool operator==(const Value &rhs) const
        { const Channel *rhsptr=dynamic_cast<const Channel*>(&rhs);
          return rhsptr!=NULL && msgs==rhsptr->msgs;
        }

        Channel &operator=(const Channel &rhs)
        { Detach();
          rhs.lock->Lock();
          ref_count=rhs.ref_count;
          ++(*ref_count);
          msgs=rhs.msgs;
          msg_count=rhs.msg_count;
          sync=rhs.sync;
          lock=rhs.lock;
          rhs.lock->Release();
          return *this;
        }


      protected:
        void Detach()
        { return; // FIXME
          lock->Lock();
          --(*ref_count);
          if (*ref_count<=0)
          { std::cout << "Deleting queue at: " << size_t(msgs) << std::endl;
            delete ref_count;
            delete msg_count;
            delete sync;
            while (msgs->size()>0)
            { delete msgs->front();
              msgs->pop();
            }
            delete msgs;
            lock->Release();
            delete lock;
          }
          else
            lock->Release();
          ref_count=NULL;
          msg_count=NULL;
          lock=NULL;
          sync=NULL;
          msgs=NULL;
        }

      private:
        std::queue<Value*> *msgs;
        Mutex *sync;
        Mutex *lock;
        std::atomic<int> *msg_count;
        std::atomic<int> *ref_count;
    };

  }
}

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
  s1->values.push_back(f);
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
    s2->values.push_back(dynamic_cast<libpi::Value*>(n1));
    s2->values.push_back(f1->Copy());
    spawn(s2);
    libpi::Value *r1=f1->Rcv();
    libpi::IntValue *r1ptr=dynamic_cast<libpi::IntValue*>(r1);

    libpi::thread::Channel *f2(new libpi::thread::Channel());
    libpi::IntValue *n2(new libpi::IntValue((*x)-libpi::IntValue(2)));
    State *s3=new State();
    s3->label=&&method_fib;
    s3->values.push_back(dynamic_cast<libpi::Value*>(n2));
    s3->values.push_back(f2->Copy());
    spawn(s3);
    libpi::Value *r2=f2->Rcv();
    libpi::IntValue *r2ptr=dynamic_cast<libpi::IntValue*>(r2);

    f0->Snd(new libpi::IntValue((*r1ptr)+(*r2ptr)));
    delete r1;
    delete r2;
    delete state;
    delete f1;
    delete f2;
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
