# Tutorial
## 1 Basics
### 1.1 Hello World
#### Source code
<pre>
// Example 1.1 - Hello world
// Includes console service, connects and makes it print the hello world string

// Include console functionality like printing
#include &lt;console.pi>

// Create console session (and get uninterupted access to console io)
c=new console(2 of 2);
// Print string and newline
c[1]&lt;&lt;^str&lt;&lt;"Hello World"&lt;&lt;^nl;
// Finish console session (and release console io)
c[1]&lt;&lt;^end;
</pre>
#### Output
<pre>
>./1.1-hello_world
Hello World
</pre>
### 1.2 Fork
#### Source code
<pre>
// Example 1.2 - Introducing fork ( | )
// Fork to 5 processes each printing a number to console, resulting in the
// numbers 1 to 5 being printed, although not necessarily in order!

#include &lt;console.pi>

// Print 1
c=new console(2 of 2);
c[1]&lt;&lt;^int&lt;&lt;1&lt;&lt;^nl&lt;&lt;^end;
| // fork
// Print 2
c=new console(2 of 2);
c[1]&lt;&lt;^int&lt;&lt;2&lt;&lt;^nl&lt;&lt;^end;
| // fork
// Print 3
c=new console(2 of 2);
c[1]&lt;&lt;^int&lt;&lt;3&lt;&lt;^nl&lt;&lt;^end;
| // fork
// Print 4
c=new console(2 of 2);
c[1]&lt;&lt;^int&lt;&lt;4&lt;&lt;^nl&lt;&lt;^end;
| // fork
// Print 5
c=new console(2 of 2);
c[1]&lt;&lt;^int&lt;&lt;5&lt;&lt;^nl&lt;&lt;^end;
</pre>
#### Output
<pre>
>./1.2-fork
1
2
3
4
5
</pre>
### 1.3 Methods
#### Source code
<pre>
// Example 1.3 - Introducing methods
// Declares method, and uses it.
// Note that methods can only be applied tail-recursively,
// if non-tail recursive functionality is required, use a
// service as introduced in tutorial 2.2.

#include &lt;console.pi>

// Implement the method PrintInt
local PrintInt(Int i)
( // Print i to console
  c=new console(2 of 2);
  c[1]&lt;&lt;^int 
      &lt;&lt;i
      &lt;&lt;^nl
      &lt;&lt;^end;
)
// Implement the method PrintInts
global PrintInts(Int i)
( // Print 1,2,...,i
  if i&lt;=0
  then
  else ( PrintInt(i); | PrintInts(i-1); )
)
// Now the declared methods can be used
PrintInts(10);
</pre>
#### Output
<pre>
>./1.3-methods
10
9
8
7
6
5
4
3
2
1
</pre>
## 2 Communication
### 2.1 Message
#### Source code
<pre>
// Example 2.1 - Message passing
// Two processes connect on channel, and
// transfer a value over the resulting session.

#include &lt;console.pi>

// Define thee $intmsg protocol (session type)
// where participant 1 sends an integer to participant 2.
#define $intmsg \
  1->2:Int; \
  $end;

// Create channel a with 2 participants using $intmsg protocol
global $intmsg a(1,2);

// First process implementation
s=new a(1 of 2); // Connect on channel a as participant 1 of 2, resulting in session s
c=new console(2 of 2);
c[1]&lt;&lt;^str&lt;&lt;"Sending value: "&lt;&lt;^int&lt;&lt;5&lt;&lt;^nl&lt;&lt;^end;
s[2]&lt;&lt;5;         // Send value 5 to participant 2 in sesssion s

| // Fork

// Second process implementation
s=new a(2 of 2); // Connect on channel a as participant 2 of 2, resulting in session s
s[1]>>x;         // Receive value from participant 1 in session s, and store as x.
c=new console(2 of 2);
c[1]&lt;&lt;^str&lt;&lt;"Received value: "&lt;&lt;^int&lt;&lt;x&lt;&lt;^nl&lt;&lt;^end;
</pre>
#### Output
<pre>
>./2.1-message
Sending value: 5
Received value: 5
</pre>
### 2.2 Service
#### Source code
<pre>
// Example 2.2 - Service
// The most basic design pattern in hapi is SOA - Implement functionality as
// services.
// Defines function type as protocol, creates channel for service, implements
// service as a participant and have client invoke service to use
// functionality.

#include &lt;console.pi>

// Define the $intfun protocol (session type)
// where participant 2 sends an integer to participant 1 (call arguent),
// and participant 1 sends an integer back to participant 2 (result).
#define $intfun \
  2->1:Int; \
  1->2:Int; \
  $end;

