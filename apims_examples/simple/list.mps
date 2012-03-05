// This example defines the interface of - and implements - an ordered list.
// The implementation can be optimized in many ways.
define $list= // The interface of a list {{{
  rec $list. 
  1=>2:1
  {^Insert: 1=>2:1<Int>;
            $list,
   ^Lookup: 1=>2:1<Int>;
            2=>1:2
            {^NONE: $list,
             ^SOME: 2=>1:2<Int>;
                    $list
            },
   ^Delete: Gend
  }
in // }}}
// Declare channel (constructor) {{{
(nu list: $list) // }}}
// Implement ordered list {{{
( def List() =
    link(2,list,s,2);
    ( List()
    | def NIL(this: $list@(2 of 2)) = // {{{
        this[1]>>
        {^Insert:
          this[1]>>val;
          link(2,list,s,1);
          def CONS(this: $list@(2 of 2), next: $list@(1 of 2), v: Int) = // {{{
            this[1]>>
            {^Insert:
              next[1]<<^Insert;
              this[1]>>newVal;
              if v<=newVal
              then next[1]<<newVal;
                   CONS(this,next,v)
              else next[1]<<v;
                   CONS(this,next,newVal),
             ^Lookup:
              this[1]>>pos;
              if pos<=0
              then this[2]<<^SOME;
                   this[2]<<v;
                   CONS(this,next,v)
              else next[1]<<^Lookup;
                   next[1]<<pos-1;
                   next[2]>>
                   {^SOME:
                     this[2]<<^SOME;
                     next[2]>>val;
                     this[2]<<val;
                     CONS(this,next,v),
                    ^NONE:
                     this[2]<<^NONE;
                     CONS(this,next,v)
                   },
             ^Delete:
              next[1]<<^Delete;
              end
            } // }}}
          in CONS(this,s,val),
         ^Lookup:
          this[1]>>pos;
          this[2]<<^NONE;
          NIL(this),
         ^Delete:
          end
        } // }}}
      in NIL(s)
    )
  in List() // }}}
| link(2,list,myList,1); // Create myList as list
  myList[1]<<^Insert;
  myList[1]<<5;          // myList = [5]
  myList[1]<<^Insert;
  myList[1]<<2;          // myList = [2,5]
  myList[1]<<^Insert;
  myList[1]<<7;          // myList = [2,5,7]
  myList[1]<<^Insert;
  myList[1]<<3;          // myList = [2,3,5,7]
  myList[1]<<^Lookup;    // Retrieve element at position 2
  myList[1]<<2;
  myList[2]>>            // Returns ^SOME, 5
  {^NONE: myList[1]<<^Delete;end,
   ^SOME: myList[2]>>val; myList[1]<<^Delete; end
  }
)
