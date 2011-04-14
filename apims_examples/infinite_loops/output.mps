(nu out: rec $o.1=>2:1<String>;$o) // String-stream type
(
  (nu stop: Gend) // Dummy channel for making receiving process stuck
  def Out(z:Int)= // Definitions must have at least one argument
     link(2,out,s,2); // Create String-stream receiver
     ( link(2,stop,unused,1); // Make process stuck
       def X(x: rec %o.1>><String>;%o@(2of2)) = // For typing use the session
          x[1]>>text;
          X(x)
       in X(s)
     | Out(z) // Be ready to create more stream receivers
     )
  in Out(1)
| def W(dest: rec %i.1<<<String>;%i@(1of2), v: String) = // Send the given string to the given String-stream
     dest[1] << v; // Send the string
     W(dest,v)
  in ( link(2,out,s,1); // Create one String-stream
       W(s,"Hi")
     | link(2,out,s,1); // Create another String-stream
       W(s,"Ho")
     )
)
