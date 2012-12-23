(nu a: 1=>2<Bool> as x;
       1=>2<Bool> as y;
       2=>1<Bool> as z[[x or y or z]];
       Gend)
( link(2,a,s,2);
  s[1]>>y;
  s[1]>>y2;
  s[1]<<not (y and y2);
  end
)
