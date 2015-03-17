// Test of bool expressions
#define $stream \
  rec $x; \
  1->2:Bool; \
  $x;
global $stream a(1,2);
global 1->2:$stream(1 of 1,2);$end; b(1,2);
dest=new a(1 of 2);
s=new b(1 of 2);
s[2] << dest;
|
source=new a(2 of 2);
s=new b(2 of 2);
s[1] >> dest(1 of 1,2);
local X($stream(2 of 2) s,
        $stream(1 of 2) d,
        Bool val)
( d[2] << not val;
  s[1] >> newB;
  X(s,d,newB);
)
X(source,dest,true);
