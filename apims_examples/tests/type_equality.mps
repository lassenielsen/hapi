(nu a: rec $z.1=>2<String>;1=>2<String>;$z)
( def X(x: rec %x. 1<<<String>;1<<<String>;%x@(1of2)) =
    x[2]<<"ping";
    X(x)
  in link(2,a,s,1);
     X(s)
| def X(x: rec %x. 1>><String>;1>><String>;%x@(2of2)) =
    x[1]>> text;
    X(x)
  in link(2,a,s,2);
     X(s)
)
