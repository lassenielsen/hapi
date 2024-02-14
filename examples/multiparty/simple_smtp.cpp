/* Pre compilation output:
******************* Program *******************
HOSTHEADER(""#include <iostream>"");
rec $console;
2->1
{ ^end where true:
    $end;,
  ^flt where true:
    2->1:Float;
    $console,
  ^input where true:
    2->1
    { ^int where true:
        1->2:Int;
        $console,
      ^str where true:
        1->2:String;
        $console
    },
  ^int where true:
    2->1:Int;
    $console,
  ^nl where true:
    $console,
  ^str where true:
    2->1:String;
    $console
} console(1, 2);
( local Console()
  ( s= new console(1 of 2);
    local ConsoleSession(rec $console;
                         2->1
                         { ^end where true:
                             $end;,
                           ^flt where true:
                             2->1:Float;
                             $console,
                           ^input where true:
                             2->1
                             { ^int where true:
                                 1->2:Int;
                                 $console,
                               ^str where true:
                                 1->2:String;
                                 $console
                             },
                           ^int where true:
                             2->1:Int;
                             $console,
                           ^nl where true:
                             $console,
                           ^str where true:
                             2->1:String;
                             $console
                         }(1 of 1, 2) s)
    ( s[2] >>
      { ^end:
        Console();,
        ^flt:
        s[2] >> f;
        HOST("std::cout << ", f", "->ToString() << std::flush;");
        ConsoleSession(s);,
        ^input:
        s[2] >>
        { ^int:
          Int val=0;
          HOST("{ long _l; std::cin >> _l; ", val", "=new libpi::Int(_l); }");
          s[2] << val;
          ConsoleSession(s);,
          ^str:
          String val="";
          HOST("{ std::string _s; std::cin >> _s; ", val", "=new libpi::String(_s); }");
          s[2] << val;
          ConsoleSession(s);
        },
        ^int:
        s[2] >> i;
        HOST("std::cout << ", i", "->ToString() << std::flush;");
        ConsoleSession(s);,
        ^nl:
        HOST("std::cout << std::endl;");
        ConsoleSession(s);,
        ^str:
        s[2] >> str;
        HOST("std::cout << ", str", "->GetValue() << std::flush;");
        ConsoleSession(s);
      }
    )
    ConsoleSession(s);
  )
  Console();
| 1->2
  { ^220_SERVICE_READY where true:
      1->2:String;
      2->1
      { ^EHLO where true:
          2->1:String;
          rec $smtp_ehlo;
          1->2
          { ^250 where true:
              1->2:String;
              2->1
              { ^QUIT where true:
                  $end;,
                ^StartTLS where true:
                  1->2
                  { ^220 where true:
                      2->1
                      { ^EHLO where true:
                          rec $smtp_tls;
                          1->2
                          { ^250 where true:
                              1->2:String;
                              rec $smtp_cmd;
                              2->1
                              { ^AUTH where true:
                                  1->2
                                  { ^235 where true:
                                      rec $smtp_auth;
                                      2->1
                                      { ^MAIL where true:
                                          2->1:String;
                                          1->2
                                          { ^250 where true:
                                              rec $smtp_mail;
                                              2->1
                                              { ^DATA where true:
                                                  1->2
                                                  { ^354 where true:
                                                      2->1:String;
                                                      2->1:String;
                                                      rec $smtp_data;
                                                      2->1
                                                      { ^DATALINE where true:
                                                          2->1:String;
                                                          2->1:String;
                                                          $smtp_data,
                                                        ^ENDOFDATA where true:
                                                          $smtp_auth,
                                                        ^SUBJECT where true:
                                                          2->1:String;
                                                          2->1:String;
                                                          $smtp_data
                                                      }
                                                  },
                                                ^RCPT where true:
                                                  2->1:String;
                                                  1->2
                                                  { ^250 where true:
                                                      $smtp_mail
                                                  }
                                              },
                                            ^501 where true:
                                              $smtp_auth
                                          },
                                        ^QUIT where true:
                                          $end;
                                      },
                                    ^535 where true:
                                      $smtp_cmd
                                  },
                                ^QUIT where true:
                                  $end;
                              },
                            ^250d where true:
                              $smtp_tls
                          },
                        ^QUIT where true:
                          $end;
                      }
                  }
              },
            ^250d where true:
              $smtp_ehlo
          },
        ^QUIT where true:
          $end;
      }
  } smtp(1, 2);
  ( local SmtpServer()
    ( this= new smtp(1 of 2);
      ( SmtpServer();
      | this[2] << ^220_SERVICE_READY;
        this[2] << "Welcome to the SMTP service!";
        this[2] >>
        { ^EHLO:
          this[2] >> msg;
          this[2] << ^250;
          this[2] << "smtpservice.pi";
          this[2] >>
          { ^QUIT:
            ,
            ^StartTLS:
            this[2] << ^220;
            this[2] >>
            { ^EHLO:
              this[2] << ^250;
              this[2] << "smtpservice.pi";
              this[2] >>
              { ^AUTH:
                this[2] << ^235;
                local SmtpServerAuth(rec $smtp_auth;
                                     2->1
                                     { ^MAIL where true:
                                         2->1:String;
                                         1->2
                                         { ^250 where true:
                                             rec $smtp_mail;
                                             2->1
                                             { ^DATA where true:
                                                 1->2
                                                 { ^354 where true:
                                                     2->1:String;
                                                     2->1:String;
                                                     rec $smtp_data;
                                                     2->1
                                                     { ^DATALINE where true:
                                                         2->1:String;
                                                         2->1:String;
                                                         $smtp_data,
                                                       ^ENDOFDATA where true:
                                                         $smtp_auth,
                                                       ^SUBJECT where true:
                                                         2->1:String;
                                                         2->1:String;
                                                         $smtp_data
                                                     }
                                                 },
                                               ^RCPT where true:
                                                 2->1:String;
                                                 1->2
                                                 { ^250 where true:
                                                     $smtp_mail
                                                 }
                                             },
                                           ^501 where true:
                                             $smtp_auth
                                         },
                                       ^QUIT where true:
                                         $end;
                                     }(1 of 1, 2) this)
                ( this[2] >>
                  { ^MAIL:
                    this[2] >> from;
                    Bool valid_from=true;
                    if valid_from
                    then this[2] << ^250;
                         local SmtpServerMail(rec $smtp_mail;
                                              2->1
                                              { ^DATA where true:
                                                  1->2
                                                  { ^354 where true:
                                                      2->1:String;
                                                      2->1:String;
                                                      rec $smtp_data;
                                                      2->1
                                                      { ^DATALINE where true:
                                                          2->1:String;
                                                          2->1:String;
                                                          $smtp_data,
                                                        ^ENDOFDATA where true:
                                                          rec $smtp_auth;
                                                          2->1
                                                          { ^MAIL where true:
                                                              2->1:String;
                                                              1->2
                                                              { ^250 where true:
                                                                  rec $smtp_mail;
                                                                  2->1
                                                                  { ^DATA where true:
                                                                      1->2
                                                                      { ^354 where true:
                                                                          2->1:String;
                                                                          2->1:String;
                                                                          rec $smtp_data;
                                                                          2->1
                                                                          { ^DATALINE where true:
                                                                              2->1:String;
                                                                              2->1:String;
                                                                              $smtp_data,
                                                                            ^ENDOFDATA where true:
                                                                              $smtp_auth,
                                                                            ^SUBJECT where true:
                                                                              2->1:String;
                                                                              2->1:String;
                                                                              $smtp_data
                                                                          }
                                                                      },
                                                                    ^RCPT where true:
                                                                      2->1:String;
                                                                      1->2
                                                                      { ^250 where true:
                                                                          $smtp_mail
                                                                      }
                                                                  },
                                                                ^501 where true:
                                                                  $smtp_auth
                                                              },
                                                            ^QUIT where true:
                                                              $end;
                                                          },
                                                        ^SUBJECT where true:
                                                          2->1:String;
                                                          2->1:String;
                                                          $smtp_data
                                                      }
                                                  },
                                                ^RCPT where true:
                                                  2->1:String;
                                                  1->2
                                                  { ^250 where true:
                                                      $smtp_mail
                                                  }
                                              }(1 of 1, 2) this,
                                              String rcpt)
                         ( this[2] >>
                           { ^DATA:
                             this[2] << ^354;
                             this[2] >> to;
                             this[2] >> from;
                             local SmtpServerData(rec $smtp_data;
                                                  2->1
                                                  { ^DATALINE where true:
                                                      2->1:String;
                                                      2->1:String;
                                                      $smtp_data,
                                                    ^ENDOFDATA where true:
                                                      rec $smtp_auth;
                                                      2->1
                                                      { ^MAIL where true:
                                                          2->1:String;
                                                          1->2
                                                          { ^250 where true:
                                                              rec $smtp_mail;
                                                              2->1
                                                              { ^DATA where true:
                                                                  1->2
                                                                  { ^354 where true:
                                                                      2->1:String;
                                                                      2->1:String;
                                                                      rec $smtp_data;
                                                                      2->1
                                                                      { ^DATALINE where true:
                                                                          2->1:String;
                                                                          2->1:String;
                                                                          $smtp_data,
                                                                        ^ENDOFDATA where true:
                                                                          $smtp_auth,
                                                                        ^SUBJECT where true:
                                                                          2->1:String;
                                                                          2->1:String;
                                                                          $smtp_data
                                                                      }
                                                                  },
                                                                ^RCPT where true:
                                                                  2->1:String;
                                                                  1->2
                                                                  { ^250 where true:
                                                                      $smtp_mail
                                                                  }
                                                              },
                                                            ^501 where true:
                                                              $smtp_auth
                                                          },
                                                        ^QUIT where true:
                                                          $end;
                                                      },
                                                    ^SUBJECT where true:
                                                      2->1:String;
                                                      2->1:String;
                                                      $smtp_data
                                                  }(1 of 1, 2) this,
                                                  String subject,
                                                  String data)
                             ( this[2] >>
                               { ^DATALINE:
                                 this[2] >> line1;
                                 this[2] >> line2;
                                 SmtpServerData(this, subject, line1);,
                                 ^ENDOFDATA:
                                 c= new console(2 of 2);
                                 c[1] << ^str;
                                 c[1] << "Server received mail";
                                 c[1] << ^nl;
                                 c[1] << ^str;
                                 c[1] << "FROM: ";
                                 c[1] << ^str;
                                 c[1] << from;
                                 c[1] << ^nl;
                                 c[1] << ^str;
                                 c[1] << "RCPT: ";
                                 c[1] << ^str;
                                 c[1] << rcpt;
                                 c[1] << ^nl;
                                 c[1] << ^str;
                                 c[1] << "SUBJ: ";
                                 c[1] << ^str;
                                 c[1] << subject;
                                 c[1] << ^nl;
                                 c[1] << ^end;
                                 SmtpServerAuth(this);,
                                 ^SUBJECT:
                                 this[2] >> s1;
                                 this[2] >> s2;
                                 SmtpServerData(this, s1, data);
                               }
                             )
                             SmtpServerData(this, "", "");,
                             ^RCPT:
                             this[2] >> r;
                             this[2] << ^250;
                             SmtpServerMail(this, r);
                           }
                         )
                         SmtpServerMail(this, "");
                    else this[2] << ^501;
                         SmtpServerAuth(this);,
                    ^QUIT:
                    
                  }
                )
                SmtpServerAuth(this);,
                ^QUIT:
                
              },
              ^QUIT:
              
            }
          },
          ^QUIT:
          
        }
      )
    )
    local StartService(Int i)
      ( if (i <= 0)
        then SmtpServer();
        else ( SmtpServer();
             | StartService((i - 1));
             )
      )
      StartService(SYSTEM & "tprocs");
  | client= new smtp(2 of 2);
    client[1] >>
    { ^220_SERVICE_READY:
      client[1] >> msg;
      client[1] << ^EHLO;
      client[1] << "user";
      local SmtpClientEhlo(rec $smtp_ehlo;
                           1->2
                           { ^250 where true:
                               1->2:String;
                               2->1
                               { ^QUIT where true:
                                   $end;,
                                 ^StartTLS where true:
                                   1->2
                                   { ^220 where true:
                                       2->1
                                       { ^EHLO where true:
                                           rec $smtp_tls;
                                           1->2
                                           { ^250 where true:
                                               1->2:String;
                                               rec $smtp_cmd;
                                               2->1
                                               { ^AUTH where true:
                                                   1->2
                                                   { ^235 where true:
                                                       rec $smtp_auth;
                                                       2->1
                                                       { ^MAIL where true:
                                                           2->1:String;
                                                           1->2
                                                           { ^250 where true:
                                                               rec $smtp_mail;
                                                               2->1
                                                               { ^DATA where true:
                                                                   1->2
                                                                   { ^354 where true:
                                                                       2->1:String;
                                                                       2->1:String;
                                                                       rec $smtp_data;
                                                                       2->1
                                                                       { ^DATALINE where true:
                                                                           2->1:String;
                                                                           2->1:String;
                                                                           $smtp_data,
                                                                         ^ENDOFDATA where true:
                                                                           $smtp_auth,
                                                                         ^SUBJECT where true:
                                                                           2->1:String;
                                                                           2->1:String;
                                                                           $smtp_data
                                                                       }
                                                                   },
                                                                 ^RCPT where true:
                                                                   2->1:String;
                                                                   1->2
                                                                   { ^250 where true:
                                                                       $smtp_mail
                                                                   }
                                                               },
                                                             ^501 where true:
                                                               $smtp_auth
                                                           },
                                                         ^QUIT where true:
                                                           $end;
                                                       },
                                                     ^535 where true:
                                                       $smtp_cmd
                                                   },
                                                 ^QUIT where true:
                                                   $end;
                                               },
                                             ^250d where true:
                                               $smtp_tls
                                           },
                                         ^QUIT where true:
                                           $end;
                                       }
                                   }
                               },
                             ^250d where true:
                               $smtp_ehlo
                           }(2 of 1, 2) this)
      ( this[1] >>
        { ^250:
          this[1] >> srv;
          this[1] << ^StartTLS;
          this[1] >>
          { ^220:
            this[1] << ^EHLO;
            local SmtpClientTls(rec $smtp_tls;
                                1->2
                                { ^250 where true:
                                    1->2:String;
                                    rec $smtp_cmd;
                                    2->1
                                    { ^AUTH where true:
                                        1->2
                                        { ^235 where true:
                                            rec $smtp_auth;
                                            2->1
                                            { ^MAIL where true:
                                                2->1:String;
                                                1->2
                                                { ^250 where true:
                                                    rec $smtp_mail;
                                                    2->1
                                                    { ^DATA where true:
                                                        1->2
                                                        { ^354 where true:
                                                            2->1:String;
                                                            2->1:String;
                                                            rec $smtp_data;
                                                            2->1
                                                            { ^DATALINE where true:
                                                                2->1:String;
                                                                2->1:String;
                                                                $smtp_data,
                                                              ^ENDOFDATA where true:
                                                                $smtp_auth,
                                                              ^SUBJECT where true:
                                                                2->1:String;
                                                                2->1:String;
                                                                $smtp_data
                                                            }
                                                        },
                                                      ^RCPT where true:
                                                        2->1:String;
                                                        1->2
                                                        { ^250 where true:
                                                            $smtp_mail
                                                        }
                                                    },
                                                  ^501 where true:
                                                    $smtp_auth
                                                },
                                              ^QUIT where true:
                                                $end;
                                            },
                                          ^535 where true:
                                            $smtp_cmd
                                        },
                                      ^QUIT where true:
                                        $end;
                                    },
                                  ^250d where true:
                                    $smtp_tls
                                }(2 of 1, 2) this)
            ( this[1] >>
              { ^250:
                this[1] >> srv;
                this[1] << ^AUTH;
                this[1] >>
                { ^235:
                  this[1] << ^MAIL;
                  this[1] << "user@smtpserver.pi";
                  this[1] >>
                  { ^250:
                    this[1] << ^RCPT;
                    this[1] << "friend@smtpserver.pi";
                    this[1] >>
                    { ^250:
                      this[1] << ^DATA;
                      this[1] >>
                      { ^354:
                        this[1] << "friend@smtpserver.pi";
                        this[1] << "user@smtpserver.pi";
                        this[1] << ^SUBJECT;
                        this[1] << "SMTP MESSAGE";
                        this[1] << "SUBJECT";
                        this[1] << ^DATALINE;
                        this[1] << "MSG1";
                        this[1] << "MSG2";
                        this[1] << ^ENDOFDATA;
                        this[1] << ^QUIT;
                        
                      }
                    },
                    ^501:
                    this[1] << ^QUIT;
                    
                  },
                  ^535:
                  this[1] << ^QUIT;
                  
                },
                ^250d:
                SmtpClientTls(this);
              }
            )
            SmtpClientTls(this);
          },
          ^250d:
          SmtpClientEhlo(this);
        }
      )
      SmtpClientEhlo(client);
    }
  )
)
************ Type Checking Program ************
************ Type Check Succeeded! ************
****** Parallelization Optimized Program ******
HOSTHEADER(""#include <iostream>"");
rec $console;
2->1
{ ^end where true:
    $end;,
  ^flt where true:
    2->1:Float;
    $console,
  ^input where true:
    2->1
    { ^int where true:
        1->2:Int;
        $console,
      ^str where true:
        1->2:String;
        $console
    },
  ^int where true:
    2->1:Int;
    $console,
  ^nl where true:
    $console,
  ^str where true:
    2->1:String;
    $console
} console(1, 2);
( local Console()
  ( s= new console(1 of 2);
    local ConsoleSession(rec $console;
                         2->1
                         { ^end where true:
                             $end;,
                           ^flt where true:
                             2->1:Float;
                             $console,
                           ^input where true:
                             2->1
                             { ^int where true:
                                 1->2:Int;
                                 $console,
                               ^str where true:
                                 1->2:String;
                                 $console
                             },
                           ^int where true:
                             2->1:Int;
                             $console,
                           ^nl where true:
                             $console,
                           ^str where true:
                             2->1:String;
                             $console
                         }(1 of 1, 2) s)
    ( s[2] >>
      { ^end:
        Console();,
        ^flt:
        s[2] >> f;
        HOST("std::cout << ", f", "->ToString() << std::flush;");
        ConsoleSession(s);,
        ^input:
        s[2] >>
        { ^int:
          Int val=0;
          HOST("{ long _l; std::cin >> _l; ", val", "=new libpi::Int(_l); }");
          s[2] << val;
          ConsoleSession(s);,
          ^str:
          String val="";
          HOST("{ std::string _s; std::cin >> _s; ", val", "=new libpi::String(_s); }");
          s[2] << val;
          ConsoleSession(s);
        },
        ^int:
        s[2] >> i;
        HOST("std::cout << ", i", "->ToString() << std::flush;");
        ConsoleSession(s);,
        ^nl:
        HOST("std::cout << std::endl;");
        ConsoleSession(s);,
        ^str:
        s[2] >> str;
        HOST("std::cout << ", str", "->GetValue() << std::flush;");
        ConsoleSession(s);
      }
    )
    ConsoleSession(s);
  )
  Console();
| 1->2
  { ^220_SERVICE_READY where true:
      1->2:String;
      2->1
      { ^EHLO where true:
          2->1:String;
          rec $smtp_ehlo;
          1->2
          { ^250 where true:
              1->2:String;
              2->1
              { ^QUIT where true:
                  $end;,
                ^StartTLS where true:
                  1->2
                  { ^220 where true:
                      2->1
                      { ^EHLO where true:
                          rec $smtp_tls;
                          1->2
                          { ^250 where true:
                              1->2:String;
                              rec $smtp_cmd;
                              2->1
                              { ^AUTH where true:
                                  1->2
                                  { ^235 where true:
                                      rec $smtp_auth;
                                      2->1
                                      { ^MAIL where true:
                                          2->1:String;
                                          1->2
                                          { ^250 where true:
                                              rec $smtp_mail;
                                              2->1
                                              { ^DATA where true:
                                                  1->2
                                                  { ^354 where true:
                                                      2->1:String;
                                                      2->1:String;
                                                      rec $smtp_data;
                                                      2->1
                                                      { ^DATALINE where true:
                                                          2->1:String;
                                                          2->1:String;
                                                          $smtp_data,
                                                        ^ENDOFDATA where true:
                                                          $smtp_auth,
                                                        ^SUBJECT where true:
                                                          2->1:String;
                                                          2->1:String;
                                                          $smtp_data
                                                      }
                                                  },
                                                ^RCPT where true:
                                                  2->1:String;
                                                  1->2
                                                  { ^250 where true:
                                                      $smtp_mail
                                                  }
                                              },
                                            ^501 where true:
                                              $smtp_auth
                                          },
                                        ^QUIT where true:
                                          $end;
                                      },
                                    ^535 where true:
                                      $smtp_cmd
                                  },
                                ^QUIT where true:
                                  $end;
                              },
                            ^250d where true:
                              $smtp_tls
                          },
                        ^QUIT where true:
                          $end;
                      }
                  }
              },
            ^250d where true:
              $smtp_ehlo
          },
        ^QUIT where true:
          $end;
      }
  } smtp(1, 2);
  ( local SmtpServer()
    ( this= new smtp(1 of 2);
      ( SmtpServer();
      | this[2] << ^220_SERVICE_READY;
        this[2] << "Welcome to the SMTP service!";
        this[2] >>
        { ^EHLO:
          this[2] >> msg;
          this[2] << ^250;
          this[2] << "smtpservice.pi";
          this[2] >>
          { ^QUIT:
            ,
            ^StartTLS:
            this[2] << ^220;
            this[2] >>
            { ^EHLO:
              this[2] << ^250;
              this[2] << "smtpservice.pi";
              this[2] >>
              { ^AUTH:
                this[2] << ^235;
                local SmtpServerAuth(rec $smtp_auth;
                                     2->1
                                     { ^MAIL where true:
                                         2->1:String;
                                         1->2
                                         { ^250 where true:
                                             rec $smtp_mail;
                                             2->1
                                             { ^DATA where true:
                                                 1->2
                                                 { ^354 where true:
                                                     2->1:String;
                                                     2->1:String;
                                                     rec $smtp_data;
                                                     2->1
                                                     { ^DATALINE where true:
                                                         2->1:String;
                                                         2->1:String;
                                                         $smtp_data,
                                                       ^ENDOFDATA where true:
                                                         $smtp_auth,
                                                       ^SUBJECT where true:
                                                         2->1:String;
                                                         2->1:String;
                                                         $smtp_data
                                                     }
                                                 },
                                               ^RCPT where true:
                                                 2->1:String;
                                                 1->2
                                                 { ^250 where true:
                                                     $smtp_mail
                                                 }
                                             },
                                           ^501 where true:
                                             $smtp_auth
                                         },
                                       ^QUIT where true:
                                         $end;
                                     }(1 of 1, 2) this)
                ( this[2] >>
                  { ^MAIL:
                    if (SYSTEM & "tprocs" <= SYSTEM & "aprocs")
                    then this[2] >> from;
                         Bool valid_from=true;
                         if valid_from
                         then this[2] << ^250;
                              local SmtpServerMail(rec $smtp_mail;
                                                   2->1
                                                   { ^DATA where true:
                                                       1->2
                                                       { ^354 where true:
                                                           2->1:String;
                                                           2->1:String;
                                                           rec $smtp_data;
                                                           2->1
                                                           { ^DATALINE where true:
                                                               2->1:String;
                                                               2->1:String;
                                                               $smtp_data,
                                                             ^ENDOFDATA where true:
                                                               rec $smtp_auth;
                                                               2->1
                                                               { ^MAIL where true:
                                                                   2->1:String;
                                                                   1->2
                                                                   { ^250 where true:
                                                                       rec $smtp_mail;
                                                                       2->1
                                                                       { ^DATA where true:
                                                                           1->2
                                                                           { ^354 where true:
                                                                               2->1:String;
                                                                               2->1:String;
                                                                               rec $smtp_data;
                                                                               2->1
                                                                               { ^DATALINE where true:
                                                                                   2->1:String;
                                                                                   2->1:String;
                                                                                   $smtp_data,
                                                                                 ^ENDOFDATA where true:
                                                                                   $smtp_auth,
                                                                                 ^SUBJECT where true:
                                                                                   2->1:String;
                                                                                   2->1:String;
                                                                                   $smtp_data
                                                                               }
                                                                           },
                                                                         ^RCPT where true:
                                                                           2->1:String;
                                                                           1->2
                                                                           { ^250 where true:
                                                                               $smtp_mail
                                                                           }
                                                                       },
                                                                     ^501 where true:
                                                                       $smtp_auth
                                                                   },
                                                                 ^QUIT where true:
                                                                   $end;
                                                               },
                                                             ^SUBJECT where true:
                                                               2->1:String;
                                                               2->1:String;
                                                               $smtp_data
                                                           }
                                                       },
                                                     ^RCPT where true:
                                                       2->1:String;
                                                       1->2
                                                       { ^250 where true:
                                                           $smtp_mail
                                                       }
                                                   }(1 of 1, 2) this,
                                                   String rcpt)
                              ( this[2] >>
                                { ^DATA:
                                  this[2] << ^354;
                                  this[2] >> to;
                                  this[2] >> from;
                                  local SmtpServerData(rec $smtp_data;
                                                       2->1
                                                       { ^DATALINE where true:
                                                           2->1:String;
                                                           2->1:String;
                                                           $smtp_data,
                                                         ^ENDOFDATA where true:
                                                           rec $smtp_auth;
                                                           2->1
                                                           { ^MAIL where true:
                                                               2->1:String;
                                                               1->2
                                                               { ^250 where true:
                                                                   rec $smtp_mail;
                                                                   2->1
                                                                   { ^DATA where true:
                                                                       1->2
                                                                       { ^354 where true:
                                                                           2->1:String;
                                                                           2->1:String;
                                                                           rec $smtp_data;
                                                                           2->1
                                                                           { ^DATALINE where true:
                                                                               2->1:String;
                                                                               2->1:String;
                                                                               $smtp_data,
                                                                             ^ENDOFDATA where true:
                                                                               $smtp_auth,
                                                                             ^SUBJECT where true:
                                                                               2->1:String;
                                                                               2->1:String;
                                                                               $smtp_data
                                                                           }
                                                                       },
                                                                     ^RCPT where true:
                                                                       2->1:String;
                                                                       1->2
                                                                       { ^250 where true:
                                                                           $smtp_mail
                                                                       }
                                                                   },
                                                                 ^501 where true:
                                                                   $smtp_auth
                                                               },
                                                             ^QUIT where true:
                                                               $end;
                                                           },
                                                         ^SUBJECT where true:
                                                           2->1:String;
                                                           2->1:String;
                                                           $smtp_data
                                                       }(1 of 1, 2) this,
                                                       String subject,
                                                       String data)
                                  ( this[2] >>
                                    { ^DATALINE:
                                      this[2] >> line1;
                                      this[2] >> line2;
                                      SmtpServerData(this, subject, line1);,
                                      ^ENDOFDATA:
                                      c= new console(2 of 2);
                                      c[1] << ^str;
                                      c[1] << "Server received mail";
                                      c[1] << ^nl;
                                      c[1] << ^str;
                                      c[1] << "FROM: ";
                                      c[1] << ^str;
                                      c[1] << from;
                                      c[1] << ^nl;
                                      c[1] << ^str;
                                      c[1] << "RCPT: ";
                                      c[1] << ^str;
                                      c[1] << rcpt;
                                      c[1] << ^nl;
                                      c[1] << ^str;
                                      c[1] << "SUBJ: ";
                                      c[1] << ^str;
                                      c[1] << subject;
                                      c[1] << ^nl;
                                      c[1] << ^end;
                                      SmtpServerAuth(this);,
                                      ^SUBJECT:
                                      this[2] >> s1;
                                      this[2] >> s2;
                                      SmtpServerData(this, s1, data);
                                    }
                                  )
                                  SmtpServerData(this, "", "");,
                                  ^RCPT:
                                  this[2] >> r;
                                  this[2] << ^250;
                                  SmtpServerMail(this, r);
                                }
                              )
                              SmtpServerMail(this, "");
                         else this[2] << ^501;
                              SmtpServerAuth(this);
                    else Bool valid_from=true;
                         this[2] >> from;
                         if valid_from
                         then this[2] << ^250;
                              local SmtpServerMail(rec $smtp_mail;
                                                   2->1
                                                   { ^DATA where true:
                                                       1->2
                                                       { ^354 where true:
                                                           2->1:String;
                                                           2->1:String;
                                                           rec $smtp_data;
                                                           2->1
                                                           { ^DATALINE where true:
                                                               2->1:String;
                                                               2->1:String;
                                                               $smtp_data,
                                                             ^ENDOFDATA where true:
                                                               rec $smtp_auth;
                                                               2->1
                                                               { ^MAIL where true:
                                                                   2->1:String;
                                                                   1->2
                                                                   { ^250 where true:
                                                                       rec $smtp_mail;
                                                                       2->1
                                                                       { ^DATA where true:
                                                                           1->2
                                                                           { ^354 where true:
                                                                               2->1:String;
                                                                               2->1:String;
                                                                               rec $smtp_data;
                                                                               2->1
                                                                               { ^DATALINE where true:
                                                                                   2->1:String;
                                                                                   2->1:String;
                                                                                   $smtp_data,
                                                                                 ^ENDOFDATA where true:
                                                                                   $smtp_auth,
                                                                                 ^SUBJECT where true:
                                                                                   2->1:String;
                                                                                   2->1:String;
                                                                                   $smtp_data
                                                                               }
                                                                           },
                                                                         ^RCPT where true:
                                                                           2->1:String;
                                                                           1->2
                                                                           { ^250 where true:
                                                                               $smtp_mail
                                                                           }
                                                                       },
                                                                     ^501 where true:
                                                                       $smtp_auth
                                                                   },
                                                                 ^QUIT where true:
                                                                   $end;
                                                               },
                                                             ^SUBJECT where true:
                                                               2->1:String;
                                                               2->1:String;
                                                               $smtp_data
                                                           }
                                                       },
                                                     ^RCPT where true:
                                                       2->1:String;
                                                       1->2
                                                       { ^250 where true:
                                                           $smtp_mail
                                                       }
                                                   }(1 of 1, 2) this,
                                                   String rcpt)
                              ( this[2] >>
                                { ^DATA:
                                  this[2] << ^354;
                                  this[2] >> to;
                                  this[2] >> from;
                                  local SmtpServerData(rec $smtp_data;
                                                       2->1
                                                       { ^DATALINE where true:
                                                           2->1:String;
                                                           2->1:String;
                                                           $smtp_data,
                                                         ^ENDOFDATA where true:
                                                           rec $smtp_auth;
                                                           2->1
                                                           { ^MAIL where true:
                                                               2->1:String;
                                                               1->2
                                                               { ^250 where true:
                                                                   rec $smtp_mail;
                                                                   2->1
                                                                   { ^DATA where true:
                                                                       1->2
                                                                       { ^354 where true:
                                                                           2->1:String;
                                                                           2->1:String;
                                                                           rec $smtp_data;
                                                                           2->1
                                                                           { ^DATALINE where true:
                                                                               2->1:String;
                                                                               2->1:String;
                                                                               $smtp_data,
                                                                             ^ENDOFDATA where true:
                                                                               $smtp_auth,
                                                                             ^SUBJECT where true:
                                                                               2->1:String;
                                                                               2->1:String;
                                                                               $smtp_data
                                                                           }
                                                                       },
                                                                     ^RCPT where true:
                                                                       2->1:String;
                                                                       1->2
                                                                       { ^250 where true:
                                                                           $smtp_mail
                                                                       }
                                                                   },
                                                                 ^501 where true:
                                                                   $smtp_auth
                                                               },
                                                             ^QUIT where true:
                                                               $end;
                                                           },
                                                         ^SUBJECT where true:
                                                           2->1:String;
                                                           2->1:String;
                                                           $smtp_data
                                                       }(1 of 1, 2) this,
                                                       String subject,
                                                       String data)
                                  ( this[2] >>
                                    { ^DATALINE:
                                      this[2] >> line1;
                                      this[2] >> line2;
                                      SmtpServerData(this, subject, line1);,
                                      ^ENDOFDATA:
                                      c= new console(2 of 2);
                                      c[1] << ^str;
                                      c[1] << "Server received mail";
                                      c[1] << ^nl;
                                      c[1] << ^str;
                                      c[1] << "FROM: ";
                                      c[1] << ^str;
                                      c[1] << from;
                                      c[1] << ^nl;
                                      c[1] << ^str;
                                      c[1] << "RCPT: ";
                                      c[1] << ^str;
                                      c[1] << rcpt;
                                      c[1] << ^nl;
                                      c[1] << ^str;
                                      c[1] << "SUBJ: ";
                                      c[1] << ^str;
                                      c[1] << subject;
                                      c[1] << ^nl;
                                      c[1] << ^end;
                                      SmtpServerAuth(this);,
                                      ^SUBJECT:
                                      this[2] >> s1;
                                      this[2] >> s2;
                                      SmtpServerData(this, s1, data);
                                    }
                                  )
                                  SmtpServerData(this, "", "");,
                                  ^RCPT:
                                  this[2] >> r;
                                  this[2] << ^250;
                                  SmtpServerMail(this, r);
                                }
                              )
                              SmtpServerMail(this, "");
                         else this[2] << ^501;
                              SmtpServerAuth(this);,
                    ^QUIT:
                    
                  }
                )
                SmtpServerAuth(this);,
                ^QUIT:
                
              },
              ^QUIT:
              
            }
          },
          ^QUIT:
          
        }
      )
    )
    local StartService(Int i)
      ( if (i <= 0)
        then SmtpServer();
        else ( SmtpServer();
             | StartService((i - 1));
             )
      )
      StartService(SYSTEM & "tprocs");
  | client= new smtp(2 of 2);
    client[1] >>
    { ^220_SERVICE_READY:
      client[1] >> msg;
      client[1] << ^EHLO;
      client[1] << "user";
      local SmtpClientEhlo(rec $smtp_ehlo;
                           1->2
                           { ^250 where true:
                               1->2:String;
                               2->1
                               { ^QUIT where true:
                                   $end;,
                                 ^StartTLS where true:
                                   1->2
                                   { ^220 where true:
                                       2->1
                                       { ^EHLO where true:
                                           rec $smtp_tls;
                                           1->2
                                           { ^250 where true:
                                               1->2:String;
                                               rec $smtp_cmd;
                                               2->1
                                               { ^AUTH where true:
                                                   1->2
                                                   { ^235 where true:
                                                       rec $smtp_auth;
                                                       2->1
                                                       { ^MAIL where true:
                                                           2->1:String;
                                                           1->2
                                                           { ^250 where true:
                                                               rec $smtp_mail;
                                                               2->1
                                                               { ^DATA where true:
                                                                   1->2
                                                                   { ^354 where true:
                                                                       2->1:String;
                                                                       2->1:String;
                                                                       rec $smtp_data;
                                                                       2->1
                                                                       { ^DATALINE where true:
                                                                           2->1:String;
                                                                           2->1:String;
                                                                           $smtp_data,
                                                                         ^ENDOFDATA where true:
                                                                           $smtp_auth,
                                                                         ^SUBJECT where true:
                                                                           2->1:String;
                                                                           2->1:String;
                                                                           $smtp_data
                                                                       }
                                                                   },
                                                                 ^RCPT where true:
                                                                   2->1:String;
                                                                   1->2
                                                                   { ^250 where true:
                                                                       $smtp_mail
                                                                   }
                                                               },
                                                             ^501 where true:
                                                               $smtp_auth
                                                           },
                                                         ^QUIT where true:
                                                           $end;
                                                       },
                                                     ^535 where true:
                                                       $smtp_cmd
                                                   },
                                                 ^QUIT where true:
                                                   $end;
                                               },
                                             ^250d where true:
                                               $smtp_tls
                                           },
                                         ^QUIT where true:
                                           $end;
                                       }
                                   }
                               },
                             ^250d where true:
                               $smtp_ehlo
                           }(2 of 1, 2) this)
      ( this[1] >>
        { ^250:
          this[1] >> srv;
          this[1] << ^StartTLS;
          this[1] >>
          { ^220:
            this[1] << ^EHLO;
            local SmtpClientTls(rec $smtp_tls;
                                1->2
                                { ^250 where true:
                                    1->2:String;
                                    rec $smtp_cmd;
                                    2->1
                                    { ^AUTH where true:
                                        1->2
                                        { ^235 where true:
                                            rec $smtp_auth;
                                            2->1
                                            { ^MAIL where true:
                                                2->1:String;
                                                1->2
                                                { ^250 where true:
                                                    rec $smtp_mail;
                                                    2->1
                                                    { ^DATA where true:
                                                        1->2
                                                        { ^354 where true:
                                                            2->1:String;
                                                            2->1:String;
                                                            rec $smtp_data;
                                                            2->1
                                                            { ^DATALINE where true:
                                                                2->1:String;
                                                                2->1:String;
                                                                $smtp_data,
                                                              ^ENDOFDATA where true:
                                                                $smtp_auth,
                                                              ^SUBJECT where true:
                                                                2->1:String;
                                                                2->1:String;
                                                                $smtp_data
                                                            }
                                                        },
                                                      ^RCPT where true:
                                                        2->1:String;
                                                        1->2
                                                        { ^250 where true:
                                                            $smtp_mail
                                                        }
                                                    },
                                                  ^501 where true:
                                                    $smtp_auth
                                                },
                                              ^QUIT where true:
                                                $end;
                                            },
                                          ^535 where true:
                                            $smtp_cmd
                                        },
                                      ^QUIT where true:
                                        $end;
                                    },
                                  ^250d where true:
                                    $smtp_tls
                                }(2 of 1, 2) this)
            ( this[1] >>
              { ^250:
                this[1] >> srv;
                this[1] << ^AUTH;
                this[1] >>
                { ^235:
                  this[1] << ^MAIL;
                  this[1] << "user@smtpserver.pi";
                  this[1] >>
                  { ^250:
                    this[1] << ^RCPT;
                    this[1] << "friend@smtpserver.pi";
                    this[1] >>
                    { ^250:
                      this[1] << ^DATA;
                      this[1] >>
                      { ^354:
                        this[1] << "friend@smtpserver.pi";
                        this[1] << "user@smtpserver.pi";
                        this[1] << ^SUBJECT;
                        this[1] << "SMTP MESSAGE";
                        this[1] << "SUBJECT";
                        this[1] << ^DATALINE;
                        this[1] << "MSG1";
                        this[1] << "MSG2";
                        this[1] << ^ENDOFDATA;
                        this[1] << ^QUIT;
                        
                      }
                    },
                    ^501:
                    this[1] << ^QUIT;
                    
                  },
                  ^535:
                  this[1] << ^QUIT;
                  
                },
                ^250d:
                SmtpClientTls(this);
              }
            )
            SmtpClientTls(this);
          },
          ^250d:
          SmtpClientEhlo(this);
        }
      )
      SmtpClientEhlo(client);
    }
  )
)
*/
/* ==== ORIGINAL ====
HOSTHEADER(""#include <iostream>"");
rec $console;
2->1
{ ^end where true:
    $end;,
  ^flt where true:
    2->1:Float;
    $console,
  ^input where true:
    2->1
    { ^int where true:
        1->2:Int;
        $console,
      ^str where true:
        1->2:String;
        $console
    },
  ^int where true:
    2->1:Int;
    $console,
  ^nl where true:
    $console,
  ^str where true:
    2->1:String;
    $console
} console(1, 2);
( local Console()
  ( s= new console(1 of 2);
    local ConsoleSession(rec $console;
                         2->1
                         { ^end where true:
                             $end;,
                           ^flt where true:
                             2->1:Float;
                             $console,
                           ^input where true:
                             2->1
                             { ^int where true:
                                 1->2:Int;
                                 $console,
                               ^str where true:
                                 1->2:String;
                                 $console
                             },
                           ^int where true:
                             2->1:Int;
                             $console,
                           ^nl where true:
                             $console,
                           ^str where true:
                             2->1:String;
                             $console
                         }(1 of 1, 2) s)
    ( s[2] >>
      { ^end:
        Console();,
        ^flt:
        s[2] >> f;
        HOST("std::cout << ", f", "->ToString() << std::flush;");
        ConsoleSession(s);,
        ^input:
        s[2] >>
        { ^int:
          Int val=0;
          HOST("{ long _l; std::cin >> _l; ", val", "=new libpi::Int(_l); }");
          s[2] << val;
          ConsoleSession(s);,
          ^str:
          String val="";
          HOST("{ std::string _s; std::cin >> _s; ", val", "=new libpi::String(_s); }");
          s[2] << val;
          ConsoleSession(s);
        },
        ^int:
        s[2] >> i;
        HOST("std::cout << ", i", "->ToString() << std::flush;");
        ConsoleSession(s);,
        ^nl:
        HOST("std::cout << std::endl;");
        ConsoleSession(s);,
        ^str:
        s[2] >> str;
        HOST("std::cout << ", str", "->GetValue() << std::flush;");
        ConsoleSession(s);
      }
    )
    ConsoleSession(s);
  )
  Console();
| 1->2
  { ^220_SERVICE_READY where true:
      1->2:String;
      2->1
      { ^EHLO where true:
          2->1:String;
          rec $smtp_ehlo;
          1->2
          { ^250 where true:
              1->2:String;
              2->1
              { ^QUIT where true:
                  $end;,
                ^StartTLS where true:
                  1->2
                  { ^220 where true:
                      2->1
                      { ^EHLO where true:
                          rec $smtp_tls;
                          1->2
                          { ^250 where true:
                              1->2:String;
                              rec $smtp_cmd;
                              2->1
                              { ^AUTH where true:
                                  1->2
                                  { ^235 where true:
                                      rec $smtp_auth;
                                      2->1
                                      { ^MAIL where true:
                                          2->1:String;
                                          1->2
                                          { ^250 where true:
                                              rec $smtp_mail;
                                              2->1
                                              { ^DATA where true:
                                                  1->2
                                                  { ^354 where true:
                                                      2->1:String;
                                                      2->1:String;
                                                      rec $smtp_data;
                                                      2->1
                                                      { ^DATALINE where true:
                                                          2->1:String;
                                                          2->1:String;
                                                          $smtp_data,
                                                        ^ENDOFDATA where true:
                                                          $smtp_auth,
                                                        ^SUBJECT where true:
                                                          2->1:String;
                                                          2->1:String;
                                                          $smtp_data
                                                      }
                                                  },
                                                ^RCPT where true:
                                                  2->1:String;
                                                  1->2
                                                  { ^250 where true:
                                                      $smtp_mail
                                                  }
                                              },
                                            ^501 where true:
                                              $smtp_auth
                                          },
                                        ^QUIT where true:
                                          $end;
                                      },
                                    ^535 where true:
                                      $smtp_cmd
                                  },
                                ^QUIT where true:
                                  $end;
                              },
                            ^250d where true:
                              $smtp_tls
                          },
                        ^QUIT where true:
                          $end;
                      }
                  }
              },
            ^250d where true:
              $smtp_ehlo
          },
        ^QUIT where true:
          $end;
      }
  } smtp(1, 2);
  ( local SmtpServer()
    ( this= new smtp(1 of 2);
      ( SmtpServer();
      | this[2] << ^220_SERVICE_READY;
        this[2] << "Welcome to the SMTP service!";
        this[2] >>
        { ^EHLO:
          this[2] >> msg;
          this[2] << ^250;
          this[2] << "smtpservice.pi";
          this[2] >>
          { ^QUIT:
            ,
            ^StartTLS:
            this[2] << ^220;
            this[2] >>
            { ^EHLO:
              this[2] << ^250;
              this[2] << "smtpservice.pi";
              this[2] >>
              { ^AUTH:
                this[2] << ^235;
                local SmtpServerAuth(rec $smtp_auth;
                                     2->1
                                     { ^MAIL where true:
                                         2->1:String;
                                         1->2
                                         { ^250 where true:
                                             rec $smtp_mail;
                                             2->1
                                             { ^DATA where true:
                                                 1->2
                                                 { ^354 where true:
                                                     2->1:String;
                                                     2->1:String;
                                                     rec $smtp_data;
                                                     2->1
                                                     { ^DATALINE where true:
                                                         2->1:String;
                                                         2->1:String;
                                                         $smtp_data,
                                                       ^ENDOFDATA where true:
                                                         $smtp_auth,
                                                       ^SUBJECT where true:
                                                         2->1:String;
                                                         2->1:String;
                                                         $smtp_data
                                                     }
                                                 },
                                               ^RCPT where true:
                                                 2->1:String;
                                                 1->2
                                                 { ^250 where true:
                                                     $smtp_mail
                                                 }
                                             },
                                           ^501 where true:
                                             $smtp_auth
                                         },
                                       ^QUIT where true:
                                         $end;
                                     }(1 of 1, 2) this)
                ( this[2] >>
                  { ^MAIL:
                    if (SYSTEM & "tprocs" <= SYSTEM & "aprocs")
                    then this[2] >> from;
                         Bool valid_from=true;
                         if valid_from
                         then this[2] << ^250;
                              local SmtpServerMail(rec $smtp_mail;
                                                   2->1
                                                   { ^DATA where true:
                                                       1->2
                                                       { ^354 where true:
                                                           2->1:String;
                                                           2->1:String;
                                                           rec $smtp_data;
                                                           2->1
                                                           { ^DATALINE where true:
                                                               2->1:String;
                                                               2->1:String;
                                                               $smtp_data,
                                                             ^ENDOFDATA where true:
                                                               rec $smtp_auth;
                                                               2->1
                                                               { ^MAIL where true:
                                                                   2->1:String;
                                                                   1->2
                                                                   { ^250 where true:
                                                                       rec $smtp_mail;
                                                                       2->1
                                                                       { ^DATA where true:
                                                                           1->2
                                                                           { ^354 where true:
                                                                               2->1:String;
                                                                               2->1:String;
                                                                               rec $smtp_data;
                                                                               2->1
                                                                               { ^DATALINE where true:
                                                                                   2->1:String;
                                                                                   2->1:String;
                                                                                   $smtp_data,
                                                                                 ^ENDOFDATA where true:
                                                                                   $smtp_auth,
                                                                                 ^SUBJECT where true:
                                                                                   2->1:String;
                                                                                   2->1:String;
                                                                                   $smtp_data
                                                                               }
                                                                           },
                                                                         ^RCPT where true:
                                                                           2->1:String;
                                                                           1->2
                                                                           { ^250 where true:
                                                                               $smtp_mail
                                                                           }
                                                                       },
                                                                     ^501 where true:
                                                                       $smtp_auth
                                                                   },
                                                                 ^QUIT where true:
                                                                   $end;
                                                               },
                                                             ^SUBJECT where true:
                                                               2->1:String;
                                                               2->1:String;
                                                               $smtp_data
                                                           }
                                                       },
                                                     ^RCPT where true:
                                                       2->1:String;
                                                       1->2
                                                       { ^250 where true:
                                                           $smtp_mail
                                                       }
                                                   }(1 of 1, 2) this,
                                                   String rcpt)
                              ( this[2] >>
                                { ^DATA:
                                  this[2] << ^354;
                                  this[2] >> to;
                                  this[2] >> from;
                                  local SmtpServerData(rec $smtp_data;
                                                       2->1
                                                       { ^DATALINE where true:
                                                           2->1:String;
                                                           2->1:String;
                                                           $smtp_data,
                                                         ^ENDOFDATA where true:
                                                           rec $smtp_auth;
                                                           2->1
                                                           { ^MAIL where true:
                                                               2->1:String;
                                                               1->2
                                                               { ^250 where true:
                                                                   rec $smtp_mail;
                                                                   2->1
                                                                   { ^DATA where true:
                                                                       1->2
                                                                       { ^354 where true:
                                                                           2->1:String;
                                                                           2->1:String;
                                                                           rec $smtp_data;
                                                                           2->1
                                                                           { ^DATALINE where true:
                                                                               2->1:String;
                                                                               2->1:String;
                                                                               $smtp_data,
                                                                             ^ENDOFDATA where true:
                                                                               $smtp_auth,
                                                                             ^SUBJECT where true:
                                                                               2->1:String;
                                                                               2->1:String;
                                                                               $smtp_data
                                                                           }
                                                                       },
                                                                     ^RCPT where true:
                                                                       2->1:String;
                                                                       1->2
                                                                       { ^250 where true:
                                                                           $smtp_mail
                                                                       }
                                                                   },
                                                                 ^501 where true:
                                                                   $smtp_auth
                                                               },
                                                             ^QUIT where true:
                                                               $end;
                                                           },
                                                         ^SUBJECT where true:
                                                           2->1:String;
                                                           2->1:String;
                                                           $smtp_data
                                                       }(1 of 1, 2) this,
                                                       String subject,
                                                       String data)
                                  ( this[2] >>
                                    { ^DATALINE:
                                      this[2] >> line1;
                                      this[2] >> line2;
                                      SmtpServerData(this, subject, line1);,
                                      ^ENDOFDATA:
                                      c= new console(2 of 2);
                                      c[1] << ^str;
                                      c[1] << "Server received mail";
                                      c[1] << ^nl;
                                      c[1] << ^str;
                                      c[1] << "FROM: ";
                                      c[1] << ^str;
                                      c[1] << from;
                                      c[1] << ^nl;
                                      c[1] << ^str;
                                      c[1] << "RCPT: ";
                                      c[1] << ^str;
                                      c[1] << rcpt;
                                      c[1] << ^nl;
                                      c[1] << ^str;
                                      c[1] << "SUBJ: ";
                                      c[1] << ^str;
                                      c[1] << subject;
                                      c[1] << ^nl;
                                      c[1] << ^end;
                                      SmtpServerAuth(this);,
                                      ^SUBJECT:
                                      this[2] >> s1;
                                      this[2] >> s2;
                                      SmtpServerData(this, s1, data);
                                    }
                                  )
                                  SmtpServerData(this, "", "");,
                                  ^RCPT:
                                  this[2] >> r;
                                  this[2] << ^250;
                                  SmtpServerMail(this, r);
                                }
                              )
                              SmtpServerMail(this, "");
                         else this[2] << ^501;
                              SmtpServerAuth(this);
                    else Bool valid_from=true;
                         this[2] >> from;
                         if valid_from
                         then this[2] << ^250;
                              local SmtpServerMail(rec $smtp_mail;
                                                   2->1
                                                   { ^DATA where true:
                                                       1->2
                                                       { ^354 where true:
                                                           2->1:String;
                                                           2->1:String;
                                                           rec $smtp_data;
                                                           2->1
                                                           { ^DATALINE where true:
                                                               2->1:String;
                                                               2->1:String;
                                                               $smtp_data,
                                                             ^ENDOFDATA where true:
                                                               rec $smtp_auth;
                                                               2->1
                                                               { ^MAIL where true:
                                                                   2->1:String;
                                                                   1->2
                                                                   { ^250 where true:
                                                                       rec $smtp_mail;
                                                                       2->1
                                                                       { ^DATA where true:
                                                                           1->2
                                                                           { ^354 where true:
                                                                               2->1:String;
                                                                               2->1:String;
                                                                               rec $smtp_data;
                                                                               2->1
                                                                               { ^DATALINE where true:
                                                                                   2->1:String;
                                                                                   2->1:String;
                                                                                   $smtp_data,
                                                                                 ^ENDOFDATA where true:
                                                                                   $smtp_auth,
                                                                                 ^SUBJECT where true:
                                                                                   2->1:String;
                                                                                   2->1:String;
                                                                                   $smtp_data
                                                                               }
                                                                           },
                                                                         ^RCPT where true:
                                                                           2->1:String;
                                                                           1->2
                                                                           { ^250 where true:
                                                                               $smtp_mail
                                                                           }
                                                                       },
                                                                     ^501 where true:
                                                                       $smtp_auth
                                                                   },
                                                                 ^QUIT where true:
                                                                   $end;
                                                               },
                                                             ^SUBJECT where true:
                                                               2->1:String;
                                                               2->1:String;
                                                               $smtp_data
                                                           }
                                                       },
                                                     ^RCPT where true:
                                                       2->1:String;
                                                       1->2
                                                       { ^250 where true:
                                                           $smtp_mail
                                                       }
                                                   }(1 of 1, 2) this,
                                                   String rcpt)
                              ( this[2] >>
                                { ^DATA:
                                  this[2] << ^354;
                                  this[2] >> to;
                                  this[2] >> from;
                                  local SmtpServerData(rec $smtp_data;
                                                       2->1
                                                       { ^DATALINE where true:
                                                           2->1:String;
                                                           2->1:String;
                                                           $smtp_data,
                                                         ^ENDOFDATA where true:
                                                           rec $smtp_auth;
                                                           2->1
                                                           { ^MAIL where true:
                                                               2->1:String;
                                                               1->2
                                                               { ^250 where true:
                                                                   rec $smtp_mail;
                                                                   2->1
                                                                   { ^DATA where true:
                                                                       1->2
                                                                       { ^354 where true:
                                                                           2->1:String;
                                                                           2->1:String;
                                                                           rec $smtp_data;
                                                                           2->1
                                                                           { ^DATALINE where true:
                                                                               2->1:String;
                                                                               2->1:String;
                                                                               $smtp_data,
                                                                             ^ENDOFDATA where true:
                                                                               $smtp_auth,
                                                                             ^SUBJECT where true:
                                                                               2->1:String;
                                                                               2->1:String;
                                                                               $smtp_data
                                                                           }
                                                                       },
                                                                     ^RCPT where true:
                                                                       2->1:String;
                                                                       1->2
                                                                       { ^250 where true:
                                                                           $smtp_mail
                                                                       }
                                                                   },
                                                                 ^501 where true:
                                                                   $smtp_auth
                                                               },
                                                             ^QUIT where true:
                                                               $end;
                                                           },
                                                         ^SUBJECT where true:
                                                           2->1:String;
                                                           2->1:String;
                                                           $smtp_data
                                                       }(1 of 1, 2) this,
                                                       String subject,
                                                       String data)
                                  ( this[2] >>
                                    { ^DATALINE:
                                      this[2] >> line1;
                                      this[2] >> line2;
                                      SmtpServerData(this, subject, line1);,
                                      ^ENDOFDATA:
                                      c= new console(2 of 2);
                                      c[1] << ^str;
                                      c[1] << "Server received mail";
                                      c[1] << ^nl;
                                      c[1] << ^str;
                                      c[1] << "FROM: ";
                                      c[1] << ^str;
                                      c[1] << from;
                                      c[1] << ^nl;
                                      c[1] << ^str;
                                      c[1] << "RCPT: ";
                                      c[1] << ^str;
                                      c[1] << rcpt;
                                      c[1] << ^nl;
                                      c[1] << ^str;
                                      c[1] << "SUBJ: ";
                                      c[1] << ^str;
                                      c[1] << subject;
                                      c[1] << ^nl;
                                      c[1] << ^end;
                                      SmtpServerAuth(this);,
                                      ^SUBJECT:
                                      this[2] >> s1;
                                      this[2] >> s2;
                                      SmtpServerData(this, s1, data);
                                    }
                                  )
                                  SmtpServerData(this, "", "");,
                                  ^RCPT:
                                  this[2] >> r;
                                  this[2] << ^250;
                                  SmtpServerMail(this, r);
                                }
                              )
                              SmtpServerMail(this, "");
                         else this[2] << ^501;
                              SmtpServerAuth(this);,
                    ^QUIT:
                    
                  }
                )
                SmtpServerAuth(this);,
                ^QUIT:
                
              },
              ^QUIT:
              
            }
          },
          ^QUIT:
          
        }
      )
    )
    local StartService(Int i)
      ( if (i <= 0)
        then SmtpServer();
        else ( SmtpServer();
             | StartService((i - 1));
             )
      )
      StartService(SYSTEM & "tprocs");
  | client= new smtp(2 of 2);
    client[1] >>
    { ^220_SERVICE_READY:
      client[1] >> msg;
      client[1] << ^EHLO;
      client[1] << "user";
      local SmtpClientEhlo(rec $smtp_ehlo;
                           1->2
                           { ^250 where true:
                               1->2:String;
                               2->1
                               { ^QUIT where true:
                                   $end;,
                                 ^StartTLS where true:
                                   1->2
                                   { ^220 where true:
                                       2->1
                                       { ^EHLO where true:
                                           rec $smtp_tls;
                                           1->2
                                           { ^250 where true:
                                               1->2:String;
                                               rec $smtp_cmd;
                                               2->1
                                               { ^AUTH where true:
                                                   1->2
                                                   { ^235 where true:
                                                       rec $smtp_auth;
                                                       2->1
                                                       { ^MAIL where true:
                                                           2->1:String;
                                                           1->2
                                                           { ^250 where true:
                                                               rec $smtp_mail;
                                                               2->1
                                                               { ^DATA where true:
                                                                   1->2
                                                                   { ^354 where true:
                                                                       2->1:String;
                                                                       2->1:String;
                                                                       rec $smtp_data;
                                                                       2->1
                                                                       { ^DATALINE where true:
                                                                           2->1:String;
                                                                           2->1:String;
                                                                           $smtp_data,
                                                                         ^ENDOFDATA where true:
                                                                           $smtp_auth,
                                                                         ^SUBJECT where true:
                                                                           2->1:String;
                                                                           2->1:String;
                                                                           $smtp_data
                                                                       }
                                                                   },
                                                                 ^RCPT where true:
                                                                   2->1:String;
                                                                   1->2
                                                                   { ^250 where true:
                                                                       $smtp_mail
                                                                   }
                                                               },
                                                             ^501 where true:
                                                               $smtp_auth
                                                           },
                                                         ^QUIT where true:
                                                           $end;
                                                       },
                                                     ^535 where true:
                                                       $smtp_cmd
                                                   },
                                                 ^QUIT where true:
                                                   $end;
                                               },
                                             ^250d where true:
                                               $smtp_tls
                                           },
                                         ^QUIT where true:
                                           $end;
                                       }
                                   }
                               },
                             ^250d where true:
                               $smtp_ehlo
                           }(2 of 1, 2) this)
      ( this[1] >>
        { ^250:
          this[1] >> srv;
          this[1] << ^StartTLS;
          this[1] >>
          { ^220:
            this[1] << ^EHLO;
            local SmtpClientTls(rec $smtp_tls;
                                1->2
                                { ^250 where true:
                                    1->2:String;
                                    rec $smtp_cmd;
                                    2->1
                                    { ^AUTH where true:
                                        1->2
                                        { ^235 where true:
                                            rec $smtp_auth;
                                            2->1
                                            { ^MAIL where true:
                                                2->1:String;
                                                1->2
                                                { ^250 where true:
                                                    rec $smtp_mail;
                                                    2->1
                                                    { ^DATA where true:
                                                        1->2
                                                        { ^354 where true:
                                                            2->1:String;
                                                            2->1:String;
                                                            rec $smtp_data;
                                                            2->1
                                                            { ^DATALINE where true:
                                                                2->1:String;
                                                                2->1:String;
                                                                $smtp_data,
                                                              ^ENDOFDATA where true:
                                                                $smtp_auth,
                                                              ^SUBJECT where true:
                                                                2->1:String;
                                                                2->1:String;
                                                                $smtp_data
                                                            }
                                                        },
                                                      ^RCPT where true:
                                                        2->1:String;
                                                        1->2
                                                        { ^250 where true:
                                                            $smtp_mail
                                                        }
                                                    },
                                                  ^501 where true:
                                                    $smtp_auth
                                                },
                                              ^QUIT where true:
                                                $end;
                                            },
                                          ^535 where true:
                                            $smtp_cmd
                                        },
                                      ^QUIT where true:
                                        $end;
                                    },
                                  ^250d where true:
                                    $smtp_tls
                                }(2 of 1, 2) this)
            ( this[1] >>
              { ^250:
                this[1] >> srv;
                this[1] << ^AUTH;
                this[1] >>
                { ^235:
                  this[1] << ^MAIL;
                  this[1] << "user@smtpserver.pi";
                  this[1] >>
                  { ^250:
                    this[1] << ^RCPT;
                    this[1] << "friend@smtpserver.pi";
                    this[1] >>
                    { ^250:
                      this[1] << ^DATA;
                      this[1] >>
                      { ^354:
                        this[1] << "friend@smtpserver.pi";
                        this[1] << "user@smtpserver.pi";
                        this[1] << ^SUBJECT;
                        this[1] << "SMTP MESSAGE";
                        this[1] << "SUBJECT";
                        this[1] << ^DATALINE;
                        this[1] << "MSG1";
                        this[1] << "MSG2";
                        this[1] << ^ENDOFDATA;
                        this[1] << ^QUIT;
                        
                      }
                    },
                    ^501:
                    this[1] << ^QUIT;
                    
                  },
                  ^535:
                  this[1] << ^QUIT;
                  
                },
                ^250d:
                SmtpClientTls(this);
              }
            )
            SmtpClientTls(this);
          },
          ^250d:
          SmtpClientEhlo(this);
        }
      )
      SmtpClientEhlo(client);
    }
  )
)
*/
/* ==== RENAMED ====
HOSTHEADER(""#include <iostream>"");
rec ~$$console1;
2->1
{ ^end where true:
    $end;,
  ^flt where true:
    2->1:Float;
    ~$$console1,
  ^input where true:
    2->1
    { ^int where true:
        1->2:Int;
        ~$$console1,
      ^str where true:
        1->2:String;
        ~$$console1
    },
  ^int where true:
    2->1:Int;
    ~$$console1,
  ^nl where true:
    ~$$console1,
  ^str where true:
    2->1:String;
    ~$$console1
} ~console4(1, 2);
( local ~Console1()
  ( ~s5= new ~console4(1 of 2);
    local ~ConsoleSession2(rec ~$$console1;
                           2->1
                           { ^end where true:
                               $end;,
                             ^flt where true:
                               2->1:Float;
                               ~$$console1,
                             ^input where true:
                               2->1
                               { ^int where true:
                                   1->2:Int;
                                   ~$$console1,
                                 ^str where true:
                                   1->2:String;
                                   ~$$console1
                               },
                             ^int where true:
                               2->1:Int;
                               ~$$console1,
                             ^nl where true:
                               ~$$console1,
                             ^str where true:
                               2->1:String;
                               ~$$console1
                           }(1 of 1, 2) ~s6)
    ( ~s6[2] >>
      { ^end:
        ~Console1();,
        ^flt:
        ~s6[2] >> ~f12;
        HOST("std::cout << ", ~f12", "->ToString() << std::flush;");
        ~ConsoleSession2(~s6);,
        ^input:
        ~s6[2] >>
        { ^int:
          Int ~val13=0;
          HOST("{ long _l; std::cin >> _l; ", ~val13", "=new libpi::Int(_l); }");
          ~s6[2] << ~val13;
          ~ConsoleSession2(~s6);,
          ^str:
          String ~val14="";
          HOST("{ std::string _s; std::cin >> _s; ", ~val14", "=new libpi::String(_s); }");
          ~s6[2] << ~val14;
          ~ConsoleSession2(~s6);
        },
        ^int:
        ~s6[2] >> ~i15;
        HOST("std::cout << ", ~i15", "->ToString() << std::flush;");
        ~ConsoleSession2(~s6);,
        ^nl:
        HOST("std::cout << std::endl;");
        ~ConsoleSession2(~s6);,
        ^str:
        ~s6[2] >> ~str16;
        HOST("std::cout << ", ~str16", "->GetValue() << std::flush;");
        ~ConsoleSession2(~s6);
      }
    )
    ~ConsoleSession2(~s5);
  )
  ~Console1();
| 1->2
  { ^220_SERVICE_READY where true:
      1->2:String;
      2->1
      { ^EHLO where true:
          2->1:String;
          rec ~$$smtp_ehlo1;
          1->2
          { ^250 where true:
              1->2:String;
              2->1
              { ^QUIT where true:
                  $end;,
                ^StartTLS where true:
                  1->2
                  { ^220 where true:
                      2->1
                      { ^EHLO where true:
                          rec ~$$smtp_tls1;
                          1->2
                          { ^250 where true:
                              1->2:String;
                              rec ~$$smtp_cmd1;
                              2->1
                              { ^AUTH where true:
                                  1->2
                                  { ^235 where true:
                                      rec ~$$smtp_auth1;
                                      2->1
                                      { ^MAIL where true:
                                          2->1:String;
                                          1->2
                                          { ^250 where true:
                                              rec ~$$smtp_mail1;
                                              2->1
                                              { ^DATA where true:
                                                  1->2
                                                  { ^354 where true:
                                                      2->1:String;
                                                      2->1:String;
                                                      rec ~$$smtp_data1;
                                                      2->1
                                                      { ^DATALINE where true:
                                                          2->1:String;
                                                          2->1:String;
                                                          ~$$smtp_data1,
                                                        ^ENDOFDATA where true:
                                                          ~$$smtp_auth1,
                                                        ^SUBJECT where true:
                                                          2->1:String;
                                                          2->1:String;
                                                          ~$$smtp_data1
                                                      }
                                                  },
                                                ^RCPT where true:
                                                  2->1:String;
                                                  1->2
                                                  { ^250 where true:
                                                      ~$$smtp_mail1
                                                  }
                                              },
                                            ^501 where true:
                                              ~$$smtp_auth1
                                          },
                                        ^QUIT where true:
                                          $end;
                                      },
                                    ^535 where true:
                                      ~$$smtp_cmd1
                                  },
                                ^QUIT where true:
                                  $end;
                              },
                            ^250d where true:
                              ~$$smtp_tls1
                          },
                        ^QUIT where true:
                          $end;
                      }
                  }
              },
            ^250d where true:
              ~$$smtp_ehlo1
          },
        ^QUIT where true:
          $end;
      }
  } ~smtp17(1, 2);
  ( local ~SmtpServer3()
    ( ~this18= new ~smtp17(1 of 2);
      ( ~SmtpServer3();
      | ~this18[2] << ^220_SERVICE_READY;
        ~this18[2] << "Welcome to the SMTP service!";
        ~this18[2] >>
        { ^EHLO:
          ~this18[2] >> ~msg19;
          ~this18[2] << ^250;
          ~this18[2] << "smtpservice.pi";
          ~this18[2] >>
          { ^QUIT:
            ,
            ^StartTLS:
            ~this18[2] << ^220;
            ~this18[2] >>
            { ^EHLO:
              ~this18[2] << ^250;
              ~this18[2] << "smtpservice.pi";
              ~this18[2] >>
              { ^AUTH:
                ~this18[2] << ^235;
                local ~SmtpServerAuth4(rec ~$$smtp_auth1;
                                       2->1
                                       { ^MAIL where true:
                                           2->1:String;
                                           1->2
                                           { ^250 where true:
                                               rec ~$$smtp_mail1;
                                               2->1
                                               { ^DATA where true:
                                                   1->2
                                                   { ^354 where true:
                                                       2->1:String;
                                                       2->1:String;
                                                       rec ~$$smtp_data1;
                                                       2->1
                                                       { ^DATALINE where true:
                                                           2->1:String;
                                                           2->1:String;
                                                           ~$$smtp_data1,
                                                         ^ENDOFDATA where true:
                                                           ~$$smtp_auth1,
                                                         ^SUBJECT where true:
                                                           2->1:String;
                                                           2->1:String;
                                                           ~$$smtp_data1
                                                       }
                                                   },
                                                 ^RCPT where true:
                                                   2->1:String;
                                                   1->2
                                                   { ^250 where true:
                                                       ~$$smtp_mail1
                                                   }
                                               },
                                             ^501 where true:
                                               ~$$smtp_auth1
                                           },
                                         ^QUIT where true:
                                           $end;
                                       }(1 of 1, 2) ~this20)
                ( ~this20[2] >>
                  { ^MAIL:
                    if (SYSTEM & "tprocs" <= SYSTEM & "aprocs")
                    then ~this20[2] >> ~from29;
                         Bool ~valid_from30=true;
                         if ~valid_from30
                         then ~this20[2] << ^250;
                              local ~SmtpServerMail5(rec ~$$smtp_mail1;
                                                     2->1
                                                     { ^DATA where true:
                                                         1->2
                                                         { ^354 where true:
                                                             2->1:String;
                                                             2->1:String;
                                                             rec ~$$smtp_data1;
                                                             2->1
                                                             { ^DATALINE where true:
                                                                 2->1:String;
                                                                 2->1:String;
                                                                 ~$$smtp_data1,
                                                               ^ENDOFDATA where true:
                                                                 rec ~$$smtp_auth1;
                                                                 2->1
                                                                 { ^MAIL where true:
                                                                     2->1:String;
                                                                     1->2
                                                                     { ^250 where true:
                                                                         rec ~$$smtp_mail1;
                                                                         2->1
                                                                         { ^DATA where true:
                                                                             1->2
                                                                             { ^354 where true:
                                                                                 2->1:String;
                                                                                 2->1:String;
                                                                                 rec ~$$smtp_data1;
                                                                                 2->1
                                                                                 { ^DATALINE where true:
                                                                                     2->1:String;
                                                                                     2->1:String;
                                                                                     ~$$smtp_data1,
                                                                                   ^ENDOFDATA where true:
                                                                                     ~$$smtp_auth1,
                                                                                   ^SUBJECT where true:
                                                                                     2->1:String;
                                                                                     2->1:String;
                                                                                     ~$$smtp_data1
                                                                                 }
                                                                             },
                                                                           ^RCPT where true:
                                                                             2->1:String;
                                                                             1->2
                                                                             { ^250 where true:
                                                                                 ~$$smtp_mail1
                                                                             }
                                                                         },
                                                                       ^501 where true:
                                                                         ~$$smtp_auth1
                                                                     },
                                                                   ^QUIT where true:
                                                                     $end;
                                                                 },
                                                               ^SUBJECT where true:
                                                                 2->1:String;
                                                                 2->1:String;
                                                                 ~$$smtp_data1
                                                             }
                                                         },
                                                       ^RCPT where true:
                                                         2->1:String;
                                                         1->2
                                                         { ^250 where true:
                                                             ~$$smtp_mail1
                                                         }
                                                     }(1 of 1, 2) ~this31,
                                                     String ~rcpt32)
                              ( ~this31[2] >>
                                { ^DATA:
                                  ~this31[2] << ^354;
                                  ~this31[2] >> ~to48;
                                  ~this31[2] >> ~from49;
                                  local ~SmtpServerData6(rec ~$$smtp_data1;
                                                         2->1
                                                         { ^DATALINE where true:
                                                             2->1:String;
                                                             2->1:String;
                                                             ~$$smtp_data1,
                                                           ^ENDOFDATA where true:
                                                             rec ~$$smtp_auth1;
                                                             2->1
                                                             { ^MAIL where true:
                                                                 2->1:String;
                                                                 1->2
                                                                 { ^250 where true:
                                                                     rec ~$$smtp_mail1;
                                                                     2->1
                                                                     { ^DATA where true:
                                                                         1->2
                                                                         { ^354 where true:
                                                                             2->1:String;
                                                                             2->1:String;
                                                                             rec ~$$smtp_data1;
                                                                             2->1
                                                                             { ^DATALINE where true:
                                                                                 2->1:String;
                                                                                 2->1:String;
                                                                                 ~$$smtp_data1,
                                                                               ^ENDOFDATA where true:
                                                                                 ~$$smtp_auth1,
                                                                               ^SUBJECT where true:
                                                                                 2->1:String;
                                                                                 2->1:String;
                                                                                 ~$$smtp_data1
                                                                             }
                                                                         },
                                                                       ^RCPT where true:
                                                                         2->1:String;
                                                                         1->2
                                                                         { ^250 where true:
                                                                             ~$$smtp_mail1
                                                                         }
                                                                     },
                                                                   ^501 where true:
                                                                     ~$$smtp_auth1
                                                                 },
                                                               ^QUIT where true:
                                                                 $end;
                                                             },
                                                           ^SUBJECT where true:
                                                             2->1:String;
                                                             2->1:String;
                                                             ~$$smtp_data1
                                                         }(1 of 1, 2) ~this50,
                                                         String ~subject51,
                                                         String ~data52)
                                  ( ~this50[2] >>
                                    { ^DATALINE:
                                      ~this50[2] >> ~line165;
                                      ~this50[2] >> ~line266;
                                      ~SmtpServerData6(~this50, ~subject51, ~line165);,
                                      ^ENDOFDATA:
                                      ~c67= new ~console4(2 of 2);
                                      ~c67[1] << ^str;
                                      ~c67[1] << "Server received mail";
                                      ~c67[1] << ^nl;
                                      ~c67[1] << ^str;
                                      ~c67[1] << "FROM: ";
                                      ~c67[1] << ^str;
                                      ~c67[1] << ~from49;
                                      ~c67[1] << ^nl;
                                      ~c67[1] << ^str;
                                      ~c67[1] << "RCPT: ";
                                      ~c67[1] << ^str;
                                      ~c67[1] << ~rcpt32;
                                      ~c67[1] << ^nl;
                                      ~c67[1] << ^str;
                                      ~c67[1] << "SUBJ: ";
                                      ~c67[1] << ^str;
                                      ~c67[1] << ~subject51;
                                      ~c67[1] << ^nl;
                                      ~c67[1] << ^end;
                                      ~SmtpServerAuth4(~this50);,
                                      ^SUBJECT:
                                      ~this50[2] >> ~s168;
                                      ~this50[2] >> ~s269;
                                      ~SmtpServerData6(~this50, ~s168, ~data52);
                                    }
                                  )
                                  ~SmtpServerData6(~this31, "", "");,
                                  ^RCPT:
                                  ~this31[2] >> ~r70;
                                  ~this31[2] << ^250;
                                  ~SmtpServerMail5(~this31, ~r70);
                                }
                              )
                              ~SmtpServerMail5(~this20, "");
                         else ~this20[2] << ^501;
                              ~SmtpServerAuth4(~this20);
                    else Bool ~valid_from71=true;
                         ~this20[2] >> ~from72;
                         if ~valid_from71
                         then ~this20[2] << ^250;
                              local ~SmtpServerMail7(rec ~$$smtp_mail1;
                                                     2->1
                                                     { ^DATA where true:
                                                         1->2
                                                         { ^354 where true:
                                                             2->1:String;
                                                             2->1:String;
                                                             rec ~$$smtp_data1;
                                                             2->1
                                                             { ^DATALINE where true:
                                                                 2->1:String;
                                                                 2->1:String;
                                                                 ~$$smtp_data1,
                                                               ^ENDOFDATA where true:
                                                                 rec ~$$smtp_auth1;
                                                                 2->1
                                                                 { ^MAIL where true:
                                                                     2->1:String;
                                                                     1->2
                                                                     { ^250 where true:
                                                                         rec ~$$smtp_mail1;
                                                                         2->1
                                                                         { ^DATA where true:
                                                                             1->2
                                                                             { ^354 where true:
                                                                                 2->1:String;
                                                                                 2->1:String;
                                                                                 rec ~$$smtp_data1;
                                                                                 2->1
                                                                                 { ^DATALINE where true:
                                                                                     2->1:String;
                                                                                     2->1:String;
                                                                                     ~$$smtp_data1,
                                                                                   ^ENDOFDATA where true:
                                                                                     ~$$smtp_auth1,
                                                                                   ^SUBJECT where true:
                                                                                     2->1:String;
                                                                                     2->1:String;
                                                                                     ~$$smtp_data1
                                                                                 }
                                                                             },
                                                                           ^RCPT where true:
                                                                             2->1:String;
                                                                             1->2
                                                                             { ^250 where true:
                                                                                 ~$$smtp_mail1
                                                                             }
                                                                         },
                                                                       ^501 where true:
                                                                         ~$$smtp_auth1
                                                                     },
                                                                   ^QUIT where true:
                                                                     $end;
                                                                 },
                                                               ^SUBJECT where true:
                                                                 2->1:String;
                                                                 2->1:String;
                                                                 ~$$smtp_data1
                                                             }
                                                         },
                                                       ^RCPT where true:
                                                         2->1:String;
                                                         1->2
                                                         { ^250 where true:
                                                             ~$$smtp_mail1
                                                         }
                                                     }(1 of 1, 2) ~this73,
                                                     String ~rcpt74)
                              ( ~this73[2] >>
                                { ^DATA:
                                  ~this73[2] << ^354;
                                  ~this73[2] >> ~to90;
                                  ~this73[2] >> ~from91;
                                  local ~SmtpServerData8(rec ~$$smtp_data1;
                                                         2->1
                                                         { ^DATALINE where true:
                                                             2->1:String;
                                                             2->1:String;
                                                             ~$$smtp_data1,
                                                           ^ENDOFDATA where true:
                                                             rec ~$$smtp_auth1;
                                                             2->1
                                                             { ^MAIL where true:
                                                                 2->1:String;
                                                                 1->2
                                                                 { ^250 where true:
                                                                     rec ~$$smtp_mail1;
                                                                     2->1
                                                                     { ^DATA where true:
                                                                         1->2
                                                                         { ^354 where true:
                                                                             2->1:String;
                                                                             2->1:String;
                                                                             rec ~$$smtp_data1;
                                                                             2->1
                                                                             { ^DATALINE where true:
                                                                                 2->1:String;
                                                                                 2->1:String;
                                                                                 ~$$smtp_data1,
                                                                               ^ENDOFDATA where true:
                                                                                 ~$$smtp_auth1,
                                                                               ^SUBJECT where true:
                                                                                 2->1:String;
                                                                                 2->1:String;
                                                                                 ~$$smtp_data1
                                                                             }
                                                                         },
                                                                       ^RCPT where true:
                                                                         2->1:String;
                                                                         1->2
                                                                         { ^250 where true:
                                                                             ~$$smtp_mail1
                                                                         }
                                                                     },
                                                                   ^501 where true:
                                                                     ~$$smtp_auth1
                                                                 },
                                                               ^QUIT where true:
                                                                 $end;
                                                             },
                                                           ^SUBJECT where true:
                                                             2->1:String;
                                                             2->1:String;
                                                             ~$$smtp_data1
                                                         }(1 of 1, 2) ~this92,
                                                         String ~subject93,
                                                         String ~data94)
                                  ( ~this92[2] >>
                                    { ^DATALINE:
                                      ~this92[2] >> ~line1107;
                                      ~this92[2] >> ~line2108;
                                      ~SmtpServerData8(~this92, ~subject93, ~line1107);,
                                      ^ENDOFDATA:
                                      ~c109= new ~console4(2 of 2);
                                      ~c109[1] << ^str;
                                      ~c109[1] << "Server received mail";
                                      ~c109[1] << ^nl;
                                      ~c109[1] << ^str;
                                      ~c109[1] << "FROM: ";
                                      ~c109[1] << ^str;
                                      ~c109[1] << ~from91;
                                      ~c109[1] << ^nl;
                                      ~c109[1] << ^str;
                                      ~c109[1] << "RCPT: ";
                                      ~c109[1] << ^str;
                                      ~c109[1] << ~rcpt74;
                                      ~c109[1] << ^nl;
                                      ~c109[1] << ^str;
                                      ~c109[1] << "SUBJ: ";
                                      ~c109[1] << ^str;
                                      ~c109[1] << ~subject93;
                                      ~c109[1] << ^nl;
                                      ~c109[1] << ^end;
                                      ~SmtpServerAuth4(~this92);,
                                      ^SUBJECT:
                                      ~this92[2] >> ~s1110;
                                      ~this92[2] >> ~s2111;
                                      ~SmtpServerData8(~this92, ~s1110, ~data94);
                                    }
                                  )
                                  ~SmtpServerData8(~this73, "", "");,
                                  ^RCPT:
                                  ~this73[2] >> ~r112;
                                  ~this73[2] << ^250;
                                  ~SmtpServerMail7(~this73, ~r112);
                                }
                              )
                              ~SmtpServerMail7(~this20, "");
                         else ~this20[2] << ^501;
                              ~SmtpServerAuth4(~this20);,
                    ^QUIT:
                    
                  }
                )
                ~SmtpServerAuth4(~this18);,
                ^QUIT:
                
              },
              ^QUIT:
              
            }
          },
          ^QUIT:
          
        }
      )
    )
    local ~StartService9(Int ~i113)
      ( if (~i113 <= 0)
        then ~SmtpServer3();
        else ( ~SmtpServer3();
             | ~StartService9((~i113 - 1));
             )
      )
      ~StartService9(SYSTEM & "tprocs");
  | ~client114= new ~smtp17(2 of 2);
    ~client114[1] >>
    { ^220_SERVICE_READY:
      ~client114[1] >> ~msg115;
      ~client114[1] << ^EHLO;
      ~client114[1] << "user";
      local ~SmtpClientEhlo10(rec ~$$smtp_ehlo1;
                              1->2
                              { ^250 where true:
                                  1->2:String;
                                  2->1
                                  { ^QUIT where true:
                                      $end;,
                                    ^StartTLS where true:
                                      1->2
                                      { ^220 where true:
                                          2->1
                                          { ^EHLO where true:
                                              rec ~$$smtp_tls1;
                                              1->2
                                              { ^250 where true:
                                                  1->2:String;
                                                  rec ~$$smtp_cmd1;
                                                  2->1
                                                  { ^AUTH where true:
                                                      1->2
                                                      { ^235 where true:
                                                          rec ~$$smtp_auth1;
                                                          2->1
                                                          { ^MAIL where true:
                                                              2->1:String;
                                                              1->2
                                                              { ^250 where true:
                                                                  rec ~$$smtp_mail1;
                                                                  2->1
                                                                  { ^DATA where true:
                                                                      1->2
                                                                      { ^354 where true:
                                                                          2->1:String;
                                                                          2->1:String;
                                                                          rec ~$$smtp_data1;
                                                                          2->1
                                                                          { ^DATALINE where true:
                                                                              2->1:String;
                                                                              2->1:String;
                                                                              ~$$smtp_data1,
                                                                            ^ENDOFDATA where true:
                                                                              ~$$smtp_auth1,
                                                                            ^SUBJECT where true:
                                                                              2->1:String;
                                                                              2->1:String;
                                                                              ~$$smtp_data1
                                                                          }
                                                                      },
                                                                    ^RCPT where true:
                                                                      2->1:String;
                                                                      1->2
                                                                      { ^250 where true:
                                                                          ~$$smtp_mail1
                                                                      }
                                                                  },
                                                                ^501 where true:
                                                                  ~$$smtp_auth1
                                                              },
                                                            ^QUIT where true:
                                                              $end;
                                                          },
                                                        ^535 where true:
                                                          ~$$smtp_cmd1
                                                      },
                                                    ^QUIT where true:
                                                      $end;
                                                  },
                                                ^250d where true:
                                                  ~$$smtp_tls1
                                              },
                                            ^QUIT where true:
                                              $end;
                                          }
                                      }
                                  },
                                ^250d where true:
                                  ~$$smtp_ehlo1
                              }(2 of 1, 2) ~this116)
      ( ~this116[1] >>
        { ^250:
          ~this116[1] >> ~srv127;
          ~this116[1] << ^StartTLS;
          ~this116[1] >>
          { ^220:
            ~this116[1] << ^EHLO;
            local ~SmtpClientTls11(rec ~$$smtp_tls1;
                                   1->2
                                   { ^250 where true:
                                       1->2:String;
                                       rec ~$$smtp_cmd1;
                                       2->1
                                       { ^AUTH where true:
                                           1->2
                                           { ^235 where true:
                                               rec ~$$smtp_auth1;
                                               2->1
                                               { ^MAIL where true:
                                                   2->1:String;
                                                   1->2
                                                   { ^250 where true:
                                                       rec ~$$smtp_mail1;
                                                       2->1
                                                       { ^DATA where true:
                                                           1->2
                                                           { ^354 where true:
                                                               2->1:String;
                                                               2->1:String;
                                                               rec ~$$smtp_data1;
                                                               2->1
                                                               { ^DATALINE where true:
                                                                   2->1:String;
                                                                   2->1:String;
                                                                   ~$$smtp_data1,
                                                                 ^ENDOFDATA where true:
                                                                   ~$$smtp_auth1,
                                                                 ^SUBJECT where true:
                                                                   2->1:String;
                                                                   2->1:String;
                                                                   ~$$smtp_data1
                                                               }
                                                           },
                                                         ^RCPT where true:
                                                           2->1:String;
                                                           1->2
                                                           { ^250 where true:
                                                               ~$$smtp_mail1
                                                           }
                                                       },
                                                     ^501 where true:
                                                       ~$$smtp_auth1
                                                   },
                                                 ^QUIT where true:
                                                   $end;
                                               },
                                             ^535 where true:
                                               ~$$smtp_cmd1
                                           },
                                         ^QUIT where true:
                                           $end;
                                       },
                                     ^250d where true:
                                       ~$$smtp_tls1
                                   }(2 of 1, 2) ~this128)
            ( ~this128[1] >>
              { ^250:
                ~this128[1] >> ~srv138;
                ~this128[1] << ^AUTH;
                ~this128[1] >>
                { ^235:
                  ~this128[1] << ^MAIL;
                  ~this128[1] << "user@smtpserver.pi";
                  ~this128[1] >>
                  { ^250:
                    ~this128[1] << ^RCPT;
                    ~this128[1] << "friend@smtpserver.pi";
                    ~this128[1] >>
                    { ^250:
                      ~this128[1] << ^DATA;
                      ~this128[1] >>
                      { ^354:
                        ~this128[1] << "friend@smtpserver.pi";
                        ~this128[1] << "user@smtpserver.pi";
                        ~this128[1] << ^SUBJECT;
                        ~this128[1] << "SMTP MESSAGE";
                        ~this128[1] << "SUBJECT";
                        ~this128[1] << ^DATALINE;
                        ~this128[1] << "MSG1";
                        ~this128[1] << "MSG2";
                        ~this128[1] << ^ENDOFDATA;
                        ~this128[1] << ^QUIT;
                        
                      }
                    },
                    ^501:
                    ~this128[1] << ^QUIT;
                    
                  },
                  ^535:
                  ~this128[1] << ^QUIT;
                  
                },
                ^250d:
                ~SmtpClientTls11(~this128);
              }
            )
            ~SmtpClientTls11(~this116);
          },
          ^250d:
          ~SmtpClientEhlo10(~this116);
        }
      )
      ~SmtpClientEhlo10(~client114);
    }
  )
)
*/
/* ==== FLATTENFORKED ====
HOSTHEADER(""#include <iostream>"");
rec ~$$console1;
2->1
{ ^end where true:
    $end;,
  ^flt where true:
    2->1:Float;
    ~$$console1,
  ^input where true:
    2->1
    { ^int where true:
        1->2:Int;
        ~$$console1,
      ^str where true:
        1->2:String;
        ~$$console1
    },
  ^int where true:
    2->1:Int;
    ~$$console1,
  ^nl where true:
    ~$$console1,
  ^str where true:
    2->1:String;
    ~$$console1
} ~console4(1, 2);
( local ~Console1()
  ( ~s5= new ~console4(1 of 2);
    local ~ConsoleSession2(rec ~$$console1;
                           2->1
                           { ^end where true:
                               $end;,
                             ^flt where true:
                               2->1:Float;
                               ~$$console1,
                             ^input where true:
                               2->1
                               { ^int where true:
                                   1->2:Int;
                                   ~$$console1,
                                 ^str where true:
                                   1->2:String;
                                   ~$$console1
                               },
                             ^int where true:
                               2->1:Int;
                               ~$$console1,
                             ^nl where true:
                               ~$$console1,
                             ^str where true:
                               2->1:String;
                               ~$$console1
                           }(1 of 1, 2) ~s6)
    ( ~s6[2] >>
      { ^end:
        ~Console1();,
        ^flt:
        ~s6[2] >> ~f12;
        HOST("std::cout << ", ~f12", "->ToString() << std::flush;");
        ~ConsoleSession2(~s6);,
        ^input:
        ~s6[2] >>
        { ^int:
          Int ~val13=0;
          HOST("{ long _l; std::cin >> _l; ", ~val13", "=new libpi::Int(_l); }");
          ~s6[2] << ~val13;
          ~ConsoleSession2(~s6);,
          ^str:
          String ~val14="";
          HOST("{ std::string _s; std::cin >> _s; ", ~val14", "=new libpi::String(_s); }");
          ~s6[2] << ~val14;
          ~ConsoleSession2(~s6);
        },
        ^int:
        ~s6[2] >> ~i15;
        HOST("std::cout << ", ~i15", "->ToString() << std::flush;");
        ~ConsoleSession2(~s6);,
        ^nl:
        HOST("std::cout << std::endl;");
        ~ConsoleSession2(~s6);,
        ^str:
        ~s6[2] >> ~str16;
        HOST("std::cout << ", ~str16", "->GetValue() << std::flush;");
        ~ConsoleSession2(~s6);
      }
    )
    ~ConsoleSession2(~s5);
  )
  ~Console1();
| local ~FlatRight14()
  ( 1->2
    { ^220_SERVICE_READY where true:
        1->2:String;
        2->1
        { ^EHLO where true:
            2->1:String;
            rec ~$$smtp_ehlo1;
            1->2
            { ^250 where true:
                1->2:String;
                2->1
                { ^QUIT where true:
                    $end;,
                  ^StartTLS where true:
                    1->2
                    { ^220 where true:
                        2->1
                        { ^EHLO where true:
                            rec ~$$smtp_tls1;
                            1->2
                            { ^250 where true:
                                1->2:String;
                                rec ~$$smtp_cmd1;
                                2->1
                                { ^AUTH where true:
                                    1->2
                                    { ^235 where true:
                                        rec ~$$smtp_auth1;
                                        2->1
                                        { ^MAIL where true:
                                            2->1:String;
                                            1->2
                                            { ^250 where true:
                                                rec ~$$smtp_mail1;
                                                2->1
                                                { ^DATA where true:
                                                    1->2
                                                    { ^354 where true:
                                                        2->1:String;
                                                        2->1:String;
                                                        rec ~$$smtp_data1;
                                                        2->1
                                                        { ^DATALINE where true:
                                                            2->1:String;
                                                            2->1:String;
                                                            ~$$smtp_data1,
                                                          ^ENDOFDATA where true:
                                                            ~$$smtp_auth1,
                                                          ^SUBJECT where true:
                                                            2->1:String;
                                                            2->1:String;
                                                            ~$$smtp_data1
                                                        }
                                                    },
                                                  ^RCPT where true:
                                                    2->1:String;
                                                    1->2
                                                    { ^250 where true:
                                                        ~$$smtp_mail1
                                                    }
                                                },
                                              ^501 where true:
                                                ~$$smtp_auth1
                                            },
                                          ^QUIT where true:
                                            $end;
                                        },
                                      ^535 where true:
                                        ~$$smtp_cmd1
                                    },
                                  ^QUIT where true:
                                    $end;
                                },
                              ^250d where true:
                                ~$$smtp_tls1
                            },
                          ^QUIT where true:
                            $end;
                        }
                    }
                },
              ^250d where true:
                ~$$smtp_ehlo1
            },
          ^QUIT where true:
            $end;
        }
    } ~smtp17(1, 2);
    ( local ~SmtpServer3()
      ( ~this18= new ~smtp17(1 of 2);
        ( ~SmtpServer3();
        | local ~FlatRight12()
          ( ~this18[2] << ^220_SERVICE_READY;
            ~this18[2] << "Welcome to the SMTP service!";
            ~this18[2] >>
            { ^EHLO:
              ~this18[2] >> ~msg19;
              ~this18[2] << ^250;
              ~this18[2] << "smtpservice.pi";
              ~this18[2] >>
              { ^QUIT:
                ,
                ^StartTLS:
                ~this18[2] << ^220;
                ~this18[2] >>
                { ^EHLO:
                  ~this18[2] << ^250;
                  ~this18[2] << "smtpservice.pi";
                  ~this18[2] >>
                  { ^AUTH:
                    ~this18[2] << ^235;
                    local ~SmtpServerAuth4(rec ~$$smtp_auth1;
                                           2->1
                                           { ^MAIL where true:
                                               2->1:String;
                                               1->2
                                               { ^250 where true:
                                                   rec ~$$smtp_mail1;
                                                   2->1
                                                   { ^DATA where true:
                                                       1->2
                                                       { ^354 where true:
                                                           2->1:String;
                                                           2->1:String;
                                                           rec ~$$smtp_data1;
                                                           2->1
                                                           { ^DATALINE where true:
                                                               2->1:String;
                                                               2->1:String;
                                                               ~$$smtp_data1,
                                                             ^ENDOFDATA where true:
                                                               ~$$smtp_auth1,
                                                             ^SUBJECT where true:
                                                               2->1:String;
                                                               2->1:String;
                                                               ~$$smtp_data1
                                                           }
                                                       },
                                                     ^RCPT where true:
                                                       2->1:String;
                                                       1->2
                                                       { ^250 where true:
                                                           ~$$smtp_mail1
                                                       }
                                                   },
                                                 ^501 where true:
                                                   ~$$smtp_auth1
                                               },
                                             ^QUIT where true:
                                               $end;
                                           }(1 of 1, 2) ~this20)
                    ( ~this20[2] >>
                      { ^MAIL:
                        if (SYSTEM & "tprocs" <= SYSTEM & "aprocs")
                        then ~this20[2] >> ~from29;
                             Bool ~valid_from30=true;
                             if ~valid_from30
                             then ~this20[2] << ^250;
                                  local ~SmtpServerMail5(rec ~$$smtp_mail1;
                                                         2->1
                                                         { ^DATA where true:
                                                             1->2
                                                             { ^354 where true:
                                                                 2->1:String;
                                                                 2->1:String;
                                                                 rec ~$$smtp_data1;
                                                                 2->1
                                                                 { ^DATALINE where true:
                                                                     2->1:String;
                                                                     2->1:String;
                                                                     ~$$smtp_data1,
                                                                   ^ENDOFDATA where true:
                                                                     rec ~$$smtp_auth1;
                                                                     2->1
                                                                     { ^MAIL where true:
                                                                         2->1:String;
                                                                         1->2
                                                                         { ^250 where true:
                                                                             rec ~$$smtp_mail1;
                                                                             2->1
                                                                             { ^DATA where true:
                                                                                 1->2
                                                                                 { ^354 where true:
                                                                                     2->1:String;
                                                                                     2->1:String;
                                                                                     rec ~$$smtp_data1;
                                                                                     2->1
                                                                                     { ^DATALINE where true:
                                                                                         2->1:String;
                                                                                         2->1:String;
                                                                                         ~$$smtp_data1,
                                                                                       ^ENDOFDATA where true:
                                                                                         ~$$smtp_auth1,
                                                                                       ^SUBJECT where true:
                                                                                         2->1:String;
                                                                                         2->1:String;
                                                                                         ~$$smtp_data1
                                                                                     }
                                                                                 },
                                                                               ^RCPT where true:
                                                                                 2->1:String;
                                                                                 1->2
                                                                                 { ^250 where true:
                                                                                     ~$$smtp_mail1
                                                                                 }
                                                                             },
                                                                           ^501 where true:
                                                                             ~$$smtp_auth1
                                                                         },
                                                                       ^QUIT where true:
                                                                         $end;
                                                                     },
                                                                   ^SUBJECT where true:
                                                                     2->1:String;
                                                                     2->1:String;
                                                                     ~$$smtp_data1
                                                                 }
                                                             },
                                                           ^RCPT where true:
                                                             2->1:String;
                                                             1->2
                                                             { ^250 where true:
                                                                 ~$$smtp_mail1
                                                             }
                                                         }(1 of 1, 2) ~this31,
                                                         String ~rcpt32)
                                  ( ~this31[2] >>
                                    { ^DATA:
                                      ~this31[2] << ^354;
                                      ~this31[2] >> ~to48;
                                      ~this31[2] >> ~from49;
                                      local ~SmtpServerData6(rec ~$$smtp_data1;
                                                             2->1
                                                             { ^DATALINE where true:
                                                                 2->1:String;
                                                                 2->1:String;
                                                                 ~$$smtp_data1,
                                                               ^ENDOFDATA where true:
                                                                 rec ~$$smtp_auth1;
                                                                 2->1
                                                                 { ^MAIL where true:
                                                                     2->1:String;
                                                                     1->2
                                                                     { ^250 where true:
                                                                         rec ~$$smtp_mail1;
                                                                         2->1
                                                                         { ^DATA where true:
                                                                             1->2
                                                                             { ^354 where true:
                                                                                 2->1:String;
                                                                                 2->1:String;
                                                                                 rec ~$$smtp_data1;
                                                                                 2->1
                                                                                 { ^DATALINE where true:
                                                                                     2->1:String;
                                                                                     2->1:String;
                                                                                     ~$$smtp_data1,
                                                                                   ^ENDOFDATA where true:
                                                                                     ~$$smtp_auth1,
                                                                                   ^SUBJECT where true:
                                                                                     2->1:String;
                                                                                     2->1:String;
                                                                                     ~$$smtp_data1
                                                                                 }
                                                                             },
                                                                           ^RCPT where true:
                                                                             2->1:String;
                                                                             1->2
                                                                             { ^250 where true:
                                                                                 ~$$smtp_mail1
                                                                             }
                                                                         },
                                                                       ^501 where true:
                                                                         ~$$smtp_auth1
                                                                     },
                                                                   ^QUIT where true:
                                                                     $end;
                                                                 },
                                                               ^SUBJECT where true:
                                                                 2->1:String;
                                                                 2->1:String;
                                                                 ~$$smtp_data1
                                                             }(1 of 1, 2) ~this50,
                                                             String ~subject51,
                                                             String ~data52)
                                      ( ~this50[2] >>
                                        { ^DATALINE:
                                          ~this50[2] >> ~line165;
                                          ~this50[2] >> ~line266;
                                          ~SmtpServerData6(~this50, ~subject51, ~line165);,
                                          ^ENDOFDATA:
                                          ~c67= new ~console4(2 of 2);
                                          ~c67[1] << ^str;
                                          ~c67[1] << "Server received mail";
                                          ~c67[1] << ^nl;
                                          ~c67[1] << ^str;
                                          ~c67[1] << "FROM: ";
                                          ~c67[1] << ^str;
                                          ~c67[1] << ~from49;
                                          ~c67[1] << ^nl;
                                          ~c67[1] << ^str;
                                          ~c67[1] << "RCPT: ";
                                          ~c67[1] << ^str;
                                          ~c67[1] << ~rcpt32;
                                          ~c67[1] << ^nl;
                                          ~c67[1] << ^str;
                                          ~c67[1] << "SUBJ: ";
                                          ~c67[1] << ^str;
                                          ~c67[1] << ~subject51;
                                          ~c67[1] << ^nl;
                                          ~c67[1] << ^end;
                                          ~SmtpServerAuth4(~this50);,
                                          ^SUBJECT:
                                          ~this50[2] >> ~s168;
                                          ~this50[2] >> ~s269;
                                          ~SmtpServerData6(~this50, ~s168, ~data52);
                                        }
                                      )
                                      ~SmtpServerData6(~this31, "", "");,
                                      ^RCPT:
                                      ~this31[2] >> ~r70;
                                      ~this31[2] << ^250;
                                      ~SmtpServerMail5(~this31, ~r70);
                                    }
                                  )
                                  ~SmtpServerMail5(~this20, "");
                             else ~this20[2] << ^501;
                                  ~SmtpServerAuth4(~this20);
                        else Bool ~valid_from71=true;
                             ~this20[2] >> ~from72;
                             if ~valid_from71
                             then ~this20[2] << ^250;
                                  local ~SmtpServerMail7(rec ~$$smtp_mail1;
                                                         2->1
                                                         { ^DATA where true:
                                                             1->2
                                                             { ^354 where true:
                                                                 2->1:String;
                                                                 2->1:String;
                                                                 rec ~$$smtp_data1;
                                                                 2->1
                                                                 { ^DATALINE where true:
                                                                     2->1:String;
                                                                     2->1:String;
                                                                     ~$$smtp_data1,
                                                                   ^ENDOFDATA where true:
                                                                     rec ~$$smtp_auth1;
                                                                     2->1
                                                                     { ^MAIL where true:
                                                                         2->1:String;
                                                                         1->2
                                                                         { ^250 where true:
                                                                             rec ~$$smtp_mail1;
                                                                             2->1
                                                                             { ^DATA where true:
                                                                                 1->2
                                                                                 { ^354 where true:
                                                                                     2->1:String;
                                                                                     2->1:String;
                                                                                     rec ~$$smtp_data1;
                                                                                     2->1
                                                                                     { ^DATALINE where true:
                                                                                         2->1:String;
                                                                                         2->1:String;
                                                                                         ~$$smtp_data1,
                                                                                       ^ENDOFDATA where true:
                                                                                         ~$$smtp_auth1,
                                                                                       ^SUBJECT where true:
                                                                                         2->1:String;
                                                                                         2->1:String;
                                                                                         ~$$smtp_data1
                                                                                     }
                                                                                 },
                                                                               ^RCPT where true:
                                                                                 2->1:String;
                                                                                 1->2
                                                                                 { ^250 where true:
                                                                                     ~$$smtp_mail1
                                                                                 }
                                                                             },
                                                                           ^501 where true:
                                                                             ~$$smtp_auth1
                                                                         },
                                                                       ^QUIT where true:
                                                                         $end;
                                                                     },
                                                                   ^SUBJECT where true:
                                                                     2->1:String;
                                                                     2->1:String;
                                                                     ~$$smtp_data1
                                                                 }
                                                             },
                                                           ^RCPT where true:
                                                             2->1:String;
                                                             1->2
                                                             { ^250 where true:
                                                                 ~$$smtp_mail1
                                                             }
                                                         }(1 of 1, 2) ~this73,
                                                         String ~rcpt74)
                                  ( ~this73[2] >>
                                    { ^DATA:
                                      ~this73[2] << ^354;
                                      ~this73[2] >> ~to90;
                                      ~this73[2] >> ~from91;
                                      local ~SmtpServerData8(rec ~$$smtp_data1;
                                                             2->1
                                                             { ^DATALINE where true:
                                                                 2->1:String;
                                                                 2->1:String;
                                                                 ~$$smtp_data1,
                                                               ^ENDOFDATA where true:
                                                                 rec ~$$smtp_auth1;
                                                                 2->1
                                                                 { ^MAIL where true:
                                                                     2->1:String;
                                                                     1->2
                                                                     { ^250 where true:
                                                                         rec ~$$smtp_mail1;
                                                                         2->1
                                                                         { ^DATA where true:
                                                                             1->2
                                                                             { ^354 where true:
                                                                                 2->1:String;
                                                                                 2->1:String;
                                                                                 rec ~$$smtp_data1;
                                                                                 2->1
                                                                                 { ^DATALINE where true:
                                                                                     2->1:String;
                                                                                     2->1:String;
                                                                                     ~$$smtp_data1,
                                                                                   ^ENDOFDATA where true:
                                                                                     ~$$smtp_auth1,
                                                                                   ^SUBJECT where true:
                                                                                     2->1:String;
                                                                                     2->1:String;
                                                                                     ~$$smtp_data1
                                                                                 }
                                                                             },
                                                                           ^RCPT where true:
                                                                             2->1:String;
                                                                             1->2
                                                                             { ^250 where true:
                                                                                 ~$$smtp_mail1
                                                                             }
                                                                         },
                                                                       ^501 where true:
                                                                         ~$$smtp_auth1
                                                                     },
                                                                   ^QUIT where true:
                                                                     $end;
                                                                 },
                                                               ^SUBJECT where true:
                                                                 2->1:String;
                                                                 2->1:String;
                                                                 ~$$smtp_data1
                                                             }(1 of 1, 2) ~this92,
                                                             String ~subject93,
                                                             String ~data94)
                                      ( ~this92[2] >>
                                        { ^DATALINE:
                                          ~this92[2] >> ~line1107;
                                          ~this92[2] >> ~line2108;
                                          ~SmtpServerData8(~this92, ~subject93, ~line1107);,
                                          ^ENDOFDATA:
                                          ~c109= new ~console4(2 of 2);
                                          ~c109[1] << ^str;
                                          ~c109[1] << "Server received mail";
                                          ~c109[1] << ^nl;
                                          ~c109[1] << ^str;
                                          ~c109[1] << "FROM: ";
                                          ~c109[1] << ^str;
                                          ~c109[1] << ~from91;
                                          ~c109[1] << ^nl;
                                          ~c109[1] << ^str;
                                          ~c109[1] << "RCPT: ";
                                          ~c109[1] << ^str;
                                          ~c109[1] << ~rcpt74;
                                          ~c109[1] << ^nl;
                                          ~c109[1] << ^str;
                                          ~c109[1] << "SUBJ: ";
                                          ~c109[1] << ^str;
                                          ~c109[1] << ~subject93;
                                          ~c109[1] << ^nl;
                                          ~c109[1] << ^end;
                                          ~SmtpServerAuth4(~this92);,
                                          ^SUBJECT:
                                          ~this92[2] >> ~s1110;
                                          ~this92[2] >> ~s2111;
                                          ~SmtpServerData8(~this92, ~s1110, ~data94);
                                        }
                                      )
                                      ~SmtpServerData8(~this73, "", "");,
                                      ^RCPT:
                                      ~this73[2] >> ~r112;
                                      ~this73[2] << ^250;
                                      ~SmtpServerMail7(~this73, ~r112);
                                    }
                                  )
                                  ~SmtpServerMail7(~this20, "");
                             else ~this20[2] << ^501;
                                  ~SmtpServerAuth4(~this20);,
                        ^QUIT:
                        
                      }
                    )
                    ~SmtpServerAuth4(~this18);,
                    ^QUIT:
                    
                  },
                  ^QUIT:
                  
                }
              },
              ^QUIT:
              
            }
          )
          ~FlatRight12();
        )
      )
      local ~StartService9(Int ~i113)
        ( if (~i113 <= 0)
          then ~SmtpServer3();
          else ( ~SmtpServer3();
               | ~StartService9((~i113 - 1));
               )
        )
        ~StartService9(SYSTEM & "tprocs");
    | local ~FlatRight13()
      ( ~client114= new ~smtp17(2 of 2);
        ~client114[1] >>
        { ^220_SERVICE_READY:
          ~client114[1] >> ~msg115;
          ~client114[1] << ^EHLO;
          ~client114[1] << "user";
          local ~SmtpClientEhlo10(rec ~$$smtp_ehlo1;
                                  1->2
                                  { ^250 where true:
                                      1->2:String;
                                      2->1
                                      { ^QUIT where true:
                                          $end;,
                                        ^StartTLS where true:
                                          1->2
                                          { ^220 where true:
                                              2->1
                                              { ^EHLO where true:
                                                  rec ~$$smtp_tls1;
                                                  1->2
                                                  { ^250 where true:
                                                      1->2:String;
                                                      rec ~$$smtp_cmd1;
                                                      2->1
                                                      { ^AUTH where true:
                                                          1->2
                                                          { ^235 where true:
                                                              rec ~$$smtp_auth1;
                                                              2->1
                                                              { ^MAIL where true:
                                                                  2->1:String;
                                                                  1->2
                                                                  { ^250 where true:
                                                                      rec ~$$smtp_mail1;
                                                                      2->1
                                                                      { ^DATA where true:
                                                                          1->2
                                                                          { ^354 where true:
                                                                              2->1:String;
                                                                              2->1:String;
                                                                              rec ~$$smtp_data1;
                                                                              2->1
                                                                              { ^DATALINE where true:
                                                                                  2->1:String;
                                                                                  2->1:String;
                                                                                  ~$$smtp_data1,
                                                                                ^ENDOFDATA where true:
                                                                                  ~$$smtp_auth1,
                                                                                ^SUBJECT where true:
                                                                                  2->1:String;
                                                                                  2->1:String;
                                                                                  ~$$smtp_data1
                                                                              }
                                                                          },
                                                                        ^RCPT where true:
                                                                          2->1:String;
                                                                          1->2
                                                                          { ^250 where true:
                                                                              ~$$smtp_mail1
                                                                          }
                                                                      },
                                                                    ^501 where true:
                                                                      ~$$smtp_auth1
                                                                  },
                                                                ^QUIT where true:
                                                                  $end;
                                                              },
                                                            ^535 where true:
                                                              ~$$smtp_cmd1
                                                          },
                                                        ^QUIT where true:
                                                          $end;
                                                      },
                                                    ^250d where true:
                                                      ~$$smtp_tls1
                                                  },
                                                ^QUIT where true:
                                                  $end;
                                              }
                                          }
                                      },
                                    ^250d where true:
                                      ~$$smtp_ehlo1
                                  }(2 of 1, 2) ~this116)
          ( ~this116[1] >>
            { ^250:
              ~this116[1] >> ~srv127;
              ~this116[1] << ^StartTLS;
              ~this116[1] >>
              { ^220:
                ~this116[1] << ^EHLO;
                local ~SmtpClientTls11(rec ~$$smtp_tls1;
                                       1->2
                                       { ^250 where true:
                                           1->2:String;
                                           rec ~$$smtp_cmd1;
                                           2->1
                                           { ^AUTH where true:
                                               1->2
                                               { ^235 where true:
                                                   rec ~$$smtp_auth1;
                                                   2->1
                                                   { ^MAIL where true:
                                                       2->1:String;
                                                       1->2
                                                       { ^250 where true:
                                                           rec ~$$smtp_mail1;
                                                           2->1
                                                           { ^DATA where true:
                                                               1->2
                                                               { ^354 where true:
                                                                   2->1:String;
                                                                   2->1:String;
                                                                   rec ~$$smtp_data1;
                                                                   2->1
                                                                   { ^DATALINE where true:
                                                                       2->1:String;
                                                                       2->1:String;
                                                                       ~$$smtp_data1,
                                                                     ^ENDOFDATA where true:
                                                                       ~$$smtp_auth1,
                                                                     ^SUBJECT where true:
                                                                       2->1:String;
                                                                       2->1:String;
                                                                       ~$$smtp_data1
                                                                   }
                                                               },
                                                             ^RCPT where true:
                                                               2->1:String;
                                                               1->2
                                                               { ^250 where true:
                                                                   ~$$smtp_mail1
                                                               }
                                                           },
                                                         ^501 where true:
                                                           ~$$smtp_auth1
                                                       },
                                                     ^QUIT where true:
                                                       $end;
                                                   },
                                                 ^535 where true:
                                                   ~$$smtp_cmd1
                                               },
                                             ^QUIT where true:
                                               $end;
                                           },
                                         ^250d where true:
                                           ~$$smtp_tls1
                                       }(2 of 1, 2) ~this128)
                ( ~this128[1] >>
                  { ^250:
                    ~this128[1] >> ~srv138;
                    ~this128[1] << ^AUTH;
                    ~this128[1] >>
                    { ^235:
                      ~this128[1] << ^MAIL;
                      ~this128[1] << "user@smtpserver.pi";
                      ~this128[1] >>
                      { ^250:
                        ~this128[1] << ^RCPT;
                        ~this128[1] << "friend@smtpserver.pi";
                        ~this128[1] >>
                        { ^250:
                          ~this128[1] << ^DATA;
                          ~this128[1] >>
                          { ^354:
                            ~this128[1] << "friend@smtpserver.pi";
                            ~this128[1] << "user@smtpserver.pi";
                            ~this128[1] << ^SUBJECT;
                            ~this128[1] << "SMTP MESSAGE";
                            ~this128[1] << "SUBJECT";
                            ~this128[1] << ^DATALINE;
                            ~this128[1] << "MSG1";
                            ~this128[1] << "MSG2";
                            ~this128[1] << ^ENDOFDATA;
                            ~this128[1] << ^QUIT;
                            
                          }
                        },
                        ^501:
                        ~this128[1] << ^QUIT;
                        
                      },
                      ^535:
                      ~this128[1] << ^QUIT;
                      
                    },
                    ^250d:
                    ~SmtpClientTls11(~this128);
                  }
                )
                ~SmtpClientTls11(~this116);
              },
              ^250d:
              ~SmtpClientEhlo10(~this116);
            }
          )
          ~SmtpClientEhlo10(~client114);
        }
      )
      ~FlatRight13();
    )
  )
  ~FlatRight14();
)
*/
/* ==== CLOSEDEFED ====
HOSTHEADER(""#include <iostream>"");
rec ~$$console1;
2->1
{ ^end where true:
    $end;,
  ^flt where true:
    2->1:Float;
    ~$$console1,
  ^input where true:
    2->1
    { ^int where true:
        1->2:Int;
        ~$$console1,
      ^str where true:
        1->2:String;
        ~$$console1
    },
  ^int where true:
    2->1:Int;
    ~$$console1,
  ^nl where true:
    ~$$console1,
  ^str where true:
    2->1:String;
    ~$$console1
} ~console4(1, 2);
( local ~Console1(rec ~$$console1;
                  2->1
                  { ^end where true:
                      $end;,
                    ^flt where true:
                      2->1:Float;
                      ~$$console1,
                    ^input where true:
                      2->1
                      { ^int where true:
                          1->2:Int;
                          ~$$console1,
                        ^str where true:
                          1->2:String;
                          ~$$console1
                      },
                    ^int where true:
                      2->1:Int;
                      ~$$console1,
                    ^nl where true:
                      ~$$console1,
                    ^str where true:
                      2->1:String;
                      ~$$console1
                  }(1, 2) ~console4)
  ( ~s5= new ~console4(1 of 2);
    local ~ConsoleSession2(rec ~$$console1;
                           2->1
                           { ^end where true:
                               $end;,
                             ^flt where true:
                               2->1:Float;
                               ~$$console1,
                             ^input where true:
                               2->1
                               { ^int where true:
                                   1->2:Int;
                                   ~$$console1,
                                 ^str where true:
                                   1->2:String;
                                   ~$$console1
                               },
                             ^int where true:
                               2->1:Int;
                               ~$$console1,
                             ^nl where true:
                               ~$$console1,
                             ^str where true:
                               2->1:String;
                               ~$$console1
                           }(1 of 1, 2) ~s6,
                           rec ~$$console1;
                           2->1
                           { ^end where true:
                               $end;,
                             ^flt where true:
                               2->1:Float;
                               ~$$console1,
                             ^input where true:
                               2->1
                               { ^int where true:
                                   1->2:Int;
                                   ~$$console1,
                                 ^str where true:
                                   1->2:String;
                                   ~$$console1
                               },
                             ^int where true:
                               2->1:Int;
                               ~$$console1,
                             ^nl where true:
                               ~$$console1,
                             ^str where true:
                               2->1:String;
                               ~$$console1
                           }(1, 2) ~console4)
    ( ~s6[2] >>
      { ^end:
        ~Console1(~console4);,
        ^flt:
        ~s6[2] >> ~f12;
        HOST("std::cout << ", ~f12", "->ToString() << std::flush;");
        ~ConsoleSession2(~s6, ~console4);,
        ^input:
        ~s6[2] >>
        { ^int:
          Int ~val13=0;
          HOST("{ long _l; std::cin >> _l; ", ~val13", "=new libpi::Int(_l); }");
          ~s6[2] << ~val13;
          ~ConsoleSession2(~s6, ~console4);,
          ^str:
          String ~val14="";
          HOST("{ std::string _s; std::cin >> _s; ", ~val14", "=new libpi::String(_s); }");
          ~s6[2] << ~val14;
          ~ConsoleSession2(~s6, ~console4);
        },
        ^int:
        ~s6[2] >> ~i15;
        HOST("std::cout << ", ~i15", "->ToString() << std::flush;");
        ~ConsoleSession2(~s6, ~console4);,
        ^nl:
        HOST("std::cout << std::endl;");
        ~ConsoleSession2(~s6, ~console4);,
        ^str:
        ~s6[2] >> ~str16;
        HOST("std::cout << ", ~str16", "->GetValue() << std::flush;");
        ~ConsoleSession2(~s6, ~console4);
      }
    )
    ~ConsoleSession2(~s5, ~console4);
  )
  ~Console1(~console4);
| local ~FlatRight14(rec ~$$console1;
                     2->1
                     { ^end where true:
                         $end;,
                       ^flt where true:
                         2->1:Float;
                         ~$$console1,
                       ^input where true:
                         2->1
                         { ^int where true:
                             1->2:Int;
                             ~$$console1,
                           ^str where true:
                             1->2:String;
                             ~$$console1
                         },
                       ^int where true:
                         2->1:Int;
                         ~$$console1,
                       ^nl where true:
                         ~$$console1,
                       ^str where true:
                         2->1:String;
                         ~$$console1
                     }(1, 2) ~console4)
  ( 1->2
    { ^220_SERVICE_READY where true:
        1->2:String;
        2->1
        { ^EHLO where true:
            2->1:String;
            rec ~$$smtp_ehlo1;
            1->2
            { ^250 where true:
                1->2:String;
                2->1
                { ^QUIT where true:
                    $end;,
                  ^StartTLS where true:
                    1->2
                    { ^220 where true:
                        2->1
                        { ^EHLO where true:
                            rec ~$$smtp_tls1;
                            1->2
                            { ^250 where true:
                                1->2:String;
                                rec ~$$smtp_cmd1;
                                2->1
                                { ^AUTH where true:
                                    1->2
                                    { ^235 where true:
                                        rec ~$$smtp_auth1;
                                        2->1
                                        { ^MAIL where true:
                                            2->1:String;
                                            1->2
                                            { ^250 where true:
                                                rec ~$$smtp_mail1;
                                                2->1
                                                { ^DATA where true:
                                                    1->2
                                                    { ^354 where true:
                                                        2->1:String;
                                                        2->1:String;
                                                        rec ~$$smtp_data1;
                                                        2->1
                                                        { ^DATALINE where true:
                                                            2->1:String;
                                                            2->1:String;
                                                            ~$$smtp_data1,
                                                          ^ENDOFDATA where true:
                                                            ~$$smtp_auth1,
                                                          ^SUBJECT where true:
                                                            2->1:String;
                                                            2->1:String;
                                                            ~$$smtp_data1
                                                        }
                                                    },
                                                  ^RCPT where true:
                                                    2->1:String;
                                                    1->2
                                                    { ^250 where true:
                                                        ~$$smtp_mail1
                                                    }
                                                },
                                              ^501 where true:
                                                ~$$smtp_auth1
                                            },
                                          ^QUIT where true:
                                            $end;
                                        },
                                      ^535 where true:
                                        ~$$smtp_cmd1
                                    },
                                  ^QUIT where true:
                                    $end;
                                },
                              ^250d where true:
                                ~$$smtp_tls1
                            },
                          ^QUIT where true:
                            $end;
                        }
                    }
                },
              ^250d where true:
                ~$$smtp_ehlo1
            },
          ^QUIT where true:
            $end;
        }
    } ~smtp17(1, 2);
    ( local ~SmtpServer3(rec ~$$console1;
                         2->1
                         { ^end where true:
                             $end;,
                           ^flt where true:
                             2->1:Float;
                             ~$$console1,
                           ^input where true:
                             2->1
                             { ^int where true:
                                 1->2:Int;
                                 ~$$console1,
                               ^str where true:
                                 1->2:String;
                                 ~$$console1
                             },
                           ^int where true:
                             2->1:Int;
                             ~$$console1,
                           ^nl where true:
                             ~$$console1,
                           ^str where true:
                             2->1:String;
                             ~$$console1
                         }(1, 2) ~console4,
                         1->2
                         { ^220_SERVICE_READY where true:
                             1->2:String;
                             2->1
                             { ^EHLO where true:
                                 2->1:String;
                                 rec ~$$smtp_ehlo1;
                                 1->2
                                 { ^250 where true:
                                     1->2:String;
                                     2->1
                                     { ^QUIT where true:
                                         $end;,
                                       ^StartTLS where true:
                                         1->2
                                         { ^220 where true:
                                             2->1
                                             { ^EHLO where true:
                                                 rec ~$$smtp_tls1;
                                                 1->2
                                                 { ^250 where true:
                                                     1->2:String;
                                                     rec ~$$smtp_cmd1;
                                                     2->1
                                                     { ^AUTH where true:
                                                         1->2
                                                         { ^235 where true:
                                                             rec ~$$smtp_auth1;
                                                             2->1
                                                             { ^MAIL where true:
                                                                 2->1:String;
                                                                 1->2
                                                                 { ^250 where true:
                                                                     rec ~$$smtp_mail1;
                                                                     2->1
                                                                     { ^DATA where true:
                                                                         1->2
                                                                         { ^354 where true:
                                                                             2->1:String;
                                                                             2->1:String;
                                                                             rec ~$$smtp_data1;
                                                                             2->1
                                                                             { ^DATALINE where true:
                                                                                 2->1:String;
                                                                                 2->1:String;
                                                                                 ~$$smtp_data1,
                                                                               ^ENDOFDATA where true:
                                                                                 ~$$smtp_auth1,
                                                                               ^SUBJECT where true:
                                                                                 2->1:String;
                                                                                 2->1:String;
                                                                                 ~$$smtp_data1
                                                                             }
                                                                         },
                                                                       ^RCPT where true:
                                                                         2->1:String;
                                                                         1->2
                                                                         { ^250 where true:
                                                                             ~$$smtp_mail1
                                                                         }
                                                                     },
                                                                   ^501 where true:
                                                                     ~$$smtp_auth1
                                                                 },
                                                               ^QUIT where true:
                                                                 $end;
                                                             },
                                                           ^535 where true:
                                                             ~$$smtp_cmd1
                                                         },
                                                       ^QUIT where true:
                                                         $end;
                                                     },
                                                   ^250d where true:
                                                     ~$$smtp_tls1
                                                 },
                                               ^QUIT where true:
                                                 $end;
                                             }
                                         }
                                     },
                                   ^250d where true:
                                     ~$$smtp_ehlo1
                                 },
                               ^QUIT where true:
                                 $end;
                             }
                         }(1, 2) ~smtp17)
      ( ~this18= new ~smtp17(1 of 2);
        ( ~SmtpServer3(~console4, ~smtp17);
        | local ~FlatRight12(rec ~$$console1;
                             2->1
                             { ^end where true:
                                 $end;,
                               ^flt where true:
                                 2->1:Float;
                                 ~$$console1,
                               ^input where true:
                                 2->1
                                 { ^int where true:
                                     1->2:Int;
                                     ~$$console1,
                                   ^str where true:
                                     1->2:String;
                                     ~$$console1
                                 },
                               ^int where true:
                                 2->1:Int;
                                 ~$$console1,
                               ^nl where true:
                                 ~$$console1,
                               ^str where true:
                                 2->1:String;
                                 ~$$console1
                             }(1, 2) ~console4,
                             2<<
                             { ^220_SERVICE_READY where true:
                               2 << String;
                               2>>
                               { ^EHLO where true:
                                 2 >> String;
                                 rec ~%%smtp_ehlo1;
                                 2<<
                                 { ^250 where true:
                                   2 << String;
                                   2>>
                                   { ^QUIT where true:
                                     %end;,
                                     ^StartTLS where true:
                                     2<<
                                     { ^220 where true:
                                       2>>
                                       { ^EHLO where true:
                                         rec ~%%smtp_tls1;
                                         2<<
                                         { ^250 where true:
                                           2 << String;
                                           rec ~%%smtp_cmd1;
                                           2>>
                                           { ^AUTH where true:
                                             2<<
                                             { ^235 where true:
                                               rec ~%%smtp_auth1;
                                               2>>
                                               { ^MAIL where true:
                                                 2 >> String;
                                                 2<<
                                                 { ^250 where true:
                                                   rec ~%%smtp_mail1;
                                                   2>>
                                                   { ^DATA where true:
                                                     2<<
                                                     { ^354 where true:
                                                       2 >> String;
                                                       2 >> String;
                                                       rec ~%%smtp_data1;
                                                       2>>
                                                       { ^DATALINE where true:
                                                         2 >> String;
                                                         2 >> String;
                                                         ~%%smtp_data1,
                                                         ^ENDOFDATA where true:
                                                         ~%%smtp_auth1,
                                                         ^SUBJECT where true:
                                                         2 >> String;
                                                         2 >> String;
                                                         ~%%smtp_data1
                                                       }
                                                     },
                                                     ^RCPT where true:
                                                     2 >> String;
                                                     2<<
                                                     { ^250 where true:
                                                       ~%%smtp_mail1
                                                     }
                                                   },
                                                   ^501 where true:
                                                   ~%%smtp_auth1
                                                 },
                                                 ^QUIT where true:
                                                 %end;
                                               },
                                               ^535 where true:
                                               ~%%smtp_cmd1
                                             },
                                             ^QUIT where true:
                                             %end;
                                           },
                                           ^250d where true:
                                           ~%%smtp_tls1
                                         },
                                         ^QUIT where true:
                                         %end;
                                       }
                                     }
                                   },
                                   ^250d where true:
                                   ~%%smtp_ehlo1
                                 },
                                 ^QUIT where true:
                                 %end;
                               }
                             }(1 of 1, 2) ~this18)
          ( ~this18[2] << ^220_SERVICE_READY;
            ~this18[2] << "Welcome to the SMTP service!";
            ~this18[2] >>
            { ^EHLO:
              ~this18[2] >> ~msg19;
              ~this18[2] << ^250;
              ~this18[2] << "smtpservice.pi";
              ~this18[2] >>
              { ^QUIT:
                ,
                ^StartTLS:
                ~this18[2] << ^220;
                ~this18[2] >>
                { ^EHLO:
                  ~this18[2] << ^250;
                  ~this18[2] << "smtpservice.pi";
                  ~this18[2] >>
                  { ^AUTH:
                    ~this18[2] << ^235;
                    local ~SmtpServerAuth4(rec ~$$smtp_auth1;
                                           2->1
                                           { ^MAIL where true:
                                               2->1:String;
                                               1->2
                                               { ^250 where true:
                                                   rec ~$$smtp_mail1;
                                                   2->1
                                                   { ^DATA where true:
                                                       1->2
                                                       { ^354 where true:
                                                           2->1:String;
                                                           2->1:String;
                                                           rec ~$$smtp_data1;
                                                           2->1
                                                           { ^DATALINE where true:
                                                               2->1:String;
                                                               2->1:String;
                                                               ~$$smtp_data1,
                                                             ^ENDOFDATA where true:
                                                               ~$$smtp_auth1,
                                                             ^SUBJECT where true:
                                                               2->1:String;
                                                               2->1:String;
                                                               ~$$smtp_data1
                                                           }
                                                       },
                                                     ^RCPT where true:
                                                       2->1:String;
                                                       1->2
                                                       { ^250 where true:
                                                           ~$$smtp_mail1
                                                       }
                                                   },
                                                 ^501 where true:
                                                   ~$$smtp_auth1
                                               },
                                             ^QUIT where true:
                                               $end;
                                           }(1 of 1, 2) ~this20,
                                           rec ~$$console1;
                                           2->1
                                           { ^end where true:
                                               $end;,
                                             ^flt where true:
                                               2->1:Float;
                                               ~$$console1,
                                             ^input where true:
                                               2->1
                                               { ^int where true:
                                                   1->2:Int;
                                                   ~$$console1,
                                                 ^str where true:
                                                   1->2:String;
                                                   ~$$console1
                                               },
                                             ^int where true:
                                               2->1:Int;
                                               ~$$console1,
                                             ^nl where true:
                                               ~$$console1,
                                             ^str where true:
                                               2->1:String;
                                               ~$$console1
                                           }(1, 2) ~console4)
                    ( ~this20[2] >>
                      { ^MAIL:
                        if (SYSTEM & "tprocs" <= SYSTEM & "aprocs")
                        then ~this20[2] >> ~from29;
                             Bool ~valid_from30=true;
                             if ~valid_from30
                             then ~this20[2] << ^250;
                                  local ~SmtpServerMail5(rec ~$$smtp_mail1;
                                                         2->1
                                                         { ^DATA where true:
                                                             1->2
                                                             { ^354 where true:
                                                                 2->1:String;
                                                                 2->1:String;
                                                                 rec ~$$smtp_data1;
                                                                 2->1
                                                                 { ^DATALINE where true:
                                                                     2->1:String;
                                                                     2->1:String;
                                                                     ~$$smtp_data1,
                                                                   ^ENDOFDATA where true:
                                                                     rec ~$$smtp_auth1;
                                                                     2->1
                                                                     { ^MAIL where true:
                                                                         2->1:String;
                                                                         1->2
                                                                         { ^250 where true:
                                                                             rec ~$$smtp_mail1;
                                                                             2->1
                                                                             { ^DATA where true:
                                                                                 1->2
                                                                                 { ^354 where true:
                                                                                     2->1:String;
                                                                                     2->1:String;
                                                                                     rec ~$$smtp_data1;
                                                                                     2->1
                                                                                     { ^DATALINE where true:
                                                                                         2->1:String;
                                                                                         2->1:String;
                                                                                         ~$$smtp_data1,
                                                                                       ^ENDOFDATA where true:
                                                                                         ~$$smtp_auth1,
                                                                                       ^SUBJECT where true:
                                                                                         2->1:String;
                                                                                         2->1:String;
                                                                                         ~$$smtp_data1
                                                                                     }
                                                                                 },
                                                                               ^RCPT where true:
                                                                                 2->1:String;
                                                                                 1->2
                                                                                 { ^250 where true:
                                                                                     ~$$smtp_mail1
                                                                                 }
                                                                             },
                                                                           ^501 where true:
                                                                             ~$$smtp_auth1
                                                                         },
                                                                       ^QUIT where true:
                                                                         $end;
                                                                     },
                                                                   ^SUBJECT where true:
                                                                     2->1:String;
                                                                     2->1:String;
                                                                     ~$$smtp_data1
                                                                 }
                                                             },
                                                           ^RCPT where true:
                                                             2->1:String;
                                                             1->2
                                                             { ^250 where true:
                                                                 ~$$smtp_mail1
                                                             }
                                                         }(1 of 1, 2) ~this31,
                                                         String ~rcpt32,
                                                         rec ~$$console1;
                                                         2->1
                                                         { ^end where true:
                                                             $end;,
                                                           ^flt where true:
                                                             2->1:Float;
                                                             ~$$console1,
                                                           ^input where true:
                                                             2->1
                                                             { ^int where true:
                                                                 1->2:Int;
                                                                 ~$$console1,
                                                               ^str where true:
                                                                 1->2:String;
                                                                 ~$$console1
                                                             },
                                                           ^int where true:
                                                             2->1:Int;
                                                             ~$$console1,
                                                           ^nl where true:
                                                             ~$$console1,
                                                           ^str where true:
                                                             2->1:String;
                                                             ~$$console1
                                                         }(1, 2) ~console4)
                                  ( ~this31[2] >>
                                    { ^DATA:
                                      ~this31[2] << ^354;
                                      ~this31[2] >> ~to48;
                                      ~this31[2] >> ~from49;
                                      local ~SmtpServerData6(rec ~$$smtp_data1;
                                                             2->1
                                                             { ^DATALINE where true:
                                                                 2->1:String;
                                                                 2->1:String;
                                                                 ~$$smtp_data1,
                                                               ^ENDOFDATA where true:
                                                                 rec ~$$smtp_auth1;
                                                                 2->1
                                                                 { ^MAIL where true:
                                                                     2->1:String;
                                                                     1->2
                                                                     { ^250 where true:
                                                                         rec ~$$smtp_mail1;
                                                                         2->1
                                                                         { ^DATA where true:
                                                                             1->2
                                                                             { ^354 where true:
                                                                                 2->1:String;
                                                                                 2->1:String;
                                                                                 rec ~$$smtp_data1;
                                                                                 2->1
                                                                                 { ^DATALINE where true:
                                                                                     2->1:String;
                                                                                     2->1:String;
                                                                                     ~$$smtp_data1,
                                                                                   ^ENDOFDATA where true:
                                                                                     ~$$smtp_auth1,
                                                                                   ^SUBJECT where true:
                                                                                     2->1:String;
                                                                                     2->1:String;
                                                                                     ~$$smtp_data1
                                                                                 }
                                                                             },
                                                                           ^RCPT where true:
                                                                             2->1:String;
                                                                             1->2
                                                                             { ^250 where true:
                                                                                 ~$$smtp_mail1
                                                                             }
                                                                         },
                                                                       ^501 where true:
                                                                         ~$$smtp_auth1
                                                                     },
                                                                   ^QUIT where true:
                                                                     $end;
                                                                 },
                                                               ^SUBJECT where true:
                                                                 2->1:String;
                                                                 2->1:String;
                                                                 ~$$smtp_data1
                                                             }(1 of 1, 2) ~this50,
                                                             String ~subject51,
                                                             String ~data52,
                                                             rec ~$$console1;
                                                             2->1
                                                             { ^end where true:
                                                                 $end;,
                                                               ^flt where true:
                                                                 2->1:Float;
                                                                 ~$$console1,
                                                               ^input where true:
                                                                 2->1
                                                                 { ^int where true:
                                                                     1->2:Int;
                                                                     ~$$console1,
                                                                   ^str where true:
                                                                     1->2:String;
                                                                     ~$$console1
                                                                 },
                                                               ^int where true:
                                                                 2->1:Int;
                                                                 ~$$console1,
                                                               ^nl where true:
                                                                 ~$$console1,
                                                               ^str where true:
                                                                 2->1:String;
                                                                 ~$$console1
                                                             }(1, 2) ~console4,
                                                             String ~from49,
                                                             String ~rcpt32)
                                      ( ~this50[2] >>
                                        { ^DATALINE:
                                          ~this50[2] >> ~line165;
                                          ~this50[2] >> ~line266;
                                          ~SmtpServerData6(~this50, ~subject51, ~line165, ~console4, ~from49, ~rcpt32);,
                                          ^ENDOFDATA:
                                          ~c67= new ~console4(2 of 2);
                                          ~c67[1] << ^str;
                                          ~c67[1] << "Server received mail";
                                          ~c67[1] << ^nl;
                                          ~c67[1] << ^str;
                                          ~c67[1] << "FROM: ";
                                          ~c67[1] << ^str;
                                          ~c67[1] << ~from49;
                                          ~c67[1] << ^nl;
                                          ~c67[1] << ^str;
                                          ~c67[1] << "RCPT: ";
                                          ~c67[1] << ^str;
                                          ~c67[1] << ~rcpt32;
                                          ~c67[1] << ^nl;
                                          ~c67[1] << ^str;
                                          ~c67[1] << "SUBJ: ";
                                          ~c67[1] << ^str;
                                          ~c67[1] << ~subject51;
                                          ~c67[1] << ^nl;
                                          ~c67[1] << ^end;
                                          ~SmtpServerAuth4(~this50, ~console4);,
                                          ^SUBJECT:
                                          ~this50[2] >> ~s168;
                                          ~this50[2] >> ~s269;
                                          ~SmtpServerData6(~this50, ~s168, ~data52, ~console4, ~from49, ~rcpt32);
                                        }
                                      )
                                      ~SmtpServerData6(~this31, "", "", ~console4, ~from49, ~rcpt32);,
                                      ^RCPT:
                                      ~this31[2] >> ~r70;
                                      ~this31[2] << ^250;
                                      ~SmtpServerMail5(~this31, ~r70, ~console4);
                                    }
                                  )
                                  ~SmtpServerMail5(~this20, "", ~console4);
                             else ~this20[2] << ^501;
                                  ~SmtpServerAuth4(~this20, ~console4);
                        else Bool ~valid_from71=true;
                             ~this20[2] >> ~from72;
                             if ~valid_from71
                             then ~this20[2] << ^250;
                                  local ~SmtpServerMail7(rec ~$$smtp_mail1;
                                                         2->1
                                                         { ^DATA where true:
                                                             1->2
                                                             { ^354 where true:
                                                                 2->1:String;
                                                                 2->1:String;
                                                                 rec ~$$smtp_data1;
                                                                 2->1
                                                                 { ^DATALINE where true:
                                                                     2->1:String;
                                                                     2->1:String;
                                                                     ~$$smtp_data1,
                                                                   ^ENDOFDATA where true:
                                                                     rec ~$$smtp_auth1;
                                                                     2->1
                                                                     { ^MAIL where true:
                                                                         2->1:String;
                                                                         1->2
                                                                         { ^250 where true:
                                                                             rec ~$$smtp_mail1;
                                                                             2->1
                                                                             { ^DATA where true:
                                                                                 1->2
                                                                                 { ^354 where true:
                                                                                     2->1:String;
                                                                                     2->1:String;
                                                                                     rec ~$$smtp_data1;
                                                                                     2->1
                                                                                     { ^DATALINE where true:
                                                                                         2->1:String;
                                                                                         2->1:String;
                                                                                         ~$$smtp_data1,
                                                                                       ^ENDOFDATA where true:
                                                                                         ~$$smtp_auth1,
                                                                                       ^SUBJECT where true:
                                                                                         2->1:String;
                                                                                         2->1:String;
                                                                                         ~$$smtp_data1
                                                                                     }
                                                                                 },
                                                                               ^RCPT where true:
                                                                                 2->1:String;
                                                                                 1->2
                                                                                 { ^250 where true:
                                                                                     ~$$smtp_mail1
                                                                                 }
                                                                             },
                                                                           ^501 where true:
                                                                             ~$$smtp_auth1
                                                                         },
                                                                       ^QUIT where true:
                                                                         $end;
                                                                     },
                                                                   ^SUBJECT where true:
                                                                     2->1:String;
                                                                     2->1:String;
                                                                     ~$$smtp_data1
                                                                 }
                                                             },
                                                           ^RCPT where true:
                                                             2->1:String;
                                                             1->2
                                                             { ^250 where true:
                                                                 ~$$smtp_mail1
                                                             }
                                                         }(1 of 1, 2) ~this73,
                                                         String ~rcpt74,
                                                         rec ~$$console1;
                                                         2->1
                                                         { ^end where true:
                                                             $end;,
                                                           ^flt where true:
                                                             2->1:Float;
                                                             ~$$console1,
                                                           ^input where true:
                                                             2->1
                                                             { ^int where true:
                                                                 1->2:Int;
                                                                 ~$$console1,
                                                               ^str where true:
                                                                 1->2:String;
                                                                 ~$$console1
                                                             },
                                                           ^int where true:
                                                             2->1:Int;
                                                             ~$$console1,
                                                           ^nl where true:
                                                             ~$$console1,
                                                           ^str where true:
                                                             2->1:String;
                                                             ~$$console1
                                                         }(1, 2) ~console4)
                                  ( ~this73[2] >>
                                    { ^DATA:
                                      ~this73[2] << ^354;
                                      ~this73[2] >> ~to90;
                                      ~this73[2] >> ~from91;
                                      local ~SmtpServerData8(rec ~$$smtp_data1;
                                                             2->1
                                                             { ^DATALINE where true:
                                                                 2->1:String;
                                                                 2->1:String;
                                                                 ~$$smtp_data1,
                                                               ^ENDOFDATA where true:
                                                                 rec ~$$smtp_auth1;
                                                                 2->1
                                                                 { ^MAIL where true:
                                                                     2->1:String;
                                                                     1->2
                                                                     { ^250 where true:
                                                                         rec ~$$smtp_mail1;
                                                                         2->1
                                                                         { ^DATA where true:
                                                                             1->2
                                                                             { ^354 where true:
                                                                                 2->1:String;
                                                                                 2->1:String;
                                                                                 rec ~$$smtp_data1;
                                                                                 2->1
                                                                                 { ^DATALINE where true:
                                                                                     2->1:String;
                                                                                     2->1:String;
                                                                                     ~$$smtp_data1,
                                                                                   ^ENDOFDATA where true:
                                                                                     ~$$smtp_auth1,
                                                                                   ^SUBJECT where true:
                                                                                     2->1:String;
                                                                                     2->1:String;
                                                                                     ~$$smtp_data1
                                                                                 }
                                                                             },
                                                                           ^RCPT where true:
                                                                             2->1:String;
                                                                             1->2
                                                                             { ^250 where true:
                                                                                 ~$$smtp_mail1
                                                                             }
                                                                         },
                                                                       ^501 where true:
                                                                         ~$$smtp_auth1
                                                                     },
                                                                   ^QUIT where true:
                                                                     $end;
                                                                 },
                                                               ^SUBJECT where true:
                                                                 2->1:String;
                                                                 2->1:String;
                                                                 ~$$smtp_data1
                                                             }(1 of 1, 2) ~this92,
                                                             String ~subject93,
                                                             String ~data94,
                                                             rec ~$$console1;
                                                             2->1
                                                             { ^end where true:
                                                                 $end;,
                                                               ^flt where true:
                                                                 2->1:Float;
                                                                 ~$$console1,
                                                               ^input where true:
                                                                 2->1
                                                                 { ^int where true:
                                                                     1->2:Int;
                                                                     ~$$console1,
                                                                   ^str where true:
                                                                     1->2:String;
                                                                     ~$$console1
                                                                 },
                                                               ^int where true:
                                                                 2->1:Int;
                                                                 ~$$console1,
                                                               ^nl where true:
                                                                 ~$$console1,
                                                               ^str where true:
                                                                 2->1:String;
                                                                 ~$$console1
                                                             }(1, 2) ~console4,
                                                             String ~from91,
                                                             String ~rcpt74)
                                      ( ~this92[2] >>
                                        { ^DATALINE:
                                          ~this92[2] >> ~line1107;
                                          ~this92[2] >> ~line2108;
                                          ~SmtpServerData8(~this92, ~subject93, ~line1107, ~console4, ~from91, ~rcpt74);,
                                          ^ENDOFDATA:
                                          ~c109= new ~console4(2 of 2);
                                          ~c109[1] << ^str;
                                          ~c109[1] << "Server received mail";
                                          ~c109[1] << ^nl;
                                          ~c109[1] << ^str;
                                          ~c109[1] << "FROM: ";
                                          ~c109[1] << ^str;
                                          ~c109[1] << ~from91;
                                          ~c109[1] << ^nl;
                                          ~c109[1] << ^str;
                                          ~c109[1] << "RCPT: ";
                                          ~c109[1] << ^str;
                                          ~c109[1] << ~rcpt74;
                                          ~c109[1] << ^nl;
                                          ~c109[1] << ^str;
                                          ~c109[1] << "SUBJ: ";
                                          ~c109[1] << ^str;
                                          ~c109[1] << ~subject93;
                                          ~c109[1] << ^nl;
                                          ~c109[1] << ^end;
                                          ~SmtpServerAuth4(~this92, ~console4);,
                                          ^SUBJECT:
                                          ~this92[2] >> ~s1110;
                                          ~this92[2] >> ~s2111;
                                          ~SmtpServerData8(~this92, ~s1110, ~data94, ~console4, ~from91, ~rcpt74);
                                        }
                                      )
                                      ~SmtpServerData8(~this73, "", "", ~console4, ~from91, ~rcpt74);,
                                      ^RCPT:
                                      ~this73[2] >> ~r112;
                                      ~this73[2] << ^250;
                                      ~SmtpServerMail7(~this73, ~r112, ~console4);
                                    }
                                  )
                                  ~SmtpServerMail7(~this20, "", ~console4);
                             else ~this20[2] << ^501;
                                  ~SmtpServerAuth4(~this20, ~console4);,
                        ^QUIT:
                        
                      }
                    )
                    ~SmtpServerAuth4(~this18, ~console4);,
                    ^QUIT:
                    
                  },
                  ^QUIT:
                  
                }
              },
              ^QUIT:
              
            }
          )
          ~FlatRight12(~console4, ~this18);
        )
      )
      local ~StartService9(Int ~i113,
                             rec ~$$console1;
                             2->1
                             { ^end where true:
                                 $end;,
                               ^flt where true:
                                 2->1:Float;
                                 ~$$console1,
                               ^input where true:
                                 2->1
                                 { ^int where true:
                                     1->2:Int;
                                     ~$$console1,
                                   ^str where true:
                                     1->2:String;
                                     ~$$console1
                                 },
                               ^int where true:
                                 2->1:Int;
                                 ~$$console1,
                               ^nl where true:
                                 ~$$console1,
                               ^str where true:
                                 2->1:String;
                                 ~$$console1
                             }(1, 2) ~console4,
                             1->2
                             { ^220_SERVICE_READY where true:
                                 1->2:String;
                                 2->1
                                 { ^EHLO where true:
                                     2->1:String;
                                     rec ~$$smtp_ehlo1;
                                     1->2
                                     { ^250 where true:
                                         1->2:String;
                                         2->1
                                         { ^QUIT where true:
                                             $end;,
                                           ^StartTLS where true:
                                             1->2
                                             { ^220 where true:
                                                 2->1
                                                 { ^EHLO where true:
                                                     rec ~$$smtp_tls1;
                                                     1->2
                                                     { ^250 where true:
                                                         1->2:String;
                                                         rec ~$$smtp_cmd1;
                                                         2->1
                                                         { ^AUTH where true:
                                                             1->2
                                                             { ^235 where true:
                                                                 rec ~$$smtp_auth1;
                                                                 2->1
                                                                 { ^MAIL where true:
                                                                     2->1:String;
                                                                     1->2
                                                                     { ^250 where true:
                                                                         rec ~$$smtp_mail1;
                                                                         2->1
                                                                         { ^DATA where true:
                                                                             1->2
                                                                             { ^354 where true:
                                                                                 2->1:String;
                                                                                 2->1:String;
                                                                                 rec ~$$smtp_data1;
                                                                                 2->1
                                                                                 { ^DATALINE where true:
                                                                                     2->1:String;
                                                                                     2->1:String;
                                                                                     ~$$smtp_data1,
                                                                                   ^ENDOFDATA where true:
                                                                                     ~$$smtp_auth1,
                                                                                   ^SUBJECT where true:
                                                                                     2->1:String;
                                                                                     2->1:String;
                                                                                     ~$$smtp_data1
                                                                                 }
                                                                             },
                                                                           ^RCPT where true:
                                                                             2->1:String;
                                                                             1->2
                                                                             { ^250 where true:
                                                                                 ~$$smtp_mail1
                                                                             }
                                                                         },
                                                                       ^501 where true:
                                                                         ~$$smtp_auth1
                                                                     },
                                                                   ^QUIT where true:
                                                                     $end;
                                                                 },
                                                               ^535 where true:
                                                                 ~$$smtp_cmd1
                                                             },
                                                           ^QUIT where true:
                                                             $end;
                                                         },
                                                       ^250d where true:
                                                         ~$$smtp_tls1
                                                     },
                                                   ^QUIT where true:
                                                     $end;
                                                 }
                                             }
                                         },
                                       ^250d where true:
                                         ~$$smtp_ehlo1
                                     },
                                   ^QUIT where true:
                                     $end;
                                 }
                             }(1, 2) ~smtp17)
        ( if (~i113 <= 0)
          then ~SmtpServer3(~console4, ~smtp17);
          else ( ~SmtpServer3(~console4, ~smtp17);
               | ~StartService9((~i113 - 1), ~console4, ~smtp17);
               )
        )
        ~StartService9(SYSTEM & "tprocs", ~console4, ~smtp17);
    | local ~FlatRight13(1->2
                         { ^220_SERVICE_READY where true:
                             1->2:String;
                             2->1
                             { ^EHLO where true:
                                 2->1:String;
                                 rec ~$$smtp_ehlo1;
                                 1->2
                                 { ^250 where true:
                                     1->2:String;
                                     2->1
                                     { ^QUIT where true:
                                         $end;,
                                       ^StartTLS where true:
                                         1->2
                                         { ^220 where true:
                                             2->1
                                             { ^EHLO where true:
                                                 rec ~$$smtp_tls1;
                                                 1->2
                                                 { ^250 where true:
                                                     1->2:String;
                                                     rec ~$$smtp_cmd1;
                                                     2->1
                                                     { ^AUTH where true:
                                                         1->2
                                                         { ^235 where true:
                                                             rec ~$$smtp_auth1;
                                                             2->1
                                                             { ^MAIL where true:
                                                                 2->1:String;
                                                                 1->2
                                                                 { ^250 where true:
                                                                     rec ~$$smtp_mail1;
                                                                     2->1
                                                                     { ^DATA where true:
                                                                         1->2
                                                                         { ^354 where true:
                                                                             2->1:String;
                                                                             2->1:String;
                                                                             rec ~$$smtp_data1;
                                                                             2->1
                                                                             { ^DATALINE where true:
                                                                                 2->1:String;
                                                                                 2->1:String;
                                                                                 ~$$smtp_data1,
                                                                               ^ENDOFDATA where true:
                                                                                 ~$$smtp_auth1,
                                                                               ^SUBJECT where true:
                                                                                 2->1:String;
                                                                                 2->1:String;
                                                                                 ~$$smtp_data1
                                                                             }
                                                                         },
                                                                       ^RCPT where true:
                                                                         2->1:String;
                                                                         1->2
                                                                         { ^250 where true:
                                                                             ~$$smtp_mail1
                                                                         }
                                                                     },
                                                                   ^501 where true:
                                                                     ~$$smtp_auth1
                                                                 },
                                                               ^QUIT where true:
                                                                 $end;
                                                             },
                                                           ^535 where true:
                                                             ~$$smtp_cmd1
                                                         },
                                                       ^QUIT where true:
                                                         $end;
                                                     },
                                                   ^250d where true:
                                                     ~$$smtp_tls1
                                                 },
                                               ^QUIT where true:
                                                 $end;
                                             }
                                         }
                                     },
                                   ^250d where true:
                                     ~$$smtp_ehlo1
                                 },
                               ^QUIT where true:
                                 $end;
                             }
                         }(1, 2) ~smtp17)
      ( ~client114= new ~smtp17(2 of 2);
        ~client114[1] >>
        { ^220_SERVICE_READY:
          ~client114[1] >> ~msg115;
          ~client114[1] << ^EHLO;
          ~client114[1] << "user";
          local ~SmtpClientEhlo10(rec ~$$smtp_ehlo1;
                                  1->2
                                  { ^250 where true:
                                      1->2:String;
                                      2->1
                                      { ^QUIT where true:
                                          $end;,
                                        ^StartTLS where true:
                                          1->2
                                          { ^220 where true:
                                              2->1
                                              { ^EHLO where true:
                                                  rec ~$$smtp_tls1;
                                                  1->2
                                                  { ^250 where true:
                                                      1->2:String;
                                                      rec ~$$smtp_cmd1;
                                                      2->1
                                                      { ^AUTH where true:
                                                          1->2
                                                          { ^235 where true:
                                                              rec ~$$smtp_auth1;
                                                              2->1
                                                              { ^MAIL where true:
                                                                  2->1:String;
                                                                  1->2
                                                                  { ^250 where true:
                                                                      rec ~$$smtp_mail1;
                                                                      2->1
                                                                      { ^DATA where true:
                                                                          1->2
                                                                          { ^354 where true:
                                                                              2->1:String;
                                                                              2->1:String;
                                                                              rec ~$$smtp_data1;
                                                                              2->1
                                                                              { ^DATALINE where true:
                                                                                  2->1:String;
                                                                                  2->1:String;
                                                                                  ~$$smtp_data1,
                                                                                ^ENDOFDATA where true:
                                                                                  ~$$smtp_auth1,
                                                                                ^SUBJECT where true:
                                                                                  2->1:String;
                                                                                  2->1:String;
                                                                                  ~$$smtp_data1
                                                                              }
                                                                          },
                                                                        ^RCPT where true:
                                                                          2->1:String;
                                                                          1->2
                                                                          { ^250 where true:
                                                                              ~$$smtp_mail1
                                                                          }
                                                                      },
                                                                    ^501 where true:
                                                                      ~$$smtp_auth1
                                                                  },
                                                                ^QUIT where true:
                                                                  $end;
                                                              },
                                                            ^535 where true:
                                                              ~$$smtp_cmd1
                                                          },
                                                        ^QUIT where true:
                                                          $end;
                                                      },
                                                    ^250d where true:
                                                      ~$$smtp_tls1
                                                  },
                                                ^QUIT where true:
                                                  $end;
                                              }
                                          }
                                      },
                                    ^250d where true:
                                      ~$$smtp_ehlo1
                                  }(2 of 1, 2) ~this116)
          ( ~this116[1] >>
            { ^250:
              ~this116[1] >> ~srv127;
              ~this116[1] << ^StartTLS;
              ~this116[1] >>
              { ^220:
                ~this116[1] << ^EHLO;
                local ~SmtpClientTls11(rec ~$$smtp_tls1;
                                       1->2
                                       { ^250 where true:
                                           1->2:String;
                                           rec ~$$smtp_cmd1;
                                           2->1
                                           { ^AUTH where true:
                                               1->2
                                               { ^235 where true:
                                                   rec ~$$smtp_auth1;
                                                   2->1
                                                   { ^MAIL where true:
                                                       2->1:String;
                                                       1->2
                                                       { ^250 where true:
                                                           rec ~$$smtp_mail1;
                                                           2->1
                                                           { ^DATA where true:
                                                               1->2
                                                               { ^354 where true:
                                                                   2->1:String;
                                                                   2->1:String;
                                                                   rec ~$$smtp_data1;
                                                                   2->1
                                                                   { ^DATALINE where true:
                                                                       2->1:String;
                                                                       2->1:String;
                                                                       ~$$smtp_data1,
                                                                     ^ENDOFDATA where true:
                                                                       ~$$smtp_auth1,
                                                                     ^SUBJECT where true:
                                                                       2->1:String;
                                                                       2->1:String;
                                                                       ~$$smtp_data1
                                                                   }
                                                               },
                                                             ^RCPT where true:
                                                               2->1:String;
                                                               1->2
                                                               { ^250 where true:
                                                                   ~$$smtp_mail1
                                                               }
                                                           },
                                                         ^501 where true:
                                                           ~$$smtp_auth1
                                                       },
                                                     ^QUIT where true:
                                                       $end;
                                                   },
                                                 ^535 where true:
                                                   ~$$smtp_cmd1
                                               },
                                             ^QUIT where true:
                                               $end;
                                           },
                                         ^250d where true:
                                           ~$$smtp_tls1
                                       }(2 of 1, 2) ~this128)
                ( ~this128[1] >>
                  { ^250:
                    ~this128[1] >> ~srv138;
                    ~this128[1] << ^AUTH;
                    ~this128[1] >>
                    { ^235:
                      ~this128[1] << ^MAIL;
                      ~this128[1] << "user@smtpserver.pi";
                      ~this128[1] >>
                      { ^250:
                        ~this128[1] << ^RCPT;
                        ~this128[1] << "friend@smtpserver.pi";
                        ~this128[1] >>
                        { ^250:
                          ~this128[1] << ^DATA;
                          ~this128[1] >>
                          { ^354:
                            ~this128[1] << "friend@smtpserver.pi";
                            ~this128[1] << "user@smtpserver.pi";
                            ~this128[1] << ^SUBJECT;
                            ~this128[1] << "SMTP MESSAGE";
                            ~this128[1] << "SUBJECT";
                            ~this128[1] << ^DATALINE;
                            ~this128[1] << "MSG1";
                            ~this128[1] << "MSG2";
                            ~this128[1] << ^ENDOFDATA;
                            ~this128[1] << ^QUIT;
                            
                          }
                        },
                        ^501:
                        ~this128[1] << ^QUIT;
                        
                      },
                      ^535:
                      ~this128[1] << ^QUIT;
                      
                    },
                    ^250d:
                    ~SmtpClientTls11(~this128);
                  }
                )
                ~SmtpClientTls11(~this116);
              },
              ^250d:
              ~SmtpClientEhlo10(~this116);
            }
          )
          ~SmtpClientEhlo10(~client114);
        }
      )
      ~FlatRight13(~smtp17);
    )
  )
  ~FlatRight14(~console4);
)
*/
#include <libpi/value.hpp>
#include <libpi/bool.hpp>
#include <libpi/int.hpp>
#include <libpi/float.hpp>
#include <libpi/string.hpp>
#include <libpi/session.hpp>
#include <libpi/task/link.hpp>
#include <libpi/task/worker.hpp>
#include <thread>
#include <signal.h>
using namespace std;
// HEADERS {{{
#include <iostream>
// }}}
// Value declerations {{{
std::vector<char*> _args;
libpi::Int* intval_0(new libpi::Int("0"));
libpi::String* stringval_d41d8cd98f00b204e9800998ecf8427e(new libpi::String(""));
libpi::String* stringval_9b054bbaf1ab61f371094ff82128f709(new libpi::String("Server received mail"));
libpi::String* stringval_d362dccf254336f6e31e2b35a396112e(new libpi::String("FROM: "));
libpi::String* stringval_130365dbea325230ace43dbe736045bf(new libpi::String("RCPT: "));
libpi::String* stringval_2f70e8bfd1389315a838ac269d90cb6e(new libpi::String("SUBJ: "));
libpi::String* label_186e35ef3dd9939615c442c30b801a33(new libpi::String("^end"));
libpi::String* label_7a11449475519daf7b8b8621e5b6eacc(new libpi::String("^nl"));
libpi::String* label_cf6b4c5b94c4f75325166dbaeb4a0aae(new libpi::String("^str"));
libpi::String* label_a4850a01ffe59cb572e05301fd3a503f(new libpi::String("^354"));
libpi::String* label_de62ae540e604d3ec7777b9c1adb33e6(new libpi::String("^250"));
libpi::String* label_bb0fe4de77e646ccff635026ab590e70(new libpi::String("^501"));
libpi::String* stringval_1d32166a901c92f5595c02ca74f9a49a(new libpi::String("Welcome to the SMTP service!"));
libpi::String* stringval_3adb336c0fd73a169b4af87a5cc99fa2(new libpi::String("smtpservice.pi"));
libpi::String* label_22b095d07ef97cc2cea12609d3bba397(new libpi::String("^235"));
libpi::String* label_321f0bf458ee529a49c77f94eab195af(new libpi::String("^220"));
libpi::String* label_e126287453c75224938478719b76cbdf(new libpi::String("^220_SERVICE_READY"));
libpi::Int* intval_1(new libpi::Int("1"));
libpi::String* stringval_42e6fae527820c9a8ce6bf7a97074aac(new libpi::String("user@smtpserver.pi"));
libpi::String* stringval_65a4ce8d05829010bc25aa9d89848b1d(new libpi::String("friend@smtpserver.pi"));
libpi::String* stringval_dad8bb550572b09371bdc9090db36936(new libpi::String("SMTP MESSAGE"));
libpi::String* stringval_171cb8f32b5bd0a653aec8cc647f17ae(new libpi::String("SUBJECT"));
libpi::String* stringval_e958e3c688dba52947168cbb0427c811(new libpi::String("MSG1"));
libpi::String* stringval_f95989b6ff49aeb3a863b60a899c044a(new libpi::String("MSG2"));
libpi::String* label_57966ff4d7ad77f4146b41d2cca13560(new libpi::String("^QUIT"));
libpi::String* label_3c2f33dd52c9e502985e3de0512b4271(new libpi::String("^ENDOFDATA"));
libpi::String* label_6ef9720fb4ee8ea4de7074ca3243db4a(new libpi::String("^DATALINE"));
libpi::String* label_628f2bd31290e9321620273d43d55cc8(new libpi::String("^SUBJECT"));
libpi::String* label_00e32b60a0f1155fe19e23d5a7b1247c(new libpi::String("^DATA"));
libpi::String* label_a614911eac73e47509fdabbe963d06c9(new libpi::String("^RCPT"));
libpi::String* label_fc80f3e0cea28c4c6e7d5690125a7c85(new libpi::String("^MAIL"));
libpi::String* label_969550f1d743109cb4f91353e73192be(new libpi::String("^AUTH"));
libpi::String* label_8a20c5f0d5bf4382d3c938deba4f10c3(new libpi::String("^EHLO"));
libpi::String* label_d2f558e10d91cc53c5e7aac2c158428f(new libpi::String("^StartTLS"));
libpi::String* stringval_ee11cbb19052e40b07aac0ca060c23ee(new libpi::String("user"));
// }}}
// Task Types {{{
class TaskMain : public libpi::task::Task
{ public:
    ~TaskMain()
    {
      if ( var___SIM__console4)
      { RemoveRef(var___SIM__console4);
        var___SIM__console4=NULL;
      }
    }
    libpi::task::Link* var___SIM__console4;
};
class Task___SIM__ConsoleSession2 : public libpi::task::Task
{ public:
    Task___SIM__ConsoleSession2()
    { 
      var___SIM__console4=NULL;
      var___SIM__f12=NULL;
      var___SIM__i15=NULL;
      var___SIM__s6=NULL;
      var___SIM__str16=NULL;
      var___SIM__val13=NULL;
      var___SIM__val14=NULL;
    }
    virtual ~Task___SIM__ConsoleSession2()
    {
      if (var___SIM__console4)
      { RemoveRef(var___SIM__console4);
        var___SIM__console4=NULL;
      }
      if (var___SIM__f12)
      { RemoveRef(var___SIM__f12);
        var___SIM__f12=NULL;
      }
      if (var___SIM__i15)
      { RemoveRef(var___SIM__i15);
        var___SIM__i15=NULL;
      }
      if (var___SIM__s6)
      { RemoveRef(var___SIM__s6);
        var___SIM__s6=NULL;
      }
      if (var___SIM__str16)
      { RemoveRef(var___SIM__str16);
        var___SIM__str16=NULL;
      }
      if (var___SIM__val13)
      { RemoveRef(var___SIM__val13);
        var___SIM__val13=NULL;
      }
      if (var___SIM__val14)
      { RemoveRef(var___SIM__val14);
        var___SIM__val14=NULL;
      }
    }
    libpi::task::Link* var___SIM__console4;
    libpi::Float* var___SIM__f12;
    libpi::Int* var___SIM__i15;
    libpi::Session* var___SIM__s6;
    libpi::String* var___SIM__str16;
    libpi::Int* var___SIM__val13;
    libpi::String* var___SIM__val14;
    inline void SetArg0(libpi::Session* val)
    {
      val->AddRef();
      if (var___SIM__s6)
        RemoveRef(var___SIM__s6);
      var___SIM__s6=val; 
    }
    inline void SetArg1(libpi::task::Link* val)
    {
      val->AddRef();
      if (var___SIM__console4)
        RemoveRef(var___SIM__console4);
      var___SIM__console4=val; 
    }
};
class Task___SIM__Console1 : public libpi::task::Task
{ public:
    Task___SIM__Console1()
    { 
      var___SIM__console4=NULL;
      var___SIM__s5=NULL;
    }
    virtual ~Task___SIM__Console1()
    {
      if (var___SIM__console4)
      { RemoveRef(var___SIM__console4);
        var___SIM__console4=NULL;
      }
      if (var___SIM__s5)
      { RemoveRef(var___SIM__s5);
        var___SIM__s5=NULL;
      }
    }
    libpi::task::Link* var___SIM__console4;
    libpi::Session* var___SIM__s5;
    inline void SetArg0(libpi::task::Link* val)
    {
      val->AddRef();
      if (var___SIM__console4)
        RemoveRef(var___SIM__console4);
      var___SIM__console4=val; 
    }
};
class Task___SIM__SmtpServerData6 : public libpi::task::Task
{ public:
    Task___SIM__SmtpServerData6()
    { 
      var___SIM__c67=NULL;
      var___SIM__console4=NULL;
      var___SIM__data52=NULL;
      var___SIM__from49=NULL;
      var___SIM__line165=NULL;
      var___SIM__line266=NULL;
      var___SIM__rcpt32=NULL;
      var___SIM__s168=NULL;
      var___SIM__s269=NULL;
      var___SIM__subject51=NULL;
      var___SIM__this50=NULL;
    }
    virtual ~Task___SIM__SmtpServerData6()
    {
      if (var___SIM__c67)
      { RemoveRef(var___SIM__c67);
        var___SIM__c67=NULL;
      }
      if (var___SIM__console4)
      { RemoveRef(var___SIM__console4);
        var___SIM__console4=NULL;
      }
      if (var___SIM__data52)
      { RemoveRef(var___SIM__data52);
        var___SIM__data52=NULL;
      }
      if (var___SIM__from49)
      { RemoveRef(var___SIM__from49);
        var___SIM__from49=NULL;
      }
      if (var___SIM__line165)
      { RemoveRef(var___SIM__line165);
        var___SIM__line165=NULL;
      }
      if (var___SIM__line266)
      { RemoveRef(var___SIM__line266);
        var___SIM__line266=NULL;
      }
      if (var___SIM__rcpt32)
      { RemoveRef(var___SIM__rcpt32);
        var___SIM__rcpt32=NULL;
      }
      if (var___SIM__s168)
      { RemoveRef(var___SIM__s168);
        var___SIM__s168=NULL;
      }
      if (var___SIM__s269)
      { RemoveRef(var___SIM__s269);
        var___SIM__s269=NULL;
      }
      if (var___SIM__subject51)
      { RemoveRef(var___SIM__subject51);
        var___SIM__subject51=NULL;
      }
      if (var___SIM__this50)
      { RemoveRef(var___SIM__this50);
        var___SIM__this50=NULL;
      }
    }
    libpi::Session* var___SIM__c67;
    libpi::task::Link* var___SIM__console4;
    libpi::String* var___SIM__data52;
    libpi::String* var___SIM__from49;
    libpi::String* var___SIM__line165;
    libpi::String* var___SIM__line266;
    libpi::String* var___SIM__rcpt32;
    libpi::String* var___SIM__s168;
    libpi::String* var___SIM__s269;
    libpi::String* var___SIM__subject51;
    libpi::Session* var___SIM__this50;
    inline void SetArg0(libpi::Session* val)
    {
      val->AddRef();
      if (var___SIM__this50)
        RemoveRef(var___SIM__this50);
      var___SIM__this50=val; 
    }
    inline void SetArg1(libpi::String* val)
    {
      val->AddRef();
      if (var___SIM__subject51)
        RemoveRef(var___SIM__subject51);
      var___SIM__subject51=val; 
    }
    inline void SetArg2(libpi::String* val)
    {
      val->AddRef();
      if (var___SIM__data52)
        RemoveRef(var___SIM__data52);
      var___SIM__data52=val; 
    }
    inline void SetArg3(libpi::task::Link* val)
    {
      val->AddRef();
      if (var___SIM__console4)
        RemoveRef(var___SIM__console4);
      var___SIM__console4=val; 
    }
    inline void SetArg4(libpi::String* val)
    {
      val->AddRef();
      if (var___SIM__from49)
        RemoveRef(var___SIM__from49);
      var___SIM__from49=val; 
    }
    inline void SetArg5(libpi::String* val)
    {
      val->AddRef();
      if (var___SIM__rcpt32)
        RemoveRef(var___SIM__rcpt32);
      var___SIM__rcpt32=val; 
    }
};
class Task___SIM__SmtpServerMail5 : public libpi::task::Task
{ public:
    Task___SIM__SmtpServerMail5()
    { 
      var___SIM__console4=NULL;
      var___SIM__from49=NULL;
      var___SIM__r70=NULL;
      var___SIM__rcpt32=NULL;
      var___SIM__this31=NULL;
      var___SIM__to48=NULL;
    }
    virtual ~Task___SIM__SmtpServerMail5()
    {
      if (var___SIM__console4)
      { RemoveRef(var___SIM__console4);
        var___SIM__console4=NULL;
      }
      if (var___SIM__from49)
      { RemoveRef(var___SIM__from49);
        var___SIM__from49=NULL;
      }
      if (var___SIM__r70)
      { RemoveRef(var___SIM__r70);
        var___SIM__r70=NULL;
      }
      if (var___SIM__rcpt32)
      { RemoveRef(var___SIM__rcpt32);
        var___SIM__rcpt32=NULL;
      }
      if (var___SIM__this31)
      { RemoveRef(var___SIM__this31);
        var___SIM__this31=NULL;
      }
      if (var___SIM__to48)
      { RemoveRef(var___SIM__to48);
        var___SIM__to48=NULL;
      }
    }
    libpi::task::Link* var___SIM__console4;
    libpi::String* var___SIM__from49;
    libpi::String* var___SIM__r70;
    libpi::String* var___SIM__rcpt32;
    libpi::Session* var___SIM__this31;
    libpi::String* var___SIM__to48;
    inline void SetArg0(libpi::Session* val)
    {
      val->AddRef();
      if (var___SIM__this31)
        RemoveRef(var___SIM__this31);
      var___SIM__this31=val; 
    }
    inline void SetArg1(libpi::String* val)
    {
      val->AddRef();
      if (var___SIM__rcpt32)
        RemoveRef(var___SIM__rcpt32);
      var___SIM__rcpt32=val; 
    }
    inline void SetArg2(libpi::task::Link* val)
    {
      val->AddRef();
      if (var___SIM__console4)
        RemoveRef(var___SIM__console4);
      var___SIM__console4=val; 
    }
};
class Task___SIM__SmtpServerData8 : public libpi::task::Task
{ public:
    Task___SIM__SmtpServerData8()
    { 
      var___SIM__c109=NULL;
      var___SIM__console4=NULL;
      var___SIM__data94=NULL;
      var___SIM__from91=NULL;
      var___SIM__line1107=NULL;
      var___SIM__line2108=NULL;
      var___SIM__rcpt74=NULL;
      var___SIM__s1110=NULL;
      var___SIM__s2111=NULL;
      var___SIM__subject93=NULL;
      var___SIM__this92=NULL;
    }
    virtual ~Task___SIM__SmtpServerData8()
    {
      if (var___SIM__c109)
      { RemoveRef(var___SIM__c109);
        var___SIM__c109=NULL;
      }
      if (var___SIM__console4)
      { RemoveRef(var___SIM__console4);
        var___SIM__console4=NULL;
      }
      if (var___SIM__data94)
      { RemoveRef(var___SIM__data94);
        var___SIM__data94=NULL;
      }
      if (var___SIM__from91)
      { RemoveRef(var___SIM__from91);
        var___SIM__from91=NULL;
      }
      if (var___SIM__line1107)
      { RemoveRef(var___SIM__line1107);
        var___SIM__line1107=NULL;
      }
      if (var___SIM__line2108)
      { RemoveRef(var___SIM__line2108);
        var___SIM__line2108=NULL;
      }
      if (var___SIM__rcpt74)
      { RemoveRef(var___SIM__rcpt74);
        var___SIM__rcpt74=NULL;
      }
      if (var___SIM__s1110)
      { RemoveRef(var___SIM__s1110);
        var___SIM__s1110=NULL;
      }
      if (var___SIM__s2111)
      { RemoveRef(var___SIM__s2111);
        var___SIM__s2111=NULL;
      }
      if (var___SIM__subject93)
      { RemoveRef(var___SIM__subject93);
        var___SIM__subject93=NULL;
      }
      if (var___SIM__this92)
      { RemoveRef(var___SIM__this92);
        var___SIM__this92=NULL;
      }
    }
    libpi::Session* var___SIM__c109;
    libpi::task::Link* var___SIM__console4;
    libpi::String* var___SIM__data94;
    libpi::String* var___SIM__from91;
    libpi::String* var___SIM__line1107;
    libpi::String* var___SIM__line2108;
    libpi::String* var___SIM__rcpt74;
    libpi::String* var___SIM__s1110;
    libpi::String* var___SIM__s2111;
    libpi::String* var___SIM__subject93;
    libpi::Session* var___SIM__this92;
    inline void SetArg0(libpi::Session* val)
    {
      val->AddRef();
      if (var___SIM__this92)
        RemoveRef(var___SIM__this92);
      var___SIM__this92=val; 
    }
    inline void SetArg1(libpi::String* val)
    {
      val->AddRef();
      if (var___SIM__subject93)
        RemoveRef(var___SIM__subject93);
      var___SIM__subject93=val; 
    }
    inline void SetArg2(libpi::String* val)
    {
      val->AddRef();
      if (var___SIM__data94)
        RemoveRef(var___SIM__data94);
      var___SIM__data94=val; 
    }
    inline void SetArg3(libpi::task::Link* val)
    {
      val->AddRef();
      if (var___SIM__console4)
        RemoveRef(var___SIM__console4);
      var___SIM__console4=val; 
    }
    inline void SetArg4(libpi::String* val)
    {
      val->AddRef();
      if (var___SIM__from91)
        RemoveRef(var___SIM__from91);
      var___SIM__from91=val; 
    }
    inline void SetArg5(libpi::String* val)
    {
      val->AddRef();
      if (var___SIM__rcpt74)
        RemoveRef(var___SIM__rcpt74);
      var___SIM__rcpt74=val; 
    }
};
class Task___SIM__SmtpServerMail7 : public libpi::task::Task
{ public:
    Task___SIM__SmtpServerMail7()
    { 
      var___SIM__console4=NULL;
      var___SIM__from91=NULL;
      var___SIM__r112=NULL;
      var___SIM__rcpt74=NULL;
      var___SIM__this73=NULL;
      var___SIM__to90=NULL;
    }
    virtual ~Task___SIM__SmtpServerMail7()
    {
      if (var___SIM__console4)
      { RemoveRef(var___SIM__console4);
        var___SIM__console4=NULL;
      }
      if (var___SIM__from91)
      { RemoveRef(var___SIM__from91);
        var___SIM__from91=NULL;
      }
      if (var___SIM__r112)
      { RemoveRef(var___SIM__r112);
        var___SIM__r112=NULL;
      }
      if (var___SIM__rcpt74)
      { RemoveRef(var___SIM__rcpt74);
        var___SIM__rcpt74=NULL;
      }
      if (var___SIM__this73)
      { RemoveRef(var___SIM__this73);
        var___SIM__this73=NULL;
      }
      if (var___SIM__to90)
      { RemoveRef(var___SIM__to90);
        var___SIM__to90=NULL;
      }
    }
    libpi::task::Link* var___SIM__console4;
    libpi::String* var___SIM__from91;
    libpi::String* var___SIM__r112;
    libpi::String* var___SIM__rcpt74;
    libpi::Session* var___SIM__this73;
    libpi::String* var___SIM__to90;
    inline void SetArg0(libpi::Session* val)
    {
      val->AddRef();
      if (var___SIM__this73)
        RemoveRef(var___SIM__this73);
      var___SIM__this73=val; 
    }
    inline void SetArg1(libpi::String* val)
    {
      val->AddRef();
      if (var___SIM__rcpt74)
        RemoveRef(var___SIM__rcpt74);
      var___SIM__rcpt74=val; 
    }
    inline void SetArg2(libpi::task::Link* val)
    {
      val->AddRef();
      if (var___SIM__console4)
        RemoveRef(var___SIM__console4);
      var___SIM__console4=val; 
    }
};
class Task___SIM__SmtpServerAuth4 : public libpi::task::Task
{ public:
    Task___SIM__SmtpServerAuth4()
    { 
      var___SIM__console4=NULL;
      var___SIM__from29=NULL;
      var___SIM__from72=NULL;
      var___SIM__this20=NULL;
    }
    virtual ~Task___SIM__SmtpServerAuth4()
    {
      if (var___SIM__console4)
      { RemoveRef(var___SIM__console4);
        var___SIM__console4=NULL;
      }
      if (var___SIM__from29)
      { RemoveRef(var___SIM__from29);
        var___SIM__from29=NULL;
      }
      if (var___SIM__from72)
      { RemoveRef(var___SIM__from72);
        var___SIM__from72=NULL;
      }
      if (var___SIM__this20)
      { RemoveRef(var___SIM__this20);
        var___SIM__this20=NULL;
      }
    }
    libpi::task::Link* var___SIM__console4;
    libpi::String* var___SIM__from29;
    libpi::String* var___SIM__from72;
    libpi::Session* var___SIM__this20;
    bool var___SIM__valid_from30;
    bool var___SIM__valid_from71;
    inline void SetArg0(libpi::Session* val)
    {
      val->AddRef();
      if (var___SIM__this20)
        RemoveRef(var___SIM__this20);
      var___SIM__this20=val; 
    }
    inline void SetArg1(libpi::task::Link* val)
    {
      val->AddRef();
      if (var___SIM__console4)
        RemoveRef(var___SIM__console4);
      var___SIM__console4=val; 
    }
};
class Task___SIM__FlatRight12 : public libpi::task::Task
{ public:
    Task___SIM__FlatRight12()
    { 
      var___SIM__console4=NULL;
      var___SIM__msg19=NULL;
      var___SIM__this18=NULL;
    }
    virtual ~Task___SIM__FlatRight12()
    {
      if (var___SIM__console4)
      { RemoveRef(var___SIM__console4);
        var___SIM__console4=NULL;
      }
      if (var___SIM__msg19)
      { RemoveRef(var___SIM__msg19);
        var___SIM__msg19=NULL;
      }
      if (var___SIM__this18)
      { RemoveRef(var___SIM__this18);
        var___SIM__this18=NULL;
      }
    }
    libpi::task::Link* var___SIM__console4;
    libpi::String* var___SIM__msg19;
    libpi::Session* var___SIM__this18;
    inline void SetArg0(libpi::task::Link* val)
    {
      val->AddRef();
      if (var___SIM__console4)
        RemoveRef(var___SIM__console4);
      var___SIM__console4=val; 
    }
    inline void SetArg1(libpi::Session* val)
    {
      val->AddRef();
      if (var___SIM__this18)
        RemoveRef(var___SIM__this18);
      var___SIM__this18=val; 
    }
};
class Task___SIM__SmtpServer3 : public libpi::task::Task
{ public:
    Task___SIM__SmtpServer3()
    { 
      var___SIM__console4=NULL;
      var___SIM__smtp17=NULL;
      var___SIM__this18=NULL;
    }
    virtual ~Task___SIM__SmtpServer3()
    {
      if (var___SIM__console4)
      { RemoveRef(var___SIM__console4);
        var___SIM__console4=NULL;
      }
      if (var___SIM__smtp17)
      { RemoveRef(var___SIM__smtp17);
        var___SIM__smtp17=NULL;
      }
      if (var___SIM__this18)
      { RemoveRef(var___SIM__this18);
        var___SIM__this18=NULL;
      }
    }
    libpi::task::Link* var___SIM__console4;
    libpi::task::Link* var___SIM__smtp17;
    libpi::Session* var___SIM__this18;
    inline void SetArg0(libpi::task::Link* val)
    {
      val->AddRef();
      if (var___SIM__console4)
        RemoveRef(var___SIM__console4);
      var___SIM__console4=val; 
    }
    inline void SetArg1(libpi::task::Link* val)
    {
      val->AddRef();
      if (var___SIM__smtp17)
        RemoveRef(var___SIM__smtp17);
      var___SIM__smtp17=val; 
    }
};
class Task___SIM__StartService9 : public libpi::task::Task
{ public:
    Task___SIM__StartService9()
    { 
      var___SIM__console4=NULL;
      var___SIM__i113=NULL;
      var___SIM__smtp17=NULL;
    }
    virtual ~Task___SIM__StartService9()
    {
      if (var___SIM__console4)
      { RemoveRef(var___SIM__console4);
        var___SIM__console4=NULL;
      }
      if (var___SIM__i113)
      { RemoveRef(var___SIM__i113);
        var___SIM__i113=NULL;
      }
      if (var___SIM__smtp17)
      { RemoveRef(var___SIM__smtp17);
        var___SIM__smtp17=NULL;
      }
    }
    libpi::task::Link* var___SIM__console4;
    libpi::Int* var___SIM__i113;
    libpi::task::Link* var___SIM__smtp17;
    inline void SetArg0(libpi::Int* val)
    {
      val->AddRef();
      if (var___SIM__i113)
        RemoveRef(var___SIM__i113);
      var___SIM__i113=val; 
    }
    inline void SetArg1(libpi::task::Link* val)
    {
      val->AddRef();
      if (var___SIM__console4)
        RemoveRef(var___SIM__console4);
      var___SIM__console4=val; 
    }
    inline void SetArg2(libpi::task::Link* val)
    {
      val->AddRef();
      if (var___SIM__smtp17)
        RemoveRef(var___SIM__smtp17);
      var___SIM__smtp17=val; 
    }
};
class Task___SIM__SmtpClientTls11 : public libpi::task::Task
{ public:
    Task___SIM__SmtpClientTls11()
    { 
      var___SIM__srv138=NULL;
      var___SIM__this128=NULL;
    }
    virtual ~Task___SIM__SmtpClientTls11()
    {
      if (var___SIM__srv138)
      { RemoveRef(var___SIM__srv138);
        var___SIM__srv138=NULL;
      }
      if (var___SIM__this128)
      { RemoveRef(var___SIM__this128);
        var___SIM__this128=NULL;
      }
    }
    libpi::String* var___SIM__srv138;
    libpi::Session* var___SIM__this128;
    inline void SetArg0(libpi::Session* val)
    {
      val->AddRef();
      if (var___SIM__this128)
        RemoveRef(var___SIM__this128);
      var___SIM__this128=val; 
    }
};
class Task___SIM__SmtpClientEhlo10 : public libpi::task::Task
{ public:
    Task___SIM__SmtpClientEhlo10()
    { 
      var___SIM__srv127=NULL;
      var___SIM__this116=NULL;
    }
    virtual ~Task___SIM__SmtpClientEhlo10()
    {
      if (var___SIM__srv127)
      { RemoveRef(var___SIM__srv127);
        var___SIM__srv127=NULL;
      }
      if (var___SIM__this116)
      { RemoveRef(var___SIM__this116);
        var___SIM__this116=NULL;
      }
    }
    libpi::String* var___SIM__srv127;
    libpi::Session* var___SIM__this116;
    inline void SetArg0(libpi::Session* val)
    {
      val->AddRef();
      if (var___SIM__this116)
        RemoveRef(var___SIM__this116);
      var___SIM__this116=val; 
    }
};
class Task___SIM__FlatRight13 : public libpi::task::Task
{ public:
    Task___SIM__FlatRight13()
    { 
      var___SIM__client114=NULL;
      var___SIM__msg115=NULL;
      var___SIM__smtp17=NULL;
    }
    virtual ~Task___SIM__FlatRight13()
    {
      if (var___SIM__client114)
      { RemoveRef(var___SIM__client114);
        var___SIM__client114=NULL;
      }
      if (var___SIM__msg115)
      { RemoveRef(var___SIM__msg115);
        var___SIM__msg115=NULL;
      }
      if (var___SIM__smtp17)
      { RemoveRef(var___SIM__smtp17);
        var___SIM__smtp17=NULL;
      }
    }
    libpi::Session* var___SIM__client114;
    libpi::String* var___SIM__msg115;
    libpi::task::Link* var___SIM__smtp17;
    inline void SetArg0(libpi::task::Link* val)
    {
      val->AddRef();
      if (var___SIM__smtp17)
        RemoveRef(var___SIM__smtp17);
      var___SIM__smtp17=val; 
    }
};
class Task___SIM__FlatRight14 : public libpi::task::Task
{ public:
    Task___SIM__FlatRight14()
    { 
      var___SIM__console4=NULL;
      var___SIM__smtp17=NULL;
    }
    virtual ~Task___SIM__FlatRight14()
    {
      if (var___SIM__console4)
      { RemoveRef(var___SIM__console4);
        var___SIM__console4=NULL;
      }
      if (var___SIM__smtp17)
      { RemoveRef(var___SIM__smtp17);
        var___SIM__smtp17=NULL;
      }
    }
    libpi::task::Link* var___SIM__console4;
    libpi::task::Link* var___SIM__smtp17;
    inline void SetArg0(libpi::task::Link* val)
    {
      val->AddRef();
      if (var___SIM__console4)
        RemoveRef(var___SIM__console4);
      var___SIM__console4=val; 
    }
};
// }}}
// All Methods {{{
inline bool _methods(libpi::task::Task*& _task)
{ size_t _steps=0;
  void *_label=_task->GetLabel();
  if (_label!=NULL)
    goto *_label;
  method_Main:
  #define _this ((TaskMain*)_task)
  { // Main {{{

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_435);
      return true;
    }
