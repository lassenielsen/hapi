define $wf t1 t2 ad r1 r2 =
  rec $workflow<test1:Bool=t1,test2:Bool=t2,administer:Bool=ad,result1:Bool=r1,result2:Bool=r2>.
    {^Test1[[not test1]]:
      3=>1:1<Bool> as x;
      3=>2:2<Bool> as y[[(x or not y) and (y or not x)]];
      $workflow<true,test2,administer,x,r2>,
     ^Test2[[not test2]]:
      3=>1:1<Bool> as x;
      3=>2:2<Bool> as y[[(x or not y) and (y or not x)]];
      $workflow<test1,true,administer,result1,x>,
     ^Administer[[test1 and test2 and (not administer) and not (result1 and result2)]]:
      $workflow<test1,test2,true,result1,result2>,
     ^Discharge[[test1 and test2 and (result1 or result2 or administer)]]:
      Gend
    }
in
(nu wf: $wf<false,false,false,false,false>)
( link(3,wf,s,1);
  def X<t1:Bool,t2:Bool,adm:Bool,r1:Bool,r2:Bool>(w: $wf<t1,t2,adm,r1,r2>@(1of3))=
    guisync(3,w,1)
    {^Test1[[not t1]]():
      w[1]>>result;
      X<true,t2,adm,result,r2>(w),
     ^Test2[[not t2]]():
      w[1]>>result;
      X<t1,true,adm,r1,result>(w),
     ^Administer[[t1 and t2 and (not adm) and not (r1 and r2)]]():
      X<t1,t2,true,r1,r2>(w),
     ^Discharge[[t1 and t2 and (adm or r1 or r2)]]():
      end
    }
  in X<false,false,false,false,false>(s)
)
