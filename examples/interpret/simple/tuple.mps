1->2:(Int,String);
2->1:Int;
2->1:String;
$end; a(1,2);
( s=new a(1 of 2);
  s[2] << (3,"aaa");
  s[2] >> x;
  s[2] >> str;
| s=new a(2 of 2);
  s[1] >> pair;
  s[1] << pair&0;
  s[1] << pair&1;
)