__SIM__checkpoint_435:
    _this->var___SIM__console4=new libpi::task::Link(2);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_436);
      return true;
    }
__SIM__checkpoint_436:
    { Task___SIM__FlatRight14 *__SIM__task437(new Task___SIM__FlatRight14());
      __SIM__task437->SetWorker(&_task->GetWorker());
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg0(_this->var___SIM__console4);
      __SIM__task437->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task437->SetLabel(&&method___SIM__FlatRight14);
      _task->GetWorker().AddTask(__SIM__task437);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_439);
      return true;
    }
__SIM__checkpoint_439:
    if (dynamic_cast<Task___SIM__Console1*>(_task)!=NULL) // Recursive call
    { Task___SIM__Console1* _taskref=(Task___SIM__Console1*)_task;
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg0(_this->var___SIM__console4);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetLabel(&&method___SIM__Console1);

    }
    else
    { Task___SIM__Console1 *__SIM__task438 = new Task___SIM__Console1();
      __SIM__task438->SetWorker(&_task->GetWorker());
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg0(_this->var___SIM__console4);
      __SIM__task438->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task438->SetLabel(&&method___SIM__Console1);
      RemoveRef(_task);
      _task=__SIM__task438;
    }
    goto method___SIM__Console1;
  } // }}}

/* Procedure implementations */
  method___SIM__ConsoleSession2: // {{{
  #undef _this
  #define _this ((Task___SIM__ConsoleSession2*)_task)
  {

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_447);
      return true;
    }
