// Example, where a doctor prescribes medicine for a patient
// Step 1: Patient contacts the doctor and describes symptoms
// Step 2: Doctor prescribes nothing and the workflow is finished
//         or the doctor sends a prescription to the pharmacy
// Step 3: The pharmacy either denies the prescription and the
//         notifies the doctor why, and theworkflow goes back to Step 2
//         or the pharmacist accepts the prescription
// Step 4: The pharmacy gives the medcin to the patient,
//         and notifies the doctor
define $prescribe2 =
  rec $x.
  {^prescribe:
    2=>1<String>;
    2=>3<String>;
    {^accept:
      {^deliver:
        3=>1<String>;
        3=>2<String>;
        Gend
      },
     ^deny:
      3=>2<String>;
      $x
    },
   ^deny:
     Gend
  }
in
define $prescribe =
{^data:
  1=>2<String>;
  $prescribe2
}
in
(nu a: $prescribe)
( link(3,a,s,1);
  guisync(3,s,1)
  { ^data(sym:String="normal"):
      s[2] << sym;
      def X(s: $prescribe2@(1 of 3))=
        guisync(3,s,1)
        { ^prescribe():
            s[2] >> id;
            guisync(3,s,1)
            { ^accept():
                guisync(3,s,1)
                { ^deliver():
                    s[3] >> rcpt;
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
      s[1] >> sym;
      def X(s: $prescribe2@(2of3))=
        guisync(3,s,2)
        { ^prescribe(id:String=""):
            s[1] << id;
            s[3] << id;
            guisync(3,s,2)
            { ^accept():
                guisync(3,s,2)
                { ^deliver():
                    s[3] >> message;
                    end
                },
              ^deny():
                s[3] >> reason;
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
      defX(s: $prescribe2@(3of3))=
        guisync(3,s,3)
        { ^prescribe():
            s[2] >> id;
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