// Create channel a with 2 participants using $intmsg protocol
global $intfun double(1,2);
// Implement participant 1 as a service
local Double()
( global s=new double(1 of 2); // Wait for connection
  Double();                    // Keep service available
  |                            // Fork process to handle request
  s[2]>>x;                     // Receive argument
  s[2]&lt;&lt;x+x;                   // Send result
)
Double();                      // Start service
|
// Implement a client using service
c=new console(2 of 2);
d=new double(2 of 2);
d[1]&lt;&lt;1;
d[1]>>x;
c[1]&lt;&lt;^str&lt;&lt;"Double "&lt;&lt;^int&lt;&lt;1&lt;&lt;^str&lt;&lt;" is "&lt;&lt;^int&lt;&lt;x&lt;&lt;^nl;
d=new double(2 of 2);
d[1]&lt;&lt;x;
d[1]>>y;
c[1]&lt;&lt;^str&lt;&lt;"Double "&lt;&lt;^int&lt;&lt;x&lt;&lt;^str&lt;&lt;" is "&lt;&lt;^int&lt;&lt;y&lt;&lt;^nl&lt;&lt;^end;
</pre>
#### Output
<pre>
>./2.2-service
Double 1 is 2
Double 2 is 4
</pre>
### 2.3 Branch
#### Source code
<pre>
// Example 2.3 - Branching
// Two processes connect on channel, and the second participant selects how the
// session proceeds.

#include &lt;console.pi>

// Define the $branch protocol (session type)
// Participant 2 selects either number or text
// If Number, partiicipant 1 sends an integer back
// If Text, participant 1 sends a string back.
#define $branch \
  2->1 \
  {^Number: 1->2:Int; $end; \
   ^Text: 1->2:String; $end; \
  }

// Create channel a with 2 participants using the $branch protocol
global $branch a(1,2);
// First participant implementation as service
local A2()
( global s=new a(1 of 2);
  A2(); |
  s[2]>>
  {^Number:
    s[2]&lt;&lt;5;
   ^Text:
    s[2]&lt;&lt;"five";
  }
)
A2();
|
// Implementation of client (selecting number)
c=new console(2 of 2);
s=new a(2 of 2);
c[1]&lt;&lt;^str&lt;&lt;"Requesting a number"&lt;&lt;^nl;
s[1]&lt;&lt;^Number;
s[1]>>x;
c[1]&lt;&lt;^str&lt;&lt;"Received: "&lt;&lt;^int&lt;&lt;x&lt;&lt;^nl;
c[1]&lt;&lt;^end;
|
// Implementation of client (selecting text)
c=new console(2 of 2);
s=new a(2 of 2);
c[1]&lt;&lt;^str&lt;&lt;"Requesting a text"&lt;&lt;^nl;
s[1]&lt;&lt;^Text;
s[1]>>x;
c[1]&lt;&lt;^str&lt;&lt;"Received: "&lt;&lt;^str&lt;&lt;x&lt;&lt;^nl&lt;&lt;^end;
</pre>
#### Output
<pre>
>./2.3-branch
Requesting a number
Received: 5
Requesting a text
Received: five
</pre>
### 2.4 Recursion
#### Source code
<pre>
// Example 2.4 - Recursive protocols
// Two processes connect on channel, and transfer
// a number of values over the resulting session.

#include &lt;console.pi>

// Define thee $intsmsg protocol (session type)
// where participant 1 sends an unspecified
// number of integers to participant 2.
#define $intmsgs \
  rec $x; \
  1->2 \
  {^Done: $end; \
   ^More: 1->2:Int; $x;\
  }

// Create channel a with 2 participants using $intmsgs protocol
global $intmsgs a(1,2);

// First process implementation
s=new a(1 of 2); // Connect on channel a as participant 1 of 2, resulting in session s
local SendInts($intmsgs(1 of 1,2) t, Int i)
( if i&lt;=0
  then t[2]&lt;&lt;^Done;
  else c=new console(2 of 2);
       c[1]&lt;&lt;^str&lt;&lt;"Sending value: "&lt;&lt;^int&lt;&lt;i&lt;&lt;^nl&lt;&lt;^end;
       t[2]&lt;&lt;^More&lt;&lt;i; SendInts(t,i-1);
)
SendInts(s,10);

| // Fork

// Second process implementation
s=new a(2 of 2); // Connect on channel a as participant 2 of 2, resulting in session s
local ReceiveInts($intmsgs(2 of 1,2) t)
( t[1]>>
  {^Done:
   ^More: t[1]>>x;
          c=new console(2 of 2);
          c[1]&lt;&lt;^str&lt;&lt;"Received value: "&lt;&lt;^int&lt;&lt;x&lt;&lt;^nl&lt;&lt;^end;
          ReceiveInts(t);
  }
)
ReceiveInts(s);
</pre>
#### Output
<pre>
>./2.4-recursion
Sending value: 10
Received value: 10
Sending value: 9
Sending value: 8
Received value: 9
Sending value: 7
Received value: 8
Sending value: 6
Received value: 7
Sending value: 5
Received value: 6
Sending value: 4
Received value: 5
Sending value: 3
Received value: 4
Sending value: 2
Received value: 3
Sending value: 1
Received value: 2
Received value: 1
</pre>
### 2.5 Multiparty
#### Source code
<pre>
// Example 2.5 - Multiparty protocols
// Four processes connect on channel, and
// transfer a value roundrobin over the resulting session.