__SIM__checkpoint_447:
    _task->SetLabel(&&__SIM__checkpoint___SIM__ConsoleSession2446);
    if (!_this->var___SIM__s6->Receive(1,_task,&_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__ConsoleSession2446:
    if (((libpi::String*)_task->tmp)->GetValue()=="^end")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->var___SIM__s6->Close(true);
      RemoveRef(_this->var___SIM__s6);
      _this->var___SIM__s6=NULL;
      _this->SetLabel(&&__SIM____SIM__ConsoleSession2end440);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^flt")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__ConsoleSession2flt441);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^input")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__ConsoleSession2input442);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^int")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__ConsoleSession2int443);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^nl")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__ConsoleSession2nl444);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^str")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__ConsoleSession2str445);
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
    goto *_this->GetLabel();
__SIM____SIM__ConsoleSession2end440:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_449);
      return true;
    }
__SIM__checkpoint_449:
    if (dynamic_cast<Task___SIM__Console1*>(_task)!=NULL) // Recursive call
    { Task___SIM__Console1* _taskref=(Task___SIM__Console1*)_task;
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg0(_this->var___SIM__console4);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetLabel(&&method___SIM__Console1);

    }
    else
    { Task___SIM__Console1 *__SIM__task448 = new Task___SIM__Console1();
      __SIM__task448->SetWorker(&_task->GetWorker());
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg0(_this->var___SIM__console4);
      __SIM__task448->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task448->SetLabel(&&method___SIM__Console1);
      RemoveRef(_task);
      _task=__SIM__task448;
    }
    goto method___SIM__Console1;
