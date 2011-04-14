// ********** Int-stream merging-type ****************
(nu a: rec $i.{ ^case1: 1=>3:1<Int>;$i,
                ^case2: 2=>3:1<Int>;$i
              })
( // Branching (splitting and merging) process
  def Branch(dest: rec %x. { ^case1: 1 << <Int>;%x,
                             ^case2: %x
                           }@(1of3)) =
     ( // Merging Process
       def Merge(source: rec %i.{ ^case1: 1>><Int>;%i,
                                  ^case2: 1>><Int>;%i
                                }@(3of3),
                 dest: rec %o.{ ^case1: 1<<<Int>;%o,
                                ^case2: %o
                              }@(1of3)) =
          sync(3,source)
          { ^case1: source[1] >> i;
                    sync(3,dest)
                    { ^case1: dest[1] << i;
                              Merge(source,dest),
                      ^case2: Merge(source,dest) // FIXME: Loop to send i
                    },
            ^case2: source[1] >> i;
                    sync(3,dest)
                    { ^case1: dest[1] << i;
                              Merge(source,dest),
                      ^case2: Merge(source,dest) // FIXME: Loop to send i
                    }
          }
       in link(3,a,source,3);
          Merge(source,dest)
     | def Send(dest: rec %o.{ ^case1: %o,
                               ^case2: 1<<<Int>;%o
                             }@(2of3),
                i: Int) =
          sync(3,dest)
          { ^case1: Send(dest,i),
            ^case2: dest[1] << i;
                    Send(dest,i)
          }
       in link(3,a,dest,2);
          Send(dest,1)
     | link(3,a,newdest,1);
       sync(3,newdest)
       { ^case1: newdest[1] << 1;
                 Branch(newdest),
         ^case2: Branch(newdest)
       }
     )
  in link(3,a,s,1);
     sync(3,s)
     { ^case1: s[1] << 1;
               Branch(s),
       ^case2: Branch(s)
     }
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
