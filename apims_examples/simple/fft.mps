(nu gcd: 1=>2:1<Int>;1=>2:1<Int>;2=>1:2<Int>;Gend)
def GCD() = // Greatest Common Divisor Service, used for fraction normalization {{{
  link(2,gcd,s,2);
  ( GCD()
  | def GcdFun(n: Int,
               m: Int,
               s: 2<<<Int>;Lend@(2of2)) =
      if (n+1)<=0
      then GcdFun(0-n,m,s)
      else if (m+1)<=0
      then GcdFun(n,0-m,s)
      else if (n+1)<=m
      then GcdFun(m,n,s)
      else if 1<=m
      then GcdFun(m,n-((n/m)*m),s)
      else s[2] << n;
           end
    in  
      s[1]>>n;
      s[1]>>m;
      GcdFun(n,m,s)
  )
in ( GCD() | // }}}
define $fracunop = // Protocol for unary operation on fractions {{{
  1=>2:1<(Int,Int)>; // Arg fraction (nominator,denominator)
  2=>1:2<(Int,Int)>; // Result fraction (nominator,denominator)
  Gend
in // }}}
(nu fracnorm: $fracunop)
def FracNorm() = // Fraction Normalisation Service {{{
  link(2,fracnorm,s,2);
  ( FracNorm()
  | s[1]>>x;
    link(2,gcd,t,1);
    t[1]<<x&0;
    t[1]<<x&1;
    t[2]>>div;
    if 0<=(x&1)
    then s[2]<<((x&0)/div,(x&1)/div);
         end
    else s[2]<<(0-((x&0)/div),0-((x&1)/div));
         end
  )
in ( FracNorm() | // }}}
define $fracop = // Protocol for binary operation on Fractions {{{
  1=>2:1<(Int,Int)>; // First fraction (nominator,denominator)
  1=>2:1<(Int,Int)>; // Second fraction (nominator,denominator)
  2=>1:2<(Int,Int)>; // Result fraction (nominator,denominator)
  Gend
in // }}}
(nu fracadd: $fracop)
def FracAdd() = // Fraction Addition Service {{{
  link(2,fracadd,s,2);
  ( FracAdd()
  | s[1]>>x;
    s[1]>>y;
    link(2,fracnorm,t,1);
    t[1]<<(((x&0)*(y&1))+((y&0)*(x&1)),(x&1)*(y&1));
    t[2]>>z;
    s[2]<<z;
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
    t[1]<<(((x&0)*(y&1))-((y&0)*(x&1)),(x&1)*(y&1));
    t[2]>>z;
    s[2]<<z;
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
    t[1]<<((x&0)*(y&0),(x&1)*(y&1));
    t[2]>>z;
    s[2]<<z;
    end
  )
in ( FracMult() | // }}}
define $compop = // {{{
  1=>2:1<((Int,Int),(Int,Int))>; // First arg
  1=>2:1<((Int,Int),(Int,Int))>; // Second arg
  2=>1:2<((Int,Int),(Int,Int))>; // Result
  Gend
in // }}}
(nu compadd: $compop)
def CompAdd() = // {{{
  link(2,compadd,s,2);
  ( CompAdd()
  | s[1]>>x;
    s[1]>>y;
    link(2,fracadd,t1,1);
    t1[1]<<x&0;
    t1[1]<<y&0;
    t1[2]>>z1;
    link(2,fracadd,t2,1);
    t2[1]<<x&1;
    t2[1]<<y&1;
    t2[2]>>z2;
    s[2]<<(z1,z2);
    end
  )
in ( CompAdd() | // }}}
(nu compmult: $compop)
def CompMult() = // {{{
  link(2,compmult,s,2);
  ( CompMult()
  | s[1]>>x;
    s[1]>>y;
    link(2,fracmult,t1,1);
    t1[1]<<x&0;
    t1[1]<<y&0;
    t1[2]>>z11;
    link(2,fracmult,t2,1);
    t2[1]<<x&1;
    t2[1]<<y&1;
    t2[2]>>z22;
    link(2,fracmult,t3,1);
    t3[1]<<x&0;
    t3[1]<<y&1;
    t3[2]>>z12;
    link(2,fracmult,t4,1);
    t4[1]<<x&1;
    t4[1]<<y&0;
    t4[2]>>z21;
    link(2,fracsub,t5,1);
    t5[1]<<z11;
    t5[1]<<z22;
    t5[2]>>z1;
    link(2,fracadd,t6,1);
    t6[1]<<z12;
    t6[1]<<z21;
    t6[2]>>z2;
    s[2]<<(z1,z2);
    end
  )
in ( CompMult() | // }}}
define $compexp = // {{{
  1=>2:1<((Int,Int),(Int,Int))>; // First arg
  1=>2:1<Int>;                   // Exponent
  2=>1:2<((Int,Int),(Int,Int))>; // Result
  Gend
in // }}}
(nu compexp: $compexp)
def CompExp() = // {{{
  link(2,compexp,s,2);
  ( CompExp()
  | s[1]>>x;
    def Exp(res: ((Int,Int),(Int,Int)),
            c: Int,
            s: 2<<<((Int,Int),(Int,Int))>;Lend@(2of2)) =
      if c<=0
      then s[2]<<res;
           end
      else link(2,compmult,t,1);
           t[1]<<res;
           t[1]<<x;
           t[2]>>res;
           Exp(res,c-1,s)
    in
      s[1]>>z;
      Exp(((1,1),(0,1)),z,s)
  )
in ( CompExp() | // }}}
(nu fft: // FFT protocol {{{
  // Type is described as:
  // Pi n.
  //  foreach(i<2^n){i->i:<nat>};
  //  foreach(l<n) {            // Number of steps
  //   foreach(i<2^l) {         // Number of butterflies in step (n-l)
  //    foreach(j<2^(n-l-1)) {  // Number of pairs in butterfly
  //     foreach(k<2) {         // The four arrows
  //      foreach(k'<2) {       // in a butterfly
  //       i*2^(n-1)+k*2^(n-l-1)+j -> i*2^(n-l)+k'*2^(n-l-1)+j :<nat> }} }} }
  // We use n=3
  // Phase One
  1=>1:1<((Int,Int),(Int,Int))>;  // i=0
  2=>2:10<((Int,Int),(Int,Int))>; // i=1
  3=>3:19<((Int,Int),(Int,Int))>; // i=2
  4=>4:28<((Int,Int),(Int,Int))>; // i=3
  5=>5:37<((Int,Int),(Int,Int))>; // i=4
  6=>6:46<((Int,Int),(Int,Int))>; // i=5
  7=>7:55<((Int,Int),(Int,Int))>; // i=6
  8=>8:64<((Int,Int),(Int,Int))>; // i=7
  // Phase Two
  // l=2, i=0, j=0
  1=>2:2<((Int,Int),(Int,Int))>;  // k=0, k'=1
  2=>1:9<((Int,Int),(Int,Int))>;  // k=1, k'=0
  1=>1:1<((Int,Int),(Int,Int))>;  // k=0, k'=0
  2=>2:10<((Int,Int),(Int,Int))>; // k=1, k'=1
  // l=2, i=1, j=0
  3=>4:20<((Int,Int),(Int,Int))>; // k=0, k'=1
  4=>3:27<((Int,Int),(Int,Int))>; // k=1, k'=0
  3=>3:19<((Int,Int),(Int,Int))>; // k=0, k'=0
  4=>4:28<((Int,Int),(Int,Int))>; // k=1, k'=1
  // l=2, i=2, j=0
  5=>6:38<((Int,Int),(Int,Int))>; // k=0, k'=1
  6=>5:45<((Int,Int),(Int,Int))>; // k=1, k'=0
  5=>5:37<((Int,Int),(Int,Int))>; // k=0, k'=0
  6=>6:46<((Int,Int),(Int,Int))>; // k=1, k'=1
  // l=2, i=3, j=0
  7=>8:56<((Int,Int),(Int,Int))>; // k=0, k'=1
  8=>7:63<((Int,Int),(Int,Int))>; // k=1, k'=0
  7=>7:55<((Int,Int),(Int,Int))>; // k=0, k'=0
  8=>8:64<((Int,Int),(Int,Int))>; // k=1, k'=1
  // l=1, i=0, j=0
  1=>3:3<((Int,Int),(Int,Int))>;  // k=0, k'=1
  3=>1:17<((Int,Int),(Int,Int))>; // k=1, k'=0
  1=>1:1<((Int,Int),(Int,Int))>;  // k=0, k'=0
  3=>3:19<((Int,Int),(Int,Int))>; // k=1, k'=1
  // l=1, i=0, j=1
  2=>4:12<((Int,Int),(Int,Int))>; // k=0, k'=1
  4=>2:26<((Int,Int),(Int,Int))>; // k=1, k'=0
  2=>2:10<((Int,Int),(Int,Int))>; // k=0, k'=0
  4=>4:28<((Int,Int),(Int,Int))>; // k=1, k'=1
  // l=1, i=1, j=0
  5=>7:39<((Int,Int),(Int,Int))>; // k=0, k'=1
  7=>5:53<((Int,Int),(Int,Int))>; // k=1, k'=0
  5=>5:37<((Int,Int),(Int,Int))>; // k=0, k'=0
  7=>7:55<((Int,Int),(Int,Int))>; // k=1, k'=1
  // l=1, i=1, j=1
  6=>8:48<((Int,Int),(Int,Int))>; // k=0, k'=1
  8=>6:62<((Int,Int),(Int,Int))>; // k=1, k'=0
  6=>6:46<((Int,Int),(Int,Int))>; // k=0, k'=0
  8=>8:64<((Int,Int),(Int,Int))>; // k=1, k'=1
  // l=0, i=0, j=0
  1=>5:5<((Int,Int),(Int,Int))>;  // k=0, k'=1
  5=>1:33<((Int,Int),(Int,Int))>; // k=1, k'=0
  1=>1:1<((Int,Int),(Int,Int))>;  // k=0, k'=0
  5=>5:37<((Int,Int),(Int,Int))>; // k=1, k'=1
  // l=0, i=0, j=1
  2=>6:14<((Int,Int),(Int,Int))>; // k=0, k'=1
  6=>2:42<((Int,Int),(Int,Int))>; // k=1, k'=0
  2=>2:10<((Int,Int),(Int,Int))>; // k=0, k'=0
  6=>6:46<((Int,Int),(Int,Int))>; // k=1, k'=1
  // l=0, i=0, j=2
  3=>7:23<((Int,Int),(Int,Int))>; // k=0, k'=1
  7=>3:51<((Int,Int),(Int,Int))>; // k=1, k'=0
  3=>3:19<((Int,Int),(Int,Int))>; // k=0, k'=0
  7=>7:55<((Int,Int),(Int,Int))>; // k=1, k'=1
  // l=0, i=0, j=3
  4=>8:32<((Int,Int),(Int,Int))>; // k=0, k'=1
  8=>4:60<((Int,Int),(Int,Int))>; // k=1, k'=0
  4=>4:28<((Int,Int),(Int,Int))>; // k=0, k'=0
  8=>8:64<((Int,Int),(Int,Int))>; // k=1, k'=1
  Gend)   // }}}
omega1 : ((Int,Int),(Int,Int)) = ((1000,1414),(1000,1414)); // (1+i)/sqrt(2)
omega2 : ((Int,Int),(Int,Int)) = ((0,1),(1,1)); // i
omega3 : ((Int,Int),(Int,Int)) = ((0-1000,1414),(1000,1414)); // (-1+i)/sqrt(2)
(
  // Process 0 {{{
  link(8,fft,s,1);
  s[1]<<((0,1),(0,1)); // x0=0
  // l=2
  s[1]>>x;
  s[2]<<x;
  s[9]>>z;
  // Calculate z+x*omega^0=z+x
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[1]<<x;
  // l=1
  s[1]>>x;
  s[3]<<x;
  s[17]>>z;
  // Calculate z+x*omega^0=z+x
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[1]<<x;
  // l=0
  s[1]>>x;
  s[5]<<x;
  s[33]>>z;
  // Calculate z+x*omega^0=z+x
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[1]<<x;
  // DONE
  ((nu a:Gend)link(2,a,bot,1); s[1]>>x; end)
  // }}}
  // Process 1 {{{
| link(8,fft,s,2);
  s[10]<<((1,1),(0,1)); // x1=1
  // l=2
  s[10]>>x;
  s[2]>>z;
  s[9]<<x;
  // Calculate z+x*omega^1
  link(2,compmult,t,1);
  t[1]<<x;
  t[1]<<omega1;
  t[2]>>x;
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[10]<<x;
  // l=1
  s[10]>>x;
  s[12]<<x;
  s[26]>>z;
  // Calculate z+x*omega^1
  link(2,compmult,t,1);
  t[1]<<x;
  t[1]<<omega1;
  t[2]>>x;
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[10]<<x;
  // l=0
  s[10]>>x;
  s[14]<<x;
  s[42]>>z;
  // Calculate z+x*omega^0=z+x
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[10]<<x;
  // DONE
  ((nu a:Gend)link(2,a,bot,1); s[10]>>x; end)
  // }}}
  // Process 2 {{{
| link(8,fft,s,3);
  s[19]<<((2,1),(0,1)); // x2=2
  // l=2
  s[19]>>x;
  s[20]<<x;
  s[27]>>z;
  // Calculate z+x*omega^2
  link(2,compmult,t,1);
  t[1]<<x;
  t[1]<<omega2;
  t[2]>>x;
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[19]<<x;
  // l=1
  s[19]>>x;
  s[3]>>z;
  s[17]<<x;
  // Calculate z+x*omega^0=z+x
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[19]<<x;
  // l=0
  s[19]>>x;
  s[23]<<x;
  s[51]>>z;
  // Calculate z+x*omega^0
  link(2,compmult,t,1);
  t[1]<<x;
  t[1]<<omega2;
  t[2]>>x;
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[19]<<x;
  // DONE
  ((nu a:Gend)link(2,a,bot,1); s[19]>>x; end)
  // }}}
  // Process 3 {{{
| link(8,fft,s,4);
  s[28]<<((3,1),(0,1)); // x3=3
  // l=2
  s[28]>>x;
  s[20]>>z;
  s[27]<<x;
  // Calculate z+x*omega^3
  link(2,compmult,t,1);
  t[1]<<x;
  t[1]<<omega3;
  t[2]>>x;
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[28]<<x;
  // l=1
  s[28]>>x;
  s[12]>>z;
  s[26]<<x;
  // Calculate z+x*omega^1
  link(2,compmult,t,1);
  t[1]<<x;
  t[1]<<omega1;
  t[2]>>x;
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[28]<<x;
  // l=0
  s[28]>>x;
  s[32]<<x;
  s[60]>>z;
  // Calculate z+x*omega^0=z+x
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[28]<<x;
  // DONE
  ((nu a:Gend)link(2,a,bot,1); s[28]>>x; end)
  // }}}
  // Process 4 {{{
| link(8,fft,s,5);
  s[37]<<((4,1),(0,1)); // x4=4
  // l=2
  s[37]>>x;
  s[38]<<x;
  s[45]>>z;
  // Calculate z+x*omega^0=z+x
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[37]<<x;
  // l=1
  s[37]>>x;
  s[39]<<x;
  s[53]>>z;
  // Calculate z+x*omega^0=z+x
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[37]<<x;
  // l=0
  s[37]>>x;
  s[5]>>z;
  s[33]<<x;
  // Calculate z+x*omega^0=z+x
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[37]<<x;
  // DONE
  ((nu a:Gend)link(2,a,bot,1); s[37]>>x; end)
  // }}}
  // Process 5 {{{
| link(8,fft,s,6);
  s[46]<<((2,1),(0,1)); // x5=2
  // l=2
  s[46]>>x;
  s[38]>>z;
  s[45]<<x;
  // Calculate z+x*omega^1
  link(2,compmult,t,1);
  t[1]<<x;
  t[1]<<omega1;
  t[2]>>x;
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[46]<<x;
  // l=1
  s[46]>>x;
  s[48]<<x;
  s[62]>>z;
  // Calculate z+x*omega^1
  link(2,compmult,t,1);
  t[1]<<x;
  t[1]<<omega1;
  t[2]>>x;
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[46]<<x;
  // l=0
  s[46]>>x;
  s[14]>>z;
  s[42]<<x;
  // Calculate z+x*omega^0=z+x
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[46]<<x;
  // DONE
  ((nu a:Gend)link(2,a,bot,1); s[46]>>x; end)
  // }}}
  // Process 6 {{{
| link(8,fft,s,7);
  s[55]<<((0,1),(0,1)); // x6=0
  // l=2
  s[55]>>x;
  s[56]<<x;
  s[63]>>z;
  // Calculate z+x*omega^2
  link(2,compmult,t,1);
  t[1]<<x;
  t[1]<<omega2;
  t[2]>>x;
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[55]<<x;
  // l=1
  s[55]>>x;
  s[39]>>z;
  s[53]<<x;
  // Calculate z+x*omega^0=z+x
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[55]<<x;
  // l=0
  s[55]>>x;
  s[23]>>z;
  s[51]<<x;
  // Calculate z+x*omega^0=z+x
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[55]<<x;
  // DONE
  ((nu a:Gend)link(2,a,bot,1); s[55]>>x; end)
  // }}}
  // Process 7 {{{
| link(8,fft,s,8);
  s[64]<<((0-2,1),(0,1)); // x7=-2
  // l=2
  s[64]>>x;
  s[56]>>z;
  s[63]<<x;
  // Calculate z+x*omega^3
  link(2,compmult,t,1);
  t[1]<<x;
  t[1]<<omega3;
  t[2]>>x;
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[64]<<x;
  // l=1
  s[64]>>x;
  s[48]>>z;
  s[62]<<x;
  // Calculate z+x*omega^1
  link(2,compmult,t,1);
  t[1]<<x;
  t[1]<<omega1;
  t[2]>>x;
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[64]<<x;
  // l=0
  s[64]>>x;
  s[32]>>z;
  s[60]<<x;
  // Calculate z+x*omega^0=z+x
  link(2,compadd,t,1);
  t[1]<<z;
  t[1]<<x;
  t[2]>>x;
  s[64]<<x;
  // DONE
  ((nu a:Gend)link(2,a,bot,1); s[64]>>x; end)
  // }}}
) ) ) ) ) ) ) ) )