__SIM____SIM__ConsoleSession2flt441:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_451);
      return true;
    }
__SIM__checkpoint_451:
    // ~s6[2] >> ~f12;
    _task->SetLabel(&&__SIM__receive450);
    if (!_this->var___SIM__s6->Receive(1,_task,(libpi::Value**)&_this->var___SIM__f12))
      return false;
    __SIM__receive450:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_454);
      return true;
    }
__SIM__checkpoint_454:
  // HOST STATEMENT BEGIN
          _this->var___SIM__f12->AddRef();
std::cout <<  _this->var___SIM__f12 ->ToString() << std::flush;  // HOST STATEMENT END

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_453);
      return true;
    }
__SIM__checkpoint_453:
    if (dynamic_cast<Task___SIM__ConsoleSession2*>(_task)!=NULL) // Recursive call
    { Task___SIM__ConsoleSession2* _taskref=(Task___SIM__ConsoleSession2*)_task;
      _this->var___SIM__s6->AddRef();
      libpi::Session* arg0(_this->var___SIM__s6);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetLabel(&&method___SIM__ConsoleSession2);

    }
    else
    { Task___SIM__ConsoleSession2 *__SIM__task452 = new Task___SIM__ConsoleSession2();
      __SIM__task452->SetWorker(&_task->GetWorker());
      _this->var___SIM__s6->AddRef();
      libpi::Session* arg0(_this->var___SIM__s6);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      __SIM__task452->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task452->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task452->SetLabel(&&method___SIM__ConsoleSession2);
      RemoveRef(_task);
      _task=__SIM__task452;
    }
    goto method___SIM__ConsoleSession2;
