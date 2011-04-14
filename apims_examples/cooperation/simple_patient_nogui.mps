  // The Orchestration
  // Patient: 1
  // Doctor: 2
  // Nurse: 3
(nu a : { #case1: 1=>2:1<String>;
                  2=>1:2<String>;
                  2=>1:3<String>;
                  Gend,
          ^case2: 1=>3:1<String>;
                  2=>1:2<String>;
                  2=>1:3<String>;
                  Gend,
          #case3: 1=>2:1<String>;
                  3=>1:2<String>;
                  2=>1:3<String>;
                  Gend,
          #case4: 1=>3:1<String>;
                  3=>1:2<String>;
                  2=>1:3<String>;
                  Gend })
( // The Patient
  link(3,a,t,1);
  sync(3,t)
  { #case1: t[1]<<"headache";
            t[2]>>time;
            t[3]>>result;
            end,
    ^case2: t[1]<<"headache";
            t[2]>>time;
            t[3]>>result;
            end,
    #case3: t[1]<<"headache";
            t[2]>>time;
            t[3]>>result;
            end,
    #case4: t[1]<<"headache";
            t[2]>>time;
            t[3]>>result;
            end
  }
| // The Doctor
  link(3,a,s,2);
  sync(3,s)
  { #case1: s[1]>>data;
            s[2]<<"now";
            s[3]<<"diagnose: concussion";
            end,
    ^case2: s[2]<<"now";
            s[3]<<"diagnose: concussion";
            end,
    #case3: s[1]>>data;
            s[3]<<"diagnose: concussion";
            end,
    #case4: s[3]<<"diagnose: consussion";
            end
  }
| // The Nurse
  link(3,a,r,3);
  sync(3,r)
  { #case1: end,
    ^case2: r[1]>>data;
            end,
    #case3: r[2]<<"now";
            end,
    #case4: r[1]>>data;
            r[2]<<"now";
            end
  }
)
