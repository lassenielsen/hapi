(nu a : rec $server.
        1=>2:1
        { ^add: 1=>2:1<Int>;
                $server,
          ^mult:1=>2:1<Int>;
                $server,
          ^neg: $server,
          ^done:2=>1:2<Int>;
                Gend
        })
( def Fork(id: Int) =
    link(2,a,s,2);
    ( Fork(id+1)
    | def Server(s: rec %server.
                    1 >>
                    { ^add: 1>><Int>;
                            %server,
                      ^mult:1>><Int>;
                            %server,
                      ^neg: %server,
                      ^done:2<<<Int>;
                            Lend
                    }@(2of2),
                 v: Int) =
        s[1] >>
        { ^add: s[1] >> x;
                Server(s,v+x),
          ^mult:s[1] >> x;
                Server(s,v*x),
          ^neg: Server(s,0-v),
          ^done:s[2]<<v;
                end
      }
      in Server(s,0)
    )
  in Fork(1)
| link(2,a,s,1);
  s[1] << ^add;
  s[1] << 5;
  s[1] << ^mult;
  s[1] << 3;
  s[1] << ^mult;
  s[1] << 2;
  s[1] << ^neg;
  s[1] << ^done;
  s[2] >> x;
  end
)