#include &lt;console.pi>

// Define the $rr4 protocol (session type)
// where participant 1 sends an integer to participant 2
// who sends an integer to participant 3
// who sends an integer to participant 4
// who sends an integer to participant 1.
#define $rr4 \
  1->2:Int; \
  2->3:Int; \
  3->4:Int; \
  4->1:Int; \
  $end;

// Create channel a with 4 participants using $rr4 protocol
global $rr4 a(1,2,3,4);

// First process implementation
s=new a(1 of 4); // Connect on channel a as participant 1 of 4, resulting in session s
c=new console(2 of 2);
c[1]&lt;&lt;^str&lt;&lt;"1: Sending value: "&lt;&lt;^int&lt;&lt;5&lt;&lt;^nl&lt;&lt;^end;
s[2]&lt;&lt;5;
s[4]>>x;
c=new console(2 of 2);
c[1]&lt;&lt;^str&lt;&lt;"1: Received value: "&lt;&lt;^int&lt;&lt;x&lt;&lt;^nl&lt;&lt;^end;

| // Fork

// Second process implementation
s=new a(2 of 4); // Connect on channel a as participant 2 of 4, resulting in session s
s[1]>>x;
c=new console(2 of 2);
c[1]&lt;&lt;^str&lt;&lt;"2: Received value: "&lt;&lt;^int&lt;&lt;x&lt;&lt;^nl
    &lt;&lt;^str&lt;&lt;"2: Sending value: "&lt;&lt;^int&lt;&lt;x&lt;&lt;^nl&lt;&lt;^end;
s[3]&lt;&lt;x;

| // Fork

// Third process implementation
s=new a(3 of 4); // Connect on channel a as participant 3 of 4, resulting in session s
s[2]>>x;
c=new console(2 of 2);
c[1]&lt;&lt;^str&lt;&lt;"3: Received value: "&lt;&lt;^int&lt;&lt;x&lt;&lt;^nl
    &lt;&lt;^str&lt;&lt;"3: Sending value: "&lt;&lt;^int&lt;&lt;x&lt;&lt;^nl&lt;&lt;^end;
s[4]&lt;&lt;x;

| // Fork

// Fourth process implementation
s=new a(4 of 4); // Connect on channel a as participant 4 of 4, resulting in session s
s[3]>>x;
c=new console(2 of 2);
c[1]&lt;&lt;^str&lt;&lt;"4: Received value: "&lt;&lt;^int&lt;&lt;x&lt;&lt;^nl
    &lt;&lt;^str&lt;&lt;"4: Sending value: "&lt;&lt;^int&lt;&lt;x&lt;&lt;^nl&lt;&lt;^end;
s[1]&lt;&lt;x;
</pre>
#### Output
<pre>
>./2.5-multiparty
1: Sending value: 5
2: Received value: 5
2: Sending value: 5
3: Received value: 5
3: Sending value: 5
4: Received value: 5
4: Sending value: 5
1: Received value: 5
</pre>
## 3 Polymorphism
### 3.1 Pair Service
#### Source code
<pre>
// This serves as an instructive example, but definition, decleration and
// implementation can also be included from strct/pair.pi,
// as shown in tutorial 4.1.
#include&lt;console.pi>
// Definition of abstract pair as protocol
#define $instpair(_fst,_snd) \
  rec $instpair; \
  2->1 \
  {^get_fst: \
    1->2: _fst; \
    $instpair; \
   ^set_fst: \
    2->1: _fst; \
    $instpair; \
   ^get_snd: \
    1->2: _snd; \
    $instpair; \
   ^set_snd: \
    2->1: _snd; \
    $instpair; \
   ^end: \
    $end; \
  }
