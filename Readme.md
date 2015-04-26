HAPI
====
HAPI (pronounced happy) is a programming language.  
The name is an abbreviation of Happy Asynchronous PI-calculus.  
Technically, HAPI provides an intuitive syntax for the asynchronous pi-calculus
with multiparty session types, and provides a parser, type-checker,
interpreter, compiler, automatic program analysis and optimizations for this
language.

Why a new language
------------------
There are so many programming languages, that just counting them is an almost
impossible task. So why yet another programming language? The answer in the
case of HAPI is that it is not just another programming language - in the sense
that most programming languages fall into a very small set of language types
(such as standard imperative, functional or logical languages), and HAPI is
different in this way.

This is because HAPI is a process-oriented programming language. This
differentiates it from imperative-, functional- and logical programming
languages at a very basic level. What this means is that the basic constructs
of HAPI are different from any other language out there and thus the way
programs are designed and structured is different and will have different
properties. In the end, the same functionality can be implemented in HAPI as in
any other languagge (ie. it is a turing complete language), but because the
programs will be structured in a different way, the solution will have
different properties. This includes but is not limited to what errors and
mistakes can be found by type-checking and how the implementations can be
automatically optimized.

This is all very abstract, so let us go through a few examples where HAPI
offers some properties that cannot be offered by existing programming
languages.

Protocol verification
---------------------
As mentioned HAPI is a process oriented programming language. This means
everything is expressed as communicating processes. This is quite similar to
communicating webservices, or UNIX threads and processes with some type pf IPC
(Inter Process Communication). HAPI is different in the way that this
communication is a 'first class citizen' of the language, and can therefore be
verified by type-checking, reasoned about and optimized in a meaningful way.

What this means - in the world of webservices - is that whenever you have a
webservice that implements some protocol, the communication with that
webservice is type-checked such that if you should happen to make a mistake
(send a choice that is not supported, or wait for an answer when you need to
provide more input) are detected at compile time, and thus the compiled
programs are guaranteed to be free of such errors.

### Guarantees provided
  * Protocol adherence
  * Assertion adherence
  * Partial protocol completion (allows non productive loops)
  * Partial deadlock freedom (full freedom for single session processes)

#### Protocol Adherence Example
A classic mistake is forgetting to handle errors as seen below.
```
#define $intopt \
  1->2 \
  {^ok: 1->2:Int; $end; \
   ^error: 1->2:String; $end; \
  }
$intopt a(1,2);
s=new a(2 of 2);
s[1]>>result;
```
The above code will not typecheck, and report an error stating that we are
trying to receive a value on session s, when we should be branching. The below
code detects and handles errors as required by the type.
```
#define $intopt \
  1->2 \
  {^ok: 1->2:Int; $end; \
   ^error: 1->2:String; $end; \
  }
$intopt a(1,2);
s=new a(2 of 2);
s[1]>>
{^ok: s[1]>>result;
 ^error: s[1]>>message;
}
```

#### Protocol Completion Example
A classic mistake is forgetting end an open session.
```
#include <console.pi>

c=new console(2 of 2);
c[1]<<^PutString<<"Hello World";
```
The above code will not typecheck, and report an error stating that we are
terminating with an open session c. This is a common but grave mistake. First
of all, the service used will keep waiting for a message, but since this means
the console ls locked to the terminated process, other processes trying to use
the console will be deadlocked because the console is never released.

The below code ends the session as required.
```
#include <console.pi>

c=new console(2 of 2);
c[1]<<^PutString<<"Hello World";
c[1]<<^End;
```

Automatic Program Optimization
------------------------------
As mentioned HAPI is a process oriented programming language. This means
everything is expressed as communicating processes, and operations like forking
and waiting for other processes are 'first class citizen' of the language, and
thus allow automatic program analysis and optimizations. The effects of this is
that if you for example wish to have a certain number of active processes (in
order to utilize that number of CPU cores), then the compiler can analyze the
program, and optimize it to be more concurrent - and hence utilize more CPU
cores - and even balance this optimization to hit the desired number of
concurrent processes.

### Example
Consider the below code-snippet implementing the recursive part of a fibonacci
service.
```
fib1=new fib(2 of 2);
fib1[1]<<n-1;
fib1[1]>>f1;           // f1=fib(n-1)
fib2=new fib(2 of 2);
fib2[1]<<n-2;
fib2[1]>>f2;           // f2=fib(n-2)
fib0[2]<<f1+f2;        // return f1+f2
```
The above code is sequential, since the call to fib(n-2) is not started until
the result has been received from fib(n-1).

There are many advantages to sequential code over concurrent code! It is often
simpler, easier to read and write - because people and thus programmers think
sequentially - it is easier to debug, maintain and so on. There is one drawback
however - it does not utilize multicore systems satisfactory. If we wish to
utilize multicore systems, we could update the code snippet to be more
concurrent as below.
```
fib1=new fib(2 of 2);
fib1[1]<<n-1;
fib2=new fib(2 of 2);
fib2[1]<<n-2;
fib1[1]>>f1;           // f1=fib(n-1)
fib2[1]>>f2;           // f2=fib(n-2)
fib0[2]<<f1+f2;        // return f1+f2
```
The above code is concurrent, since the call to fib(n-2) is started before the
result has been received from fib(n-1).

This means the above code will utilize multicore systems better, because there
are more active processes at the same time. In theory this should cause the
concurrent version to perform better on multicore systems, but this is not what
we observe in tests. The reason is that the above version is infinitely
concurrent - it keeps increasing the number of active processes until it is
complete - creating an enormous amount (for large n) of active processes - a
lot more than there are likely to be CPU-cores, and this we believe causes a
large scheduler overhead eating up much of the CPU-time.

It is therefore better to use a finitely concurrent implementation ensuring
only a certain amount of active processes. This requires us to keep track of
the number of active processes, and target amount of active processes. Luckily
HAPI does this automatically, so the number of currently active processes can
be obtained with the expression `SYSTEM&"aprocs"` while the number of target
processes can be set with the CLI argument `-pi_tprocs n`, and obtained with the
expression `SYSTEM&"tprocs"`. Thus a finitely concurrent version can be written
as
```
if SYSTEM&"tprocs"<=SYSTEM&"aprocs"
then fib1=new fib(2 of 2);  // Sequential version
     fib1[1]<<n-1;
     fib1[1]>>f1;
     fib2=new fib(2 of 2);
     fib2[1]<<n-2;
     fib2[1]>>f2;
     fib0[2]<<f1+f2;
else fib1=new fib(2 of 2);  // Parallel version
     fib1[1]<<n-1;
     fib2=new fib(2 of 2);
     fib2[1]<<n-2;
     fib1[1]>>f1;
     fib2[1]>>f2;
     fib0[2]<<f1+f2;
```
The above code will perform better than the sequential version on multicore
systems, but as it is plain to see it includes two versions of the body, and
thus compared to the sequential version, it is more cumbersome to write, harder
to read, debug, maintain and so on.

The solution in HAPI is for the programmer to write the sequential version
(marking the service as pure) and then HAPI is able to automatically optimize
the implementation by rewriting the program to the finitely concurrent version
only when semantical guarantees are respected.

This should be the dream scenario for programmers - write your programs
sequentially, and enjoy the benefits in simplicity, debugging, testing,
maintaining and so on, and then have the implementation automatically
transformed to a finitely concurrent version with semantical preservation
guarantees, and enjoy the benefits on multicore systems - getting the best of
both worlds.
