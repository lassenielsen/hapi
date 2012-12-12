// This example defines the interface of - and implements - a binary tree.
// The implementation can be optimized in many ways.
define $bintree= // The interface of a set {{{
  rec $bintree. 
  1=>2
  {^Insert: 1=>2<Int>;
            1=>2<String>;
            $bintree,
   ^Lookup: 1=>2<Int>;
            2=>1
            {^NONE: $bintree,
             ^SOME: 2=>1<String>; $bintree
            },
   ^Delete: Gend
  }
in // }}}
(nu bintree: $bintree)
( def BinTree() =
    link(2,bintree,s,2);
    ( BinTree()
    | def EmptyTree(this: $bintree@(2of2)) =
        this[1]>>
        {^Insert:
          def Tree(this: $bintree@(2of2),
                   left: $bintree@(1of2),
                   idx: Int, val : String,
                   right: $bintree@(1of2)) =
            this[1]>>
            {^Insert:
              this[1]>>newIdx;
              this[1]>>newVal;
              if newIdx=idx
              then Tree(this,left,idx,newVal,right)
              else if newIdx<=idx
              then left[2]<<^Insert;
                   left[2]<<newIdx;
                   left[2]<<newVal;
                   Tree(this,left,idx,val,right)
              else right[2]<<^Insert;
                   right[2]<<newIdx;
                   right[2]<<newVal;
                   Tree(this,left,idx,val,right),
             ^Lookup:
              this[1]>>seek;
              if idx=seek
              then this[1]<<^SOME;
                   this[1]<<val;
                   Tree(this,left,idx,val,right)
              else if seek<=idx
              then left[2]<<^Lookup;
                   left[2]<<seek;
                   left[2]>>
                   {^NONE:
                     this[1]<<^NONE;
                     Tree(this,left,idx,val,right),
                    ^SOME:
                     this[1]<<^SOME;
                     left[2]>>result;
                     this[1]<<result;
                     Tree(this,left,idx,val,right)
                   }
              else right[2]<<^Lookup;
                   right[2]<<seek;
                   right[2]>>
                   {^NONE:
                     this[1]<<^NONE;
                     Tree(this,left,idx,val,right),
                    ^SOME:
                     this[1]<<^SOME;
                     right[2]>>result;
                     this[1]<<result;
                     Tree(this,left,idx,val,right)
                   },
             ^Delete:
              left[2]<<^Delete;
              right[2]<<^Delete;
              end
            }
          in
            link(2,bintree,left,1);
            link(2,bintree,right,1);
            this[1]>>idx;
            this[1]>>val;
            Tree(this,left,idx,val,right),
         ^Lookup:
          this[1]>>find;
          this[1]<<^NONE;
          EmptyTree(this),
         ^Delete:
          end
        }
      in EmptyTree(s)
    )
  in BinTree()
| link(2,bintree,myTree,1);
  myTree[2]<<^Insert;
  myTree[2]<<6;
  myTree[2]<<"6";
  myTree[2]<<^Insert;
  myTree[2]<<2;
  myTree[2]<<"2";
  myTree[2]<<^Insert;
  myTree[2]<<7;
  myTree[2]<<"7";
  myTree[2]<<^Insert;
  myTree[2]<<3;
  myTree[2]<<"3";
  myTree[2]<<^Lookup;
  myTree[2]<<3;
  myTree[2]>>
  {^NONE: myTree[2]<<^Delete;end,
   ^SOME: myTree[2]>>val; myTree[2]<<^Delete; end
  }
)