__SIM____SIM__ConsoleSession2input442:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_458);
      return true;
    }
__SIM__checkpoint_458:
    _task->SetLabel(&&__SIM__checkpoint___SIM__ConsoleSession2457);
    if (!_this->var___SIM__s6->Receive(1,_task,&_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__ConsoleSession2457:
    if (((libpi::String*)_task->tmp)->GetValue()=="^int")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__ConsoleSession2int455);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^str")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__ConsoleSession2str456);
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
    goto *_this->GetLabel();
__SIM____SIM__ConsoleSession2int455:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_459);
      return true;
    }
__SIM__checkpoint_459:
    {
      intval_0->AddRef();
      _this->var___SIM__val13=intval_0;
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_464);
      return true;
    }
__SIM__checkpoint_464:
  // HOST STATEMENT BEGIN
          _this->var___SIM__val13->AddRef();
{ long _l; std::cin >> _l;  _this->var___SIM__val13 =new libpi::Int(_l); }  // HOST STATEMENT END

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_461);
      return true;
    }
__SIM__checkpoint_461:
    // ~s6[2] << ~val13;
    { 
      _this->var___SIM__val13->AddRef();
      _this->var___SIM__s6->Send(1,_task,_this->var___SIM__val13);
      RemoveRef(_this->var___SIM__val13);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_463);
      return true;
    }
