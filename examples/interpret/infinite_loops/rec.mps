define $sstream =
  rec $sstream.1=>2<String>;$sstream
in
def X(x: $sstream@(1of2),
      y: $sstream@(2of2),
      s: String) =
  x[2]<<s;
  y[1]>>newS;
  X(x,y,newS)
in (nu a: $sstream)
   (nu b: 1=>2<$sstream@(1of2)>; Gend)
   ( link(2,a,s1,2);
     link(2,b,t,2);
     t[1]>>s2@(1of2);
     X(s2,s1,"Hello World")
   | link(2,a,s,1);
     link(2,b,t,1);
     t[2] << s;
     end
   )
