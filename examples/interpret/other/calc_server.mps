define $a=
  rec $server.
  1=>2
  { ^add: 1=>2<Int>;
          $server,
    ^mult:1=>2<Int>;
          $server,
    ^neg: $server,
    ^done:2=>1<Int>;
          Gend
  }
in
(nu a : $a)
( def Fork(id: Int) =
    link(2,a,s,2);
    ( Fork(id+1)
    | def Server(s: $a@(2of2),
                 v: Int) =
        s[1] >>
        { ^add: s[1] >> x;
                Server(s,v+x),
          ^mult:s[1] >> x;
                Server(s,v*x),
          ^neg: Server(s,0-v),
          ^done:s[1]<<v;
                end
      }
      in Server(s,0)
    )
  in Fork(1)
| link(2,a,s,1);
  s[2] << ^add;
  s[2] << 5;
  s[2] << ^mult;
  s[2] << 3;
  s[2] << ^mult;
  s[2] << 2;
  s[2] << ^neg;
  s[2] << ^done;
  s[2] >> x;
  end
)
