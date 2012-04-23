// Adding type values to enable parametricaly polymorfic types
// Showing example of polymorphic ordered lists
define $list= // The interface of a list {{{
  1=>2:1<Type x>; // Specify type (can be any type except sesstion types)
  1=>2:1<1=>2:1<x>;1=>2:1<x>;2=>1:2<bool>;Gend>; // Specify leq service
  rec $list. 
  1=>2:1
  {^Insert: 1=>2:1<x>;
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
      s[1]>>type_leq;
      def NIL(this: $list@(2 of 2)) = // {{{
        this[1]>>
        {^Insert:
          this[1]>>val;
          link(2,list,s,1);
          def CONS(this: $list@(2 of 2), next: $list@(1 of 2), v: type) = // {{{
            this[1]>>
            {^Insert:
              next[1]<<^Insert;
              this[1]>>newVal;
              link(2,type_leq,leq,1);
              leq[1]<<v;
              leq[1]<<newVal;
              leq[2]>>is_leq;
              if (is_leq)
              then next[1]<<newVal; CONS(this,next,v),
              else next[1]<<v; CONS(this,next,newVal),
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
| (nu int_leq : 1=>2:1<Int>;1=>2:1<Int>;2=>1:2<Bool>;Gend)
( def IntLEQ() = // {{{
    link(2,int_leq,s,2);
    s[1]>>x;
    s[1]>>y;
    s[2]<<x<=y;
    IntLEQ();
  in IntLEQ() // }}}
| (nu string_leq : 1=>2:1<String>;1=>2:1<String>;2=>1:2<Bool>;Gend)
( def StringLEQ() = // {{{
    link(2,string_leq,s,2);
    s[1]>>x;
    s[1]>>y;
    s[2]<<x<=y;
    IntLEQ();
  in StringLEQ() | // }}}
  link(2,list,myIntList,1); // Create Int list
  myIntList[1]<<Int;
  myIntList[1]<<int_leq;
  myIntList[1]<<^Insert;
  myIntList[1]<<3;          // myIntList = [0:3]
  myIntList[1]<<^Insert;
  myIntList[1]<<1;          // myIntList = [0:1, 1:3]
  myIntList[1]<<^Insert;
  myIntList[1]<<2;          // myIntList = [0:1, 1:2, 2:3]
  link(2,list,myStrList,1); // Create String list
  myStrList[1]<<String;
  myIntList[1]<<string_leq;
  myStrList[1]<<^Insert;
  myStrList[1]<<"Hello";    // myStrList = [0:"Hello"]
  myStrList[1]<<^Insert;
  myStrList[1]<<" ";        // myStrList = [0:" ", 1:"Hello"]
  myStrList[1]<<^Insert;
  myStrList[1]<<"World";    // myStrList = [0:" ", 1:"Hello", 2:"World"]
  myIntList[1]<<^Lookup;    // Retrieve element from myintList
  myIntList[1]<<2;          // with index 2
  myIntList[2]>>            // Returns ^SOME, 3
  {^NONE: myIntList[1]<<^Delete; myStrList[1]<<^Delete; end,
   ^SOME: myList[2]>>val; myIntList[1]<<^Delete; myStrList[1]<<^Delete; end
  }
) )
