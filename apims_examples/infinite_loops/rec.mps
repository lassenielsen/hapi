def X(x: rec %x. 1<< <String>;%x@(1of2),
      y: rec %y. 1>> <String>;%y@(2of2),
      s: String) =
  x[1]<<s;
  y[1]>>newS;
  X(x,y,newS)
in (nu a: rec $z.1=>2:1<String>;$z)
   (nu b: 1=>2:1 <rec %v. 1 << <String>; %v@(1 of 2) >; Gend)
   ( link(2,a,s1,2);
     link(2,b,t,2);
     t[1] >> s2;
     X(s2,s1,"Hello World")
   | link(2,a,s,1);
     link(2,b,t,1);
     t[1] << s;
     end
   )
