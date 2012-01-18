// Simple workflow with two (parrallel) tests, {{{
// and treatment depending on the results.
// Participants:
// 1: Patient
// 2: Doctor
// 3: Nurse 1
// 4: Nurse 2
define $wf2 r1 r2 adm =
  rec $treat<adm:Bool=adm>.
  {#Administer[[(r1 or r2) and not adm]]:
    2=>1:1<String>;
    $treat<true>,
   ^Discharge:
    Gend
  }
in
define $wf t1 r1 t2 r2 =
  rec $tests<t1:Bool=t1,r1:Bool=r1,t2:Bool=t2,r2:Bool=r2>.
  {^Test1[[not t1]]:
    3=>4:4<Bool> as x;
    3=>2:2<Bool> as y[[(x or not y) and (y or not x)]];
    3=>1:1<Bool> as z[[(x or not z) and (z or not x)]];
    $tests<true,x,t2,r2>,
   ^Test2[[not t2]]:
    4=>3:3<Bool> as x;
    4=>2:2<Bool> as y[[(x or not y) and (y or not x)]];
    4=>1:1<Bool> as z[[(x or not z) and (z or not x)]];
    $tests<t1,r1,true,x>,
   ^Treat[[t1 and t2]]:
    $wf2<r1,r2,false>
  }
in // }}}
(nu wf: $wf<false,false,false,false>)
(  // Patient GUI Implementation {{{
  def Tests<t1:Bool,r1:Bool,t2:Bool,r2:Bool>(w: $wf<t1,r1,t2,r2>@(1of4))=
    guisync(4,w,1)
    {^Test1[[not t1]]():
      w[1]>>result;
      Tests<true,result,t2,r2>(w),
     ^Test2[[not t2]]():
      w[1]>>result;
      Tests<t1,r1,true,result>(w),
     ^Treat[[t1 and t2]]():
      guivalue(4,w,1,"Result of first test",r1);
      guivalue(4,w,1,"Result of second test",r2);
      def Treat<r1:Bool,r2:Bool,adm:Bool>(w: $wf2<r1,r2,adm>@(1of4))=
        guisync(4,w,1)
        {#Administer[[(r1 or r2) and not adm]](comment:String=""):
          w[1]>>prescription;
          guivalue(4,w,1,"Received medication",prescription);
          Treat<r1,r2,true>(w),
         ^Discharge(comment:String="Treatment complete"):
          end          
        }
      in Treat<r1,r2,false>(w)
    }
  in
    link(4,wf,treatment,1);
    guivalue(4,treatment,1,"User","Patient");
    Tests<false,false,false,false>(treatment)
  // }}}
|  // Doctor GUI Implementation {{{
  def Tests<t1:Bool,r1:Bool,t2:Bool,r2:Bool>(w: $wf<t1,r1,t2,r2>@(2of4))=
    guisync(4,w,2)
    {^Test1[[not t1]]():
      w[2]>>result;
      guivalue(4,w,2,"Result of first test",result);
      Tests<true,result,t2,r2>(w),
     ^Test2[[not t2]]():
      w[2]>>result;
      guivalue(4,w,2,"Result of second test",result);
      Tests<t1,r1,true,result>(w),
     ^Treat[[t1 and t2]]():
      def Treat<r1:Bool,r2:Bool,adm:Bool>(w: $wf2<r1,r2,adm>@(2of4))=
        guisync(4,w,2)
        {#Administer[[r1 and r2 and not adm]](prescription:String="One week of penicillin"):
          guivalue(4,w,2,"Administered",prescription);
          w[1]<<prescription;
          Treat<r1,r2,true>(w),
         ^Discharge(comment:String="Treatment complete"):
          end          
        }
      in Treat<r1,r2,false>(w)
    }
  in
    link(4,wf,treatment,2);
    guivalue(4,treatment,2,"User","Doctor");
    Tests<false,false,false,false>(treatment)
  // }}}
)
