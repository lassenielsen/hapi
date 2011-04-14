(nu a: 1=>2:1<Bool> as x;
       1=>2:1<Bool> as y;
       2=>1:2
       {^A[[x or y]]: Gend,
        ^B[[not x or not y]]: Gend
       })
( link(2,a,s,2);
  s[1]>>q1;
  s[1]>>q2;
  if q1 and q2
  then s[2]<<^A;
       end
  else s[2]<<^B;
       end
)
