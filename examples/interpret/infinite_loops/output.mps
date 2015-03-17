define $stringstream = // String-stream type
  rec $o.1=>2<String>;$o
in
(nu out: $stringstream)
(
  def Out()=
     link(2,out,s,2); // Create String-stream receiver
     ( (nu stop: Gend) // Dummy channel for making receiving process stuck
       link(2,stop,unused,1); // Make process stuck
       def X(x: $stringstream@(2of2)) = // For typing use the session
         x[1]>>text;
         X(x)
       in X(s)
     | Out() // Be ready to create more stream receivers
     )
  in Out()
| def W(dest: $stringstream@(1of2), v: String) = // Send the given string to the given String-stream
     dest[2] << v; // Send the string
     W(dest,v)
  in ( link(2,out,s,1); // Create one String-stream
       W(s,"Hi")
     | link(2,out,s,1); // Create another String-stream
       W(s,"Ho")
     )
)
