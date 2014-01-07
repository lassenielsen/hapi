(nu a : 1=>2<Int>;2=>1<Int>;Gend)       // Fib Server Protocol
( def Fib() =                           // Define Fib Server
      link(2,a,s,2);                    // Receive Connection
      ( Fib()                           // Start new server
      | s[1] >> x;                      // Receive argument
        if (x=0) or (x=1)               // If simple case
        then s[1] << 1;end              // Return directly
        else link(2,a,t1,1);            // Connect to (another) Fib Server
             link(2,a,t2,1);            // Connect to (a third) Fib Server
             t1[2]<<x-1;                // Send x-1 to second server
             t2[2]<<x-2;                // Send x-2 to third server
             t1[2]>>y1;                 // Receive fib(n-1)
             t2[2]>>y2;                 // Receive fib(n-2)
             s[1]<<y1+y2;               // Send result: fib(n-1)+fib(n-2)
             end
      )
  in Fib()
| (nu gui: {^Fib: {^Quit: Gend} })      // User Interface Behavior
  ( link(1,gui,user,1);                 // Create User Interface
    guisync(1,user,1)                   // Wait for User Input
    {^Fib(n: Int=5):                    // Request n
      link(2,a,s,1);                    // Connect to Fib Server
      s[2] << n;                        // Send n
      s[2] >> result;                   // Receive fib(n)
      guivalue(1,user,1,"Fib:",result); // Add result to User Interface
      guisync(1,user,1)                 // Wait for user to quit
      {^Quit(comment:String=""): end }
    }
) )
