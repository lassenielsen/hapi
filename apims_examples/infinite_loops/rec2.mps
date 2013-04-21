define $stream =
  rec $stream.1=>2<String>;$stream
in
(nu a: $stream)
( def X(x: $stream@(1of2)) =
    x[2]<<"ping";
    X(x)
  in link(2,a,s,1);
     X(s)
| def X(x: $stream@(2of2)) =
    x[1]>> text;
    X(x)
  in link(2,a,s,2);
     link(2,a,t,2);
     (X(s) | X(t))
)
