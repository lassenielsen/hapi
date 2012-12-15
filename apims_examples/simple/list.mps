// This example defines the interface of - and implements - an ordered list.
// The implementation can be optimized in many ways.
define $list= // The interface of a list {{{
  rec $list. 
  1=>2
  {^Insert: 1=>2<Int>;
            $list,
   ^Lookup: 1=>2<Int>;
            2=>1
            {^NONE: $list,
             ^SOME: 2=>1<Int>;
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
        def CONS(this: $list@(2 of 2), next: $list@(1 of 2), v: Int) = // {{{
          this[1]>>
          {^Insert:
            next[2]<<^Insert;
            this[1]>>newVal;
            if v<=newVal
            then next[2]<<newVal;
                 CONS(this,next,v)
            else next[2]<<v;
                 CONS(this,next,newVal),
           ^Lookup:
            this[1]>>pos;
            if pos<=0
            then this[1]<<^SOME;
                 this[1]<<v;
                 CONS(this,next,v)
            else next[2]<<^Lookup;
                 next[2]<<pos-1;
                 next[2]>>
                 {^SOME:
                   this[1]<<^SOME;
                   next[2]>>val;
                   this[1]<<val;
                   CONS(this,next,v),
                  ^NONE:
                   this[1]<<^NONE;
                   CONS(this,next,v)
                 },
           ^Delete:
            next[2]<<^Delete;
            end
          } // }}}
        in         
          this[1]>>
          {^Insert:
            this[1]>>val;
            link(2,list,s,1);
            CONS(this,s,val),
           ^Lookup:
            this[1]>>pos;
            this[1]<<^NONE;
            NIL(this),
           ^Delete:
            end
          } // }}}
      in NIL(s)
    )
  in List() // }}}
| link(2,list,myList,1); // Create myList as list
  myList[2]<<^Insert;
  myList[2]<<5;          // myList = [0:5]
  myList[2]<<^Insert;
  myList[2]<<2;          // myList = [0:2, 1:5]
  myList[2]<<^Insert;
  myList[2]<<7;          // myList = [0:2, 1:5, 2:7]
  myList[2]<<^Insert;
  myList[2]<<3;          // myList = [0:2, 1:3, 2:5, 3:7]
  myList[2]<<^Lookup;    // Retrieve element
  myList[2]<<2;          // with index 2
  myList[2]>>            // Returns ^SOME, 5
  {^NONE: myList[2]<<^Delete; end,
   ^SOME: myList[2]>>val; myList[2]<<^Delete; end
  }
)
