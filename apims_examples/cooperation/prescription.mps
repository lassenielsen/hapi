// Example, where a doctor prescribes medicine for a patient
// Step 1: Patient contacts the doctor and describes symptoms
// Step 2: Doctor prescribes nothing and the workflow is finished
//         or the doctor sends a prescription to the pharmacy
// Step 3: The pharmacy either denies the prescription and the
//         notifies the doctor why, and theworkflow goes back to Step 2
//         or the pharmacist accepts the prescription
// Step 4: The pharmacy gives the medcin to the patient,
//         and notifies the doctor
(nu a: { ^data:
           1=>2:2<String>;
           rec $x.
             { ^prescribe:
                 2=>1:1<String>;
                 2=>3:3<String>;
                 { ^accept:
                     { ^deliver:
                         3=>1:1<String>;
                         3=>2:2<String>;
                         Gend
                     },
                   ^deny:
                     3=>2:2<String>;
                     $x
                 },
               ^deny:
                 Gend
             }
       }
)
( link(3,a,s,1);
  guisync(3,s,1)
  { ^data(sym:String="normal"):
      s[2] << sym;
      def X(s:rec %x.{^deny:Lend,^prescribe:1>><String>;
              {^accept:{^deliver:1>><String>;Lend},^deny: %x}}@(1 of 3))=
        guisync(3,s,1)
        { ^prescribe():
            s[1] >> id;
            guisync(3,s,1)
            { ^accept():
                guisync(3,s,1)
                { ^deliver():
                    s[1] >> rcpt;
                    end
                },
              ^deny():
                X(s)
            },
          ^deny():
            end
        }
      in X(s)
  }
| link(3,a,s,2);
  guisync(3,s,2)
  { ^data():
      s[2] >> sym;
      def X(s: rec%x.{^deny:Lend,^prescribe:1<<<String>;3<<<String>;
              {^accept:{^deliver:2>><String>;Lend},^deny:2>><String>;%x}}@(2of3))=
        guisync(3,s,2)
        { ^prescribe(id:String=""):
            s[1] << id;
            s[3] << id;
            guisync(3,s,2)
            { ^accept():
                guisync(3,s,2)
                { ^deliver():
                    s[2] >> message;
                    end
                },
              ^deny():
                s[2] >> reason;
                X(s)
            },
          ^deny(reason:String=""):
            end
        }
      in X(s)
  }
| link(3,a,s,3);
  guisync(3,s,3)
  { ^data():
      defX(s: rec%x.{^deny:Lend,^prescribe:3>><String>;{^accept:{^deliver:
              1<<<String>;2<<<String>;Lend},^deny:2<<<String>;%x}}@(3of3))=
        guisync(3,s,3)
        { ^prescribe():
            s[3] >> id;
            guisync(3,s,3)
            { ^accept(reason:String=""):
                guisync(3,s,3)
                { ^deliver(message: String=""):
                    s[1] << message;
                    s[2] << message;
                    end
                },
              ^deny(reason:String=""):
                s[2] << reason;
                X(s)
            },
          ^deny():
            end
        }
      in X(s)
  }
)
