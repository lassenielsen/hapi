  // The Orchestration
  // Patient: 1
  // Doctor: 2
  // Nurse: 3
(nu a : { #case1: 1=>2<String>;
                  2=>1<String>;
                  2=>1<String>;
                  Gend,
          ^case2: 1=>3<String>;
                  2=>1<String>;
                  2=>1<String>;
                  Gend,
          #case3: 1=>2<String>;
                  3=>1<String>;
                  2=>1<String>;
                  Gend,
          #case4: 1=>3<String>;
                  3=>1<String>;
                  2=>1<String>;
                  Gend })
( // The Patient
  link(3,a,t,1);
  sync(3,t)
  { #case1: t[2]<<"headache";
            t[2]>>time;
            t[2]>>result;
            end,
    ^case2: t[3]<<"headache";
            t[2]>>time;
            t[2]>>result;
            end,
    #case3: t[2]<<"headache";
            t[3]>>time;
            t[2]>>result;
            end,
    #case4: t[3]<<"headache";
            t[3]>>time;
            t[2]>>result;
            end
  }
| // The Doctor
  link(3,a,s,2);
  sync(3,s)
  { #case1: s[1]>>data;
            s[1]<<"now";
            s[1]<<"diagnose: concussion";
            end,
    ^case2: s[1]<<"now";
            s[1]<<"diagnose: concussion";
            end,
    #case3: s[1]>>data;
            s[1]<<"diagnose: concussion";
            end,
    #case4: s[1]<<"diagnose: consussion";
            end
  }
| // The Nurse
  link(3,a,r,3);
  sync(3,r)
  { #case1: end,
    ^case2: r[1]>>data;
            end,
    #case3: r[1]<<"now";
            end,
    #case4: r[1]>>data;
            r[1]<<"now";
            end
  }
)
