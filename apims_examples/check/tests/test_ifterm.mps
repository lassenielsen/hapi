(nu a: 2=>1<Bool>;
       1=>2<Int>;
       Gend)
( link(2,a,s,1);
  s[2] >> b;
  if not if b then false else true
  then s[2] << 1;
       end
  else s[2] << 2;
       end
| link(2,a,s,2);
  s[1] << false;
  s[1] >> val;
  end
)