#define $pair \
  2: #fst; \
  2: #snd; \
  2->1: #fst; \
  2->1: #snd; \
  $instpair(#fst,#snd)
// Declaration of abstract pair service
global $pair pair(1 pure,2);
// Implementation of abstract pair
local pure service Pair(pair (1 of 2) s)
( s >> #fst;
  s >> #snd;
  s[2] >> fst;
  s[2] >> snd;
  local pure InstPair(#fst fst, #snd snd, $instpair(#fst,#snd)(1 of 1 pure, 2) this)
  ( this[2]>>
    {^get_fst:
      this[2]&lt;&lt;fst;
      InstPair(fst,snd,this);
     ^get_snd:
      this[2]&lt;&lt;snd;
      InstPair(fst,snd,this);
     ^set_fst:
      this[2]>>x;
      InstPair(x,snd,this);
     ^set_snd:
      this[2]>>x;
      InstPair(fst,x,this);
     ^end:
    }
  )
  InstPair(fst,snd,s);
)
|
// Testing pair
p=new pair(2 of 2);
p&lt;&lt;Int;
p&lt;&lt;String;
p[1]&lt;&lt;5&lt;&lt;"five";
p[1]&lt;&lt;^get_fst;
p[1]>>p_fst;
p[1]&lt;&lt;^set_snd&lt;&lt;"six"&lt;&lt;^get_snd;
p[1]>>p_snd;
c=new console(2 of 2);
c[1]&lt;&lt;^str&lt;&lt;"p=("&lt;&lt;^int&lt;&lt;p_fst&lt;&lt;^str&lt;&lt;","&lt;&lt;^str&lt;&lt;p_snd&lt;&lt;^str&lt;&lt;")"&lt;&lt;^nl&lt;&lt;^end;
// Remember to delete objects, or type checker will tell you!
p[1]&lt;&lt;^end;
</pre>
#### Output
<pre>
>./3.1-pair
p=(5,six)
</pre>
## 4 Structs
### 4.1 Pairs
#### Source code
<pre>
// Same as tutorial 3.1, but using struct from library
#include&lt;console.pi>
#include&lt;struct/pair.pi>
// Testing pair
p=new pair(2 of 2);
p&lt;&lt;Int;
p&lt;&lt;String;
p[1]&lt;&lt;5&lt;&lt;"five";
p[1]&lt;&lt;^get_fst;
p[1]>>p_fst;
p[1]&lt;&lt;^set_snd&lt;&lt;"six"&lt;&lt;^get_snd;
p[1]>>p_snd;
c=new console(2 of 2);
c[1]&lt;&lt;^str&lt;&lt;"p=("&lt;&lt;^int&lt;&lt;p_fst&lt;&lt;^str&lt;&lt;","&lt;&lt;^str&lt;&lt;p_snd&lt;&lt;^str&lt;&lt;")"&lt;&lt;^nl&lt;&lt;^end;
// Remember to delete objects, or type checker will tell you!
p[1]&lt;&lt;^end;
</pre>
#### Output
<pre>
>./4.1-pairs
p=(5,six)
</pre>
### 4.2 Lists
#### Source code
<pre>
#include&lt;console.pi>
#include&lt;struct/list.pi>
#include&lt;math/fib.pi>
#include&lt;convert.pi>
#include&lt;succ.pi>

// printlist: Print a list
#define $printlist \
  2:#elt; \
  2->1:$convert(#elt,String)(1pure,2); \
  2->1:$list(#elt)(2 of 1 pure,2); \
  $end;
global $printlist printlist(1,2);
local service PrintList(printlist (1 of 2) this) // {{{
( this>>#elt;
  this[2]>>tostring;
  this[2]>>l;
  local Print($list(#elt)(2 of 1 pure,2) elts, $console(2 of 1,2) c, Bool first)
  ( elts[1]&lt;&lt;^pop;
    elts[1]>>
    {^nil:
      c[1]&lt;&lt;^str&lt;&lt;"]"&lt;&lt;^end;
     ^cons:
      elts[1]>>e>>rest;
      ts=new tostring(2 of 2);
      ts[1]&lt;&lt;e;
      ts[1]>>
      {^Error:
        ts[1]>>err;
        c[1]&lt;&lt;^str&lt;&lt;",&lt;Error:"&lt;&lt;^str&lt;&lt;err&lt;&lt;^str&lt;&lt;">";
        Print(rest,c,false);
       ^Ok:
        ts[1]>>estr;
        c[1]&lt;&lt;^str&lt;&lt;(if first then "" else ",")&lt;&lt;^str&lt;&lt;estr;
        Print(rest,c,false);
      }
    }
  )
  c = new console(2 of 2);
  c[1]&lt;&lt;^str&lt;&lt;"[";
  Print(l,c,true);
)
|
// Working with lists
ilist=new range(2 of 2);
ilist&lt;&lt;Int;
isucc=new stdsucc_Int(2 of 2);
iorder=new stdorder_Int(2 of 2);
ilist[1]&lt;&lt;isucc&lt;&lt;iorder&lt;&lt;0&lt;&lt;20; // ilist is now the range [1;20]
ilist[1]&lt;&lt;^apply&lt;&lt;fib;          // ilist is now contains the 20 first fibonacci numbers
// Print list
print=new printlist(2 of 2);
print&lt;&lt;Int;
print[1]&lt;&lt;int2string&lt;&lt;ilist;
</pre>
#### Output
<pre>
>./4.2-lists
[1,1,2,3,5,8,13,21,34,55,89,144,233,377,610,987,1597,2584,4181,6765,10946]</pre>
## 5 Standard libraries
### 5.1 Args
#### Source code
<pre>
// Example 3.1 - Introducing args service
// Iterates over program arguments

#include &lt;console.pi>
c = new console(2 of 2);
// Include sys servicee
#include &lt;sys/args.pi>
// Connect to sys service
s = new args(2 of 2);

// Obtain and print the number of program arguments
s[1]&lt;&lt;^count;
s[1]>>argc;
c[1]&lt;&lt;^str&lt;&lt;"Number of args is "&lt;&lt;^int&lt;&lt;argc&lt;&lt;^nl;
// Method taking open sessions as arguments
// Method prints program args i, i+1, ..., argc-1
local PrintArgs(Int i, $console(2 of 1,2) c, $args(2 of 1 pure,2) s)
( s[1]&lt;&lt;^arg&lt;&lt;i;
  s[1]>>
  {^some: s[1]>>a;
          c[1]&lt;&lt;^str&lt;&lt;a&lt;&lt;^nl;
          PrintArgs(i+1,c,s);
   ^none: s[1]&lt;&lt;^end;
          c[1]&lt;&lt;^end;
  }
)
PrintArgs(0,c,s); // Print all args
</pre>
#### Output
<pre>
>./5.1-args arg
Number of args is 2
./5.1-args
arg
</pre>
### 5.2 Timer
#### Source code
<pre>
// Example 3.2 - Introducing timer service
// Process sleeping and timing.

#include &lt;console.pi>
// Include timer service
#include &lt;sys/timer.pi>
c = new console(2 of 2);
// Connect to timer service
t = new timer(2 of 2);

// Start message
c[1]&lt;&lt;^str&lt;&lt;"Waiting 5 seconds"&lt;&lt;^nl;
// Wait 5 secs
t[1]&lt;&lt;^wait&lt;&lt;5.0;
t[1]>>x;
// Read timer (should be roughly 5 seconds)
t[1]&lt;&lt;^read;
t[1]>>secs;
// Print result
c[1]&lt;&lt;^str&lt;&lt;"Timer says the wait took "&lt;&lt;^flt&lt;&lt;secs&lt;&lt;^str&lt;&lt;" seconds."&lt;&lt;^nl;
// Clean up
t[1]&lt;&lt;^end;
c[1]&lt;&lt;^end;
</pre>
#### Output
<pre>
>./5.2-timer
Waiting 5 seconds
Timer says the wait took 5.0 seconds.
</pre>
### 5.3 File
#### Source code
<pre>
// Example 3.3 - Introducing the file service
// Read write and delete local files
#include &lt;console.pi>
// Include file service
#include &lt;sys/file.pi>

// Error handling
local FError(String s, $console(2 of 1,2) c, 1->2:String;$end;(2 of 1,2) f)
( f[1]>>e;
  c[1]&lt;&lt;^str&lt;&lt;s&lt;&lt;^str&lt;&lt;": "&lt;&lt;^str&lt;&lt;e&lt;&lt;^nl&lt;&lt;^end;
)
 
c = new console(2 of 2);
// Create file with content: Hello World
f = new file(2 of 2);
f[1]&lt;&lt;^Create;
f[1]&lt;&lt;"test.txt";
f[1]>>
{^Error:
  FError("Write Error:",c,f);
 ^OK:
  f[1]&lt;&lt;^Write&lt;&lt;"Hello World"&lt;&lt;^Close;
  c[1]&lt;&lt;^str&lt;&lt;"File written with content: Hello World"&lt;&lt;^nl;
  // Open created file, and read content
  f = new file(2 of 2);
  f[1]&lt;&lt;^Read&lt;&lt;"test.txt";
  f[1]>>
  {^Error:
    FError("Read Error:",c,f);
   ^OK:
    f[1]&lt;&lt;^All;
    f[1]>>s;
    c[1]&lt;&lt;^str&lt;&lt;"Read content: "&lt;&lt;^str&lt;&lt;s&lt;&lt;^nl;
    // Remove file
    f = new file(2 of 2);
    f[1]&lt;&lt;^Remove&lt;&lt;"file.txt";
    f[1]>>
    {^Error:
      FError("Remove Error:",c,f);
     ^OK:
      c[1]&lt;&lt;^str&lt;&lt;"Removed file"&lt;&lt;^nl&lt;&lt;^end;
    }
  }
}
</pre>
#### Output
<pre>
>./5.3-file
File written with content: Hello World
Read content: Hello World
Remove Error:: Not implemented
</pre>
### 5.4 Convert
#### Source code
<pre>
// Exampe 3.4 - Introduconig the convert service
// Converting between string and int

#include &lt;console.pi>
// Include convertion services like string2int
#include &lt;convert.pi>

c=new console(2 of 2);
// Connect to string2int service
s=new string2int(2 of 2);
s[1]&lt;&lt;"123";
s[1]>>
{^Error:
  s[1]>>e;
  c[1]&lt;&lt;^str&lt;&lt;"Error: "&lt;&lt;^str&lt;&lt;e&lt;&lt;^nl&lt;&lt;^end;
 ^Ok:
  s[1]>>n;
  c[1]&lt;&lt;^str&lt;&lt;"The result of converting 123 to an integer is: "&lt;&lt;^int&lt;&lt;n&lt;&lt;^nl&lt;&lt;^end;
}
</pre>
#### Output
<pre>
>./5.4-convert
The result of converting 123 to an integer is: 123
</pre>
## 6 Purity
### 6.1 Purity
#### Source code
<pre>
// Example 6.1 - Introducing notion of purity
// Pure methods and services separate functionality by guaranteeing not to
// change the imparative state of other processes and services.
// This is useful in testing and debugging, and allows automatic optimization
// of concurrency while providing strong semantics preservation properties.
// We give examples of pure and impure services and methods.

#include &lt;console.pi>

// Example 1: Counter service
// Not pure, because different connections will get different answers from service
#define $state \
  1->2:Int; \
  $end;
global $state counter(1,2);
local Counter(Int i)
( global s=new counter(1 of 2);
  Counter(i+1);
  |
  s[2]&lt;&lt;i;
)
Counter(1);
|
// Example 2: Random service
// Not pure, because different connections may get different "version" of service
#define $rand \
  1->2:Bool; \
  $end;
global $rand random(1,2);
local Random1()
( global s=new random(1 of 2);
  Random1();
  |
  s[2]&lt;&lt;true;
)
Random1();
|
local Random2()
( global s=new random(1 of 2);
  Random2();
  |
  s[2]&lt;&lt;false;
)
Random2();
|
// Example 3: Inherited impurity and host language impurity
// Not pure, because using impure services
#define $fun \
  2->1:Int; \
  1->2:Int; \
  $end;
HOSTHEADER("#include &lt;iostream>");
global $fun fun(1,2);
local Fun()
( global s=new fun(1 of 2);
  Fun();
  |
  s[2]>>x;
  c=new counter(2 of 2); //! Inharited impurity
  c[1]>>val;
  HOST("std::cout &lt;&lt; ((libpi::String*)", val, ".get())->GetValue() &lt;&lt; std::endl;"); // HOST impurity
  s[2]&lt;&lt;5*x*x-3*x;
)
Fun();
|
// Example 4: Pure service
global $fun fact(1 pure,2); // Notice pure declaration of 1st participant
local pure Fact()           // Notice pure declaration of method
( global s=new fact(1 of 2);
  Fact();
  |
  s[2]>>n;
  // Can use pure services (itself or others)
  f=new fact(2 of 2);
  f[1]&lt;&lt;n-1;
  f[1]>>res;
  // Can use HOST statements, but must be 'pure'
  HOST(";") pure; // Notice pure declaration
  s[2]&lt;&lt;res*n;
)
local pure StartFact(Int n)
( if n&lt;=0
  then Fact();
  else (Fact(); | StartFact(n-1);)
)
StartFact(1);
|
// The following tutorials will show examples where purity is used to obtain
// highly (and customizably) concurrent programs from sequentially defined
// processes.
</pre>
#### Output
<pre>
>./6.1-purity
</pre>
### 6.2 Fib
#### Source code
<pre>
// Example 6.2 - Calculating fibonacchi numbers
// Utilizinf pure services to automatically obtain concurrency
// Include console functionality like printing
#include &lt;console.pi>

// Define api for service
#define $fib \
  2->1:Int; \
  1->2:Int; \
  $end;

// Declare service
global $fib fib(1 pure,2);

// Implement service
local pure Fib()
( global s=new fib(1 of 2);              // Wait for client / invocation
  Fib();                                 // Maintain number of processes waiting for clients
  |                                      // Start process to serve the request
  s[2]>>n;                               // Receive n from client
  if n&lt;=1
  then s[2]&lt;&lt;1;                          // Simple case
  else f1=new fib(2 of 2);               // Recursive case
       f1[1]&lt;&lt;n-1;
       f1[1]>>r1;                        // r1 = fib(n-1)
       f2=new fib(2 of 2);
       f2[1]&lt;&lt;n-2;
       f2[1]>>r2;                        // r2 = fib(n-2)
       s[2]&lt;&lt;r1+r2;                      // return r1+r2
)
local pure StartFib(Int n)               // Start n copies of service
( if n&lt;=0
  then Fib();
  else ( Fib(); | StartFib(n-1); )
)
StartFib(SYSTEM & "tprocs");             // Start as many processes as the target number of active processes
|

// Main program
c=new console(2 of 2);                   // Get access to console
c[1]&lt;&lt;^str&lt;&lt;"Input n: "&lt;&lt;^input&lt;&lt;^int;   // Ask for n
c[1]>>n;                                 // Read n
c[1]&lt;&lt;^end;                              // Release access to console
s=new fib(2 of 2);                       // Call fib(n)
s[1]&lt;&lt;n;
s[1]>>f;                                 // Get result
c=new console(2 of 2);
c[1]&lt;&lt;^str&lt;&lt;"Fib(n)="&lt;&lt;^int&lt;&lt;f&lt;&lt;^nl;     // Print result
c[1]&lt;&lt;^end;                              // Release access to console
</pre>
#### Output
<pre>
>echo 20 | ./6.2-fib
Input n: Fib(n)=10946
</pre>
### 6.3 Fib Time
#### Source code
<pre>
// Example 6.3 - Calculating fibonacchi numbers, and measuring the time used
// Utilizing pure services to automatically obtain concurrency
// Avoid optimization corrupting time measurements
// Include console functionality like printing
#include &lt;console.pi>
// Include timer functionality like measuring time spent
#include &lt;sys/timer.pi>

// Macro that pretends to use n in an impure way.
// This stops impure actions, and actions using n
// to be moved across here,
// Ensuring that n is available before any of the
// impure actions below this one is performed.
#define require(n) HOST("/* ",n," */")

// Define api for service
#define $fib \
  2->1:Int; \
  1->2:Int; \
  $end;

// Declare service
global $fib fib(1 pure,2);

// Implement service
local pure Fib()
( global s=new fib(1 of 2);              // Wait for client / invocation
  Fib();                                 // Maintain number of processes waiting for clients
  |                                      // Start process to serve the request
  s[2]>>n;                               // Receive n from client
  if n&lt;=1
  then s[2]&lt;&lt;1;                          // Simple case
  else f1=new fib(2 of 2);               // Recursive case
       f1[1]&lt;&lt;n-1;
       f1[1]>>r1;                        // r1 = fib(n-1)
       f2=new fib(2 of 2);
       f2[1]&lt;&lt;n-2;
       f2[1]>>r2;                        // r2 = fib(n-2)
       s[2]&lt;&lt;r1+r2;                      // return r1+r2
)
local pure StartFib(Int n)
( if n&lt;=0
  then Fib();
  else ( Fib(); | StartFib(n-1); )
)
StartFib(SYSTEM & "tprocs");             // Start as many processes as the target number of active processes
|

// Main program
c=new console(2 of 2);                   // Get access to console
c[1]&lt;&lt;^str&lt;&lt;"Input n: "&lt;&lt;^input&lt;&lt;^int;   // Ask for n
c[1]>>n;                                 // Read n
c[1]&lt;&lt;^end;                              // Release access to console

t=new timer(2 of 2);                     // Start timer
s=new fib(2 of 2);                       // Call fib(n)
s[1]&lt;&lt;n;
s[1]>>f;                                 // Get result
t[1]&lt;&lt;^sync;t&lt;&lt;Int;t[1]&lt;&lt;f;              // Make timer wait for result (avoids optimizing timer to not wait for result)
//require(f);                            // A more generic method to avoid optimizing timer, but also avoids other optimizations
t[1]&lt;&lt;^read;                             // Get used time
t[1]>>secs;                              // secs = used time
t[1]&lt;&lt;^end;                              // Stop / release timer
c=new console(2 of 2);
c[1]&lt;&lt;^str&lt;&lt;"Fib(n)="&lt;&lt;^int&lt;&lt;f&lt;&lt;^nl      // Print result
    &lt;&lt;^str&lt;&lt;"Time: "&lt;&lt;^flt&lt;&lt;secs&lt;&lt;^nl    // Print time used
    &lt;&lt;^end;
</pre>
#### Output
<pre>
>echo 20 | ./6.3-fib_time
Input n: Fib(n)=10946
Time: 0.1979999542236328125
</pre>
### 6.4 Fact
#### Source code
<pre>
// Example 6.4 - Factorial service
// Implement pure prodseq service, and use to compute factorial.

#include &lt;console.pi>

#define $prodseq \
  2->1:Int; \
  2->1:Int; \
  1->2:Int; \
  $end;

global $prodseq prodseq(1 pure ,2);
local pure service Prodseq(prodseq (1 of 2) s)
( s[2]>>x1;
  s[2]>>x2;
  if x2+1 &lt;= x1
  then s[2]&lt;&lt;1;
  else if x2 &lt;= x1
  then s[2]&lt;&lt;x1;
  else s1 = new prodseq(2 of 2);
       Int mid=(x1+x2)/2;
       s1[1]&lt;&lt;x1;
       s1[1]&lt;&lt;mid;
       s1[1]>>r1;
       s2 = new prodseq(2 of 2);
       s2[1]&lt;&lt;mid+1;
       s2[1]&lt;&lt;x2;
       s2[1]>>r2;
       s[2]&lt;&lt;r1*r2;
)
|
c = new console(2 of 2);
c[1] &lt;&lt; ^str &lt;&lt; "What is n? "
     &lt;&lt; ^input&lt;&lt;^int;
c[1] >> n;
s = new prodseq(2 of 2);
s[1] &lt;&lt; 1 &lt;&lt; n;
s[1] >> r;
c[1] &lt;&lt; ^str &lt;&lt; "Fact of n is"
     &lt;&lt; ^int &lt;&lt; r &lt;&lt; ^nl
     &lt;&lt; ^end;
</pre>
#### Output
<pre>
>echo 20 | ./6.4-fact
What is n? Fact of n is2432902008176640000
</pre>
## 7 Extras
### 7.1 Assertions
#### Source code
<pre>
// Example 7.1 - Assertions
// Create session with assertions, and observe the sent messages respect them
// - trying to change program to break them would result in typechecking errors.

#include &lt;console.pi>

// Define protocol $type as three booleans with propositional assertions
#define $type \
  1->2:Bool as x; \
  1->2:Bool as y where x or y; \
  2->1:Bool as z where z; \
  $end;

// Create channel a with 2 participants using protocol $type
global $type a(1,2);

c=new console(2 of 2);
c[1]&lt;&lt;^str&lt;&lt;"Input bool x: "&lt;&lt;^input&lt;&lt;^str;
c[1]>>x_str;
Bool x=(x_str="true");
c[1]&lt;&lt;^end;
s=new a(1 of 2);
s[2]&lt;&lt;x;
s[2]&lt;&lt;not x;
s[2]>>z;
c=new console(2 of 2);
c[1]&lt;&lt;^str&lt;&lt;"We know that z is true - it has been proven!"&lt;&lt;^nl
    &lt;&lt;^str&lt;&lt;"z="&lt;&lt;^str&lt;&lt;(if z then "true" else "false")&lt;&lt;^nl
    &lt;&lt;^end;

|
s=new a(2 of 2);
s[1]>>q1;
s[1]>>q2;
// Type checking verifies that this message will always fulfill the assertion
// from the used protocol, and hence q1 or q2 will always evaluate to true.
s[1]&lt;&lt;q1 or q2;
</pre>
#### Output
<pre>
>echo true | ./7.1-assertions
Input bool x: We know that z is true - it has been proven!
z=true
</pre>
### 7.2 Host
#### Source code
<pre>
// Example 7.2 - The HOST and HOSTHEADER statements.
// Hoists the full power of the host language (c++), mixing hapi and host
// language statements and data.

// HOSTHEADER is added to generated c++ code header. Even though HOSTHEADER
// statement may be 'executed' many times, the header is only added once.
HOSTHEADER("#include &lt;iostream>");
// Use assorted types of values
Int x1=1;
String x2="2";
Bool x3=true;
Int x4=4;
String x5="5";
Bool x6=false;
Int x7=7;
String x8="8";
Bool x9=true;
( // Simple printing, although be aware the pitfalls of using raw c++ io
  // ie no coordination with other outputting processes.
  HOST("std::cout &lt;&lt; ((libpi::Int*)", x1, ".get())->ToString() &lt;&lt; std::flush &lt;&lt; ((libpi::String*)", x2, ".get())->GetValue() &lt;&lt; std::flush &lt;&lt; ((libpi::Bool*)", x3, ".get())->ToString() &lt;&lt; std::endl;");
| HOST("std::cout &lt;&lt; ((libpi::Int*)", x4, ".get())->ToString() &lt;&lt; std::flush &lt;&lt; ((libpi::String*)", x5, ".get())->GetValue() &lt;&lt; std::flush &lt;&lt; ((libpi::Bool*)", x6, ".get())->ToString() &lt;&lt; std::endl;");
| HOST("std::cout &lt;&lt; ((libpi::Int*)", x7, ".get())->ToString() &lt;&lt; std::flush &lt;&lt; ((libpi::String*)", x8, ".get())->GetValue() &lt;&lt; std::flush &lt;&lt; ((libpi::Bool*)", x9, ".get())->ToString() &lt;&lt; std::endl;");
| // Use host language (c++) functionality in output
  HOST("std::cout &lt;&lt; (((libpi::Bool*)", x3, ".get())->GetValue()?((libpi::Int*)", x1, ".get())->ToString():((libpi::String*)", x2, ".get())->GetValue()) &lt;&lt; std::endl;");
| HOST("std::cout &lt;&lt; (((libpi::Bool*)", x6, ".get())->GetValue()?((libpi::Int*)", x4, ".get())->ToString():((libpi::String*)", x5, ".get())->GetValue()) &lt;&lt; std::endl;");
| HOST("std::cout &lt;&lt; (((libpi::Bool*)", x9, ".get())->GetValue()?((libpi::Int*)", x7, ".get())->ToString():((libpi::String*)", x8, ".get())->GetValue()) &lt;&lt; std::endl;");
)
| // Retrieve/import values from host language to hapi process
  String str1="Hello";
  String space=" ";
  String str2="World";
  String res="";
  HOST("", res, ".reset(new libpi::String(((libpi::String*)", str1, ".get())->GetValue() + ((libpi::String*)", space, ".get())->GetValue() + ((libpi::String*)", str2, ".get())->GetValue()));");
  HOST("std::cout &lt;&lt; ((libpi::String*)", res, ".get())->GetValue() &lt;&lt; std::endl;"); // Outputs "Hello World"
</pre>
#### Output
<pre>
>./7.2-host
Hello World1
2true
45false
78true
1
5
7
</pre>
