// This example defines the interface of - and implements - a binary tree.
// The implementation can be optimized in many ways.
define $bintree= // The interface of a set {{{
  rec $bintree. 
  1=>2:1
  {^Insert: 1=>2:1<Int>;
            1=>2:1<String>;
            $bintree,
   ^Lookup: 1=>2:1<Int>;
            2=>1:2
            {^NONE: $bintree,
             ^SOME: 2=>1:2<String>; $bintree
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
              then left[1]<<^Insert;
                   left[1]<<newIdx;
                   left[1]<<newVal;
                   Tree(this,left,idx,val,right)
              else right[1]<<^Insert;
                   right[1]<<newIdx;
                   right[1]<<newVal;
                   Tree(this,left,idx,val,right),
             ^Lookup:
              this[1]>>seek;
              if idx=seek
              then this[2]<<^SOME;
                   this[2]<<val;
                   Tree(this,left,idx,val,right)
              else if seek<=idx
              then left[1]<<^Lookup;
                   left[1]<<seek;
                   left[2]>>
                   {^NONE:
                     this[2]<<^NONE;
                     Tree(this,left,idx,val,right),
                    ^SOME:
                     this[2]<<^SOME;
                     left[2]>>result;
                     this[2]<<result;
                     Tree(this,left,idx,val,right)
                   }
              else right[1]<<^Lookup;
                   right[1]<<seek;
                   right[2]>>
                   {^NONE:
                     this[2]<<^NONE;
                     Tree(this,left,idx,val,right),
                    ^SOME:
                     this[2]<<^SOME;
                     right[2]>>result;
                     this[2]<<result;
                     Tree(this,left,idx,val,right)
                   },
             ^Delete:
              left[1]<<^Delete;
              right[1]<<^Delete;
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
          this[2]<<^NONE;
          EmptyTree(this),
         ^Delete:
          end
        }
      in EmptyTree(s)
    )
  in BinTree()
| link(2,bintree,myTree,1);
  myTree[1]<<^Insert;
  myTree[1]<<6;
  myTree[1]<<"6";
  myTree[1]<<^Insert;
  myTree[1]<<2;
  myTree[1]<<"2";
  myTree[1]<<^Insert;
  myTree[1]<<7;
  myTree[1]<<"7";
  myTree[1]<<^Insert;
  myTree[1]<<3;
  myTree[1]<<"3";
  myTree[1]<<^Lookup;
  myTree[1]<<3;
  end
//  myTree[2]>>
//  {^NONE: myTree[1]<<^Delete;end,
//   ^SOME: myTree[2]>>val; myTree[1]<<^Delete; end
//  }
)
