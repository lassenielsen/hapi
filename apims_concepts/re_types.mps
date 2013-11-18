(nu a: 1=>2:1<\RE{(a+b)*}>; 2=>1:2<\RE{a*}>;Gend)
( def CollectAsService() =
    link(2,a,s,2);
    s[1]>>str;
    def CollectAs(abs : \RE{(a+b)*}, s: 2=>1:2<\RE{a*}@(2 of 2) ) =
      match abs
      { b* => s[1]<<nil;end,
        (b*,(a,tail:(a+b)*)) => s[1]<<cons; CollectAs(tail,s)
      }
    in CollectAs(str,s)
  in CollectAsService()
| link(2,a,s,1);
  s[1]<< \RE{(a+b)*}("abba"); // Sends cons inl cons inr cons inr cons inl nil
  s[2]>> x; // Receives \RE{a*}("aa") = cons cons nil;
)
