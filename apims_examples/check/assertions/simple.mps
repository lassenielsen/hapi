(nu a: 1=>2<Bool> as x;
       1=>2<Bool> as y[[x or y]];
       2=>1<Bool> as z[[z]];
       Gend)
( link(2,a,s,2);
  s[1]>>y;
  s[1]>>x;
  s[1]<<y or x;
  end
)
