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
