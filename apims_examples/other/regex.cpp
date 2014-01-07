/* Pre compilation output:
******************* Program *******************
(nu newlist:rec $intlist.
            1=>2
            { ^Delete[[true]]:
                Gend,
              ^Pop[[true]]:
                $intlist,
              ^Read[[true]]:
                2=>1
                { ^NONE[[true]]:
                    $intlist,
                  ^SOME[[true]]:
                    2=>1<Int>;
                    $intlist
                }
            })
  def EmptyList(this: rec %intlist.
                      1>>
                      { ^Delete[[true]]:
                        Lend,
                        ^Pop[[true]]:
                        %intlist,
                        ^Read[[true]]:
                        1<<
                        { ^NONE[[true]]:
                          %intlist,
                          ^SOME[[true]]:
                          1 << <Int>;
                          %intlist
                        }
                      }@(2 of 2))=
     this[1] >>
     { ^Delete:
       end,
       ^Pop:
       EmptyList(this),
       ^Read:
       this[1] << ^NONE;
       EmptyList(this)
     }
  in ( def NewList()=
          link(2,newlist,s,2);
          ( NewList()
          | EmptyList(s)
          )
       in NewList()
     | (nu cons:1=>2<Int>;
                1=>2<rec %intlist.
                     2<<
                     { ^Delete[[true]]:
                       Lend,
                       ^Pop[[true]]:
                       %intlist,
                       ^Read[[true]]:
                       2>>
                       { ^NONE[[true]]:
                         %intlist,
                         ^SOME[[true]]:
                         2 >> <Int>
                         %intlist
                       }
                     }@(1 of 2)>;
                2=>1<rec %intlist.
                     2<<
                     { ^Delete[[true]]:
                       Lend,
                       ^Pop[[true]]:
                       %intlist,
                       ^Read[[true]]:
                       2>>
                       { ^NONE[[true]]:
                         %intlist,
                         ^SOME[[true]]:
                         2 >> <Int>
                         %intlist
                       }
                     }@(1 of 2)>;
                Gend)
         ( def ConsService()=
              link(2,cons,s,2);
              ( ConsService()
              | s[1] >> elt;
                s[1] >> tail@(1 of 2);
                (nu locallink:rec $intlist.
                              1=>2
                              { ^Delete[[true]]:
                                  Gend,
                                ^Pop[[true]]:
                                  $intlist,
                                ^Read[[true]]:
                                  2=>1
                                  { ^NONE[[true]]:
                                      $intlist,
                                    ^SOME[[true]]:
                                      2=>1<Int>;
                                      $intlist
                                  }
                              })
                  ( link(2,locallink,lnk,2);
                    def Cons(this: rec %intlist.
                                   1>>
                                   { ^Delete[[true]]:
                                     Lend,
                                     ^Pop[[true]]:
                                     %intlist,
                                     ^Read[[true]]:
                                     1<<
                                     { ^NONE[[true]]:
                                       %intlist,
                                       ^SOME[[true]]:
                                       1 << <Int>;
                                       %intlist
                                     }
                                   }@(2 of 2),
                             tail: rec %intlist.
                                   2<<
                                   { ^Delete[[true]]:
                                     Lend,
                                     ^Pop[[true]]:
                                     %intlist,
                                     ^Read[[true]]:
                                     2>>
                                     { ^NONE[[true]]:
                                       %intlist,
                                       ^SOME[[true]]:
                                       2 >> <Int>
                                       %intlist
                                     }
                                   }@(1 of 2),
                             val: Int)=
                       this[1] >>
                       { ^Delete:
                         tail[2] << ^Delete;
                         end,
                         ^Pop:
                         tail[2] << ^Read;
                         tail[2] >>
                         { ^NONE:
                           tail[2] << ^Delete;
                           EmptyList(this),
                           ^SOME:
                           tail[2] >> newval;
                           tail[2] << ^Pop;
                           Cons(this, tail, newval)
                         },
                         ^Read:
                         this[1] << ^SOME;
                         this[1] << val;
                         Cons(this, tail, val)
                       }
                    in Cons(lnk, tail, elt)
                  | link(2,locallink,lnk,1);
                    s[1] << lnk;
                    end
                  )
              )
           in ConsService()
         | (nu listrev:1=>2<rec %intlist.
                            2<<
                            { ^Delete[[true]]:
                              Lend,
                              ^Pop[[true]]:
                              %intlist,
                              ^Read[[true]]:
                              2>>
                              { ^NONE[[true]]:
                                %intlist,
                                ^SOME[[true]]:
                                2 >> <Int>
                                %intlist
                              }
                            }@(1 of 2)>;
                       2=>1<rec %intlist.
                            2<<
                            { ^Delete[[true]]:
                              Lend,
                              ^Pop[[true]]:
                              %intlist,
                              ^Read[[true]]:
                              2>>
                              { ^NONE[[true]]:
                                %intlist,
                                ^SOME[[true]]:
                                2 >> <Int>
                                %intlist
                              }
                            }@(1 of 2)>;
                       Gend)
             ( def ListRevService()=
                  link(2,listrev,s,2);
                  ( ListRevService()
                  | def ListRev(lst: rec %intlist.
                                     2<<
                                     { ^Delete[[true]]:
                                       Lend,
                                       ^Pop[[true]]:
                                       %intlist,
                                       ^Read[[true]]:
                                       2>>
                                       { ^NONE[[true]]:
                                         %intlist,
                                         ^SOME[[true]]:
                                         2 >> <Int>
                                         %intlist
                                       }
                                     }@(1 of 2),
                                rev: rec %intlist.
                                     2<<
                                     { ^Delete[[true]]:
                                       Lend,
                                       ^Pop[[true]]:
                                       %intlist,
                                       ^Read[[true]]:
                                       2>>
                                       { ^NONE[[true]]:
                                         %intlist,
                                         ^SOME[[true]]:
                                         2 >> <Int>
                                         %intlist
                                       }
                                     }@(1 of 2),
                                dest: 1 << <rec %intlist.
                                        2<<
                                        { ^Delete[[true]]:
                                          Lend,
                                          ^Pop[[true]]:
                                          %intlist,
                                          ^Read[[true]]:
                                          2>>
                                          { ^NONE[[true]]:
                                            %intlist,
                                            ^SOME[[true]]:
                                            2 >> <Int>
                                            %intlist
                                          }
                                        }@(1 of 2)>;
                                      Lend@(2 of 2))=
                       lst[2] << ^Read;
                       lst[2] >>
                       { ^NONE:
                         lst[2] << ^Delete;
                         dest[1] << rev;
                         end,
                         ^SOME:
                         lst[2] >> head;
                         lst[2] << ^Pop;
                         link(2,cons,cs,1);
                         cs[2] << head;
                         cs[2] << rev;
                         cs[2] >> newrev@(1 of 2);
                         ListRev(lst, newrev, dest)
                       }
                    in s[1] >> lst@(1 of 2);
                       link(2,newlist,rev,1);
                       ListRev(lst, rev, s)
                  )
               in ListRevService()
             | (nu listcopy:1=>2<rec %intlist.
                                 2<<
                                 { ^Delete[[true]]:
                                   Lend,
                                   ^Pop[[true]]:
                                   %intlist,
                                   ^Read[[true]]:
                                   2>>
                                   { ^NONE[[true]]:
                                     %intlist,
                                     ^SOME[[true]]:
                                     2 >> <Int>
                                     %intlist
                                   }
                                 }@(1 of 2)>;
                            2=>1<rec %intlist.
                                 2<<
                                 { ^Delete[[true]]:
                                   Lend,
                                   ^Pop[[true]]:
                                   %intlist,
                                   ^Read[[true]]:
                                   2>>
                                   { ^NONE[[true]]:
                                     %intlist,
                                     ^SOME[[true]]:
                                     2 >> <Int>
                                     %intlist
                                   }
                                 }@(1 of 2)>;
                            2=>1<rec %intlist.
                                 2<<
                                 { ^Delete[[true]]:
                                   Lend,
                                   ^Pop[[true]]:
                                   %intlist,
                                   ^Read[[true]]:
                                   2>>
                                   { ^NONE[[true]]:
                                     %intlist,
                                     ^SOME[[true]]:
                                     2 >> <Int>
                                     %intlist
                                   }
                                 }@(1 of 2)>;
                            Gend)
                 ( def ListCopyService()=
                      link(2,listcopy,s,2);
                      ( ListCopyService()
                      | def ListCopy(lst: rec %intlist.
                                          2<<
                                          { ^Delete[[true]]:
                                            Lend,
                                            ^Pop[[true]]:
                                            %intlist,
                                            ^Read[[true]]:
                                            2>>
                                            { ^NONE[[true]]:
                                              %intlist,
                                              ^SOME[[true]]:
                                              2 >> <Int>
                                              %intlist
                                            }
                                          }@(1 of 2),
                                     dest: 1 << <rec %intlist.
                                             2<<
                                             { ^Delete[[true]]:
                                               Lend,
                                               ^Pop[[true]]:
                                               %intlist,
                                               ^Read[[true]]:
                                               2>>
                                               { ^NONE[[true]]:
                                                 %intlist,
                                                 ^SOME[[true]]:
                                                 2 >> <Int>
                                                 %intlist
                                               }
                                             }@(1 of 2)>;
                                           1 << <rec %intlist.
                                             2<<
                                             { ^Delete[[true]]:
                                               Lend,
                                               ^Pop[[true]]:
                                               %intlist,
                                               ^Read[[true]]:
                                               2>>
                                               { ^NONE[[true]]:
                                                 %intlist,
                                                 ^SOME[[true]]:
                                                 2 >> <Int>
                                                 %intlist
                                               }
                                             }@(1 of 2)>;
                                           Lend@(2 of 2))=
                           lst[2] << ^Read;
                           lst[2] >>
                           { ^NONE:
                             link(2,newlist,copylst,1);
                             dest[1] << copylst;
                             dest[1] << lst;
                             end,
                             ^SOME:
                             lst[2] >> head;
                             lst[2] << ^Pop;
                             link(2,listcopy,call,1);
                             call[2] << lst;
                             call[2] >> copy1@(1 of 2);
                             link(2,cons,cons1,1);
                             cons1[2] << head;
                             cons1[2] << copy1;
                             cons1[2] >> copy1@(1 of 2);
                             dest[1] << copy1;
                             call[2] >> copy2@(1 of 2);
                             link(2,cons,cons2,1);
                             cons2[2] << head;
                             cons2[2] << copy2;
                             cons2[2] >> copy2@(1 of 2);
                             dest[1] << copy2;
                             end
                           }
                        in s[1] >> lst@(1 of 2);
                           ListCopy(lst, s)
                      )
                   in ListCopyService()
                 | (nu append:1=>2<rec %intlist.
                                   2<<
                                   { ^Delete[[true]]:
                                     Lend,
                                     ^Pop[[true]]:
                                     %intlist,
                                     ^Read[[true]]:
                                     2>>
                                     { ^NONE[[true]]:
                                       %intlist,
                                       ^SOME[[true]]:
                                       2 >> <Int>
                                       %intlist
                                     }
                                   }@(1 of 2)>;
                              1=>2<rec %intlist.
                                   2<<
                                   { ^Delete[[true]]:
                                     Lend,
                                     ^Pop[[true]]:
                                     %intlist,
                                     ^Read[[true]]:
                                     2>>
                                     { ^NONE[[true]]:
                                       %intlist,
                                       ^SOME[[true]]:
                                       2 >> <Int>
                                       %intlist
                                     }
                                   }@(1 of 2)>;
                              2=>1<rec %intlist.
                                   2<<
                                   { ^Delete[[true]]:
                                     Lend,
                                     ^Pop[[true]]:
                                     %intlist,
                                     ^Read[[true]]:
                                     2>>
                                     { ^NONE[[true]]:
                                       %intlist,
                                       ^SOME[[true]]:
                                       2 >> <Int>
                                       %intlist
                                     }
                                   }@(1 of 2)>;
                              Gend)
                     ( def AppendService()=
                          link(2,append,s,2);
                          ( AppendService()
                          | def Append(lhs: rec %intlist.
                                            2<<
                                            { ^Delete[[true]]:
                                              Lend,
                                              ^Pop[[true]]:
                                              %intlist,
                                              ^Read[[true]]:
                                              2>>
                                              { ^NONE[[true]]:
                                                %intlist,
                                                ^SOME[[true]]:
                                                2 >> <Int>
                                                %intlist
                                              }
                                            }@(1 of 2),
                                       rhs: rec %intlist.
                                            2<<
                                            { ^Delete[[true]]:
                                              Lend,
                                              ^Pop[[true]]:
                                              %intlist,
                                              ^Read[[true]]:
                                              2>>
                                              { ^NONE[[true]]:
                                                %intlist,
                                                ^SOME[[true]]:
                                                2 >> <Int>
                                                %intlist
                                              }
                                            }@(1 of 2),
                                       stack: rec %intlist.
                                              2<<
                                              { ^Delete[[true]]:
                                                Lend,
                                                ^Pop[[true]]:
                                                %intlist,
                                                ^Read[[true]]:
                                                2>>
                                                { ^NONE[[true]]:
                                                  %intlist,
                                                  ^SOME[[true]]:
                                                  2 >> <Int>
                                                  %intlist
                                                }
                                              }@(1 of 2),
                                       dest: 1 << <rec %intlist.
                                               2<<
                                               { ^Delete[[true]]:
                                                 Lend,
                                                 ^Pop[[true]]:
                                                 %intlist,
                                                 ^Read[[true]]:
                                                 2>>
                                                 { ^NONE[[true]]:
                                                   %intlist,
                                                   ^SOME[[true]]:
                                                   2 >> <Int>
                                                   %intlist
                                                 }
                                               }@(1 of 2)>;
                                             Lend@(2 of 2))=
                               lhs[2] << ^Read;
                               lhs[2] >>
                               { ^NONE:
                                 lhs[2] << ^Delete;
                                 def AppendRev(lhs: rec %intlist.
                                                    2<<
                                                    { ^Delete[[true]]:
                                                      Lend,
                                                      ^Pop[[true]]:
                                                      %intlist,
                                                      ^Read[[true]]:
                                                      2>>
                                                      { ^NONE[[true]]:
                                                        %intlist,
                                                        ^SOME[[true]]:
                                                        2 >> <Int>
                                                        %intlist
                                                      }
                                                    }@(1 of 2),
                                               rhs: rec %intlist.
                                                    2<<
                                                    { ^Delete[[true]]:
                                                      Lend,
                                                      ^Pop[[true]]:
                                                      %intlist,
                                                      ^Read[[true]]:
                                                      2>>
                                                      { ^NONE[[true]]:
                                                        %intlist,
                                                        ^SOME[[true]]:
                                                        2 >> <Int>
                                                        %intlist
                                                      }
                                                    }@(1 of 2),
                                               dest: 1 << <rec %intlist.
                                                       2<<
                                                       { ^Delete[[true]]:
                                                         Lend,
                                                         ^Pop[[true]]:
                                                         %intlist,
                                                         ^Read[[true]]:
                                                         2>>
                                                         { ^NONE[[true]]:
                                                           %intlist,
                                                           ^SOME[[true]]:
                                                           2 >> <Int>
                                                           %intlist
                                                         }
                                                       }@(1 of 2)>;
                                                     Lend@(2 of 2))=
                                    lhs[2] << ^Read;
                                    lhs[2] >>
                                    { ^NONE:
                                      lhs[2] << ^Delete;
                                      dest[1] << rhs;
                                      end,
                                      ^SOME:
                                      lhs[2] >> elt;
                                      lhs[2] << ^Pop;
                                      link(2,cons,cs,1);
                                      cs[2] << elt;
                                      cs[2] << rhs;
                                      cs[2] >> rhs@(1 of 2);
                                      AppendRev(lhs, rhs, dest)
                                    }
                                 in AppendRev(stack, rhs, dest),
                                 ^SOME:
                                 lhs[2] >> elt;
                                 lhs[2] << ^Pop;
                                 link(2,cons,cs,1);
                                 cs[2] << elt;
                                 cs[2] << stack;
                                 cs[2] >> stack@(1 of 2);
                                 Append(lhs, rhs, stack, dest)
                               }
                            in link(2,newlist,stack,1);
                               s[1] >> lhs@(1 of 2);
                               s[1] >> rhs@(1 of 2);
                               Append(lhs, rhs, stack, s)
                          )
                       in AppendService()
                     | (nu re_0:rec $regexp.
                                1=>2
                                { ^Delete[[true]]:
                                    Gend,
                                  ^Match[[true]]:
                                    1=>2<rec %intlist.
                                         2<<
                                         { ^Delete[[true]]:
                                           Lend,
                                           ^Pop[[true]]:
                                           %intlist,
                                           ^Read[[true]]:
                                           2>>
                                           { ^NONE[[true]]:
                                             %intlist,
                                             ^SOME[[true]]:
                                             2 >> <Int>
                                             %intlist
                                           }
                                         }@(1 of 2)>;
                                    2=>1<Bool>;
                                    $regexp
                                })
                         def RE0Service()=
                            link(2,re_0,connection,2);
                            ( RE0Service()
                            | def RE0(this: rec %regexp.
                                            1>>
                                            { ^Delete[[true]]:
                                              Lend,
                                              ^Match[[true]]:
                                              1 >> <rec %intlist.
                                                2<<
                                                { ^Delete[[true]]:
                                                  Lend,
                                                  ^Pop[[true]]:
                                                  %intlist,
                                                  ^Read[[true]]:
                                                  2>>
                                                  { ^NONE[[true]]:
                                                    %intlist,
                                                    ^SOME[[true]]:
                                                    2 >> <Int>
                                                    %intlist
                                                  }
                                                }@(1 of 2)>
                                              1 << <Bool>;
                                              %regexp
                                            }@(2 of 2))=
                                 this[1] >>
                                 { ^Delete:
                                   end,
                                   ^Match:
                                   this[1] >> str@(1 of 2);
                                   str[2] << ^Delete;
                                   this[1] << false;
                                   RE0(this)
                                 }
                              in RE0(connection)
                            )
                         in (nu re_1:rec $regexp.
                                     1=>2
                                     { ^Delete[[true]]:
                                         Gend,
                                       ^Match[[true]]:
                                         1=>2<rec %intlist.
                                              2<<
                                              { ^Delete[[true]]:
                                                Lend,
                                                ^Pop[[true]]:
                                                %intlist,
                                                ^Read[[true]]:
                                                2>>
                                                { ^NONE[[true]]:
                                                  %intlist,
                                                  ^SOME[[true]]:
                                                  2 >> <Int>
                                                  %intlist
                                                }
                                              }@(1 of 2)>;
                                         2=>1<Bool>;
                                         $regexp
                                     })
                              def RE1Service()=
                                 link(2,re_1,connection,2);
                                 ( RE1Service()
                                 | def RE1(this: rec %regexp.
                                                 1>>
                                                 { ^Delete[[true]]:
                                                   Lend,
                                                   ^Match[[true]]:
                                                   1 >> <rec %intlist.
                                                     2<<
                                                     { ^Delete[[true]]:
                                                       Lend,
                                                       ^Pop[[true]]:
                                                       %intlist,
                                                       ^Read[[true]]:
                                                       2>>
                                                       { ^NONE[[true]]:
                                                         %intlist,
                                                         ^SOME[[true]]:
                                                         2 >> <Int>
                                                         %intlist
                                                       }
                                                     }@(1 of 2)>
                                                   1 << <Bool>;
                                                   %regexp
                                                 }@(2 of 2))=
                                      this[1] >>
                                      { ^Delete:
                                        end,
                                        ^Match:
                                        this[1] >> str@(1 of 2);
                                        str[2] << ^Read;
                                        str[2] >>
                                        { ^NONE:
                                          str[2] << ^Delete;
                                          this[1] << true;
                                          RE1(this),
                                          ^SOME:
                                          this[1] << false;
                                          str[2] >> lit;
                                          str[2] << ^Delete;
                                          RE1(this)
                                        }
                                      }
                                   in RE1(connection)
                                 )
                              in (nu re_literal:1=>2<Int>;
                                                rec $regexp.
                                                1=>2
                                                { ^Delete[[true]]:
                                                    Gend,
                                                  ^Match[[true]]:
                                                    1=>2<rec %intlist.
                                                         2<<
                                                         { ^Delete[[true]]:
                                                           Lend,
                                                           ^Pop[[true]]:
                                                           %intlist,
                                                           ^Read[[true]]:
                                                           2>>
                                                           { ^NONE[[true]]:
                                                             %intlist,
                                                             ^SOME[[true]]:
                                                             2 >> <Int>
                                                             %intlist
                                                           }
                                                         }@(1 of 2)>;
                                                    2=>1<Bool>;
                                                    $regexp
                                                })
                                   ( def RELiteralService()=
                                        link(2,re_literal,connection,2);
                                        ( RELiteralService()
                                        | def RELiteral(this: rec %regexp.
                                                              1>>
                                                              { ^Delete[[true]]:
                                                                Lend,
                                                                ^Match[[true]]:
                                                                1 >> <rec %intlist.
                                                                  2<<
                                                                  { ^Delete[[true]]:
                                                                    Lend,
                                                                    ^Pop[[true]]:
                                                                    %intlist,
                                                                    ^Read[[true]]:
                                                                    2>>
                                                                    { ^NONE[[true]]:
                                                                      %intlist,
                                                                      ^SOME[[true]]:
                                                                      2 >> <Int>
                                                                      %intlist
                                                                    }
                                                                  }@(1 of 2)>
                                                                1 << <Bool>;
                                                                %regexp
                                                              }@(2 of 2),
                                                        literal: Int)=
                                             this[1] >>
                                             { ^Delete:
                                               end,
                                               ^Match:
                                               this[1] >> str@(1 of 2);
                                               str[2] << ^Read;
                                               str[2] >>
                                               { ^NONE:
                                                 str[2] << ^Delete;
                                                 this[1] << false;
                                                 RELiteral(this, literal),
                                                 ^SOME:
                                                 str[2] >> lit;
                                                 if (literal = lit)
                                                 then str[2] << ^Pop;
                                                      str[2] << ^Read;
                                                      str[2] >>
                                                      { ^NONE:
                                                        this[1] << true;
                                                        str[2] << ^Delete;
                                                        RELiteral(this, literal),
                                                        ^SOME:
                                                        this[1] << false;
                                                        str[2] >> lit;
                                                        str[2] << ^Delete;
                                                        RELiteral(this, literal)
                                                      }
                                                 else this[1] << false;
                                                      str[2] << ^Delete;
                                                      RELiteral(this, literal)
                                               }
                                             }
                                          in connection[1] >> lit;
                                             RELiteral(connection, lit)
                                        )
                                     in RELiteralService()
                                   | (nu re_sequence:1=>2<rec %regexp.
                                                          2<<
                                                          { ^Delete[[true]]:
                                                            Lend,
                                                            ^Match[[true]]:
                                                            2 << <rec %intlist.
                                                              2<<
                                                              { ^Delete[[true]]:
                                                                Lend,
                                                                ^Pop[[true]]:
                                                                %intlist,
                                                                ^Read[[true]]:
                                                                2>>
                                                                { ^NONE[[true]]:
                                                                  %intlist,
                                                                  ^SOME[[true]]:
                                                                  2 >> <Int>
                                                                  %intlist
                                                                }
                                                              }@(1 of 2)>;
                                                            2 >> <Bool>
                                                            %regexp
                                                          }@(1 of 2)>;
                                                     1=>2<rec %regexp.
                                                          2<<
                                                          { ^Delete[[true]]:
                                                            Lend,
                                                            ^Match[[true]]:
                                                            2 << <rec %intlist.
                                                              2<<
                                                              { ^Delete[[true]]:
                                                                Lend,
                                                                ^Pop[[true]]:
                                                                %intlist,
                                                                ^Read[[true]]:
                                                                2>>
                                                                { ^NONE[[true]]:
                                                                  %intlist,
                                                                  ^SOME[[true]]:
                                                                  2 >> <Int>
                                                                  %intlist
                                                                }
                                                              }@(1 of 2)>;
                                                            2 >> <Bool>
                                                            %regexp
                                                          }@(1 of 2)>;
                                                     rec $regexp.
                                                     1=>2
                                                     { ^Delete[[true]]:
                                                         Gend,
                                                       ^Match[[true]]:
                                                         1=>2<rec %intlist.
                                                              2<<
                                                              { ^Delete[[true]]:
                                                                Lend,
                                                                ^Pop[[true]]:
                                                                %intlist,
                                                                ^Read[[true]]:
                                                                2>>
                                                                { ^NONE[[true]]:
                                                                  %intlist,
                                                                  ^SOME[[true]]:
                                                                  2 >> <Int>
                                                                  %intlist
                                                                }
                                                              }@(1 of 2)>;
                                                         2=>1<Bool>;
                                                         $regexp
                                                     })
                                       ( def RESequenceService()=
                                            link(2,re_sequence,connection,2);
                                            ( RESequenceService()
                                            | def RESequence(this: rec %regexp.
                                                                   1>>
                                                                   { ^Delete[[true]]:
                                                                     Lend,
                                                                     ^Match[[true]]:
                                                                     1 >> <rec %intlist.
                                                                       2<<
                                                                       { ^Delete[[true]]:
                                                                         Lend,
                                                                         ^Pop[[true]]:
                                                                         %intlist,
                                                                         ^Read[[true]]:
                                                                         2>>
                                                                         { ^NONE[[true]]:
                                                                           %intlist,
                                                                           ^SOME[[true]]:
                                                                           2 >> <Int>
                                                                           %intlist
                                                                         }
                                                                       }@(1 of 2)>
                                                                     1 << <Bool>;
                                                                     %regexp
                                                                   }@(2 of 2),
                                                             left: rec %regexp.
                                                                   2<<
                                                                   { ^Delete[[true]]:
                                                                     Lend,
                                                                     ^Match[[true]]:
                                                                     2 << <rec %intlist.
                                                                       2<<
                                                                       { ^Delete[[true]]:
                                                                         Lend,
                                                                         ^Pop[[true]]:
                                                                         %intlist,
                                                                         ^Read[[true]]:
                                                                         2>>
                                                                         { ^NONE[[true]]:
                                                                           %intlist,
                                                                           ^SOME[[true]]:
                                                                           2 >> <Int>
                                                                           %intlist
                                                                         }
                                                                       }@(1 of 2)>;
                                                                     2 >> <Bool>
                                                                     %regexp
                                                                   }@(1 of 2),
                                                             right: rec %regexp.
                                                                    2<<
                                                                    { ^Delete[[true]]:
                                                                      Lend,
                                                                      ^Match[[true]]:
                                                                      2 << <rec %intlist.
                                                                        2<<
                                                                        { ^Delete[[true]]:
                                                                          Lend,
                                                                          ^Pop[[true]]:
                                                                          %intlist,
                                                                          ^Read[[true]]:
                                                                          2>>
                                                                          { ^NONE[[true]]:
                                                                            %intlist,
                                                                            ^SOME[[true]]:
                                                                            2 >> <Int>
                                                                            %intlist
                                                                          }
                                                                        }@(1 of 2)>;
                                                                      2 >> <Bool>
                                                                      %regexp
                                                                    }@(1 of 2))=
                                                 this[1] >>
                                                 { ^Delete:
                                                   left[2] << ^Delete;
                                                   right[2] << ^Delete;
                                                   end,
                                                   ^Match:
                                                   def MatchSeq(this: 1 << <Bool>;
                                                                      rec %regexp.
                                                                      1>>
                                                                      { ^Delete[[true]]:
                                                                        Lend,
                                                                        ^Match[[true]]:
                                                                        1 >> <rec %intlist.
                                                                          2<<
                                                                          { ^Delete[[true]]:
                                                                            Lend,
                                                                            ^Pop[[true]]:
                                                                            %intlist,
                                                                            ^Read[[true]]:
                                                                            2>>
                                                                            { ^NONE[[true]]:
                                                                              %intlist,
                                                                              ^SOME[[true]]:
                                                                              2 >> <Int>
                                                                              %intlist
                                                                            }
                                                                          }@(1 of 2)>
                                                                        1 << <Bool>;
                                                                        %regexp
                                                                      }@(2 of 2),
                                                                left: rec %regexp.
                                                                      2<<
                                                                      { ^Delete[[true]]:
                                                                        Lend,
                                                                        ^Match[[true]]:
                                                                        2 << <rec %intlist.
                                                                          2<<
                                                                          { ^Delete[[true]]:
                                                                            Lend,
                                                                            ^Pop[[true]]:
                                                                            %intlist,
                                                                            ^Read[[true]]:
                                                                            2>>
                                                                            { ^NONE[[true]]:
                                                                              %intlist,
                                                                              ^SOME[[true]]:
                                                                              2 >> <Int>
                                                                              %intlist
                                                                            }
                                                                          }@(1 of 2)>;
                                                                        2 >> <Bool>
                                                                        %regexp
                                                                      }@(1 of 2),
                                                                right: rec %regexp.
                                                                       2<<
                                                                       { ^Delete[[true]]:
                                                                         Lend,
                                                                         ^Match[[true]]:
                                                                         2 << <rec %intlist.
                                                                           2<<
                                                                           { ^Delete[[true]]:
                                                                             Lend,
                                                                             ^Pop[[true]]:
                                                                             %intlist,
                                                                             ^Read[[true]]:
                                                                             2>>
                                                                             { ^NONE[[true]]:
                                                                               %intlist,
                                                                               ^SOME[[true]]:
                                                                               2 >> <Int>
                                                                               %intlist
                                                                             }
                                                                           }@(1 of 2)>;
                                                                         2 >> <Bool>
                                                                         %regexp
                                                                       }@(1 of 2),
                                                                str1: rec %intlist.
                                                                      2<<
                                                                      { ^Delete[[true]]:
                                                                        Lend,
                                                                        ^Pop[[true]]:
                                                                        %intlist,
                                                                        ^Read[[true]]:
                                                                        2>>
                                                                        { ^NONE[[true]]:
                                                                          %intlist,
                                                                          ^SOME[[true]]:
                                                                          2 >> <Int>
                                                                          %intlist
                                                                        }
                                                                      }@(1 of 2),
                                                                str2: rec %intlist.
                                                                      2<<
                                                                      { ^Delete[[true]]:
                                                                        Lend,
                                                                        ^Pop[[true]]:
                                                                        %intlist,
                                                                        ^Read[[true]]:
                                                                        2>>
                                                                        { ^NONE[[true]]:
                                                                          %intlist,
                                                                          ^SOME[[true]]:
                                                                          2 >> <Int>
                                                                          %intlist
                                                                        }
                                                                      }@(1 of 2))=
                                                      str1[2] << ^Read;
                                                      str1[2] >>
                                                      { ^NONE:
                                                        link(2,listrev,r1,1);
                                                        r1[2] << str2;
                                                        left[2] << ^Match;
                                                        left[2] << str1;
                                                        left[2] >> result1;
                                                        right[2] << ^Match;
                                                        r1[2] >> str2@(1 of 2);
                                                        right[2] << str2;
                                                        right[2] >> result2;
                                                        this[1] << (result1 and result2);
                                                        RESequence(this, left, right),
                                                        ^SOME:
                                                        str1[2] >> head;
                                                        link(2,listcopy,cpy1,1);
                                                        cpy1[2] << str1;
                                                        cpy1[2] >> str1@(1 of 2);
                                                        cpy1[2] >> str1cpy@(1 of 2);
                                                        link(2,listcopy,cpy2,1);
                                                        cpy2[2] << str2;
                                                        cpy2[2] >> str2@(1 of 2);
                                                        cpy2[2] >> str2cpy@(1 of 2);
                                                        left[2] << ^Match;
                                                        left[2] << str1;
                                                        left[2] >> result1;
                                                        link(2,listrev,r1,1);
                                                        r1[2] << str2;
                                                        r1[2] >> str2@(1 of 2);
                                                        right[2] << ^Match;
                                                        right[2] << str2;
                                                        right[2] >> result2;
                                                        if (result1 and result2)
                                                        then str1cpy[2] << ^Delete;
                                                             str2cpy[2] << ^Delete;
                                                             this[1] << true;
                                                             RESequence(this, left, right)
                                                        else str1cpy[2] << ^Pop;
                                                             link(2,cons,cons1,1);
                                                             cons1[2] << head;
                                                             cons1[2] << str2cpy;
                                                             cons1[2] >> str2cpy@(1 of 2);
                                                             MatchSeq(this, left, right, str1cpy, str2cpy)
                                                      }
                                                   in this[1] >> str1@(1 of 2);
                                                      link(2,newlist,str2,1);
                                                      MatchSeq(this, left, right, str1, str2)
                                                 }
                                              in connection[1] >> left@(1 of 2);
                                                 connection[1] >> right@(1 of 2);
                                                 RESequence(connection, left, right)
                                            )
                                         in RESequenceService()
                                       | (nu re_sum:1=>2<rec %regexp.
                                                         2<<
                                                         { ^Delete[[true]]:
                                                           Lend,
                                                           ^Match[[true]]:
                                                           2 << <rec %intlist.
                                                             2<<
                                                             { ^Delete[[true]]:
                                                               Lend,
                                                               ^Pop[[true]]:
                                                               %intlist,
                                                               ^Read[[true]]:
                                                               2>>
                                                               { ^NONE[[true]]:
                                                                 %intlist,
                                                                 ^SOME[[true]]:
                                                                 2 >> <Int>
                                                                 %intlist
                                                               }
                                                             }@(1 of 2)>;
                                                           2 >> <Bool>
                                                           %regexp
                                                         }@(1 of 2)>;
                                                    1=>2<rec %regexp.
                                                         2<<
                                                         { ^Delete[[true]]:
                                                           Lend,
                                                           ^Match[[true]]:
                                                           2 << <rec %intlist.
                                                             2<<
                                                             { ^Delete[[true]]:
                                                               Lend,
                                                               ^Pop[[true]]:
                                                               %intlist,
                                                               ^Read[[true]]:
                                                               2>>
                                                               { ^NONE[[true]]:
                                                                 %intlist,
                                                                 ^SOME[[true]]:
                                                                 2 >> <Int>
                                                                 %intlist
                                                               }
                                                             }@(1 of 2)>;
                                                           2 >> <Bool>
                                                           %regexp
                                                         }@(1 of 2)>;
                                                    rec $regexp.
                                                    1=>2
                                                    { ^Delete[[true]]:
                                                        Gend,
                                                      ^Match[[true]]:
                                                        1=>2<rec %intlist.
                                                             2<<
                                                             { ^Delete[[true]]:
                                                               Lend,
                                                               ^Pop[[true]]:
                                                               %intlist,
                                                               ^Read[[true]]:
                                                               2>>
                                                               { ^NONE[[true]]:
                                                                 %intlist,
                                                                 ^SOME[[true]]:
                                                                 2 >> <Int>
                                                                 %intlist
                                                               }
                                                             }@(1 of 2)>;
                                                        2=>1<Bool>;
                                                        $regexp
                                                    })
                                           ( def RESumService()=
                                                link(2,re_sum,connection,2);
                                                ( RESumService()
                                                | def RESum(this: rec %regexp.
                                                                  1>>
                                                                  { ^Delete[[true]]:
                                                                    Lend,
                                                                    ^Match[[true]]:
                                                                    1 >> <rec %intlist.
                                                                      2<<
                                                                      { ^Delete[[true]]:
                                                                        Lend,
                                                                        ^Pop[[true]]:
                                                                        %intlist,
                                                                        ^Read[[true]]:
                                                                        2>>
                                                                        { ^NONE[[true]]:
                                                                          %intlist,
                                                                          ^SOME[[true]]:
                                                                          2 >> <Int>
                                                                          %intlist
                                                                        }
                                                                      }@(1 of 2)>
                                                                    1 << <Bool>;
                                                                    %regexp
                                                                  }@(2 of 2),
                                                            left: rec %regexp.
                                                                  2<<
                                                                  { ^Delete[[true]]:
                                                                    Lend,
                                                                    ^Match[[true]]:
                                                                    2 << <rec %intlist.
                                                                      2<<
                                                                      { ^Delete[[true]]:
                                                                        Lend,
                                                                        ^Pop[[true]]:
                                                                        %intlist,
                                                                        ^Read[[true]]:
                                                                        2>>
                                                                        { ^NONE[[true]]:
                                                                          %intlist,
                                                                          ^SOME[[true]]:
                                                                          2 >> <Int>
                                                                          %intlist
                                                                        }
                                                                      }@(1 of 2)>;
                                                                    2 >> <Bool>
                                                                    %regexp
                                                                  }@(1 of 2),
                                                            right: rec %regexp.
                                                                   2<<
                                                                   { ^Delete[[true]]:
                                                                     Lend,
                                                                     ^Match[[true]]:
                                                                     2 << <rec %intlist.
                                                                       2<<
                                                                       { ^Delete[[true]]:
                                                                         Lend,
                                                                         ^Pop[[true]]:
                                                                         %intlist,
                                                                         ^Read[[true]]:
                                                                         2>>
                                                                         { ^NONE[[true]]:
                                                                           %intlist,
                                                                           ^SOME[[true]]:
                                                                           2 >> <Int>
                                                                           %intlist
                                                                         }
                                                                       }@(1 of 2)>;
                                                                     2 >> <Bool>
                                                                     %regexp
                                                                   }@(1 of 2))=
                                                     this[1] >>
                                                     { ^Delete:
                                                       left[2] << ^Delete;
                                                       right[2] << ^Delete;
                                                       end,
                                                       ^Match:
                                                       this[1] >> str1@(1 of 2);
                                                       left[2] << ^Match;
                                                       right[2] << ^Match;
                                                       link(2,listcopy,cpy,1);
                                                       cpy[2] << str1;
                                                       cpy[2] >> str2@(1 of 2);
                                                       left[2] << str2;
                                                       cpy[2] >> str3@(1 of 2);
                                                       right[2] << str3;
                                                       left[2] >> r1;
                                                       right[2] >> r2;
                                                       this[1] << (r1 or r2);
                                                       RESum(this, left, right)
                                                     }
                                                  in connection[1] >> left@(1 of 2);
                                                     connection[1] >> right@(1 of 2);
                                                     RESum(connection, left, right)
                                                )
                                             in RESumService()
                                           | (nu re_star:1=>2<rec %regexp.
                                                              2<<
                                                              { ^Delete[[true]]:
                                                                Lend,
                                                                ^Match[[true]]:
                                                                2 << <rec %intlist.
                                                                  2<<
                                                                  { ^Delete[[true]]:
                                                                    Lend,
                                                                    ^Pop[[true]]:
                                                                    %intlist,
                                                                    ^Read[[true]]:
                                                                    2>>
                                                                    { ^NONE[[true]]:
                                                                      %intlist,
                                                                      ^SOME[[true]]:
                                                                      2 >> <Int>
                                                                      %intlist
                                                                    }
                                                                  }@(1 of 2)>;
                                                                2 >> <Bool>
                                                                %regexp
                                                              }@(1 of 2)>;
                                                         rec $regexp.
                                                         1=>2
                                                         { ^Delete[[true]]:
                                                             Gend,
                                                           ^Match[[true]]:
                                                             1=>2<rec %intlist.
                                                                  2<<
                                                                  { ^Delete[[true]]:
                                                                    Lend,
                                                                    ^Pop[[true]]:
                                                                    %intlist,
                                                                    ^Read[[true]]:
                                                                    2>>
                                                                    { ^NONE[[true]]:
                                                                      %intlist,
                                                                      ^SOME[[true]]:
                                                                      2 >> <Int>
                                                                      %intlist
                                                                    }
                                                                  }@(1 of 2)>;
                                                             2=>1<Bool>;
                                                             $regexp
                                                         })
                                               ( def REStarService()=
                                                    link(2,re_star,connection,2);
                                                    ( REStarService()
                                                    | def REStar(this: rec %regexp.
                                                                       1>>
                                                                       { ^Delete[[true]]:
                                                                         Lend,
                                                                         ^Match[[true]]:
                                                                         1 >> <rec %intlist.
                                                                           2<<
                                                                           { ^Delete[[true]]:
                                                                             Lend,
                                                                             ^Pop[[true]]:
                                                                             %intlist,
                                                                             ^Read[[true]]:
                                                                             2>>
                                                                             { ^NONE[[true]]:
                                                                               %intlist,
                                                                               ^SOME[[true]]:
                                                                               2 >> <Int>
                                                                               %intlist
                                                                             }
                                                                           }@(1 of 2)>
                                                                         1 << <Bool>;
                                                                         %regexp
                                                                       }@(2 of 2),
                                                                 body: rec %regexp.
                                                                       2<<
                                                                       { ^Delete[[true]]:
                                                                         Lend,
                                                                         ^Match[[true]]:
                                                                         2 << <rec %intlist.
                                                                           2<<
                                                                           { ^Delete[[true]]:
                                                                             Lend,
                                                                             ^Pop[[true]]:
                                                                             %intlist,
                                                                             ^Read[[true]]:
                                                                             2>>
                                                                             { ^NONE[[true]]:
                                                                               %intlist,
                                                                               ^SOME[[true]]:
                                                                               2 >> <Int>
                                                                               %intlist
                                                                             }
                                                                           }@(1 of 2)>;
                                                                         2 >> <Bool>
                                                                         %regexp
                                                                       }@(1 of 2))=
                                                         this[1] >>
                                                         { ^Delete:
                                                           body[2] << ^Delete;
                                                           end,
                                                           ^Match:
                                                           this[1] >> str1@(1 of 2);
                                                           str1[2] << ^Delete;
                                                           this[1] << false;
                                                           REStar(this, body)
                                                         }
                                                      in connection[1] >> body@(1 of 2);
                                                         REStar(connection, body)
                                                    )
                                                 in REStarService()
                                               | link(2,newlist,list1,1);
                                                 link(2,cons,cs,1);
                                                 cs[2] << 1;
                                                 cs[2] << list1;
                                                 cs[2] >> list1@(1 of 2);
                                                 link(2,cons,cs,1);
                                                 cs[2] << 2;
                                                 cs[2] << list1;
                                                 cs[2] >> list1@(1 of 2);
                                                 link(2,cons,cs,1);
                                                 cs[2] << 2;
                                                 cs[2] << list1;
                                                 cs[2] >> list1@(1 of 2);
                                                 link(2,cons,cs,1);
                                                 cs[2] << 1;
                                                 cs[2] << list1;
                                                 cs[2] >> list1@(1 of 2);
                                                 link(2,re_literal,lit_a1,1);
                                                 lit_a1[2] << 1;
                                                 link(2,re_literal,lit_a2,1);
                                                 lit_a2[2] << 1;
                                                 link(2,re_literal,lit_a3,1);
                                                 lit_a3[2] << 1;
                                                 link(2,re_literal,lit_a4,1);
                                                 lit_a4[2] << 1;
                                                 link(2,re_literal,lit_b1,1);
                                                 lit_b1[2] << 2;
                                                 link(2,re_literal,lit_b2,1);
                                                 lit_b2[2] << 2;
                                                 link(2,re_sum,sum_ab1,1);
                                                 sum_ab1[2] << lit_a1;
                                                 sum_ab1[2] << lit_b1;
                                                 link(2,re_sum,sum_ab2,1);
                                                 sum_ab2[2] << lit_a2;
                                                 sum_ab2[2] << lit_b2;
                                                 link(2,re_sequence,re1,1);
                                                 re1[2] << lit_a3;
                                                 re1[2] << sum_ab1;
                                                 link(2,re_sequence,re2,1);
                                                 re2[2] << re1;
                                                 re2[2] << sum_ab2;
                                                 link(2,re_sequence,re3,1);
                                                 re3[2] << re2;
                                                 re3[2] << lit_a4;
                                                 re3[2] << ^Match;
                                                 re3[2] << list1;
                                                 re3[2] >> x;
                                                 re3[2] << ^Delete;
                                                 end
                                               )
                                           )
                                       )
                                   )
                     )
                 )
             )
         )
     )
************ Type Checking Program ************
TYPE: rec %intlist.
1>>
{ ^Delete[[true]]:
  Lend,
  ^Pop[[true]]:
  %intlist,
  ^Read[[true]]:
  1<<
  { ^NONE[[true]]:
    %intlist,
    ^SOME[[true]]:
    1 << <Int>;
    %intlist
  }
}
ISDONE: 0
TYPE: Lend
ISDONE: 1
TYPE: Lend
ISDONE: 1
TYPE: 2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: 2 << <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>;
2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: Lend
ISDONE: 1
TYPE: 1 << <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>;
Lend
ISDONE: 0
TYPE: Lend
ISDONE: 1
TYPE: 2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: 2 << <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>;
2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: 1 << <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>;
Lend
ISDONE: 0
TYPE: 2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: 2 << <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>;
2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: 2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: Lend
ISDONE: 1
TYPE: 2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: 2 << <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>;
2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: 2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: 2 << <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>;
2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: rec %regexp.
1>>
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  1 >> <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>
  1 << <Bool>;
  %regexp
}
ISDONE: 0
TYPE: rec %regexp.
1>>
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  1 >> <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>
  1 << <Bool>;
  %regexp
}
ISDONE: 0
TYPE: rec %regexp.
1>>
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  1 >> <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>
  1 << <Bool>;
  %regexp
}
ISDONE: 0
TYPE: rec %regexp.
1>>
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  1 >> <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>
  1 << <Bool>;
  %regexp
}
ISDONE: 0
TYPE: rec %regexp.
1>>
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  1 >> <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>
  1 << <Bool>;
  %regexp
}
ISDONE: 0
TYPE: 2 >> <Bool>
rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: 2 >> <Bool>
rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: 2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: rec %regexp.
1>>
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  1 >> <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>
  1 << <Bool>;
  %regexp
}
ISDONE: 0
TYPE: 2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: 2 << <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>;
2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: 2 >> <Bool>
rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: 2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: 2 >> <Bool>
rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: 2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: 2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: rec %regexp.
1>>
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  1 >> <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>
  1 << <Bool>;
  %regexp
}
ISDONE: 0
TYPE: 2 >> <Bool>
rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: 2 >> <Bool>
rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: 2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: rec %regexp.
1>>
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  1 >> <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>
  1 << <Bool>;
  %regexp
}
ISDONE: 0
TYPE: 2 >> <Bool>
rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: 2 << <rec %regexp.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Match[[true]]:
    2 << <rec %intlist.
      2<<
      { ^Delete[[true]]:
        Lend,
        ^Pop[[true]]:
        %intlist,
        ^Read[[true]]:
        2>>
        { ^NONE[[true]]:
          %intlist,
          ^SOME[[true]]:
          2 >> <Int>
          %intlist
        }
      }@(1 of 2)>;
    2 >> <Bool>
    %regexp
  }@(1 of 2)>;
rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: 2 << <rec %regexp.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Match[[true]]:
    2 << <rec %intlist.
      2<<
      { ^Delete[[true]]:
        Lend,
        ^Pop[[true]]:
        %intlist,
        ^Read[[true]]:
        2>>
        { ^NONE[[true]]:
          %intlist,
          ^SOME[[true]]:
          2 >> <Int>
          %intlist
        }
      }@(1 of 2)>;
    2 >> <Bool>
    %regexp
  }@(1 of 2)>;
rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: 2 << <rec %regexp.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Match[[true]]:
    2 << <rec %intlist.
      2<<
      { ^Delete[[true]]:
        Lend,
        ^Pop[[true]]:
        %intlist,
        ^Read[[true]]:
        2>>
        { ^NONE[[true]]:
          %intlist,
          ^SOME[[true]]:
          2 >> <Int>
          %intlist
        }
      }@(1 of 2)>;
    2 >> <Bool>
    %regexp
  }@(1 of 2)>;
rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: 2 << <rec %regexp.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Match[[true]]:
    2 << <rec %intlist.
      2<<
      { ^Delete[[true]]:
        Lend,
        ^Pop[[true]]:
        %intlist,
        ^Read[[true]]:
        2>>
        { ^NONE[[true]]:
          %intlist,
          ^SOME[[true]]:
          2 >> <Int>
          %intlist
        }
      }@(1 of 2)>;
    2 >> <Bool>
    %regexp
  }@(1 of 2)>;
rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: 2 << <rec %regexp.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Match[[true]]:
    2 << <rec %intlist.
      2<<
      { ^Delete[[true]]:
        Lend,
        ^Pop[[true]]:
        %intlist,
        ^Read[[true]]:
        2>>
        { ^NONE[[true]]:
          %intlist,
          ^SOME[[true]]:
          2 >> <Int>
          %intlist
        }
      }@(1 of 2)>;
    2 >> <Bool>
    %regexp
  }@(1 of 2)>;
rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: rec %regexp.
2<<
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  2 << <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>;
  2 >> <Bool>
  %regexp
}
ISDONE: 0
TYPE: 2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: 2 << <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>;
2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: 2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: 2 << <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>;
2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: 2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: 2 << <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>;
2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: 2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: 2 << <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>;
2 >> <rec %intlist.
  2<<
  { ^Delete[[true]]:
    Lend,
    ^Pop[[true]]:
    %intlist,
    ^Read[[true]]:
    2>>
    { ^NONE[[true]]:
      %intlist,
      ^SOME[[true]]:
      2 >> <Int>
      %intlist
    }
  }@(1 of 2)>
Lend
ISDONE: 0
TYPE: rec %regexp.
1>>
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  1 >> <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>
  1 << <Bool>;
  %regexp
}
ISDONE: 0
TYPE: rec %regexp.
1>>
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  1 >> <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>
  1 << <Bool>;
  %regexp
}
ISDONE: 0
TYPE: rec %regexp.
1>>
{ ^Delete[[true]]:
  Lend,
  ^Match[[true]]:
  1 >> <rec %intlist.
    2<<
    { ^Delete[[true]]:
      Lend,
      ^Pop[[true]]:
      %intlist,
      ^Read[[true]]:
      2>>
      { ^NONE[[true]]:
        %intlist,
        ^SOME[[true]]:
        2 >> <Int>
        %intlist
      }
    }@(1 of 2)>
  1 << <Bool>;
  %regexp
}
ISDONE: 0
************ Type Check Succeeded! ************
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 1
ToC: ISDONE: 1
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 1
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 1
ToC: ISDONE: 1
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
ToC: ISDONE: 0
*/
#include <unistd.h>
#include <vector>
#include <iostream>
#include <libpi/session_fifo.hpp>
#include <libpi/value.hpp>
using namespace std;
using namespace libpi;


