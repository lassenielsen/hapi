define $type x =
  rec $state<x:Bool=x>.
  {^X[[x]]: 1=>2:2<Bool> as newX;
            2=>3:3<Bool> as theX[[(not theX or newX) and (theX or not newX)]];
            $state<newX>,
   ^Y[[not x]]: Gend
  }
in
(nu a: $type<true>)
link(3,a,s,3);
def State<x:Bool>(s: $type<x>@(3of3)) =
  guisync(3,s,3)
  {^X[[x]]():
    s[3]>>anX;
    State<x>(s),
   ^Y[[not x]](): end
  }
in State<true>(s)
