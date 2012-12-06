(nu fact : 1=>2<Int>;2=>1<Int>;Gend)     // Fact Server protocol
(nu fib  : 1=>2<Int>;1=>2<Int>;          // Fib Server Protocol
           1=>2<Int>;2=>1<Int>;Gend)
( def Fact() =                           // Define Fact Server
      link(2,fact,s,2);                  // Receive Connection
      ( Fact()                           // Start new server
      | s[1] >> x;                       // Receive argument
        if x=1                           // If simple case
        then s[1] << 1;end               // Return directly
        else link(2,fact,t,1);           // Connect to (another) Fact Server
             t[2]<<x-1;                  // Send x-1
             t[2]>>y;                    // Receive (x-1)!
             s[1]<<y*x;                  // Send result (x-1)!*x
             end
      )
  in Fact()                              // Start Fact Server
| def Fib() =                            // Define Fib Server
      link(2,fib,s,2);                   // Receive Connection
      ( Fib()                            // Start new server
      | s[1] >> n;                       // Receive argument
        s[1] >> x1;                      // Receive argument
        s[1] >> x2;                      // Receive argument
        if (n=0)                         // If simple case
        then s[1] << x1;end              // Return directly
        else link(2,fib,t,1);            // Connect to (another) Fib Server
             t[2]<<n-1;                  // Send x-1 to second server
             t[2]<<x2;                   // Send x-2 to third server
             t[2]<<x1+x2;                // Receive fib(n-1)
             t[2]>>y;                    // Receive fib(n-2)
             s[1]<<y;                    // Send result: fib(n-1)+fib(n-2)
             end
      )
  in Fib()                              // Start Fib Server
| (nu gui: rec $x.{#Fact: $x,
                   #Fib: $x,
                   ^Quit: Gend})         // User Interface Behavior
  ( link(1,gui,user,1);                  // Create User Interface
    def X(u : rec %x.{#Fact: %x,
                      #Fib: %x,
                      ^Quit: Lend}@(1 of 1)) =
        guisync(1,u,1)                   // Wait for User Input
        {#Fact(n: Int=5):                // Call Fact(n)
          link(2,fact,s,1);              // Connect to Fact Server
          s[2] << n;                     // Send n
          s[2] >> result;                // Receive n!
          guivalue(1,u,1,"Fact:",result);// Add result to User Interface
          X(u),
         #Fib(n: Int=5):                 // Request n
          link(2,fib,s,1);               // Connect to Fib Server
          s[2] << n;                     // Send n
          s[2] << 1;                     // Send fib(0)
          s[2] << 1;                     // Send fib(1)
          s[2] >> result;                // Receive fib(n)
          guivalue(1,u,1,"Fib:",result); // Add result to User Interface
          X(u),
         ^Quit(comment: String=""): end  // Quit session
        }
    in X(user)
) )

