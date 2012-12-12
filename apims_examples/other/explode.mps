// ********** Int-stream merging-type ****************
define $a =
  rec $i.
  { ^case1: 1=>3<Int>;$i,
    ^case2: 2=>3<Int>;$i
  }
in
(nu a: $a)
( // Branching (splitting and merging) process
  def Branch(dest: $a@(1of3)) =
     ( // Merging Process
       def Merge(source: $a@(3of3),
                 dest: $a@(1of3)) =
          sync(3,source)
          { ^case1: source[1] >> i;
                    sync(3,dest)
                    { ^case1: dest[3] << i;
                              Merge(source,dest),
                      ^case2: Merge(source,dest) // FIXME: Loop to send i
                    },
            ^case2: source[2] >> i;
                    sync(3,dest)
                    { ^case1: dest[3] << i;
                              Merge(source,dest),
                      ^case2: Merge(source,dest) // FIXME: Loop to send i
                    }
          }
       in link(3,a,source,3);
          Merge(source,dest)
     | def Send(dest: $a@(2of3),
                i: Int) =
          sync(3,dest)
          { ^case1: Send(dest,i),
            ^case2: dest[3] << i;
                    Send(dest,i)
          }
       in link(3,a,dest,2);
          Send(dest,1)
     | link(3,a,newdest,1);
       sync(3,newdest)
       { ^case1: newdest[3] << 1;
                 Branch(newdest),
         ^case2: Branch(newdest)
       }
     )
  in link(3,a,s,1);
     sync(3,s)
     { ^case1: s[3] << 1;
               Branch(s),
       ^case2: Branch(s)
     }
   | // Sending Process
     def Send(dest: $a@(2of3),
              i: Int) =
        sync(3,dest)
        { ^case1: Send(dest,i),
          ^case2: dest[3] << i;
                  Send(dest,i)
        }
     in link(3,a,dest,2);
        Send(dest,2)
   | // Receiving Process
     def Rcv(source: $a@(3of3)) =
        sync(3,source)
        { ^case1: source[1] >> i;
                  Rcv(source),
          ^case2: source[2] >> i;
                  Rcv(source)
        }
     in link(3,a,source,3);
        Rcv(source)
)
