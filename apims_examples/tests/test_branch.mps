// Implementation of math server using branching
(nu a : 1=>2:1
        { ^sum: 1=>2:1<Int>;
                1=>2:1<Int>;
                2=>1:2<Int>;
                Gend,
          ^mult:1=>2:1<Int>;
                1=>2:1<Int>;
                2=>1:2<Int>;
                Gend,
          ^neg: 1=>2:1<Int>;
                2=>1:2<Int>;
                Gend
        })
( def X(var: Int) =
    link(2,a,s,2);
    ( X(var)
    | s[1] >>
      { ^sum: s[1] >> x;
              s[1] >> y;
              s[2] << x+y;
              end,
        ^mult:s[1] >> x;
              s[1] >> y;
              s[2] << x*y;
              end,
        ^neg: s[1] >> x;
              s[2] << 0-x;
              end
      }
    )
  in X(0)
| link(2,a,s,1);
  s[1] << ^mult;
  s[1] << 5;
  s[1] << 3;
  s[2] >> x;
  link(2,a,s,1);
  s[1] << ^sum;
  s[1] << x;
  s[1] << x;
  s[2] >> x;
  link(2,a,s,1);
  s[1] << ^neg;
  s[1] << x;
  s[2] >> x;
  end
)