__SIM__checkpoint_463:
    if (dynamic_cast<Task___SIM__ConsoleSession2*>(_task)!=NULL) // Recursive call
    { Task___SIM__ConsoleSession2* _taskref=(Task___SIM__ConsoleSession2*)_task;
      _this->var___SIM__s6->AddRef();
      libpi::Session* arg0(_this->var___SIM__s6);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetLabel(&&method___SIM__ConsoleSession2);

    }
    else
    { Task___SIM__ConsoleSession2 *__SIM__task462 = new Task___SIM__ConsoleSession2();
      __SIM__task462->SetWorker(&_task->GetWorker());
      _this->var___SIM__s6->AddRef();
      libpi::Session* arg0(_this->var___SIM__s6);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      __SIM__task462->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task462->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task462->SetLabel(&&method___SIM__ConsoleSession2);
      RemoveRef(_task);
      _task=__SIM__task462;
    }
    goto method___SIM__ConsoleSession2;
__SIM____SIM__ConsoleSession2str456:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_465);
      return true;
    }
__SIM__checkpoint_465:
    {
      stringval_d41d8cd98f00b204e9800998ecf8427e->AddRef();
      _this->var___SIM__val14=stringval_d41d8cd98f00b204e9800998ecf8427e;
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_470);
      return true;
    }
__SIM__checkpoint_470:
  // HOST STATEMENT BEGIN
          _this->var___SIM__val14->AddRef();
{ std::string _s; std::cin >> _s;  _this->var___SIM__val14 =new libpi::String(_s); }  // HOST STATEMENT END

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_467);
      return true;
    }
__SIM__checkpoint_467:
    // ~s6[2] << ~val14;
    { 
      _this->var___SIM__val14->AddRef();
      _this->var___SIM__s6->Send(1,_task,_this->var___SIM__val14);
      RemoveRef(_this->var___SIM__val14);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_469);
      return true;
    }
__SIM__checkpoint_469:
    if (dynamic_cast<Task___SIM__ConsoleSession2*>(_task)!=NULL) // Recursive call
    { Task___SIM__ConsoleSession2* _taskref=(Task___SIM__ConsoleSession2*)_task;
      _this->var___SIM__s6->AddRef();
      libpi::Session* arg0(_this->var___SIM__s6);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetLabel(&&method___SIM__ConsoleSession2);

    }
    else
    { Task___SIM__ConsoleSession2 *__SIM__task468 = new Task___SIM__ConsoleSession2();
      __SIM__task468->SetWorker(&_task->GetWorker());
      _this->var___SIM__s6->AddRef();
      libpi::Session* arg0(_this->var___SIM__s6);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      __SIM__task468->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task468->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task468->SetLabel(&&method___SIM__ConsoleSession2);
      RemoveRef(_task);
      _task=__SIM__task468;
    }
    goto method___SIM__ConsoleSession2;
__SIM____SIM__ConsoleSession2int443:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_472);
      return true;
    }
__SIM__checkpoint_472:
    // ~s6[2] >> ~i15;
    _task->SetLabel(&&__SIM__receive471);
    if (!_this->var___SIM__s6->Receive(1,_task,(libpi::Value**)&_this->var___SIM__i15))
      return false;
    __SIM__receive471:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_475);
      return true;
    }
__SIM__checkpoint_475:
  // HOST STATEMENT BEGIN
          _this->var___SIM__i15->AddRef();
std::cout <<  _this->var___SIM__i15 ->ToString() << std::flush;  // HOST STATEMENT END

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_474);
      return true;
    }
__SIM__checkpoint_474:
    if (dynamic_cast<Task___SIM__ConsoleSession2*>(_task)!=NULL) // Recursive call
    { Task___SIM__ConsoleSession2* _taskref=(Task___SIM__ConsoleSession2*)_task;
      _this->var___SIM__s6->AddRef();
      libpi::Session* arg0(_this->var___SIM__s6);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetLabel(&&method___SIM__ConsoleSession2);

    }
    else
    { Task___SIM__ConsoleSession2 *__SIM__task473 = new Task___SIM__ConsoleSession2();
      __SIM__task473->SetWorker(&_task->GetWorker());
      _this->var___SIM__s6->AddRef();
      libpi::Session* arg0(_this->var___SIM__s6);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      __SIM__task473->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task473->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task473->SetLabel(&&method___SIM__ConsoleSession2);
      RemoveRef(_task);
      _task=__SIM__task473;
    }
    goto method___SIM__ConsoleSession2;
__SIM____SIM__ConsoleSession2nl444:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_478);
      return true;
    }
__SIM__checkpoint_478:
  // HOST STATEMENT BEGIN
    std::cout << std::endl;  // HOST STATEMENT END

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_477);
      return true;
    }
__SIM__checkpoint_477:
    if (dynamic_cast<Task___SIM__ConsoleSession2*>(_task)!=NULL) // Recursive call
    { Task___SIM__ConsoleSession2* _taskref=(Task___SIM__ConsoleSession2*)_task;
      _this->var___SIM__s6->AddRef();
      libpi::Session* arg0(_this->var___SIM__s6);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetLabel(&&method___SIM__ConsoleSession2);

    }
    else
    { Task___SIM__ConsoleSession2 *__SIM__task476 = new Task___SIM__ConsoleSession2();
      __SIM__task476->SetWorker(&_task->GetWorker());
      _this->var___SIM__s6->AddRef();
      libpi::Session* arg0(_this->var___SIM__s6);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      __SIM__task476->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task476->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task476->SetLabel(&&method___SIM__ConsoleSession2);
      RemoveRef(_task);
      _task=__SIM__task476;
    }
    goto method___SIM__ConsoleSession2;
__SIM____SIM__ConsoleSession2str445:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_480);
      return true;
    }
__SIM__checkpoint_480:
    // ~s6[2] >> ~str16;
    _task->SetLabel(&&__SIM__receive479);
    if (!_this->var___SIM__s6->Receive(1,_task,(libpi::Value**)&_this->var___SIM__str16))
      return false;
    __SIM__receive479:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_483);
      return true;
    }
__SIM__checkpoint_483:
  // HOST STATEMENT BEGIN
          _this->var___SIM__str16->AddRef();
std::cout <<  _this->var___SIM__str16 ->GetValue() << std::flush;  // HOST STATEMENT END

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_482);
      return true;
    }
__SIM__checkpoint_482:
    if (dynamic_cast<Task___SIM__ConsoleSession2*>(_task)!=NULL) // Recursive call
    { Task___SIM__ConsoleSession2* _taskref=(Task___SIM__ConsoleSession2*)_task;
      _this->var___SIM__s6->AddRef();
      libpi::Session* arg0(_this->var___SIM__s6);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetLabel(&&method___SIM__ConsoleSession2);

    }
    else
    { Task___SIM__ConsoleSession2 *__SIM__task481 = new Task___SIM__ConsoleSession2();
      __SIM__task481->SetWorker(&_task->GetWorker());
      _this->var___SIM__s6->AddRef();
      libpi::Session* arg0(_this->var___SIM__s6);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      __SIM__task481->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task481->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task481->SetLabel(&&method___SIM__ConsoleSession2);
      RemoveRef(_task);
      _task=__SIM__task481;
    }
    goto method___SIM__ConsoleSession2;
  } // }}}
  method___SIM__Console1: // {{{
  #undef _this
  #define _this ((Task___SIM__Console1*)_task)
  {
    // ~s5 = new ~console4(1 of 2)

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_484);
      return true;
    }
__SIM__checkpoint_484:
    { // link
      _task->tmps.clear();
        _task->SetLabel(&&__SIM__receive___SIM__Console1485);
        if (!_this->var___SIM__console4->GetChannels()[0]->Receive(_task,&_task->tmp))
          return false;
        __SIM__receive___SIM__Console1485:
        _task->tmps.push_back(_task->tmp);
      vector<vector<libpi::task::Channel*> > inChannels(2);
      vector<vector<libpi::task::Channel*> > outChannels(2);
      // Optimize vector inserts
        inChannels[0].resize(2);
        outChannels[0].resize(2);
        inChannels[1].resize(2);
        outChannels[1].resize(2);
      // Create channels
      inChannels[0][0]=new libpi::task::Channel();
      inChannels[0][1]=new libpi::task::Channel();
      inChannels[1][0]=dynamic_cast<libpi::task::Channel*>(_task->tmps[0]);
      inChannels[1][1]=new libpi::task::Channel();
      outChannels[0][0]=inChannels[0][0];
      outChannels[0][1]=inChannels[1][0];
      outChannels[1][0]=inChannels[0][1];
      outChannels[1][1]=inChannels[1][1];
      // Send sessions
      { libpi::Session* _s(new libpi::Session(1,2, inChannels[1], outChannels[1]));
        ((libpi::task::Channel*)_task->tmps[0])->Send(_task,_s);
        RemoveRef(_s);
      }
      // Create local session
      AssignNewValue((libpi::Value**)&_this->var___SIM__s5,new libpi::Session(0,2,inChannels[0],outChannels[0]));
      // Clean up
      while (!inChannels.empty())
      { while (!inChannels.back().empty())
        { RemoveRef(inChannels.back().back());
          inChannels.back().pop_back();
        }
        inChannels.pop_back();
      }
      // outChannels refs inChannels and should not be unreferenced
      _task->tmps.clear();
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_487);
      return true;
    }
__SIM__checkpoint_487:
    if (dynamic_cast<Task___SIM__ConsoleSession2*>(_task)!=NULL) // Recursive call
    { Task___SIM__ConsoleSession2* _taskref=(Task___SIM__ConsoleSession2*)_task;
      _this->var___SIM__s5->AddRef();
      libpi::Session* arg0(_this->var___SIM__s5);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetLabel(&&method___SIM__ConsoleSession2);

    }
    else
    { Task___SIM__ConsoleSession2 *__SIM__task486 = new Task___SIM__ConsoleSession2();
      __SIM__task486->SetWorker(&_task->GetWorker());
      _this->var___SIM__s5->AddRef();
      libpi::Session* arg0(_this->var___SIM__s5);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      __SIM__task486->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task486->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task486->SetLabel(&&method___SIM__ConsoleSession2);
      RemoveRef(_task);
      _task=__SIM__task486;
    }
    goto method___SIM__ConsoleSession2;
  } // }}}
  method___SIM__SmtpServerData6: // {{{
  #undef _this
  #define _this ((Task___SIM__SmtpServerData6*)_task)
  {

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_492);
      return true;
    }
__SIM__checkpoint_492:
    _task->SetLabel(&&__SIM__checkpoint___SIM__SmtpServerData6491);
    if (!_this->var___SIM__this50->Receive(1,_task,&_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__SmtpServerData6491:
    if (((libpi::String*)_task->tmp)->GetValue()=="^DATALINE")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpServerData6DATALINE488);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^ENDOFDATA")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpServerData6ENDOFDATA489);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^SUBJECT")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpServerData6SUBJECT490);
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
    goto *_this->GetLabel();
__SIM____SIM__SmtpServerData6DATALINE488:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_494);
      return true;
    }
__SIM__checkpoint_494:
    // ~this50[2] >> ~line165;
    _task->SetLabel(&&__SIM__receive493);
    if (!_this->var___SIM__this50->Receive(1,_task,(libpi::Value**)&_this->var___SIM__line165))
      return false;
    __SIM__receive493:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_496);
      return true;
    }
__SIM__checkpoint_496:
    // ~this50[2] >> ~line266;
    _task->SetLabel(&&__SIM__receive495);
    if (!_this->var___SIM__this50->Receive(1,_task,(libpi::Value**)&_this->var___SIM__line266))
      return false;
    __SIM__receive495:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_498);
      return true;
    }
__SIM__checkpoint_498:
    if (dynamic_cast<Task___SIM__SmtpServerData6*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpServerData6* _taskref=(Task___SIM__SmtpServerData6*)_task;
      _this->var___SIM__this50->AddRef();
      libpi::Session* arg0(_this->var___SIM__this50);
      _this->var___SIM__subject51->AddRef();
      libpi::String* arg1(_this->var___SIM__subject51);
      _this->var___SIM__line165->AddRef();
      libpi::String* arg2(_this->var___SIM__line165);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg3(_this->var___SIM__console4);
      _this->var___SIM__from49->AddRef();
      libpi::String* arg4(_this->var___SIM__from49);
      _this->var___SIM__rcpt32->AddRef();
      libpi::String* arg5(_this->var___SIM__rcpt32);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetArg2( arg2);
      RemoveRef(arg2);
      _taskref->SetArg3( arg3);
      RemoveRef(arg3);
      _taskref->SetArg4( arg4);
      RemoveRef(arg4);
      _taskref->SetArg5( arg5);
      RemoveRef(arg5);
      _taskref->SetLabel(&&method___SIM__SmtpServerData6);

    }
    else
    { Task___SIM__SmtpServerData6 *__SIM__task497 = new Task___SIM__SmtpServerData6();
      __SIM__task497->SetWorker(&_task->GetWorker());
      _this->var___SIM__this50->AddRef();
      libpi::Session* arg0(_this->var___SIM__this50);
      _this->var___SIM__subject51->AddRef();
      libpi::String* arg1(_this->var___SIM__subject51);
      _this->var___SIM__line165->AddRef();
      libpi::String* arg2(_this->var___SIM__line165);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg3(_this->var___SIM__console4);
      _this->var___SIM__from49->AddRef();
      libpi::String* arg4(_this->var___SIM__from49);
      _this->var___SIM__rcpt32->AddRef();
      libpi::String* arg5(_this->var___SIM__rcpt32);
      __SIM__task497->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task497->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task497->SetArg2( arg2);
      RemoveRef(arg2);
      __SIM__task497->SetArg3( arg3);
      RemoveRef(arg3);
      __SIM__task497->SetArg4( arg4);
      RemoveRef(arg4);
      __SIM__task497->SetArg5( arg5);
      RemoveRef(arg5);
      __SIM__task497->SetLabel(&&method___SIM__SmtpServerData6);
      RemoveRef(_task);
      _task=__SIM__task497;
    }
    goto method___SIM__SmtpServerData6;
__SIM____SIM__SmtpServerData6ENDOFDATA489:
    // ~c67 = new ~console4(2 of 2)

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_499);
      return true;
    }
__SIM__checkpoint_499:
    { _task->tmp=new libpi::task::Channel();
      _this->var___SIM__console4->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__SmtpServerData6500);
      if (!((libpi::task::Channel*)_task->tmp)->Receive(_task,(libpi::Value**)&_this->var___SIM__c67))
        return false;
      __SIM__receive___SIM__SmtpServerData6500:
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_502);
      return true;
    }
__SIM__checkpoint_502:
    // ~c67[1] << ^str;
    _this->var___SIM__c67->Send(0,_task,label_cf6b4c5b94c4f75325166dbaeb4a0aae);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_504);
      return true;
    }
__SIM__checkpoint_504:
    // ~c67[1] << "Server received mail";
    { 
      stringval_9b054bbaf1ab61f371094ff82128f709->AddRef();
      _this->var___SIM__c67->Send(0,_task,stringval_9b054bbaf1ab61f371094ff82128f709);
      RemoveRef(stringval_9b054bbaf1ab61f371094ff82128f709);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_506);
      return true;
    }
__SIM__checkpoint_506:
    // ~c67[1] << ^nl;
    _this->var___SIM__c67->Send(0,_task,label_7a11449475519daf7b8b8621e5b6eacc);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_508);
      return true;
    }
__SIM__checkpoint_508:
    // ~c67[1] << ^str;
    _this->var___SIM__c67->Send(0,_task,label_cf6b4c5b94c4f75325166dbaeb4a0aae);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_510);
      return true;
    }
__SIM__checkpoint_510:
    // ~c67[1] << "FROM: ";
    { 
      stringval_d362dccf254336f6e31e2b35a396112e->AddRef();
      _this->var___SIM__c67->Send(0,_task,stringval_d362dccf254336f6e31e2b35a396112e);
      RemoveRef(stringval_d362dccf254336f6e31e2b35a396112e);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_512);
      return true;
    }
__SIM__checkpoint_512:
    // ~c67[1] << ^str;
    _this->var___SIM__c67->Send(0,_task,label_cf6b4c5b94c4f75325166dbaeb4a0aae);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_514);
      return true;
    }
__SIM__checkpoint_514:
    // ~c67[1] << ~from49;
    { 
      _this->var___SIM__from49->AddRef();
      _this->var___SIM__c67->Send(0,_task,_this->var___SIM__from49);
      RemoveRef(_this->var___SIM__from49);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_516);
      return true;
    }
__SIM__checkpoint_516:
    // ~c67[1] << ^nl;
    _this->var___SIM__c67->Send(0,_task,label_7a11449475519daf7b8b8621e5b6eacc);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_518);
      return true;
    }
__SIM__checkpoint_518:
    // ~c67[1] << ^str;
    _this->var___SIM__c67->Send(0,_task,label_cf6b4c5b94c4f75325166dbaeb4a0aae);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_520);
      return true;
    }
__SIM__checkpoint_520:
    // ~c67[1] << "RCPT: ";
    { 
      stringval_130365dbea325230ace43dbe736045bf->AddRef();
      _this->var___SIM__c67->Send(0,_task,stringval_130365dbea325230ace43dbe736045bf);
      RemoveRef(stringval_130365dbea325230ace43dbe736045bf);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_522);
      return true;
    }
__SIM__checkpoint_522:
    // ~c67[1] << ^str;
    _this->var___SIM__c67->Send(0,_task,label_cf6b4c5b94c4f75325166dbaeb4a0aae);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_524);
      return true;
    }
__SIM__checkpoint_524:
    // ~c67[1] << ~rcpt32;
    { 
      _this->var___SIM__rcpt32->AddRef();
      _this->var___SIM__c67->Send(0,_task,_this->var___SIM__rcpt32);
      RemoveRef(_this->var___SIM__rcpt32);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_526);
      return true;
    }
__SIM__checkpoint_526:
    // ~c67[1] << ^nl;
    _this->var___SIM__c67->Send(0,_task,label_7a11449475519daf7b8b8621e5b6eacc);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_528);
      return true;
    }
__SIM__checkpoint_528:
    // ~c67[1] << ^str;
    _this->var___SIM__c67->Send(0,_task,label_cf6b4c5b94c4f75325166dbaeb4a0aae);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_530);
      return true;
    }
__SIM__checkpoint_530:
    // ~c67[1] << "SUBJ: ";
    { 
      stringval_2f70e8bfd1389315a838ac269d90cb6e->AddRef();
      _this->var___SIM__c67->Send(0,_task,stringval_2f70e8bfd1389315a838ac269d90cb6e);
      RemoveRef(stringval_2f70e8bfd1389315a838ac269d90cb6e);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_532);
      return true;
    }
__SIM__checkpoint_532:
    // ~c67[1] << ^str;
    _this->var___SIM__c67->Send(0,_task,label_cf6b4c5b94c4f75325166dbaeb4a0aae);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_534);
      return true;
    }
__SIM__checkpoint_534:
    // ~c67[1] << ~subject51;
    { 
      _this->var___SIM__subject51->AddRef();
      _this->var___SIM__c67->Send(0,_task,_this->var___SIM__subject51);
      RemoveRef(_this->var___SIM__subject51);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_536);
      return true;
    }
__SIM__checkpoint_536:
    // ~c67[1] << ^nl;
    _this->var___SIM__c67->Send(0,_task,label_7a11449475519daf7b8b8621e5b6eacc);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_538);
      return true;
    }
__SIM__checkpoint_538:
    // ~c67[1] << ^end;
    _this->var___SIM__c67->Send(0,_task,label_186e35ef3dd9939615c442c30b801a33);
    _this->var___SIM__c67->Close(true);
    RemoveRef(_this->var___SIM__c67);
    _this->var___SIM__c67=NULL;

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_540);
      return true;
    }
__SIM__checkpoint_540:
    if (dynamic_cast<Task___SIM__SmtpServerAuth4*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpServerAuth4* _taskref=(Task___SIM__SmtpServerAuth4*)_task;
      _this->var___SIM__this50->AddRef();
      libpi::Session* arg0(_this->var___SIM__this50);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetLabel(&&method___SIM__SmtpServerAuth4);

    }
    else
    { Task___SIM__SmtpServerAuth4 *__SIM__task539 = new Task___SIM__SmtpServerAuth4();
      __SIM__task539->SetWorker(&_task->GetWorker());
      _this->var___SIM__this50->AddRef();
      libpi::Session* arg0(_this->var___SIM__this50);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      __SIM__task539->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task539->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task539->SetLabel(&&method___SIM__SmtpServerAuth4);
      RemoveRef(_task);
      _task=__SIM__task539;
    }
    goto method___SIM__SmtpServerAuth4;
__SIM____SIM__SmtpServerData6SUBJECT490:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_542);
      return true;
    }
__SIM__checkpoint_542:
    // ~this50[2] >> ~s168;
    _task->SetLabel(&&__SIM__receive541);
    if (!_this->var___SIM__this50->Receive(1,_task,(libpi::Value**)&_this->var___SIM__s168))
      return false;
    __SIM__receive541:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_544);
      return true;
    }
__SIM__checkpoint_544:
    // ~this50[2] >> ~s269;
    _task->SetLabel(&&__SIM__receive543);
    if (!_this->var___SIM__this50->Receive(1,_task,(libpi::Value**)&_this->var___SIM__s269))
      return false;
    __SIM__receive543:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_546);
      return true;
    }
__SIM__checkpoint_546:
    if (dynamic_cast<Task___SIM__SmtpServerData6*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpServerData6* _taskref=(Task___SIM__SmtpServerData6*)_task;
      _this->var___SIM__this50->AddRef();
      libpi::Session* arg0(_this->var___SIM__this50);
      _this->var___SIM__s168->AddRef();
      libpi::String* arg1(_this->var___SIM__s168);
      _this->var___SIM__data52->AddRef();
      libpi::String* arg2(_this->var___SIM__data52);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg3(_this->var___SIM__console4);
      _this->var___SIM__from49->AddRef();
      libpi::String* arg4(_this->var___SIM__from49);
      _this->var___SIM__rcpt32->AddRef();
      libpi::String* arg5(_this->var___SIM__rcpt32);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetArg2( arg2);
      RemoveRef(arg2);
      _taskref->SetArg3( arg3);
      RemoveRef(arg3);
      _taskref->SetArg4( arg4);
      RemoveRef(arg4);
      _taskref->SetArg5( arg5);
      RemoveRef(arg5);
      _taskref->SetLabel(&&method___SIM__SmtpServerData6);

    }
    else
    { Task___SIM__SmtpServerData6 *__SIM__task545 = new Task___SIM__SmtpServerData6();
      __SIM__task545->SetWorker(&_task->GetWorker());
      _this->var___SIM__this50->AddRef();
      libpi::Session* arg0(_this->var___SIM__this50);
      _this->var___SIM__s168->AddRef();
      libpi::String* arg1(_this->var___SIM__s168);
      _this->var___SIM__data52->AddRef();
      libpi::String* arg2(_this->var___SIM__data52);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg3(_this->var___SIM__console4);
      _this->var___SIM__from49->AddRef();
      libpi::String* arg4(_this->var___SIM__from49);
      _this->var___SIM__rcpt32->AddRef();
      libpi::String* arg5(_this->var___SIM__rcpt32);
      __SIM__task545->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task545->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task545->SetArg2( arg2);
      RemoveRef(arg2);
      __SIM__task545->SetArg3( arg3);
      RemoveRef(arg3);
      __SIM__task545->SetArg4( arg4);
      RemoveRef(arg4);
      __SIM__task545->SetArg5( arg5);
      RemoveRef(arg5);
      __SIM__task545->SetLabel(&&method___SIM__SmtpServerData6);
      RemoveRef(_task);
      _task=__SIM__task545;
    }
    goto method___SIM__SmtpServerData6;
  } // }}}
  method___SIM__SmtpServerMail5: // {{{
  #undef _this
  #define _this ((Task___SIM__SmtpServerMail5*)_task)
  {

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_550);
      return true;
    }
