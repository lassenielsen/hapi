(nu a: 1=>2:1<Bool> as x;
       1=>2:1<Bool> as y;
       {^A[[x or y]]:
         {^A[[x]]:
           2=>1:2<Bool> as z [[x or z]];
           Gend,
          ^B[[y]]:
           2=>1:2<Bool> as z [[x or z]];
           Gend
         },
        ^B[[not x or not y]]:
         Gend,
        #C[[x or y]]:
         Gend
       })
( link(2,a,s,2);
  s[1]>>q1;
  s[1]>>q2;
  sync(2,s)
  {^A[[q2 or q1]]: sync(2,s)
       {^A[[q1]]:
         s[2]<<false;
         end,
        ^B[[q2]]:
         s[2]<<true;
         end
       },
   ^B[[not (q1 and q2)]]:
    end,
   #C[[q1]]:
    end
  }
)
