/* Pre compilation output:
******************* Program *******************
HOSTHEADER(""#include <iostream>"");
rec $console;
2->1
{ ^End where true:
    $end;,
  ^GetInt where true:
    1->2:Int;
    $console,
  ^GetString where true:
    1->2:String;
    $console,
  ^PutInt where true:
    2->1:Int;
    $console,
  ^PutString where true:
    2->1:String;
    $console,
  ^Stop where true:
    $end;
} console(1, 2);
( local Console()
  ( s= new console(1 of 2);
    local ConsoleSession(rec %console;
                         2>>
                         { ^End where true:
                           %end;,
                           ^GetInt where true:
                           2 << Int;
                           %console,
                           ^GetString where true:
                           2 << String;
                           %console,
                           ^PutInt where true:
                           2 >> Int;
                           %console,
                           ^PutString where true:
                           2 >> String;
                           %console,
                           ^Stop where true:
                           %end;
                         }(1 of 1, 2) s)
    ( s[2] >>
      { ^End:
        Console();,
        ^GetInt:
        Int val=0;
        HOST("{ long _l; std::cin >> _l; ", val", "=shared_ptr<libpi::Int>(new libpi::Int(_l)); }");
        s[2] << val;
        ConsoleSession(s);,
        ^GetString:
        String val="";
        HOST("{ std::string _s; std::cin >> _s; ", val", "=shared_ptr<libpi::String>(new libpi::String(_s)); }");
        s[2] << val;
        ConsoleSession(s);,
        ^PutInt:
        s[2] >> i;
        HOST("std::cout << (", i", ")->ToString() << std::endl;");
        ConsoleSession(s);,
        ^PutString:
        s[2] >> str;
        HOST("std::cout << (", str", ")->GetValue() << std::endl;");
        ConsoleSession(s);,
        ^Stop:
        
      }
    )
    ConsoleSession(s);
  )
  Console();
| 2->1:Int;
  1->2:Int;
  $end; fib(1 pure, 2);
  ( local Fib()
    ( s= new fib(1 of 2);
      ( Fib();
      | s[2] >> x;
        if (x <= 1)
        then s[2] << 1;
             
        else s1= new fib(2 of 2);
             s1[1] << (x - 1);
             s1[1] >> f1;
             s2= new fib(2 of 2);
             s2[1] << (x - 2);
             s2[1] >> f2;
             s[2] << (f1 + f2);
             
      )
    )
    local StartService(Int i)
      ( if (i <= 0)
        then Fib();
        else ( Fib();
             | StartService((i - 1));
             )
      )
      StartService(SYSTEM & "tprocs");
  | c= new console(2 of 2);
    c[1] << ^PutString;
    c[1] << "What is n?";
    c[1] << ^GetInt;
    c[1] >> v;
    f= new fib(2 of 2);
    f[1] << v;
    f[1] >> r;
    c[1] << ^PutString;
    c[1] << "Fib(n) is";
    c[1] << ^PutInt;
    c[1] << r;
    c[1] << ^Stop;
    
  )
)
************ Type Checking Program ************
************ Type Check Succeeded! ************
****** Parallelization Optimized Program ******
HOSTHEADER(""#include <iostream>"");
rec $console;
2->1
{ ^End where true:
    $end;,
  ^GetInt where true:
    1->2:Int;
    $console,
  ^GetString where true:
    1->2:String;
    $console,
  ^PutInt where true:
    2->1:Int;
    $console,
  ^PutString where true:
    2->1:String;
    $console,
  ^Stop where true:
    $end;
} console(1, 2);
( local Console()
  ( s= new console(1 of 2);
    local ConsoleSession(rec %console;
                         2>>
                         { ^End where true:
                           %end;,
                           ^GetInt where true:
                           2 << Int;
                           %console,
                           ^GetString where true:
                           2 << String;
                           %console,
                           ^PutInt where true:
                           2 >> Int;
                           %console,
                           ^PutString where true:
                           2 >> String;
                           %console,
                           ^Stop where true:
                           %end;
                         }(1 of 1, 2) s)
    ( s[2] >>
      { ^End:
        Console();,
        ^GetInt:
        Int val=0;
        HOST("{ long _l; std::cin >> _l; ", val", "=shared_ptr<libpi::Int>(new libpi::Int(_l)); }");
        s[2] << val;
        ConsoleSession(s);,
        ^GetString:
        String val="";
        HOST("{ std::string _s; std::cin >> _s; ", val", "=shared_ptr<libpi::String>(new libpi::String(_s)); }");
        s[2] << val;
        ConsoleSession(s);,
        ^PutInt:
        s[2] >> i;
        HOST("std::cout << (", i", ")->ToString() << std::endl;");
        ConsoleSession(s);,
        ^PutString:
        s[2] >> str;
        HOST("std::cout << (", str", ")->GetValue() << std::endl;");
        ConsoleSession(s);,
        ^Stop:
        
      }
    )
    ConsoleSession(s);
  )
  Console();
| 2->1:Int;
  1->2:Int;
  $end; fib(1 pure, 2);
  ( local Fib()
    ( s= new fib(1 of 2);
      ( Fib();
      | s[2] >> x;
        if (x <= 1)
        then s[2] << 1;
             
        else if (SYSTEM & "tprocs" <= SYSTEM & "aprocs")
             then s1= new fib(2 of 2);
                  s1[1] << (x - 1);
                  s1[1] >> f1;
                  s2= new fib(2 of 2);
                  s2[1] << (x - 2);
                  s2[1] >> f2;
                  s[2] << (f1 + f2);
                  
             else s1= new fib(2 of 2);
                  s1[1] << (x - 1);
                  s2= new fib(2 of 2);
                  s2[1] << (x - 2);
                  s1[1] >> f1;
                  s2[1] >> f2;
                  s[2] << (f1 + f2);
                  
      )
    )
    local StartService(Int i)
      ( if (i <= 0)
        then Fib();
        else ( Fib();
             | StartService((i - 1));
             )
      )
      StartService(SYSTEM & "tprocs");
  | if (SYSTEM & "tprocs" <= SYSTEM & "aprocs")
    then c= new console(2 of 2);
         c[1] << ^PutString;
         c[1] << "What is n?";
         c[1] << ^GetInt;
         c[1] >> v;
         f= new fib(2 of 2);
         f[1] << v;
         f[1] >> r;
         c[1] << ^PutString;
         c[1] << "Fib(n) is";
         c[1] << ^PutInt;
         c[1] << r;
         c[1] << ^Stop;
         
    else c= new console(2 of 2);
         c[1] << ^PutString;
         c[1] << "What is n?";
         c[1] << ^GetInt;
         f= new fib(2 of 2);
         c[1] >> v;
         f[1] << v;
         c[1] << ^PutString;
         c[1] << "Fib(n) is";
         c[1] << ^PutInt;
         f[1] >> r;
         c[1] << r;
         c[1] << ^Stop;
         
  )
)
*/
/* ==== ORIGINAL ====
HOSTHEADER(""#include <iostream>"");
rec $console;
2->1
{ ^End where true:
    $end;,
  ^GetInt where true:
    1->2:Int;
    $console,
  ^GetString where true:
    1->2:String;
    $console,
  ^PutInt where true:
    2->1:Int;
    $console,
  ^PutString where true:
    2->1:String;
    $console,
  ^Stop where true:
    $end;
} console(1, 2);
( local Console()
  ( s= new console(1 of 2);
    local ConsoleSession(rec %console;
                         2>>
                         { ^End where true:
                           %end;,
                           ^GetInt where true:
                           2 << Int;
                           %console,
                           ^GetString where true:
                           2 << String;
                           %console,
                           ^PutInt where true:
                           2 >> Int;
                           %console,
                           ^PutString where true:
                           2 >> String;
                           %console,
                           ^Stop where true:
                           %end;
                         }(1 of 1, 2) s)
    ( s[2] >>
      { ^End:
        Console();,
        ^GetInt:
        Int val=0;
        HOST("{ long _l; std::cin >> _l; ", val", "=shared_ptr<libpi::Int>(new libpi::Int(_l)); }");
        s[2] << val;
        ConsoleSession(s);,
        ^GetString:
        String val="";
        HOST("{ std::string _s; std::cin >> _s; ", val", "=shared_ptr<libpi::String>(new libpi::String(_s)); }");
        s[2] << val;
        ConsoleSession(s);,
        ^PutInt:
        s[2] >> i;
        HOST("std::cout << (", i", ")->ToString() << std::endl;");
        ConsoleSession(s);,
        ^PutString:
        s[2] >> str;
        HOST("std::cout << (", str", ")->GetValue() << std::endl;");
        ConsoleSession(s);,
        ^Stop:
        
      }
    )
    ConsoleSession(s);
  )
  Console();
| 2->1:Int;
  1->2:Int;
  $end; fib(1 pure, 2);
  ( local Fib()
    ( s= new fib(1 of 2);
      ( Fib();
      | s[2] >> x;
        if (x <= 1)
        then s[2] << 1;
             
        else if (SYSTEM & "tprocs" <= SYSTEM & "aprocs")
             then s1= new fib(2 of 2);
                  s1[1] << (x - 1);
                  s1[1] >> f1;
                  s2= new fib(2 of 2);
                  s2[1] << (x - 2);
                  s2[1] >> f2;
                  s[2] << (f1 + f2);
                  
             else s1= new fib(2 of 2);
                  s1[1] << (x - 1);
                  s2= new fib(2 of 2);
                  s2[1] << (x - 2);
                  s1[1] >> f1;
                  s2[1] >> f2;
                  s[2] << (f1 + f2);
                  
      )
    )
    local StartService(Int i)
      ( if (i <= 0)
        then Fib();
        else ( Fib();
             | StartService((i - 1));
             )
      )
      StartService(SYSTEM & "tprocs");
  | if (SYSTEM & "tprocs" <= SYSTEM & "aprocs")
    then c= new console(2 of 2);
         c[1] << ^PutString;
         c[1] << "What is n?";
         c[1] << ^GetInt;
         c[1] >> v;
         f= new fib(2 of 2);
         f[1] << v;
         f[1] >> r;
         c[1] << ^PutString;
         c[1] << "Fib(n) is";
         c[1] << ^PutInt;
         c[1] << r;
         c[1] << ^Stop;
         
    else c= new console(2 of 2);
         c[1] << ^PutString;
         c[1] << "What is n?";
         c[1] << ^GetInt;
         f= new fib(2 of 2);
         c[1] >> v;
         f[1] << v;
         c[1] << ^PutString;
         c[1] << "Fib(n) is";
         c[1] << ^PutInt;
         f[1] >> r;
         c[1] << r;
         c[1] << ^Stop;
         
  )
)
*/
/* ==== RENAMED ====
HOSTHEADER(""#include <iostream>"");
rec ~$x1;
2->1
{ ^End where true:
    $end;,
  ^GetInt where true:
    1->2:Int;
    ~$x1,
  ^GetString where true:
    1->2:String;
    ~$x1,
  ^PutInt where true:
    2->1:Int;
    ~$x1,
  ^PutString where true:
    2->1:String;
    ~$x1,
  ^Stop where true:
    $end;
} ~console3(1, 2);
( local ~Console1()
  ( ~s4= new ~console3(1 of 2);
    local ~ConsoleSession2(rec ~%x1;
                           2>>
                           { ^End where true:
                             %end;,
                             ^GetInt where true:
                             2 << Int;
                             ~%x1,
                             ^GetString where true:
                             2 << String;
                             ~%x1,
                             ^PutInt where true:
                             2 >> Int;
                             ~%x1,
                             ^PutString where true:
                             2 >> String;
                             ~%x1,
                             ^Stop where true:
                             %end;
                           }(1 of 1, 2) ~s5)
    ( ~s5[2] >>
      { ^End:
        ~Console1();,
        ^GetInt:
        Int ~val10=0;
        HOST("{ long _l; std::cin >> _l; ", ~val10", "=shared_ptr<libpi::Int>(new libpi::Int(_l)); }");
        ~s5[2] << ~val10;
        ~ConsoleSession2(~s5);,
        ^GetString:
        String ~val11="";
        HOST("{ std::string _s; std::cin >> _s; ", ~val11", "=shared_ptr<libpi::String>(new libpi::String(_s)); }");
        ~s5[2] << ~val11;
        ~ConsoleSession2(~s5);,
        ^PutInt:
        ~s5[2] >> ~i12;
        HOST("std::cout << (", ~i12", ")->ToString() << std::endl;");
        ~ConsoleSession2(~s5);,
        ^PutString:
        ~s5[2] >> ~str13;
        HOST("std::cout << (", ~str13", ")->GetValue() << std::endl;");
        ~ConsoleSession2(~s5);,
        ^Stop:
        
      }
    )
    ~ConsoleSession2(~s4);
  )
  ~Console1();
| 2->1:Int;
  1->2:Int;
  $end; ~fib14(1 pure, 2);
  ( local ~Fib3()
    ( ~s15= new ~fib14(1 of 2);
      ( ~Fib3();
      | ~s15[2] >> ~x16;
        if (~x16 <= 1)
        then ~s15[2] << 1;
             
        else if (SYSTEM & "tprocs" <= SYSTEM & "aprocs")
             then ~s117= new ~fib14(2 of 2);
                  ~s117[1] << (~x16 - 1);
                  ~s117[1] >> ~f118;
                  ~s219= new ~fib14(2 of 2);
                  ~s219[1] << (~x16 - 2);
                  ~s219[1] >> ~f220;
                  ~s15[2] << (~f118 + ~f220);
                  
             else ~s121= new ~fib14(2 of 2);
                  ~s121[1] << (~x16 - 1);
                  ~s222= new ~fib14(2 of 2);
                  ~s222[1] << (~x16 - 2);
                  ~s121[1] >> ~f123;
                  ~s222[1] >> ~f224;
                  ~s15[2] << (~f123 + ~f224);
                  
      )
    )
    local ~StartService4(Int ~i25)
      ( if (~i25 <= 0)
        then ~Fib3();
        else ( ~Fib3();
             | ~StartService4((~i25 - 1));
             )
      )
      ~StartService4(SYSTEM & "tprocs");
  | if (SYSTEM & "tprocs" <= SYSTEM & "aprocs")
    then ~c26= new ~console3(2 of 2);
         ~c26[1] << ^PutString;
         ~c26[1] << "What is n?";
         ~c26[1] << ^GetInt;
         ~c26[1] >> ~v27;
         ~f28= new ~fib14(2 of 2);
         ~f28[1] << ~v27;
         ~f28[1] >> ~r29;
         ~c26[1] << ^PutString;
         ~c26[1] << "Fib(n) is";
         ~c26[1] << ^PutInt;
         ~c26[1] << ~r29;
         ~c26[1] << ^Stop;
         
    else ~c30= new ~console3(2 of 2);
         ~c30[1] << ^PutString;
         ~c30[1] << "What is n?";
         ~c30[1] << ^GetInt;
         ~f31= new ~fib14(2 of 2);
         ~c30[1] >> ~v32;
         ~f31[1] << ~v32;
         ~c30[1] << ^PutString;
         ~c30[1] << "Fib(n) is";
         ~c30[1] << ^PutInt;
         ~f31[1] >> ~r33;
         ~c30[1] << ~r33;
         ~c30[1] << ^Stop;
         
  )
)
*/
/* ==== FLATTENFORKED ====
HOSTHEADER(""#include <iostream>"");
rec ~$x1;
2->1
{ ^End where true:
    $end;,
  ^GetInt where true:
    1->2:Int;
    ~$x1,
  ^GetString where true:
    1->2:String;
    ~$x1,
  ^PutInt where true:
    2->1:Int;
    ~$x1,
  ^PutString where true:
    2->1:String;
    ~$x1,
  ^Stop where true:
    $end;
} ~console3(1, 2);
( local ~Console1()
  ( ~s4= new ~console3(1 of 2);
    local ~ConsoleSession2(rec ~%x1;
                           2>>
                           { ^End where true:
                             %end;,
                             ^GetInt where true:
                             2 << Int;
                             ~%x1,
                             ^GetString where true:
                             2 << String;
                             ~%x1,
                             ^PutInt where true:
                             2 >> Int;
                             ~%x1,
                             ^PutString where true:
                             2 >> String;
                             ~%x1,
                             ^Stop where true:
                             %end;
                           }(1 of 1, 2) ~s5)
    ( ~s5[2] >>
      { ^End:
        ~Console1();,
        ^GetInt:
        Int ~val10=0;
        HOST("{ long _l; std::cin >> _l; ", ~val10", "=shared_ptr<libpi::Int>(new libpi::Int(_l)); }");
        ~s5[2] << ~val10;
        ~ConsoleSession2(~s5);,
        ^GetString:
        String ~val11="";
        HOST("{ std::string _s; std::cin >> _s; ", ~val11", "=shared_ptr<libpi::String>(new libpi::String(_s)); }");
        ~s5[2] << ~val11;
        ~ConsoleSession2(~s5);,
        ^PutInt:
        ~s5[2] >> ~i12;
        HOST("std::cout << (", ~i12", ")->ToString() << std::endl;");
        ~ConsoleSession2(~s5);,
        ^PutString:
        ~s5[2] >> ~str13;
        HOST("std::cout << (", ~str13", ")->GetValue() << std::endl;");
        ~ConsoleSession2(~s5);,
        ^Stop:
        
      }
    )
    ~ConsoleSession2(~s4);
  )
  ~Console1();
| local ~FlatRight7()
  ( 2->1:Int;
    1->2:Int;
    $end; ~fib14(1 pure, 2);
    ( local ~Fib3()
      ( ~s15= new ~fib14(1 of 2);
        ( ~Fib3();
        | local ~FlatRight5()
          ( ~s15[2] >> ~x16;
            if (~x16 <= 1)
            then ~s15[2] << 1;
                 
            else if (SYSTEM & "tprocs" <= SYSTEM & "aprocs")
                 then ~s117= new ~fib14(2 of 2);
                      ~s117[1] << (~x16 - 1);
                      ~s117[1] >> ~f118;
                      ~s219= new ~fib14(2 of 2);
                      ~s219[1] << (~x16 - 2);
                      ~s219[1] >> ~f220;
                      ~s15[2] << (~f118 + ~f220);
                      
                 else ~s121= new ~fib14(2 of 2);
                      ~s121[1] << (~x16 - 1);
                      ~s222= new ~fib14(2 of 2);
                      ~s222[1] << (~x16 - 2);
                      ~s121[1] >> ~f123;
                      ~s222[1] >> ~f224;
                      ~s15[2] << (~f123 + ~f224);
                      
          )
          ~FlatRight5();
        )
      )
      local ~StartService4(Int ~i25)
        ( if (~i25 <= 0)
          then ~Fib3();
          else ( ~Fib3();
               | ~StartService4((~i25 - 1));
               )
        )
        ~StartService4(SYSTEM & "tprocs");
    | local ~FlatRight6()
      ( if (SYSTEM & "tprocs" <= SYSTEM & "aprocs")
        then ~c26= new ~console3(2 of 2);
             ~c26[1] << ^PutString;
             ~c26[1] << "What is n?";
             ~c26[1] << ^GetInt;
             ~c26[1] >> ~v27;
             ~f28= new ~fib14(2 of 2);
             ~f28[1] << ~v27;
             ~f28[1] >> ~r29;
             ~c26[1] << ^PutString;
             ~c26[1] << "Fib(n) is";
             ~c26[1] << ^PutInt;
             ~c26[1] << ~r29;
             ~c26[1] << ^Stop;
             
        else ~c30= new ~console3(2 of 2);
             ~c30[1] << ^PutString;
             ~c30[1] << "What is n?";
             ~c30[1] << ^GetInt;
             ~f31= new ~fib14(2 of 2);
             ~c30[1] >> ~v32;
             ~f31[1] << ~v32;
             ~c30[1] << ^PutString;
             ~c30[1] << "Fib(n) is";
             ~c30[1] << ^PutInt;
             ~f31[1] >> ~r33;
             ~c30[1] << ~r33;
             ~c30[1] << ^Stop;
             
      )
      ~FlatRight6();
    )
  )
  ~FlatRight7();
)
*/
/* ==== CLOSEDEFED ====
HOSTHEADER(""#include <iostream>"");
rec ~$x1;
2->1
{ ^End where true:
    $end;,
  ^GetInt where true:
    1->2:Int;
    ~$x1,
  ^GetString where true:
    1->2:String;
    ~$x1,
  ^PutInt where true:
    2->1:Int;
    ~$x1,
  ^PutString where true:
    2->1:String;
    ~$x1,
  ^Stop where true:
    $end;
} ~console3(1, 2);
( local ~Console1(rec ~$x1;
                  2->1
                  { ^End where true:
                      $end;,
                    ^GetInt where true:
                      1->2:Int;
                      ~$x1,
                    ^GetString where true:
                      1->2:String;
                      ~$x1,
                    ^PutInt where true:
                      2->1:Int;
                      ~$x1,
                    ^PutString where true:
                      2->1:String;
                      ~$x1,
                    ^Stop where true:
                      $end;
                  }(1, 2) ~console3)
  ( ~s4= new ~console3(1 of 2);
    local ~ConsoleSession2(rec ~%x1;
                           2>>
                           { ^End where true:
                             %end;,
                             ^GetInt where true:
                             2 << Int;
                             ~%x1,
                             ^GetString where true:
                             2 << String;
                             ~%x1,
                             ^PutInt where true:
                             2 >> Int;
                             ~%x1,
                             ^PutString where true:
                             2 >> String;
                             ~%x1,
                             ^Stop where true:
                             %end;
                           }(1 of 1, 2) ~s5,
                           rec ~$x1;
                           2->1
                           { ^End where true:
                               $end;,
                             ^GetInt where true:
                               1->2:Int;
                               ~$x1,
                             ^GetString where true:
                               1->2:String;
                               ~$x1,
                             ^PutInt where true:
                               2->1:Int;
                               ~$x1,
                             ^PutString where true:
                               2->1:String;
                               ~$x1,
                             ^Stop where true:
                               $end;
                           }(1, 2) ~console3)
    ( ~s5[2] >>
      { ^End:
        ~Console1(~console3);,
        ^GetInt:
        Int ~val10=0;
        HOST("{ long _l; std::cin >> _l; ", ~val10", "=shared_ptr<libpi::Int>(new libpi::Int(_l)); }");
        ~s5[2] << ~val10;
        ~ConsoleSession2(~s5, ~console3);,
        ^GetString:
        String ~val11="";
        HOST("{ std::string _s; std::cin >> _s; ", ~val11", "=shared_ptr<libpi::String>(new libpi::String(_s)); }");
        ~s5[2] << ~val11;
        ~ConsoleSession2(~s5, ~console3);,
        ^PutInt:
        ~s5[2] >> ~i12;
        HOST("std::cout << (", ~i12", ")->ToString() << std::endl;");
        ~ConsoleSession2(~s5, ~console3);,
        ^PutString:
        ~s5[2] >> ~str13;
        HOST("std::cout << (", ~str13", ")->GetValue() << std::endl;");
        ~ConsoleSession2(~s5, ~console3);,
        ^Stop:
        
      }
    )
    ~ConsoleSession2(~s4, ~console3);
  )
  ~Console1(~console3);
| local ~FlatRight7(rec ~$x1;
                    2->1
                    { ^End where true:
                        $end;,
                      ^GetInt where true:
                        1->2:Int;
                        ~$x1,
                      ^GetString where true:
                        1->2:String;
                        ~$x1,
                      ^PutInt where true:
                        2->1:Int;
                        ~$x1,
                      ^PutString where true:
                        2->1:String;
                        ~$x1,
                      ^Stop where true:
                        $end;
                    }(1, 2) ~console3)
  ( 2->1:Int;
    1->2:Int;
    $end; ~fib14(1 pure, 2);
    ( local ~Fib3(2->1:Int;
                  1->2:Int;
                  $end;(1 pure, 2) ~fib14)
      ( ~s15= new ~fib14(1 of 2);
        ( ~Fib3(~fib14);
        | local ~FlatRight5(2->1:Int;
                            1->2:Int;
                            $end;(1 pure, 2) ~fib14,
                            2 >> Int;
                            2 << Int;
                            %end;(1 of 1 pure, 2) ~s15)
          ( ~s15[2] >> ~x16;
            if (~x16 <= 1)
            then ~s15[2] << 1;
                 
            else if (SYSTEM & "tprocs" <= SYSTEM & "aprocs")
                 then ~s117= new ~fib14(2 of 2);
                      ~s117[1] << (~x16 - 1);
                      ~s117[1] >> ~f118;
                      ~s219= new ~fib14(2 of 2);
                      ~s219[1] << (~x16 - 2);
                      ~s219[1] >> ~f220;
                      ~s15[2] << (~f118 + ~f220);
                      
                 else ~s121= new ~fib14(2 of 2);
                      ~s121[1] << (~x16 - 1);
                      ~s222= new ~fib14(2 of 2);
                      ~s222[1] << (~x16 - 2);
                      ~s121[1] >> ~f123;
                      ~s222[1] >> ~f224;
                      ~s15[2] << (~f123 + ~f224);
                      
          )
          ~FlatRight5(~fib14, ~s15);
        )
      )
      local ~StartService4(Int ~i25,
                             2->1:Int;
                             1->2:Int;
                             $end;(1 pure, 2) ~fib14)
        ( if (~i25 <= 0)
          then ~Fib3(~fib14);
          else ( ~Fib3(~fib14);
               | ~StartService4((~i25 - 1), ~fib14);
               )
        )
        ~StartService4(SYSTEM & "tprocs", ~fib14);
    | local ~FlatRight6(rec ~$x1;
                        2->1
                        { ^End where true:
                            $end;,
                          ^GetInt where true:
                            1->2:Int;
                            ~$x1,
                          ^GetString where true:
                            1->2:String;
                            ~$x1,
                          ^PutInt where true:
                            2->1:Int;
                            ~$x1,
                          ^PutString where true:
                            2->1:String;
                            ~$x1,
                          ^Stop where true:
                            $end;
                        }(1, 2) ~console3,
                        2->1:Int;
                        1->2:Int;
                        $end;(1 pure, 2) ~fib14)
      ( if (SYSTEM & "tprocs" <= SYSTEM & "aprocs")
        then ~c26= new ~console3(2 of 2);
             ~c26[1] << ^PutString;
             ~c26[1] << "What is n?";
             ~c26[1] << ^GetInt;
             ~c26[1] >> ~v27;
             ~f28= new ~fib14(2 of 2);
             ~f28[1] << ~v27;
             ~f28[1] >> ~r29;
             ~c26[1] << ^PutString;
             ~c26[1] << "Fib(n) is";
             ~c26[1] << ^PutInt;
             ~c26[1] << ~r29;
             ~c26[1] << ^Stop;
             
        else ~c30= new ~console3(2 of 2);
             ~c30[1] << ^PutString;
             ~c30[1] << "What is n?";
             ~c30[1] << ^GetInt;
             ~f31= new ~fib14(2 of 2);
             ~c30[1] >> ~v32;
             ~f31[1] << ~v32;
             ~c30[1] << ^PutString;
             ~c30[1] << "Fib(n) is";
             ~c30[1] << ^PutInt;
             ~f31[1] >> ~r33;
             ~c30[1] << ~r33;
             ~c30[1] << ^Stop;
             
      )
      ~FlatRight6(~console3, ~fib14);
    )
  )
  ~FlatRight7(~console3);
)
*/
#include <iostream>
#include <thread>
#include <cstdlib>
#include <pthread.h>
#include <libpi/value.hpp>
#include <libpi/bool.hpp>
#include <libpi/int.hpp>
#include <libpi/float.hpp>
#include <libpi/quotient.hpp>
#include <libpi/string.hpp>
#include <libpi/tuple.hpp>
#include <libpi/session.hpp>
#include <libpi/thread/channel.hpp>
#include <libpi/thread/link.hpp>
#include <vector>
#include <queue>
#include <sstream>
#include <atomic>
#include <memory>
#include <sys/mman.h>
#include <signal.h>
#include <iostream>
using namespace std;
shared_ptr<libpi::Int> intval_0(new libpi::Int("0"));
shared_ptr<libpi::String> stringval_d41d8cd98f00b204e9800998ecf8427e(new libpi::String(""));
shared_ptr<libpi::Int> intval_1(new libpi::Int("1"));
shared_ptr<libpi::Int> intval_2(new libpi::Int("2"));
shared_ptr<libpi::String> stringval_3f31902b37cab22976b013dcd26ed920(new libpi::String("What is n?"));
shared_ptr<libpi::String> stringval_7559769f36c2ade263483f982b001498(new libpi::String("Fib(n) is"));
using namespace libpi;

