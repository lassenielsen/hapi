(nu a: 1=>2:1<Bool> as x1;
       2=>1:2<Bool> as x2[[x1 or x2]];
       Gend)
( link(2,a,s,2);
  s[1]>>q;
  q : Bool = not q;
  s[2] << not q;
  end
)
