(nu cont : 1=>2<Int>;                   // Fib Continuation Server Protocol
           1=>2<Int>;
           1=>2<Int>;
           1=>2<1<<<Int>;Lend@(2 of 2)>;
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
| (nu gui: {^Fib: {^Quit: Gend} })      // User Interface Behavior
  ( link(1,gui,user,1);                 // Create User Interface
    guisync(1,user,1)                   // Wait for User Input
    {^Fib(n:Int=5):                     // Request n
      (nu result: 2=>1<Int>;Gend)       // Result protocol
      ( link(2,result,r,2);
        link(2,cont,s,1);               // Connect to Fib Server
        s[2]<<n;                        // Send n
        s[2]<<1;                        // Send fib(0)
        s[2]<<1;                        // Send fib(1)
        s[2]<<r; end                    // Send receiving session
      | link(2,result,r,1);
        r[2]>>result;                   // Receive fib(n)
        guivalue(1,user,1,"Fib",result);// Add result to User Interface
        guisync(1,user,1)               // Wait for user to quit
        {^Quit(comment:String=""): end }
      )
    }
  )
)