__SIM__checkpoint_550:
    _task->SetLabel(&&__SIM__checkpoint___SIM__SmtpServerMail5549);
    if (!_this->var___SIM__this31->Receive(1,_task,&_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__SmtpServerMail5549:
    if (((libpi::String*)_task->tmp)->GetValue()=="^DATA")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpServerMail5DATA547);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^RCPT")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpServerMail5RCPT548);
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
    goto *_this->GetLabel();
__SIM____SIM__SmtpServerMail5DATA547:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_552);
      return true;
    }
__SIM__checkpoint_552:
    // ~this31[2] << ^354;
    _this->var___SIM__this31->Send(1,_task,label_a4850a01ffe59cb572e05301fd3a503f);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_554);
      return true;
    }
__SIM__checkpoint_554:
    // ~this31[2] >> ~to48;
    _task->SetLabel(&&__SIM__receive553);
    if (!_this->var___SIM__this31->Receive(1,_task,(libpi::Value**)&_this->var___SIM__to48))
      return false;
    __SIM__receive553:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_556);
      return true;
    }
__SIM__checkpoint_556:
    // ~this31[2] >> ~from49;
    _task->SetLabel(&&__SIM__receive555);
    if (!_this->var___SIM__this31->Receive(1,_task,(libpi::Value**)&_this->var___SIM__from49))
      return false;
    __SIM__receive555:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_558);
      return true;
    }
__SIM__checkpoint_558:
    if (dynamic_cast<Task___SIM__SmtpServerData6*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpServerData6* _taskref=(Task___SIM__SmtpServerData6*)_task;
      _this->var___SIM__this31->AddRef();
      libpi::Session* arg0(_this->var___SIM__this31);
      stringval_d41d8cd98f00b204e9800998ecf8427e->AddRef();
      libpi::String* arg1(stringval_d41d8cd98f00b204e9800998ecf8427e);
      stringval_d41d8cd98f00b204e9800998ecf8427e->AddRef();
      libpi::String* arg2(stringval_d41d8cd98f00b204e9800998ecf8427e);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg3(_this->var___SIM__console4);
      _this->var___SIM__from49->AddRef();
      libpi::String* arg4(_this->var___SIM__from49);
      _this->var___SIM__rcpt32->AddRef();
      libpi::String* arg5(_this->var___SIM__rcpt32);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetArg2( arg2);
      RemoveRef(arg2);
      _taskref->SetArg3( arg3);
      RemoveRef(arg3);
      _taskref->SetArg4( arg4);
      RemoveRef(arg4);
      _taskref->SetArg5( arg5);
      RemoveRef(arg5);
      _taskref->SetLabel(&&method___SIM__SmtpServerData6);

    }
    else
    { Task___SIM__SmtpServerData6 *__SIM__task557 = new Task___SIM__SmtpServerData6();
      __SIM__task557->SetWorker(&_task->GetWorker());
      _this->var___SIM__this31->AddRef();
      libpi::Session* arg0(_this->var___SIM__this31);
      stringval_d41d8cd98f00b204e9800998ecf8427e->AddRef();
      libpi::String* arg1(stringval_d41d8cd98f00b204e9800998ecf8427e);
      stringval_d41d8cd98f00b204e9800998ecf8427e->AddRef();
      libpi::String* arg2(stringval_d41d8cd98f00b204e9800998ecf8427e);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg3(_this->var___SIM__console4);
      _this->var___SIM__from49->AddRef();
      libpi::String* arg4(_this->var___SIM__from49);
      _this->var___SIM__rcpt32->AddRef();
      libpi::String* arg5(_this->var___SIM__rcpt32);
      __SIM__task557->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task557->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task557->SetArg2( arg2);
      RemoveRef(arg2);
      __SIM__task557->SetArg3( arg3);
      RemoveRef(arg3);
      __SIM__task557->SetArg4( arg4);
      RemoveRef(arg4);
      __SIM__task557->SetArg5( arg5);
      RemoveRef(arg5);
      __SIM__task557->SetLabel(&&method___SIM__SmtpServerData6);
      RemoveRef(_task);
      _task=__SIM__task557;
    }
    goto method___SIM__SmtpServerData6;
__SIM____SIM__SmtpServerMail5RCPT548:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_560);
      return true;
    }
__SIM__checkpoint_560:
    // ~this31[2] >> ~r70;
    _task->SetLabel(&&__SIM__receive559);
    if (!_this->var___SIM__this31->Receive(1,_task,(libpi::Value**)&_this->var___SIM__r70))
      return false;
    __SIM__receive559:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_562);
      return true;
    }
__SIM__checkpoint_562:
    // ~this31[2] << ^250;
    _this->var___SIM__this31->Send(1,_task,label_de62ae540e604d3ec7777b9c1adb33e6);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_564);
      return true;
    }
__SIM__checkpoint_564:
    if (dynamic_cast<Task___SIM__SmtpServerMail5*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpServerMail5* _taskref=(Task___SIM__SmtpServerMail5*)_task;
      _this->var___SIM__this31->AddRef();
      libpi::Session* arg0(_this->var___SIM__this31);
      _this->var___SIM__r70->AddRef();
      libpi::String* arg1(_this->var___SIM__r70);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg2(_this->var___SIM__console4);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetArg2( arg2);
      RemoveRef(arg2);
      _taskref->SetLabel(&&method___SIM__SmtpServerMail5);

    }
    else
    { Task___SIM__SmtpServerMail5 *__SIM__task563 = new Task___SIM__SmtpServerMail5();
      __SIM__task563->SetWorker(&_task->GetWorker());
      _this->var___SIM__this31->AddRef();
      libpi::Session* arg0(_this->var___SIM__this31);
      _this->var___SIM__r70->AddRef();
      libpi::String* arg1(_this->var___SIM__r70);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg2(_this->var___SIM__console4);
      __SIM__task563->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task563->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task563->SetArg2( arg2);
      RemoveRef(arg2);
      __SIM__task563->SetLabel(&&method___SIM__SmtpServerMail5);
      RemoveRef(_task);
      _task=__SIM__task563;
    }
    goto method___SIM__SmtpServerMail5;
  } // }}}
  method___SIM__SmtpServerData8: // {{{
  #undef _this
  #define _this ((Task___SIM__SmtpServerData8*)_task)
  {

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_569);
      return true;
    }
__SIM__checkpoint_569:
    _task->SetLabel(&&__SIM__checkpoint___SIM__SmtpServerData8568);
    if (!_this->var___SIM__this92->Receive(1,_task,&_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__SmtpServerData8568:
    if (((libpi::String*)_task->tmp)->GetValue()=="^DATALINE")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpServerData8DATALINE565);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^ENDOFDATA")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpServerData8ENDOFDATA566);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^SUBJECT")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpServerData8SUBJECT567);
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
    goto *_this->GetLabel();
__SIM____SIM__SmtpServerData8DATALINE565:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_571);
      return true;
    }
__SIM__checkpoint_571:
    // ~this92[2] >> ~line1107;
    _task->SetLabel(&&__SIM__receive570);
    if (!_this->var___SIM__this92->Receive(1,_task,(libpi::Value**)&_this->var___SIM__line1107))
      return false;
    __SIM__receive570:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_573);
      return true;
    }
__SIM__checkpoint_573:
    // ~this92[2] >> ~line2108;
    _task->SetLabel(&&__SIM__receive572);
    if (!_this->var___SIM__this92->Receive(1,_task,(libpi::Value**)&_this->var___SIM__line2108))
      return false;
    __SIM__receive572:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_575);
      return true;
    }
__SIM__checkpoint_575:
    if (dynamic_cast<Task___SIM__SmtpServerData8*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpServerData8* _taskref=(Task___SIM__SmtpServerData8*)_task;
      _this->var___SIM__this92->AddRef();
      libpi::Session* arg0(_this->var___SIM__this92);
      _this->var___SIM__subject93->AddRef();
      libpi::String* arg1(_this->var___SIM__subject93);
      _this->var___SIM__line1107->AddRef();
      libpi::String* arg2(_this->var___SIM__line1107);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg3(_this->var___SIM__console4);
      _this->var___SIM__from91->AddRef();
      libpi::String* arg4(_this->var___SIM__from91);
      _this->var___SIM__rcpt74->AddRef();
      libpi::String* arg5(_this->var___SIM__rcpt74);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetArg2( arg2);
      RemoveRef(arg2);
      _taskref->SetArg3( arg3);
      RemoveRef(arg3);
      _taskref->SetArg4( arg4);
      RemoveRef(arg4);
      _taskref->SetArg5( arg5);
      RemoveRef(arg5);
      _taskref->SetLabel(&&method___SIM__SmtpServerData8);

    }
    else
    { Task___SIM__SmtpServerData8 *__SIM__task574 = new Task___SIM__SmtpServerData8();
      __SIM__task574->SetWorker(&_task->GetWorker());
      _this->var___SIM__this92->AddRef();
      libpi::Session* arg0(_this->var___SIM__this92);
      _this->var___SIM__subject93->AddRef();
      libpi::String* arg1(_this->var___SIM__subject93);
      _this->var___SIM__line1107->AddRef();
      libpi::String* arg2(_this->var___SIM__line1107);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg3(_this->var___SIM__console4);
      _this->var___SIM__from91->AddRef();
      libpi::String* arg4(_this->var___SIM__from91);
      _this->var___SIM__rcpt74->AddRef();
      libpi::String* arg5(_this->var___SIM__rcpt74);
      __SIM__task574->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task574->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task574->SetArg2( arg2);
      RemoveRef(arg2);
      __SIM__task574->SetArg3( arg3);
      RemoveRef(arg3);
      __SIM__task574->SetArg4( arg4);
      RemoveRef(arg4);
      __SIM__task574->SetArg5( arg5);
      RemoveRef(arg5);
      __SIM__task574->SetLabel(&&method___SIM__SmtpServerData8);
      RemoveRef(_task);
      _task=__SIM__task574;
    }
    goto method___SIM__SmtpServerData8;
__SIM____SIM__SmtpServerData8ENDOFDATA566:
    // ~c109 = new ~console4(2 of 2)

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_576);
      return true;
    }
__SIM__checkpoint_576:
    { _task->tmp=new libpi::task::Channel();
      _this->var___SIM__console4->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__SmtpServerData8577);
      if (!((libpi::task::Channel*)_task->tmp)->Receive(_task,(libpi::Value**)&_this->var___SIM__c109))
        return false;
      __SIM__receive___SIM__SmtpServerData8577:
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_579);
      return true;
    }
__SIM__checkpoint_579:
    // ~c109[1] << ^str;
    _this->var___SIM__c109->Send(0,_task,label_cf6b4c5b94c4f75325166dbaeb4a0aae);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_581);
      return true;
    }
__SIM__checkpoint_581:
    // ~c109[1] << "Server received mail";
    { 
      stringval_9b054bbaf1ab61f371094ff82128f709->AddRef();
      _this->var___SIM__c109->Send(0,_task,stringval_9b054bbaf1ab61f371094ff82128f709);
      RemoveRef(stringval_9b054bbaf1ab61f371094ff82128f709);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_583);
      return true;
    }
__SIM__checkpoint_583:
    // ~c109[1] << ^nl;
    _this->var___SIM__c109->Send(0,_task,label_7a11449475519daf7b8b8621e5b6eacc);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_585);
      return true;
    }
__SIM__checkpoint_585:
    // ~c109[1] << ^str;
    _this->var___SIM__c109->Send(0,_task,label_cf6b4c5b94c4f75325166dbaeb4a0aae);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_587);
      return true;
    }
__SIM__checkpoint_587:
    // ~c109[1] << "FROM: ";
    { 
      stringval_d362dccf254336f6e31e2b35a396112e->AddRef();
      _this->var___SIM__c109->Send(0,_task,stringval_d362dccf254336f6e31e2b35a396112e);
      RemoveRef(stringval_d362dccf254336f6e31e2b35a396112e);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_589);
      return true;
    }
__SIM__checkpoint_589:
    // ~c109[1] << ^str;
    _this->var___SIM__c109->Send(0,_task,label_cf6b4c5b94c4f75325166dbaeb4a0aae);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_591);
      return true;
    }
__SIM__checkpoint_591:
    // ~c109[1] << ~from91;
    { 
      _this->var___SIM__from91->AddRef();
      _this->var___SIM__c109->Send(0,_task,_this->var___SIM__from91);
      RemoveRef(_this->var___SIM__from91);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_593);
      return true;
    }
__SIM__checkpoint_593:
    // ~c109[1] << ^nl;
    _this->var___SIM__c109->Send(0,_task,label_7a11449475519daf7b8b8621e5b6eacc);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_595);
      return true;
    }
__SIM__checkpoint_595:
    // ~c109[1] << ^str;
    _this->var___SIM__c109->Send(0,_task,label_cf6b4c5b94c4f75325166dbaeb4a0aae);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_597);
      return true;
    }
__SIM__checkpoint_597:
    // ~c109[1] << "RCPT: ";
    { 
      stringval_130365dbea325230ace43dbe736045bf->AddRef();
      _this->var___SIM__c109->Send(0,_task,stringval_130365dbea325230ace43dbe736045bf);
      RemoveRef(stringval_130365dbea325230ace43dbe736045bf);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_599);
      return true;
    }
__SIM__checkpoint_599:
    // ~c109[1] << ^str;
    _this->var___SIM__c109->Send(0,_task,label_cf6b4c5b94c4f75325166dbaeb4a0aae);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_601);
      return true;
    }
__SIM__checkpoint_601:
    // ~c109[1] << ~rcpt74;
    { 
      _this->var___SIM__rcpt74->AddRef();
      _this->var___SIM__c109->Send(0,_task,_this->var___SIM__rcpt74);
      RemoveRef(_this->var___SIM__rcpt74);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_603);
      return true;
    }
__SIM__checkpoint_603:
    // ~c109[1] << ^nl;
    _this->var___SIM__c109->Send(0,_task,label_7a11449475519daf7b8b8621e5b6eacc);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_605);
      return true;
    }
__SIM__checkpoint_605:
    // ~c109[1] << ^str;
    _this->var___SIM__c109->Send(0,_task,label_cf6b4c5b94c4f75325166dbaeb4a0aae);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_607);
      return true;
    }
__SIM__checkpoint_607:
    // ~c109[1] << "SUBJ: ";
    { 
      stringval_2f70e8bfd1389315a838ac269d90cb6e->AddRef();
      _this->var___SIM__c109->Send(0,_task,stringval_2f70e8bfd1389315a838ac269d90cb6e);
      RemoveRef(stringval_2f70e8bfd1389315a838ac269d90cb6e);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_609);
      return true;
    }
__SIM__checkpoint_609:
    // ~c109[1] << ^str;
    _this->var___SIM__c109->Send(0,_task,label_cf6b4c5b94c4f75325166dbaeb4a0aae);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_611);
      return true;
    }
__SIM__checkpoint_611:
    // ~c109[1] << ~subject93;
    { 
      _this->var___SIM__subject93->AddRef();
      _this->var___SIM__c109->Send(0,_task,_this->var___SIM__subject93);
      RemoveRef(_this->var___SIM__subject93);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_613);
      return true;
    }
__SIM__checkpoint_613:
    // ~c109[1] << ^nl;
    _this->var___SIM__c109->Send(0,_task,label_7a11449475519daf7b8b8621e5b6eacc);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_615);
      return true;
    }
__SIM__checkpoint_615:
    // ~c109[1] << ^end;
    _this->var___SIM__c109->Send(0,_task,label_186e35ef3dd9939615c442c30b801a33);
    _this->var___SIM__c109->Close(true);
    RemoveRef(_this->var___SIM__c109);
    _this->var___SIM__c109=NULL;

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_617);
      return true;
    }
__SIM__checkpoint_617:
    if (dynamic_cast<Task___SIM__SmtpServerAuth4*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpServerAuth4* _taskref=(Task___SIM__SmtpServerAuth4*)_task;
      _this->var___SIM__this92->AddRef();
      libpi::Session* arg0(_this->var___SIM__this92);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetLabel(&&method___SIM__SmtpServerAuth4);

    }
    else
    { Task___SIM__SmtpServerAuth4 *__SIM__task616 = new Task___SIM__SmtpServerAuth4();
      __SIM__task616->SetWorker(&_task->GetWorker());
      _this->var___SIM__this92->AddRef();
      libpi::Session* arg0(_this->var___SIM__this92);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      __SIM__task616->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task616->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task616->SetLabel(&&method___SIM__SmtpServerAuth4);
      RemoveRef(_task);
      _task=__SIM__task616;
    }
    goto method___SIM__SmtpServerAuth4;
__SIM____SIM__SmtpServerData8SUBJECT567:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_619);
      return true;
    }
__SIM__checkpoint_619:
    // ~this92[2] >> ~s1110;
    _task->SetLabel(&&__SIM__receive618);
    if (!_this->var___SIM__this92->Receive(1,_task,(libpi::Value**)&_this->var___SIM__s1110))
      return false;
    __SIM__receive618:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_621);
      return true;
    }
__SIM__checkpoint_621:
    // ~this92[2] >> ~s2111;
    _task->SetLabel(&&__SIM__receive620);
    if (!_this->var___SIM__this92->Receive(1,_task,(libpi::Value**)&_this->var___SIM__s2111))
      return false;
    __SIM__receive620:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_623);
      return true;
    }
__SIM__checkpoint_623:
    if (dynamic_cast<Task___SIM__SmtpServerData8*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpServerData8* _taskref=(Task___SIM__SmtpServerData8*)_task;
      _this->var___SIM__this92->AddRef();
      libpi::Session* arg0(_this->var___SIM__this92);
      _this->var___SIM__s1110->AddRef();
      libpi::String* arg1(_this->var___SIM__s1110);
      _this->var___SIM__data94->AddRef();
      libpi::String* arg2(_this->var___SIM__data94);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg3(_this->var___SIM__console4);
      _this->var___SIM__from91->AddRef();
      libpi::String* arg4(_this->var___SIM__from91);
      _this->var___SIM__rcpt74->AddRef();
      libpi::String* arg5(_this->var___SIM__rcpt74);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetArg2( arg2);
      RemoveRef(arg2);
      _taskref->SetArg3( arg3);
      RemoveRef(arg3);
      _taskref->SetArg4( arg4);
      RemoveRef(arg4);
      _taskref->SetArg5( arg5);
      RemoveRef(arg5);
      _taskref->SetLabel(&&method___SIM__SmtpServerData8);

    }
    else
    { Task___SIM__SmtpServerData8 *__SIM__task622 = new Task___SIM__SmtpServerData8();
      __SIM__task622->SetWorker(&_task->GetWorker());
      _this->var___SIM__this92->AddRef();
      libpi::Session* arg0(_this->var___SIM__this92);
      _this->var___SIM__s1110->AddRef();
      libpi::String* arg1(_this->var___SIM__s1110);
      _this->var___SIM__data94->AddRef();
      libpi::String* arg2(_this->var___SIM__data94);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg3(_this->var___SIM__console4);
      _this->var___SIM__from91->AddRef();
      libpi::String* arg4(_this->var___SIM__from91);
      _this->var___SIM__rcpt74->AddRef();
      libpi::String* arg5(_this->var___SIM__rcpt74);
      __SIM__task622->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task622->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task622->SetArg2( arg2);
      RemoveRef(arg2);
      __SIM__task622->SetArg3( arg3);
      RemoveRef(arg3);
      __SIM__task622->SetArg4( arg4);
      RemoveRef(arg4);
      __SIM__task622->SetArg5( arg5);
      RemoveRef(arg5);
      __SIM__task622->SetLabel(&&method___SIM__SmtpServerData8);
      RemoveRef(_task);
      _task=__SIM__task622;
    }
    goto method___SIM__SmtpServerData8;
  } // }}}
  method___SIM__SmtpServerMail7: // {{{
  #undef _this
  #define _this ((Task___SIM__SmtpServerMail7*)_task)
  {

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_627);
      return true;
    }
__SIM__checkpoint_627:
    _task->SetLabel(&&__SIM__checkpoint___SIM__SmtpServerMail7626);
    if (!_this->var___SIM__this73->Receive(1,_task,&_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__SmtpServerMail7626:
    if (((libpi::String*)_task->tmp)->GetValue()=="^DATA")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpServerMail7DATA624);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^RCPT")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpServerMail7RCPT625);
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
    goto *_this->GetLabel();
__SIM____SIM__SmtpServerMail7DATA624:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_629);
      return true;
    }
__SIM__checkpoint_629:
    // ~this73[2] << ^354;
    _this->var___SIM__this73->Send(1,_task,label_a4850a01ffe59cb572e05301fd3a503f);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_631);
      return true;
    }
__SIM__checkpoint_631:
    // ~this73[2] >> ~to90;
    _task->SetLabel(&&__SIM__receive630);
    if (!_this->var___SIM__this73->Receive(1,_task,(libpi::Value**)&_this->var___SIM__to90))
      return false;
    __SIM__receive630:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_633);
      return true;
    }
__SIM__checkpoint_633:
    // ~this73[2] >> ~from91;
    _task->SetLabel(&&__SIM__receive632);
    if (!_this->var___SIM__this73->Receive(1,_task,(libpi::Value**)&_this->var___SIM__from91))
      return false;
    __SIM__receive632:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_635);
      return true;
    }
__SIM__checkpoint_635:
    if (dynamic_cast<Task___SIM__SmtpServerData8*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpServerData8* _taskref=(Task___SIM__SmtpServerData8*)_task;
      _this->var___SIM__this73->AddRef();
      libpi::Session* arg0(_this->var___SIM__this73);
      stringval_d41d8cd98f00b204e9800998ecf8427e->AddRef();
      libpi::String* arg1(stringval_d41d8cd98f00b204e9800998ecf8427e);
      stringval_d41d8cd98f00b204e9800998ecf8427e->AddRef();
      libpi::String* arg2(stringval_d41d8cd98f00b204e9800998ecf8427e);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg3(_this->var___SIM__console4);
      _this->var___SIM__from91->AddRef();
      libpi::String* arg4(_this->var___SIM__from91);
      _this->var___SIM__rcpt74->AddRef();
      libpi::String* arg5(_this->var___SIM__rcpt74);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetArg2( arg2);
      RemoveRef(arg2);
      _taskref->SetArg3( arg3);
      RemoveRef(arg3);
      _taskref->SetArg4( arg4);
      RemoveRef(arg4);
      _taskref->SetArg5( arg5);
      RemoveRef(arg5);
      _taskref->SetLabel(&&method___SIM__SmtpServerData8);

    }
    else
    { Task___SIM__SmtpServerData8 *__SIM__task634 = new Task___SIM__SmtpServerData8();
      __SIM__task634->SetWorker(&_task->GetWorker());
      _this->var___SIM__this73->AddRef();
      libpi::Session* arg0(_this->var___SIM__this73);
      stringval_d41d8cd98f00b204e9800998ecf8427e->AddRef();
      libpi::String* arg1(stringval_d41d8cd98f00b204e9800998ecf8427e);
      stringval_d41d8cd98f00b204e9800998ecf8427e->AddRef();
      libpi::String* arg2(stringval_d41d8cd98f00b204e9800998ecf8427e);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg3(_this->var___SIM__console4);
      _this->var___SIM__from91->AddRef();
      libpi::String* arg4(_this->var___SIM__from91);
      _this->var___SIM__rcpt74->AddRef();
      libpi::String* arg5(_this->var___SIM__rcpt74);
      __SIM__task634->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task634->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task634->SetArg2( arg2);
      RemoveRef(arg2);
      __SIM__task634->SetArg3( arg3);
      RemoveRef(arg3);
      __SIM__task634->SetArg4( arg4);
      RemoveRef(arg4);
      __SIM__task634->SetArg5( arg5);
      RemoveRef(arg5);
      __SIM__task634->SetLabel(&&method___SIM__SmtpServerData8);
      RemoveRef(_task);
      _task=__SIM__task634;
    }
    goto method___SIM__SmtpServerData8;
__SIM____SIM__SmtpServerMail7RCPT625:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_637);
      return true;
    }
__SIM__checkpoint_637:
    // ~this73[2] >> ~r112;
    _task->SetLabel(&&__SIM__receive636);
    if (!_this->var___SIM__this73->Receive(1,_task,(libpi::Value**)&_this->var___SIM__r112))
      return false;
    __SIM__receive636:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_639);
      return true;
    }
__SIM__checkpoint_639:
    // ~this73[2] << ^250;
    _this->var___SIM__this73->Send(1,_task,label_de62ae540e604d3ec7777b9c1adb33e6);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_641);
      return true;
    }
__SIM__checkpoint_641:
    if (dynamic_cast<Task___SIM__SmtpServerMail7*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpServerMail7* _taskref=(Task___SIM__SmtpServerMail7*)_task;
      _this->var___SIM__this73->AddRef();
      libpi::Session* arg0(_this->var___SIM__this73);
      _this->var___SIM__r112->AddRef();
      libpi::String* arg1(_this->var___SIM__r112);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg2(_this->var___SIM__console4);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetArg2( arg2);
      RemoveRef(arg2);
      _taskref->SetLabel(&&method___SIM__SmtpServerMail7);

    }
    else
    { Task___SIM__SmtpServerMail7 *__SIM__task640 = new Task___SIM__SmtpServerMail7();
      __SIM__task640->SetWorker(&_task->GetWorker());
      _this->var___SIM__this73->AddRef();
      libpi::Session* arg0(_this->var___SIM__this73);
      _this->var___SIM__r112->AddRef();
      libpi::String* arg1(_this->var___SIM__r112);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg2(_this->var___SIM__console4);
      __SIM__task640->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task640->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task640->SetArg2( arg2);
      RemoveRef(arg2);
      __SIM__task640->SetLabel(&&method___SIM__SmtpServerMail7);
      RemoveRef(_task);
      _task=__SIM__task640;
    }
    goto method___SIM__SmtpServerMail7;
  } // }}}
  method___SIM__SmtpServerAuth4: // {{{
  #undef _this
  #define _this ((Task___SIM__SmtpServerAuth4*)_task)
  {

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_645);
      return true;
    }
__SIM__checkpoint_645:
    _task->SetLabel(&&__SIM__checkpoint___SIM__SmtpServerAuth4644);
    if (!_this->var___SIM__this20->Receive(1,_task,&_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__SmtpServerAuth4644:
    if (((libpi::String*)_task->tmp)->GetValue()=="^MAIL")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpServerAuth4MAIL642);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^QUIT")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->var___SIM__this20->Close(true);
      RemoveRef(_this->var___SIM__this20);
      _this->var___SIM__this20=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpServerAuth4QUIT643);
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
    goto *_this->GetLabel();
__SIM____SIM__SmtpServerAuth4MAIL642:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_648);
      return true;
    }
__SIM__checkpoint_648:
    {
      libpi::Int* __SIM__systemexp650(new libpi::Int(libpi::task::Worker::TargetTasks));
      libpi::Int* __SIM__systemexp651(new libpi::Int(libpi::task::Worker::ActiveTasks));
      bool __SIM__binop649((*__SIM__systemexp650) <= (*__SIM__systemexp651));
      RemoveRef(__SIM__systemexp650);
      RemoveRef(__SIM__systemexp651);
      if (__SIM__binop649)
        _task->SetLabel(&&__SIM____SIM__SmtpServerAuth4_true646);
      else
        _task->SetLabel(&&__SIM____SIM__SmtpServerAuth4_false647);
    }
    goto *_task->GetLabel();
    __SIM____SIM__SmtpServerAuth4_true646:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_653);
      return true;
    }
__SIM__checkpoint_653:
    // ~this20[2] >> ~from29;
    _task->SetLabel(&&__SIM__receive652);
    if (!_this->var___SIM__this20->Receive(1,_task,(libpi::Value**)&_this->var___SIM__from29))
      return false;
    __SIM__receive652:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_654);
      return true;
    }
__SIM__checkpoint_654:
    {
      _this->var___SIM__valid_from30=true;
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_657);
      return true;
    }
__SIM__checkpoint_657:
    {
      if (_this->var___SIM__valid_from30)
        _task->SetLabel(&&__SIM____SIM__SmtpServerAuth4_true655);
      else
        _task->SetLabel(&&__SIM____SIM__SmtpServerAuth4_false656);
    }
    goto *_task->GetLabel();
    __SIM____SIM__SmtpServerAuth4_true655:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_659);
      return true;
    }
__SIM__checkpoint_659:
    // ~this20[2] << ^250;
    _this->var___SIM__this20->Send(1,_task,label_de62ae540e604d3ec7777b9c1adb33e6);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_661);
      return true;
    }
__SIM__checkpoint_661:
    if (dynamic_cast<Task___SIM__SmtpServerMail5*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpServerMail5* _taskref=(Task___SIM__SmtpServerMail5*)_task;
      _this->var___SIM__this20->AddRef();
      libpi::Session* arg0(_this->var___SIM__this20);
      stringval_d41d8cd98f00b204e9800998ecf8427e->AddRef();
      libpi::String* arg1(stringval_d41d8cd98f00b204e9800998ecf8427e);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg2(_this->var___SIM__console4);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetArg2( arg2);
      RemoveRef(arg2);
      _taskref->SetLabel(&&method___SIM__SmtpServerMail5);

    }
    else
    { Task___SIM__SmtpServerMail5 *__SIM__task660 = new Task___SIM__SmtpServerMail5();
      __SIM__task660->SetWorker(&_task->GetWorker());
      _this->var___SIM__this20->AddRef();
      libpi::Session* arg0(_this->var___SIM__this20);
      stringval_d41d8cd98f00b204e9800998ecf8427e->AddRef();
      libpi::String* arg1(stringval_d41d8cd98f00b204e9800998ecf8427e);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg2(_this->var___SIM__console4);
      __SIM__task660->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task660->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task660->SetArg2( arg2);
      RemoveRef(arg2);
      __SIM__task660->SetLabel(&&method___SIM__SmtpServerMail5);
      RemoveRef(_task);
      _task=__SIM__task660;
    }
    goto method___SIM__SmtpServerMail5;
    __SIM____SIM__SmtpServerAuth4_false656:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_663);
      return true;
    }
__SIM__checkpoint_663:
    // ~this20[2] << ^501;
    _this->var___SIM__this20->Send(1,_task,label_bb0fe4de77e646ccff635026ab590e70);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_665);
      return true;
    }
__SIM__checkpoint_665:
    if (dynamic_cast<Task___SIM__SmtpServerAuth4*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpServerAuth4* _taskref=(Task___SIM__SmtpServerAuth4*)_task;
      _this->var___SIM__this20->AddRef();
      libpi::Session* arg0(_this->var___SIM__this20);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetLabel(&&method___SIM__SmtpServerAuth4);

    }
    else
    { Task___SIM__SmtpServerAuth4 *__SIM__task664 = new Task___SIM__SmtpServerAuth4();
      __SIM__task664->SetWorker(&_task->GetWorker());
      _this->var___SIM__this20->AddRef();
      libpi::Session* arg0(_this->var___SIM__this20);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      __SIM__task664->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task664->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task664->SetLabel(&&method___SIM__SmtpServerAuth4);
      RemoveRef(_task);
      _task=__SIM__task664;
    }
    goto method___SIM__SmtpServerAuth4;
    __SIM____SIM__SmtpServerAuth4_false647:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_666);
      return true;
    }
__SIM__checkpoint_666:
    {
      _this->var___SIM__valid_from71=true;
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_668);
      return true;
    }
__SIM__checkpoint_668:
    // ~this20[2] >> ~from72;
    _task->SetLabel(&&__SIM__receive667);
    if (!_this->var___SIM__this20->Receive(1,_task,(libpi::Value**)&_this->var___SIM__from72))
      return false;
    __SIM__receive667:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_671);
      return true;
    }
__SIM__checkpoint_671:
    {
      if (_this->var___SIM__valid_from71)
        _task->SetLabel(&&__SIM____SIM__SmtpServerAuth4_true669);
      else
        _task->SetLabel(&&__SIM____SIM__SmtpServerAuth4_false670);
    }
    goto *_task->GetLabel();
    __SIM____SIM__SmtpServerAuth4_true669:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_673);
      return true;
    }
__SIM__checkpoint_673:
    // ~this20[2] << ^250;
    _this->var___SIM__this20->Send(1,_task,label_de62ae540e604d3ec7777b9c1adb33e6);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_675);
      return true;
    }
__SIM__checkpoint_675:
    if (dynamic_cast<Task___SIM__SmtpServerMail7*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpServerMail7* _taskref=(Task___SIM__SmtpServerMail7*)_task;
      _this->var___SIM__this20->AddRef();
      libpi::Session* arg0(_this->var___SIM__this20);
      stringval_d41d8cd98f00b204e9800998ecf8427e->AddRef();
      libpi::String* arg1(stringval_d41d8cd98f00b204e9800998ecf8427e);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg2(_this->var___SIM__console4);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetArg2( arg2);
      RemoveRef(arg2);
      _taskref->SetLabel(&&method___SIM__SmtpServerMail7);

    }
    else
    { Task___SIM__SmtpServerMail7 *__SIM__task674 = new Task___SIM__SmtpServerMail7();
      __SIM__task674->SetWorker(&_task->GetWorker());
      _this->var___SIM__this20->AddRef();
      libpi::Session* arg0(_this->var___SIM__this20);
      stringval_d41d8cd98f00b204e9800998ecf8427e->AddRef();
      libpi::String* arg1(stringval_d41d8cd98f00b204e9800998ecf8427e);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg2(_this->var___SIM__console4);
      __SIM__task674->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task674->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task674->SetArg2( arg2);
      RemoveRef(arg2);
      __SIM__task674->SetLabel(&&method___SIM__SmtpServerMail7);
      RemoveRef(_task);
      _task=__SIM__task674;
    }
    goto method___SIM__SmtpServerMail7;
    __SIM____SIM__SmtpServerAuth4_false670:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_677);
      return true;
    }
__SIM__checkpoint_677:
    // ~this20[2] << ^501;
    _this->var___SIM__this20->Send(1,_task,label_bb0fe4de77e646ccff635026ab590e70);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_679);
      return true;
    }
__SIM__checkpoint_679:
    if (dynamic_cast<Task___SIM__SmtpServerAuth4*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpServerAuth4* _taskref=(Task___SIM__SmtpServerAuth4*)_task;
      _this->var___SIM__this20->AddRef();
      libpi::Session* arg0(_this->var___SIM__this20);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetLabel(&&method___SIM__SmtpServerAuth4);

    }
    else
    { Task___SIM__SmtpServerAuth4 *__SIM__task678 = new Task___SIM__SmtpServerAuth4();
      __SIM__task678->SetWorker(&_task->GetWorker());
      _this->var___SIM__this20->AddRef();
      libpi::Session* arg0(_this->var___SIM__this20);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      __SIM__task678->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task678->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task678->SetLabel(&&method___SIM__SmtpServerAuth4);
      RemoveRef(_task);
      _task=__SIM__task678;
    }
    goto method___SIM__SmtpServerAuth4;
__SIM____SIM__SmtpServerAuth4QUIT643:
    RemoveRef(_task);
    return false;

  } // }}}
  method___SIM__FlatRight12: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight12*)_task)
  {

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_681);
      return true;
    }
__SIM__checkpoint_681:
    // ~this18[2] << ^220_SERVICE_READY;
    _this->var___SIM__this18->Send(1,_task,label_e126287453c75224938478719b76cbdf);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_683);
      return true;
    }
__SIM__checkpoint_683:
    // ~this18[2] << "Welcome to the SMTP service!";
    { 
      stringval_1d32166a901c92f5595c02ca74f9a49a->AddRef();
      _this->var___SIM__this18->Send(1,_task,stringval_1d32166a901c92f5595c02ca74f9a49a);
      RemoveRef(stringval_1d32166a901c92f5595c02ca74f9a49a);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_687);
      return true;
    }