inline atomic<int> *_new_shared_int()
{ return (std::atomic<int>*)mmap(NULL, sizeof(atomic<int>), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); // Actual number of active processes
}
std::atomic<int> *_aprocs=_new_shared_int(); // Actual number of active processes
inline void _inc_aprocs() // {{{
{ ++(*_aprocs);
} // }}}
inline void _dec_aprocs() // {{{
{ --(*_aprocs);
} // }}}
int _tprocs=std::thread::hardware_concurrency(); // Target number of active processes
std::vector<char*> _args;   // Store args for use in processes

// Declare implementation
void *_methods(void *arg);

struct State
{ void *label;
  vector<shared_ptr<libpi::Value> > values;
};

inline void _spawn_thread(State *state)
{ pthread_t x;
  pthread_attr_t y;
  pthread_attr_init(&y);
  pthread_attr_setstacksize(&y,16384);
  pthread_attr_setdetachstate(&y,PTHREAD_CREATE_DETACHED);
  pthread_create(&x,&y,_methods,(void*)state);
}


/* All methods */
#define _state ((State*)(_arg))
void *_methods(void *_arg)
{
  if (_state==NULL) // Error
  { std::cerr << "Error in call to all_methods: null state provided" << endl;
    return NULL;
  }  
  if (_state->label!=NULL)
    goto *_state->label;
  { // Main
  shared_ptr<thread::Link> __SIM__console3(new thread::Link(2));
  _inc_aprocs();
  { State *__SIM__state146 = new State();
  __SIM__state146->values.push_back( static_pointer_cast<libpi::Value>(__SIM__console3));
  __SIM__state146->label=&&method___SIM__FlatRight7;
    _spawn_thread(__SIM__state146);
  }
  { // Left process
  _state->values.clear();
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__console3));
  _state->label=&&method___SIM__Console1;
  goto *_state->label;
  }
  }

