(nu a: 1=>2<(Int,String)>;
       2=>1<Int>;
       2=>1<String>;
       Gend)
( link(2,a,s,1);
  s[2] << (3,"aaa");
  s[2] >> x;
  s[2] >> str;
  end
| link(2,a,s,2);
  s[1] >> pair;
  s[1] << pair&0;
  s[1] << pair&1;
  end
)
