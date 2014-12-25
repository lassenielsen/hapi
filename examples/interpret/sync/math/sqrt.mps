(nu gcd: 1=>2<Int>;1=>2<Int>;2=>1<Int>;Gend)
def GCD() = // Greatest Common Divisor Service, used for fraction normalization {{{
  link(2,gcd,s,2);
  ( GCD()
  | def GcdFun(n: Int,
               m: Int,
               s: 1<<<Int>;Lend@(2 of 2)) =
      if (n+1)<=0
      then GcdFun(0-n,m,s)
      else if (m+1)<=0
      then GcdFun(n,0-m,s)
      else if (n+1)<=m
      then GcdFun(m,n,s)
      else if 1<=m
      then GcdFun(m,n-((n/m)*m),s)
      else s[1] << n;
           end
    in  
      s[1]>>n;
      s[1]>>m;
      GcdFun(n,m,s)
  )
in ( GCD() | // }}}
define $fracunop = // Protocol for unary operation on fractions {{{
  1=>2<(Int,Int)>; // Arg fraction (nominator,denominator)
  2=>1<(Int,Int)>; // Result fraction (nominator,denominator)
  Gend
in // }}}
(nu fracnorm: $fracunop)
def FracNorm() = // Fraction Normalisation Service {{{
  link(2,fracnorm,s,2);
  ( FracNorm()
  | s[1]>>x;
    link(2,gcd,t,1);
    t[2]<<x&0;
    t[2]<<x&1;
    t[2]>>div;
    if 0<=(x&1)
    then s[1]<<((x&0)/div,(x&1)/div);
         end
    else s[1]<<(0-((x&0)/div),0-((x&1)/div));
         end
  )
in ( FracNorm() | // }}}
define $fracop = // Protocol for binary operation on Fractions {{{
  1=>2<(Int,Int)>; // First fraction (nominator,denominator)
  1=>2<(Int,Int)>; // Second fraction (nominator,denominator)
  2=>1<(Int,Int)>; // Result fraction (nominator,denominator)
  Gend
in // }}}
(nu fracadd: $fracop)
def FracAdd() = // Fraction Addition Service {{{
  link(2,fracadd,s,2);
  ( FracAdd()
  | s[1]>>x;
    s[1]>>y;
    link(2,fracnorm,t,1);
    t[2]<<(((x&0)*(y&1))+((y&0)*(x&1)),(x&1)*(y&1));
    t[2]>>z;
    s[1]<<z;
    end
  )
in ( FracAdd() | // }}}
(nu fracsub: $fracop)
def FracSub() = // Fraction Subtraction Service {{{
  link(2,fracsub,s,2);
  ( FracSub()
  | s[1]>>x;
    s[1]>>y;
    link(2,fracnorm,t,1);
    t[2]<<(((x&0)*(y&1))-((y&0)*(x&1)),(x&1)*(y&1));
    t[2]>>z;
    s[1]<<z;
    end
  )
in ( FracSub() | // }}}
(nu fracmult: $fracop)
def FracMult() = // {{{
  link(2,fracmult,s,2);
  ( FracMult()
  | s[1]>>x;
    s[1]>>y;
    link(2,fracnorm,t,1);
    t[2]<<((x&0)*(y&0),(x&1)*(y&1));
    t[2]>>z;
    s[1]<<z;
    end
  )
in ( FracMult() | // }}}
define $fraccomp =   // Protocol for binary operation on Fractions {{{
  1=>2<(Int,Int)>; // First fraction (nominator,denominator)
  1=>2<(Int,Int)>; // Second fraction (nominator,denominator)
  2=>1<Bool>;      // Result of comparrison
  Gend
in // }}}
(nu fracleq: $fraccomp)
def FracLEQ() = // {{{
  link(2,fracleq,s,2);
  ( FracLEQ()
  | s[1]>>x;
    s[1]>>y;
    s[1]<<((x&0)*(y&1))<=((y&0)*(x&1));
    end
  )
in ( FracLEQ() | // }}}
define $sqrt = // Protocol for square-root query {{{
  1=>2<(Int,Int)>; // Arg fraction (nominator,denominator)
  1=>2<(Int,Int)>; // Minimum search bound
  1=>2<(Int,Int)>; // Maximum search bound
  1=>2<(Int,Int)>; // Precission
  2=>1<(Int,Int)>; // Result fraction (nominator,denominator)
  Gend
in // }}}
(nu sqrt: $sqrt)
def Sqrt() = // {{{
  link(2,sqrt,s,2);
  ( Sqrt()
  | s[1]>>square;              // Read input
    s[1]>>min;
    s[1]>>max;
    s[1]>>precission;
    link(2,fracsub,sub1,1);    // Find precission
    sub1[2]<<max;
    sub1[2]<<min;
    sub1[2]>>diff;
    link(2,fracleq,leq1,1);    // Compare with minimum precission
    leq1[2]<<diff;
    leq1[2]<<precission;
    leq1[2]>>done;
    link(2,fracadd,add1,1);    // Add max and min to find 2*avg
    add1[2]<<min;
    add1[2]<<max;
    add1[2]>>avg2;
    link(2,fracmult,mult1,1);  // Divide by 2 to get avg
    mult1[2]<<avg2;
    mult1[2]<<(1,2);
    mult1[2]>>avg;
    if done
    then s[1] << avg;
         end
    else link(2,fracmult,mult2,1);
         mult2[2]<<avg;
         mult2[2]<<avg;
         mult2[2]>>avgsqr;
         if ((square&0)*(avgsqr&1)) = ((avgsqr&0)*(square&1))
    then s[1]<<avg;
         end
    else link(2,fracleq,leq2,1);
         leq2[2]<<square;
         leq2[2]<<avgsqr;
         leq2[2]>>less;
         if less
    then link(2,sqrt,t,1);
         t[2]<<square;
         t[2]<<min;
         t[2]<<avg;
         t[2]<<precission;
         t[2]>>result;
         s[1]<<result;
         end
    else link(2,sqrt,t,1);
         t[2]<<square;
         t[2]<<avg;
         t[2]<<max;
         t[2]<<precission;
         t[2]>>result;
         s[1]<<result;
         end
  )
in ( Sqrt() | // }}}
  link(2,sqrt,s,1);
  (nu gui: {^Squareroot: {^Quit: Gend}})
  link(1,gui,g,1);
  guisync(1,g,1)
  {^Squareroot(top:Int=9,bot:Int=1):
    s[2]<<(top,bot);
    s[2]<<(0,1);
    s[2]<<(top,bot);
    s[2]<<(1,1000);
    s[2]>>result;
    guivalue(1,g,1,"Result",result);
    guisync(1,g,1)
    {^Quit(comment:String=""):
      end
    }
  }
)))))))
