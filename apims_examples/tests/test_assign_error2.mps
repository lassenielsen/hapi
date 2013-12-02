// Test assign
define $a =
  1=>2<Int>;
  Gend
in
(nu a : $a)
( link(2,a,s,1);
  s : Int = 5;
  s[1]<<5;
  end
| link(2,a,s,2);
  s[2]>>x;
  end
)

