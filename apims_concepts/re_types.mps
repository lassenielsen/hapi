(nu a: 1=>2<String[(a+b)*]>; 2=>1<String[a*]>;Gend)
( def CollectAsService() =
    link(2,a,s,2);
    s[1]>>str;
    def CollectAs(abs : String[(a+b)*], s: 2=>1:2<String[a*]>@(2 of 2) ) =
      match abs
      { r"b*" => s[1]<<nil;end,
        r"(b*a)(a+b)*" as ((bs,a),abs2) => s[1]<<cons; CollectAs(abs2,s)
      }
    in CollectAs(str,s)
  in CollectAsService()
| link(2,a,s,1);
  s[1]<< String[(a+b)*]("abba"); // Sends cons inl cons inr cons inr cons inl nil
  s[2]>> x; // Receives String[a*]("aa") = cons cons nil;
)
