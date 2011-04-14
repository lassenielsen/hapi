(nu a : 1=>2:1<Int>;2=>1:2<Int>;Gend)    // Fact Server protocol
( def Fact() =                           // Define Fact Server
      link(2,a,s,2);                     // Receive Connection
      ( Fact()                           // Start new server
      | s[1] >> x;                       // Receive argument
        if x=1                           // If simple case
        then s[2] << 1;end               // Return directly
        else link(2,a,t,1);              // Connect to (another) Fact Server
             t[1]<<x-1;                  // Send x-1
             t[2]>>y;                    // Receive (x-1)!
             s[2]<<y*x;                  // Send result (x-1)!*x
             end
      )
  in Fact()                              // Start Fact Server
| (nu gui: {^Fact: {^Quit: Gend} })      // User Interface Behavior
  ( link(1,gui,user,1);                  // Create User Interface
    guisync(1,user,1)                    // Wait for User Input
    {^Fact(n: Int=5):                    // Request n
      link(2,a,s,1);                     // Connect to Fact Server
      s[1] << n;                         // Send n
      s[2] >> result;                    // Receive n!
      guivalue(1,user,1,"Fact:",result); // Add result to User Interface
      guisync(1,user,1)                  // Wait for user to quit
      {^Quit(comment: String=""): end }
    }
) )
