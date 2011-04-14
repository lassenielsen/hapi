  // The Orchestration
  // Player One: 1
  // Player Two: 2
  // Referee: 3
(nu a: rec $x.
       { ^play:
           1=>3:1<Int>;
           2=>3:2<Int>;
           3=>1:3
           { ^onewins:
               3=>2:4 { ^onewins: $x},
             ^twowins:
               3=>2:4 { ^twowins: $x }
           },
         ^stop:
           3=>1:1<Int>;
           3=>2:2<Int>;
           Gend
       }
)
( // Player One
  def X(wins: Int,
        s: rec %x. {^play: 1<<<Int>;3>>{^onewins: %x, ^twowins: %x},
                    ^stop: 1>><Int>; Lend}@(1of3)) =
  guivalue(3,s,1,"Score",wins);
  guisync(3,s,1)
  { ^play(val: Int=0):
      s[1]<<val;
      s[3]>>
      {
        ^onewins:
          X(wins+1,s),
        ^twowins:
          X(wins,s)
      },
    ^stop(x: String=""):
      s[1]>>allwins;
      end
  }
  in
    link(3,a,s,1);
    X(0,s)
| // Player Two
  def X(wins: Int,
        s: rec %x. {^play: 2<<<Int>;4>>{^onewins: %x, ^twowins: %x},
                    ^stop: 2>><Int>; Lend}@(2of3)) =
  guivalue(3,s,2,"Score",wins);
  guisync(3,s,2)
  { ^play(val: Int=0):
      s[2]<<val;
      s[4]>>
      {
        ^onewins:
          X(wins,s),
        ^twowins:
          X(wins+1,s)
      },
    ^stop(x: String=""):
      s[2]>>allwins;
      end
  }
  in
    link(3,a,s,2);
    X(0,s)
| def X(w1: Int, w2: Int, s: rec %x.{^play: 1>><Int>;2>><Int>;3<<{^onewins: 4<<{^onewins: %x}, ^twowins: 4<<{^twowins: %x}}, ^stop: 1<<<Int>;2<<<Int>;Lend}@(3of3)) = 
    guivalue(3,s,3,"P1",w1);
    guivalue(3,s,3,"P2",w2);
    sync(3,s)
    { ^play:
        s[1]>>v1;
        s[2]>>v2;
        if v1=v2
        then s[3]<<^onewins;
             s[4]<<^onewins;
             X(w1+1,w2,s)
        else s[3]<<^twowins;
             s[4]<<^twowins;
             X(w1,w2+1,s),
      ^stop:
        s[1]<<w1;
        s[2]<<w2;
        end
    }
  in
    link(3,a,s,3);
    X(0,0,s)
)
