(nu a: 1=>2<Bool> as x;
       1=>2<Bool> as y;
       2=>1<Bool> as z[[x or y or z]];
       Gend)
( link(2,a,s,2);
  s[1]>>q1;
  s[1]>>q2;
  if (not q1 and not q2)
  then s[1]<<true;
       end
  else s[1]<<false;
             end
)