/* Procedure implementations */
  method___SIM__ConsoleSession2: // {{{
  {
    shared_ptr<libpi::thread::Link > __SIM__console3 = static_pointer_cast<libpi::thread::Link>(_state->values.back());
    _state->values.pop_back();
    shared_ptr<libpi::Session > __SIM__s5 = static_pointer_cast<libpi::Session>(_state->values.back());
    _state->values.pop_back();
  { _dec_aprocs();
    shared_ptr<libpi::String> __SIM__branch99 = static_pointer_cast<libpi::String>(__SIM__s5->Receive(1));
    _inc_aprocs();
    if (__SIM__branch99->GetValue()=="^End")
    {
      __SIM__s5->Close(true);
      __SIM__s5 = NULL;
  _state->values.clear();
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__console3));
  _state->label=&&method___SIM__Console1;
  goto *_state->label;
    }
    else 
    if (__SIM__branch99->GetValue()=="^GetInt")
    {
shared_ptr<libpi::Int > __SIM__val10(intval_0);
{ long _l; std::cin >> _l;  __SIM__val10 =shared_ptr<libpi::Int>(new libpi::Int(_l)); }  { 
    __SIM__s5->Send(1,static_pointer_cast<libpi::Value>(__SIM__val10));
  }
  _state->values.clear();
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__s5));
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__console3));
  _state->label=&&method___SIM__ConsoleSession2;
  goto *_state->label;
    }
    else 
    if (__SIM__branch99->GetValue()=="^GetString")
    {
shared_ptr<libpi::String > __SIM__val11(stringval_d41d8cd98f00b204e9800998ecf8427e);
{ std::string _s; std::cin >> _s;  __SIM__val11 =shared_ptr<libpi::String>(new libpi::String(_s)); }  { 
    __SIM__s5->Send(1,static_pointer_cast<libpi::Value>(__SIM__val11));
  }
  _state->values.clear();
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__s5));
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__console3));
  _state->label=&&method___SIM__ConsoleSession2;
  goto *_state->label;
    }
    else 
    if (__SIM__branch99->GetValue()=="^PutInt")
    {
  shared_ptr<libpi::Int > __SIM__i12;
  _dec_aprocs();
  __SIM__i12 = static_pointer_cast<libpi::Int >(__SIM__s5->Receive(1));
  _inc_aprocs();
std::cout << ( __SIM__i12 )->ToString() << std::endl;  _state->values.clear();
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__s5));
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__console3));
  _state->label=&&method___SIM__ConsoleSession2;
  goto *_state->label;
    }
    else 
    if (__SIM__branch99->GetValue()=="^PutString")
    {
  shared_ptr<libpi::String > __SIM__str13;
  _dec_aprocs();
  __SIM__str13 = static_pointer_cast<libpi::String >(__SIM__s5->Receive(1));
  _inc_aprocs();
std::cout << ( __SIM__str13 )->GetValue() << std::endl;  _state->values.clear();
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__s5));
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__console3));
  _state->label=&&method___SIM__ConsoleSession2;
  goto *_state->label;
    }
    else 
    if (__SIM__branch99->GetValue()=="^Stop")
    {
      __SIM__s5->Close(true);
      __SIM__s5 = NULL;
  _dec_aprocs();
  delete _state;
  return NULL;
    }
    else throw (string)"Unknown branch: __SIM__branch99";
  }
  } // }}}
  method___SIM__Console1: // {{{
  {
    shared_ptr<libpi::thread::Link > __SIM__console3 = static_pointer_cast<libpi::thread::Link>(_state->values.back());
    _state->values.pop_back();
  _dec_aprocs();
  shared_ptr<Session> __SIM__s4 = __SIM__console3->Connect(0, 2);
  _inc_aprocs();
  _state->values.clear();
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__s4));
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__console3));
  _state->label=&&method___SIM__ConsoleSession2;
  goto *_state->label;
  } // }}}
  method___SIM__FlatRight5: // {{{
  {
    shared_ptr<libpi::Session > __SIM__s15 = static_pointer_cast<libpi::Session>(_state->values.back());
    _state->values.pop_back();
    shared_ptr<libpi::thread::Link > __SIM__fib14 = static_pointer_cast<libpi::thread::Link>(_state->values.back());
    _state->values.pop_back();
  shared_ptr<libpi::Int > __SIM__x16;
  _dec_aprocs();
  __SIM__x16 = static_pointer_cast<libpi::Int >(__SIM__s15->Receive(1));
  _inc_aprocs();
    shared_ptr<libpi::Bool> __SIM__binop102((*__SIM__x16) <= (*intval_1));
  if (__SIM__binop102->GetValue())
  {
  { 
    __SIM__s15->Send(1,static_pointer_cast<libpi::Value>(intval_1));
  }
  __SIM__s15->Close(true);
  __SIM__s15=NULL;
  _dec_aprocs();
  delete _state;
  return NULL;
  }
  else
  {
    shared_ptr<libpi::Int> __SIM__sysval104(new libpi::Int(_tprocs));
    shared_ptr<libpi::Int> __SIM__sysval105(new libpi::Int(*_aprocs));
    shared_ptr<libpi::Bool> __SIM__binop103((*__SIM__sysval104) <= (*__SIM__sysval105));
  if (__SIM__binop103->GetValue())
  {
  _dec_aprocs();
  shared_ptr<Session> __SIM__s117 = __SIM__fib14->Connect(1, 2);
  _inc_aprocs();
  { 
    shared_ptr<libpi::Int> __SIM__binop113((*__SIM__x16) - (*intval_1));
    __SIM__s117->Send(0,static_pointer_cast<libpi::Value>(__SIM__binop113));
  }
  shared_ptr<libpi::Int > __SIM__f118;
  _dec_aprocs();
  __SIM__f118 = static_pointer_cast<libpi::Int >(__SIM__s117->Receive(0));
  _inc_aprocs();
  __SIM__s117->Close(true);
  __SIM__s117=NULL;
  _dec_aprocs();
  shared_ptr<Session> __SIM__s219 = __SIM__fib14->Connect(1, 2);
  _inc_aprocs();
  { 
    shared_ptr<libpi::Int> __SIM__binop115((*__SIM__x16) - (*intval_2));
    __SIM__s219->Send(0,static_pointer_cast<libpi::Value>(__SIM__binop115));
  }
  shared_ptr<libpi::Int > __SIM__f220;
  _dec_aprocs();
  __SIM__f220 = static_pointer_cast<libpi::Int >(__SIM__s219->Receive(0));
  _inc_aprocs();
  __SIM__s219->Close(true);
  __SIM__s219=NULL;
  { 
    shared_ptr<libpi::Int> __SIM__binop117((*__SIM__f118) + (*__SIM__f220));
    __SIM__s15->Send(1,static_pointer_cast<libpi::Value>(__SIM__binop117));
  }
  __SIM__s15->Close(true);
  __SIM__s15=NULL;
  _dec_aprocs();
  delete _state;
  return NULL;
  }
  else
  {
  _dec_aprocs();
  shared_ptr<Session> __SIM__s121 = __SIM__fib14->Connect(1, 2);
  _inc_aprocs();
  { 
    shared_ptr<libpi::Int> __SIM__binop107((*__SIM__x16) - (*intval_1));
    __SIM__s121->Send(0,static_pointer_cast<libpi::Value>(__SIM__binop107));
  }
  _dec_aprocs();
  shared_ptr<Session> __SIM__s222 = __SIM__fib14->Connect(1, 2);
  _inc_aprocs();
  { 
    shared_ptr<libpi::Int> __SIM__binop109((*__SIM__x16) - (*intval_2));
    __SIM__s222->Send(0,static_pointer_cast<libpi::Value>(__SIM__binop109));
  }
  shared_ptr<libpi::Int > __SIM__f123;
  _dec_aprocs();
  __SIM__f123 = static_pointer_cast<libpi::Int >(__SIM__s121->Receive(0));
  _inc_aprocs();
  __SIM__s121->Close(true);
  __SIM__s121=NULL;
  shared_ptr<libpi::Int > __SIM__f224;
  _dec_aprocs();
  __SIM__f224 = static_pointer_cast<libpi::Int >(__SIM__s222->Receive(0));
  _inc_aprocs();
  __SIM__s222->Close(true);
  __SIM__s222=NULL;
  { 
    shared_ptr<libpi::Int> __SIM__binop111((*__SIM__f123) + (*__SIM__f224));
    __SIM__s15->Send(1,static_pointer_cast<libpi::Value>(__SIM__binop111));
  }
  __SIM__s15->Close(true);
  __SIM__s15=NULL;
  _dec_aprocs();
  delete _state;
  return NULL;
  }
  }
  } // }}}
  method___SIM__Fib3: // {{{
  {
    shared_ptr<libpi::thread::Link > __SIM__fib14 = static_pointer_cast<libpi::thread::Link>(_state->values.back());
    _state->values.pop_back();
  _dec_aprocs();
  shared_ptr<Session> __SIM__s15 = __SIM__fib14->Connect(0, 2);
  _inc_aprocs();
  _inc_aprocs();
  { State *__SIM__state119 = new State();
  __SIM__state119->values.push_back( static_pointer_cast<libpi::Value>(__SIM__fib14));
  __SIM__state119->values.push_back( static_pointer_cast<libpi::Value>(__SIM__s15));
  __SIM__state119->label=&&method___SIM__FlatRight5;
    _spawn_thread(__SIM__state119);
  }
  { // Left process
  _state->values.clear();
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__fib14));
  _state->label=&&method___SIM__Fib3;
  goto *_state->label;
  }
  } // }}}
  method___SIM__StartService4: // {{{
  {
    shared_ptr<libpi::thread::Link > __SIM__fib14 = static_pointer_cast<libpi::thread::Link>(_state->values.back());
    _state->values.pop_back();
    shared_ptr<libpi::Int > __SIM__i25 = static_pointer_cast<libpi::Int>(_state->values.back());
    _state->values.pop_back();
    shared_ptr<libpi::Bool> __SIM__binop120((*__SIM__i25) <= (*intval_0));
  if (__SIM__binop120->GetValue())
  {
  _state->values.clear();
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__fib14));
  _state->label=&&method___SIM__Fib3;
  goto *_state->label;
  }
  else
  {
  _inc_aprocs();
  { State *__SIM__state121 = new State();
    shared_ptr<libpi::Int> __SIM__binop122((*__SIM__i25) - (*intval_1));
  __SIM__state121->values.push_back( static_pointer_cast<libpi::Value>(__SIM__binop122));
  __SIM__state121->values.push_back( static_pointer_cast<libpi::Value>(__SIM__fib14));
  __SIM__state121->label=&&method___SIM__StartService4;
    _spawn_thread(__SIM__state121);
  }
  { // Left process
  _state->values.clear();
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__fib14));
  _state->label=&&method___SIM__Fib3;
  goto *_state->label;
  }
  }
  } // }}}
  method___SIM__FlatRight6: // {{{
  {
    shared_ptr<libpi::thread::Link > __SIM__fib14 = static_pointer_cast<libpi::thread::Link>(_state->values.back());
    _state->values.pop_back();
    shared_ptr<libpi::thread::Link > __SIM__console3 = static_pointer_cast<libpi::thread::Link>(_state->values.back());
    _state->values.pop_back();
    shared_ptr<libpi::Int> __SIM__sysval124(new libpi::Int(_tprocs));
    shared_ptr<libpi::Int> __SIM__sysval125(new libpi::Int(*_aprocs));
    shared_ptr<libpi::Bool> __SIM__binop123((*__SIM__sysval124) <= (*__SIM__sysval125));
  if (__SIM__binop123->GetValue())
  {
  _dec_aprocs();
  shared_ptr<Session> __SIM__c26 = __SIM__console3->Connect(1, 2);
  _inc_aprocs();
  shared_ptr<libpi::String> __SIM__select135(new libpi::String("^PutString"));
  __SIM__c26->Send(0,__SIM__select135);
  { 
    __SIM__c26->Send(0,static_pointer_cast<libpi::Value>(stringval_3f31902b37cab22976b013dcd26ed920));
  }
  shared_ptr<libpi::String> __SIM__select137(new libpi::String("^GetInt"));
  __SIM__c26->Send(0,__SIM__select137);
  shared_ptr<libpi::Int > __SIM__v27;
  _dec_aprocs();
  __SIM__v27 = static_pointer_cast<libpi::Int >(__SIM__c26->Receive(0));
  _inc_aprocs();
  _dec_aprocs();
  shared_ptr<Session> __SIM__f28 = __SIM__fib14->Connect(1, 2);
  _inc_aprocs();
  { 
    __SIM__f28->Send(0,static_pointer_cast<libpi::Value>(__SIM__v27));
  }
  shared_ptr<libpi::Int > __SIM__r29;
  _dec_aprocs();
  __SIM__r29 = static_pointer_cast<libpi::Int >(__SIM__f28->Receive(0));
  _inc_aprocs();
  __SIM__f28->Close(true);
  __SIM__f28=NULL;
  shared_ptr<libpi::String> __SIM__select139(new libpi::String("^PutString"));
  __SIM__c26->Send(0,__SIM__select139);
  { 
    __SIM__c26->Send(0,static_pointer_cast<libpi::Value>(stringval_7559769f36c2ade263483f982b001498));
  }
  shared_ptr<libpi::String> __SIM__select141(new libpi::String("^PutInt"));
  __SIM__c26->Send(0,__SIM__select141);
  { 
    __SIM__c26->Send(0,static_pointer_cast<libpi::Value>(__SIM__r29));
  }
  shared_ptr<libpi::String> __SIM__select143(new libpi::String("^Stop"));
  __SIM__c26->Send(0,__SIM__select143);
  __SIM__c26->Close(true);
  __SIM__c26=NULL;
  _dec_aprocs();
  delete _state;
  return NULL;
  }
  else
  {
  _dec_aprocs();
  shared_ptr<Session> __SIM__c30 = __SIM__console3->Connect(1, 2);
  _inc_aprocs();
  shared_ptr<libpi::String> __SIM__select126(new libpi::String("^PutString"));
  __SIM__c30->Send(0,__SIM__select126);
  { 
    __SIM__c30->Send(0,static_pointer_cast<libpi::Value>(stringval_3f31902b37cab22976b013dcd26ed920));
  }
  shared_ptr<libpi::String> __SIM__select128(new libpi::String("^GetInt"));
  __SIM__c30->Send(0,__SIM__select128);
  _dec_aprocs();
  shared_ptr<Session> __SIM__f31 = __SIM__fib14->Connect(1, 2);
  _inc_aprocs();
  shared_ptr<libpi::Int > __SIM__v32;
  _dec_aprocs();
  __SIM__v32 = static_pointer_cast<libpi::Int >(__SIM__c30->Receive(0));
  _inc_aprocs();
  { 
    __SIM__f31->Send(0,static_pointer_cast<libpi::Value>(__SIM__v32));
  }
  shared_ptr<libpi::String> __SIM__select130(new libpi::String("^PutString"));
  __SIM__c30->Send(0,__SIM__select130);
  { 
    __SIM__c30->Send(0,static_pointer_cast<libpi::Value>(stringval_7559769f36c2ade263483f982b001498));
  }
  shared_ptr<libpi::String> __SIM__select132(new libpi::String("^PutInt"));
  __SIM__c30->Send(0,__SIM__select132);
  shared_ptr<libpi::Int > __SIM__r33;
  _dec_aprocs();
  __SIM__r33 = static_pointer_cast<libpi::Int >(__SIM__f31->Receive(0));
  _inc_aprocs();
  __SIM__f31->Close(true);
  __SIM__f31=NULL;
  { 
    __SIM__c30->Send(0,static_pointer_cast<libpi::Value>(__SIM__r33));
  }
  shared_ptr<libpi::String> __SIM__select134(new libpi::String("^Stop"));
  __SIM__c30->Send(0,__SIM__select134);
  __SIM__c30->Close(true);
  __SIM__c30=NULL;
  _dec_aprocs();
  delete _state;
  return NULL;
  }
  } // }}}
  method___SIM__FlatRight7: // {{{
  {
    shared_ptr<libpi::thread::Link > __SIM__console3 = static_pointer_cast<libpi::thread::Link>(_state->values.back());
    _state->values.pop_back();
  shared_ptr<thread::Link> __SIM__fib14(new thread::Link(2));
  _inc_aprocs();
  { State *__SIM__state144 = new State();
  __SIM__state144->values.push_back( static_pointer_cast<libpi::Value>(__SIM__console3));
  __SIM__state144->values.push_back( static_pointer_cast<libpi::Value>(__SIM__fib14));
  __SIM__state144->label=&&method___SIM__FlatRight6;
    _spawn_thread(__SIM__state144);
  }
  { // Left process
  _state->values.clear();
    shared_ptr<libpi::Int> __SIM__sysval145(new libpi::Int(_tprocs));
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__sysval145));
  _state->values.push_back( static_pointer_cast<libpi::Value>(__SIM__fib14));
  _state->label=&&method___SIM__StartService4;
  goto *_state->label;
  }
  } // }}}
}


/*Start process, and its continuations */
int main(int argc, char **argv) // {{{
{ // PARSE ARGS!!
  for (int i=0; i<argc; ++i)
  { if (string(argv[i])=="-pi_tprocs" && i+1<argc)
      _tprocs=atoi(argv[++i]);
    else
      _args.push_back(argv[i]);
  }
  (*_aprocs)=1;
  try
  { signal(SIGCHLD, SIG_IGN); // Fork optimization
    State *s0=new State();
    s0->label=NULL;
    _methods(s0);
    //munmap(_aprocs,sizeof(int));
  } catch (const string &error)
  { cerr << "Error: " << error << endl;
    return 1;
  }
  return 0;
} // }}}
