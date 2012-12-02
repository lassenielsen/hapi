(nu a : 1=>2<Int>;2=>1<Int>;Gend)          // Fact Server Protocol
( link(2,a,s,2);                           // Receive Connection
  s[1] >> x;                               // Receive argument
  def Fact(n: Int, acc: Int,               // Define Fact Function
           dest: 1<<<Int>;Lend@(2of2)) =
      if n=1                               // If simple case
      then dest[1] << acc;end              // Send accumulated value
      else Fact(n-1, acc*n, dest)          // Else make recursive call
  in Fact(x,1,s)                           // Call Fact Function
| (nu gui: {^Fact: {^Quit: Gend} })        // User Interface Behavior
  ( link(1,gui,user,1);                    // Create User Interface
    guisync(1,user,1)                      // Wait for User Input
    {^Fact(n: Int=5):                      // Request n
      link(2,a,s,1);                       // Connect to Fact Server
      s[2] << n;                           // Send n
      s[2] >> result;                      // Receive n!
      guivalue(1,user,1,"Fact:",result);   // Add result to User Interface
      guisync(1,user,1)                    // Wait for user to quit
      {^Quit(comment: String=""): end }
    }
) )
