(nu prod : 1=>2:1<Int>;                  // Send argument: x1
           1=>2:1<Int>;                  // Send argument: x2
           2=>1:2<Int>;Gend)             // Send result: x1*...*x2
def Prod() =                             // Define SubFact Service {{{
    link(2,prod,s,2);                    // Receive Connection
    ( Prod()                             // Start new Server
    | s[1] >> x1;                        // Receive x1
      s[1] >> x2;                        // Receive x2
      if x1+1 <= x2                      // If not trivial case
      then link(2,prod,s1,1);            // Connect to (another) SubFact Server
           s1[1] << x1;                  // Send x1
           s1[1] << (x1+x2)/2;           // Send average of x1 and x2 (x3)
           link(2,prod,s2,1);            // Connect to (a third) SubFact Server
           s2[1] << (x1+x2)/2 +1;        // Send x3+1
           s2[1] << x2;                  // Send x2
           s1[2] >> fact1;               // Receive x1*...*x3
           s2[2] >> fact2;               // Receive x3+1*...*x2
           s[2] << fact1*fact2;          // Send x1*...*x3*(x3+1)*...*x2
           end
      else if x1 = x2                    // If trivial (x1=x2)
      then s[2] << x1;                   // Send x1 (=x2)
           end
      else s[2] << 1;                    // Send 1 (if x1>x2)
           end
    )
in ( Prod() |                            // Start Prod Server }}}
(nu gui: {^Fact: {^Quit: Gend} })        // User Interface Behavior
( link(1,gui,user,1);                    // Create User Interface
  guisync(1,user,1)                      // Wait for User Input
  {^Fact(n: Int=5):                      // Request n
    link(2,prod,s,1);                    // Connect to Fact Server
    s[1] << 1;                           // Send 1
    s[1] << n;                           // Send n
    s[2] >> result;                      // Receive 1*...*n
    guivalue(1,user,1,"Fact:",result);   // Add result to User Interface
    guisync(1,user,1)                    // Wait for user to quit
    {^Quit(comment: String=""): end }
  }
) )
