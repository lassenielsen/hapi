(nu a: 1=>2:1<(Int,String)>;
       2=>1:2<Int>;
       2=>1:2<String>;
       Gend)
( link(2,a,s,1);
  s[1] << (3,"aaa");
  s[2] >> x;
  s[2] >> s;
  end
| link(2,a,s,2);
  s[1] >> pair;
  s[2] << pair&0;
  s[2] << pair&1;
  end
)