/* Procedure declerations */
class Cnt
{
  public:
    Cnt() {}
    virtual ~Cnt() {}
    virtual bool IsEmpty() { return true; }
    virtual Cnt *Run() { return new Cnt(); }
};
Cnt *__SIM__EmptyList1(Session* &__SIM__this2);
class __Cnt____SIM__EmptyList1 : public Cnt
{
  public:
    __Cnt____SIM__EmptyList1(Session* &__SIM__this2): __SIM__this2(__SIM__this2)
{}
    virtual ~__Cnt____SIM__EmptyList1() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__EmptyList1(__SIM__this2); }
  private:
Session* __SIM__this2;
};
;
Cnt *__SIM__NewList2(ChannelsValue &__SIM__newlist1);
class __Cnt____SIM__NewList2 : public Cnt
{
  public:
    __Cnt____SIM__NewList2(ChannelsValue &__SIM__newlist1): __SIM__newlist1(__SIM__newlist1)
{}
    virtual ~__Cnt____SIM__NewList2() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__NewList2(__SIM__newlist1); }
  private:
ChannelsValue __SIM__newlist1;
};
;
Cnt *__SIM__Cons4(Session* &__SIM__this11,Session* &__SIM__tail12,IntValue &__SIM__val13);
class __Cnt____SIM__Cons4 : public Cnt
{
  public:
    __Cnt____SIM__Cons4(Session* &__SIM__this11,Session* &__SIM__tail12,IntValue &__SIM__val13): __SIM__this11(__SIM__this11)
, __SIM__tail12(__SIM__tail12)
, __SIM__val13(__SIM__val13)
{}
    virtual ~__Cnt____SIM__Cons4() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__Cons4(__SIM__this11, __SIM__tail12, __SIM__val13); }
  private:
