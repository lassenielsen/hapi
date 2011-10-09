(nu rnd: 2=>1:1<Bool>;Gend)
( def Rnd() = // {{{
    link(2,rnd,s,2);
    ( Rnd()
    | (nu a: {^TRUE: Gend, ^FALSE: Gend})
      link(1,a,select,1);
      sync(1,select)
      {^TRUE: s[1]<<true;end,
       ^FALSE: s[1]<<false;end
      }
    )
  in Rnd() | // }}}
(nu rand: 1=>2:1<Int>;2=>1:2<Int>;Gend)
( def Rand() = // {{{
    link(2,rand,s,2);
    ( Rand()
    | def Random(min: Int, max: Int, dest: 2<<<Int>;Lend@(2of2)) =
        if max <= min
        then dest[2]<<min; end
        else link(2,rnd,select,1);
             select[1]>>choice;
             if choice
             then Random(min,(min+max)/2,dest)
             else Random((min+max)/2+1,max,dest)
      in
        s[1]>>max;
        Random(0,max-1,s)
    )
  in Rand() | // }}}
  link(2,rand,s,1);
  s[1]<<256;
  s[2]>>x;
  end
) )
