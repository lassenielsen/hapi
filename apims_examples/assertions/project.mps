define $cpg =
1=>2<Bool> as x;
1=>3<Bool> as y[[(x or not y) and (not x or y)]];
2=>3<Bool> as z[[(x and y) or z]];
Gend
in
(nu a : $cpg)
( link(3,a,s,2);
  s[1]>>x1;
  if x1
  then s[3]<<false;
       end
  else s[3]<<true;
       end
)