Session* __SIM__this11;
Session* __SIM__tail12;
IntValue __SIM__val13;
};
;
Cnt *__SIM__ConsService3(ChannelsValue &__SIM__cons5);
class __Cnt____SIM__ConsService3 : public Cnt
{
  public:
    __Cnt____SIM__ConsService3(ChannelsValue &__SIM__cons5): __SIM__cons5(__SIM__cons5)
{}
    virtual ~__Cnt____SIM__ConsService3() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__ConsService3(__SIM__cons5); }
  private:
ChannelsValue __SIM__cons5;
};
;
Cnt *__SIM__ListRev6(Session* &__SIM__lst23,Session* &__SIM__rev24,Session* &__SIM__dest25,ChannelsValue &__SIM__cons5);
class __Cnt____SIM__ListRev6 : public Cnt
{
  public:
    __Cnt____SIM__ListRev6(Session* &__SIM__lst23,Session* &__SIM__rev24,Session* &__SIM__dest25,ChannelsValue &__SIM__cons5): __SIM__lst23(__SIM__lst23)
, __SIM__rev24(__SIM__rev24)
, __SIM__dest25(__SIM__dest25)
, __SIM__cons5(__SIM__cons5)
{}
    virtual ~__Cnt____SIM__ListRev6() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__ListRev6(__SIM__lst23, __SIM__rev24, __SIM__dest25, __SIM__cons5); }
  private:
Session* __SIM__lst23;
Session* __SIM__rev24;
Session* __SIM__dest25;
ChannelsValue __SIM__cons5;
};
;
Cnt *__SIM__ListRevService5(ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__listrev21,ChannelsValue &__SIM__newlist1);
class __Cnt____SIM__ListRevService5 : public Cnt
{
  public:
    __Cnt____SIM__ListRevService5(ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__listrev21,ChannelsValue &__SIM__newlist1): __SIM__cons5(__SIM__cons5)
, __SIM__listrev21(__SIM__listrev21)
, __SIM__newlist1(__SIM__newlist1)
{}
    virtual ~__Cnt____SIM__ListRevService5() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__ListRevService5(__SIM__cons5, __SIM__listrev21, __SIM__newlist1); }
  private:
ChannelsValue __SIM__cons5;
ChannelsValue __SIM__listrev21;
ChannelsValue __SIM__newlist1;
};
;
Cnt *__SIM__ListCopy8(Session* &__SIM__lst40,Session* &__SIM__dest41,ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__listcopy38,ChannelsValue &__SIM__newlist1);
class __Cnt____SIM__ListCopy8 : public Cnt
{
  public:
    __Cnt____SIM__ListCopy8(Session* &__SIM__lst40,Session* &__SIM__dest41,ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__listcopy38,ChannelsValue &__SIM__newlist1): __SIM__lst40(__SIM__lst40)
, __SIM__dest41(__SIM__dest41)
, __SIM__cons5(__SIM__cons5)
, __SIM__listcopy38(__SIM__listcopy38)
, __SIM__newlist1(__SIM__newlist1)
{}
    virtual ~__Cnt____SIM__ListCopy8() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__ListCopy8(__SIM__lst40, __SIM__dest41, __SIM__cons5, __SIM__listcopy38, __SIM__newlist1); }
  private:
Session* __SIM__lst40;
Session* __SIM__dest41;
ChannelsValue __SIM__cons5;
ChannelsValue __SIM__listcopy38;
ChannelsValue __SIM__newlist1;
};
;
Cnt *__SIM__ListCopyService7(ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__listcopy38,ChannelsValue &__SIM__newlist1);
class __Cnt____SIM__ListCopyService7 : public Cnt
{
  public:
    __Cnt____SIM__ListCopyService7(ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__listcopy38,ChannelsValue &__SIM__newlist1): __SIM__cons5(__SIM__cons5)
, __SIM__listcopy38(__SIM__listcopy38)
, __SIM__newlist1(__SIM__newlist1)
{}
    virtual ~__Cnt____SIM__ListCopyService7() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__ListCopyService7(__SIM__cons5, __SIM__listcopy38, __SIM__newlist1); }
  private:
ChannelsValue __SIM__cons5;
ChannelsValue __SIM__listcopy38;
ChannelsValue __SIM__newlist1;
};
;
Cnt *__SIM__AppendRev11(Session* &__SIM__lhs77,Session* &__SIM__rhs78,Session* &__SIM__dest79,ChannelsValue &__SIM__cons5);
class __Cnt____SIM__AppendRev11 : public Cnt
{
  public:
    __Cnt____SIM__AppendRev11(Session* &__SIM__lhs77,Session* &__SIM__rhs78,Session* &__SIM__dest79,ChannelsValue &__SIM__cons5): __SIM__lhs77(__SIM__lhs77)
, __SIM__rhs78(__SIM__rhs78)
, __SIM__dest79(__SIM__dest79)
, __SIM__cons5(__SIM__cons5)
{}
    virtual ~__Cnt____SIM__AppendRev11() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__AppendRev11(__SIM__lhs77, __SIM__rhs78, __SIM__dest79, __SIM__cons5); }
  private:
Session* __SIM__lhs77;
Session* __SIM__rhs78;
Session* __SIM__dest79;
ChannelsValue __SIM__cons5;
};
;
Cnt *__SIM__Append10(Session* &__SIM__lhs69,Session* &__SIM__rhs70,Session* &__SIM__stack71,Session* &__SIM__dest72,ChannelsValue &__SIM__cons5);
class __Cnt____SIM__Append10 : public Cnt
{
  public:
    __Cnt____SIM__Append10(Session* &__SIM__lhs69,Session* &__SIM__rhs70,Session* &__SIM__stack71,Session* &__SIM__dest72,ChannelsValue &__SIM__cons5): __SIM__lhs69(__SIM__lhs69)
, __SIM__rhs70(__SIM__rhs70)
, __SIM__stack71(__SIM__stack71)
, __SIM__dest72(__SIM__dest72)
, __SIM__cons5(__SIM__cons5)
{}
    virtual ~__Cnt____SIM__Append10() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__Append10(__SIM__lhs69, __SIM__rhs70, __SIM__stack71, __SIM__dest72, __SIM__cons5); }
  private:
Session* __SIM__lhs69;
Session* __SIM__rhs70;
Session* __SIM__stack71;
Session* __SIM__dest72;
ChannelsValue __SIM__cons5;
};
;
Cnt *__SIM__AppendService9(ChannelsValue &__SIM__append67,ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__newlist1);
class __Cnt____SIM__AppendService9 : public Cnt
{
  public:
    __Cnt____SIM__AppendService9(ChannelsValue &__SIM__append67,ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__newlist1): __SIM__append67(__SIM__append67)
, __SIM__cons5(__SIM__cons5)
, __SIM__newlist1(__SIM__newlist1)
{}
    virtual ~__Cnt____SIM__AppendService9() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__AppendService9(__SIM__append67, __SIM__cons5, __SIM__newlist1); }
  private:
ChannelsValue __SIM__append67;
ChannelsValue __SIM__cons5;
ChannelsValue __SIM__newlist1;
};
;
Cnt *__SIM__RE013(Session* &__SIM__this101);
class __Cnt____SIM__RE013 : public Cnt
{
  public:
    __Cnt____SIM__RE013(Session* &__SIM__this101): __SIM__this101(__SIM__this101)
{}
    virtual ~__Cnt____SIM__RE013() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__RE013(__SIM__this101); }
  private:
Session* __SIM__this101;
};
;
Cnt *__SIM__RE0Service12(ChannelsValue &__SIM__re_099);
class __Cnt____SIM__RE0Service12 : public Cnt
{
  public:
    __Cnt____SIM__RE0Service12(ChannelsValue &__SIM__re_099): __SIM__re_099(__SIM__re_099)
{}
    virtual ~__Cnt____SIM__RE0Service12() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__RE0Service12(__SIM__re_099); }
  private:
ChannelsValue __SIM__re_099;
};
;
Cnt *__SIM__RE115(Session* &__SIM__this109);
class __Cnt____SIM__RE115 : public Cnt
{
  public:
    __Cnt____SIM__RE115(Session* &__SIM__this109): __SIM__this109(__SIM__this109)
{}
    virtual ~__Cnt____SIM__RE115() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__RE115(__SIM__this109); }
  private:
Session* __SIM__this109;
};
;
Cnt *__SIM__RE1Service14(ChannelsValue &__SIM__re_1107);
class __Cnt____SIM__RE1Service14 : public Cnt
{
  public:
    __Cnt____SIM__RE1Service14(ChannelsValue &__SIM__re_1107): __SIM__re_1107(__SIM__re_1107)
{}
    virtual ~__Cnt____SIM__RE1Service14() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__RE1Service14(__SIM__re_1107); }
  private:
ChannelsValue __SIM__re_1107;
};
;
Cnt *__SIM__RELiteral17(Session* &__SIM__this118,IntValue &__SIM__literal119);
class __Cnt____SIM__RELiteral17 : public Cnt
{
  public:
    __Cnt____SIM__RELiteral17(Session* &__SIM__this118,IntValue &__SIM__literal119): __SIM__this118(__SIM__this118)
, __SIM__literal119(__SIM__literal119)
{}
    virtual ~__Cnt____SIM__RELiteral17() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__RELiteral17(__SIM__this118, __SIM__literal119); }
  private:
Session* __SIM__this118;
IntValue __SIM__literal119;
};
;
Cnt *__SIM__RELiteralService16(ChannelsValue &__SIM__re_literal116);
class __Cnt____SIM__RELiteralService16 : public Cnt
{
  public:
    __Cnt____SIM__RELiteralService16(ChannelsValue &__SIM__re_literal116): __SIM__re_literal116(__SIM__re_literal116)
{}
    virtual ~__Cnt____SIM__RELiteralService16() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__RELiteralService16(__SIM__re_literal116); }
  private:
ChannelsValue __SIM__re_literal116;
};
;
Cnt *__SIM__MatchSeq20(Session* &__SIM__this142,Session* &__SIM__left143,Session* &__SIM__right144,Session* &__SIM__str1145,Session* &__SIM__str2146,ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__listcopy38,ChannelsValue &__SIM__listrev21,ChannelsValue &__SIM__newlist1);
class __Cnt____SIM__MatchSeq20 : public Cnt
{
  public:
    __Cnt____SIM__MatchSeq20(Session* &__SIM__this142,Session* &__SIM__left143,Session* &__SIM__right144,Session* &__SIM__str1145,Session* &__SIM__str2146,ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__listcopy38,ChannelsValue &__SIM__listrev21,ChannelsValue &__SIM__newlist1): __SIM__this142(__SIM__this142)
, __SIM__left143(__SIM__left143)
, __SIM__right144(__SIM__right144)
, __SIM__str1145(__SIM__str1145)
, __SIM__str2146(__SIM__str2146)
, __SIM__cons5(__SIM__cons5)
, __SIM__listcopy38(__SIM__listcopy38)
, __SIM__listrev21(__SIM__listrev21)
, __SIM__newlist1(__SIM__newlist1)
{}
    virtual ~__Cnt____SIM__MatchSeq20() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__MatchSeq20(__SIM__this142, __SIM__left143, __SIM__right144, __SIM__str1145, __SIM__str2146, __SIM__cons5, __SIM__listcopy38, __SIM__listrev21, __SIM__newlist1); }
  private:
