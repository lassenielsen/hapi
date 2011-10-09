// This example defines the interface of - and implements - an integer set.
// The implementation can be optimized in many ways.
define $set= // The interface of a set
  rec $set. 
  1=>2:1
  {^Insert: 1=>2:1<Int>;
            $list,
   ^Erase: 1=>2:1<Int>;
           $list,
   ^Member: 1=>2:1<Int>;
            2=>1:2<Bool>;
            $list,
   ^Delete: Gend
  }
in
(nu set: $set)
( def Set() =
    link(2,set,s,2);
    ( Set()
    | def Empty(this: $list@(2of2)) =
        this[1]>>
        {^Insert:
          this[1]>>val;
          link(2,list,s,1);
          def Union(this: $list@(2of2), next: $list@(1of2), v: Int) =
            this[1]>>
            {^Insert:
              next[1]<<^Insert;
              this[1]>>newVal;
              if v<=newVal
              then next[1]<<newVal;
                   Union(this,next,v)
              else next[1]<<v;
                   Union(this,next,newVal),
             ^Lookup:
              this[1]>>pos;
              if pos<=0
              then this[2]<<^SOME;
                   this[2]<<v;
                   Union(this,next,v)
              else next[1]<<^Lookup;
                   next[1]<<pos-1;
                   next[2]>>
                   {^SOME:
                     this[2]<<^SOME;
                     next[2]>>val;
                     this[2]<<val;
                     Union(this,next,v),
                    ^NONE:
                     this[2]<<^NONE;
                     Union(this,next,v)
                   },
             ^Delete:
              next[1]<<^Delete;
              end
            }
          in
            Union(this,s,val),
         ^Lookup:
          this[1]>>pos;
          this[2]<<^NONE;
          Empty(this),
         ^Delete:
          end
        }
      in Empty(s)
    )
  in List()
| link(2,list,myList,1);
  myList[1]<<^Insert;
  myList[1]<<5;
  myList[1]<<^Insert;
  myList[1]<<2;
  myList[1]<<^Insert;
  myList[1]<<7;
  myList[1]<<^Insert;
  myList[1]<<3;
  myList[1]<<^Lookup;
  myList[1]<<2;
  myList[2]>>
  {^NONE: myList[1]<<^Delete;end,
   ^SOME: myList[2]>>val; myList[1]<<^Delete; end
  }
)
