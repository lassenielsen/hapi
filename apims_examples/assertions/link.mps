define $cpg =
1=>2:1<Bool> as x;
1=>3:2<Bool> as y[[(x or not y) and (not x or y)]];
3=>2:3<Bool> as z[[x or z]];
Gend
in
(nu a : $cpg)
( link(3,a,s,3);
  link(3,a,t,3);
  s[2]>>x1;
  t[2]>>x2;
  if x1 and x2
  then s[3]<<false;
       t[3]<<false;
       end
  else s[3]<<true;
       t[3]<<true;
       end
)
