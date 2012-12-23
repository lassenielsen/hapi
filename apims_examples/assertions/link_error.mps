define $cpg =
1=>2<Bool> as x;
1=>3<Bool> as y[[(x or not y) and (not x or y)]];
3=>2<Bool> as z[[x or z]];
Gend
in
(nu a : $cpg)
( link(3,a,s,3);
  link(3,a,t,3);
  s[1]>>x1;
  t[1]>>x2;
  if x1
  then s[2]<<false;
       t[2]<<false;
       end
  else s[2]<<true;
       t[2]<<true;
       end
)
