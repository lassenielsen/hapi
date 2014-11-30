(nu a: 1=>2<Bool> as x;
       1=>2<Bool> as y;
       2=>1
       {^A[[x or y]]: Gend,
        ^B[[not x or not y]]: Gend
       })
( link(2,a,s,2);
  s[1]>>q1;
  s[1]>>q2;
  if q1 and q2
  then s[1]<<^A;
       end
  else s[1]<<^B;
       end
)
