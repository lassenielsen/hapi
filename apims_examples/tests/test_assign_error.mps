// Test assign
define $a =
  1=>2:1<Int>;
  Gend
in
(nu a : $a)
( link(2,a,s,2);
  s[1]>>x;
  link(2,a,t,1);
  t2 : $a@(1of2) = t;
  t[1]<<x;
  t2[1]<<x;
  end
| link(2,a,s,1);
  s[1]<<5;
  link(2,a,t,2);
  t[1]>>x;
  end
)

