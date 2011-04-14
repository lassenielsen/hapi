// Merging of stream with two senders into a single
// int-stream

// ********** Int-stream with two senders *********
(nu a: rec $i.{ ^case1: 1=>3:1<Int>;$i,
                ^case2: 2=>3:1<Int>;$i,
                ^case3: Gend
              })
( // ************** Merging Process ***************
  def Merge(source: rec %i.{ ^case1: 1>><Int>;%i,
                             ^case2: 1>><Int>;%i,
                             ^case3: Lend
                           }@(3of3)) =
     sync(3,source)
     { ^case1: source[1] >> i;
               Merge(source),
       ^case2: source[1] >> i;
               Merge(source),
       ^case3: end
     }
  in link(3,a,source,3);
     Merge(source)
  // *********** First Sending Process ************
| def Send(dest: rec %o.{ ^case1: 1<<<Int>;%o,
                          ^case2: %o,
                          ^case3: Lend
                        }@(1of3),
           i: Int) =
     sync(3,dest)
     { ^case1: dest[1] << i;
               Send(dest,i),
       ^case2: Send(dest,i),
       ^case3: end
     }
  in link(3,a,dest,1);
     Send(dest,1)
  // ********** Second Sending Process ************
| def Send(dest: rec %o.{ ^case1: %o,
                          ^case2: 1<<<Int>;%o,
                          ^case3: Lend
                        }@(2of3),
           i: Int) =
     sync(3,dest)
     { ^case1: Send(dest,i),
       ^case2: dest[1] << i;
               Send(dest,i),
       ^case3: end
     }
  in link(3,a,dest,2);
     Send(dest,2)
)
