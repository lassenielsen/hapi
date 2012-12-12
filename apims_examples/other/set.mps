// This example defines the interface of - and implements - an integer set.
// The implementation can be optimized in many ways.
define $set= // The interface of a set
  rec $set. 
  1=>2
  {//^Erase: 1=>2<Int>;
   //        $set,
   ^Member: 1=>2<Int>;
            2=>1<Bool>;
            $set,
   ^Delete: Gend
  }
in
(nu empty_set: $set)
( def EmptySet() =
    link(2,empty_set,s,2);
    ( EmptySet()
    | def Empty(this: $set@(2of2)) =
        this[1]>>
        {^Member:
          this[1]>>x;
          this[1]<<false;
          Empty(this),
         ^Delete:
          end
        }
      in Empty(s)
    )
  in EmptySet() |
(nu singleton_set: 1=>2<Int>;$set)
( def SingletonSet() =
    link(2,singleton_set,s,2);
    ( SingletonSet()
    | s[1]>>elt;
      def Singleton(this: $set@(2of2)) =
        this[1]>>
        {^Member:
          this[1]>>x;
          this[1]<<x=elt;
          Singleton(this),
         ^Delete:
          end
        }
      in Singleton(s)
    )
  in SingletonSet() |
(nu union_set: 1=>2<$set@(1of2)>;1=>2<$set@(1of2)>;$set)
( def UnionSet() =
    link(2,union_set,s,2);
    ( UnionSet()
    | def Union(this: $set@(2of2), set1: $set@(1of2), set2: $set@(1of2)) =
        this[1]>>
        {^Member:
          set1[2]<<^Member;
          set2[2]<<^Member;
          this[1]>>x;
          set1[2]<<x;
          set2[2]<<x;
          set1[2]>>r1;
          set2[2]>>r2;
          this[1]<<r1 or r2;
          Union(this,set1,set2),
         ^Delete:
          set1[2]<<^Delete;
          set2[2]<<^Delete;
          end
        }
      in
        s[1]>>s1@(1 of 2);
        s[1]>>s2@(1 of 2);
        Union(s,s1,s2)
    )
  in UnionSet() |
link(2,singleton_set,s1,1); s1[2]<<1;
link(2,singleton_set,s2,1); s2[2]<<2;
link(2,singleton_set,s3,1); s3[2]<<3;
link(2,singleton_set,s4,1); s4[2]<<4;
link(2,singleton_set,s5,1); s5[2]<<5;
link(2,singleton_set,s6,1); s6[2]<<6;
link(2,singleton_set,s7,1); s7[2]<<7;
link(2,singleton_set,s8,1); s8[2]<<8;
link(2,singleton_set,s9,1); s9[2]<<9;
link(2,union_set,u1,1); u1[2]<<s1; u1[2]<<s2;
link(2,union_set,u2,1); u2[2]<<s3; u2[2]<<s4;
link(2,union_set,u3,1); u3[2]<<s5; u3[2]<<s6;
link(2,union_set,u4,1); u4[2]<<s7; u4[2]<<s8;
link(2,union_set,u5,1); u5[2]<<s9; u5[2]<<u1;
link(2,union_set,u6,1); u6[2]<<u2; u6[2]<<u3;
link(2,union_set,u7,1); u7[2]<<u4; u7[2]<<u5;
link(2,union_set,u8,1); u8[2]<<u6; u8[2]<<u7;
u8[2]<<^Member;
u8[2]<<6;
u8[2]>>x;
u8[2]<<^Delete;
end
) ) )
