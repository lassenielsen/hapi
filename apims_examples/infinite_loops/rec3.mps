// ********** Int-stream merging-type ****************
define $merge =
  rec $merge.{ ^case1: 1=>3<Int>;$merge,
               ^case2: 2=>3<Int>;$merge
             }
in
(nu a: $merge)
( def Send(dest: $merge@(1of3),
           i: Int) =
      sync(3,dest)
      { ^case1: dest[3] << i;
                Send(dest,i),
        ^case2: Send(dest,i)
      }
  in link(3,a,s,1);
     Send(s,3)
| // Sending Process
  def Send(dest: $merge@(2of3),
           i: Int) =
     sync(3,dest)
     { ^case1: Send(dest,i),
       ^case2: dest[3] << i;
               Send(dest,i)
     }
  in link(3,a,dest,2);
     Send(dest,2)
| // Receiving Process
  def Rcv(source: $merge@(3of3)) =
     sync(3,source)
     { ^case1: source[1] >> i;
               Rcv(source),
       ^case2: source[2] >> i;
               Rcv(source)
     }
  in link(3,a,source,3);
     Rcv(source)
)