Session* __SIM__this142;
Session* __SIM__left143;
Session* __SIM__right144;
Session* __SIM__str1145;
Session* __SIM__str2146;
ChannelsValue __SIM__cons5;
ChannelsValue __SIM__listcopy38;
ChannelsValue __SIM__listrev21;
ChannelsValue __SIM__newlist1;
};
;
Cnt *__SIM__RESequence19(Session* &__SIM__this130,Session* &__SIM__left131,Session* &__SIM__right132,ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__listcopy38,ChannelsValue &__SIM__listrev21,ChannelsValue &__SIM__newlist1);
class __Cnt____SIM__RESequence19 : public Cnt
{
  public:
    __Cnt____SIM__RESequence19(Session* &__SIM__this130,Session* &__SIM__left131,Session* &__SIM__right132,ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__listcopy38,ChannelsValue &__SIM__listrev21,ChannelsValue &__SIM__newlist1): __SIM__this130(__SIM__this130)
, __SIM__left131(__SIM__left131)
, __SIM__right132(__SIM__right132)
, __SIM__cons5(__SIM__cons5)
, __SIM__listcopy38(__SIM__listcopy38)
, __SIM__listrev21(__SIM__listrev21)
, __SIM__newlist1(__SIM__newlist1)
{}
    virtual ~__Cnt____SIM__RESequence19() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__RESequence19(__SIM__this130, __SIM__left131, __SIM__right132, __SIM__cons5, __SIM__listcopy38, __SIM__listrev21, __SIM__newlist1); }
  private:
Session* __SIM__this130;
Session* __SIM__left131;
Session* __SIM__right132;
ChannelsValue __SIM__cons5;
ChannelsValue __SIM__listcopy38;
ChannelsValue __SIM__listrev21;
ChannelsValue __SIM__newlist1;
};
;
Cnt *__SIM__RESequenceService18(ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__listcopy38,ChannelsValue &__SIM__listrev21,ChannelsValue &__SIM__newlist1,ChannelsValue &__SIM__re_sequence128);
class __Cnt____SIM__RESequenceService18 : public Cnt
{
  public:
    __Cnt____SIM__RESequenceService18(ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__listcopy38,ChannelsValue &__SIM__listrev21,ChannelsValue &__SIM__newlist1,ChannelsValue &__SIM__re_sequence128): __SIM__cons5(__SIM__cons5)
, __SIM__listcopy38(__SIM__listcopy38)
, __SIM__listrev21(__SIM__listrev21)
, __SIM__newlist1(__SIM__newlist1)
, __SIM__re_sequence128(__SIM__re_sequence128)
{}
    virtual ~__Cnt____SIM__RESequenceService18() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__RESequenceService18(__SIM__cons5, __SIM__listcopy38, __SIM__listrev21, __SIM__newlist1, __SIM__re_sequence128); }
  private:
ChannelsValue __SIM__cons5;
ChannelsValue __SIM__listcopy38;
ChannelsValue __SIM__listrev21;
ChannelsValue __SIM__newlist1;
ChannelsValue __SIM__re_sequence128;
};
;
Cnt *__SIM__RESum22(Session* &__SIM__this205,Session* &__SIM__left206,Session* &__SIM__right207,ChannelsValue &__SIM__listcopy38);
class __Cnt____SIM__RESum22 : public Cnt
{
  public:
    __Cnt____SIM__RESum22(Session* &__SIM__this205,Session* &__SIM__left206,Session* &__SIM__right207,ChannelsValue &__SIM__listcopy38): __SIM__this205(__SIM__this205)
, __SIM__left206(__SIM__left206)
, __SIM__right207(__SIM__right207)
, __SIM__listcopy38(__SIM__listcopy38)
{}
    virtual ~__Cnt____SIM__RESum22() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__RESum22(__SIM__this205, __SIM__left206, __SIM__right207, __SIM__listcopy38); }
  private:
Session* __SIM__this205;
Session* __SIM__left206;
Session* __SIM__right207;
ChannelsValue __SIM__listcopy38;
};
;
Cnt *__SIM__RESumService21(ChannelsValue &__SIM__listcopy38,ChannelsValue &__SIM__re_sum203);
class __Cnt____SIM__RESumService21 : public Cnt
{
  public:
    __Cnt____SIM__RESumService21(ChannelsValue &__SIM__listcopy38,ChannelsValue &__SIM__re_sum203): __SIM__listcopy38(__SIM__listcopy38)
, __SIM__re_sum203(__SIM__re_sum203)
{}
    virtual ~__Cnt____SIM__RESumService21() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__RESumService21(__SIM__listcopy38, __SIM__re_sum203); }
  private:
ChannelsValue __SIM__listcopy38;
ChannelsValue __SIM__re_sum203;
};
;
Cnt *__SIM__REStar24(Session* &__SIM__this239,Session* &__SIM__body240);
class __Cnt____SIM__REStar24 : public Cnt
{
  public:
    __Cnt____SIM__REStar24(Session* &__SIM__this239,Session* &__SIM__body240): __SIM__this239(__SIM__this239)
, __SIM__body240(__SIM__body240)
{}
    virtual ~__Cnt____SIM__REStar24() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__REStar24(__SIM__this239, __SIM__body240); }
  private:
Session* __SIM__this239;
Session* __SIM__body240;
};
;
Cnt *__SIM__REStarService23(ChannelsValue &__SIM__re_star237);
class __Cnt____SIM__REStarService23 : public Cnt
{
  public:
    __Cnt____SIM__REStarService23(ChannelsValue &__SIM__re_star237): __SIM__re_star237(__SIM__re_star237)
{}
    virtual ~__Cnt____SIM__REStarService23() {}
    virtual bool IsEmpty() { return false; }
    virtual Cnt *Run() { return __SIM__REStarService23(__SIM__re_star237); }
  private:
ChannelsValue __SIM__re_star237;
};
;