__SIM__checkpoint_687:
    _task->SetLabel(&&__SIM__checkpoint___SIM__FlatRight12686);
    if (!_this->var___SIM__this18->Receive(1,_task,&_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__FlatRight12686:
    if (((libpi::String*)_task->tmp)->GetValue()=="^EHLO")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__FlatRight12EHLO684);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^QUIT")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->var___SIM__this18->Close(true);
      RemoveRef(_this->var___SIM__this18);
      _this->var___SIM__this18=NULL;
      _this->SetLabel(&&__SIM____SIM__FlatRight12QUIT685);
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
    goto *_this->GetLabel();
__SIM____SIM__FlatRight12EHLO684:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_689);
      return true;
    }
__SIM__checkpoint_689:
    // ~this18[2] >> ~msg19;
    _task->SetLabel(&&__SIM__receive688);
    if (!_this->var___SIM__this18->Receive(1,_task,(libpi::Value**)&_this->var___SIM__msg19))
      return false;
    __SIM__receive688:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_691);
      return true;
    }
__SIM__checkpoint_691:
    // ~this18[2] << ^250;
    _this->var___SIM__this18->Send(1,_task,label_de62ae540e604d3ec7777b9c1adb33e6);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_693);
      return true;
    }
__SIM__checkpoint_693:
    // ~this18[2] << "smtpservice.pi";
    { 
      stringval_3adb336c0fd73a169b4af87a5cc99fa2->AddRef();
      _this->var___SIM__this18->Send(1,_task,stringval_3adb336c0fd73a169b4af87a5cc99fa2);
      RemoveRef(stringval_3adb336c0fd73a169b4af87a5cc99fa2);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_697);
      return true;
    }
__SIM__checkpoint_697:
    _task->SetLabel(&&__SIM__checkpoint___SIM__FlatRight12696);
    if (!_this->var___SIM__this18->Receive(1,_task,&_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__FlatRight12696:
    if (((libpi::String*)_task->tmp)->GetValue()=="^QUIT")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->var___SIM__this18->Close(true);
      RemoveRef(_this->var___SIM__this18);
      _this->var___SIM__this18=NULL;
      _this->SetLabel(&&__SIM____SIM__FlatRight12QUIT694);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^StartTLS")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__FlatRight12StartTLS695);
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
    goto *_this->GetLabel();
__SIM____SIM__FlatRight12QUIT694:
    RemoveRef(_task);
    return false;

__SIM____SIM__FlatRight12StartTLS695:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_699);
      return true;
    }
__SIM__checkpoint_699:
    // ~this18[2] << ^220;
    _this->var___SIM__this18->Send(1,_task,label_321f0bf458ee529a49c77f94eab195af);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_703);
      return true;
    }
__SIM__checkpoint_703:
    _task->SetLabel(&&__SIM__checkpoint___SIM__FlatRight12702);
    if (!_this->var___SIM__this18->Receive(1,_task,&_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__FlatRight12702:
    if (((libpi::String*)_task->tmp)->GetValue()=="^EHLO")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__FlatRight12EHLO700);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^QUIT")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->var___SIM__this18->Close(true);
      RemoveRef(_this->var___SIM__this18);
      _this->var___SIM__this18=NULL;
      _this->SetLabel(&&__SIM____SIM__FlatRight12QUIT701);
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
    goto *_this->GetLabel();
__SIM____SIM__FlatRight12EHLO700:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_705);
      return true;
    }
__SIM__checkpoint_705:
    // ~this18[2] << ^250;
    _this->var___SIM__this18->Send(1,_task,label_de62ae540e604d3ec7777b9c1adb33e6);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_707);
      return true;
    }
__SIM__checkpoint_707:
    // ~this18[2] << "smtpservice.pi";
    { 
      stringval_3adb336c0fd73a169b4af87a5cc99fa2->AddRef();
      _this->var___SIM__this18->Send(1,_task,stringval_3adb336c0fd73a169b4af87a5cc99fa2);
      RemoveRef(stringval_3adb336c0fd73a169b4af87a5cc99fa2);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_711);
      return true;
    }
__SIM__checkpoint_711:
    _task->SetLabel(&&__SIM__checkpoint___SIM__FlatRight12710);
    if (!_this->var___SIM__this18->Receive(1,_task,&_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__FlatRight12710:
    if (((libpi::String*)_task->tmp)->GetValue()=="^AUTH")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__FlatRight12AUTH708);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^QUIT")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->var___SIM__this18->Close(true);
      RemoveRef(_this->var___SIM__this18);
      _this->var___SIM__this18=NULL;
      _this->SetLabel(&&__SIM____SIM__FlatRight12QUIT709);
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
    goto *_this->GetLabel();
__SIM____SIM__FlatRight12AUTH708:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_713);
      return true;
    }
__SIM__checkpoint_713:
    // ~this18[2] << ^235;
    _this->var___SIM__this18->Send(1,_task,label_22b095d07ef97cc2cea12609d3bba397);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_715);
      return true;
    }
__SIM__checkpoint_715:
    if (dynamic_cast<Task___SIM__SmtpServerAuth4*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpServerAuth4* _taskref=(Task___SIM__SmtpServerAuth4*)_task;
      _this->var___SIM__this18->AddRef();
      libpi::Session* arg0(_this->var___SIM__this18);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetLabel(&&method___SIM__SmtpServerAuth4);

    }
    else
    { Task___SIM__SmtpServerAuth4 *__SIM__task714 = new Task___SIM__SmtpServerAuth4();
      __SIM__task714->SetWorker(&_task->GetWorker());
      _this->var___SIM__this18->AddRef();
      libpi::Session* arg0(_this->var___SIM__this18);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      __SIM__task714->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task714->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task714->SetLabel(&&method___SIM__SmtpServerAuth4);
      RemoveRef(_task);
      _task=__SIM__task714;
    }
    goto method___SIM__SmtpServerAuth4;
__SIM____SIM__FlatRight12QUIT709:
    RemoveRef(_task);
    return false;

__SIM____SIM__FlatRight12QUIT701:
    RemoveRef(_task);
    return false;

__SIM____SIM__FlatRight12QUIT685:
    RemoveRef(_task);
    return false;

  } // }}}
  method___SIM__SmtpServer3: // {{{
  #undef _this
  #define _this ((Task___SIM__SmtpServer3*)_task)
  {
    // ~this18 = new ~smtp17(1 of 2)

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_716);
      return true;
    }
__SIM__checkpoint_716:
    { // link
      _task->tmps.clear();
        _task->SetLabel(&&__SIM__receive___SIM__SmtpServer3717);
        if (!_this->var___SIM__smtp17->GetChannels()[0]->Receive(_task,&_task->tmp))
          return false;
        __SIM__receive___SIM__SmtpServer3717:
        _task->tmps.push_back(_task->tmp);
      vector<vector<libpi::task::Channel*> > inChannels(2);
      vector<vector<libpi::task::Channel*> > outChannels(2);
      // Optimize vector inserts
        inChannels[0].resize(2);
        outChannels[0].resize(2);
        inChannels[1].resize(2);
        outChannels[1].resize(2);
      // Create channels
      inChannels[0][0]=new libpi::task::Channel();
      inChannels[0][1]=new libpi::task::Channel();
      inChannels[1][0]=dynamic_cast<libpi::task::Channel*>(_task->tmps[0]);
      inChannels[1][1]=new libpi::task::Channel();
      outChannels[0][0]=inChannels[0][0];
      outChannels[0][1]=inChannels[1][0];
      outChannels[1][0]=inChannels[0][1];
      outChannels[1][1]=inChannels[1][1];
      // Send sessions
      { libpi::Session* _s(new libpi::Session(1,2, inChannels[1], outChannels[1]));
        ((libpi::task::Channel*)_task->tmps[0])->Send(_task,_s);
        RemoveRef(_s);
      }
      // Create local session
      AssignNewValue((libpi::Value**)&_this->var___SIM__this18,new libpi::Session(0,2,inChannels[0],outChannels[0]));
      // Clean up
      while (!inChannels.empty())
      { while (!inChannels.back().empty())
        { RemoveRef(inChannels.back().back());
          inChannels.back().pop_back();
        }
        inChannels.pop_back();
      }
      // outChannels refs inChannels and should not be unreferenced
      _task->tmps.clear();
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_718);
      return true;
    }
__SIM__checkpoint_718:
    { Task___SIM__FlatRight12 *__SIM__task719(new Task___SIM__FlatRight12());
      __SIM__task719->SetWorker(&_task->GetWorker());
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg0(_this->var___SIM__console4);
      _this->var___SIM__this18->AddRef();
      libpi::Session* arg1(_this->var___SIM__this18);
      __SIM__task719->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task719->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task719->SetLabel(&&method___SIM__FlatRight12);
      _task->GetWorker().AddTask(__SIM__task719);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_721);
      return true;
    }
__SIM__checkpoint_721:
    if (dynamic_cast<Task___SIM__SmtpServer3*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpServer3* _taskref=(Task___SIM__SmtpServer3*)_task;
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg0(_this->var___SIM__console4);
      _this->var___SIM__smtp17->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__smtp17);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetLabel(&&method___SIM__SmtpServer3);

    }
    else
    { Task___SIM__SmtpServer3 *__SIM__task720 = new Task___SIM__SmtpServer3();
      __SIM__task720->SetWorker(&_task->GetWorker());
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg0(_this->var___SIM__console4);
      _this->var___SIM__smtp17->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__smtp17);
      __SIM__task720->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task720->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task720->SetLabel(&&method___SIM__SmtpServer3);
      RemoveRef(_task);
      _task=__SIM__task720;
    }
    goto method___SIM__SmtpServer3;
  } // }}}
  method___SIM__StartService9: // {{{
  #undef _this
  #define _this ((Task___SIM__StartService9*)_task)
  {

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_724);
      return true;
    }
__SIM__checkpoint_724:
    {
      _this->var___SIM__i113->AddRef();
      intval_0->AddRef();
      bool __SIM__binop725((*_this->var___SIM__i113) <= (*intval_0));
      RemoveRef(_this->var___SIM__i113);
      RemoveRef(intval_0);
      if (__SIM__binop725)
        _task->SetLabel(&&__SIM____SIM__StartService9_true722);
      else
        _task->SetLabel(&&__SIM____SIM__StartService9_false723);
    }
    goto *_task->GetLabel();
    __SIM____SIM__StartService9_true722:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_727);
      return true;
    }
__SIM__checkpoint_727:
    if (dynamic_cast<Task___SIM__SmtpServer3*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpServer3* _taskref=(Task___SIM__SmtpServer3*)_task;
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg0(_this->var___SIM__console4);
      _this->var___SIM__smtp17->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__smtp17);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetLabel(&&method___SIM__SmtpServer3);

    }
    else
    { Task___SIM__SmtpServer3 *__SIM__task726 = new Task___SIM__SmtpServer3();
      __SIM__task726->SetWorker(&_task->GetWorker());
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg0(_this->var___SIM__console4);
      _this->var___SIM__smtp17->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__smtp17);
      __SIM__task726->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task726->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task726->SetLabel(&&method___SIM__SmtpServer3);
      RemoveRef(_task);
      _task=__SIM__task726;
    }
    goto method___SIM__SmtpServer3;
    __SIM____SIM__StartService9_false723:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_728);
      return true;
    }
__SIM__checkpoint_728:
    { Task___SIM__StartService9 *__SIM__task729(new Task___SIM__StartService9());
      __SIM__task729->SetWorker(&_task->GetWorker());
      _this->var___SIM__i113->AddRef();
      intval_1->AddRef();
      libpi::Int* __SIM__binop730((*_this->var___SIM__i113) - (*intval_1));
      RemoveRef(_this->var___SIM__i113);
      RemoveRef(intval_1);
      libpi::Int* arg0(__SIM__binop730);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      _this->var___SIM__smtp17->AddRef();
      libpi::task::Link* arg2(_this->var___SIM__smtp17);
      __SIM__task729->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task729->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task729->SetArg2( arg2);
      RemoveRef(arg2);
      __SIM__task729->SetLabel(&&method___SIM__StartService9);
      _task->GetWorker().AddTask(__SIM__task729);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_732);
      return true;
    }
__SIM__checkpoint_732:
    if (dynamic_cast<Task___SIM__SmtpServer3*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpServer3* _taskref=(Task___SIM__SmtpServer3*)_task;
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg0(_this->var___SIM__console4);
      _this->var___SIM__smtp17->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__smtp17);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetLabel(&&method___SIM__SmtpServer3);

    }
    else
    { Task___SIM__SmtpServer3 *__SIM__task731 = new Task___SIM__SmtpServer3();
      __SIM__task731->SetWorker(&_task->GetWorker());
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg0(_this->var___SIM__console4);
      _this->var___SIM__smtp17->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__smtp17);
      __SIM__task731->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task731->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task731->SetLabel(&&method___SIM__SmtpServer3);
      RemoveRef(_task);
      _task=__SIM__task731;
    }
    goto method___SIM__SmtpServer3;
  } // }}}
  method___SIM__SmtpClientTls11: // {{{
  #undef _this
  #define _this ((Task___SIM__SmtpClientTls11*)_task)
  {

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_736);
      return true;
    }
__SIM__checkpoint_736:
    _task->SetLabel(&&__SIM__checkpoint___SIM__SmtpClientTls11735);
    if (!_this->var___SIM__this128->Receive(0,_task,&_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__SmtpClientTls11735:
    if (((libpi::String*)_task->tmp)->GetValue()=="^250")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpClientTls11250733);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^250d")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpClientTls11250d734);
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
    goto *_this->GetLabel();
__SIM____SIM__SmtpClientTls11250733:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_738);
      return true;
    }
__SIM__checkpoint_738:
    // ~this128[1] >> ~srv138;
    _task->SetLabel(&&__SIM__receive737);
    if (!_this->var___SIM__this128->Receive(0,_task,(libpi::Value**)&_this->var___SIM__srv138))
      return false;
    __SIM__receive737:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_740);
      return true;
    }
__SIM__checkpoint_740:
    // ~this128[1] << ^AUTH;
    _this->var___SIM__this128->Send(0,_task,label_969550f1d743109cb4f91353e73192be);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_744);
      return true;
    }
__SIM__checkpoint_744:
    _task->SetLabel(&&__SIM__checkpoint___SIM__SmtpClientTls11743);
    if (!_this->var___SIM__this128->Receive(0,_task,&_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__SmtpClientTls11743:
    if (((libpi::String*)_task->tmp)->GetValue()=="^235")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpClientTls11235741);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^535")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpClientTls11535742);
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
    goto *_this->GetLabel();
__SIM____SIM__SmtpClientTls11235741:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_746);
      return true;
    }
__SIM__checkpoint_746:
    // ~this128[1] << ^MAIL;
    _this->var___SIM__this128->Send(0,_task,label_fc80f3e0cea28c4c6e7d5690125a7c85);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_748);
      return true;
    }
__SIM__checkpoint_748:
    // ~this128[1] << "user@smtpserver.pi";
    { 
      stringval_42e6fae527820c9a8ce6bf7a97074aac->AddRef();
      _this->var___SIM__this128->Send(0,_task,stringval_42e6fae527820c9a8ce6bf7a97074aac);
      RemoveRef(stringval_42e6fae527820c9a8ce6bf7a97074aac);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_752);
      return true;
    }
__SIM__checkpoint_752:
    _task->SetLabel(&&__SIM__checkpoint___SIM__SmtpClientTls11751);
    if (!_this->var___SIM__this128->Receive(0,_task,&_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__SmtpClientTls11751:
    if (((libpi::String*)_task->tmp)->GetValue()=="^250")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpClientTls11250749);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^501")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpClientTls11501750);
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
    goto *_this->GetLabel();
__SIM____SIM__SmtpClientTls11250749:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_754);
      return true;
    }
__SIM__checkpoint_754:
    // ~this128[1] << ^RCPT;
    _this->var___SIM__this128->Send(0,_task,label_a614911eac73e47509fdabbe963d06c9);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_756);
      return true;
    }
__SIM__checkpoint_756:
    // ~this128[1] << "friend@smtpserver.pi";
    { 
      stringval_65a4ce8d05829010bc25aa9d89848b1d->AddRef();
      _this->var___SIM__this128->Send(0,_task,stringval_65a4ce8d05829010bc25aa9d89848b1d);
      RemoveRef(stringval_65a4ce8d05829010bc25aa9d89848b1d);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_759);
      return true;
    }
__SIM__checkpoint_759:
    _task->SetLabel(&&__SIM__checkpoint___SIM__SmtpClientTls11758);
    if (!_this->var___SIM__this128->Receive(0,_task,&_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__SmtpClientTls11758:
    if (((libpi::String*)_task->tmp)->GetValue()=="^250")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpClientTls11250757);
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
    goto *_this->GetLabel();
__SIM____SIM__SmtpClientTls11250757:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_761);
      return true;
    }
__SIM__checkpoint_761:
    // ~this128[1] << ^DATA;
    _this->var___SIM__this128->Send(0,_task,label_00e32b60a0f1155fe19e23d5a7b1247c);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_764);
      return true;
    }
__SIM__checkpoint_764:
    _task->SetLabel(&&__SIM__checkpoint___SIM__SmtpClientTls11763);
    if (!_this->var___SIM__this128->Receive(0,_task,&_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__SmtpClientTls11763:
    if (((libpi::String*)_task->tmp)->GetValue()=="^354")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpClientTls11354762);
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
    goto *_this->GetLabel();
__SIM____SIM__SmtpClientTls11354762:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_766);
      return true;
    }
__SIM__checkpoint_766:
    // ~this128[1] << "friend@smtpserver.pi";
    { 
      stringval_65a4ce8d05829010bc25aa9d89848b1d->AddRef();
      _this->var___SIM__this128->Send(0,_task,stringval_65a4ce8d05829010bc25aa9d89848b1d);
      RemoveRef(stringval_65a4ce8d05829010bc25aa9d89848b1d);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_768);
      return true;
    }
__SIM__checkpoint_768:
    // ~this128[1] << "user@smtpserver.pi";
    { 
      stringval_42e6fae527820c9a8ce6bf7a97074aac->AddRef();
      _this->var___SIM__this128->Send(0,_task,stringval_42e6fae527820c9a8ce6bf7a97074aac);
      RemoveRef(stringval_42e6fae527820c9a8ce6bf7a97074aac);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_770);
      return true;
    }
__SIM__checkpoint_770:
    // ~this128[1] << ^SUBJECT;
    _this->var___SIM__this128->Send(0,_task,label_628f2bd31290e9321620273d43d55cc8);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_772);
      return true;
    }
__SIM__checkpoint_772:
    // ~this128[1] << "SMTP MESSAGE";
    { 
      stringval_dad8bb550572b09371bdc9090db36936->AddRef();
      _this->var___SIM__this128->Send(0,_task,stringval_dad8bb550572b09371bdc9090db36936);
      RemoveRef(stringval_dad8bb550572b09371bdc9090db36936);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_774);
      return true;
    }
__SIM__checkpoint_774:
    // ~this128[1] << "SUBJECT";
    { 
      stringval_171cb8f32b5bd0a653aec8cc647f17ae->AddRef();
      _this->var___SIM__this128->Send(0,_task,stringval_171cb8f32b5bd0a653aec8cc647f17ae);
      RemoveRef(stringval_171cb8f32b5bd0a653aec8cc647f17ae);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_776);
      return true;
    }
__SIM__checkpoint_776:
    // ~this128[1] << ^DATALINE;
    _this->var___SIM__this128->Send(0,_task,label_6ef9720fb4ee8ea4de7074ca3243db4a);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_778);
      return true;
    }
__SIM__checkpoint_778:
    // ~this128[1] << "MSG1";
    { 
      stringval_e958e3c688dba52947168cbb0427c811->AddRef();
      _this->var___SIM__this128->Send(0,_task,stringval_e958e3c688dba52947168cbb0427c811);
      RemoveRef(stringval_e958e3c688dba52947168cbb0427c811);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_780);
      return true;
    }
__SIM__checkpoint_780:
    // ~this128[1] << "MSG2";
    { 
      stringval_f95989b6ff49aeb3a863b60a899c044a->AddRef();
      _this->var___SIM__this128->Send(0,_task,stringval_f95989b6ff49aeb3a863b60a899c044a);
      RemoveRef(stringval_f95989b6ff49aeb3a863b60a899c044a);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_782);
      return true;
    }
__SIM__checkpoint_782:
    // ~this128[1] << ^ENDOFDATA;
    _this->var___SIM__this128->Send(0,_task,label_3c2f33dd52c9e502985e3de0512b4271);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_784);
      return true;
    }
__SIM__checkpoint_784:
    // ~this128[1] << ^QUIT;
    _this->var___SIM__this128->Send(0,_task,label_57966ff4d7ad77f4146b41d2cca13560);
    _this->var___SIM__this128->Close(true);
    RemoveRef(_this->var___SIM__this128);
    _this->var___SIM__this128=NULL;
    RemoveRef(_task);
    return false;

__SIM____SIM__SmtpClientTls11501750:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_786);
      return true;
    }
__SIM__checkpoint_786:
    // ~this128[1] << ^QUIT;
    _this->var___SIM__this128->Send(0,_task,label_57966ff4d7ad77f4146b41d2cca13560);
    _this->var___SIM__this128->Close(true);
    RemoveRef(_this->var___SIM__this128);
    _this->var___SIM__this128=NULL;
    RemoveRef(_task);
    return false;

__SIM____SIM__SmtpClientTls11535742:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_788);
      return true;
    }
__SIM__checkpoint_788:
    // ~this128[1] << ^QUIT;
    _this->var___SIM__this128->Send(0,_task,label_57966ff4d7ad77f4146b41d2cca13560);
    _this->var___SIM__this128->Close(true);
    RemoveRef(_this->var___SIM__this128);
    _this->var___SIM__this128=NULL;
    RemoveRef(_task);
    return false;

__SIM____SIM__SmtpClientTls11250d734:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_790);
      return true;
    }
__SIM__checkpoint_790:
    if (dynamic_cast<Task___SIM__SmtpClientTls11*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpClientTls11* _taskref=(Task___SIM__SmtpClientTls11*)_task;
      _this->var___SIM__this128->AddRef();
      libpi::Session* arg0(_this->var___SIM__this128);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetLabel(&&method___SIM__SmtpClientTls11);

    }
    else
    { Task___SIM__SmtpClientTls11 *__SIM__task789 = new Task___SIM__SmtpClientTls11();
      __SIM__task789->SetWorker(&_task->GetWorker());
      _this->var___SIM__this128->AddRef();
      libpi::Session* arg0(_this->var___SIM__this128);
      __SIM__task789->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task789->SetLabel(&&method___SIM__SmtpClientTls11);
      RemoveRef(_task);
      _task=__SIM__task789;
    }
    goto method___SIM__SmtpClientTls11;
  } // }}}
  method___SIM__SmtpClientEhlo10: // {{{
  #undef _this
  #define _this ((Task___SIM__SmtpClientEhlo10*)_task)
  {

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_794);
      return true;
    }
__SIM__checkpoint_794:
    _task->SetLabel(&&__SIM__checkpoint___SIM__SmtpClientEhlo10793);
    if (!_this->var___SIM__this116->Receive(0,_task,&_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__SmtpClientEhlo10793:
    if (((libpi::String*)_task->tmp)->GetValue()=="^250")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpClientEhlo10250791);
    }
    else 
    if (((libpi::String*)_task->tmp)->GetValue()=="^250d")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpClientEhlo10250d792);
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
    goto *_this->GetLabel();
__SIM____SIM__SmtpClientEhlo10250791:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_796);
      return true;
    }
__SIM__checkpoint_796:
    // ~this116[1] >> ~srv127;
    _task->SetLabel(&&__SIM__receive795);
    if (!_this->var___SIM__this116->Receive(0,_task,(libpi::Value**)&_this->var___SIM__srv127))
      return false;
    __SIM__receive795:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_798);
      return true;
    }
__SIM__checkpoint_798:
    // ~this116[1] << ^StartTLS;
    _this->var___SIM__this116->Send(0,_task,label_d2f558e10d91cc53c5e7aac2c158428f);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_801);
      return true;
    }
__SIM__checkpoint_801:
    _task->SetLabel(&&__SIM__checkpoint___SIM__SmtpClientEhlo10800);
    if (!_this->var___SIM__this116->Receive(0,_task,&_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__SmtpClientEhlo10800:
    if (((libpi::String*)_task->tmp)->GetValue()=="^220")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__SmtpClientEhlo10220799);
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
    goto *_this->GetLabel();
__SIM____SIM__SmtpClientEhlo10220799:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_803);
      return true;
    }
__SIM__checkpoint_803:
    // ~this116[1] << ^EHLO;
    _this->var___SIM__this116->Send(0,_task,label_8a20c5f0d5bf4382d3c938deba4f10c3);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_805);
      return true;
    }
__SIM__checkpoint_805:
    if (dynamic_cast<Task___SIM__SmtpClientTls11*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpClientTls11* _taskref=(Task___SIM__SmtpClientTls11*)_task;
      _this->var___SIM__this116->AddRef();
      libpi::Session* arg0(_this->var___SIM__this116);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetLabel(&&method___SIM__SmtpClientTls11);

    }
    else
    { Task___SIM__SmtpClientTls11 *__SIM__task804 = new Task___SIM__SmtpClientTls11();
      __SIM__task804->SetWorker(&_task->GetWorker());
      _this->var___SIM__this116->AddRef();
      libpi::Session* arg0(_this->var___SIM__this116);
      __SIM__task804->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task804->SetLabel(&&method___SIM__SmtpClientTls11);
      RemoveRef(_task);
      _task=__SIM__task804;
    }
    goto method___SIM__SmtpClientTls11;
__SIM____SIM__SmtpClientEhlo10250d792:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_807);
      return true;
    }
__SIM__checkpoint_807:
    if (dynamic_cast<Task___SIM__SmtpClientEhlo10*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpClientEhlo10* _taskref=(Task___SIM__SmtpClientEhlo10*)_task;
      _this->var___SIM__this116->AddRef();
      libpi::Session* arg0(_this->var___SIM__this116);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetLabel(&&method___SIM__SmtpClientEhlo10);

    }
    else
    { Task___SIM__SmtpClientEhlo10 *__SIM__task806 = new Task___SIM__SmtpClientEhlo10();
      __SIM__task806->SetWorker(&_task->GetWorker());
      _this->var___SIM__this116->AddRef();
      libpi::Session* arg0(_this->var___SIM__this116);
      __SIM__task806->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task806->SetLabel(&&method___SIM__SmtpClientEhlo10);
      RemoveRef(_task);
      _task=__SIM__task806;
    }
    goto method___SIM__SmtpClientEhlo10;
  } // }}}
  method___SIM__FlatRight13: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight13*)_task)
  {
    // ~client114 = new ~smtp17(2 of 2)

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_808);
      return true;
    }
__SIM__checkpoint_808:
    { _task->tmp=new libpi::task::Channel();
      _this->var___SIM__smtp17->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__FlatRight13809);
      if (!((libpi::task::Channel*)_task->tmp)->Receive(_task,(libpi::Value**)&_this->var___SIM__client114))
        return false;
      __SIM__receive___SIM__FlatRight13809:
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_812);
      return true;
    }
__SIM__checkpoint_812:
    _task->SetLabel(&&__SIM__checkpoint___SIM__FlatRight13811);
    if (!_this->var___SIM__client114->Receive(0,_task,&_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__FlatRight13811:
    if (((libpi::String*)_task->tmp)->GetValue()=="^220_SERVICE_READY")
    {
      RemoveRef(_task->tmp);
      _task->tmp=NULL;
      _this->SetLabel(&&__SIM____SIM__FlatRight13220_SERVICE_READY810);
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp)->GetValue();
    goto *_this->GetLabel();
__SIM____SIM__FlatRight13220_SERVICE_READY810:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_814);
      return true;
    }
__SIM__checkpoint_814:
    // ~client114[1] >> ~msg115;
    _task->SetLabel(&&__SIM__receive813);
    if (!_this->var___SIM__client114->Receive(0,_task,(libpi::Value**)&_this->var___SIM__msg115))
      return false;
    __SIM__receive813:

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_816);
      return true;
    }
__SIM__checkpoint_816:
    // ~client114[1] << ^EHLO;
    _this->var___SIM__client114->Send(0,_task,label_8a20c5f0d5bf4382d3c938deba4f10c3);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_818);
      return true;
    }
__SIM__checkpoint_818:
    // ~client114[1] << "user";
    { 
      stringval_ee11cbb19052e40b07aac0ca060c23ee->AddRef();
      _this->var___SIM__client114->Send(0,_task,stringval_ee11cbb19052e40b07aac0ca060c23ee);
      RemoveRef(stringval_ee11cbb19052e40b07aac0ca060c23ee);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_820);
      return true;
    }
__SIM__checkpoint_820:
    if (dynamic_cast<Task___SIM__SmtpClientEhlo10*>(_task)!=NULL) // Recursive call
    { Task___SIM__SmtpClientEhlo10* _taskref=(Task___SIM__SmtpClientEhlo10*)_task;
      _this->var___SIM__client114->AddRef();
      libpi::Session* arg0(_this->var___SIM__client114);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetLabel(&&method___SIM__SmtpClientEhlo10);

    }
    else
    { Task___SIM__SmtpClientEhlo10 *__SIM__task819 = new Task___SIM__SmtpClientEhlo10();
      __SIM__task819->SetWorker(&_task->GetWorker());
      _this->var___SIM__client114->AddRef();
      libpi::Session* arg0(_this->var___SIM__client114);
      __SIM__task819->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task819->SetLabel(&&method___SIM__SmtpClientEhlo10);
      RemoveRef(_task);
      _task=__SIM__task819;
    }
    goto method___SIM__SmtpClientEhlo10;
  } // }}}
  method___SIM__FlatRight14: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight14*)_task)
  {

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_821);
      return true;
    }
__SIM__checkpoint_821:
    _this->var___SIM__smtp17=new libpi::task::Link(2);

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_822);
      return true;
    }
__SIM__checkpoint_822:
    { Task___SIM__FlatRight13 *__SIM__task823(new Task___SIM__FlatRight13());
      __SIM__task823->SetWorker(&_task->GetWorker());
      _this->var___SIM__smtp17->AddRef();
      libpi::task::Link* arg0(_this->var___SIM__smtp17);
      __SIM__task823->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task823->SetLabel(&&method___SIM__FlatRight13);
      _task->GetWorker().AddTask(__SIM__task823);
    }

    if (++_steps>=libpi::task::Task::MaxSteps)
    { _task->SetLabel(&&__SIM__checkpoint_825);
      return true;
    }
__SIM__checkpoint_825:
    if (dynamic_cast<Task___SIM__StartService9*>(_task)!=NULL) // Recursive call
    { Task___SIM__StartService9* _taskref=(Task___SIM__StartService9*)_task;
      libpi::Int* __SIM__systemexp826(new libpi::Int(libpi::task::Worker::TargetTasks));
      libpi::Int* arg0(__SIM__systemexp826);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      _this->var___SIM__smtp17->AddRef();
      libpi::task::Link* arg2(_this->var___SIM__smtp17);
      _taskref->SetArg0( arg0);
      RemoveRef(arg0);
      _taskref->SetArg1( arg1);
      RemoveRef(arg1);
      _taskref->SetArg2( arg2);
      RemoveRef(arg2);
      _taskref->SetLabel(&&method___SIM__StartService9);

    }
    else
    { Task___SIM__StartService9 *__SIM__task824 = new Task___SIM__StartService9();
      __SIM__task824->SetWorker(&_task->GetWorker());
      libpi::Int* __SIM__systemexp827(new libpi::Int(libpi::task::Worker::TargetTasks));
      libpi::Int* arg0(__SIM__systemexp827);
      _this->var___SIM__console4->AddRef();
      libpi::task::Link* arg1(_this->var___SIM__console4);
      _this->var___SIM__smtp17->AddRef();
      libpi::task::Link* arg2(_this->var___SIM__smtp17);
      __SIM__task824->SetArg0( arg0);
      RemoveRef(arg0);
      __SIM__task824->SetArg1( arg1);
      RemoveRef(arg1);
      __SIM__task824->SetArg2( arg2);
      RemoveRef(arg2);
      __SIM__task824->SetLabel(&&method___SIM__StartService9);
      RemoveRef(_task);
      _task=__SIM__task824;
    }
    goto method___SIM__StartService9;
  } // }}}
} // }}}
void libpi::task::Worker_Pool::Work() // {{{
{ try
  { while (true) // Continue until termination
    { // Find next task
      while (myActiveTasks.empty())
      { ourIdleWorkersLock.Lock();
        // Test if program is complete
        if (ourIdleWorkersSize==libpi::task::Worker::TargetTasks-1)
        { Task *nullTask=NULL;
          while (!ourIdleWorkers.empty())
          { ourIdleWorkers.front()->EmployTask(nullTask);
            ourIdleWorkers.pop();
          }
          myActiveTasks.push(nullTask);
          ourIdleWorkersSize=0;
          ourIdleWorkersLock.Release();
        }
        else
        { ourIdleWorkers.push(this);
          ++ourIdleWorkersSize;
          ourIdleWorkersLock.Release();
          myWaitLock.Lock();
        }
      }
      Task* task=myActiveTasks.front();
      myActiveTasks.pop();

      resume_task:
      if (!task)
        break;
      if (_methods(task))
      { if (myActiveTasks.empty())
          goto resume_task;
        else QueueTask(task);
      }
      else
        --ActiveTasks;
    }
  } catch (const string &error)
  { cerr << "Error: " << error << endl;
  }
  return;
} // }}}
void *_start_worker(void *arg) // {{{
{ libpi::task::Worker *worker=(libpi::task::Worker*)arg;
  worker->Work();
  delete worker;
  return NULL;
} // }}}
inline void _start_workers() // {{{
{ pthread_t x[libpi::task::Worker::TargetTasks];
  pthread_attr_t y;
  pthread_attr_init(&y);
  pthread_attr_setstacksize(&y,16384);
  //pthread_attr_setdetachstate(&y,PTHREAD_CREATE_DETACHED);
  for (size_t wc=0; wc<libpi::task::Worker::TargetTasks; ++wc)
  { libpi::task::Worker *_worker=new libpi::task::Worker_Pool();
    if (wc==0)
    { // Create main task
      libpi::task::Task* _main(new TaskMain());
      _main->SetLabel(NULL);
      _main->SetWorker(_worker);
      _worker->AddTask(_main);
    }
    // Bind thread to specific core
    //cpu_set_t cpuset;
    //CPU_ZERO(&cpuset);
    //CPU_SET(wc%cores, &cpuset);
    //pthread_setaffinity_np(x[wc],sizeof(cpu_set_t), &cpuset);
    pthread_create(&(x[wc]),&y,_start_worker,(void*)_worker);
  }
  for (size_t wc=0; wc<libpi::task::Worker::TargetTasks; ++wc)
    pthread_join(x[wc],NULL);
} // }}}
/*Start process, and its continuations */
int main(int argc, char **argv) // {{{
{ // PARSE ARGS!!
  for (int i=0; i<argc; ++i)
  { if (string(argv[i])=="-pi_tprocs" && i+1<argc)
    libpi::task::Worker::TargetTasks=atoi(argv[++i]);
    else
      _args.push_back(argv[i]);
  }
  try
  { signal(SIGCHLD, SIG_IGN); // Fork optimization
    // Start workers
    _start_workers();
    //munmap(_aprocs,sizeof(int));
  } catch (const string &error)
  { cerr << "Error: " << error << endl;
    return 1;
  }
  return 0;
} // }}}
