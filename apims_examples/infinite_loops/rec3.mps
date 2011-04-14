// ********** Int-stream merging-type ****************
(nu a: rec $i.{ ^case1: 1=>3:1<Int>;$i,
                ^case2: 2=>3:1<Int>;$i
              })
( def Send(dest: rec %o.{ ^case1: 1<<<Int>;%o,
                          ^case2: %o
                        }@(1of3),
           i: Int) =
      sync(3,dest)
      { ^case1: dest[1] << i;
                Send(dest,i),
        ^case2: Send(dest,i)
      }
  in link(3,a,s,1);
     Send(s,3)
| // Sending Process
  def Send(dest: rec %o.{ ^case1: %o,
                          ^case2: 1<<<Int>;%o
                        }@(2of3),
           i: Int) =
     sync(3,dest)
     { ^case1: Send(dest,i),
       ^case2: dest[1] << i;
               Send(dest,i)
     }
  in link(3,a,dest,2);
     Send(dest,2)
| // Receiving Process
  def Rcv(source: rec %i.{ ^case1: 1>><Int>;%i,
                           ^case2: 1>><Int>;%i
                         }@(3of3)) =
     sync(3,source)
     { ^case1: source[1] >> i;
               Rcv(source),
       ^case2: source[1] >> i;
               Rcv(source)
     }
  in link(3,a,source,3);
     Rcv(source)
)