/* Procedure implementations */
Cnt *__SIM__EmptyList1(Session* &__SIM__this2)
{
  StringValue __SIM__branch511;
  {
    Message __SIM__branch512;
    __SIM__this2->Receive(0, __SIM__branch512);
    __SIM__branch512.GetValue(__SIM__branch511);
  }
  if (__SIM__branch511.ToString()=="^Delete")
  {
  return new Cnt();
  }
  else   if (__SIM__branch511.ToString()=="^Pop")
  {
  return new __Cnt____SIM__EmptyList1(__SIM__this2);
  }
  else   if (__SIM__branch511.ToString()=="^Read")
  {
  {
    Message  __SIM__select513;
    __SIM__select513.AddData("^NONE", 6);
__SIM__this2->Send(0,__SIM__select513);
  }
  return new __Cnt____SIM__EmptyList1(__SIM__this2);
  }
  else throw (string)"Unknown branch: __SIM__branch511";
}Cnt *__SIM__NewList2(ChannelsValue &__SIM__newlist1)
{
  Session*__SIM__s4=new Session_FIFO(__SIM__newlist1.GetValues(), 1, 2);
  int __SIM__fork514=fork();
  if (__SIM__fork514>0)
  {
    __SIM__s4->Close(false);
    delete __SIM__s4;
  return new __Cnt____SIM__NewList2(__SIM__newlist1);
  }
  else if (__SIM__fork514==0)
  {
  return new __Cnt____SIM__EmptyList1(__SIM__s4);
  }
else throw (string)"Error during fork!";
return 0;
}Cnt *__SIM__Cons4(Session* &__SIM__this11,Session* &__SIM__tail12,IntValue &__SIM__val13)
{
  StringValue __SIM__branch515;
  {
    Message __SIM__branch516;
    __SIM__this11->Receive(0, __SIM__branch516);
    __SIM__branch516.GetValue(__SIM__branch515);
  }
  if (__SIM__branch515.ToString()=="^Delete")
  {
  {
    Message  __SIM__select517;
    __SIM__select517.AddData("^Delete", 8);
__SIM__tail12->Send(1,__SIM__select517);
  }
  return new Cnt();
  __SIM__tail12->Close(true);
  delete __SIM__tail12;
  }
  else   if (__SIM__branch515.ToString()=="^Pop")
  {
  {
    Message  __SIM__select518;
    __SIM__select518.AddData("^Read", 6);
__SIM__tail12->Send(1,__SIM__select518);
  }
  StringValue __SIM__branch519;
  {
    Message __SIM__branch520;
    __SIM__tail12->Receive(1, __SIM__branch520);
    __SIM__branch520.GetValue(__SIM__branch519);
  }
  if (__SIM__branch519.ToString()=="^NONE")
  {
  {
    Message  __SIM__select521;
    __SIM__select521.AddData("^Delete", 8);
__SIM__tail12->Send(1,__SIM__select521);
  }
  return new __Cnt____SIM__EmptyList1(__SIM__this11);
  __SIM__tail12->Close(true);
  delete __SIM__tail12;
  }
  else   if (__SIM__branch519.ToString()=="^SOME")
  {
  IntValue __SIM__newval16;
  { Message __SIM__receive522;
    __SIM__tail12->Receive(1,__SIM__receive522);
    __SIM__receive522.GetValue(__SIM__newval16);
  }
  {
    Message  __SIM__select523;
    __SIM__select523.AddData("^Pop", 5);
__SIM__tail12->Send(1,__SIM__select523);
  }
  return new __Cnt____SIM__Cons4(__SIM__this11, __SIM__tail12, __SIM__newval16);
  }
  else throw (string)"Unknown branch: __SIM__branch519";
  }
  else   if (__SIM__branch515.ToString()=="^Read")
  {
  {
    Message  __SIM__select524;
    __SIM__select524.AddData("^SOME", 6);
__SIM__this11->Send(0,__SIM__select524);
  }
  { 
    Message  __SIM__send525;
    __SIM__val13.ToMessage(__SIM__send525);
    __SIM__this11->Send(0,__SIM__send525);
  }
  return new __Cnt____SIM__Cons4(__SIM__this11, __SIM__tail12, __SIM__val13);
  }
  else throw (string)"Unknown branch: __SIM__branch515";
}Cnt *__SIM__ConsService3(ChannelsValue &__SIM__cons5)
{
  Session*__SIM__s6=new Session_FIFO(__SIM__cons5.GetValues(), 1, 2);
  int __SIM__fork526=fork();
  if (__SIM__fork526>0)
  {
    __SIM__s6->Close(false);
    delete __SIM__s6;
  return new __Cnt____SIM__ConsService3(__SIM__cons5);
  }
  else if (__SIM__fork526==0)
  {
  IntValue __SIM__elt7;
  { Message __SIM__receive527;
    __SIM__s6->Receive(0,__SIM__receive527);
    __SIM__receive527.GetValue(__SIM__elt7);
  }
  Session* __SIM__tail8;
    __SIM__tail8=__SIM__s6->ReceiveSession(0);
  vector<Channel*> __SIM__chvector528;
  __SIM__chvector528.push_back(new Channel_FIFO());
  ChannelsValue __SIM__locallink9(__SIM__chvector528);
  int __SIM__fork529=fork();
  if (__SIM__fork529>0)
  {
    __SIM__s6->Close(false);
    delete __SIM__s6;
  Session*__SIM__lnk10=new Session_FIFO(__SIM__locallink9.GetValues(), 1, 2);
  return new __Cnt____SIM__Cons4(__SIM__lnk10, __SIM__tail8, __SIM__elt7);
  }
  else if (__SIM__fork529==0)
  {
    __SIM__tail8->Close(false);
    delete __SIM__tail8;
  Session*__SIM__lnk17=new Session_FIFO(__SIM__locallink9.GetValues(), 0, 2);
  { 
    __SIM__s6->Delegate(0, *__SIM__lnk17);
  }
  __SIM__lnk17->Close(false);
  delete __SIM__lnk17;
  __SIM__s6->Close(true);
  delete __SIM__s6;
  return new Cnt();
  }
else throw (string)"Error during fork!";
return 0;
  }
else throw (string)"Error during fork!";
return 0;
}Cnt *__SIM__ListRev6(Session* &__SIM__lst23,Session* &__SIM__rev24,Session* &__SIM__dest25,ChannelsValue &__SIM__cons5)
{
  {
    Message  __SIM__select531;
    __SIM__select531.AddData("^Read", 6);
__SIM__lst23->Send(1,__SIM__select531);
  }
  StringValue __SIM__branch532;
  {
    Message __SIM__branch533;
    __SIM__lst23->Receive(1, __SIM__branch533);
    __SIM__branch533.GetValue(__SIM__branch532);
  }
  if (__SIM__branch532.ToString()=="^NONE")
  {
  {
    Message  __SIM__select534;
    __SIM__select534.AddData("^Delete", 8);
__SIM__lst23->Send(1,__SIM__select534);
  }
  { 
    __SIM__dest25->Delegate(0, *__SIM__rev24);
  }
  __SIM__rev24->Close(false);
  delete __SIM__rev24;
  __SIM__dest25->Close(true);
  delete __SIM__dest25;
  return new Cnt();
  __SIM__lst23->Close(true);
  delete __SIM__lst23;
  }
  else   if (__SIM__branch532.ToString()=="^SOME")
  {
  IntValue __SIM__head30;
  { Message __SIM__receive536;
    __SIM__lst23->Receive(1,__SIM__receive536);
    __SIM__receive536.GetValue(__SIM__head30);
  }
  {
    Message  __SIM__select537;
    __SIM__select537.AddData("^Pop", 5);
__SIM__lst23->Send(1,__SIM__select537);
  }
  Session*__SIM__cs31=new Session_FIFO(__SIM__cons5.GetValues(), 0, 2);
  { 
    Message  __SIM__send538;
    __SIM__head30.ToMessage(__SIM__send538);
    __SIM__cs31->Send(1,__SIM__send538);
  }
  { 
    __SIM__cs31->Delegate(1, *__SIM__rev24);
  }
  __SIM__rev24->Close(false);
  delete __SIM__rev24;
  Session* __SIM__newrev32;
    __SIM__newrev32=__SIM__cs31->ReceiveSession(1);
  __SIM__cs31->Close(true);
  delete __SIM__cs31;
  return new __Cnt____SIM__ListRev6(__SIM__lst23, __SIM__newrev32, __SIM__dest25, __SIM__cons5);
  }
  else throw (string)"Unknown branch: __SIM__branch532";
}Cnt *__SIM__ListRevService5(ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__listrev21,ChannelsValue &__SIM__newlist1)
{
  Session*__SIM__s22=new Session_FIFO(__SIM__listrev21.GetValues(), 1, 2);
  int __SIM__fork540=fork();
  if (__SIM__fork540>0)
  {
    __SIM__s22->Close(false);
    delete __SIM__s22;
  return new __Cnt____SIM__ListRevService5(__SIM__cons5, __SIM__listrev21, __SIM__newlist1);
  }
  else if (__SIM__fork540==0)
  {
  Session* __SIM__lst35;
    __SIM__lst35=__SIM__s22->ReceiveSession(0);
  Session*__SIM__rev36=new Session_FIFO(__SIM__newlist1.GetValues(), 0, 2);
  return new __Cnt____SIM__ListRev6(__SIM__lst35, __SIM__rev36, __SIM__s22, __SIM__cons5);
  }
else throw (string)"Error during fork!";
return 0;
}Cnt *__SIM__ListCopy8(Session* &__SIM__lst40,Session* &__SIM__dest41,ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__listcopy38,ChannelsValue &__SIM__newlist1)
{
  {
    Message  __SIM__select541;
    __SIM__select541.AddData("^Read", 6);
__SIM__lst40->Send(1,__SIM__select541);
  }
  StringValue __SIM__branch542;
  {
    Message __SIM__branch543;
    __SIM__lst40->Receive(1, __SIM__branch543);
    __SIM__branch543.GetValue(__SIM__branch542);
  }
  if (__SIM__branch542.ToString()=="^NONE")
  {
  Session*__SIM__copylst45=new Session_FIFO(__SIM__newlist1.GetValues(), 0, 2);
  { 
    __SIM__dest41->Delegate(0, *__SIM__copylst45);
  }
  __SIM__copylst45->Close(false);
  delete __SIM__copylst45;
  { 
    __SIM__dest41->Delegate(0, *__SIM__lst40);
  }
  __SIM__lst40->Close(false);
  delete __SIM__lst40;
  __SIM__dest41->Close(true);
  delete __SIM__dest41;
  return new Cnt();
  }
  else   if (__SIM__branch542.ToString()=="^SOME")
  {
  IntValue __SIM__head48;
  { Message __SIM__receive546;
    __SIM__lst40->Receive(1,__SIM__receive546);
    __SIM__receive546.GetValue(__SIM__head48);
  }
  {
    Message  __SIM__select547;
    __SIM__select547.AddData("^Pop", 5);
__SIM__lst40->Send(1,__SIM__select547);
  }
  Session*__SIM__call49=new Session_FIFO(__SIM__listcopy38.GetValues(), 0, 2);
  { 
    __SIM__call49->Delegate(1, *__SIM__lst40);
  }
  __SIM__lst40->Close(false);
  delete __SIM__lst40;
  Session* __SIM__copy150;
    __SIM__copy150=__SIM__call49->ReceiveSession(1);
  Session*__SIM__cons151=new Session_FIFO(__SIM__cons5.GetValues(), 0, 2);
  { 
    Message  __SIM__send549;
    __SIM__head48.ToMessage(__SIM__send549);
    __SIM__cons151->Send(1,__SIM__send549);
  }
  { 
    __SIM__cons151->Delegate(1, *__SIM__copy150);
  }
  __SIM__copy150->Close(false);
  delete __SIM__copy150;
  Session* __SIM__copy152;
    __SIM__copy152=__SIM__cons151->ReceiveSession(1);
  __SIM__cons151->Close(true);
  delete __SIM__cons151;
  { 
    __SIM__dest41->Delegate(0, *__SIM__copy152);
  }
  __SIM__copy152->Close(false);
  delete __SIM__copy152;
  Session* __SIM__copy253;
    __SIM__copy253=__SIM__call49->ReceiveSession(1);
  __SIM__call49->Close(true);
  delete __SIM__call49;
  Session*__SIM__cons254=new Session_FIFO(__SIM__cons5.GetValues(), 0, 2);
  { 
    Message  __SIM__send552;
    __SIM__head48.ToMessage(__SIM__send552);
    __SIM__cons254->Send(1,__SIM__send552);
  }
  { 
    __SIM__cons254->Delegate(1, *__SIM__copy253);
  }
  __SIM__copy253->Close(false);
  delete __SIM__copy253;
  Session* __SIM__copy255;
    __SIM__copy255=__SIM__cons254->ReceiveSession(1);
  __SIM__cons254->Close(true);
  delete __SIM__cons254;
  { 
    __SIM__dest41->Delegate(0, *__SIM__copy255);
  }
  __SIM__copy255->Close(false);
  delete __SIM__copy255;
  __SIM__dest41->Close(true);
  delete __SIM__dest41;
  return new Cnt();
  }
  else throw (string)"Unknown branch: __SIM__branch542";
}Cnt *__SIM__ListCopyService7(ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__listcopy38,ChannelsValue &__SIM__newlist1)
{
  Session*__SIM__s39=new Session_FIFO(__SIM__listcopy38.GetValues(), 1, 2);
  int __SIM__fork555=fork();
  if (__SIM__fork555>0)
  {
    __SIM__s39->Close(false);
    delete __SIM__s39;
  return new __Cnt____SIM__ListCopyService7(__SIM__cons5, __SIM__listcopy38, __SIM__newlist1);
  }
  else if (__SIM__fork555==0)
  {
  Session* __SIM__lst65;
    __SIM__lst65=__SIM__s39->ReceiveSession(0);
  return new __Cnt____SIM__ListCopy8(__SIM__lst65, __SIM__s39, __SIM__cons5, __SIM__listcopy38, __SIM__newlist1);
  }
else throw (string)"Error during fork!";
return 0;
}Cnt *__SIM__AppendRev11(Session* &__SIM__lhs77,Session* &__SIM__rhs78,Session* &__SIM__dest79,ChannelsValue &__SIM__cons5)
{
  {
    Message  __SIM__select556;
    __SIM__select556.AddData("^Read", 6);
__SIM__lhs77->Send(1,__SIM__select556);
  }
  StringValue __SIM__branch557;
  {
    Message __SIM__branch558;
    __SIM__lhs77->Receive(1, __SIM__branch558);
    __SIM__branch558.GetValue(__SIM__branch557);
  }
  if (__SIM__branch557.ToString()=="^NONE")
  {
  {
    Message  __SIM__select559;
    __SIM__select559.AddData("^Delete", 8);
__SIM__lhs77->Send(1,__SIM__select559);
  }
  { 
    __SIM__dest79->Delegate(0, *__SIM__rhs78);
  }
  __SIM__rhs78->Close(false);
  delete __SIM__rhs78;
  __SIM__dest79->Close(true);
  delete __SIM__dest79;
  return new Cnt();
  __SIM__lhs77->Close(true);
  delete __SIM__lhs77;
  }
  else   if (__SIM__branch557.ToString()=="^SOME")
  {
  IntValue __SIM__elt84;
  { Message __SIM__receive561;
    __SIM__lhs77->Receive(1,__SIM__receive561);
    __SIM__receive561.GetValue(__SIM__elt84);
  }
  {
    Message  __SIM__select562;
    __SIM__select562.AddData("^Pop", 5);
__SIM__lhs77->Send(1,__SIM__select562);
  }
  Session*__SIM__cs85=new Session_FIFO(__SIM__cons5.GetValues(), 0, 2);
  { 
    Message  __SIM__send563;
    __SIM__elt84.ToMessage(__SIM__send563);
    __SIM__cs85->Send(1,__SIM__send563);
  }
  { 
    __SIM__cs85->Delegate(1, *__SIM__rhs78);
  }
  __SIM__rhs78->Close(false);
  delete __SIM__rhs78;
  Session* __SIM__rhs86;
    __SIM__rhs86=__SIM__cs85->ReceiveSession(1);
  __SIM__cs85->Close(true);
  delete __SIM__cs85;
  return new __Cnt____SIM__AppendRev11(__SIM__lhs77, __SIM__rhs86, __SIM__dest79, __SIM__cons5);
  }
  else throw (string)"Unknown branch: __SIM__branch557";
}Cnt *__SIM__Append10(Session* &__SIM__lhs69,Session* &__SIM__rhs70,Session* &__SIM__stack71,Session* &__SIM__dest72,ChannelsValue &__SIM__cons5)
{
  {
    Message  __SIM__select565;
    __SIM__select565.AddData("^Read", 6);
__SIM__lhs69->Send(1,__SIM__select565);
  }
  StringValue __SIM__branch566;
  {
    Message __SIM__branch567;
    __SIM__lhs69->Receive(1, __SIM__branch567);
    __SIM__branch567.GetValue(__SIM__branch566);
  }
  if (__SIM__branch566.ToString()=="^NONE")
  {
  {
    Message  __SIM__select568;
    __SIM__select568.AddData("^Delete", 8);
__SIM__lhs69->Send(1,__SIM__select568);
  }
  return new __Cnt____SIM__AppendRev11(__SIM__stack71, __SIM__rhs70, __SIM__dest72, __SIM__cons5);
  __SIM__lhs69->Close(true);
  delete __SIM__lhs69;
  }
  else   if (__SIM__branch566.ToString()=="^SOME")
  {
  IntValue __SIM__elt89;
  { Message __SIM__receive569;
    __SIM__lhs69->Receive(1,__SIM__receive569);
    __SIM__receive569.GetValue(__SIM__elt89);
  }
  {
    Message  __SIM__select570;
    __SIM__select570.AddData("^Pop", 5);
__SIM__lhs69->Send(1,__SIM__select570);
  }
  Session*__SIM__cs90=new Session_FIFO(__SIM__cons5.GetValues(), 0, 2);
  { 
    Message  __SIM__send571;
    __SIM__elt89.ToMessage(__SIM__send571);
    __SIM__cs90->Send(1,__SIM__send571);
  }
  { 
    __SIM__cs90->Delegate(1, *__SIM__stack71);
  }
  __SIM__stack71->Close(false);
  delete __SIM__stack71;
  Session* __SIM__stack91;
    __SIM__stack91=__SIM__cs90->ReceiveSession(1);
  __SIM__cs90->Close(true);
  delete __SIM__cs90;
  return new __Cnt____SIM__Append10(__SIM__lhs69, __SIM__rhs70, __SIM__stack91, __SIM__dest72, __SIM__cons5);
  }
  else throw (string)"Unknown branch: __SIM__branch566";
}Cnt *__SIM__AppendService9(ChannelsValue &__SIM__append67,ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__newlist1)
{
  Session*__SIM__s68=new Session_FIFO(__SIM__append67.GetValues(), 1, 2);
  int __SIM__fork573=fork();
  if (__SIM__fork573>0)
  {
    __SIM__s68->Close(false);
    delete __SIM__s68;
  return new __Cnt____SIM__AppendService9(__SIM__append67, __SIM__cons5, __SIM__newlist1);
  }
  else if (__SIM__fork573==0)
  {
  Session*__SIM__stack94=new Session_FIFO(__SIM__newlist1.GetValues(), 0, 2);
  Session* __SIM__lhs95;
    __SIM__lhs95=__SIM__s68->ReceiveSession(0);
  Session* __SIM__rhs96;
    __SIM__rhs96=__SIM__s68->ReceiveSession(0);
  return new __Cnt____SIM__Append10(__SIM__lhs95, __SIM__rhs96, __SIM__stack94, __SIM__s68, __SIM__cons5);
  }
else throw (string)"Error during fork!";
return 0;
}Cnt *__SIM__RE013(Session* &__SIM__this101)
{
  StringValue __SIM__branch574;
  {
    Message __SIM__branch575;
    __SIM__this101->Receive(0, __SIM__branch575);
    __SIM__branch575.GetValue(__SIM__branch574);
  }
  if (__SIM__branch574.ToString()=="^Delete")
  {
  return new Cnt();
  }
  else   if (__SIM__branch574.ToString()=="^Match")
  {
  Session* __SIM__str105;
    __SIM__str105=__SIM__this101->ReceiveSession(0);
  {
    Message  __SIM__select576;
    __SIM__select576.AddData("^Delete", 8);
__SIM__str105->Send(1,__SIM__select576);
  }
  { 
    BoolValue __SIM__boolval578(false);
    Message  __SIM__send577;
    __SIM__boolval578.ToMessage(__SIM__send577);
    __SIM__this101->Send(0,__SIM__send577);
  }
  return new __Cnt____SIM__RE013(__SIM__this101);
  __SIM__str105->Close(true);
  delete __SIM__str105;
  }
  else throw (string)"Unknown branch: __SIM__branch574";
}Cnt *__SIM__RE0Service12(ChannelsValue &__SIM__re_099)
{
  Session*__SIM__connection100=new Session_FIFO(__SIM__re_099.GetValues(), 1, 2);
  int __SIM__fork579=fork();
  if (__SIM__fork579>0)
  {
    __SIM__connection100->Close(false);
    delete __SIM__connection100;
  return new __Cnt____SIM__RE0Service12(__SIM__re_099);
  }
  else if (__SIM__fork579==0)
  {
  return new __Cnt____SIM__RE013(__SIM__connection100);
  }
else throw (string)"Error during fork!";
return 0;
}Cnt *__SIM__RE115(Session* &__SIM__this109)
{
  StringValue __SIM__branch580;
  {
    Message __SIM__branch581;
    __SIM__this109->Receive(0, __SIM__branch581);
    __SIM__branch581.GetValue(__SIM__branch580);
  }
  if (__SIM__branch580.ToString()=="^Delete")
  {
  return new Cnt();
  }
  else   if (__SIM__branch580.ToString()=="^Match")
  {
  Session* __SIM__str113;
    __SIM__str113=__SIM__this109->ReceiveSession(0);
  {
    Message  __SIM__select582;
    __SIM__select582.AddData("^Read", 6);
__SIM__str113->Send(1,__SIM__select582);
  }
  StringValue __SIM__branch583;
  {
    Message __SIM__branch584;
    __SIM__str113->Receive(1, __SIM__branch584);
    __SIM__branch584.GetValue(__SIM__branch583);
  }
  if (__SIM__branch583.ToString()=="^NONE")
  {
  {
    Message  __SIM__select585;
    __SIM__select585.AddData("^Delete", 8);
__SIM__str113->Send(1,__SIM__select585);
  }
  { 
    BoolValue __SIM__boolval587(true);
    Message  __SIM__send586;
    __SIM__boolval587.ToMessage(__SIM__send586);
    __SIM__this109->Send(0,__SIM__send586);
  }
  return new __Cnt____SIM__RE115(__SIM__this109);
  __SIM__str113->Close(true);
  delete __SIM__str113;
  }
  else   if (__SIM__branch583.ToString()=="^SOME")
  {
  { 
    BoolValue __SIM__boolval589(false);
    Message  __SIM__send588;
    __SIM__boolval589.ToMessage(__SIM__send588);
    __SIM__this109->Send(0,__SIM__send588);
  }
  IntValue __SIM__lit114;
  { Message __SIM__receive590;
    __SIM__str113->Receive(1,__SIM__receive590);
    __SIM__receive590.GetValue(__SIM__lit114);
  }
  {
    Message  __SIM__select591;
    __SIM__select591.AddData("^Delete", 8);
__SIM__str113->Send(1,__SIM__select591);
  }
  return new __Cnt____SIM__RE115(__SIM__this109);
  __SIM__str113->Close(true);
  delete __SIM__str113;
  }
  else throw (string)"Unknown branch: __SIM__branch583";
  }
  else throw (string)"Unknown branch: __SIM__branch580";
}Cnt *__SIM__RE1Service14(ChannelsValue &__SIM__re_1107)
{
  Session*__SIM__connection108=new Session_FIFO(__SIM__re_1107.GetValues(), 1, 2);
  int __SIM__fork592=fork();
  if (__SIM__fork592>0)
  {
    __SIM__connection108->Close(false);
    delete __SIM__connection108;
  return new __Cnt____SIM__RE1Service14(__SIM__re_1107);
  }
  else if (__SIM__fork592==0)
  {
  return new __Cnt____SIM__RE115(__SIM__connection108);
  }
else throw (string)"Error during fork!";
return 0;
}Cnt *__SIM__RELiteral17(Session* &__SIM__this118,IntValue &__SIM__literal119)
{
  StringValue __SIM__branch593;
  {
    Message __SIM__branch594;
    __SIM__this118->Receive(0, __SIM__branch594);
    __SIM__branch594.GetValue(__SIM__branch593);
  }
  if (__SIM__branch593.ToString()=="^Delete")
  {
  return new Cnt();
  }
  else   if (__SIM__branch593.ToString()=="^Match")
  {
  Session* __SIM__str123;
    __SIM__str123=__SIM__this118->ReceiveSession(0);
  {
    Message  __SIM__select595;
    __SIM__select595.AddData("^Read", 6);
__SIM__str123->Send(1,__SIM__select595);
  }
  StringValue __SIM__branch596;
  {
    Message __SIM__branch597;
    __SIM__str123->Receive(1, __SIM__branch597);
    __SIM__branch597.GetValue(__SIM__branch596);
  }
  if (__SIM__branch596.ToString()=="^NONE")
  {
  {
    Message  __SIM__select598;
    __SIM__select598.AddData("^Delete", 8);
__SIM__str123->Send(1,__SIM__select598);
  }
  { 
    BoolValue __SIM__boolval600(false);
    Message  __SIM__send599;
    __SIM__boolval600.ToMessage(__SIM__send599);
    __SIM__this118->Send(0,__SIM__send599);
  }
  return new __Cnt____SIM__RELiteral17(__SIM__this118, __SIM__literal119);
  __SIM__str123->Close(true);
  delete __SIM__str123;
  }
  else   if (__SIM__branch596.ToString()=="^SOME")
  {
  IntValue __SIM__lit124;
  { Message __SIM__receive601;
    __SIM__str123->Receive(1,__SIM__receive601);
    __SIM__receive601.GetValue(__SIM__lit124);
  }
    BoolValue __SIM__binop602(__SIM__literal119 == __SIM__lit124);
  if (__SIM__binop602.GetValue())
  {
  {
    Message  __SIM__select606;
    __SIM__select606.AddData("^Pop", 5);
__SIM__str123->Send(1,__SIM__select606);
  }
  {
    Message  __SIM__select607;
    __SIM__select607.AddData("^Read", 6);
__SIM__str123->Send(1,__SIM__select607);
  }
  StringValue __SIM__branch608;
  {
    Message __SIM__branch609;
    __SIM__str123->Receive(1, __SIM__branch609);
    __SIM__branch609.GetValue(__SIM__branch608);
  }
  if (__SIM__branch608.ToString()=="^NONE")
  {
  { 
    BoolValue __SIM__boolval611(true);
    Message  __SIM__send610;
    __SIM__boolval611.ToMessage(__SIM__send610);
    __SIM__this118->Send(0,__SIM__send610);
  }
  {
    Message  __SIM__select612;
    __SIM__select612.AddData("^Delete", 8);
__SIM__str123->Send(1,__SIM__select612);
  }
  return new __Cnt____SIM__RELiteral17(__SIM__this118, __SIM__literal119);
  __SIM__str123->Close(true);
  delete __SIM__str123;
  }
  else   if (__SIM__branch608.ToString()=="^SOME")
  {
  { 
    BoolValue __SIM__boolval614(false);
    Message  __SIM__send613;
    __SIM__boolval614.ToMessage(__SIM__send613);
    __SIM__this118->Send(0,__SIM__send613);
  }
  IntValue __SIM__lit125;
  { Message __SIM__receive615;
    __SIM__str123->Receive(1,__SIM__receive615);
    __SIM__receive615.GetValue(__SIM__lit125);
  }
  {
    Message  __SIM__select616;
    __SIM__select616.AddData("^Delete", 8);
__SIM__str123->Send(1,__SIM__select616);
  }
  return new __Cnt____SIM__RELiteral17(__SIM__this118, __SIM__literal119);
  __SIM__str123->Close(true);
  delete __SIM__str123;
  }
  else throw (string)"Unknown branch: __SIM__branch608";
  }
  else
  {
  { 
    BoolValue __SIM__boolval604(false);
    Message  __SIM__send603;
    __SIM__boolval604.ToMessage(__SIM__send603);
    __SIM__this118->Send(0,__SIM__send603);
  }
  {
    Message  __SIM__select605;
    __SIM__select605.AddData("^Delete", 8);
__SIM__str123->Send(1,__SIM__select605);
  }
  return new __Cnt____SIM__RELiteral17(__SIM__this118, __SIM__literal119);
  __SIM__str123->Close(true);
  delete __SIM__str123;
  }
  }
  else throw (string)"Unknown branch: __SIM__branch596";
  }
  else throw (string)"Unknown branch: __SIM__branch593";
}Cnt *__SIM__RELiteralService16(ChannelsValue &__SIM__re_literal116)
{
  Session*__SIM__connection117=new Session_FIFO(__SIM__re_literal116.GetValues(), 1, 2);
  int __SIM__fork617=fork();
  if (__SIM__fork617>0)
  {
    __SIM__connection117->Close(false);
    delete __SIM__connection117;
  return new __Cnt____SIM__RELiteralService16(__SIM__re_literal116);
  }
  else if (__SIM__fork617==0)
  {
  IntValue __SIM__lit127;
  { Message __SIM__receive618;
    __SIM__connection117->Receive(0,__SIM__receive618);
    __SIM__receive618.GetValue(__SIM__lit127);
  }
  return new __Cnt____SIM__RELiteral17(__SIM__connection117, __SIM__lit127);
  }
else throw (string)"Error during fork!";
return 0;
}Cnt *__SIM__MatchSeq20(Session* &__SIM__this142,Session* &__SIM__left143,Session* &__SIM__right144,Session* &__SIM__str1145,Session* &__SIM__str2146,ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__listcopy38,ChannelsValue &__SIM__listrev21,ChannelsValue &__SIM__newlist1)
{
  {
    Message  __SIM__select619;
    __SIM__select619.AddData("^Read", 6);
__SIM__str1145->Send(1,__SIM__select619);
  }
  StringValue __SIM__branch620;
  {
    Message __SIM__branch621;
    __SIM__str1145->Receive(1, __SIM__branch621);
    __SIM__branch621.GetValue(__SIM__branch620);
  }
  if (__SIM__branch620.ToString()=="^NONE")
  {
  Session*__SIM__r1159=new Session_FIFO(__SIM__listrev21.GetValues(), 0, 2);
  { 
    __SIM__r1159->Delegate(1, *__SIM__str2146);
  }
  __SIM__str2146->Close(false);
  delete __SIM__str2146;
  {
    Message  __SIM__select623;
    __SIM__select623.AddData("^Match", 7);
__SIM__left143->Send(1,__SIM__select623);
  }
  { 
    __SIM__left143->Delegate(1, *__SIM__str1145);
  }
  __SIM__str1145->Close(false);
  delete __SIM__str1145;
  BoolValue __SIM__result1160;
  { Message __SIM__receive625;
    __SIM__left143->Receive(1,__SIM__receive625);
    __SIM__receive625.GetValue(__SIM__result1160);
  }
  {
    Message  __SIM__select626;
    __SIM__select626.AddData("^Match", 7);
__SIM__right144->Send(1,__SIM__select626);
  }
  Session* __SIM__str2161;
    __SIM__str2161=__SIM__r1159->ReceiveSession(1);
  __SIM__r1159->Close(true);
  delete __SIM__r1159;
  { 
    __SIM__right144->Delegate(1, *__SIM__str2161);
  }
  __SIM__str2161->Close(false);
  delete __SIM__str2161;
  BoolValue __SIM__result2162;
  { Message __SIM__receive628;
    __SIM__right144->Receive(1,__SIM__receive628);
    __SIM__receive628.GetValue(__SIM__result2162);
  }
  { 
    BoolValue __SIM__binop630(__SIM__result1160 && __SIM__result2162);
    Message  __SIM__send629;
    __SIM__binop630.ToMessage(__SIM__send629);
    __SIM__this142->Send(0,__SIM__send629);
  }
  return new __Cnt____SIM__RESequence19(__SIM__this142, __SIM__left143, __SIM__right144, __SIM__cons5, __SIM__listcopy38, __SIM__listrev21, __SIM__newlist1);
  }
  else   if (__SIM__branch620.ToString()=="^SOME")
  {
  IntValue __SIM__head167;
  { Message __SIM__receive631;
    __SIM__str1145->Receive(1,__SIM__receive631);
    __SIM__receive631.GetValue(__SIM__head167);
  }
  Session*__SIM__cpy1168=new Session_FIFO(__SIM__listcopy38.GetValues(), 0, 2);
  { 
    __SIM__cpy1168->Delegate(1, *__SIM__str1145);
  }
  __SIM__str1145->Close(false);
  delete __SIM__str1145;
  Session* __SIM__str1169;
    __SIM__str1169=__SIM__cpy1168->ReceiveSession(1);
  Session* __SIM__str1cpy170;
    __SIM__str1cpy170=__SIM__cpy1168->ReceiveSession(1);
  __SIM__cpy1168->Close(true);
  delete __SIM__cpy1168;
  Session*__SIM__cpy2171=new Session_FIFO(__SIM__listcopy38.GetValues(), 0, 2);
  { 
    __SIM__cpy2171->Delegate(1, *__SIM__str2146);
  }
  __SIM__str2146->Close(false);
  delete __SIM__str2146;
  Session* __SIM__str2172;
    __SIM__str2172=__SIM__cpy2171->ReceiveSession(1);
  Session* __SIM__str2cpy173;
    __SIM__str2cpy173=__SIM__cpy2171->ReceiveSession(1);
  __SIM__cpy2171->Close(true);
  delete __SIM__cpy2171;
  {
    Message  __SIM__select634;
    __SIM__select634.AddData("^Match", 7);
__SIM__left143->Send(1,__SIM__select634);
  }
  { 
    __SIM__left143->Delegate(1, *__SIM__str1169);
  }
  __SIM__str1169->Close(false);
  delete __SIM__str1169;
  BoolValue __SIM__result1174;
  { Message __SIM__receive636;
    __SIM__left143->Receive(1,__SIM__receive636);
    __SIM__receive636.GetValue(__SIM__result1174);
  }
  Session*__SIM__r1175=new Session_FIFO(__SIM__listrev21.GetValues(), 0, 2);
  { 
    __SIM__r1175->Delegate(1, *__SIM__str2172);
  }
  __SIM__str2172->Close(false);
  delete __SIM__str2172;
  Session* __SIM__str2176;
    __SIM__str2176=__SIM__r1175->ReceiveSession(1);
  __SIM__r1175->Close(true);
  delete __SIM__r1175;
  {
    Message  __SIM__select638;
    __SIM__select638.AddData("^Match", 7);
__SIM__right144->Send(1,__SIM__select638);
  }
  { 
    __SIM__right144->Delegate(1, *__SIM__str2176);
  }
  __SIM__str2176->Close(false);
  delete __SIM__str2176;
  BoolValue __SIM__result2177;
  { Message __SIM__receive640;
    __SIM__right144->Receive(1,__SIM__receive640);
    __SIM__receive640.GetValue(__SIM__result2177);
  }
    BoolValue __SIM__binop641(__SIM__result1174 && __SIM__result2177);
  if (__SIM__binop641.GetValue())
  {
  {
    Message  __SIM__select645;
    __SIM__select645.AddData("^Delete", 8);
__SIM__str1cpy170->Send(1,__SIM__select645);
  }
  {
    Message  __SIM__select646;
    __SIM__select646.AddData("^Delete", 8);
__SIM__str2cpy173->Send(1,__SIM__select646);
  }
  { 
    BoolValue __SIM__boolval648(true);
    Message  __SIM__send647;
    __SIM__boolval648.ToMessage(__SIM__send647);
    __SIM__this142->Send(0,__SIM__send647);
  }
  return new __Cnt____SIM__RESequence19(__SIM__this142, __SIM__left143, __SIM__right144, __SIM__cons5, __SIM__listcopy38, __SIM__listrev21, __SIM__newlist1);
  __SIM__str2cpy173->Close(true);
  delete __SIM__str2cpy173;
  __SIM__str1cpy170->Close(true);
  delete __SIM__str1cpy170;
  }
  else
  {
  {
    Message  __SIM__select642;
    __SIM__select642.AddData("^Pop", 5);
__SIM__str1cpy170->Send(1,__SIM__select642);
  }
  Session*__SIM__cons1178=new Session_FIFO(__SIM__cons5.GetValues(), 0, 2);
  { 
    Message  __SIM__send643;
    __SIM__head167.ToMessage(__SIM__send643);
    __SIM__cons1178->Send(1,__SIM__send643);
  }
  { 
    __SIM__cons1178->Delegate(1, *__SIM__str2cpy173);
  }
  __SIM__str2cpy173->Close(false);
  delete __SIM__str2cpy173;
  Session* __SIM__str2cpy179;
    __SIM__str2cpy179=__SIM__cons1178->ReceiveSession(1);
  __SIM__cons1178->Close(true);
  delete __SIM__cons1178;
  return new __Cnt____SIM__MatchSeq20(__SIM__this142, __SIM__left143, __SIM__right144, __SIM__str1cpy170, __SIM__str2cpy179, __SIM__cons5, __SIM__listcopy38, __SIM__listrev21, __SIM__newlist1);
  }
  }
  else throw (string)"Unknown branch: __SIM__branch620";
}Cnt *__SIM__RESequence19(Session* &__SIM__this130,Session* &__SIM__left131,Session* &__SIM__right132,ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__listcopy38,ChannelsValue &__SIM__listrev21,ChannelsValue &__SIM__newlist1)
{
  StringValue __SIM__branch649;
  {
    Message __SIM__branch650;
    __SIM__this130->Receive(0, __SIM__branch650);
    __SIM__branch650.GetValue(__SIM__branch649);
  }
  if (__SIM__branch649.ToString()=="^Delete")
  {
  {
    Message  __SIM__select651;
    __SIM__select651.AddData("^Delete", 8);
__SIM__left131->Send(1,__SIM__select651);
  }
  {
    Message  __SIM__select652;
    __SIM__select652.AddData("^Delete", 8);
__SIM__right132->Send(1,__SIM__select652);
  }
  return new Cnt();
  __SIM__right132->Close(true);
  delete __SIM__right132;
  __SIM__left131->Close(true);
  delete __SIM__left131;
  }
  else   if (__SIM__branch649.ToString()=="^Match")
  {
  Session* __SIM__str1192;
    __SIM__str1192=__SIM__this130->ReceiveSession(0);
  Session*__SIM__str2193=new Session_FIFO(__SIM__newlist1.GetValues(), 0, 2);
  return new __Cnt____SIM__MatchSeq20(__SIM__this130, __SIM__left131, __SIM__right132, __SIM__str1192, __SIM__str2193, __SIM__cons5, __SIM__listcopy38, __SIM__listrev21, __SIM__newlist1);
  }
  else throw (string)"Unknown branch: __SIM__branch649";
}Cnt *__SIM__RESequenceService18(ChannelsValue &__SIM__cons5,ChannelsValue &__SIM__listcopy38,ChannelsValue &__SIM__listrev21,ChannelsValue &__SIM__newlist1,ChannelsValue &__SIM__re_sequence128)
{
  Session*__SIM__connection129=new Session_FIFO(__SIM__re_sequence128.GetValues(), 1, 2);
  int __SIM__fork653=fork();
  if (__SIM__fork653>0)
  {
    __SIM__connection129->Close(false);
    delete __SIM__connection129;
  return new __Cnt____SIM__RESequenceService18(__SIM__cons5, __SIM__listcopy38, __SIM__listrev21, __SIM__newlist1, __SIM__re_sequence128);
  }
  else if (__SIM__fork653==0)
  {
  Session* __SIM__left195;
    __SIM__left195=__SIM__connection129->ReceiveSession(0);
  Session* __SIM__right196;
    __SIM__right196=__SIM__connection129->ReceiveSession(0);
  return new __Cnt____SIM__RESequence19(__SIM__connection129, __SIM__left195, __SIM__right196, __SIM__cons5, __SIM__listcopy38, __SIM__listrev21, __SIM__newlist1);
  }
else throw (string)"Error during fork!";
return 0;
}Cnt *__SIM__RESum22(Session* &__SIM__this205,Session* &__SIM__left206,Session* &__SIM__right207,ChannelsValue &__SIM__listcopy38)
{
  StringValue __SIM__branch654;
  {
    Message __SIM__branch655;
    __SIM__this205->Receive(0, __SIM__branch655);
    __SIM__branch655.GetValue(__SIM__branch654);
  }
  if (__SIM__branch654.ToString()=="^Delete")
  {
  {
    Message  __SIM__select656;
    __SIM__select656.AddData("^Delete", 8);
__SIM__left206->Send(1,__SIM__select656);
  }
  {
    Message  __SIM__select657;
    __SIM__select657.AddData("^Delete", 8);
__SIM__right207->Send(1,__SIM__select657);
  }
  return new Cnt();
  __SIM__right207->Close(true);
  delete __SIM__right207;
  __SIM__left206->Close(true);
  delete __SIM__left206;
  }
  else   if (__SIM__branch654.ToString()=="^Match")
  {
  Session* __SIM__str1217;
    __SIM__str1217=__SIM__this205->ReceiveSession(0);
  {
    Message  __SIM__select658;
    __SIM__select658.AddData("^Match", 7);
__SIM__left206->Send(1,__SIM__select658);
  }
  {
    Message  __SIM__select659;
    __SIM__select659.AddData("^Match", 7);
__SIM__right207->Send(1,__SIM__select659);
  }
  Session*__SIM__cpy218=new Session_FIFO(__SIM__listcopy38.GetValues(), 0, 2);
  { 
    __SIM__cpy218->Delegate(1, *__SIM__str1217);
  }
  __SIM__str1217->Close(false);
  delete __SIM__str1217;
  Session* __SIM__str2219;
    __SIM__str2219=__SIM__cpy218->ReceiveSession(1);
  { 
    __SIM__left206->Delegate(1, *__SIM__str2219);
  }
  __SIM__str2219->Close(false);
  delete __SIM__str2219;
  Session* __SIM__str3220;
    __SIM__str3220=__SIM__cpy218->ReceiveSession(1);
  __SIM__cpy218->Close(true);
  delete __SIM__cpy218;
  { 
    __SIM__right207->Delegate(1, *__SIM__str3220);
  }
  __SIM__str3220->Close(false);
  delete __SIM__str3220;
  BoolValue __SIM__r1221;
  { Message __SIM__receive663;
    __SIM__left206->Receive(1,__SIM__receive663);
    __SIM__receive663.GetValue(__SIM__r1221);
  }
  BoolValue __SIM__r2222;
  { Message __SIM__receive664;
    __SIM__right207->Receive(1,__SIM__receive664);
    __SIM__receive664.GetValue(__SIM__r2222);
  }
  { 
    BoolValue __SIM__binop666(__SIM__r1221 || __SIM__r2222);
    Message  __SIM__send665;
    __SIM__binop666.ToMessage(__SIM__send665);
    __SIM__this205->Send(0,__SIM__send665);
  }
  return new __Cnt____SIM__RESum22(__SIM__this205, __SIM__left206, __SIM__right207, __SIM__listcopy38);
  }
  else throw (string)"Unknown branch: __SIM__branch654";
}Cnt *__SIM__RESumService21(ChannelsValue &__SIM__listcopy38,ChannelsValue &__SIM__re_sum203)
{
  Session*__SIM__connection204=new Session_FIFO(__SIM__re_sum203.GetValues(), 1, 2);
  int __SIM__fork667=fork();
  if (__SIM__fork667>0)
  {
    __SIM__connection204->Close(false);
    delete __SIM__connection204;
  return new __Cnt____SIM__RESumService21(__SIM__listcopy38, __SIM__re_sum203);
  }
  else if (__SIM__fork667==0)
  {
  Session* __SIM__left229;
    __SIM__left229=__SIM__connection204->ReceiveSession(0);
  Session* __SIM__right230;
    __SIM__right230=__SIM__connection204->ReceiveSession(0);
  return new __Cnt____SIM__RESum22(__SIM__connection204, __SIM__left229, __SIM__right230, __SIM__listcopy38);
  }
else throw (string)"Error during fork!";
return 0;
}Cnt *__SIM__REStar24(Session* &__SIM__this239,Session* &__SIM__body240)
{
  StringValue __SIM__branch668;
  {
    Message __SIM__branch669;
    __SIM__this239->Receive(0, __SIM__branch669);
    __SIM__branch669.GetValue(__SIM__branch668);
  }
  if (__SIM__branch668.ToString()=="^Delete")
  {
  {
    Message  __SIM__select670;
    __SIM__select670.AddData("^Delete", 8);
__SIM__body240->Send(1,__SIM__select670);
  }
  return new Cnt();
  __SIM__body240->Close(true);
  delete __SIM__body240;
  }
  else   if (__SIM__branch668.ToString()=="^Match")
  {
  Session* __SIM__str1247;
    __SIM__str1247=__SIM__this239->ReceiveSession(0);
  {
    Message  __SIM__select671;
    __SIM__select671.AddData("^Delete", 8);
__SIM__str1247->Send(1,__SIM__select671);
  }
  { 
    BoolValue __SIM__boolval673(false);
    Message  __SIM__send672;
    __SIM__boolval673.ToMessage(__SIM__send672);
    __SIM__this239->Send(0,__SIM__send672);
  }
  return new __Cnt____SIM__REStar24(__SIM__this239, __SIM__body240);
  __SIM__str1247->Close(true);
  delete __SIM__str1247;
  }
  else throw (string)"Unknown branch: __SIM__branch668";
}Cnt *__SIM__REStarService23(ChannelsValue &__SIM__re_star237)
{
  Session*__SIM__connection238=new Session_FIFO(__SIM__re_star237.GetValues(), 1, 2);
  int __SIM__fork674=fork();
  if (__SIM__fork674>0)
  {
    __SIM__connection238->Close(false);
    delete __SIM__connection238;
  return new __Cnt____SIM__REStarService23(__SIM__re_star237);
  }
  else if (__SIM__fork674==0)
  {
  Session* __SIM__body249;
    __SIM__body249=__SIM__connection238->ReceiveSession(0);
  return new __Cnt____SIM__REStar24(__SIM__connection238, __SIM__body249);
  }
else throw (string)"Error during fork!";
return 0;
}

