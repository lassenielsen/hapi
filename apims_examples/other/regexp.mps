define $list = // INTEGER LIST INTERFACE {{{
  rec $intlist.
  1=>2:2
  {^Read:
    2=>1:1
    {^NONE:
      $intlist,
     ^SOME:
      2=>1:1<Int>;
      $intlist
    },
   ^Pop:
    $intlist,
   ^Delete:
    Gend
  }
in
// }}}
(nu newlist: $list) // INT LIST IMPLEMENTATION {{{
( def EmptyList(this: $list@(2of2)) = 
        this[2]>>
        {^Read:
          this[1]<<^NONE;
          EmptyList(this),
         ^Pop:
          EmptyList(this),
         ^Delete:
          end
        }
  in 
( def NewList() =
    link(2,newlist,s,2);
    ( NewList()
    | EmptyList(s)
    )
  in NewList() | 
// }}}
(nu cons: 1=>2:2<Int>; // CONS DECLARATION {{{
          1=>2:2<$list@(1of2)>;
          2=>1:1<$list@(1of2)>;
          Gend) // }}}
( def ConsService() = // CONS IMPLEMENTATION {{{
    link(2,cons,s,2);
    ( ConsService()
    | s[2]>>elt;
      s[2]>>tail;
      (nu locallink: $list)
      ( link(2,locallink,lnk,2);
        def Cons(this: $list@(2of2), tail: $list@(1of2), val: Int) = // {{{
          this[2]>>
          {^Read:
            this[1]<<^SOME;
            this[1]<<val;
            Cons(this,tail,val),
           ^Pop:
            tail[2]<<^Read;
            tail[1]>>
            {^NONE:
              tail[2]<<^Delete;
              EmptyList(this),
             ^SOME:
              tail[1]>>newval;
              tail[2]<<^Pop;
              Cons(this,tail,newval)
            },
           ^Delete:
            tail[2]<<^Delete;
            end
          } // }}}
        in
          Cons(lnk,tail,elt)
      | link(2,locallink,lnk,1);
        s[1]<<lnk;
        end
      )
    )
  in ConsService() | 
// }}}
(nu listrev: 1=>2:2<$list@(1of2)>; // LIST COPY DECLARATION {{{
             2=>1:1<$list@(1of2)>;
	     Gend) // }}}
( def ListRevService() = // CONS IMPLEMENTATION {{{
    link(2,listrev,s,2);
    ( ListRevService()
    | def ListRev(lst: $list@(1of2), rev: $list@(1of2), dest: 1<<<$list@(1of2)>;Lend@(2of2)) =
        lst[2]<<^Read;
        lst[1]>>
        {^NONE:
          lst[2]<<^Delete;
          dest[1]<<rev;
          end,
         ^SOME:
          lst[1]>>head;
          lst[2]<<^Pop;
          link(2,cons,cs,1);
          cs[2]<<head;
          cs[2]<<rev;
          cs[1]>>newrev;
	  ListRev(lst,newrev,dest)
        }
        in
          s[2]>>lst;
	  link(2,newlist,rev,1);
          ListRev(lst,rev,s)
    )
  in ListRevService() | 
// }}}
(nu listcopy: 1=>2:2<$list@(1of2)>; // LIST COPY DECLARATION {{{
              2=>1:1<$list@(1of2)>;
              2=>1:1<$list@(1of2)>;
              Gend) // }}}
( def ListCopyService() = // CONS IMPLEMENTATION {{{
    link(2,listcopy,s,2);
    ( ListCopyService()
    | def ListCopy(lst: $list@(1of2), dest: 1<<<$list@(1of2)>;1<<<$list@(1of2)>;Lend@(2of2)) =
        lst[2]<<^Read;
        lst[1]>>
        {^NONE:
          lst[2]<<^Delete;
          link(2,newlist,copy1,1);
          dest[1]<<copy1;
          link(2,newlist,copy2,1);
          dest[1]<<copy2;
          end,
         ^SOME:
          lst[1]>>head;
          lst[2]<<^Pop;
          link(2,listcopy,call,1);
          call[2]<<lst;
          call[1]>>copy1;
          link(2,cons,cons1,1);
          cons1[2]<<head;
          cons1[2]<<copy1;
          cons1[1]>>copy1;
          dest[1]<<copy1;
          call[1]>>copy2;
          link(2,cons,cons2,1);
          cons2[2]<<head;
          cons2[2]<<copy2;
          cons2[1]>>copy2;
          dest[1]<<copy2;
          end
        }
        in
          s[2]>>lst;
          ListCopy(lst,s)
    )
  in ListCopyService() | 
// }}}
(nu append: 1=>2:2<$list@(1of2)>; // APPEND DECLARATION {{{
            1=>2:2<$list@(1of2)>;
            2=>1:1<$list@(1of2)>;
            Gend) // }}}
( def AppendService() = // APPEND IMPLEMENTATION {{{
    link(2,append,s,2);
    ( AppendService()
    | def Append(lhs: $list@(1of2), rhs: $list@(1of2), stack: $list@(1of2), dest: 1<<<$list@(1of2)>;Lend@(2of2)) = 
        lhs[2]<<^Read;
        lhs[1]>>
        {^NONE:
          lhs[2]<<^Delete;
          def AppendRev(lhs: $list@(1of2), rhs: $list@(1of2), dest: 1<<<$list@(1of2)>;Lend@(2of2)) =
            lhs[2]<<^Read;
            lhs[1]>>
            {^NONE:
              lhs[2]<<^Delete;
              dest[1]<<rhs;
              end,
             ^SOME:
              lhs[1]>>elt;
              lhs[2]<<^Pop;
              link(2,cons,cs,1);
              cs[2]<<elt;
              cs[2]<<rhs;
              cs[1]>>rhs;
              AppendRev(lhs,rhs,dest)
            }
          in
            AppendRev(stack,rhs,dest),
         ^SOME:
          lhs[1]>>elt;
          lhs[2]<<^Pop;
          link(2,cons,cs,1);
          cs[2]<<elt;
          cs[2]<<stack;
          cs[1]>>stack;
          Append(lhs,rhs,stack,dest)
        }
      in
        link(2,newlist,stack,1);
        s[2]>>lhs;
        s[2]>>rhs;
        Append(lhs,rhs,stack,s)
    )
  in AppendService() | 
// }}}
define $re = // INTERFACE OF A REGULAR EXPRESSION {{{
  rec $regexp.
  1=>2:2
  {^Match:
    1=>2:2<$list@(1of2)>;
    2=>1:1<Bool>;
    $regexp,
//   ^Compress:
//    1=>2:2<$list@(1of2)>;
//    2=>1:1<$list@(1of2)>;
//    $regexp,
//   ^Decompress:
//    1=>2:2<$list@(1of2)>;
//    2=>1:1<$list@(1of2)>;
//    $regexp,
   ^Delete:
    Gend
  }
in // }}}
(nu re_0: $re) // RE0 Decleration
( def RE0Service() = // RE0 Implementation {{{
    link(2,re_0,connection,2);
    ( RE0Service()
    | def RE0(this: $re@(2of2)) =
        this[2]>>
        {^Match:
          this[2]>>str;
          str[2]<<^Delete;
          this[1]<<false;
          RE0(this),
         ^Delete:
          end
        }
      in RE0(connection)
    )
  in // }}}
(nu re_1: $re) // RE1 Decleration
( def RE1Service() = // RE1 Implementation {{{
    link(2,re_1,connection,2);
    ( RE1Service()
    | def RE1(this: $re@(2of2)) =
        this[2]>>
        {^Match:
          this[2]>>str;
          str[2]<<^Read;
          str[1]>>
          {^NONE:
            str[2]<<^Delete;
            this[1]<<true;
            RE1(this),
           ^SOME:
            this[1]<<false;
            str[1]>>lit;
            str[2]<<^Delete;
            RE1(this)
          },
         ^Delete:
          end
        }
      in RE1(connection)
    )
  in // }}}
(nu re_literal: 1=>2:2<Int>;$re) // RE_Literal Decleration
( def RELiteralService() = // RE_Literal Implementation {{{
    link(2,re_literal,connection,2);
    ( RELiteralService()
    | def RELiteral(this: $re@(2of2), literal: Int) =
        this[2]>>
        {^Match:
          this[2]>>str;
          str[2]<<^Read;
          str[1]>>
          {^NONE:
            str[2]<<^Delete;
            this[1]<<false;
            RELiteral(this,literal),
           ^SOME:
            str[1]>>lit;
            if (literal<=lit) and (lit<=literal)
            then str[2]<<^Pop;
                 str[2]<<^Read;
                 str[1]>>
                 {^NONE:
                   this[1]<<true;
                   str[2]<<^Delete;
                   RELiteral(this,literal),
                  ^SOME:
                   this[1]<<false;
                   str[1]>>lit;
                   str[2]<<^Delete;
                   RELiteral(this,literal)
                 }
            else this[1]<<false;
                 str[2]<<^Delete;
                 RELiteral(this,literal)
          },
         ^Delete:
          end
        }
      in
        connection[2]>>lit;
        RELiteral(connection,lit)
    )
  in RELiteralService() | // }}}
(nu re_sequence: 1=>2:2<$re@(1of2)>;1=>2:2<$re@(1of2)>;$re) // RE_Sequence Decleration
( def RESequenceService() = // RE_Sequence Implementation {{{
    link(2,re_sequence,connection,2);
    ( RESequenceService()
    | def RESequence(this: $re@(2of2), left: $re@(1of2), right: $re@(1of2)) =
        this[2]>>
        {^Match:
	  def MatchSeq(this: 2=>1:1<Bool>;$re@(2of2), left: $re@(1of2), right: $re@(1of2), str1: $list@(1of2), str2: $list@(1of2)) =
	    str1[2]<<^Read;
	    str1[1]>>
	    {^NONE:
	      link(2,listrev,r1,1);
	      r1[2]<<str2;
	      left[2]<<^Match;
	      left[2]<<str1;
	      left[1]>>result1;
	      right[2]<<^Match;
	      r1[1]>>str2;
	      right[2]<<str2;
	      right[1]>>result2;
	      this[1]<<result1 and result2;
	      RESequence(this,left,right),
	     ^SOME:
	      str1[1]>>head;
	      // Copy both strings
	      link(2,listcopy,cpy1,1);
	      cpy1[2]<<str1;
	      cpy1[1]>>str1;
	      cpy1[1]>>str1cpy;
	      link(2,listcopy,cpy2,1);
	      cpy2[2]<<str2;
	      cpy2[1]>>str2;
	      cpy2[1]>>str2cpy;
	      // Check if current division is correct
	      left[2]<<^Match;
	      left[2]<<str1;
	      left[1]>>result1;
	      link(2,listrev,r1,1);
	      r1[2]<<str2;
	      r1[1]>>str2;
	      right[2]<<^Match;
	      right[2]<<str2;
	      right[1]>>result2;
	      if (result1 and result2) // If match return
	      then str1cpy[2]<<^Delete;
	           str2cpy[2]<<^Delete;
		   this[1]<<true;
		   RESequence(this,left,right)
	      else str1cpy[2]<<^Pop;   // Else try with later separations
	           link(2,cons,cons1,1);
		   cons1[2]<<head;
		   cons1[2]<<str2cpy;
		   cons1[1]>>str2cpy;
                   MatchSeq(this,left,right,str1cpy,str2cpy)
	    }
          in
	    this[2]>>str1;
	    link(2,newlist,str2,1);
	    MatchSeq(this,left,right,str1,str2),
         ^Delete:
          left[2]<<^Delete;
          right[2]<<^Delete;
          end
        }
      in
        connection[2]>>left;
        connection[2]>>right;
        RESequence(connection,left,right)
    )
  in RESequenceService() | // }}}
(nu re_sum: 1=>2:2<$re@(1of2)>;1=>2:2<$re@(1of2)>;$re) // RE_Sum Decleration
( def RESumService() = // RE_Sequence Implementation {{{
    link(2,re_sum,connection,2);
    ( RESumService()
    | def RESum(this: $re@(2of2), left: $re@(1of2), right: $re@(1of2)) =
        this[2]>>
        {^Match:
	  this[2]>>str1;
          left[2]<<^Match;
          right[2]<<^Match;
          link(2,listcopy,cpy,1);
          cpy[2]<<str1;
          cpy[1]>>str2;
          left[2]<<str2;
          cpy[1]>>str3;
          right[2]<<str3;
          left[1]>>r1;
          right[1]>>r2;
          this[1]<<r1 or r2;
	  RESum(this,left,right),
         ^Delete:
          left[2]<<^Delete;
          right[2]<<^Delete;
          end
        }
      in
        connection[2]>>left;
        connection[2]>>right;
        RESum(connection,left,right)
    )
  in RESumService() | // }}}
// LIST TEST  link(2,newlist,list1,1);
// LIST TEST  link(2,cons,cs,1);cs[2]<<3;cs[2]<<list1;cs[1]>>list1;
// LIST TEST  link(2,cons,cs,1);cs[2]<<2;cs[2]<<list1;cs[1]>>list1;
// LIST TEST  link(2,cons,cs,1);cs[2]<<1;cs[2]<<list1;cs[1]>>list1;
// LIST TEST  link(2,cons,cs,1);cs[2]<<0;cs[2]<<list1;cs[1]>>list1;
// LIST TEST  link(2,cons,cs,1);cs[2]<<0-1;cs[2]<<list1;cs[1]>>list1;
// LIST TEST  link(2,cons,cs,1);cs[2]<<0-2;cs[2]<<list1;cs[1]>>list1;
// LIST TEST  link(2,newlist,list2,1);
// LIST TEST  link(2,cons,cs,1);cs[2]<<9;cs[2]<<list2;cs[1]>>list2;
// LIST TEST  link(2,cons,cs,1);cs[2]<<8;cs[2]<<list2;cs[1]>>list2;
// LIST TEST  link(2,cons,cs,1);cs[2]<<7;cs[2]<<list2;cs[1]>>list2;
// LIST TEST  link(2,cons,cs,1);cs[2]<<6;cs[2]<<list2;cs[1]>>list2;
// LIST TEST  link(2,cons,cs,1);cs[2]<<5;cs[2]<<list2;cs[1]>>list2;
// LIST TEST  link(2,cons,cs,1);cs[2]<<4;cs[2]<<list2;cs[1]>>list2;
// LIST TEST  link(2,append,app,1);
// LIST TEST  app[2]<<list1; // -2,-1,0,1,2,3
// LIST TEST  app[2]<<list2; // 4,5,6,7,8,9
// LIST TEST  app[1]>>list3; // -2,-1,0,1,2,3,4,5,6,7,8,9
// LIST TEST  list3[2]<<^Delete;
// TEST abba: a(a+b)(a+b)a  link(2,newlist,list1,1);
// TEST abba: a(a+b)(a+b)a  link(2,cons,cs,1);cs[2]<<1;cs[2]<<list1;cs[1]>>list1;
// TEST abba: a(a+b)(a+b)a  link(2,cons,cs,1);cs[2]<<2;cs[2]<<list1;cs[1]>>list1;
// TEST abba: a(a+b)(a+b)a  link(2,cons,cs,1);cs[2]<<2;cs[2]<<list1;cs[1]>>list1;
// TEST abba: a(a+b)(a+b)a  link(2,cons,cs,1);cs[2]<<1;cs[2]<<list1;cs[1]>>list1;
// TEST abba: a(a+b)(a+b)a  link(2,re_literal,lit_a1,1);lit_a1[2]<<1;
// TEST abba: a(a+b)(a+b)a  link(2,re_literal,lit_a2,1);lit_a2[2]<<1;
// TEST abba: a(a+b)(a+b)a  link(2,re_literal,lit_a3,1);lit_a3[2]<<1;
// TEST abba: a(a+b)(a+b)a  link(2,re_literal,lit_a4,1);lit_a4[2]<<1;
// TEST abba: a(a+b)(a+b)a  link(2,re_literal,lit_b1,1);lit_b1[2]<<2;
// TEST abba: a(a+b)(a+b)a  link(2,re_literal,lit_b2,1);lit_b2[2]<<2;
// TEST abba: a(a+b)(a+b)a  link(2,re_sum,sum_ab1,1);sum_ab1[2]<<lit_a1;sum_ab1[2]<<lit_b1;
// TEST abba: a(a+b)(a+b)a  link(2,re_sum,sum_ab2,1);sum_ab2[2]<<lit_a2;sum_ab2[2]<<lit_b2;
// TEST abba: a(a+b)(a+b)a  link(2,re_sequence,re1,1);re1[2]<<lit_a3;re1[2]<<sum_ab1;
// TEST abba: a(a+b)(a+b)a  link(2,re_sequence,re2,1);re2[2]<<re1;re2[2]<<sum_ab2;
// TEST abba: a(a+b)(a+b)a  link(2,re_sequence,re3,1);re3[2]<<re2;re3[2]<<lit_a4;
// TEST abba: a(a+b)(a+b)a  re3[2]<<^Match;
// TEST abba: a(a+b)(a+b)a  re3[2]<<list1;
// TEST abba: a(a+b)(a+b)a  re3[1]>>x;
// TEST abba: a(a+b)(a+b)a  re3[2]<<^Delete;
  link(2,newlist,list1,1);
  link(2,cons,c1,1);c1[2]<<1;c1[2]<<list1;c1[1]>>list1;
  link(2,re_literal,r1,1); r1[2]<<1;
  link(2,re_literal,r2,1); r2[2]<<2;
  link(2,re_sum,r,1); r[2]<<r1; r[2]<<r2;
  r[2]<<^Match;
  r[2]<<list1;
  r[1]>>x;
  r[2]<<^Delete;
  end
) ) ) ) ) ) ) ) ) ) )
