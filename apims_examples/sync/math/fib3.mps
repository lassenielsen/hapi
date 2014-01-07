(nu cont : 1=>2<Int>;                   // Fib Continuation Server Protocol
           1=>2<Int>;
           1=>2<Int>;
           1=>2<1<<<Int>;Lend@(2 of 2)>;
           Gend)
(nu fib : 1=>2<Int>;                    // Fib Server Protocol
          2=>1<Int>;
          Gend)
( def FibCont() =
      link(2,cont,s,2);                 // Receive Connection
      ( FibCont()                       // Start new Server
      | s[1]>>n;                        // Receive argument
        s[1]>>x1;                       // Receive accumulated value 1
        s[1]>>x2;                       // Receive accumulated value 2
        s[1]>>client@(2 of 2);          // Receive client connection
        if n=0
        then client[1]<<x1; end
        else link(2,cont,t,1);
             t[2]<<n-1;
             t[2]<<x2;
             t[2]<<x1+x2;
             t[2]<<client;
             end
      ) in FibCont()
| def Fib() =                           // Define Fib Server
      link(2,fib,client,2);             // Receive Connection
      ( Fib()                           // Start new server
      | client[1]>>n;                   // Receive argument
        link(2,cont,t,1);               // Connect to FibCont Server
        t[2]<<n;                        // Send x-1 to second server
        t[2]<<1;                        // Send x-2 to third server
        t[2]<<1;                        // Receive fib(n-1)
        t[2]<<client;                   // Receive fib(n-2)
        end
      )
  in Fib()                              // Start Fib Server
| (nu gui: {^Fib: {^Quit: Gend} })      // User Interface Behavior
  ( link(1,gui,user,1);                 // Create User Interface
    guisync(1,user,1)                   // Wait for User Input
    {^Fib(n: Int=5):                    // Request n
      link(2,fib,s,1);                  // Connect to Fib Server
      s[2] << n;                        // Send n
      s[2] >> result;                   // Receive fib(n)
      guivalue(1,user,1,"Fib:",result); // Add result to User Interface
      guisync(1,user,1)                 // Wait for user to quit
      {^Quit(comment: String=""): end }
    }
  )
)