/* Main process */
Cnt *__MAIN__()
{
  vector<Channel*> __SIM__chvector453;
  __SIM__chvector453.push_back(new Channel_FIFO());
  ChannelsValue __SIM__newlist1(__SIM__chvector453);
  int __SIM__fork454=fork();
  if (__SIM__fork454>0)
  {
  return new __Cnt____SIM__NewList2(__SIM__newlist1);
  }
  else if (__SIM__fork454==0)
  {
  vector<Channel*> __SIM__chvector455;
  __SIM__chvector455.push_back(new Channel_FIFO());
  ChannelsValue __SIM__cons5(__SIM__chvector455);
  int __SIM__fork456=fork();
  if (__SIM__fork456>0)
  {
  return new __Cnt____SIM__ConsService3(__SIM__cons5);
  }
  else if (__SIM__fork456==0)
  {
  vector<Channel*> __SIM__chvector457;
  __SIM__chvector457.push_back(new Channel_FIFO());
  ChannelsValue __SIM__listrev21(__SIM__chvector457);
  int __SIM__fork458=fork();
  if (__SIM__fork458>0)
  {
  return new __Cnt____SIM__ListRevService5(__SIM__cons5, __SIM__listrev21, __SIM__newlist1);
  }
  else if (__SIM__fork458==0)
  {
  vector<Channel*> __SIM__chvector459;
  __SIM__chvector459.push_back(new Channel_FIFO());
  ChannelsValue __SIM__listcopy38(__SIM__chvector459);
  int __SIM__fork460=fork();
  if (__SIM__fork460>0)
  {
  return new __Cnt____SIM__ListCopyService7(__SIM__cons5, __SIM__listcopy38, __SIM__newlist1);
  }
  else if (__SIM__fork460==0)
  {
  vector<Channel*> __SIM__chvector461;
  __SIM__chvector461.push_back(new Channel_FIFO());
  ChannelsValue __SIM__append67(__SIM__chvector461);
  int __SIM__fork462=fork();
  if (__SIM__fork462>0)
  {
  return new __Cnt____SIM__AppendService9(__SIM__append67, __SIM__cons5, __SIM__newlist1);
  }
  else if (__SIM__fork462==0)
  {
  vector<Channel*> __SIM__chvector463;
  __SIM__chvector463.push_back(new Channel_FIFO());
  ChannelsValue __SIM__re_099(__SIM__chvector463);
  vector<Channel*> __SIM__chvector464;
  __SIM__chvector464.push_back(new Channel_FIFO());
  ChannelsValue __SIM__re_1107(__SIM__chvector464);
  vector<Channel*> __SIM__chvector465;
  __SIM__chvector465.push_back(new Channel_FIFO());
  ChannelsValue __SIM__re_literal116(__SIM__chvector465);
  int __SIM__fork466=fork();
  if (__SIM__fork466>0)
  {
  return new __Cnt____SIM__RELiteralService16(__SIM__re_literal116);
  }
  else if (__SIM__fork466==0)
  {
  vector<Channel*> __SIM__chvector467;
  __SIM__chvector467.push_back(new Channel_FIFO());
  ChannelsValue __SIM__re_sequence128(__SIM__chvector467);
  int __SIM__fork468=fork();
  if (__SIM__fork468>0)
  {
  return new __Cnt____SIM__RESequenceService18(__SIM__cons5, __SIM__listcopy38, __SIM__listrev21, __SIM__newlist1, __SIM__re_sequence128);
  }
  else if (__SIM__fork468==0)
  {
  vector<Channel*> __SIM__chvector469;
  __SIM__chvector469.push_back(new Channel_FIFO());
  ChannelsValue __SIM__re_sum203(__SIM__chvector469);
  int __SIM__fork470=fork();
  if (__SIM__fork470>0)
  {
  return new __Cnt____SIM__RESumService21(__SIM__listcopy38, __SIM__re_sum203);
  }
  else if (__SIM__fork470==0)
  {
  vector<Channel*> __SIM__chvector471;
  __SIM__chvector471.push_back(new Channel_FIFO());
  ChannelsValue __SIM__re_star237(__SIM__chvector471);
  int __SIM__fork472=fork();
  if (__SIM__fork472>0)
  {
  return new __Cnt____SIM__REStarService23(__SIM__re_star237);
  }
  else if (__SIM__fork472==0)
  {
  Session*__SIM__list1253=new Session_FIFO(__SIM__newlist1.GetValues(), 0, 2);
  Session*__SIM__cs254=new Session_FIFO(__SIM__cons5.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval474("1");
    Message  __SIM__send473;
    __SIM__intval474.ToMessage(__SIM__send473);
    __SIM__cs254->Send(1,__SIM__send473);
  }
  { 
    __SIM__cs254->Delegate(1, *__SIM__list1253);
  }
  __SIM__list1253->Close(false);
  delete __SIM__list1253;
  Session* __SIM__list1255;
    __SIM__list1255=__SIM__cs254->ReceiveSession(1);
  __SIM__cs254->Close(true);
  delete __SIM__cs254;
  Session*__SIM__cs256=new Session_FIFO(__SIM__cons5.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval477("2");
    Message  __SIM__send476;
    __SIM__intval477.ToMessage(__SIM__send476);
    __SIM__cs256->Send(1,__SIM__send476);
  }
  { 
    __SIM__cs256->Delegate(1, *__SIM__list1255);
  }
  __SIM__list1255->Close(false);
  delete __SIM__list1255;
  Session* __SIM__list1257;
    __SIM__list1257=__SIM__cs256->ReceiveSession(1);
  __SIM__cs256->Close(true);
  delete __SIM__cs256;
  Session*__SIM__cs258=new Session_FIFO(__SIM__cons5.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval480("2");
    Message  __SIM__send479;
    __SIM__intval480.ToMessage(__SIM__send479);
    __SIM__cs258->Send(1,__SIM__send479);
  }
  { 
    __SIM__cs258->Delegate(1, *__SIM__list1257);
  }
  __SIM__list1257->Close(false);
  delete __SIM__list1257;
  Session* __SIM__list1259;
    __SIM__list1259=__SIM__cs258->ReceiveSession(1);
  __SIM__cs258->Close(true);
  delete __SIM__cs258;
  Session*__SIM__cs260=new Session_FIFO(__SIM__cons5.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval483("1");
    Message  __SIM__send482;
    __SIM__intval483.ToMessage(__SIM__send482);
    __SIM__cs260->Send(1,__SIM__send482);
  }
  { 
    __SIM__cs260->Delegate(1, *__SIM__list1259);
  }
  __SIM__list1259->Close(false);
  delete __SIM__list1259;
  Session* __SIM__list1261;
    __SIM__list1261=__SIM__cs260->ReceiveSession(1);
  __SIM__cs260->Close(true);
  delete __SIM__cs260;
  Session*__SIM__lit_a1262=new Session_FIFO(__SIM__re_literal116.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval486("1");
    Message  __SIM__send485;
    __SIM__intval486.ToMessage(__SIM__send485);
    __SIM__lit_a1262->Send(1,__SIM__send485);
  }
  Session*__SIM__lit_a2263=new Session_FIFO(__SIM__re_literal116.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval488("1");
    Message  __SIM__send487;
    __SIM__intval488.ToMessage(__SIM__send487);
    __SIM__lit_a2263->Send(1,__SIM__send487);
  }
  Session*__SIM__lit_a3264=new Session_FIFO(__SIM__re_literal116.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval490("1");
    Message  __SIM__send489;
    __SIM__intval490.ToMessage(__SIM__send489);
    __SIM__lit_a3264->Send(1,__SIM__send489);
  }
  Session*__SIM__lit_a4265=new Session_FIFO(__SIM__re_literal116.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval492("1");
    Message  __SIM__send491;
    __SIM__intval492.ToMessage(__SIM__send491);
    __SIM__lit_a4265->Send(1,__SIM__send491);
  }
  Session*__SIM__lit_b1266=new Session_FIFO(__SIM__re_literal116.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval494("2");
    Message  __SIM__send493;
    __SIM__intval494.ToMessage(__SIM__send493);
    __SIM__lit_b1266->Send(1,__SIM__send493);
  }
  Session*__SIM__lit_b2267=new Session_FIFO(__SIM__re_literal116.GetValues(), 0, 2);
  { 
    IntValue __SIM__intval496("2");
    Message  __SIM__send495;
    __SIM__intval496.ToMessage(__SIM__send495);
    __SIM__lit_b2267->Send(1,__SIM__send495);
  }
  Session*__SIM__sum_ab1268=new Session_FIFO(__SIM__re_sum203.GetValues(), 0, 2);
  { 
    __SIM__sum_ab1268->Delegate(1, *__SIM__lit_a1262);
  }
  __SIM__lit_a1262->Close(false);
  delete __SIM__lit_a1262;
  { 
    __SIM__sum_ab1268->Delegate(1, *__SIM__lit_b1266);
  }
  __SIM__lit_b1266->Close(false);
  delete __SIM__lit_b1266;
  Session*__SIM__sum_ab2269=new Session_FIFO(__SIM__re_sum203.GetValues(), 0, 2);
  { 
    __SIM__sum_ab2269->Delegate(1, *__SIM__lit_a2263);
  }
  __SIM__lit_a2263->Close(false);
  delete __SIM__lit_a2263;
  { 
    __SIM__sum_ab2269->Delegate(1, *__SIM__lit_b2267);
  }
  __SIM__lit_b2267->Close(false);
  delete __SIM__lit_b2267;
  Session*__SIM__re1270=new Session_FIFO(__SIM__re_sequence128.GetValues(), 0, 2);
  { 
    __SIM__re1270->Delegate(1, *__SIM__lit_a3264);
  }
  __SIM__lit_a3264->Close(false);
  delete __SIM__lit_a3264;
  { 
    __SIM__re1270->Delegate(1, *__SIM__sum_ab1268);
  }
  __SIM__sum_ab1268->Close(false);
  delete __SIM__sum_ab1268;
  Session*__SIM__re2271=new Session_FIFO(__SIM__re_sequence128.GetValues(), 0, 2);
  { 
    __SIM__re2271->Delegate(1, *__SIM__re1270);
  }
  __SIM__re1270->Close(false);
  delete __SIM__re1270;
  { 
    __SIM__re2271->Delegate(1, *__SIM__sum_ab2269);
  }
  __SIM__sum_ab2269->Close(false);
  delete __SIM__sum_ab2269;
  Session*__SIM__re3272=new Session_FIFO(__SIM__re_sequence128.GetValues(), 0, 2);
  { 
    __SIM__re3272->Delegate(1, *__SIM__re2271);
  }
  __SIM__re2271->Close(false);
  delete __SIM__re2271;
  { 
    __SIM__re3272->Delegate(1, *__SIM__lit_a4265);
  }
  __SIM__lit_a4265->Close(false);
  delete __SIM__lit_a4265;
  {
    Message  __SIM__select507;
    __SIM__select507.AddData("^Match", 7);
__SIM__re3272->Send(1,__SIM__select507);
  }
  { 
    __SIM__re3272->Delegate(1, *__SIM__list1261);
  }
  __SIM__list1261->Close(false);
  delete __SIM__list1261;
  BoolValue __SIM__x273;
  { Message __SIM__receive509;
    __SIM__re3272->Receive(1,__SIM__receive509);
    __SIM__receive509.GetValue(__SIM__x273);
  }
  {
    Message  __SIM__select510;
    __SIM__select510.AddData("^Delete", 8);
__SIM__re3272->Send(1,__SIM__select510);
  }
  cout << "XXXXXXXXXXXXX" << endl;
  return new Cnt();
  __SIM__re3272->Close(true);
  delete __SIM__re3272;
  }
else throw (string)"Error during fork!";
return 0;
  }
else throw (string)"Error during fork!";
return 0;
  }
else throw (string)"Error during fork!";
return 0;
  }
else throw (string)"Error during fork!";
return 0;
  }
else throw (string)"Error during fork!";
return 0;
  }
else throw (string)"Error during fork!";
return 0;
  }
else throw (string)"Error during fork!";
return 0;
  }
else throw (string)"Error during fork!";
return 0;
  }
else throw (string)"Error during fork!";
return 0;
}

/*Start process, and its continuations */
int main()
{ try {
    Cnt *cnt = __MAIN__();
    while (!cnt->IsEmpty())
    { Cnt *cnt2=cnt->Run();
      delete cnt;
      cnt=cnt2;
    }
  } catch (const string &error) {
    cerr << "Error: " << error << endl;
    return 1;
  }
  cout << "Done" << endl;
  return 0;
}
