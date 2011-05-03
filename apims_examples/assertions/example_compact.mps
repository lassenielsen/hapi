// Define Sequence protocol {{{
define $seq bit1 bit2 bit3 bit4 =
  rec $state<x1:Bool=bit1,x2:Bool=bit2,x3:Bool=bit3,x4:Bool=bit4>.
         {^Continue[[not x1 or not x2 or not x3 or not x4]]: // x<16
           1=>2:1<Int>;
           $state<not x1,
                  ((x1) or x2) and (not (x1) or not x2),
                  ((x1 and x2) or x3) and (not (x1 and x2) or not x3),
                  ((x1 and x2 and x3) or x4) and (not (x1 and x2 and x3) or not x4)>,
          ^End[[x1 and x2 and x3 and x4]]:                   // x=16
           Gend
         }
in // }}}
// Define XOR protocol {{{
define $xor=
  1=>2:2<Bool> as x;
  1=>2:2<Bool> as y;
  2=>1:1<Bool> as z[[(z or (x and y) or (not x and not y)) and (not z or (x and not y) or (not x and y))]];
  Gend
in // }}}
(nu a: $seq<false,false,false,false>)
(nu xor: $xor)
(
  // Implement XOR service {{{
  def XOR() =
    link(2,xor,s,2);
    ( XOR()
    | s[2]>>x1;
      s[2]>>x2;
      s[1]<<(x1 and not x2) or (not x1 and x2);
      end
    )
  in
    XOR() // }}}
| link(2,a,s,1); // Sending process {{{
  def SEQ1<bit1:Bool,bit2:Bool,bit3:Bool,bit4:Bool>(s:$seq<bit1,bit2,bit3,bit4>@(1of2),i:Int) =
    sync(2,s)
    {^Continue[[not (bit1 and bit2 and bit3 and bit4)]]:
      s[1]<<i;
      link(2,xor,t,1);
      t[2]<<bit1;
      t[2]<<bit2;
      t[1]>>newBit2;
      link(2,xor,t,1);
      t[2]<<bit1 and bit2;
      t[2]<<bit3;
      t[1]>>newBit3;
      link(2,xor,t,1);
      t[2]<<bit1 and bit2 and bit3;
      t[2]<<bit4;
      t[1]>>newBit4;
      SEQ1<not bit1,newBit2,newBit3,newBit4>(s,i+1),
     ^End[[bit1 and bit2 and bit3 and bit4]]:
      end
    }
  in
    SEQ1<false,false,false,false>(s,1) // }}}
| link(2,a,s,2); // Receiving process {{{
  def SEQ2<bit1:Bool,bit2:Bool,bit3:Bool,bit4:Bool>(s:$seq<bit1,bit2,bit3,bit4>@(2of2)) =
    sync(2,s)
    {^Continue[[not (bit1 and bit2 and bit3 and bit4)]]:
      s[1]>>i;
      link(2,xor,t,1);
      t[2]<<bit1;
      t[2]<<bit2;
      t[1]>>newBit2;
      link(2,xor,t,1);
      t[2]<<bit1 and bit2;
      t[2]<<bit3;
      t[1]>>newBit3;
      link(2,xor,t,1);
      t[2]<<bit1 and bit2 and bit3;
      t[2]<<bit4;
      t[1]>>newBit4;
      SEQ2<not bit1,newBit2,newBit3,newBit4>(s),
     ^End[[bit1 and bit2 and bit3 and bit4]]:
      end
    }
  in
    SEQ2<false,false,false,false>(s) // }}}
)
