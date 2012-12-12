define $list = // INTEGER LIST INTERFACE {{{
  rec $intlist.
  1=>2
  {^Read:
    2=>1
    {^NONE:
      $intlist,
     ^SOME:
      2=>1<Int>;
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
        this[1]>>
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
(nu cons: 1=>2<Int>; // CONS DECLARATION {{{
          1=>2<$list@(1of2)>;
          2=>1<$list@(1of2)>;
          Gend) // }}}
( def ConsService() = // CONS IMPLEMENTATION {{{
    link(2,cons,s,2);
    ( ConsService()
    | s[1]>>elt;
      s[1]>>tail@(1 of 2);
      (nu locallink: $list)
      ( link(2,locallink,lnk,2);
        def Cons(this: $list@(2of2), tail: $list@(1of2), val: Int) = // {{{
          this[1]>>
          {^Read:
            this[1]<<^SOME;
            this[1]<<val;
            Cons(this,tail,val),
           ^Pop:
            tail[2]<<^Read;
            tail[2]>>
            {^NONE:
              tail[2]<<^Delete;
              EmptyList(this),
             ^SOME:
              tail[2]>>newval;
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
(nu listrev: 1=>2<$list@(1of2)>; // LIST COPY DECLARATION {{{
             2=>1<$list@(1of2)>;
	     Gend) // }}}
( def ListRevService() = // CONS IMPLEMENTATION {{{
    link(2,listrev,s,2);
    ( ListRevService()
    | def ListRev(lst: $list@(1of2), rev: $list@(1of2), dest: 1<<<$list@(1of2)>;Lend@(2of2)) =
        lst[2]<<^Read;
        lst[2]>>
        {^NONE:
          lst[2]<<^Delete;
          dest[1]<<rev;
          end,
         ^SOME:
          lst[2]>>head;
          lst[2]<<^Pop;
          link(2,cons,cs,1);
          cs[2]<<head;
          cs[2]<<rev;
          cs[2]>>newrev@(1 of 2);
	  ListRev(lst,newrev,dest)
        }
        in
          s[1]>>lst@(1 of 2);
	  link(2,newlist,rev,1);
          ListRev(lst,rev,s)
    )
  in ListRevService() | 
// }}}
(nu listcopy: 1=>2<$list@(1of2)>; // LIST COPY DECLARATION {{{
              2=>1<$list@(1of2)>;
              2=>1<$list@(1of2)>;
              Gend) // }}}
( def ListCopyService() = // CONS IMPLEMENTATION {{{
    link(2,listcopy,s,2);
    ( ListCopyService()
    | def ListCopy(lst: $list@(1of2), dest: 1<<<$list@(1of2)>;1<<<$list@(1of2)>;Lend@(2of2)) =
        lst[2]<<^Read;
        lst[2]>>
        {^NONE:
          link(2,newlist,copylst,1);
          dest[1]<<copylst;
          dest[1]<<lst;
          end,
         ^SOME:
          lst[2]>>head;
          lst[2]<<^Pop;
          link(2,listcopy,call,1);
          call[2]<<lst;
          call[2]>>copy1@(1 of 2);
          link(2,cons,cons1,1);
          cons1[2]<<head;
          cons1[2]<<copy1;
          cons1[2]>>copy1@(1 of 2);
          dest[1]<<copy1;
          call[2]>>copy2@(1 of 2);
          link(2,cons,cons2,1);
          cons2[2]<<head;
          cons2[2]<<copy2;
          cons2[2]>>copy2@(1 of 2);
          dest[1]<<copy2;
          end
        }
        in
          s[1]>>lst@(1 of 2);
          ListCopy(lst,s)
    )
  in ListCopyService() | 
// }}}
(nu append: 1=>2<$list@(1of2)>; // APPEND DECLARATION {{{
            1=>2<$list@(1of2)>;
            2=>1<$list@(1of2)>;
            Gend) // }}}
( def AppendService() = // APPEND IMPLEMENTATION {{{
    link(2,append,s,2);
    ( AppendService()
    | def Append(lhs: $list@(1of2), rhs: $list@(1of2), stack: $list@(1of2), dest: 1<<<$list@(1of2)>;Lend@(2of2)) = 
        lhs[2]<<^Read;
        lhs[2]>>
        {^NONE:
          lhs[2]<<^Delete;
          def AppendRev(lhs: $list@(1of2), rhs: $list@(1of2), dest: 1<<<$list@(1of2)>;Lend@(2of2)) =
            lhs[2]<<^Read;
            lhs[2]>>
            {^NONE:
              lhs[2]<<^Delete;
              dest[1]<<rhs;
              end,
             ^SOME:
              lhs[2]>>elt;
              lhs[2]<<^Pop;
              link(2,cons,cs,1);
              cs[2]<<elt;
              cs[2]<<rhs;
              cs[2]>>rhs@(1 of 2);
              AppendRev(lhs,rhs,dest)
            }
          in
            AppendRev(stack,rhs,dest),
         ^SOME:
          lhs[2]>>elt;
          lhs[2]<<^Pop;
          link(2,cons,cs,1);
          cs[2]<<elt;
          cs[2]<<stack;
          cs[2]>>stack@(1 of 2);
          Append(lhs,rhs,stack,dest)
        }
      in
        link(2,newlist,stack,1);
        s[1]>>lhs@(1 of 2);
        s[1]>>rhs@(1 of 2);
        Append(lhs,rhs,stack,s)
    )
  in AppendService() | 
// }}}
define $re = // INTERFACE OF A REGULAR EXPRESSION {{{
  rec $regexp.
  1=>2
  {^Match:
    1=>2<$list@(1of2)>;
    2=>1<Bool>;
    $regexp,
//   ^Compress:
//    1=>2<$list@(1of2)>;
//    2=>1<$list@(1of2)>;
//    $regexp,
//   ^Decompress:
//    1=>2<$list@(1of2)>;
//    2=>1<$list@(1of2)>;
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
        this[1]>>
        {^Match:
          this[1]>>str@(1 of 2);
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
        this[1]>>
        {^Match:
          this[1]>>str@(1 of 2);
          str[2]<<^Read;
          str[2]>>
          {^NONE:
            str[2]<<^Delete;
            this[1]<<true;
            RE1(this),
           ^SOME:
            this[1]<<false;
            str[2]>>lit;
            str[2]<<^Delete;
            RE1(this)
          },
         ^Delete:
          end
        }
      in RE1(connection)
    )
  in // }}}
(nu re_literal: 1=>2<Int>;$re) // RE_Literal Decleration
( def RELiteralService() = // RE_Literal Implementation {{{
    link(2,re_literal,connection,2);
    ( RELiteralService()
    | def RELiteral(this: $re@(2of2), literal: Int) =
        this[1]>>
        {^Match:
          this[1]>>str@(1 of 2);
          str[2]<<^Read;
          str[2]>>
          {^NONE:
            str[2]<<^Delete;
            this[1]<<false;
            RELiteral(this,literal),
           ^SOME:
            str[2]>>lit;
            if literal=lit
            then str[2]<<^Pop;
                 str[2]<<^Read;
                 str[2]>>
                 {^NONE:
                   this[1]<<true;
                   str[2]<<^Delete;
                   RELiteral(this,literal),
                  ^SOME:
                   this[1]<<false;
                   str[2]>>lit;
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
        connection[1]>>lit;
        RELiteral(connection,lit)
    )
  in RELiteralService() | // }}}
(nu re_sequence: 1=>2<$re@(1of2)>;1=>2<$re@(1of2)>;$re) // RE_Sequence Decleration
( def RESequenceService() = // RE_Sequence Implementation {{{
    link(2,re_sequence,connection,2);
    ( RESequenceService()
    | def RESequence(this: $re@(2of2), left: $re@(1of2), right: $re@(1of2)) =
        this[1]>>
        {^Match:
	  def MatchSeq(this: 2=>1<Bool>;$re@(2of2), left: $re@(1of2), right: $re@(1of2), str1: $list@(1of2), str2: $list@(1of2)) =
	    str1[2]<<^Read;
	    str1[2]>>
	    {^NONE:
	      link(2,listrev,r1,1);
	      r1[2]<<str2;
	      left[2]<<^Match;
	      left[2]<<str1;
	      left[2]>>result1;
	      right[2]<<^Match;
	      r1[2]>>str2@(1 of 2);
	      right[2]<<str2;
	      right[2]>>result2;
	      this[1]<<result1 and result2;
	      RESequence(this,left,right),
	     ^SOME:
	      str1[2]>>head;
	      // Copy both strings
	      link(2,listcopy,cpy1,1);
	      cpy1[2]<<str1;
	      cpy1[2]>>str1@(1 of 2);
	      cpy1[2]>>str1cpy@(1 of 2);
	      link(2,listcopy,cpy2,1);
	      cpy2[2]<<str2;
	      cpy2[2]>>str2@(1 of 2);
	      cpy2[2]>>str2cpy@(1 of 2);
	      // Check if current division is correct
	      left[2]<<^Match;
	      left[2]<<str1;
	      left[2]>>result1;
	      link(2,listrev,r1,1);
	      r1[2]<<str2;
	      r1[2]>>str2@(1 of 2);
	      right[2]<<^Match;
	      right[2]<<str2;
	      right[2]>>result2;
	      if (result1 and result2) // If match return
	      then str1cpy[2]<<^Delete;
	           str2cpy[2]<<^Delete;
		   this[1]<<true;
		   RESequence(this,left,right)
	      else str1cpy[2]<<^Pop;   // Else try with later separations
	           link(2,cons,cons1,1);
		   cons1[2]<<head;
		   cons1[2]<<str2cpy;
		   cons1[2]>>str2cpy@(1 of 2);
                   MatchSeq(this,left,right,str1cpy,str2cpy)
	    }
          in
	    this[1]>>str1@(1 of 2);
	    link(2,newlist,str2,1);
	    MatchSeq(this,left,right,str1,str2),
         ^Delete:
          left[2]<<^Delete;
          right[2]<<^Delete;
          end
        }
      in
        connection[1]>>left@(1 of 2);
        connection[1]>>right@(1 of 2);
        RESequence(connection,left,right)
    )
  in RESequenceService() | // }}}
(nu re_sum: 1=>2<$re@(1of2)>;1=>2<$re@(1of2)>;$re) // RE_Sum Decleration
( def RESumService() = // RE_Sequence Implementation {{{
    link(2,re_sum,connection,2);
    ( RESumService()
    | def RESum(this: $re@(2of2), left: $re@(1of2), right: $re@(1of2)) =
        this[1]>>
        {^Match:
	  this[1]>>str1@(1 of 2);
          left[2]<<^Match;
          right[2]<<^Match;
          link(2,listcopy,cpy,1);
          cpy[2]<<str1;
          cpy[2]>>str2@(1 of 2);
          left[2]<<str2;
          cpy[2]>>str3@(1 of 2);
          right[2]<<str3;
          left[2]>>r1;
          right[2]>>r2;
          this[1]<<r1 or r2;
	  RESum(this,left,right),
         ^Delete:
          left[2]<<^Delete;
          right[2]<<^Delete;
          end
        }
      in
        connection[1]>>left@(1 of 2);
        connection[1]>>right@(1 of 2);
        RESum(connection,left,right)
    )
  in RESumService() | // }}}
(nu re_star: 1=>2<$re@(1of2)>;$re) // RE_Star Decleration
( def REStarService() = // RE_Star Implementation {{{
    link(2,re_star,connection,2);
    ( REStarService()
    | def REStar(this: $re@(2of2), body: $re@(1of2)) =
        this[1]>>
        {^Match:
	  this[1]>>str1@(1 of 2);
          str1[2]<<^Delete;
          this[1]<<false;
	  REStar(this,body),
         ^Delete:
          body[2]<<^Delete;
          end
        }
      in
        connection[1]>>body@(1 of 2);
        REStar(connection,body)
    )
  in REStarService() | // }}}
  link(2,newlist,list1,1);
  link(2,cons,cs,1);cs[2]<<1;cs[2]<<list1;cs[2]>>list1@(1 of 2);
  link(2,cons,cs,1);cs[2]<<2;cs[2]<<list1;cs[2]>>list1@(1 of 2);
  link(2,cons,cs,1);cs[2]<<2;cs[2]<<list1;cs[2]>>list1@(1 of 2);
  link(2,cons,cs,1);cs[2]<<1;cs[2]<<list1;cs[2]>>list1@(1 of 2);
  link(2,re_literal,lit_a1,1);lit_a1[2]<<1;
  link(2,re_literal,lit_a2,1);lit_a2[2]<<1;
  link(2,re_literal,lit_a3,1);lit_a3[2]<<1;
  link(2,re_literal,lit_a4,1);lit_a4[2]<<1;
  link(2,re_literal,lit_b1,1);lit_b1[2]<<2;
  link(2,re_literal,lit_b2,1);lit_b2[2]<<2;
  link(2,re_sum,sum_ab1,1);sum_ab1[2]<<lit_a1;sum_ab1[2]<<lit_b1;
  link(2,re_sum,sum_ab2,1);sum_ab2[2]<<lit_a2;sum_ab2[2]<<lit_b2;
  link(2,re_sequence,re1,1);re1[2]<<lit_a3;re1[2]<<sum_ab1;
  link(2,re_sequence,re2,1);re2[2]<<re1;re2[2]<<sum_ab2;
  link(2,re_sequence,re3,1);re3[2]<<re2;re3[2]<<lit_a4;
  re3[2]<<^Match;
  re3[2]<<list1;
  re3[2]>>x;
  re3[2]<<^Delete;
  end
) ) ) ) ) ) ) ) ) ) ) )
