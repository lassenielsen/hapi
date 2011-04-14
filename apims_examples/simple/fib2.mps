(nu a : 1=>2:1<Int>;1=>2:1<Int>;        // Fib Server Protocol
        1=>2:1<Int>;2=>1:2<Int>;Gend)
( def Fib() =                           // Define Fib Server
      link(2,a,s,2);                    // Receive Connection
      ( Fib()                           // Start new server
      | s[1] >> n;                      // Receive argument
        s[1] >> x1;                     // Receive argument
        s[1] >> x2;                     // Receive argument
        if (n=0)                        // If simple case
        then s[2] << x1;end             // Return directly
        else link(2,a,t,1);             // Connect to (another) Fib Server
             t[1]<<n-1;                 // Send x-1 to second server
             t[1]<<x2;                  // Send x-2 to third server
             t[1]<<x1+x2;               // Receive fib(n-1)
             t[2]>>y;                   // Receive fib(n-2)
             s[2]<<y;                   // Send result: fib(n-1)+fib(n-2)
             end
      )
  in Fib()                              // Start Fib Server
| (nu gui: {^Fib: {^Quit: Gend} })      // User Interface Behavior
  ( link(1,gui,user,1);                 // Create User Interface
    guisync(1,user,1)                   // Wait for User Input
    {^Fib(n: Int=5):                    // Request n
      link(2,a,s,1);                    // Connect to Fib Server
      s[1] << n;                        // Send n
      s[1] << 1;                        // Send fib(0)
      s[1] << 1;                        // Send fib(1)
      s[2] >> result;                   // Receive fib(n)
      guivalue(1,user,1,"Fib:",result); // Add result to User Interface
      guisync(1,user,1)                 // Wait for user to quit
      {^Quit(comment: String=""): end }
    }
  )
)
