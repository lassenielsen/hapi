(nu a: 2=>1<Bool>;
       1=>2<Int>;
       Gend)
( link(2,a,s,1);
  s[2] >> b;
  s[2] << if not if b then false else true then 1 else 2;
  end
| link(2,a,s,2);
  s[1] << not true;
  s[1] >> val;
  end
)
