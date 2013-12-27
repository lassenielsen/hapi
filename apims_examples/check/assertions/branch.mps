(nu a: 1=>2
       {^A[[x or y]]:
         1=>2
         {^A[[x]]:
           2=>1<Bool> as z [[x or z]];
           Gend,
          ^B[[y]]:
           2=>1<Bool> as z [[x or z]];
           Gend
         },
        ^B[[not x or not y]]:
         Gend
       })
( link(2,a,s,2);
  s[1]>>
  {^A: s[1]>>
       {^A: s[1]<<false;end,
        ^B: s[1]<<true;end
       },
   ^B: end
  }
)
