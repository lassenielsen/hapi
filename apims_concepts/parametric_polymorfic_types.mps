// Adding type values to enable parametricaly polymorfic types
// Showing example of polymorphic lists
define $list= // The interface of a list {{{
  1=>2:1<Type x>;
  rec $list. 
  1=>2:1
  {^Cons: 1=>2:1<x>;
            $list,
   ^Lookup: 1=>2:1<Int>;
            2=>1:2
            {^NONE: $list,
             ^SOME: 2=>1:2<x>;
                    $list
            },
   ^Delete: Gend
  }
in // }}}
// Declare channel (constructor) {{{
(nu list: $list) // }}}
// Implement list {{{
( def List() =
    link(2,list,s,2);
    ( List()
    | s[1]>>type;
      def NIL(this: $list@(2 of 2)) = // {{{
        this[1]>>
        {^Cons:
          this[1]>>val;
          link(2,list,s,1);
          def CONS(this: $list@(2 of 2), next: $list@(1 of 2), v: type) = // {{{
            this[1]>>
            {^Cons:
              next[1]<<^Cons;
              this[1]>>newVal;
              next[1]<<v;
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
| link(2,list,myIntList,1); // Create Int list
  myIntList[1]<<Int;
  myIntList[1]<<^Cons;
  myIntList[1]<<3;          // myList = [0:3]
  myIntList[1]<<^Cons;
  myIntList[1]<<2;          // myList = [0:2, 1:3]
  myIntList[1]<<^Cons;
  myIntList[1]<<1;          // myList = [0:1, 1:2, 2:3]
  link(2,list,myStrList,1); // Create String list
  myStrList[1]<<String;
  myStrList[1]<<^Cons;
  myStrList[1]<<"Hello";    // myList = [0:3]
  myStrList[1]<<^Cons;
  myStrList[1]<<" ";        // myList = [0:2, 1:3]
  myStrList[1]<<^Cons;
  myStrList[1]<<"World";    // myList = [0:1, 1:2, 2:3]
  myIntList[1]<<^Lookup;    // Retrieve element from myintList
  myIntList[1]<<2;          // with index 2
  myIntList[2]>>            // Returns ^SOME, 3
  {^NONE: myIntList[1]<<^Delete; myStrList[1]<<^Delete; end,
   ^SOME: myList[2]>>val; myIntList[1]<<^Delete; myStrList[1]<<^Delete; end
  }
)
