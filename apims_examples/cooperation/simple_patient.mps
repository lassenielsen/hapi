  // The Choreography
  // Patient: 1
  // Doctor: 2
  // Nurse: 3
// DEFINE WORKFLOW {{{
define $consultation_DS =
rec $stateDS.
{^Pdata: 1=>2:2<String>;1=>3:3<String>; rec $stateD.
  {^Pdata: 1=>2:2<String>;1=>3:3<String>;$stateD,
   ^Dschedule: 2=>1:1<String>;2=>3:3<String>;$stateDS,
   ^Nschedule: 3=>1:1<String>;3=>2:2<String>;$stateDS
  },
 ^Dschedule: 2=>1:1<String>;2=>3:3<String>;$stateDS,
 ^Nschedule: 3=>1:1<String>;3=>2:2<String>;$stateDS,
 ^Dresult: 2=>1:1<String>;Gend
}
in
define $consultation_D =
rec $stateD.
{^Pdata: 1=>2:2<String>;1=>3:3<String>;$stateD,
 ^Dschedule: 2=>1:1<String>;2=>3:3<String>;$consultation_DS,
 ^Nschedule: 3=>1:1<String>;3=>2:2<String>;$consultation_DS
}
in
define $consultation =
{^Pdata: 1=>2:2<String>;1=>3:3<String>; $consultation_D }
in // }}}
(nu a: $consultation)
( // Patient
  link(3,a,s,1);
  guisync(3,s,1)
  { ^Pdata(symptoms:String=""):
      guivalue(3,s,1,"symptoms",symptoms);
      s[2]<<symptoms;
      s[3]<<symptoms;
      def StateD(s: $consultation_D@(1of3)) =
        def StateDS(s: $consultation_DS@(1 of 3)) =
          guisync(3,s,1)
          { ^Pdata(symptoms:String=""):
              guivalue(3,s,1,"symptoms",symptoms);
              s[2]<<symptoms;
              s[3]<<symptoms;
              StateD(s),
            ^Nschedule():
              s[1]>>schedule;
              guivalue(3,s,1,"schedule",schedule);
              StateDS(s),
            ^Dschedule():
              s[1]>>schedule;
              guivalue(3,s,1,"schedule",schedule);
              StateDS(s),
            ^Dresult():
              s[1]>>result;
              guivalue(3,s,1,"result",result);
              end
          }
        in guisync(3,s,1)
           { ^Pdata(symptoms:String=""):
               guivalue(3,s,1,"symptoms",symptoms);
               s[2]<<symptoms;
               s[3]<<symptoms;
               StateD(s),
             ^Dschedule():
               s[1]>>schedule;
               guivalue(3,s,1,"schedule",schedule);
               StateDS(s),
             ^Nschedule():
               s[1]>>schedule;
               guivalue(3,s,1,"schedule",schedule);
               StateDS(s)
           }
      in StateD(s)
  }
| // Doctor
  link(3,a,s,2);
  guisync(3,s,2)
  { ^Pdata():
      s[2]>>symptoms;
      guivalue(3,s,2,"symptoms",symptoms);
      def StateD(s: $consultation_D@(2of3)) =
        def StateDS(s: $consultation_DS@(2 of 3)) =
          guisync(3,s,2)
          { ^Pdata():
              s[2]>>symptoms;
              guivalue(3,s,2,"symptoms",symptoms);
              StateD(s),
            ^Nschedule():
              s[2]>>schedule;
              guivalue(3,s,2,"schedule",schedule);
              StateDS(s),
            ^Dschedule(schedule:String=""):
              guivalue(3,s,2,"schedule",schedule);
              s[1]<<schedule;
              s[3]<<schedule;
              StateDS(s),
            ^Dresult(result:String=""):
              guivalue(3,s,2,"result",result);
              s[1]<<result;
              end
          }
        in guisync(3,s,2)
           { ^Pdata():
               s[2]>>symptoms;
               guivalue(3,s,2,"symptoms",symptoms);
               StateD(s),
             ^Dschedule(schedule:String=""):
               guivalue(3,s,2,"schedule",schedule);
               s[1]<<schedule;
               s[3]<<schedule;
               StateDS(s),
             ^Nschedule():
               s[2]>>schedule;
               guivalue(3,s,2,"schedule",schedule);
               StateDS(s)
           }
      in StateD(s)
  }
| // Nurse
  link(3,a,s,3);
  guisync(3,s,3)
  { ^Pdata():
      s[3]>>symptoms;
      guivalue(3,s,3,"symptoms",symptoms);
      def StateD(s: $consultation_D@(3 of 3)) =
        def StateDS(s: $consultation_DS@(3 of 3)) =
          guisync(3,s,3)
          { ^Pdata():
              s[3]>>symptoms;
              guivalue(3,s,3,"symptoms",symptoms);
              StateD(s),
            ^Nschedule(schedule:String=""):
              guivalue(3,s,3,"schedule",schedule);
              s[1]<<schedule;
              s[2]<<schedule;
              StateDS(s),
            ^Dschedule():
              s[3]>>schedule;
              guivalue(3,s,3,"schedule",schedule);
              StateDS(s),
            ^Dresult():
              end
          }
        in guisync(3,s,3)
           { ^Pdata():
               s[3]>>symptoms;
               guivalue(3,s,3,"symptoms",symptoms);
               StateD(s),
             ^Dschedule():
               s[3]>>schedule;
               guivalue(3,s,3,"schedule",schedule);
               StateDS(s),
             ^Nschedule(schedule:String=""):
               guivalue(3,s,3,"schedule",schedule);
               s[1]<<schedule;
               s[2]<<schedule;
               StateDS(s)
           }
      in StateD(s)
  }
)
