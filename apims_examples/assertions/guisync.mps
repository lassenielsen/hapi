(nu a: 1=>2<Bool> as x;
       1=>2<Bool> as y;
       {^A[[x or y]]:
         {^A[[x]]:
           2=>1<Bool> as z [[x or z]];
           Gend,
          ^B[[y]]:
           2=>1<Bool> as z [[x or z]];
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
  guisync(2,s,2)
  {^A[[q2 or q1]](v:Bool=not q2):
    guisync(2,s,2)
    {^A[[q1]](v:Bool=v):
      s[1]<<false;
      end,
     ^B[[q2]](v:Bool=v):
      s[1]<<true;
      end
    },
   ^B[[not (q1 and q2)]](comment:String=""):
    end,
   #C[[q1]](comment:String=""):
    end
  }
)
