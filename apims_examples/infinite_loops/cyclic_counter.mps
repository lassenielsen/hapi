// Setup
// [Gui]-x->[Merge]-z->[Inc]-w->[Split]-q->[Gui]
//             ^___________________/
// Since the Merge process is not well types,
// the GUI delegates x to Split (over q) which gives the setup
// [Merge]-z->[Inc]-w->[Split]-q->[Gui]
//    ^_________x_________/
// Therefor Merge is not realy merge, but just a proxy
(nu x: rec $x. 1=>2:1<Int>;$x)                // The protocols for
(nu z: rec $x. 1=>2:1<Int>;$x)                // x,z and w are just
(nu w: rec $x. 1=>2:1<Int>;$x)                // Integer streams
(nu q: 2=>1:2<rec %x.1<<<Int>;%x@(1of2)>;     // q delegates the x connection
       rec $x. 1=>2:1<Int>;$x)                // before proceding as a stream
( def Merge(s: rec %x.1>><Int>;%x@(2of2),     // The Merge process
            d: rec %x.1<<<Int>;%x@(1of2)) =   // Remember Merge is just a proxy
      s[1]>>n;
      d[1]<<n;
      Merge(s,d)
  in link(2,z,sz,1);
     link(2,x,sx,2);
     Merge(sx,sz) 
| def Inc(s: rec %x.1>><Int>;%x@(2of2),       // The Inc process
          d: rec %x.1<<<Int>;%x@(1of2)) =
      s[1]>>n;
      d[1]<<n+1;
      Inc(s,d)
  in link(2,z,sz,2);
     link(2,w,sw,1);
     Inc(sz,sw)
| def Split(s: rec %x.1>><Int>;%x@(2of2),     // The Split process
            d1: rec %x.1<<<Int>;%x@(1of2),
            d2: rec %x.1<<<Int>;%x@(1of2)) =
      s[1]>>n;
      d1[1]<<n;
      d2[1]<<n;
      Split(s,d1,d2)
  in link(2,q,sq,1);
     link(2,w,sw,2);
     sq[2]>>sy;
     Split(sw,sy,sq)
| def Gui(s: rec %x.1>><Int>;%x@(2of2)) =     // The Gui process
      s[1]>>n;                                // When the Gui receives a value,
      guivalue(2,s,2,"val",n);                // it updates the gui to display
      Gui(s)                                  // the newest value
  in link(2,x,sx,1);
     link(2,q,sq,2);
     sx[1]<<0;
     sq[2]<<sx;
     Gui(sq)
)
