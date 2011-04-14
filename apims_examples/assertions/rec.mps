// The protocol states the following:
// The possible actions are A, B, C, D and END
// Each action must be executed at most once
// END ends the session
// Before the session is ended, A and B must be executed
define $protocol a b c d =
  rec $state<a:Bool=a, b:Bool=b, c:Bool=c, d:Bool=d>.
  {^A[[not a]]: $state<true,b,c,d>,
   ^B[[not b]]: $state<a,true,c,d>,
   #C[[not c]]: $state<a,b,true,d>,
   #D[[not d]]: $state<a,b,c,true>,
   ^END[[a and b]]: Gend
  }
in
(nu a: $protocol<false,false,false,false>)
( def X<s1:Bool, s2:Bool, s3:Bool, s4:Bool>(s: $protocol<s1, s2, s3, s4>@(1of1) ) =
    if (s1)
    then sync(1,s)
         {^B[[not s2]]: X<s1,true,s3,s4>(s),
          ^END[[s2]]: end
         }
    else sync(1,s)
         {^A[[not s1]]: X<true,s2,s3,s4>(s),
          ^B[[not s2]]: X<s1,true,s3,s4>(s),
          #C[[not s2 and not s3]]: X<s1,s2,true,s4>(s),
          #D[[not s3 and not s4]]: X<s1,s2,s3,true>(s)
         }
  in link(1,a,s,1);
     X<false,false,false,false>(s)
)
