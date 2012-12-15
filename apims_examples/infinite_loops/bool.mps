// Test of bool expressions
(nu a : rec $x.1=>2<Bool>;$x)
(nu b : 1=>2<rec %x.2<<<Bool>;%x@(1of2)>;Gend)
( link(2,a,dest,1);
  link(2,b,s,1);
  s[2] << dest;
  end
| link(2,a,source,2);
  link(2,b,s,2);
  s[1] >> dest@(1of2);
  def X(s: rec %x.1>><Bool>;%x@(2of2),
        d: rec %x.2<<<Bool>;%x@(1of2),
        val: Bool) =
     d[2] << not val;
     s[1] >> newB;
     X(s,d,newB)
  in X(source,dest,true)
)
