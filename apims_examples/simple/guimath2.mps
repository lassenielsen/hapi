(nu fact : 1=>2:1<Int>;2=>1:2<Int>;Gend)  // Fact Server protocol
(nu fib  : 1=>2:1<Int>;2=>1:2<Int>;Gend)  // Fib Server protocol
( (nu factcont : 1=>2:1<Int>;             // Fact Continuation protocol
                 1=>2:1<Int>;
                 1=>2:1<2<<<Int>;Lend@(2 of 2)>;
                 Gend)
  ( def FactCont() =                      // Fact Continuation Server
        link(2,factcont,s,2);                 // Receive Connection
        ( FactCont()                      // Start new Server
        | s[1] >> x;                      // Receive argument (x)
          s[1] >> factor;                 // Receive (x+1)*...*n
          s[1] >> client;                 // Receive Client Session
          if x=1                          // If simple case
          then client[2] << factor; end   // Return directly
          else link(2,factcont,q,1);          // Connect to (another) Cont Server
               q[1] << x-1;               // Send x-1
               q[1] << x*factor;          // Send x*(x+1)*...*n
               q[1] << client;            // Send Client Session
               end
        ) in FactCont()                   // Start Continuation Server
  | def Fact() =                          // Define Fact Server
        link(2,fact,s,2);                 // Receive Connection
        ( Fact()                          // Start new Server
        | link(2,factcont,t,1);               // Connect to Cont. Server
          s[1]>>n;                        // Receive n
          t[1]<<n;t[1]<<1;t[1]<<s;        // Delegate to Cont
          end
        ) in Fact()                       // Start Fact Server
  )
| (nu cont : 1=>2:1<Int>;                 // Fib Continuation Server Protocol
             1=>2:1<Int>;
             1=>2:1<Int>;
             1=>2:1<2<<<Int>;Lend@(2 of 2)>;
             Gend)
  ( def FibCont() =
        link(2,cont,s,2);                 // Receive Connection
        ( FibCont()                       // Start new Server
        | s[1]>>n;                        // Receive argument
          s[1]>>x1;                       // Receive accumulated value 1
          s[1]>>x2;                       // Receive accumulated value 2
          s[1]>>client;                   // Receive client connection
          if n=0
          then client[2]<<x1; end
          else link(2,cont,t,1);
               t[1]<<n-1;
               t[1]<<x2;
               t[1]<<x1+x2;
               t[1]<<client;
               end
        ) in FibCont()
  | def Fib() =                           // Define Fib Server
        link(2,fib,client,2);             // Receive Connection
        ( Fib()                           // Start new server
        | client[1]>>n;                   // Receive argument
          link(2,cont,t,1);               // Connect to FibCont Server
          t[1]<<n;                        // Send x-1 to second server
          t[1]<<1;                        // Send x-2 to third server
          t[1]<<1;                        // Receive fib(n-1)
          t[1]<<client;                   // Receive fib(n-2)
          end
        )
    in Fib()                              // Start Fib Server
  )
| (nu gui: rec $x.{#Fact: $x,
                   #Fib: $x,
                   ^Quit: Gend})          // User Interface Behavior
  ( link(1,gui,user,1);                   // Create User Interface
    def X(u : rec %x.{#Fact: %x,
                      #Fib: %x,
                      ^Quit: Lend}@(1 of 1)) =
        guisync(1,u,1)                    // Wait for User Input
        {#Fact(n: Int=5):                 // Choice: Faculty
          link(2,fact,s,1);               // Connect to Fact Server
          s[1] << n;                      // Send n
          s[2] >> result;                 // Receive n!
          guivalue(1,u,1,"Fact:",result); // Add result to User Interface
          X(u),
         #Fib(n: Int=5):                  // Choice: Fibonacci
          link(2,fib,s,1);                // Connect to Fib Server
          s[1] << n;                      // Send n
          s[2] >> result;                 // Receive fib(n)
          guivalue(1,u,1,"Fib:",result);  // Add result to User Interface
          X(u),
         ^Quit(comment: String=""): end   // Quit session
        }
    in X(user)
) )

