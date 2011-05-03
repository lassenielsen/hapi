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
(nu a: $seq<false,false,false,false>)
(
  // Sending process {{{
  link(2,a,s,1);
  def SEQ1<bit1:Bool,bit2:Bool,bit3:Bool,bit4:Bool>(s:$seq<bit1,bit2,bit3,bit4>@(1of2),i:Int) =
    sync(2,s)
    {^Continue[[not (bit1 and bit2 and bit3 and bit4)]]:
      s[1]<<i;
      SEQ1<not bit1,
           ((bit1) or bit2) and (not (bit1) or not bit2),
           ((bit1 and bit2) or bit3) and (not (bit1 and bit2) or not bit3),
           ((bit1 and bit2 and bit3) or bit4) and (not (bit1 and bit2 and bit3) or not bit4)>(s,i+1),
     ^End[[bit1 and bit2 and bit3 and bit4]]:
      end
    }
  in
    SEQ1<false,false,false,false>(s,1) // }}}
|
  // Receiving process {{{
  link(2,a,s,2);
  def SEQ2<bit1:Bool,bit2:Bool,bit3:Bool,bit4:Bool>(s:$seq<bit1,bit2,bit3,bit4>@(2of2)) =
    sync(2,s)
    {^Continue[[not (bit1 and bit2 and bit3 and bit4)]]:
      s[1]>>i;
      SEQ2<not bit1,
           ((bit1) or bit2) and (not (bit1) or not bit2),
           ((bit1 and bit2) or bit3) and (not (bit1 and bit2) or not bit3),
           ((bit1 and bit2 and bit3) or bit4) and (not (bit1 and bit2 and bit3) or not bit4)>(s),
     ^End[[bit1 and bit2 and bit3 and bit4]]:
      end
    }
  in
    SEQ2<false,false,false,false>(s) // }}}
)
