// Setup
// [Gui]-x->[Merge]-z->[Inc]-w->[Split]-q->[Gui]
//             ^___________________/
// Since the Merge process is not well types,
// the GUI delegates x to Split (over q) which gives the setup
// [Merge]-z->[Inc]-w->[Split]-q->[Gui]
//    ^_________x_________/
// Therefor Merge is not realy merge, but just a proxy
define $infstream =
  rec $infstream.1=>2<Int>;$infstream
in
(nu x: $infstream)                // The protocols for
(nu z: $infstream)                // x,z and w are just
(nu w: $infstream)                // Integer streams
(nu q: 2=>1<$infstream@(1of2)>;     // q delegates the x connection
       $infstream)                // before proceding as a stream
( def Merge(s: $infstream@(2of2),     // The Merge process
            d: $infstream@(1of2)) =   // Remember Merge is just a proxy
      s[1]>>n;
      d[2]<<n;
      Merge(s,d)
  in link(2,z,sz,1);
     link(2,x,sx,2);
     Merge(sx,sz) 
| def Inc(s: $infstream@(2of2),       // The Inc process
          d: $infstream@(1of2)) =
      s[1]>>n;
      d[2]<<n+1;
      Inc(s,d)
  in link(2,z,sz,2);
     link(2,w,sw,1);
     Inc(sz,sw)
| def Split(s: $infstream@(2of2),     // The Split process
            d1: $infstream@(1of2),
            d2: $infstream@(1of2)) =
      s[1]>>n;
      d1[2]<<n;
      d2[2]<<n;
      Split(s,d1,d2)
  in link(2,q,sq,1);
     link(2,w,sw,2);
     sq[2]>>sy@(1of2);
     Split(sw,sy,sq)
| def Gui(s: $infstream@(2of2)) =     // The Gui process
      s[1]>>n;                                // When the Gui receives a value,
      guivalue(2,s,2,"val",n);                // it updates the gui to display
      Gui(s)                                  // the newest value
  in link(2,x,sx,1);
     link(2,q,sq,2);
     sx[2]<<0;
     sq[1]<<sx;
     Gui(sq)
)
